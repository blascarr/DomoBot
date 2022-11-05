/*      GUI in THREE JS 
*       https://lil-gui.georgealways.com/
*/

const GUI = lil.GUI;
const gui = new GUI();
const current_shape= { Shape: 'Robot' };
const shapes = [ 'Cone', 'Robot', 'Model' ];

//--------------------- BOT EVENTS -------------------------//
reset_bot = ( )=>{
    console.log("Reset Bot");
    const dataJSON = {};
    // Send RESET BOT
    HTTPRequest( domo_endpoint, domo_input_endpoint , dataJSON );
}

reset_origin = ( )=>{
    console.log("Reset Origin");
    const dataJSON = {};
    // Send RESET ORIGIN
    HTTPRequest( domo_endpoint, domo_input_endpoint , dataJSON );
}

visible_origin = ( evt )=>{
    console.log("Visible Target");
    bot_origin.visible = evt;
    origin_transformControl.visible = evt;
}

//--------------------- TARGET EVENTS -------------------------//
visible_target = ( evt )=>{
    console.log("Visible Target");
    target.visible = evt;
    target_transformControl.visible = evt;
}

draggable_target = ( evt )=>{
    console.log("Draggable Target");
    console.log(target.position);
    const dataJSON = {};
    // Send TARGET
    HTTPRequest( domo_endpoint, domo_input_endpoint , dataJSON );
}

//--------------------- SHAPE EVENTS -------------------------//
changeShape = ( evt )=>{
    console.log("Change Shape");
}

//--------------------- OPT EVENTS -------------------------//
visible_OPT = ( )=>{
    console.log("Visible OPT");
}

OPT_timer = ( evt )=>{
    console.log("Timer in OPT");
    console.log(evt);
}

//--------------------- TRAJECTORY EVENTS -------------------------//
visible_trajectory = ( )=>{
    console.log("Visible Trajectory");
}

Trajectory_timer = ( evt )=>{
    console.log("Timer in Trajectory");
    console.log(evt);
}

//--------------------- MAP EVENTS -------------------------//
visible_map = ( )=>{
    console.log("Visible Map");
}

load_makerspace_map = ( evt )=>{
    console.log("Load Makerspace");
    console.log(evt);
}

//--------------------- OBSTACLE EVENTS -------------------------//
visible_obstacles = ( )=>{
    console.log("Visible Obstacles");
}

send_obstacles = ( evt )=>{
    console.log("Send Obstacles");
    console.log(evt);
}


const GUI_model =[
    {
        "folder_name": 'Bot Controller',
        "options" :[
            {
                ref : 'input',
                input: true,
                onChange : visible_origin,
                name : 'Visible Origin'
            },
            {
                ref : 'input',
                input : reset_bot,
                name : 'Reset Bot'
            },
            {
                ref : 'input',
                input : reset_origin,
                name : 'Reset Origin'
            }
            
        ],
        "isOpen": false    
    },
    {
        "folder_name": 'Draggable Cube',
        "options" :[
            {
                ref : 'input',
                input: true,
                onChange : visible_target,
                name : 'Visible Target'
            },
            {
                ref : 'input',
                input : draggable_target,
                name : 'Send Target'
            }
            
        ],
        "isOpen": false    
    },
    {
        "folder_name": 'Shapes',
        "options" :[
            {
                ref : 'input',
                input: shapes,
                onChange : changeShape,
                name : 'Shape Selector',
                current : current_shape
            }
            
        ],
        "isOpen": false    
    },
    {
        "folder_name": 'Load Map',
        "options" :[
            {
                ref : 'input',
                input: true,
                onChange : visible_map,
                name : 'Visible Map'
            },
            {
                ref : 'input',
                input : load_makerspace_map,
                name : 'Load Map'
            }
            
        ],
        "isOpen": false    
    },
    {
        "folder_name": 'Load Obstacles',
        "options" :[
            {
                ref : 'input',
                input: true,
                onChange : visible_obstacles,
                name : 'Visible Obstacles'
            },
            {
                ref : 'input',
                input : send_obstacles,
                name : 'Send Obstacles'
            }
            
        ],
        "isOpen": false    
    },
    {
        "folder_name": 'OPT Points',
        "options" :[
            {
                ref : 'input',
                input: true,
                onChange : visible_OPT ,
                name : 'Visible OPT'
            },
            {
                ref : 'input',
                input: 200,
                onChange : OPT_timer,
                name : 'OPT_Timer',
                min : 100,
                max : 1000,
                inc : 1
            }
        ],
        "isOpen": false
    },
    {
        "folder_name": 'Trajectory Drawing',
        "options" :[
            {
                ref : 'input',
                input: true,
                onChange : visible_trajectory ,
                name : 'Visible'
            },
            {
                ref : 'input',
                input: 200,
                onChange : Trajectory_timer,
                name : 'TRJ_Timer',
                min : 100,
                max : 1000,
                inc : 1
            }
        ],
        "isOpen": false
    }
]


addGUI = () => {
    GUI_model.forEach( el => {
        const folder = gui.addFolder( el.folder_name );
        folder.open(el.isOpen);
        el.options.forEach( opt => {
            switch (typeof opt.input) {
                case 'number':
                    const slider_controller = folder.add( opt, opt.ref , opt.min, opt.max, opt.inc ).name( opt.name );
                    slider_controller.onChange( opt.onChange );
                return;
                case 'object':
                    const array_controller = folder.add( opt.current, 'Shape').options( opt.input ).name( opt.name );
                    array_controller.onChange( opt.onChange );
                return;
                default:
                    const input_controller = folder.add( opt, opt.ref ).name( opt.name );
                    input_controller.onChange( opt.onChange );
                return;
            }
        })
    });
}

addGUI();