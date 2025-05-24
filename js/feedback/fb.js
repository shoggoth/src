import * as THREE from "three";
var scene;
var camera;
var renderer;

let container = document.querySelector("#container");

function scene_setup() {
    // Root scene setup
    scene = new THREE.Scene();
    var width = window.innerWidth;
    var height = window.innerHeight;
    // Ortho camera setup
    camera = new THREE.OrthographicCamera(width / -2, width / 2, height / 2, height / -2, 1, 1000);
    camera.position.z = 2;
    
    renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setClearColor(0xff0000, 1);
    container.appendChild(renderer.domElement);
}

// Initialize the Threejs scene
scene_setup();

var bufferScene;
var pingTex, pongTex;
var bufferMaterial;
var plane;
var bufferObject;
var finalMaterial;
var quad;

function buffer_texture_setup() {
    // Create buffer scene
    bufferScene = new THREE.Scene();
    // Create ping and pong buffer textures
    pingTex = new THREE.WebGLRenderTarget(window.innerWidth, window.innerHeight, {
        minFilter: THREE.LinearFilter,
        magFilter: THREE.NearestFilter
    });
    pongTex = new THREE.WebGLRenderTarget(window.innerWidth, window.innerHeight, {
        minFilter: THREE.LinearFilter,
        magFilter: THREE.NearestFilter
    });
    // Pass ping texture to shader
    bufferMaterial = new THREE.ShaderMaterial({
        uniforms: {
            bufferTexture: { type: "t", value: pingTex },
            res: {
                type: "v2",
                value: new THREE.Vector2(window.innerWidth, window.innerHeight)
            }, //Keeps the resolution
            smokeSource: { type: "v3", value: new THREE.Vector3(0, 0, 0) } /// This keeps the position of the mouse and whether it was clicked or not
        },
        fragmentShader: document.getElementById("fragShader").innerHTML
    });
    plane = new THREE.PlaneGeometry(window.innerWidth, window.innerHeight);
    bufferObject = new THREE.Mesh(plane, bufferMaterial);
    bufferScene.add(bufferObject);
    
    // Draw pong texture to screen
    finalMaterial = new THREE.MeshBasicMaterial({ map: pongTex.texture });
    quad = new THREE.Mesh(plane, finalMaterial);
    scene.add(quad);
}
buffer_texture_setup();

// Send position of smoke source with value
var mouseDown = false;
function UpdateMousePosition(X, Y) {
    var mouseX = X;
    var mouseY = window.innerHeight - Y;
    bufferMaterial.uniforms.smokeSource.value.x = mouseX;
    bufferMaterial.uniforms.smokeSource.value.y = mouseY;
}
document.onmousemove = function (event) {
    UpdateMousePosition(event.clientX, event.clientY);
};

document.onmousedown = function (event) {
    mouseDown = true;
    bufferMaterial.uniforms.smokeSource.value.z = 0.1;
};
document.onmouseup = function (event) {
    mouseDown = false;
    bufferMaterial.uniforms.smokeSource.value.z = 0;
};

function render() {
    requestAnimationFrame(render);
    
    // Draw to pong texture
    renderer.setRenderTarget(pongTex);
    renderer.render(bufferScene, camera);
    
    // Swap ping and pong textures
    var t = pingTex;
    pingTex = pongTex;
    pongTex = t;
    quad.material.map = pongTex.texture;
    bufferMaterial.uniforms.bufferTexture.value = pingTex.texture;
    
    // Draw to the screen
    renderer.setRenderTarget(null);
    renderer.render(scene, camera);
}

render();

