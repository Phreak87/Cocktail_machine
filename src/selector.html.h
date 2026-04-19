const char selector_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cocktailmaschine - ESP32</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <style>
        :root {
            --primary: #2c3e50;
            --secondary: #3498db;
            --success: #27ae60;
            --warning: #f39c12;
            --danger: #e74c3c;
            --light: #ecf0f1;
            --dark: #34495e;
            --gray: #95a5a6;
        }
        
        * { margin: 0; padding: 0; box-sizing: border-box; }
        
html, body {
            margin: 0;
            padding: 0;
            height: 100%;
            overflow: hidden;
        }
        
        body {
            background: linear-gradient(135deg, #1a2a6c, #b21f1f, #1a2a6c);
            background-attachment: fixed;
            color: #333;
            min-height: 100vh;
            padding: 10px;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }
        
        .container { 
            max-width: 1400px; 
            margin: 0 auto; 
            display: flex;
            flex-direction: column;
            height: 100vh;
            overflow: hidden;
        }
        
        header {
            text-align: center;
            padding: 15px;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            margin-bottom: 15px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            display: flex;
            flex-direction: column;
            gap: 10px;
            flex-shrink: 0;
        }
        
        .header-left {
            display: flex;
            flex-wrap: wrap;
            align-items: center;
            gap: 10px;
            flex: 1;
        }
        
        .header-top {
            display: flex;
            justify-content: space-between;
            align-items: center;
            flex-wrap: wrap;
            gap: 15px;
            width: 100%;
            border-bottom: 3px solid var(--secondary);
            padding-bottom: 15px;
        }
        
        .header-left h1 {
            color: var(--primary);
            font-size: 1.8rem;
            display: flex;
            align-items: center;
            gap: 10px;
            flex-wrap: wrap;
            border: none;
        }
        
        .panel {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            padding: 20px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            display: flex;
            flex-direction: column;
            overflow: hidden;
            box-sizing: border-box;
            max-height: calc(100vh - 160px);
        }
        
        .panel h2 {
            color: var(--primary);
            margin-bottom: 15px;
            padding-bottom: 10px;
            border-bottom: 2px solid var(--secondary);
            display: flex;
            align-items: center;
            gap: 10px;
        }
        
        .filter-section {
            display: flex;
            gap: 10px;
            justify-content: center;
            flex-wrap: wrap;
        }
        
        .filter-inline {
            margin-left: 20px;
            display: inline-flex;
            gap: 5px;
        }
        
        .filter-inline .filter-btn {
            padding: 4px 10px;
            font-size: 0.75rem;
            border-radius: 5px;
        }
        
        .filter-btn {
            padding: 10px 20px;
            border: none;
            border-radius: 50px;
            background: var(--light);
            color: var(--dark);
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
        .filter-btn:hover { transform: translateY(-2px); box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2); }
        .filter-btn.active { background: var(--primary); color: white; }
        
        .cocktail-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(180px, 1fr));
            gap: 15px;
            flex: 1;
            overflow-y: auto;
            padding: 5px;
        }
        
        .header-pump-bar {
            display: flex;
            justify-content: flex-end;
            flex-wrap: wrap;
            gap: 5px;
        }
        
        .header-left {
            display: flex;
            flex-wrap: wrap;
            align-items: center;
            gap: 10px;
        }
        
        .header-right {
            display: flex;
            gap: 10px;
            align-items: center;
        }
        
        .status-display {
            background: var(--primary);
            color: white;
            padding: 8px 15px;
            border-radius: 8px;
            font-weight: bold;
            display: flex;
            align-items: center;
            gap: 8px;
            white-space: nowrap;
        }
        
