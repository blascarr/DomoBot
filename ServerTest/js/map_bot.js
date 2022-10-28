const distanceFromBot = 2500;

const gridWidth = 10000;
const gridHeight = 10000;
const gridH = 100;
const gridV = 100;

// Grid units 
// Unitary units in distance defined by 
//  width / columns 
//  height / rows
//  In this case 100 steps = 1 cm

const gridSizeH = gridWidth/gridH;
const gridSizeV = gridHeight/gridV;
const objectScaleH = gridSizeH;
const objectScaleV = gridSizeV;

let container;
let camera, controls, scene, projector, renderer;
let plane;
var geometry = new THREE.CubeGeometry( 1, 1, 1 );
var object = new THREE.Mesh( geometry, new THREE.MeshLambertMaterial( { color: 0x0ab86b } ) );

init();
animate();

function init() {

    container = document.createElement( 'container' );
    document.body.appendChild( container );
    
    // Set scenario
    setCamera();
    controlsConfig();
    scene = new THREE.Scene();

    setLight();
    setBot();
    setGround();

    setRenderer();

    window.addEventListener( 'resize', onWindowResize, false );

}

function setBot(){
    object.material.ambient = object.material.color;

    object.position.x = 50;
    object.position.y = 50;
    object.position.z = 0;

    object.scale.x = objectScaleH;
    object.scale.y = objectScaleV;
    object.scale.z = objectScaleH;

    object.castShadow = true;
    object.receiveShadow = true;

    scene.add( object );
}
function controlsConfig(){
    controls = new THREE.TrackballControls( camera );
    controls.rotateSpeed = 1.0;
    controls.zoomSpeed = 1.2;
    controls.panSpeed = 0.8;
    controls.noZoom = false;
    controls.noRotate = true;
    controls.noPan = false;
    controls.staticMoving = true;
    controls.dynamicDampingFactor = 0.3;
}

function setGround(){
    plane = new THREE.Mesh( 
        new THREE.PlaneGeometry( gridWidth, gridHeight, gridH, gridV ), 
        new THREE.MeshBasicMaterial( { 
            color: 0x000000, 
            opacity: 0.25, 
            transparent: true, 
            wireframe: true 
        }) 
    );
    plane.visible = true;
    scene.add( plane );
}

function setRenderer(){
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.sortObjects = false;
    renderer.setSize( window.innerWidth, window.innerHeight );

    renderer.shadowMapEnabled = true;
    renderer.shadowMapSoft = true;
    container.appendChild( renderer.domElement );
}

function setLight(){
    scene.add( new THREE.AmbientLight( 0x505050 ) );

    const light = new THREE.SpotLight( 0xffffff, 1.5 );
    light.position.set( 0, 500, 2000 );
    light.castShadow = true;

    light.shadowCameraNear = 200;
    light.shadowCameraFar = camera.far;
    light.shadowCameraFov = 50;

    light.shadowBias = -0.00022;
    light.shadowDarkness = 0.5;

    light.shadowMapWidth = 2048;
    light.shadowMapHeight = 2048;

    scene.add( light );
}

function setCamera(){
    camera = new THREE.PerspectiveCamera( 70, window.innerWidth / window.innerHeight, 1, 10000 );
    camera.position.z = distanceFromBot;
}

function onWindowResize() {

    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( window.innerWidth, window.innerHeight );

}

function animate() {

    requestAnimationFrame( animate );

    render();

}

function render() {

    controls.update();

    renderer.render( scene, camera );

}

if (!!window.EventSource) {
    var source = new EventSource('/map_events');
    source.addEventListener('open', function (e) {console.log(
        "Events Connected");
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
        }
    }, false);

    //Bot Position
    source.addEventListener('map_data', function (e) {
        let obj = JSON.parse(e.data);
        console.log(obj);
        object.position.x = obj.x;
        object.position.y = obj.y;
        object.updateMatrix ();
        object.rotation.z = obj.theta;
        domget("x_pos_data").innerHTML = obj.x;
        domget("y_pos_data").innerHTML = obj.y;
        domget("theta_pos_data").innerHTML = (obj.theta*180/Math.PI)%360;
    }, false)

};