window.onload = function() {
    
    var menu = "<div class='btn-group btn-group-lg row' role='group'>"
    +
    "<a href='index.html' class='btn btn-danger px-5' role='button'>Inicio</a>"
    +
    "<div class='btn-group' role='group'>"
    +
        "<button id='btnMonitorDrop' class='btn btn-lg btn-success px-5 dropdown-toggle' data-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>Monitoreo</button>"
    +
        "<div class='dropdown-menu' aria-labelledby='btnMonitorDrop' style='background-color: whitesmoke'>"
    +
          "<a class='dropdown-item' href='wheater.html'>Clima</a>"
    +
          "<a class='dropdown-item' href='#'>Contanimación</a>"
          +
        "</div>"
    +
        "</div>" 
    +
        "<div class='btn-group' role='group'>"
    +
        "<button id='btnControlDrop' class='btn btn-lg btn-primary px-5 dropdown-toggle' data-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>Control</button>"
    +
        "<div class='dropdown-menu' aria-labelledby='btnControlDrop' style='background-color: whitesmoke'>"
    +
           "<a href='light.html' class='dropdown-item'>Iluminación</a>"
    +
           "<a href='#' class='dropdown-item'>Ventilación</a>"
    +
        "</div>"
    +
        "</div>"
    + 
        "<a href='about.html' class='btn btn-secondary px-5'>Acerca de</a>"
    +
    "</div>";
    
    document.getElementById("menu").innerHTML = menu;
    
    if (typeof getValues === "function") { 
        getValues();
    }
    
};