.pump-bubble {
            display: inline-flex;
            align-items: center;
            gap: 3px;
            padding: 4px 8px;
            border-radius: 12px;
            font-size: 0.7rem;
            background: #e0e0e0;
            color: #333;
            margin: 2px;
            cursor: pointer;
            user-select: none;
            -webkit-user-select: none;
            -webkit-touch-callout: none;
            border: 1px solid #999;
        }
        
        .pump-bubble i {
            font-size: 0.6rem;
            opacity: 0.6;
        }
        
        .pump-bubble.active {
            background: var(--success);
            color: #fff;
            border-color: var(--success);
        }
        
        .pump-bubble .dot {
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background: #666;
        }
        
        .pump-bubble.active .dot {
            background: var(--success);
            animation: pulse-dot 1s infinite;
        }
        
        @keyframes pulse-dot {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .btn {
            padding: 10px 18px;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
        .btn-primary { background: var(--primary); color: white; }
        .btn-success { background: var(--success); color: white; }
        .btn-warning { background: var(--warning); color: white; }
        .btn-danger { background: var(--danger); color: white; }
        .btn:hover { transform: translateY(-2px); box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2); }
        
        .status-bar {
            display: flex;
            justify-content: space-between;
            background: rgba(44, 62, 80, 0.85);
            color: white;
            padding: 15px;
            border-radius: 10px;
            margin-bottom: 20px;
            flex-wrap: wrap;
            gap: 15px;
        }
        
#pump-status-bar {
            display: flex;
            flex-wrap: wrap;
            gap: 5px;
        }
        
        .pump-bar-row {
            display: flex;
            flex-wrap: wrap;
            gap: 5px;
            margin-bottom: 20px;
            padding: 10px;
            border-radius: 10px;
            background: rgba(44, 62, 80, 0.85);
        }
        
        .pump-bar-row .pump-bubble {
            font-size: 0.75rem;
        }
        
        .pump-bar-inline .pump-bubble {
            font-size: 0.65rem;
            padding: 3px 6px;
        }
        
        .status-item {
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
.main-content {
            display: grid;
            grid-template-columns: 1fr;
            gap: 20px;
            flex: 1;
            overflow: hidden;
        }
        
        @media (max-width: 900px) {
            .main-content { grid-template-columns: 1fr; }
        }
        
        .panel {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            padding: 15px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            display: flex;
            flex-direction: column;
            flex: 1;
            min-height: 0;
            overflow: hidden;
            box-sizing: border-box;
            max-height: calc(100vh - 160px);
        }
        
@media (max-width: 900px) {
            .main-content { grid-template-columns: 1fr; }
            .panel {
                max-height: 70vh;
                overflow-y: auto;
            }
            .cocktail-grid {
                max-height: calc(70vh - 120px);
                overflow-y: auto;
            }
            body {
                padding: 5px;
            }
            .container {
                overflow: hidden;
            }
            header {
                padding: 10px;
                margin-bottom: 10px;
            }
            .header-top {
                flex-direction: column;
                gap: 8px;
            }
            .header-right {
                width: 100%;
                justify-content: center;
                flex-wrap: wrap;
            }
        }

        .collapsible {
            background: var(--light);
            border-radius: 8px;
            margin: 10px 0;
            overflow: hidden;
        }

        .collapsible-header {
            padding: 12px 15px;
            cursor: pointer;
            display: flex;
            justify-content: space-between;
            align-items: center;
            font-weight: bold;
            color: var(--primary);
            user-select: none;
        }

        .collapsible-header i {
            transition: transform 0.3s ease;
        }

        .collapsible.open .collapsible-header i {
            transform: rotate(180deg);
        }

        .collapsible-content {
            display: none;
            padding: 0 15px 15px;
        }

        .collapsible.open .collapsible-content {
            display: block;
        }

        .panel h2 {
            color: var(--primary);
            margin-bottom: 15px;
            padding-bottom: 10px;
            border-bottom: 2px solid var(--secondary);
            display: flex;
            align-items: center;
            gap: 10px;
        }
        
.filter-section {
            display: flex;
            gap: 10px;
            justify-content: center;
            flex-wrap: wrap;
        }
        
        .filter-inline {
            margin-left: 20px;
            display: inline-flex;
            gap: 5px;
        }
        
        .filter-inline .filter-btn {
            padding: 4px 10px;
            font-size: 0.75rem;
            border-radius: 5px;
        }
        
        .filter-btn {
            padding: 10px 20px;
            border: none;
            border-radius: 50px;
            background: var(--light);
            color: var(--dark);
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
        .filter-btn:hover { transform: translateY(-2px); box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2); }
        .filter-btn.active { background: var(--primary); color: white; }
        
        .cocktail-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(180px, 1fr));
            gap: 15px;
            flex: 1;
            overflow-y: auto;
            padding: 5px;
            padding-bottom: 20px;
            align-content: start;
        }
        
        .cocktail-card {
            background: white;
            border-radius: 10px;
            padding: 12px;
            text-align: center;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
            border: 2px solid transparent;
        }
        
        .cocktail-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 8px 25px rgba(0, 0, 0, 0.2);
            border-color: var(--secondary);
        }
        
        .cocktail-card img {
            width: 100%;
            height: 120px;
            object-fit: cover;
            border-radius: 8px;
            margin-bottom: 10px;
            background: #eee;
        }
        
        .cocktail-name {
            font-weight: bold;
            margin-bottom: 5px;
            color: var(--primary);
            font-size: 1rem;
        }
        
        .cocktail-type {
            font-size: 0.8rem;
            color: var(--gray);
        }
        
        
            50% { box-shadow: 0 0 0 10px rgba(39, 174, 96, 0); }
        }
        
        .pump-number {
            font-size: 1.5rem;
            font-weight: bold;
            margin-bottom: 5px;
        }
        
        .pump-drink {
            font-size: 0.85rem;
            margin-bottom: 5px;
        }
        
        .pump-status {
            font-size: 0.75rem;
            opacity: 0.8;
        }
        
        .modal {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.8);
            z-index: 1000;
            place-items: center;
            justify-items: center;
        }
        
        .modal.active {
            display: grid;
        }
        
        .modal-content {
            background: white;
            border-radius: 15px;
            width: 90%;
            max-width: 550px;
            max-height: 90vh;
            overflow-y: auto;
            padding: 20px;
            position: relative;
            animation: modalOpen 0.3s ease;
        }
        
        @keyframes modalOpen {
            from { opacity: 0; transform: scale(0.8); }
            to { opacity: 1; transform: scale(1); }
        }
        
        .close-modal {
            position: absolute;
            top: 15px;
            right: 15px;
            font-size: 1.5rem;
            cursor: pointer;
            color: var(--gray);
        }
        
        .modal-header {
            text-align: center;
            margin-bottom: 20px;
            padding-bottom: 15px;
            border-bottom: 1px solid #eee;
        }
        
        .modal-header h2 { color: var(--primary); margin-bottom: 5px; }
        .modal-header p { color: var(--gray); }
        
        .modal-img {
            width: 100%;
            max-height: 200px;
            object-fit: cover;
            border-radius: 10px;
            margin-bottom: 15px;
        }
        
        .ingredient-list { margin: 15px 0; }
        
        .ingredient-item {
            display: flex;
            justify-content: space-between;
            padding: 10px 0;
            border-bottom: 1px solid #eee;
        }
        
        .step-list { margin: 15px 0; }
        
        .step-item {
            display: flex;
            gap: 12px;
            margin-bottom: 12px;
            align-items: center;
        }
        
        .step-number {
            background: var(--secondary);
            color: white;
            width: 28px;
            height: 28px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            flex-shrink: 0;
            font-weight: bold;
        }
        
        .step-item.done .step-number {
            background: var(--success);
        }
        
        .progress-container {
            margin: 20px 0;
        }
        
        .progress-bar {
            height: 24px;
            background: #eee;
            border-radius: 12px;
            overflow: hidden;
            margin-bottom: 8px;
        }
        
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, var(--success), var(--secondary));
            border-radius: 12px;
            width: 0%;
            transition: width 0.5s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
            font-size: 0.8rem;
            font-weight: bold;
        }
        
        .progress-text {
            text-align: center;
            font-weight: bold;
            color: var(--primary);
        }
        
        .modal-footer {
            margin-top: 20px;
            display: flex;
            gap: 10px;
            justify-content: center;
        }
        
        .modal-footer .btn { flex: 1; max-width: 200px; }
        
        .size-buttons, .size-selector {
            display: flex;
            gap: 10px;
        }
        
        .size-btn {
            background: var(--light);
            color: var(--dark);
            padding: 8px 12px;
            border: 2px solid transparent;
            border-radius: 8px;
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s;
            display: flex;
            flex-direction: column;
            align-items: center;
            font-size: 0.75rem;
        }
        
        .size-btn:hover {
            border-color: var(--secondary);
        }
        
        .size-btn i {
            font-size: 1.2rem;
            margin-bottom: 2px;
        }
        
        .size-btn.active {
            background: var(--primary);
            color: white;
            border-color: var(--primary);
            box-shadow: 0 0 10px rgba(44, 62, 80, 0.5);
        }
        
        .settings-form .form-group {
            margin-bottom: 15px;
        }
        
        .settings-form label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
            color: var(--dark);
        }
        
        .settings-form input,
        .settings-form select {
            width: 100%;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 8px;
            font-size: 1rem;
        }
        
        .pump-config {
            background: var(--light);
            border-radius: 10px;
            padding: 15px;
            margin-bottom: 15px;
        }
        
        .pump-config h4 {
            margin-bottom: 10px;
            color: var(--primary);
        }
        
        .pump-control {
            display: flex;
            gap: 10px;
            margin-top: 10px;
            flex-wrap: wrap;
        }
        
        .pump-control .btn {
            flex: 1;
            min-width: 80px;
        }
        
        .wifi-form input {
            width: 100%;
            padding: 12px;
            margin-bottom: 10px;
            border: 1px solid #ddd;
            border-radius: 8px;
        }
        
        .wifi-list {
            list-style: none;
            padding: 0;
            max-height: 150px;
            overflow-y: auto;
            margin: 10px 0;
        }
        
        .wifi-list li {
            padding: 10px;
            background: #eee;
            margin: 5px 0;
            border-radius: 5px;
            cursor: pointer;
        }
        
