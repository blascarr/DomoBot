
//let texture = loadTexture('https://assetbundle-tutorial.s3.eu-west-2.amazonaws.com/water.jpg');
//let texture = loadTexture('models/disturb.jpg');
let pos = new THREE.Vector3( -5, 0, 20 );
let rot = new THREE.Vector3( Math.PI/2, Math.PI/2, 0 );
let scalevalue= 3;
let scalation = new THREE.Vector3( ).addScalar( scalevalue );

let pose ={
    p: pos,
    r: rot,
    s: scalation
}

/*let pose ={
    position: pos,
    rotation: rot,
    scale : scalation
}

let mesh ={
    texture: texture
}

let mesh2 ={
    texture: texture,
    material: 'models/wild_town.mtl'
}*/

init();
animate();
addShape( bot_shape.shape , bot_shape.extrudeSettings, scene, pose );
//loadObjects( 'https://assetbundle-tutorial.s3.eu-west-2.amazonaws.com/wild_town.obj' , texture, pos, scalation, 'https://assetbundle-tutorial.s3.eu-west-2.amazonaws.com/wild_town.mtl');
//loadObjects( 'models/Burton_High.obj' , pose, mesh );
//loadObjects( 'models/wild_town.obj' , pose , mesh2);

//loadObjects( 'models/wild_town.obj' , texture, pos, scalation);
//loadObjects( 'models/stl/ascii/pr2_head_pan.stl' , texture, pos, scalation);