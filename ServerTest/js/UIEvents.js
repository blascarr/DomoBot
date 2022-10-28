bindUIEvents = () => {
    $('#toggle_retain')[0].checked = false;
    $('#toggle_control')[0].checked = true;
    $('#toggle_retain').click(
        function() {
            HTTPRequest( domo_endpoint , domo_input_endpoint,  {"auto":  $('#toggle_retain')[0].checked }  );
        }
    );
    $('#toggle_control').click(
        function() {
            HTTPRequest( domo_endpoint , domo_input_endpoint,  {"movemode":  $('#toggle_control')[0].checked }  );
        }
    );
}


$(function () {
    setTimeout(() => {
        console.log("Bind Ready");
        bindUIEvents ();
    }, 1000)
})