.wifi-list li:hover { background: #ddd; }

        .settings-tabs {
            display: flex;
            gap: 5px;
            margin-bottom: 20px;
            border-bottom: 2px solid var(--secondary);
            padding-bottom: 10px;
        }

        .tab-btn {
            flex: 1;
            padding: 12px 15px;
            border: none;
            border-radius: 8px 8px 0 0;
            background: var(--light);
            color: var(--dark);
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 8px;
        }

        .tab-btn:hover { background: #ddd; }
        .tab-btn.active { background: var(--primary); color: white; }

        .tab-content { display: none; }
        .tab-content.active { display: block; }

        .calibration-step {
            background: var(--light);
            border-radius: 12px;
            padding: 20px;
            margin: 15px 0;
        }
        
        .step-indicator {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 5px;
            margin-bottom: 20px;
        }
        
        .step-circle {
            width: 30px;
            height: 30px;
            border-radius: 50%;
            background: #ddd;
            color: #666;
            display: flex;
            align-items: center;
            justify-content: center;
            font-weight: bold;
            transition: all 0.3s ease;
        }
        
        .step-circle.active {
            background: var(--secondary);
            color: white;
        }
        
        .step-circle.done {
            background: var(--success);
            color: white;
        }
        
        .step-line {
            flex: 1;
            max-width: 30px;
            height: 3px;
            background: #ddd;
        }
        
        .step-content h4 {
            color: var(--primary);
            margin-bottom: 10px;
        }
        
        .step-content p {
            color: #666;
            margin-bottom: 10px;
        }
        
        .info-bubble {
            display: none;
            position: absolute;
            z-index: 100;
            background: var(--dark);
            color: white;
            padding: 12px;
            border-radius: 8px;
            font-size: 0.85rem;
            max-width: 250px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.3);
            margin-top: -10px;
        }
        
        .info-bubble::before {
            content: '';
            position: absolute;
            top: -8px;
            left: 20px;
            border-width: 0 8px 8px 8px;
            border-style: solid;
            border-color: transparent transparent var(--dark) transparent;
        }
        
        .info-bubble.show {
            display: block;
            animation: fadeIn 0.3s ease;
        }
        
        .info-bubble i.fa-times {
            position: absolute;
            top: 5px;
            right: 8px;
            cursor: pointer;
            font-size: 0.9rem;
            opacity: 0.7;
        }
        
        .info-bubble i.fa-times:hover {
            opacity: 1;
        }
        
        .info-icon {
            color: var(--secondary);
            cursor: pointer;
            margin-left: 5px;
            font-size: 0.9rem;
        }
        
        .info-icon:hover {
            color: var(--primary);
        }
        
        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(-10px); }
            to { opacity: 1; transform: translateY(0); }
        }
        
        .form-group {
            position: relative;
        }
        
        .loading {
            text-align: center;
            padding: 40px;
            color: var(--gray);
        }
        
        .loading i {
            font-size: 3rem;
            animation: spin 1s linear infinite;
        }
        
        @keyframes spin { 100% { transform: rotate(360deg); } }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <div class="header-top">
                <div class="header-left">
                    <h1><i class="fas fa-cocktail"></i> Cocktailmaschine</h1>
                </div>
                <div class="header-right">
                    <div class="status-display">
                        <i class="fas fa-weight-hanging"></i>
                        <span id="weight-display" style="cursor:pointer" title="Klicken zum Tarieren">-- g</span>
                    </div>
                    <button class="btn btn-warning" onclick="openSettingsModal()">
                        <i class="fas fa-cogs"></i> Einstellungen
                    </button>
                    <button class="btn btn-primary" id="wifi-btn" onclick="openWiFiModal()">
                        <i class="fas fa-wifi"></i> <span id="wifi-ssid">--</span>
                    </button>
                </div>
            </div>
            <div class="header-pump-bar" id="pump-status-bar"></div>
        </header>
        
        <div class="main-content">
            <div class="panel">
                <h2><i class="fas fa-glass-martini-alt"></i> Cocktails
                    <span class="filter-inline">
                        <button class="filter-btn active" id="filter-all" onclick="setFilter('all')">Alle</button>
                        <button class="filter-btn" id="filter-alc" onclick="setFilter('alcoholic')">Alkoholisch</button>
                        <button class="filter-btn" id="filter-nonalc" onclick="setFilter('non-alcoholic')">Nicht Alkoholisch</button>
                    </span>
                </h2>
                <div class="cocktail-grid" id="cocktail-grid">
                    <div class="loading"><i class="fas fa-spinner"></i><p>Lade Cocktails...</p></div>
                </div>
            </div>
        </div>
    </div>
    
    <!-- Cocktail Modal -->
    <div id="cocktail-modal" class="modal">
        <div class="modal-content">
            <span class="close-modal" onclick="document.getElementById('cocktail-modal').classList.remove('active')">&times;</span>
            <div class="modal-header">
                <h2 id="modal-name">Cocktailname</h2>
                <p id="modal-type">Typ</p>
            </div>
            <img id="modal-img" class="modal-img" src="" alt="">
            
            <div class="collapsible" id="ingredients-collapsible">
                <div class="collapsible-header" onclick="this.parentElement.classList.toggle('open')">
                    <span><i class="fas fa-list"></i> Zutaten</span>
                    <i class="fas fa-chevron-down"></i>
                </div>
                <div class="ingredient-list collapsible-content" id="modal-ingredients"></div>
            </div>
            
            <h3><i class="fas fa-step-forward"></i> Zubereitung</h3>
            <div class="step-list" id="modal-steps"></div>
            
            <div class="progress-container">
                <div class="progress-bar">
                    <div class="progress-fill" id="progress-fill"></div>
                </div>
                <p class="progress-text" id="progress-text">Bereit zum Starten</p>
            </div>
            
            <div class="modal-footer">
                <div class="size-buttons">
                    <button type="button" class="btn size-btn" onclick="selectSize(200, 0)" title="Klein">
                        <i class="fas fa-glass-whiskey"></i> 200
                    </button>
                    <button type="button" class="btn size-btn" onclick="selectSize(300, 1)" title="Mittel">
                        <i class="fas fa-glass-martini"></i> 300
                    </button>
                    <button type="button" class="btn size-btn active" onclick="selectSize(500, 2)" title="Groß">
                        <i class="fas fa-glass-whiskey"></i> 500
                    </button>
                </div>
                <button type="button" class="btn btn-danger" onclick="closeModal()">Schließen</button>
                <button type="button" class="btn btn-success" id="make-btn" onclick="startMake()">
                    <i class="fas fa-play"></i> Starten
                </button>
            </div>
        </div>
    </div>
    
    <!-- Settings Modal -->
    <div id="settings-modal" class="modal">
        <div class="modal-content">
            <span class="close-modal" onclick="closeSettingsModal()">&times;</span>
            <div class="modal-header">
                <h2><i class="fas fa-cogs"></i> Einstellungen</h2>
            </div>
            <div class="settings-tabs">
                <button class="tab-btn active" data-tab="pump" onclick="switchTab('pump')">
                    <i class="fas fa-water"></i> Pumpe
                </button>
                <button class="tab-btn" data-tab="scale" onclick="switchTab('scale')">
                    <i class="fas fa-weight-hanging"></i> Waage
                </button>
                <button class="tab-btn" data-tab="wifi" onclick="switchTab('wifi')">
                    <i class="fas fa-wifi"></i> WLAN
                </button>
            </div>
            <div class="settings-form">
                <div id="tab-pump" class="tab-content active">
                    <h3>Pumpen-Einstellungen</h3>
                    <div class="form-group">
                        <label>
                            Pulsdauer (ms)
                            <i class="fas fa-info-circle info-icon" onclick="showInfoBubble(this, 'info-pulse')"></i>
                        </label>
                        <input type="number" id="pump-pulse-duration" value="500">
                        <div class="info-bubble" id="info-pulse">
                            <i class="fas fa-times" onclick="closeInfoBubble('info-pulse')"></i>
                            <strong>Pulsdauer</strong><br>
                            Wie lange eine Pumpe pro ml aktiviert wird. Typisch: 10ms/ml. Höhere Werte = mehr Durchfluss.
                        </div>
                    </div>
                    <div class="form-group">
                        <label>
                            Threshold (g)
                            <i class="fas fa-info-circle info-icon" onclick="showInfoBubble(this, 'info-pumpthreshold')"></i>
                        </label>
                        <input type="number" id="pump-threshold" value="10">
                        <div class="info-bubble" id="info-pumpthreshold">
                            <i class="fas fa-times" onclick="closeInfoBubble('info-pumpthreshold')"></i>
                            <strong>Threshold</strong><br>
                            Maximale Abweichung in Gramm, die beim Abmessen toleriert wird. Typisch: 5-10g.
                        </div>
                    </div>
                    <button class="btn btn-primary" onclick="savePumpGlobals()" style="width:100%;margin-bottom:20px;">
                        <i class="fas fa-save"></i> Speichern
                    </button>
                    
                    <h3>Getränk-Zuordnung</h3>
                    <select id="pump-select" onchange="loadPumpConfig()" style="margin-bottom:15px;">
                        <option value="0">Alle Pumpen</option>
                        <option value="1">Pumpe 1</option>
                        <option value="2">Pumpe 2</option>
                        <option value="3">Pumpe 3</option>
                        <option value="4">Pumpe 4</option>
                        <option value="5">Pumpe 5</option>
                        <option value="6">Pumpe 6</option>
                        <option value="7">Pumpe 7</option>
                        <option value="8">Pumpe 8</option>
                    </select>
                    
                    <div id="pump-config-area">
                        <div class="pump-config">
                            <h4>Pumpe <span id="config-pump-num">1</span></h4>
                            <div class="form-group">
                                <label>Angeschlossenes Getränk</label>
                                <select id="pump-drink">
                                    <option value="">-- Nicht belegt --</option>
                                    <option value="Vodka">Vodka</option>
                                    <option value="Rum weiß">Rum weiß</option>
                                    <option value="Dunkler Rum">Dunkler Rum</option>
                                    <option value="Gin">Gin</option>
                                    <option value="Tequila">Tequila</option>
                                    <option value="Whiskey">Whiskey</option>
                                    <option value="Triple Sec">Triple Sec</option>
                                    <option value="Cointreau">Cointreau</option>
                                    <option value="Amaretto">Amaretto</option>
                                    <option value="Kahlua">Kahlua</option>
                                    <option value="Limette">Limette</option>
                                    <option value="Zitronensaft">Zitronensaft</option>
                                    <option value="Orangensaft">Orangensaft</option>
                                    <option value="Ananassaft">Ananassaft</option>
                                    <option value="Cranberrysaft">Cranberrysaft</option>
                                    <option value="Kokosmilch">Kokosmilch</option>
                                    <option value="Ingwersirup">Ingwersirup</option>
                                    <option value="Minze">Minze</option>
                                    <option value="Soda">Soda</option>
                                    <option value="Cola">Cola</option>
                                    <option value="Kirschsaft">Kirschsaft</option>
                                    <option value="Bananensaft">Bananensaft</option>
                                </select>
                            </div>
                            <div class="form-group">
                                <label>
                                    g/ml Faktor
                                    <i class="fas fa-info-circle info-icon" onclick="showInfoBubble(this, 'info-rate')"></i>
                                </label>
                                <input type="number" id="pump-rate" step="0.01" value="1.00">
                                <div class="info-bubble" id="info-rate">
                                    <i class="fas fa-times" onclick="closeInfoBubble('info-rate')"></i>
                                    <strong>g/ml Faktor</strong><br>
                                    Korrekturfaktor für die Durchflussmenge. 1.0 = Standard. Größer = mehr pro ml ausgegeben.
                                </div>
                            </div>
                            <button class="btn btn-success" onclick="savePumpConfig()">
                                <i class="fas fa-save"></i> Speichern
                            </button>
                        </div>
                    </div>
                </div>
                
                <div id="tab-scale" class="tab-content">
                    <h3>Waage Kalibrierung</h3>
                    
                    <div id="calibration-wizard">
                        <div class="calibration-step" data-step="1">
                            <div class="step-indicator">
                                <div class="step-circle active">1</div>
                                <div class="step-line"></div>
                                <div class="step-circle">2</div>
                                <div class="step-line"></div>
                                <div class="step-circle">3</div>
                            </div>
                            <div class="step-content">
                                <h4><i class="fas fa-hand-paper"></i> Schritt 1: Leere Waage</h4>
                                <p>Stellen Sie sicher, dass die Waage leer ist und nichts darauf liegt.</p>
                                <button class="btn btn-primary" onclick="calibrationStep(2)" style="width:100%;margin-top:15px;">
                                    <i class="fas fa-arrow-right"></i> Weiter
                                </button>
                            </div>
                        </div>
                        
                        <div class="calibration-step" data-step="2" style="display:none;">
                            <div class="step-indicator">
                                <div class="step-circle done">1</div>
                                <div class="step-line"></div>
                                <div class="step-circle active">2</div>
                                <div class="step-line"></div>
                                <div class="step-circle">3</div>
                            </div>
                            <div class="step-content">
                                <h4><i class="fas fa-weight-hanging"></i> Schritt 2: 1kg Referenzgewicht</h4>
                                <p>Platzieren Sie jetzt ein 1kg Gewicht auf der Waage.</p>
                                <p style="text-align:center;font-size:1.5rem;font-weight:bold;margin:20px 0;">
                                    <i class="fas fa-weight-hanging"></i> 1000g
                                </p>
                                <button class="btn btn-success" onclick="calibrationStep(3)" style="width:100%;margin-top:15px;">
                                    <i class="fas fa-check"></i> Kalibrieren
                                </button>
                            </div>
                        </div>
                        
                        <div class="calibration-step" data-step="3" style="display:none;">
                            <div class="step-indicator">
                                <div class="step-circle done">1</div>
                                <div class="step-line"></div>
                                <div class="step-circle done">2</div>
                                <div class="step-line"></div>
                                <div class="step-circle done">3</div>
                            </div>
                            <div class="step-content">
                                <h4><i class="fas fa-check-circle"></i> Kalibrierung abgeschlossen</h4>
                                <p id="calibration-result" style="margin:15px 0;padding:15px;background:#d4edda;border-radius:8px;color:#155724;">
                                    <i class="fas fa-check"></i> Waage wurde erfolgreich kalibriert!
                                </p>
                                <button class="btn btn-primary" onclick="resetCalibration()" style="width:100%;">
                                    <i class="fas fa-redo"></i> Erneut kalibrieren
                                </button>
                            </div>
                        </div>
                    </div>
                    
                    <hr style="margin:25px 0;border:none;border-top:1px solid #ddd;">
                    
                    <h3>Grenzwerte</h3>
                    <div class="form-group">
                        <label>
                            Schwellwert (g)
                            <i class="fas fa-info-circle info-icon" onclick="showInfoBubble(this, 'info-threshold')"></i>
                        </label>
                        <input type="number" id="weight-threshold" value="10">
                        <div class="info-bubble" id="info-threshold">
                            <i class="fas fa-times" onclick="closeInfoBubble('info-threshold')"></i>
                            <strong>Schwellwert</strong><br>
                            Mindestgewicht in Gramm, das die Waage erkennen muss, um einen Cocktail zu starten. Empfohlen: 10g
                        </div>
                    </div>
                    <div class="form-group">
                        <label>
                            Schwingdauer (ms)
                            <i class="fas fa-info-circle info-icon" onclick="showInfoBubble(this, 'info-swing')"></i>
                        </label>
                        <input type="number" id="swing-time" value="100">
                        <div class="info-bubble" id="info-swing">
                            <i class="fas fa-times" onclick="closeInfoBubble('info-swing')"></i>
                            <strong>Schwingdauer</strong><br>
                            Wartezeit nach dem Einschenken, damit die Waage nicht durch Schwingungen falsche Werte misst.
                        </div>
                    </div>
                    <button class="btn btn-primary" onclick="saveLimits()" style="width:100%;">
                        <i class="fas fa-save"></i> Speichern
                    </button>
                </div>
                
                <div id="tab-wifi" class="tab-content">
                    <h3>WLAN Einstellungen</h3>
                    <div class="status-item" style="margin-bottom:15px;">
                        <strong>Aktuell:</strong> <span id="current-wifi">--</span>
                    </div>
                    <button class="btn btn-primary" onclick="scanWiFi()" style="width:100%;">
                        <i class="fas fa-sync"></i> Netzwerke scannen
                    </button>
                    <ul class="wifi-list" id="wifi-list"></ul>
                    <input type="text" id="wifi-ssid" placeholder="SSID">
                    <input type="password" id="wifi-pass" placeholder="Passwort">
                    <button class="btn btn-success" onclick="saveWiFi()" style="width:100%;margin-top:15px;">
                        <i class="fas fa-save"></i> Speichern & Neustarten
                    </button>
                </div>
            </div>
        </div>
    </div>
    
    <script>
        // WebSocket
        var ws = null;
        var cocktails = [];
        var settings = { pumps: [] };
        var currentFilter = 'all';
        var makeQueue = [];
        var currentStep = 0;
        var selectedCocktail = null;
        var selectedSize = 500;
        var wsConnected = false;
        
        function init() {
            connectWS();
            loadCocktails();
            loadSettings();
            renderPumpStatus();
            setInterval(sendStatusRequest, 2000);
        }
        
        function connectWS() {
            ws = new WebSocket('ws://' + window.location.host + '/ws');
            
            ws.onopen = function() {
                console.log('WS verbunden');
                wsConnected = true;
                ws.send(JSON.stringify({ type: 'status' }));
                
                document.getElementById('weight-display').onclick = function() {
                    tareScale();
                };
            };
            
            ws.onmessage = function(event) {
                try {
                    var data = JSON.parse(event.data);
                    handleWSMessage(data);
                } catch(e) { console.log('WS parse error:', e); }
            };
            
            ws.onclose = function() {
                console.log('WS getrennt');
                wsConnected = false;
                setTimeout(connectWS, 3000);
            };
            
            ws.onerror = function(e) { console.log('WS error:', e); };
        }
        
        function handleWSMessage(data) {
            if (data.weight !== undefined) {
                var w = parseFloat(data.weight);
                if (!isNaN(w)) {
                    document.getElementById('weight-display').textContent = Math.round(w) + ' g';
                }
            }
            
            if (data.wifi !== undefined) {
                var w = data.wifi;
                var ssid = w.connected ? (w.ssid || 'Verbunden') : 'Getrennt';
                var rssi = w.rssi || 0;
                var bars = 0;
                if (rssi >= -50) bars = 4;
                else if (rssi >= -60) bars = 3;
                else if (rssi >= -70) bars = 2;
                else if (rssi >= -80) bars = 1;
                
                document.getElementById('wifi-ssid').textContent = ssid;
                var wifiBtn = document.getElementById('wifi-btn');
                wifiBtn.title = ssid + ' (' + rssi + ' dBm)';
            }
            
            if (data.pumps !== undefined) {
                data.pumps.forEach(function(p, i) {
                    if (settings.pumps[i]) settings.pumps[i].active = p.active;
                });
                renderPumpStatus();
            }
        }
        
        function sendStatusRequest() {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify({ type: 'status' }));
            }
        }
        
        function loadCocktails() {
            fetch('/cocktails')
                .then(function(r) { return r.json(); })
                .then(function(data) {
                    cocktails = data;
                    renderCocktails();
                })
                .catch(function(e) {
                    console.error('Load cocktails failed:', e);
                });
        }
        
        function loadSettings() {
            fetch('/settings')
                .then(function(r) { return r.json(); })
                .then(function(data) {
                    settings = data;
                    renderPumpStatus();
                })
                .catch(function(e) {
                    console.error('Load settings failed:', e);
                    for (var i = 0; i < 8; i++) {
                        settings.pumps[i] = { drink: '', rate: 10, prime: 2000 };
                    }
                    renderPumpStatus();
                });
        }
        
        function renderCocktails() {
            var grid = document.getElementById('cocktail-grid');
            grid.innerHTML = '';
            
            var filtered = cocktails;
            if (currentFilter !== 'all') {
                filtered = cocktails.filter(function(c) { return c.type === currentFilter; });
            }
            
            filtered.forEach(function(c) {
                var card = document.createElement('div');
                card.className = 'cocktail-card';
                card.innerHTML = '<img src="' + c.image + '" alt="' + c.name + '">' +
                    '<div class="cocktail-name">' + c.name + '</div>' +
                    '<div class="cocktail-type">' + (c.type === 'alcoholic' ? 'Alkoholisch' : 'Alkoholfrei') + '</div>';
                card.onclick = function() { openCocktailModal(c); };
                grid.appendChild(card);
            });
        }
        
        function renderPumpStatus() {
            var bar = document.getElementById('pump-status-bar');
            if (!bar) return;
            bar.innerHTML = '';
            
            for (var i = 0; i < 8; i++) {
                var pump = settings.pumps[i] || { drink: '', rate: 1.0, prime: 2000, active: false };
                var bubble = document.createElement('span');
                bubble.className = 'pump-bubble';
                bubble.dataset.port = i;
                bubble.innerHTML = '<span class="dot"></span>P' + (i + 1) + ':' + (pump.drink || '-');
                bubble.style.cursor = 'pointer';
                bubble.title = 'P' + (i + 1) + ': ' + (pump.drink || 'Nicht belegt');
                
                if (pump.active) {
                    bubble.classList.add('active');
                }
                
                (function(el, port) {
                    el.onmousedown = function() {
                        el.classList.add('active');
                        if (ws && ws.readyState === WebSocket.OPEN) {
                            ws.send(JSON.stringify({ type: 'start', port: port }));
                        }
                    };
                    el.onmouseup = function() {
                        el.classList.remove('active');
                        if (ws && ws.readyState === WebSocket.OPEN) {
                            ws.send(JSON.stringify({ type: 'stop', port: port }));
                        }
                    };
                    el.onmouseleave = function() {
                        if (el.classList.contains('active')) {
                            el.classList.remove('active');
                            if (ws && ws.readyState === WebSocket.OPEN) {
                                ws.send(JSON.stringify({ type: 'stop', port: port }));
                            }
                        }
                    };
                    el.ontouchstart = function(e) { e.preventDefault(); el.onmousedown(); };
                    el.ontouchend = function(e) { e.preventDefault(); el.onmouseup(); };
                })(bubble, i);
                
                bar.appendChild(bubble);
            }
        }
        
        function setFilter(type) {
            currentFilter = type;
            document.querySelectorAll('.filter-btn').forEach(function(btn) { btn.classList.remove('active'); });
            document.getElementById('filter-' + (type === 'all' ? 'all' : (type === 'alcoholic' ? 'alc' : 'nonalc'))).classList.add('active');
            renderCocktails();
        }
        
        function openCocktailModal(c) {
            selectedCocktail = c;
            document.getElementById('modal-name').textContent = c.name;
            document.getElementById('modal-type').textContent = c.type === 'alcoholic' ? 'Alkoholisch' : 'Alkoholfrei';
            document.getElementById('modal-img').src = c.image;
            
            var ingList = document.getElementById('modal-ingredients');
            ingList.innerHTML = '';
            c.ingredients.forEach(function(i) {
                var item = document.createElement('div');
                item.className = 'ingredient-item';
                item.innerHTML = '<span>' + i.name + '</span><span>' + i.ml + ' ml</span>';
                ingList.appendChild(item);
            });
            
            var stepList = document.getElementById('modal-steps');
            stepList.innerHTML = '';
            c.steps.forEach(function(s, idx) {
                var item = document.createElement('div');
                item.className = 'step-item';
                item.id = 'step-' + idx;
                item.innerHTML = '<div class="step-number">' + (idx + 1) + '</div><span>' + s.text + '</span>';
                stepList.appendChild(item);
            });
            
            document.getElementById('progress-fill').style.width = '0%';
            document.getElementById('progress-text').textContent = 'Bereit zum Starten';
            document.getElementById('make-btn').disabled = false;
            
            currentStep = 0;
            makeQueue = [];
            
            document.getElementById('cocktail-modal').classList.add('active');
        }
        
        function closeModal() {
            document.getElementById('cocktail-modal').classList.remove('active');
            selectedCocktail = null;
        }
        
        function selectSize(size, index) {
            selectedSize = size;
            document.querySelectorAll('.size-btn').forEach(function(btn, i) {
                btn.classList.remove('active');
                if (i === index) btn.classList.add('active');
            });
        }
        
        function startMake() {
            if (!selectedCocktail || !ws || ws.readyState !== WebSocket.OPEN) return;
            
            var c = selectedCocktail;
            var base = c.baseMl || 250;
            var factor = selectedSize / base;
            makeQueue = c.steps.map(function(s) { 
                var ml = s.ml ? Math.round(s.ml * factor) : 0;
                return { drink: s.drink, ml: ml, text: s.text }; 
            });
            currentStep = 0;
            
            document.getElementById('make-btn').disabled = true;
            processNextStep();
        }
        
        function processNextStep() {
            if (currentStep >= makeQueue.length) {
                document.getElementById('progress-fill').style.width = '100%';
                document.getElementById('progress-text').textContent = 'Fertig! Cocktail servieren.';
                document.getElementById('make-status').textContent = 'Fertig';
                return;
            }
            
            var step = makeQueue[currentStep];
            var progress = ((currentStep + 1) / makeQueue.length) * 100;
            document.getElementById('progress-fill').style.width = progress + '%';
            document.getElementById('progress-text').textContent = step.text;
            
            document.querySelectorAll('.step-item').forEach(function(el, i) {
                if (i < currentStep) el.classList.add('done');
                else el.classList.remove('done');
            });
            
            if (step.drink && !step.text.includes('Minze')) {
                var port = findPortByDrink(step.drink);
                if (port >= 0) {
                    var ml = step.ml || 30;
                    ws.send(JSON.stringify({
                        type: 'dispense',
                        port: port,
                        ml: ml
                    }));
                }
            }
            
            currentStep++;
            
            if (currentStep < makeQueue.length) {
                setTimeout(processNextStep, 1500);
            } else {
                setTimeout(processNextStep, 500);
            }
        }
        
        function findPortByDrink(drinkName) {
            if (!settings.pumps) return -1;
            for (var i = 0; i < settings.pumps.length; i++) {
                if (settings.pumps[i] && settings.pumps[i].drink === drinkName) {
                    return i;
                }
            }
            return -1;
        }
        
