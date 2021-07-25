function getValues() {
    if(document.getElementById("light1").innerHTML == "encendida"){
      $("#2").attr("checked", "checked");
  }
    if(document.getElementById("light2").innerHTML == "encendida"){
      $("#23").attr("checked", "checked");
  }
    if(document.getElementById("light3").innerHTML == "encendida"){
      $("#22").attr("checked", "checked");
  }
};

function setLight(element) {
    var xhr = new XMLHttpRequest();
    
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if (element.id == 2){
                document.getElementById("light1").innerHTML = this.responseText;
            }
            else if (element.id == 23){
                document.getElementById("light2").innerHTML = this.responseText;
            }
            else if (element.id == 22){
                document.getElementById("light3").innerHTML = this.responseText;
            } 
        }
      };
    
    xhr.open('POST', "/postupdate", true);
    xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    
    var params;
    if(element.checked){ params = "output="+element.id+"&state=1"; }
    else { params = "output="+element.id+"&state=0"; }
    
    xhr.send(params);
}
