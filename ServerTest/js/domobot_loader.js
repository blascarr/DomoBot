let elDebug;
let joystick;
let dumpContainer;
let els;
let joysticks ;
var nbEvents = 0;

// Mapping Keys
const keys = {
    "x_pos_data":"position.x",
    "y_pos_data":"position.y",
    "rad_angle_data":"angle.radian",
    "deg_angle_data":"angle.degree",
    "force":"force",
    "pressure":"pressure",
    "distance":"distance",
    "x_dir_data":"direction.x",
    "y_dir_data":"direction.y",
    "angle_dir_data":"direction.angle"
}

$(function () {
    $.getJSON( domobot_json , function(data) {
        datainputs = data.inputs;
        datamodel = data.model;
        jsonData = Object.assign(datamodel, datainputs);
        template = data.template;
        $("body").json2html(jsonData,template);

    } ).done(function() {
        
        dumpContainer = document.getElementById('dump');

        elDebug = document.getElementById('dump');

        els = {
            position: {
                x: elDebug.querySelector('.position .x .data'),
                y: elDebug.querySelector('.position .y .data')
            },
            force: elDebug.querySelector('.force .data'),
            pressure: elDebug.querySelector('.pressure .data'),
            distance: elDebug.querySelector('.distance .data'),
            angle: {
                radian: elDebug.querySelector('.angle .radian .data'),
                degree: elDebug.querySelector('.angle .degree .data')
            },
            direction: {
                x: elDebug.querySelector('.direction .x .data'),
                y: elDebug.querySelector('.direction .y .data'),
                angle: elDebug.querySelector('.direction .angle .data')
            }
        };
        
        joysticks = {
            static: {
                zone: document.querySelector('.zone.static'),
                mode: 'static',
                position: {
                left: '15%',
                top: '15%'
                },
                color: 'blue'
            }
        };

        createNipple('static'); 

    }).fail(function() {
        console.log( "error getting JSON" );
    });
});

// Dump data
dump = (obj) => {
    // console.log( obj );
    for (var el in keys) {
        const attparser =  keys[el].split(".");
        if( attparser.length > 1 ){
            const parseElement = parser(attparser, obj);
            if( parseElement ){
                const element = domget(el).innerHTML = parseElement;
            }
        }else{
            if (typeof (obj[keys[el]]) == "number"){
                domget(el).innerHTML = obj[keys[el]].toFixed(2);
            }else{
                domget(el).innerHTML = obj[keys[el]];
            }
        }  
    }
}

parser = ( att_array, obj )=>{
    
    if( att_array.length > 1 ){
        const att = att_array[0];
        att_array.shift();
        return parser( att_array, obj[att]);
    }else{
        if( !obj ){
            //console.log("OBJ Undefined");
            return "";
        }
        if (typeof (obj[att_array]) == "number"){
            return obj[att_array].toFixed(2)
        }
        return obj[att_array];
    }
}

bindNipple = () => {
    joystick.on('start end', function(evt, data) {
        // Data mapping is different From move event
    }).on('move', function(evt, data) {
        domoData( data);
        dump(data);
    }).on('dir:up plain:up dir:left plain:left dir:down ' +
            'plain:down dir:right plain:right',
            function(evt, data) {
    }
    ).on('pressure', function(evt, data) {
        console.log(data);
    }).on('end', function(evt, data) {
        console.log("STOP");
        stopBot();
    })
}

createNipple = (evt) => {
    var type = typeof evt === 'string' ?
        evt : evt.target.getAttribute('data-type');
    joystick = nipplejs.create(joysticks['static']);
    bindNipple();
}

domlabelvalue = (id, label) => {
    domget(id).innerHTML = domvalue(label);
}

domvalue = (dom) => {
    const v = domget(dom).value;
    if (parseInt(v) > 0) return Number(v);
    return v;
}

domget = (dom) => {
    return document.getElementById(dom);
}

domoData = ( data ) => {
    const [toggle] = $('#toggle_retain');
    let controlmode = ""; 
    if ( toggle.checked ){
        controlmode = "joystick"
    }else{
        controlmode = "move"
    }
    let dataJSON = { power: data.distance, angle: data.angle, direction: data.direction, mode: controlmode };
    HTTPRequest( "domobot", "botData" , dataJSON );
}

stopBot = () =>{
    const dataJSON = { event: "STOP" };
    HTTPRequest( "domobot", "botData" , dataJSON );
}

HTTPRequest = ( endpoint, data_endpoint , dataJSON , method = "GET" )=>{
    const xhr = new XMLHttpRequest();
    
    xhr.onload = () => {

        if (xhr.status == 200) {
            try{ 

                console.log(JSON.parse(xhr.response));
            }catch(e) { 
                // Get OK response which is not parseable
                // console.log(xhr.response);
            }
        } else {
            console.warn('No request processed!', xhr.status);
        }
    };
    xhr.open( method, endpoint+"?"+data_endpoint+"=" + JSON.stringify(dataJSON), true);
    xhr.send();
}