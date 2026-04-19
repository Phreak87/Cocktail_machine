#include <Arduino.h>
#include "HX711.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include "selector.html.h"
#include "Cocktails.json.h"
#include "Settings.json.h"

const int RelaisPins[8] = {32, 33, 25, 26, 27, 14, 12, 13};

#define HX_DT  4
#define HX_SCK 16
#define AP_SSID "Cocktail"
#define AP_PASS "12345678"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
HX711 scale;

const char* WIFI_FILE = "/wifi.json";
const char* SETTINGS_FILE = "/settings.json";

String sta_ssid = "";
String sta_pass = "";

struct PumpConfig {
  String drink;
  int rate;
  int prime;
  bool active;
};

struct ScaleConfig {
  long offset;
  float factor;
  int reference;
  long referenceRaw;
};

struct Settings {
  int maxDiff;
  int maxPumpTime;
  int swingTime;
  int weightThreshold;
  ScaleConfig scale;
  PumpConfig pumps[8];
};

Settings globalSettings;

void loadDefaultSettings();
bool saveSettings();
void calibrateScale(int referenceGram);
float getWeightGram();

void setupOTA() {
  ArduinoOTA.setHostname("cocktail-esp32");
  ArduinoOTA.onStart([]() { Serial.println("OTA Start"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nOTA End"); });
  ArduinoOTA.onProgress([](unsigned int p, unsigned int t) { Serial.printf("Progress: %u%%\r", (p * 100) / t); });
  ArduinoOTA.onError([](ota_error_t e) {
    Serial.printf("Error[%u]: ", e);
    if (e == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (e == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (e == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (e == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (e == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void dispensePort(int port, int ms) {
  if (port < 0 || port > 7) return;
  digitalWrite(RelaisPins[port], HIGH);
  delay(ms);
  digitalWrite(RelaisPins[port], LOW);
}

void dispenseML(int port, int ml) {
  if (port < 0 || port > 7) return;
  
  int rate = globalSettings.pumps[port].rate;
  if (rate <= 0) rate = 10;
  
  int ms = (ml * 1000) / rate;
  
  digitalWrite(RelaisPins[port], HIGH);
  delay(ms);
  digitalWrite(RelaisPins[port], LOW);
  
  Serial.printf("Dispensed %d ml (rate: %d ml/s)\n", ml, rate);
}

void primePump(int port) {
  if (port < 0 || port > 7) return;
  int primeTime = globalSettings.pumps[port].prime;
  if (primeTime <= 0) primeTime = 2000;
  digitalWrite(RelaisPins[port], HIGH);
  delay(primeTime);
  digitalWrite(RelaisPins[port], LOW);
}

void drainPump(int port) {
  if (port < 0 || port > 7) return;
  digitalWrite(RelaisPins[port], HIGH);
  delay(500);
  digitalWrite(RelaisPins[port], LOW);
}

void testAllPins() {
  for (int i = 0; i < 8; i++) {
    dispensePort(i, 200);
  }
}

bool connectToWiFi() {
  if (sta_ssid.isEmpty()) {
    Serial.println("No WiFi credentials stored.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());

  Serial.print("Connecting to WiFi: ");
  Serial.println(sta_ssid);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    return true;
  } else {
    Serial.println("\nWiFi connection failed!");
    return false;
  }
}

void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println("AP mode started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

bool loadWiFiConfig() {
  if (!LittleFS.exists(WIFI_FILE)) return false;
  File file = LittleFS.open(WIFI_FILE, "r");
  if (!file) return false;

  JsonDocument doc;
  if (deserializeJson(doc, file)) {
    file.close();
    return false;
  }

  sta_ssid = doc["ssid"].as<String>();
  sta_pass = doc["pass"].as<String>();
  file.close();
  return true;
}

void saveWiFiConfig(String ssid, String pass) {
  JsonDocument doc;
  doc["ssid"] = ssid;
  doc["pass"] = pass;

  File file = LittleFS.open(WIFI_FILE, "w");
  serializeJson(doc, file);
  file.close();
}

bool loadSettings() {
  if (!LittleFS.exists(SETTINGS_FILE)) {
    loadDefaultSettings();
    return false;
  }
  
  File file = LittleFS.open(SETTINGS_FILE, "r");
  if (!file) {
    loadDefaultSettings();
    return false;
  }

  JsonDocument doc;
  if (deserializeJson(doc, file)) {
    file.close();
    loadDefaultSettings();
    return false;
  }

  globalSettings.maxDiff = doc["limits"]["maxDiff"] | 5;
  globalSettings.maxPumpTime = doc["limits"]["maxPumpTime"] | 10000;
  globalSettings.swingTime = doc["limits"]["swingTime"] | 100;
  globalSettings.weightThreshold = doc["limits"]["weightThreshold"] | 10;

  globalSettings.scale.offset = doc["scale"]["offset"] | 0;
  globalSettings.scale.factor = doc["scale"]["factor"] | 1.0;
  globalSettings.scale.reference = doc["scale"]["reference"] | 1000;
  globalSettings.scale.referenceRaw = doc["scale"]["referenceRaw"] | 0;

  for (int i = 0; i < 8; i++) {
    globalSettings.pumps[i].drink = doc["pumps"][i]["drink"].as<String>();
    globalSettings.pumps[i].rate = doc["pumps"][i]["rate"] | 10;
    globalSettings.pumps[i].prime = doc["pumps"][i]["prime"] | 2000;
    globalSettings.pumps[i].active = false;
  }
  
  file.close();
  return true;
}

void loadDefaultSettings() {
  globalSettings.maxDiff = 5;
  globalSettings.maxPumpTime = 10000;
  globalSettings.swingTime = 100;
  globalSettings.weightThreshold = 10;
  
  globalSettings.scale.offset = 0;
  globalSettings.scale.factor = 1.0;
  globalSettings.scale.reference = 1000;
  globalSettings.scale.referenceRaw = 0;
  
  for (int i = 0; i < 8; i++) {
    globalSettings.pumps[i].drink = "";
    globalSettings.pumps[i].rate = 10;
    globalSettings.pumps[i].prime = 2000;
    globalSettings.pumps[i].active = false;
  }
}

bool saveSettings() {
  JsonDocument doc;
  JsonObject limits = doc["limits"].to<JsonObject>();
  limits["maxDiff"] = globalSettings.maxDiff;
  limits["maxPumpTime"] = globalSettings.maxPumpTime;
  limits["swingTime"] = globalSettings.swingTime;
  limits["weightThreshold"] = globalSettings.weightThreshold;

  JsonObject sc = doc["scale"].to<JsonObject>();
  sc["offset"] = globalSettings.scale.offset;
  sc["factor"] = globalSettings.scale.factor;
  sc["reference"] = globalSettings.scale.reference;
  sc["referenceRaw"] = globalSettings.scale.referenceRaw;

  JsonArray pumps = doc["pumps"].to<JsonArray>();
  for (int i = 0; i < 8; i++) {
    JsonObject p = pumps.add<JsonObject>();
    p["drink"] = globalSettings.pumps[i].drink;
    p["rate"] = globalSettings.pumps[i].rate;
    p["prime"] = globalSettings.pumps[i].prime;
  }

  File file = LittleFS.open(SETTINGS_FILE, "w");
  if (!file) return false;
  
  serializeJson(doc, file);
  file.close();
  return true;
}

void calibrateScale(int referenceGram) {
  long reading = scale.read_average(10);
  scale.set_offset(reading);
  // factor = weight / (reading - offset), so factor = weight / (reading - reading) which is undefined
  // Use calibrate_scale(weight) from HX711 which does: scale = weight / (read - offset)
  float scaleFactor = (float)referenceGram / (scale.read_average(10) - reading);
  scale.set_scale(scaleFactor);
  globalSettings.scale.offset = reading;
  globalSettings.scale.factor = scaleFactor;
  globalSettings.scale.reference = referenceGram;
  saveSettings();
  Serial.printf("Scale calibrated: %dg, offset: %d, factor: %.6f\n", 
    referenceGram, globalSettings.scale.offset, globalSettings.scale.factor);
}

float getWeightGram() {
  float val = scale.get_units(5);
  return val > 0 ? val : 0;
}

int findPortByDrink(String drinkName) {
  for (int i = 0; i < 8; i++) {
    if (globalSettings.pumps[i].drink == drinkName) {
      return i;
    }
  }
  return -1;
}

String getWiFiStatusJson() {
  JsonDocument doc;
  doc["wifi"]["mode"] = WiFi.getMode() == WIFI_STA ? "STA" : "AP";
  doc["wifi"]["connected"] = WiFi.status() == WL_CONNECTED;
  if (WiFi.status() == WL_CONNECTED) {
    doc["wifi"]["ssid"] = WiFi.SSID();
    doc["wifi"]["ip"] = WiFi.localIP().toString();
    doc["wifi"]["rssi"] = WiFi.RSSI();
  }

  String json;
  serializeJson(doc, json);
  return json;
}

String getScaleJson() {
  JsonDocument doc;
  doc["weight"] = getWeightGram() / 1000.0;
  doc["weightG"] = getWeightGram();

  String json;
  serializeJson(doc, json);
  return json;
}

String getSettingsJson() {
  JsonDocument doc;
  JsonObject limits = doc["limits"].to<JsonObject>();
  limits["maxDiff"] = globalSettings.maxDiff;
  limits["maxPumpTime"] = globalSettings.maxPumpTime;
  limits["swingTime"] = globalSettings.swingTime;
  limits["weightThreshold"] = globalSettings.weightThreshold;

  JsonObject sc = doc["scale"].to<JsonObject>();
  sc["offset"] = globalSettings.scale.offset;
  sc["factor"] = globalSettings.scale.factor;
  sc["reference"] = globalSettings.scale.reference;
  sc["referenceRaw"] = globalSettings.scale.referenceRaw;

  JsonArray pumps = doc["pumps"].to<JsonArray>();
  for (int i = 0; i < 8; i++) {
    JsonObject p = pumps.add<JsonObject>();
    p["drink"] = globalSettings.pumps[i].drink;
    p["rate"] = globalSettings.pumps[i].rate;
    p["prime"] = globalSettings.pumps[i].prime;
    p["active"] = globalSettings.pumps[i].active;
  }

  String json;
  serializeJson(doc, json);
  return json;
}

bool isAnyPumpActive() {
  for (int i = 0; i < 8; i++) {
    if (globalSettings.pumps[i].active) return true;
  }
  return false;
}

void sendBroadcastStatus() {
  JsonDocument doc;
  doc["weight"] = getWeightGram() / 1000.0;
  
  if (!isAnyPumpActive()) {
    doc["wifi"]["mode"] = WiFi.getMode() == WIFI_STA ? "STA" : "AP";
    doc["wifi"]["connected"] = WiFi.status() == WL_CONNECTED;
    if (WiFi.status() == WL_CONNECTED) {
      doc["wifi"]["ssid"] = WiFi.SSID();
      doc["wifi"]["rssi"] = WiFi.RSSI();
    }

    JsonArray pumps = doc["pumps"].to<JsonArray>();
    for (int i = 0; i < 8; i++) {
      JsonObject p = pumps.add<JsonObject>();
      p["active"] = globalSettings.pumps[i].active;
    }
  }

  String json;
  serializeJson(doc, json);
  ws.textAll(json);
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WS Client connected: %u\n", client->id());
    String status = getWiFiStatusJson();
    client->text(status.c_str());
    
    String scaleJson = getScaleJson();
    client->text(scaleJson.c_str());
    
    String settingsJson = getSettingsJson();
    client->text(settingsJson.c_str());
  }

  if (type == WS_EVT_DATA) {
    String msg;
    for (size_t i = 0; i < len; i++) msg += (char)data[i];

    Serial.println("WS Message: " + msg);

    JsonDocument doc;
    deserializeJson(doc, msg);
    String msgType = doc["type"].as<String>();

    if (msgType == "status") {
      if (isAnyPumpActive()) return;
      
      String status = getWiFiStatusJson();
      client->text(status.c_str());
      String settingsJson = getSettingsJson();
      client->text(settingsJson.c_str());
    }
    else if (msgType == "dispense") {
      int port = doc["port"];
      if (doc.containsKey("ml")) {
        int ml = doc["ml"];
        Serial.printf("Dispense %d ml from port %d\n", ml, port);
        globalSettings.pumps[port].active = true;
        dispenseML(port, ml);
        globalSettings.pumps[port].active = false;
      } else if (doc.containsKey("ms")) {
        int ms = doc["ms"];
        Serial.printf("Dispense %d ms from port %d\n", ms, port);
        globalSettings.pumps[port].active = true;
        dispensePort(port, ms);
        globalSettings.pumps[port].active = false;
      }
    }
    else if (msgType == "prime") {
      int port = doc["port"];
      Serial.printf("Prime pump %d\n", port);
      globalSettings.pumps[port].active = true;
      primePump(port);
      globalSettings.pumps[port].active = false;
    }
    else if (msgType == "drain") {
      int port = doc["port"];
      Serial.printf("Drain pump %d\n", port);
      globalSettings.pumps[port].active = true;
      drainPump(port);
      globalSettings.pumps[port].active = false;
    }
    else if (msgType == "start") {
      int port = doc["port"];
      Serial.printf("Start pump %d\n", port);
      digitalWrite(RelaisPins[port], HIGH);
      globalSettings.pumps[port].active = true;
    }
    else if (msgType == "stop") {
      int port = doc["port"];
      Serial.printf("Stop pump %d\n", port);
      digitalWrite(RelaisPins[port], LOW);
      globalSettings.pumps[port].active = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
  }
  
  loadSettings();

  scale.begin(HX_DT, HX_SCK);
  if (globalSettings.scale.offset != 0) {
    scale.set_offset(globalSettings.scale.offset);
  }
  if (globalSettings.scale.factor != 0) {
    scale.set_scale(globalSettings.scale.factor);
  } else {
    scale.set_scale(1.0);
  }

  for (int i = 0; i < 8; i++) {
    pinMode(RelaisPins[i], OUTPUT);
    digitalWrite(RelaisPins[i], LOW);
  }
  testAllPins();

  loadWiFiConfig();

  if (!connectToWiFi()) {
    startAP();
  } else {
    setupOTA();
  }

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", selector_html);
  });

  server.on("/cocktails", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", cocktails_json);
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getSettingsJson());
  });

  server.on("/settings", HTTP_POST, 
    [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      deserializeJson(doc, data);
      
      globalSettings.maxDiff = doc["limits"]["maxDiff"] | globalSettings.maxDiff;
      globalSettings.maxPumpTime = doc["limits"]["maxPumpTime"] | globalSettings.maxPumpTime;
      globalSettings.swingTime = doc["limits"]["swingTime"] | globalSettings.swingTime;
      globalSettings.weightThreshold = doc["limits"]["weightThreshold"] | globalSettings.weightThreshold;
      
      if (doc["scale"].containsKey("referenceRaw")) {
        globalSettings.scale.referenceRaw = doc["scale"]["referenceRaw"];
        globalSettings.scale.reference = doc["scale"]["reference"] | 1000;
        globalSettings.scale.factor = (float)globalSettings.scale.reference / globalSettings.scale.referenceRaw;
      } else if (doc["scale"].containsKey("factor")) {
        globalSettings.scale.factor = doc["scale"]["factor"];
      }
      
      for (int i = 0; i < 8; i++) {
        if (!doc["pumps"][i].isNull()) {
          globalSettings.pumps[i].drink = doc["pumps"][i]["drink"].as<String>();
          globalSettings.pumps[i].rate = doc["pumps"][i]["rate"] | globalSettings.pumps[i].rate;
          globalSettings.pumps[i].prime = doc["pumps"][i]["prime"] | globalSettings.pumps[i].prime;
        }
      }
      
      saveSettings();
      request->send(200, "application/json", "{\"status\":\"saved\"}");
    });

  server.on("/calibrate", HTTP_POST,
    [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      deserializeJson(doc, data);
      int reference = doc["reference"] | 1000;
      calibrateScale(reference);
      JsonDocument resp;
      resp["status"] = "calibrated";
      resp["reference"] = reference;
      resp["factor"] = globalSettings.scale.factor;
      String json;
      serializeJson(resp, json);
      request->send(200, "application/json", json);
    });

  server.on("/tare", HTTP_POST, [](AsyncWebServerRequest *request) {
    scale.tare(10);
    globalSettings.scale.offset = scale.get_offset();
    saveSettings();
    JsonDocument resp;
    resp["status"] = "tared";
    resp["offset"] = globalSettings.scale.offset;
    String json;
    serializeJson(resp, json);
    request->send(200, "application/json", json);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getWiFiStatusJson());
  });

  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (WiFi.getMode() == WIFI_AP) {
      request->send(400, "application/json", "{\"error\":\"Nur im STA-Modus verfuegbar\"}");
      return;
    }
    
    int n = WiFi.scanNetworks();
    if (n < 0) {
      request->send(500, "application/json", "{\"error\":\"Scan fehlgeschlagen\"}");
      return;
    }
    
    JsonDocument doc;
    JsonArray arr = doc["networks"].to<JsonArray>();
    
    for (int i = 0; i < n; i++) {
      JsonObject net = arr.add<JsonObject>();
      net["ssid"] = WiFi.SSID(i);
      net["rssi"] = WiFi.RSSI(i);
      net["secure"] = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;
    }
    
    WiFi.scanDelete();
    
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
  });

  server.on("/setup", HTTP_POST,
    [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      deserializeJson(doc, data);
      String ssid = doc["ssid"].as<String>();
      String pass = doc["pass"].as<String>();
      saveWiFiConfig(ssid, pass);
      request->send(200, "application/json", "{\"status\":\"saved\"}");
      delay(1000);
      ESP.restart();
    });

  server.on("/dispense", HTTP_POST,
    [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      deserializeJson(doc, data);
      int port = doc["port"];
      int amount = doc["amount"];
      dispensePort(port, amount);
      request->send(200, "application/json", "{\"status\":\"dispensed\"}");
    });

  server.on("/pump", HTTP_POST,
    [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      JsonDocument doc;
      deserializeJson(doc, data);
      int port = doc["port"];
      String action = doc["action"].as<String>();
      
      if (action == "prime") {
        globalSettings.pumps[port].active = true;
        primePump(port);
        globalSettings.pumps[port].active = false;
      } else if (action == "drain") {
        globalSettings.pumps[port].active = true;
        drainPump(port);
        globalSettings.pumps[port].active = false;
      } else if (action == "dispense") {
        int ml = doc["ml"];
        globalSettings.pumps[port].active = true;
        dispenseML(port, ml);
        globalSettings.pumps[port].active = false;
      }
      
      request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

  server.begin();
  
  static bool scanned = false;
  if (!scanned && WiFi.status() == WL_CONNECTED) {
    scanned = true;
    int n = WiFi.scanNetworks(false);
    Serial.printf("Pre-scanned %d networks\n", n);
  }
  
  Serial.println("Server started");
}

void loop() {
  ws.cleanupClients();
  ArduinoOTA.handle();
  
  static unsigned long lastBroadcast = 0;
  if (millis() - lastBroadcast > 1000) {
    sendBroadcastStatus();
    lastBroadcast = millis();
  }
}