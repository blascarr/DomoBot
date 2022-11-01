const distanceFromBot = 2500;

const gridWidth = 10000;
const gridHeight = 10000;
const gridH = 100;
const gridV = 100;
const helperLength = 150;
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
let camera, controls, scene, projector, renderer,helper;
let plane;
let geometry = new THREE.ConeGeometry( 1, 5, 15 );
geometry.rotateZ( -Math.PI/2);
var object = new THREE.Mesh( geometry, new THREE.MeshLambertMaterial( { color: 0x0ab86b } ) );
const axesHelper = new THREE.AxesHelper( helperLength );
const originAxisHelper = new THREE.AxesHelper( helperLength );

let transformControl;

function init() {

    container = document.createElement( 'container' );
    document.body.appendChild( container );
    
    // Set scenario
    setRenderer();
    setCamera();
    controlsConfig();
    scene = new THREE.Scene();
    scene.background = new THREE.Color( 0xf0f0f0 );
    setLight();
    setBot();
    setGround();
    setWorldAxis();
    scene.add( originAxisHelper );

    attachtransform();
    //setGridHelper();
    objectControl();
    window.addEventListener( 'resize', onWindowResize, false );

}

function setBot(){
    object.material.ambient = object.material.color;

    object.position.x = 0;
    object.position.y = 0;
    object.position.z = 0;

    object.scale.x = objectScaleH;
    object.scale.y = objectScaleV;
    object.scale.z = objectScaleH;

    object.castShadow = true;
    object.receiveShadow = true;

    scene.add( object );
}

function controlsConfig(){
    controls = new THREE.TrackballControls( camera, renderer.domElement );
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

function setGridHelper(){
    helper = new THREE.GridHelper( 2000, 100 );
    helper.position.y = 0;
    helper.material.opacity = 0.25;
    helper.material.transparent = true;
    scene.add( helper );
}

function setWorldAxis(){
    scene.add( axesHelper );
    object.attach( axesHelper );
}
function setRenderer(){
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.sortObjects = false;
    renderer.setSize( window.innerWidth, window.innerHeight );

    renderer.shadowMap.enabled = true;
    renderer.shadowMapSoft = true;
    container.appendChild( renderer.domElement );
}

function setLight(){
    scene.add( new THREE.AmbientLight( 0x505050 ) );

    const light = new THREE.SpotLight( 0xffffff, 1.5 );
    light.position.set( 0, 500, 1000 );
    light.castShadow = false;

    light.shadow.camera.near = 200;
    light.shadow.camera.far = camera.far;
    light.shadow.camera.fov = 50;

    light.shadow.bias = -0.00022;
    light.shadowDarkness = 0.5;

    light.shadow.mapSize.width = 2048;
    light.shadow.mapSize.height = 2048;

    scene.add( light );
}

function setCamera(){
    camera = new THREE.PerspectiveCamera( 70, window.innerWidth / window.innerHeight, 1, 10000 );
    camera.position.z = distanceFromBot;
}

function objectControl(){
    const transformControl = new THREE.TransformControls( camera, renderer.domElement );
}

function attachtransform(){
    transformControl = new THREE.TransformControls( camera, renderer.domElement );
    transformControl.addEventListener( 'change', render );
    transformControl.addEventListener( 'dragging-changed', function ( event ) {

        controls.enabled = ! event.value;

    } );
    scene.add( transformControl );

    transformControl.addEventListener( 'objectChange', function () {


    } );
    transformControl.attach( object );
}

function onWindowResize() {

    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( window.innerWidth, window.innerHeight );

}

function animate() {
    const m = new THREE.Matrix4();
    
    requestAnimationFrame( animate );

    render();

}

function render() {

    controls.update();

    renderer.render( scene, camera );

}
