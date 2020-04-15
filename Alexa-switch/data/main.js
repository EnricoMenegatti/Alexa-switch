
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

function Func_mod() {
    var Btn = document.getElementById("Btn_modalita");
    var Mod_S = document.getElementById("Semplice");
    var Mod_A = document.getElementById("Avanzata");

    if(Mod_A.style.display === "inline")
    {
      Mod_A.style.display = "none";
      Btn.innerHTML = "Semplice";
      Btn.style.color = "#000000";
      Btn.style.backgroundColor = "#FFFFFF";

    }
    else
    {
      Mod_A.style.display = "inline";
      Btn.innerHTML = "Avanzata";
      Btn.style.color = "#FFFFFF";
      Btn.style.backgroundColor = "#00857C";
    }
}

function Cambia_pagina(page) {
    var Debug = document.getElementById("debug");
    Debug.innerHTML = page;
    connection.send(page);
}

function Send_Shoot(ind) {
    var Debug = document.getElementById("debug");
    var T_focus = document.getElementById('t_fuoco').value;
    var T_shoot = document.getElementById('t_scatto').value;
    var Time_f = document.getElementById('tempo_f').value;
    var Time_s = document.getElementById('tempo_s').value;

    var head_1 = "F";
    var head_2 = "S";

    if(T_focus > 9999)
    {
      T_focus.value = 9999;
    }

    if(T_shoot > 9999)
    {
      T_shoot.value = 9999;
    }

    if(ind == "0") //FS
    {
      var bottom_1 = Time_f;
      var bottom_2 = Time_s;

      if(1 <= T_focus && T_focus <= 9)//Millisecondi
      {
        var center_1 = T_focus;
        var linee_1 = "000";
      }
      else if(10 <= T_focus && T_focus <= 99)//Secondi
      {
        var center_1 = T_focus;
        var linee_1 = "00";
      }
      else if(100 <= T_focus && T_focus <= 999)//Minuti
      {
        var center_1 = T_focus;
        var linee_1 = "0";
      }
      else if(1000 <= T_focus && T_focus <= 9999)//Minuti
      {
        var center_1 = T_focus;
        var linee_1 = "";
      }
      else
      {
        var center_1 = "";
        var linee_1 = "1000";
      }


      if(1 <= T_shoot && T_shoot <= 9)//Millisecondi
      {
        var center_2 = T_shoot;
        var linee_2 = "000";
      }
      else if(10 <= T_shoot && T_shoot <= 99)//Secondi
      {
        var center_2 = T_shoot;
        var linee_2 = "00";
      }
      else if(100 <= T_shoot && T_shoot <= 999)//Minuti
      {
        var center_2 = T_shoot;
        var linee_2 = "0";
      }
      else if(1000 <= T_shoot && T_shoot <= 9999)//Minuti
      {
        var center_2 = T_shoot;
        var linee_2 = "";
      }
      else
      {
        var center_2 = "";
        var linee_2 = "0100";
      }
    }


    else if(ind == "1") //FF
    {
      var bottom_1 = Time_f;
      var center_2 = "0000";
      var linee_2 = "";
      var bottom_2 = Time_s;

      if(1 <= T_focus && T_focus <= 9)//Millisecondi
      {
        var center_1 = T_focus;
        var linee_1 = "000";
      }
      else if(10 <= T_focus && T_focus <= 99)//Secondi
      {
        var center_1 = T_focus;
        var linee_1 = "00";
      }
      else if(100 <= T_focus && T_focus <= 999)//Minuti
      {
        var center_1 = T_focus;
        var linee_1 = "0";
      }
      else if(1000 <= T_focus && T_focus <= 9999)//Minuti
      {
        var center_1 = T_focus;
        var linee_1 = "";
      }
      else
      {
        var center_1 = "";
        var linee_1 = "1000";
      }

    }


    else if(ind == "2") //SS
    {
      var center_1 = "0000";
      var linee_1 = "";
      var bottom_1 = Time_f;
      var bottom_2 = Time_s;

      if(1 <= T_shoot && T_shoot <= 9)//Millisecondi
      {
        var center_2 = T_shoot;
        var linee_2 = "000";
      }
      else if(10 <= T_shoot && T_shoot <= 99)//Secondi
      {
        var center_2 = T_shoot;
        var linee_2 = "00";
      }
      else if(100 <= T_shoot && T_shoot <= 999)//Minuti
      {
        var center_2 = T_shoot;
        var linee_2 = "0";
      }
      else if(1000 <= T_shoot && T_shoot <= 9999)//Minuti
      {
        var center_2 = T_shoot;
        var linee_2 = "";
      }
      else
      {
        var center_2 = "";
        var linee_2 = "0100";
      }
    }

    var Tempo_finale = head_1 + linee_1 + center_1 + bottom_1 + head_2 + linee_2 + center_2 + bottom_2 + "*";

    Debug.innerHTML = Tempo_finale;
    connection.send(Tempo_finale);
}
