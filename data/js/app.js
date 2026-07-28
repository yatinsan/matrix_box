document.addEventListener('DOMContentLoaded', () => {
    // Navigation
    document.querySelectorAll('.sidebar a').forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            document.querySelectorAll('.sidebar a').forEach(a => a.classList.remove('active'));
            document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
            
            link.classList.add('active');
            const pageId = link.getAttribute('data-page');
            document.getElementById(`page-${pageId}`).classList.add('active');
        });
    });

    // Brightness Control
    const brightnessSlider = document.getElementById('brightness-slider');
    brightnessSlider.addEventListener('change', (e) => {
        fetch('/api/brightness', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ brightness: parseInt(e.target.value) })
        });
    });

    // Load initial data
    fetch('/api/status')
        .then(res => res.json())
        .then(data => {
            brightnessSlider.value = data.brightness;
            document.getElementById('heap-free').innerText = data.heap;
            document.getElementById('psram-free').innerText = data.psram;
        });

    fetch('/api/effects')
        .then(res => res.json())
        .then(data => {
            const grid = document.getElementById('effects-grid');
            data.effects.forEach(effect => {
                const btn = document.createElement('button');
                btn.className = 'effect-btn';
                btn.innerText = effect;
                btn.onclick = () => {
                    // TODO: Implement set effect endpoint
                    console.log('Set effect', effect);
                };
                grid.appendChild(btn);
            });
        });
});
