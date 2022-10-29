const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 2.0rem; }
      p { font-size: 2.0rem; }
      .units { font-size: 1.2rem; }
      .dht-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 10px;
      }
    </style>
  </head>
  <body>
    <h1></h1>
    <p>
      <span class="dht-labels">Dust Density : </span> 
      <span id="DustD" style="color:#ffaa56;">0</span>
     
    </p>

    <p>
      <span class="dht-labels">Soil Mosture : </span> 
      <span id="SoilM" style="color:#ffaa56;">0</span>
      
    </p>

    <p>
      <span class="dht-labels">Water Level : </span> 
      <span id="WaterL" style="color:#ffaa56;">0</span>
    </p>

    <p>
      <span class="dht-labels">Weather Temperature : </span> 
      <span id="WTemperatureValue" style="color:#ffaa56;">0</span>
      <sup class="units">&deg;C</sup>
    </p>

    <p>
      <span class="dht-labels">Weather Humidity : </span>
      <span id="WHumidityValue" style="color:#4da5fc;">0</span>
      <sup class="units">%</sup>
    </p>
    <p>
      <span class="dht-labels">Soil Temperature : </span> 
      <span id="STemperatureValue" style="color:#ffaa56;">0</span>
      <sup class="units">&deg;C</sup>
    </p>

    <p>

      <span style="font-size:1.0rem;">Time </span>
      <span id="time" style="font-size:1.0rem;"></span>
    
    <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAACXBIWXMAAAsTAAALEwEAmpwYAAAA1ElEQVR4nO3bwQqCQBgA4TZ6Rh/Sl6y7QiIlX+nMUXEZhnUPPzpuGzyn+fnu/pinsbXGkXzqd/+uzv9RAC2gGVvv0Nm5/A4ogBbQFEALaAqgBTQF0AKaAmgBTQG0gKYAWkDzWF7QM76jWc4/Lr8DCqAFNKszYIu9M8TlmaKfX3L5HVAALaApgBbQFEALaAqgBTQF0AKa1fcBzQMuRgG0gKZ5wJ7FzkgBtICmAFpAUwAtoCmAFtAUQAtomgcokV+hAFpAUwAtoCmAFtD036AW0BRAC2heXrU4ekB4r6wAAAAASUVORK5CYII=" width="20" height="20" align="middle" style="padding-bottom: 12px;">
      <span style="font-size:1.0rem;">Date </span>
      <span id="date" style="font-size:1.0rem;"></span>
    </P>
  <p>
   
  </P>
    <script>
      setInterval(function() {
        // Call a function repetatively with 2 Second interval
        getWTemperatureData();
        getWHumidityData();
        getDD();
        getSM();
        getWL() 
        getST()
      }, 5000); 

      setInterval(function() {
        // Call a function repetatively with 1 Second interval
        Time_Date();
      }, 1000); 

      function getWTemperatureData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("WTemperatureValue").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "readWeatherTemp", true);
        xhttp.send();
      }

      function getWHumidityData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("WHumidityValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readWeatherHum", true);
        xhttp.send();
      }
      function getDD() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("DustD").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readDustDensity", true);
        xhttp.send();
      }

       function getSM() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("SoilM").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readSoilMost", true);
        xhttp.send();
      }
       function getWL() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("WaterL").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readWaterLevel", true);
        xhttp.send();
      }
      function getST() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("STemperatureValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readSoilTemp", true);
        xhttp.send();
      }

      function Time_Date() {
        var t = new Date();
        document.getElementById("time").innerHTML = t.toLocaleTimeString();
        var d = new Date();
        const dayNames = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday","Saturday"];
        const monthNames = ["January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"];
        document.getElementById("date").innerHTML = dayNames[d.getDay()] + ", " + d.getDate() + "-" + monthNames[d.getMonth()] + "-" + d.getFullYear();
      }
    </script>
  </body>
</html>
)=====";