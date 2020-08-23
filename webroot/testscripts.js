
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
    var xmlReq = new XMLHttpRequest();
    xmlReq.open("GET","/ledstate",true);
    xmlReq.onreadystatechange = function(){
      if(this.readyState == 4 && this.status == 200){
        var retData = xmlReq.responseText;
        //debugger;
      }
    }
    xmlReq.send();
  }

  function sendPost(data){
    var xmlReq = new XMLHttpRequest();
    xmlReq.open("POST","/posttest?data=hello",true);
    xmlReq.onreadystatechange = function(){
      if(this.readyState == 4 && this.status == 200){
        var retData = xmlReq.responseText;
        //debugger;
      }
    }
    if(data){
    xmlReq.send(data);
    } else {
      xmlReq.send();
    }
  }

  function getSwitch(){
    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');
    ctx.fillStyle = 'green';
    var xmlReq = new XMLHttpRequest();
    xmlReq.open("GET","/switchstate",true);
    xmlReq.onreadystatechange = function(){
      if(this.readyState == 4 && this.status == 200){
        var retData = xmlReq.responseText;
        if(retData == "high"){
              ctx.fillStyle = 'green';
              ctx.fillRect(10, 10, 150, 100);
        } else if(retData == "low"){
              ctx.fillStyle = 'blue';
              ctx.fillRect(10, 10, 150, 100);
        }
      }
    }
    xmlReq.send(); 
  }

  window.onload = function() {
    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');

    ctx.fillStyle = 'green';
    ctx.fillRect(10, 10, 150, 100);

    getStatus();

    var button1 = document.getElementById("button1");
    var button2 = document.getElementById("button2");
    var postbutton = document.getElementById("postbutton");
    // attach button events
    button1.onclick = function() {
      ledOn();
    }

    button2.onclick = function() {
      ledOff();
    }

    postbutton.onclick = function() {
      sendPost();
    }

    //window.setInterval(getSwitch,100) // lol don't run unless you mean it!

  }

})();
