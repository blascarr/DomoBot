function loadObjects( objectFile , pose, mesh = null){
    let loader;
    let format = objectFile.substring( objectFile.indexOf(".") ).toLowerCase();
    switch (format) {
        case '.obj':
            loader = new THREE.OBJLoader();
            if( mesh !== null  && mesh.material != ''){
                loader = new THREE.OBJMTLLoader();
            }
            
            break;
        case '.stl':
            loader = new THREE.STLLoader();
            break;
        
        case '.dae':
            loader = new THREE.ColladaLoader();
            break;
        

        default:
            return null;
            break;
    }
    
    if (!texture){

    }
    loader.addEventListener( 'load', function ( event ) {

        let object = event.content;
        if( object.children ){
            for ( let i = 0, l = object.children.length; i < l; i ++ ) {
                if(mesh !== null && mesh.material == ''){
                    object.children[ i ].material.map = mesh.texture;
                }
            }
        }
        
        object.position = pose.position;
        object.rotation = pose.rotation;
        object.scale = pose.scale;
        scene.add( object );
    });

    if(mesh != null && mesh.texture != null ){
        loadTexture(mesh.texture);
    }

    if( mesh !== null  && mesh.material != ''){
        loader.load( objectFile, mesh.material );
    }else{
        loader.load( objectFile );
    }
    
}

function loadTexture( textureFile ){
    // Texture
    var texture = new THREE.Texture();

    var loader = new THREE.ImageLoader();
    loader.addEventListener( 'load', function ( event ) {

        texture.image = event.content;
        texture.needsUpdate = true;

    } );
    loader.load( textureFile );
    return texture
}

function shapeLoader( url ){
    const points = [];
    const object = new THREE.Shape( points );

    return object;
}

function addShape( pts, extrudeSettings, parent, transform ) {
    let shape = new THREE.Shape( pts );
    const geometry = new THREE.ShapeGeometry( shape );
    const material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
    const mesh = new THREE.Mesh( geometry, material ) ;
    parent.add( mesh );
    //var points = shape.createPointsGeometry();
    //var spacedPoints = shape.createSpacedPointsGeometry( 100 );

    // flat shape

    /*var geometry = new THREE.ShapeGeometry( shape );

    var mesh = THREE.SceneUtils.createMultiMaterialObject( geometry, [ new THREE.MeshLambertMaterial( { color: extrudeSettings.color } ), new THREE.MeshBasicMaterial( { color: 0x000000, wireframe: true, transparent: true } ) ] );
    mesh.position.set( transform.p.x, transform.p.y, transform.p.z - 125 );
    mesh.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    mesh.scale.set( transform.s.x, transform.s.y, transform.s.z );
    parent.add( mesh );

    // 3d shape

    var geometry = new THREE.ExtrudeGeometry( shape, extrudeSettings );

    var mesh = THREE.SceneUtils.createMultiMaterialObject( geometry, [ new THREE.MeshLambertMaterial( { color: color } ), new THREE.MeshBasicMaterial( { color: 0x000000, wireframe: true, transparent: true } ) ] );
    mesh.position.set( transform.p.x, transform.p.y, transform.p.z - 75 );
    mesh.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    mesh.scale.set( transform.s.x, transform.s.y, transform.s.z );
    parent.add( mesh );

    // solid line

    var line = new THREE.Line( pts, new THREE.LineBasicMaterial( { color: extrudeSettings.color, linewidth: 2 } ) );
    line.position.set( transform.p.x, transform.p.y, transform.p.z + 25 );
    line.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    line.scale.set( transform.s.x, transform.s.y, transform.s.z );
    parent.add( line );

    // transparent line from real points

    var line = new THREE.Line( points, new THREE.LineBasicMaterial( { color: color, opacity: 0.5 } ) );
    line.position.set( transform.p.x, transform.p.y, transform.p.z + 75 );
    line.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    line.scale.set( transform.s.x, transform.s.y, transform.s.z );
    parent.add( line );

    // vertices from real points

    var pgeo = THREE.GeometryUtils.clone( points );
    var particles = new THREE.ParticleSystem( pgeo, new THREE.ParticleBasicMaterial( { color: color, size: 2, opacity: 0.75 } ) );
    particles.position.set( transform.p.x, transform.p.y, transform.p.z + 75 );
    particles.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    particles.scale.set( transform.s.x, transform.s.y, transform.s.z );
    parent.add( particles );

    // transparent line from equidistance sampled points

    var line = new THREE.Line( spacedPoints, new THREE.LineBasicMaterial( { color: color, opacity: 0.2 } ) );
    line.position.set( transform.p.x, transform.p.y, transform.p.z + 125 );
    line.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    line.scale.set( transform.s, transform.s, transform.s );
    parent.add( line );

    // equidistance sampled points

    var pgeo = THREE.GeometryUtils.clone( spacedPoints );
    var particles2 = new THREE.ParticleSystem( pgeo, new THREE.ParticleBasicMaterial( { color: color, size: 2, opacity: 0.5 } ) );
    particles2.position.set( transform.p.x, transform.p.y, transform.p.z + 125 );
    particles2.rotation.set( transform.r.x, transform.r.y, transform.r.z );
    particles2.scale.set( transform.s.x, transform.s.y, transform.s.z );
    parent.add( particles2 );*/

}
