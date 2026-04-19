const char settings_json[] PROGMEM = R"rawliteral(
{
  "limits": {
    "maxDiff": 5,
    "maxPumpTime": 10000,
    "swingTime": 100,
    "weightThreshold": 10
  },
  "scale": {
    "offset": 0,
    "factor": 1.0,
    "reference": 1000,
    "referenceRaw": 0
  },
  "pumpPulseDuration": 500,
  "pumpThreshold": 10,
  "pumps": [
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 },
    { "drink": "", "rate": 10 }
  ]
}
)rawliteral";