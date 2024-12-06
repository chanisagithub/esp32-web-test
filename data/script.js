let cpuData = [];
async function fetchData() {
  const response = await fetch('/system-info');
  const data = await response.json();
  document.getElementById('cpuFreq').innerText = data.cpuFreq + ' MHz';
  document.getElementById('freeHeap').innerText = data.freeHeap + ' bytes';
  document.getElementById('uptime').innerText = data.uptime + ' seconds';

  if (cpuData.length > 20) cpuData.shift();
  cpuData.push(data.cpuFreq);
  drawGraph();
}

function drawGraph() {
  const canvas = document.getElementById('cpuGraph');
  const ctx = canvas.getContext('2d');
  const width = canvas.width;
  const height = canvas.height;
  ctx.clearRect(0, 0, width, height);
  ctx.beginPath();

  const maxValue = 240;
  for (let i = 0; i < cpuData.length; i++) {
    const x = (width / (cpuData.length - 1)) * i;
    const y = height - (cpuData[i] / maxValue) * height;
    if (i === 0) ctx.moveTo(x, y);
    else ctx.lineTo(x, y);
  }
  ctx.strokeStyle = '#007BFF';
  ctx.lineWidth = 2;
  ctx.stroke();
}

setInterval(fetchData, 1000);
window.onload = fetchData;
