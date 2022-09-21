bindUIEvents = () => {
    console.log( $('#toggle_retain').checked );
    $('toggle_retain').click(
        function() {
            HTTPRequest( "domomode" , "UIController", JSON.stringify( {"toggle":  $('#toggle_retain').checked}  ));
        }
    );
}


$(function () {
    setTimeout(() => {
        console.log("Bind Ready");
        bindUIEvents ();
    }, 2000)
})