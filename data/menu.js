window.onload = function() {
    
    var client = new XMLHttpRequest();
    client.open('GET', 'menu.html');
    client.onreadystatechange = function() {
        document.getElementById("menu").innerHTML = client.responseText;
    }
    client.send();
    
    if (typeof getValues === "function") { 
        getValues();
    }
    
};
