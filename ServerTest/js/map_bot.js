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
let geometry = new THREE.ConeGeometry( 0.5, 2, 15 );
geometry.rotateZ( -Math.PI/2);
let object = new THREE.Mesh( geometry, new THREE.MeshLambertMaterial( { color: 0x0ab86b } ) );

let bot_origin_geometry = new THREE.BoxGeometry( 1, 1, 1 );
let target_geometry = new THREE.BoxGeometry( 1, 1, 1 );
const target =  new THREE.Mesh( target_geometry, new THREE.MeshLambertMaterial( { color: 0x0ab86b } ) );
const bot_origin =  new THREE.Mesh( bot_origin_geometry, new THREE.MeshLambertMaterial( { color: 0xaaff22 } ) ); 

const axesHelper = new THREE.AxesHelper( helperLength );
const originAxisHelper = new THREE.AxesHelper( helperLength );

let target_transformControl, origin_transformControl;

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
    setTarget();
    setBotOrigin();
    setGround();
    setWorldAxis();
    scene.add( originAxisHelper );

    target_transform();
    origin_transform();
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

function setTarget(){
    target.material.ambient = target.material.color;

    target.position.x = 400;
    target.position.y = 500;
    target.position.z = 0;

    target.scale.x = objectScaleH;
    target.scale.y = objectScaleV;
    target.scale.z = objectScaleH;

    target.castShadow = true;
    target.receiveShadow = true;

    scene.add( target );
}

function setBotOrigin(){
    bot_origin.material.ambient = bot_origin.material.color;

    bot_origin.position.x = 0;
    bot_origin.position.y = 0;
    bot_origin.position.z = 0;

    bot_origin.scale.x = objectScaleH;
    bot_origin.scale.y = objectScaleV;
    bot_origin.scale.z = objectScaleH;

    bot_origin.castShadow = true;
    bot_origin.receiveShadow = true;
    bot_origin.visible = false;
    scene.add( bot_origin );
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
    light.position.set( 0, 500, 3000 );
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

function target_transform(){
    target_transformControl = new THREE.TransformControls( camera, renderer.domElement );
    /*target_transformControl.addEventListener( 'change', render );
    target_transformControl.addEventListener( 'dragging-changed', function ( event ) {
        controls.enabled = ! event.value;
    } );
    target_transformControl.addEventListener( 'objectChange', function () {    } );
    */
    scene.add( target_transformControl );

    target_transformControl.showZ = false;
    target_transformControl.attach( target );
}

function origin_transform(){
    origin_transformControl = new THREE.TransformControls( camera, renderer.domElement );
    //origin_transformControl.addEventListener( 'change', render );
    //origin_transformControl.addEventListener( 'dragging-changed', function ( event ) {controls.enabled = ! event.value;} );
    //origin_transformControl.addEventListener( 'objectChange', function () {} );
    scene.add( origin_transformControl );

    origin_transformControl.showZ = false;
    origin_transformControl.attach( bot_origin );
    origin_transformControl.visible = false;
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

/*if (!!window.EventSource) {
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
        object.rotation.z = obj.theta;
        domget("x_pos_data").innerHTML = obj.x;
        domget("y_pos_data").innerHTML = obj.y;
        domget("theta_pos_data").innerHTML = (obj.theta*180/Math.PI)%360;
    }, false)

};*/