#include "WebServer.h"
#include "API.h"
#include "WebSocket.h"
#include "WifiManager.h"

WebServerManager Web;

WebServerManager::WebServerManager() : server(80) {}

static const char FALLBACK_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MatrixOS</title>
<style>
* { box-sizing: border-box; }
body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: #0f172a; color: #f8fafc; margin: 0; padding: 16px; }
.container { max-width: 480px; margin: 0 auto; }
header { display: flex; justify-content: space-between; align-items: center; padding: 12px 16px; background: #1e293b; border-radius: 12px; margin-bottom: 16px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }
h1 { font-size: 20px; color: #38bdf8; margin: 0; }
.badge { background: #0284c7; color: #fff; font-size: 11px; padding: 4px 8px; border-radius: 12px; font-weight: bold; }
.tabs { display: flex; gap: 8px; margin-bottom: 16px; }
.tab { flex: 1; padding: 10px; background: #1e293b; color: #94a3b8; border: none; border-radius: 8px; font-weight: bold; cursor: pointer; text-align: center; font-size: 13px; }
.tab.active { background: #38bdf8; color: #0f172a; }
.card { background: #1e293b; padding: 16px; border-radius: 12px; margin-bottom: 16px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }
.card h2 { font-size: 15px; margin: 0 0 12px; color: #cbd5e1; display: flex; justify-content: space-between; }
.btn-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
.btn { padding: 12px; background: #334155; color: white; border: none; border-radius: 8px; font-weight: bold; cursor: pointer; font-size: 14px; text-align: center; }
.btn.active { background: #0284c7; border: 2px solid #38bdf8; }
.btn:hover { background: #475569; }
label { display: block; margin: 10px 0 4px; font-size: 12px; color: #94a3b8; }
input[type="text"], input[type="password"] { width: 100%; padding: 10px; border-radius: 6px; border: 1px solid #334155; background: #0f172a; color: #fff; }
input[type="range"] { width: 100%; height: 6px; border-radius: 3px; background: #334155; accent-color: #38bdf8; }
.submit-btn { width: 100%; padding: 12px; margin-top: 12px; background: #0284c7; color: white; border: none; border-radius: 8px; font-weight: bold; cursor: pointer; }
#matrix-grid { display: grid; grid-template-columns: repeat(17, 1fr); gap: 2px; width: 100%; aspect-ratio: 1; background: #0f172a; padding: 4px; border-radius: 8px; touch-action: none; }
.pixel { background: #334155; border-radius: 2px; aspect-ratio: 1; cursor: pointer; }
.palette { display: flex; gap: 8px; margin-bottom: 12px; align-items: center; }
.color-dot { width: 28px; height: 28px; border-radius: 50%; cursor: pointer; border: 2px solid transparent; }
.color-dot.active { border-color: #fff; scale: 1.1; }
</style>
</head>
<body>
<div class="container">
<header>
  <h1>✨ MatrixOS</h1>
  <span class="badge" id="net-mode">Connected</span>
</header>

<div class="tabs">
  <button class="tab active" onclick="showPage('fx')">🎨 Effects</button>
  <button class="tab" onclick="showPage('draw')">🖌️ Studio</button>
  <button class="tab" onclick="showPage('wifi')">📶 Wi-Fi</button>
</div>

<!-- PAGE: EFFECTS -->
<div id="page-fx">
  <div class="card">
    <h2>✨ Select Animation</h2>
    <div class="btn-grid" id="fx-list">
      <button class="btn active" onclick="setFx('Rainbow')">🌈 Rainbow</button>
      <button class="btn" onclick="setFx('Solid Color')">🎨 Solid Color</button>
    </div>
  </div>
  <div class="card">
    <h2>🎛️ Master Brightness (<span id="b-val">64</span>)</h2>
    <input type="range" min="0" max="255" value="64" oninput="setB(this.value)">
  </div>
</div>

<!-- PAGE: STUDIO (DRAWING) -->
<div id="page-draw" style="display:none">
  <div class="card">
    <h2>🖌️ Live 17x17 Matrix Paint</h2>
    <div class="palette">
      <div class="color-dot active" style="background:#ff0000" onclick="pickC('#ff0000',this)"></div>
      <div class="color-dot" style="background:#00ff00" onclick="pickC('#00ff00',this)"></div>
      <div class="color-dot" style="background:#0000ff" onclick="pickC('#0000ff',this)"></div>
      <div class="color-dot" style="background:#ffff00" onclick="pickC('#ffff00',this)"></div>
      <div class="color-dot" style="background:#ff00ff" onclick="pickC('#ff00ff',this)"></div>
      <div class="color-dot" style="background:#ffffff" onclick="pickC('#ffffff',this)"></div>
      <div class="color-dot" style="background:#000000" onclick="pickC('#000000',this)"></div>
      <input type="color" id="c-picker" value="#ff0000" onchange="curColor=this.value" style="width:32px;height:32px;border:none;background:none">
    </div>
    <div id="matrix-grid"></div>
  </div>
</div>

<!-- PAGE: WIFI SETTINGS -->
<div id="page-wifi" style="display:none">
  <div class="card">
    <h2>📶 Connect to Home Router</h2>
    <form id="wf-form">
      <label>Wi-Fi Network (SSID)</label>
      <input type="text" id="ssid" placeholder="Router Name" required>
      <label>Wi-Fi Password</label>
      <input type="password" id="pass" placeholder="Password">
      <button type="submit" class="submit-btn">Save & Connect</button>
    </form>
  </div>
  <div class="card">
    <h2>ℹ️ Device Info</h2>
    <p style="font-size:13px;color:#94a3b8;margin:4px 0">IP: <span id="dev-ip" style="color:#fff">Loading...</span></p>
    <p style="font-size:13px;color:#94a3b8;margin:4px 0">MAC: <span id="dev-mac" style="color:#fff">Loading...</span></p>
    <p style="font-size:13px;color:#94a3b8;margin:4px 0">FPS: <span id="dev-fps" style="color:#fff">30</span></p>
  </div>
</div>

</div>

<script>
var ws;
var curColor = '#ff0000';
function initWS(){
  var loc = window.location;
  var wsUri = (loc.protocol==='https:'?'wss:':'ws:') + '//' + loc.host + '/ws';
  ws = new WebSocket(wsUri);
}
initWS();

function showPage(p){
  document.getElementById('page-fx').style.display = p==='fx'?'block':'none';
  document.getElementById('page-draw').style.display = p==='draw'?'block':'none';
  document.getElementById('page-wifi').style.display = p==='wifi'?'block':'none';
  document.querySelectorAll('.tab').forEach((t,i)=>{
    t.classList.toggle('active', (p==='fx'&&i===0)||(p==='draw'&&i===1)||(p==='wifi'&&i===2));
  });
}

function setFx(n){
  fetch('/api/effect',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({name:n})});
  document.querySelectorAll('.btn-grid .btn').forEach(b=>b.classList.toggle('active', b.innerText.includes(n)));
}

function setB(v){
  document.getElementById('b-val').innerText = v;
  fetch('/api/brightness',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({brightness:parseInt(v)})});
}

function pickC(c,el){
  curColor = c;
  document.querySelectorAll('.color-dot').forEach(d=>d.classList.remove('active'));
  if(el) el.classList.add('active');
}

// Build 17x17 Grid
var grid = document.getElementById('matrix-grid');
for(var y=0; y<17; y++){
  for(var x=0; x<17; x++){
    var p = document.createElement('div');
    p.className = 'pixel';
    p.dataset.x = x;
    p.dataset.y = y;
    p.onpointerdown = p.onpointerenter = function(e){
      if(e.buttons === 1 || e.type === 'pointerdown'){
        this.style.background = curColor;
        var r = parseInt(curColor.substr(1,2),16);
        var g = parseInt(curColor.substr(3,2),16);
        var b = parseInt(curColor.substr(5,2),16);
        if(ws && ws.readyState === 1){
          ws.send(JSON.stringify({type:'drawPixel',x:parseInt(this.dataset.x),y:parseInt(this.dataset.y),r:r,g:g,b:b}));
        }
      }
    };
    grid.appendChild(p);
  }
}

document.getElementById('wf-form').onsubmit=function(e){
  e.preventDefault();
  fetch('/api/wifi',{
    method:'POST',
    headers:{'Content-Type':'application/json'},
    body:JSON.stringify({ssid:document.getElementById('ssid').value,password:document.getElementById('pass').value})
  }).then(r=>r.json()).then(d=>alert('Saved! MatrixOS is rebooting and connecting to your Wi-Fi router.\n\n1. Switch your phone/PC back to your home Wi-Fi.\n2. Open http://matrix.local in your browser.'));
};

fetch('/api/effects').then(r=>r.json()).then(d=>{
  if(d.effects && d.effects.length){
    var list = document.getElementById('fx-list');
    list.innerHTML = '';
    d.effects.forEach((n, idx)=>{
      var b = document.createElement('button');
      b.className = 'btn' + (idx===0?' active':'');
      b.innerText = n;
      b.onclick = function(){ setFx(n); };
      list.appendChild(b);
    });
  }
});

fetch('/api/status').then(r=>r.json()).then(d=>{
  if(d.ip) document.getElementById('dev-ip').innerText = d.ip;
  if(d.mac) document.getElementById('dev-mac').innerText = d.mac;
  if(d.fps) document.getElementById('dev-fps').innerText = d.fps;
  if(d.brightness) document.getElementById('b-val').innerText = d.brightness;
  if(d.currentEffect){
    document.querySelectorAll('#fx-list .btn').forEach(b=>b.classList.toggle('active', b.innerText===d.currentEffect));
  }
});
</script>
</body>
</html>
)rawliteral";

void WebServerManager::begin() {
    setupCaptivePortal();
    
    // Explicit handler for root URL /
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (LittleFS.exists("/index.html")) {
            request->send(LittleFS, "/index.html", "text/html");
        } else {
            request->send(200, "text/html", FALLBACK_HTML);
        }
    });
    
    // API setup
    API.begin(&server);
    
    // WebSocket setup
    WS.begin(&server);
    
    setupStaticFiles();
    
    // Catch-all for root / 404 and SPA routing
    server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
        } else if (LittleFS.exists("/index.html")) {
            request->send(LittleFS, "/index.html", "text/html");
        } else {
            request->send(200, "text/html", FALLBACK_HTML);
        }
    });
    
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    
    server.begin();
    Serial.println("Web server started on port 80");
}

AsyncWebServer* WebServerManager::getServer() {
    return &server;
}

void WebServerManager::setupCaptivePortal() {
    // If the host isn't our IP or matrix.local, redirect to our captive portal
    AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
    handler->setFilter([](AsyncWebServerRequest *request) -> bool {
        String host = request->host();
        String path = request->url();
        String apIP = WifiMgr.getIP();
        
        // Exclude mDNS and API requests from captive portal redirection
        return (host != apIP && host != "matrix.local" && !path.startsWith("/api") && !path.startsWith("/ws"));
    });
    handler->onRequest([](AsyncWebServerRequest *request) {
        String apIP = WifiMgr.getIP();
        request->redirect("http://" + apIP + "/");
    });
    server.addHandler(handler);
}

void WebServerManager::setupStaticFiles() {
    // Serve everything in LittleFS root to /
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("max-age=86400");
}