function openWiFiModal() {
            document.getElementById('settings-modal').classList.add('active');
            document.getElementById('settings-modal').style.display = 'grid';
            document.getElementById('settings-modal').style.placeItems = 'center';
            switchTab('wifi');
        }
        
        function closeWiFiModal() {
            var m = document.getElementById('settings-modal');
            m.classList.remove('active');
            m.style.display = '';
        }
        
        function closeWiFiModal() {
            var m = document.getElementById('wifi-modal');
            m.classList.remove('active');
            m.style.display = '';
        }
        
        function scanWiFi() {
            var list = document.getElementById('wifi-list');
            list.innerHTML = '<li>Scannen...</li>';
            
            setTimeout(function() {
                fetch('/scan')
                    .then(function(r) { 
                        if (!r.ok) throw new Error('Scan failed');
                        return r.json(); 
                    })
                    .then(function(data) {
                        list.innerHTML = '';
                        if (data.error) {
                            list.innerHTML = '<li>' + data.error + '</li>';
                            return;
                        }
                        data.networks.forEach(function(n) {
                            var li = document.createElement('li');
                            li.textContent = n.ssid + ' (' + n.rssi + ' dBm)';
                            li.onclick = function() {
                                document.getElementById('wifi-ssid').value = n.ssid;
                            };
                            list.appendChild(li);
                        });
                    })
                    .catch(function(e) {
                        list.innerHTML = '<li>Scan fehlgeschlagen</li>';
                    });
            }, 500);
        }
        
        function saveWiFi() {
            var ssid = document.getElementById('wifi-ssid').value;
            var pass = document.getElementById('wifi-pass').value;
            
            if (!ssid) { alert('Bitte SSID eingeben'); return; }
            
            fetch('/setup', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ ssid: ssid, pass: pass })
            }).then(function() {
                alert('Gespeichert! ESP startet neu...');
            });
        }
        
        function switchTab(tab) {
            document.querySelectorAll('.tab-btn').forEach(function(btn) { btn.classList.remove('active'); });
            document.querySelectorAll('.tab-content').forEach(function(c) { c.classList.remove('active'); });
            document.querySelector('.tab-btn[data-tab="' + tab + '"]').classList.add('active');
            document.getElementById('tab-' + tab).classList.add('active');
        }

        function openSettingsModal() {
            var m = document.getElementById('settings-modal');
            m.classList.add('active');
            m.style.display = 'grid';
            m.style.placeItems = 'center';
            switchTab('pump');
            loadPumpConfig();
        }
        
