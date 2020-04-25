
(function(){

  function ledOff(){
    console.log("button off");
    var xmlReq = new XMLHttpRequest();
    xmlReq.open("GET","/ledstate?st=off",true);
    xmlReq.send();
  }

  function ledOn(){
    console.log("button on");
    var xmlReq = new XMLHttpRequest();
    xmlReq.open("GET","/ledstate?st=on",true);
    xmlReq.send();
  }

  function getStatus(){
  
  }

  window.onload = function() {
    var button1 = document.getElementById("button1");
    var button2 = document.getElementById("button2");
    // attach button events
    button1.onclick = function() {
      ledOn();
    }

    button2.onclick = function() {
      ledOff();
    }
  }

})();
