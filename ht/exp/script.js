function load() {

  draw_canvas();
  cgi_fetch();
}

function draw_canvas() {

  const canvas = document.getElementById("render");
  const ctx = canvas.getContext("2d");

  ctx.fillStyle = "rgb(200, 0, 0)";
  ctx.fillRect(10, 10, 50, 50);

  ctx.fillStyle = "rgb(0, 200, 0, 0.5)";
  ctx.fillRect(25, 25, 50, 50);
}

async function cgi_fetch() {

  const url="http://dhole.lan:3002/cgi-bin/hello";

  const response = await fetch(url);

  var data = await response.text();
  document.getElementById("cgi_out").innerHTML=data;
}
