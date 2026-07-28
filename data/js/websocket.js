let ws;
const connStatus = document.getElementById('conn-status');

function connectWebSocket() {
    ws = new WebSocket(`ws://${window.location.host}/ws`);
    
    ws.onopen = () => {
        connStatus.innerText = 'Connected';
        connStatus.className = 'connected';
    };
    
    ws.onclose = () => {
        connStatus.innerText = 'Disconnected';
        connStatus.className = 'disconnected';
        setTimeout(connectWebSocket, 2000);
    };
    
    ws.onmessage = (event) => {
        if (typeof event.data === 'string') {
            const data = JSON.parse(event.data);
            if (data.type === 'status') {
                document.getElementById('fps-counter').innerText = `FPS: ${data.fps}`;
            }
        }
    };
}

// Start connection
window.addEventListener('load', connectWebSocket);
