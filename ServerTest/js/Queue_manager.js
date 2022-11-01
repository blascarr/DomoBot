class Queue {
    constructor( timeout = 50) {
        this.elements = [];
        this.pointer = 0;
        this.timeout = timeout;
    }
    enqueue(element) {
        this.elements.push( element );
    }
    dequeue() {
        this.elements.shift();
        return item;
    }

    dequeue( element ) {
        const result = this.elements.filter( (el, index) => el !== element );
        this.elements = result;
    }

    peek() {
        return this.elements[this.pointer];
    }
    get length() {
        return this.elements.length;
    }
    get isEmpty() {
        return this.elements.length === 0;
    }
}

headerAddScripts = ()=> {
    if (!scriptHeadQueue.isEmpty){
        addScript( scriptHeadQueue.peek(), scriptHeadQueue );
    }
}

init = async()=>{
    console.log("Loader Manager is working");
    let resources = await rscManager();

    let headerScriptsTimer = setInterval( headerAddScripts, scriptHeadQueue.timeout);

    //Load scripts
    setTimeout(async function(){
        console.log("Stop Timeout");
        await window.clearInterval(headerScriptsTimer);
        // console.log("Loader Manager is working");
    }, loadTime );

}

rscManager = async ()=>{
    await fetch(remote_rsc)
        .then(res => res.json())
        .then(async out =>{
            out.links.forEach( el =>{
                cssHeadQueue.enqueue(el);
            });
            sourceManager( out );

            out.scripts.forEach( el =>{
                scriptHeadQueue.enqueue(el);
            });
            return out ;
        })
        .catch(err => console.log('Error:', err));
}

sourceManager = async ( sources ) =>{
    sources.links.forEach( el =>{
        addCSS(el, cssHeadQueue );
    })
    config_json = sources.config_json;
    config_shape = sources.config_shape;
}

addScript = async( script, scriptQueue )=>{
    await checkResource( script, scriptQueue, appendScript );
}

addCSS = ( CSSFileName, cssQueue )=>{
    checkResource( CSSFileName, cssQueue, appendCSS );
}

checkResource = async( resource, queue, fn )=>{
    await fetch( resource.link , { method: 'HEAD', cache: 'reload' })
        .then(res => {
            if (res.ok) {
                
                console.log('Resource Exist -'+resource.priority+' - '+ resource.link  + ' Status : '+ res.status);
                const rscToLoad = fn( resource.link );
                document.head.append(rscToLoad);
                queue.dequeue(resource);
            } else {
                console.log('Resource does not exist - '+ resource + ' Status Error: '+ res.status+ ' ResOK : '+ res.ok);
            }
        }).catch(err => console.log('Error:', err));
}

appendScript = ( srcFileName ) =>{
    const scriptToLoad = document.createElement('script');
    scriptToLoad.src = srcFileName;
    return scriptToLoad;
}

appendCSS = ( srcFileName ) =>{
    const linkToLoad = document.createElement('link');
    linkToLoad.rel = 'stylesheet';
    linkToLoad.type = 'text/css';
    linkToLoad.href = srcFileName;
    return linkToLoad;
}

scriptHeadQueue = new Queue( 300 );
cssHeadQueue = new Queue( 100 );
init();