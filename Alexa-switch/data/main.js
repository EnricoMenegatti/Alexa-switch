
function Startup()
{
    Reset();
    Ripristino();
}

function Reset() 
{
    var x = document.getElementById("checkReset").checked;
    document.getElementById("reset").disabled = !x;
}

function Ripristino() 
{
    var x = document.getElementById("checkRipr").checked;
    document.getElementById("ripristino").disabled = !x;
}