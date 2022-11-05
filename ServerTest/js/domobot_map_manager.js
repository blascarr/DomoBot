/*
    Project created by Blascarr from ZMS
    - Loader Scripts
    - Script and CSS Manager
    
    DOMOBOT MAP MANAGER
*/

const remote_rsc = "js/src.config.json";

const queue_rsc = "js/Queue_manager.js";
let config_json ;
let config_shape ;
let bot_shape ;
const loadTime = 7000;
let scriptHeadQueue;
let cssHeadQueue;

fetch( queue_rsc , { method: 'HEAD', cache: 'reload' })
    .then(async res => {
        const scriptToLoad = document.createElement('script');
        scriptToLoad.src = queue_rsc;
        document.head.append(scriptToLoad);
})
.catch(err => console.log('Error:', err));