function closeSettingsModal() {
            var m = document.getElementById('settings-modal');
            m.classList.remove('active');
            m.style.display = '';
        }
        
        function loadPumpConfig() {
            var idx = parseInt(document.getElementById('pump-select').value);
            if (idx === 0) {
                document.getElementById('pump-config-area').style.display = 'none';
                return;
            }
            document.getElementById('pump-config-area').style.display = 'block';
            document.getElementById('config-pump-num').textContent = idx;
            
            var pump = settings.pumps[idx - 1] || { drink: '', rate: 10 };
            document.getElementById('pump-drink').value = pump.drink || '';
            document.getElementById('pump-rate').value = pump.rate || 10;
        }
        
        function savePumpGlobals() {
            var pulseDuration = parseInt(document.getElementById('pump-pulse-duration').value) || 500;
            var threshold = parseInt(document.getElementById('pump-threshold').value) || 10;
            
            fetch('/settings', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    pumpPulseDuration: pulseDuration,
                    pumpThreshold: threshold
                })
            }).then(function() {
                alert('Einstellungen gespeichert!');
            });
        }
        
        function savePumpConfig() {
            var idx = parseInt(document.getElementById('pump-select').value);
            if (idx === 0) return;
            
            var pump = {
                drink: document.getElementById('pump-drink').value,
                rate: parseInt(document.getElementById('pump-rate').value) || 10
            };
            
            settings.pumps[idx - 1] = pump;
            
            fetch('/settings', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ pumps: settings.pumps })
            }).then(function() {
                alert('Einstellungen gespeichert!');
                renderPumpStatus();
            });
        }
        
        function dispensePump(port, ms) {
            if (!ws || ws.readyState !== WebSocket.OPEN) return;
            
            if (port === -1) {
                for (var i = 0; i < 6; i++) {
                    setTimeout(function(p) {
                        ws.send(JSON.stringify({ type: 'prime', port: p }));
                    }, i * 100, i);
                }
            } else if (port === -2) {
                for (var i = 0; i < 6; i++) {
                    setTimeout(function(p) {
                        ws.send(JSON.stringify({ type: 'drain', port: p }));
                    }, i * 100, i);
                }
            } else if (ms > 0) {
                ws.send(JSON.stringify({ type: 'dispense', port: port, ms: ms }));
            }
        }
        
        function tareScale() {
            fetch('/tare', { method: 'POST' })
                .then(function(r) { return r.json(); })
                .then(function(data) {
                    document.getElementById('calibrate-result').textContent = 'Waage tariert! Offset: ' + data.offset;
                })
                .catch(function(e) {
                    console.error('Tare failed:', e);
                });
        }
        
        function calibrationStep(step) {
            document.querySelectorAll('.calibration-step').forEach(function(el) {
                el.style.display = 'none';
            });
            document.querySelector('.calibration-step[data-step="' + step + '"]').style.display = 'block';
            
            if (step === 3) {
                calibrateScale();
            }
        }
        
        function resetCalibration() {
            calibrationStep(1);
        }
        
        function showInfoBubble(el, bubbleId) {
            var bubble = document.getElementById(bubbleId);
            document.querySelectorAll('.info-bubble').forEach(function(b) {
                b.classList.remove('show');
            });
            bubble.classList.add('show');
            bubble.style.top = '30px';
            bubble.style.left = '10px';
        }
        
        function closeInfoBubble(bubbleId) {
            document.getElementById(bubbleId).classList.remove('show');
        }
        
        function calibrateScale() {
            fetch('/calibrate', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ reference: 1000 })
            })
                .then(function(r) { return r.json(); })
                .then(function(data) {
                    var result = document.getElementById('calibration-result');
                    result.innerHTML = '<i class="fas fa-check"></i> Waage kalibriert mit 1000g, Faktor: ' + data.factor.toFixed(6);
                })
                .catch(function(e) {
                    console.error('Calibrate failed:', e);
                    document.getElementById('calibration-result').innerHTML = '<i class="fas fa-exclamation-triangle"></i> Fehler bei Kalibrierung';
                });
        }
        
        function saveLimits() {
            var threshold = parseInt(document.getElementById('weight-threshold').value) || 10;
            var swing = parseInt(document.getElementById('swing-time').value) || 100;
            fetch('/settings', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    limits: {
                        weightThreshold: threshold,
                        swingTime: swing
                    }
                })
            })
                .then(function(r) { return r.json(); })
                .then(function(data) {
                    alert('Grenzwerte gespeichert!');
                })
                .catch(function(e) {
                    console.error('Save failed:', e);
                });
        }
        
        window.onclick = function(e) {
            if (e.target.classList.contains('modal')) {
                e.target.classList.remove('active');
            }
        };
        
        window.addEventListener('DOMContentLoaded', init);
    </script>
</body>
</html>
)rawliteral";