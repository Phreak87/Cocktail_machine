const char cocktails_json[] PROGMEM = R"rawliteral(
[
  {
    "id": 1,
    "name": "Mojito",
    "type": "alcoholic",
    "baseMl": 250,
    "image": "https://images.unsplash.com/photo-1551538827-9c037cb4f32a?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Rum weiß", "ml": 45 },
      { "name": "Limette", "ml": 15 },
      { "name": "Minze", "ml": 5 },
      { "name": "Soda", "ml": 100 },
      { "name": "Zucker", "ml": 10 }
    ],
    "steps": [
      { "text": "Rum mit Limette und Zucker mixen", "drink": "Rum weiß", "ml": 45, "manual": false },
      { "text": "Minze hinzufügen", "drink": "Minze", "ml": 5, "manual": true },
      { "text": "Mit Soda auffüllen", "drink": "Soda", "ml": 100, "manual": false }
    ]
  },
  {
    "id": 2,
    "name": "Margarita",
    "type": "alcoholic",
    "baseMl": 200,
    "image": "https://images.unsplash.com/photo-1556679343-c7306c0d27bd?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Tequila", "ml": 50 },
      { "name": "Triple Sec", "ml": 25 },
      { "name": "Limette", "ml": 15 }
    ],
    "steps": [
      { "text": "Tequila und Triple Sec mixen", "drink": "Tequila", "ml": 50, "manual": false },
      { "text": "Limette hinzufügen", "drink": "Limette", "ml": 15, "manual": false },
      { "text": "Mit Soda auffüllen", "drink": "Soda", "ml": 50, "manual": false }
    ]
  },
  {
    "id": 3,
    "name": "Long Island Iced Tea",
    "type": "alcoholic",
    "baseMl": 400,
    "image": "https://images.unsplash.com/photo-1551751299-1b51cab2694c?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Gin", "ml": 15 },
      { "name": "Rum weiß", "ml": 15 },
      { "name": "Vodka", "ml": 15 },
      { "name": "Triple Sec", "ml": 15 },
      { "name": "Cola", "ml": 100 }
    ],
    "steps": [
      { "text": "Gin einschenken", "drink": "Gin", "ml": 15, "manual": false },
      { "text": "Rum hinzufügen", "drink": "Rum weiß", "ml": 15, "manual": false },
      { "text": "Vodka hinzufügen", "drink": "Vodka", "ml": 15, "manual": false },
      { "text": "Triple Sec hinzufügen", "drink": "Triple Sec", "ml": 15, "manual": false },
      { "text": "Mit Cola auffüllen", "drink": "Cola", "ml": 100, "manual": false }
    ]
  },
  {
    "id": 4,
    "name": "Virgin Mojito",
    "type": "non-alcoholic",
    "baseMl": 300,
    "image": "https://images.unsplash.com/photo-1582816379308-1e51a5f7bc8d?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Limette", "ml": 30 },
      { "name": "Minze", "ml": 10 },
      { "name": "Soda", "ml": 150 },
      { "name": "Zucker", "ml": 10 }
    ],
    "steps": [
      { "text": "Limette und Zucker mixen", "drink": "Limette", "ml": 30, "manual": false },
      { "text": "Minze hinzufügen", "drink": "Minze", "ml": 10, "manual": true },
      { "text": "Mit Soda auffüllen", "drink": "Soda", "ml": 150, "manual": false }
    ]
  },
  {
    "id": 5,
    "name": "Orange Mojito",
    "type": "non-alcoholic",
    "baseMl": 250,
    "image": "https://images.unsplash.com/photo-1582671988569-e3e6d2e2d8e9?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Orangensaft", "ml": 45 },
      { "name": "Minze", "ml": 5 },
      { "name": "Soda", "ml": 100 },
      { "name": "Zucker", "ml": 10 }
    ],
    "steps": [
      { "text": "Orangensaft und Zucker mixen", "drink": "Orangensaft", "ml": 45, "manual": false },
      { "text": "Mit Soda auffüllen", "drink": "Soda", "ml": 100, "manual": false }
    ]
  },
  {
    "id": 6,
    "name": "Cranberry Cooler",
    "type": "non-alcoholic",
    "baseMl": 350,
    "image": "https://images.unsplash.com/photo-1563227812-9ea7e58a3471?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Cranberrysaft", "ml": 100 },
      { "name": "Soda", "ml": 150 },
      { "name": "Zucker", "ml": 10 }
    ],
    "steps": [
      { "text": "Cranberrysaft mit Zucker mischen", "drink": "Cranberrysaft", "ml": 100, "manual": false },
      { "text": "Mit Soda auffüllen", "drink": "Soda", "ml": 150, "manual": false }
    ]
  },
  {
    "id": 7,
    "name": "Pina Colada",
    "type": "alcoholic",
    "baseMl": 300,
    "image": "https://images.unsplash.com/photo-1574052953476-f0bb4ddb5420?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Rum weiß", "ml": 30 },
      { "name": "Ananassaft", "ml": 60 },
      { "name": "Kokosmilch", "ml": 40 },
      { "name": "Limette", "ml": 15 }
    ],
    "steps": [
      { "text": "Rum und Ananassaft mixen", "drink": "Rum weiß", "ml": 30, "manual": false },
      { "text": "Kokosmilch hinzufügen", "drink": "Kokosmilch", "ml": 40, "manual": false },
      { "text": "Limette einpressen", "drink": "Limette", "ml": 15, "manual": false }
    ]
  },
  {
    "id": 8,
    "name": "Ginger Fizz",
    "type": "non-alcoholic",
    "baseMl": 300,
    "image": "https://images.unsplash.com/photo-1513558161293-c296afd8e7a1?w=400&h=300&fit=crop",
    "ingredients": [
      { "name": "Ingwersirup", "ml": 20 },
      { "name": "Zitronensaft", "ml": 15 },
      { "name": "Soda", "ml": 150 },
      { "name": "Zucker", "ml": 10 }
    ],
    "steps": [
      { "text": "Ingwer und Zitrone mischen", "drink": "Ingwersirup", "ml": 20, "manual": false },
      { "text": "Mit Soda auffüllen", "drink": "Soda", "ml": 150, "manual": false }
    ]
  }
])rawliteral";