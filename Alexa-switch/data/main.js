
var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);

connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
    console.log('Server: ', e.data);
};
connection.onclose = function(){
    console.log('WebSocket connection closed');
};

function Save_Eeprom() {
  var Debug = document.getElementById("debug");
  var Ssid = document.getElementById("ssid").value;
  var Pw = document.getElementById("pw").value;
  var Name = document.getElementById("name").value;

  var Setup = "ES*" + Ssid + "*" + Pw + "*" + Name + "**";
  Debug.innerHTML = Setup;
  connection.send(Setup);
}