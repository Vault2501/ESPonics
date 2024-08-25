const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESPonics</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      margin: 0;
    }
    html {
      font-family: Arial, Helvetica, sans-serif;
      text-align: center;
    }
    h1 {
      font-size: 1.8rem;
      color: white;
    }
    h2{
      font-size: 1.5rem;
      font-weight: bold;
      color: #143642;
    }
    .topnav {
      overflow: hidden;
      background-color: #143642;
    }
    .tab button {
    background-color: inherit;
    float: left;
    border: none;
    outline: none;
    cursor: pointer;
    padding: 14px 16px;
    transition: 0.3s;
    font-size: 17px;
    }
    .tab button:hover {
    background-color: #ddd;
    }
    .tab button.active {
    background-color: #ccc;
    }
    .tabcontent {
    display: none;
    padding: 6px 12px;
    border: 1px solid #ccc;
    border-top: none;
    }
    .tab {
    overflow: hidden;
    border: 1px solid #ccc;
    background-color: #f1f1f1;
    }
    .card {
      background-color: #F8F7F9;;
      box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
      padding-top:10px;
      padding-bottom:20px;
    }
    .button {
      padding: 15px 50px;
      font-size: 24px;
      text-align: center;
      outline: none;
      color: #fff;
      background-color: #0f8b8d;
      border: none;
      border-radius: 5px;
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      -khtml-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
      user-select: none;
      -webkit-tap-highlight-color: rgba(0,0,0,0);
    }
    .button:active {
      background-color: #0f8b8d;
      box-shadow: 2 2px #CDCDCD;
      transform: translateY(2px);
    }
    .pump1_state {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .pump2_state {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .valve1_state {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .valve2_state {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .flow_rate {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .flow_quantity {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .spray_period {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .spray_duration {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .fan1_speed {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .dhtValueTemp {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .dallasValueTemp {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .dhtValueHumidity {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .water_state {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .ph_value {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .ph_analog {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .ph_calibrated {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .tds_value {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .tds_analog {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    .tds_calibrated {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
  </style>
<title>ESPonics</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  
  <div class="topnav">
    <h1>ESPonics</h1>
  </div>

  <div class="tab">
    <button class="tablinks" onclick="openTab(event, 'Pumps')">Pumps</button>
    <button class="tablinks" onclick="openTab(event, 'Sensors')">Sensors</button>
    <button class="tablinks" onclick="openTab(event, 'FansLight')">Fans and Light</button>
    <button class="tablinks" onclick="openTab(event, 'Scheduler')">Scheduler</button>
    <button class="tablinks" onclick="openTab(event, 'Calibrate')">Calibrate</button>
    <button class="">
  </div>
  
  <div id="Pumps" class="tabcontent">
    <div class="card">
      <h2>Pump 1</h2>
      <p class="pump1_state">State: <span id="pump1_state">%PUMP1_STATE%</span></p>
      <p><button id="pump1_button" class="button">Toggle</button></p>

      <h2>Pump 2</h2>
      <p class="pump2_state">State: <span id="pump2_state">%PUMP2_STATE%</span></p>
      <p><button id="pump2_button" class="button">Toggle</button></p>

      <h2>Valve 1</h2>
      <p class="valve1_state">State: <span id="valve1_state">%VALVE1_STATE%</span></p>
      <p><button id="valve1_button" class="button">Toggle</button></p>

      <h2>Valve 2</h2>
      <p class="valve2_state">State: <span id="valve2_state">%VALVE2_STATE%</span></p>
      <p><button id="valve2_button" class="button">Toggle</button></p>

      <h2>Flow Meter</h2>
      <p class="flow_rate">Flow rate (L/m): <span id="flow_rate">%FLOW_RATE%</span></p>
      <p class="flow_quantity">Flow Quantity Total (ml): <span id="flow_quantity">%FLOW_QUANTITY%</span></p>
    </div>
  </div>

  <div id="Sensors" class="tabcontent">
    <div class="card">
      <h2>DHT Temperature</h2>
      <p class="dhtValueTemp">State: <span id="dhtValueTemp">%DHT_TEMP_VALUE%</span></p>

      <h2>DHT Humidity</h2>
      <p class="dhtValueHumidity">State: <span id="dhtValueHumidity">%DHT_HUMIDITY_VALUE%</span></p>

      <h2>Dallas Temperature</h2>
      <p class="dallasValueTemp">State: <span id="dallasValueTemp">%DALLAS_TEMP_VALUE%</span></p>

      <h2>ph Value</h2>
      <p class="ph_value">State: <span id="ph_value">%PH_VALUE%</span></p>

      <h2>tds Value</h2>
      <p class="tds_value">State: <span id="tds_value">%TDS_VALUE%</span></p>

      <h2>Water State</h2>
      <p class="water_state">State: <span id="water_state">%WATER_STATE%</span></p>
    </div>
  </div>

  <div id="FansLight" class="tabcontent">
    <div class="card">
      <h2>Fan 1</h2>
      <p class="fan1_state">State: <span id="fan1_state">%FAN1_STATE%</span></p>
      <p><button id="fan1_button" class="button">Toggle</button></p>

      <h2>Fan 1 Speed</h2>
      <p class="fan1_speed">
        Fan 1 Speed (0-100): <span id="fan1_speed">%FAN1_SPEED%</span><br>
        New value: <input type="text" id="new_fan1_speed" value="%FAN1_SPEED%">
        <button id="update_fan1_speed_button" class="button">Update</button>
      </p>

      <h2>Fan 2</h2>
      <p class="fan2_state">State: <span id="fan2_state">%FAN2_STATE%</span></p>
      <p><button id="fan2_button" class="button">Toggle</button></p>

      <h2>Light</h2>
      <p class="light_state">State: <span id="light_state">%LIGHT_STATE%</span></p>
      <p><button id="light_button" class="button">Toggle</button></p>

      <h2>Light Times (h)</h2>
      <p class="light_on">
        Light On time(h): <span id="light_on">%LIGHT_ON%</span><br>
        New value: <input type="text" id="new_light_on" value="%LIGHT_ON%">
        <button id="update_light_on_button" class="button">Update</button>
      </p>
  
      <p class="light_off">
        Spray Off time(h): <span id="light_off">%LIGHT_OFF%</span><br>
        New value: <input type="text" id="new_light_off" value="%LIGHT_OFF%">
        <button id="update_light_off_button" class="button">Update</button>
      </p>
    </div>
  </div>

  <div id="Scheduler" class="tabcontent">
    <div class="card">

      <h2>Scheduler</h2>
      <p class="scheduler_active">State: <span id="scheduler_active">%SCHEDULER_ACTIVE%</span></p>
      <p><button id="scheduler_button" class="button">Toggle</button></p>

      <h2>Spray Period</h2>
      <p class="spray_period">
        Spray period (ms): <span id="spray_period">%SPRAY_PERIOD%</span><br>
        New value: <input type="text" id="new_spray_period" value="%SPRAY_PERIOD%">
        <button id="update_spray_period_button" class="button">Update</button>
      </p>
      <p class="spray_duration">
        Spray duration (ms): <span id="spray_duration">%SPRAY_DURATION%</span><br>
        New value: <input type="text" id="new_spray_duration" value="%SPRAY_DURATION%">
        <button id="update_spray_duration_button" class="button">Update</button>
      </p>
    </div>
  </div>

  <div id="Calibrate" class="tabcontent">
    <div class="card">    
      <p class="ph_calibrated">State: <span id="ph_calibrated">%PH_CALIBRATED%</span></p>
      <p class "ph_analog">PH Analog Value: <span id="ph_analog">%PH_ANALOG%</span></p>
      <p class "ph_neutralVoltage">PH neutralVoltage: <span id="ph_neutralVoltage">%PH_NEUTRALVOLTAGE%</span></p>
      <p class "ph_acidVoltage">PH acidVoltage: <span id="ph_acidVoltage">%PH_ACIDVOLTAGE%</span></p>

      <h2>Calibrate pH 4</h2>
      <p>Put ph sensor in calibration liquid with ph 4 and wait until the value does not change anymore.
         Then click the calibrate button</p>
      <p><button id="calibrate_ph4_button" class="button">Calibrate</button></p>

      <h2>Calibrate pH 7</h2>
      <p>Put ph sensor in calibration liquid with ph 7 and wait until the value does not change anymore.
         Then click the calibrate button</p>
      <p><button id="calibrate_ph7_button" class="button">Calibrate</button></p>

      <p class="tds_calibrated">State: <span id="tds_calibrated">%TDS_CALIBRATED%</span></p>
      <p class "tds_analog">TDS Analog Value: <span id="tds_analog">%TDS_ANALOG%</span></p>

      <h2>Calibrate tds 1412</h2>
      <p>Put tds sensor in calibration liquid with tds value of 1412 and wait until the value does not change anymore.
         Then click the calibrate button</p>
      <p><button id="calibrate_tds_button" class="button">Calibrate</button></p>  
    </div>
  </div>

<script>
  var gateway = `ws://${window.location.hostname}:8080/ws`;
  var websocket;
  var garden_command = {
    type: "unset",
    item: "unset",
    value: 0
  };

  window.addEventListener('load', onLoad);

  function openTab(evt, tabName) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
      tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
      tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";
  }

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    console.log(event.data);
    var garden = JSON.parse(event.data);
    if (garden.pump1_state == "1"){
      pump1_state_display = "OFF";
    }
    else{
      pump1_state_display = "ON";
    }
    if (garden.pump2_state == "1"){
      pump2_state_display = "OFF";
    }
    else{
      pump2_state_display = "ON";
    }
    if (garden.valve1_state == "1"){
      valve1_state_display = "CLOSED";
    }
    else{
      valve1_state_display = "OPEN";
    }
    if (garden.valve2_state == "1"){
      valve2_state_display = "CLOSED";
    }
    else{
      valve2_state_display = "OPEN";
    }
    if (garden.fan1_state == "1"){
      fan1_state_display = "OFF";
    }
    else{
      fan1_state_display = "ON";
    }
    if (garden.fan2_state == "1"){
      fan2_state_display = "OFF";
    }
    else{
      fan2_state_display = "ON";
    }
    if (garden.light_state == "1"){
      light_state_display = "OFF";
    }
    else{
      light_state_display = "ON";
    }
    if (garden.scheduler_active == "1"){
      scheduler_active_display = "ACTIVE";
    }
    else{
      scheduler_active_display = "INACTIVE";
    }
    if (garden.water_state == "1"){
      water_state_display = "OK";
    }
    else {
      water_state_display = "LOW";
    }
    if (garden.ph_calibrated == true) {
      ph_calibrated_display = "CALIBRATED";
    }
    else {
      ph_calibrated_display = "UNCALIBRATED";
    }
    if (garden.tds_calibrated == true) {
      tds_calibrated_display = "CALIBRATED";
    }
    else {
      tds_calibrated_display = "UNCALIBRATED";
    }
    document.getElementById('pump1_state').innerHTML = pump1_state_display;
    document.getElementById('pump2_state').innerHTML = pump2_state_display;
    document.getElementById('valve1_state').innerHTML = valve1_state_display;
    document.getElementById('valve2_state').innerHTML = valve2_state_display;
    document.getElementById('fan1_state').innerHTML = fan1_state_display;
    document.getElementById('fan1_speed').innerHTML = garden.fan1_speed;
    document.getElementById('fan2_state').innerHTML = fan2_state_display;
    document.getElementById('light_state').innerHTML = light_state_display;
    document.getElementById('flow_quantity').innerHTML = garden.flow_quantity;
    document.getElementById('light_on').innerHTML = garden.light_on;
    document.getElementById('light_off').innerHTML = garden.light_off;
    document.getElementById('scheduler_active').innerHTML = scheduler_active_display;
    document.getElementById('flow_rate').innerHTML = garden.flow_rate;
    document.getElementById('flow_quantity').innerHTML = garden.flow_quantity;
    document.getElementById('spray_period').innerHTML = garden.spray_period;
    document.getElementById('spray_duration').innerHTML = garden.spray_duration;
    document.getElementById('dhtValueTemp').innerHTML = garden.dhtValueTemp;
    document.getElementById('dhtValueHumidity').innerHTML = garden.dhtValueHumidity;
    document.getElementById('dallasValueTemp').innerHTML = garden.dallasValueTemp;
    document.getElementById('ph_value').innerHTML = garden.ph_value;
    document.getElementById('ph_analog').innerHTML = garden.ph_analog;
    document.getElementById('ph_calibrated').innerHTML = ph_calibrated_display;
    document.getElementById('ph_neutralVoltage').innerHTML = garden.neutralVoltage;
    document.getElementById('ph_acidVoltage').innerHTML = garden.acidVoltage;
    document.getElementById('tds_value').innerHTML = garden.tds_value;
    document.getElementById('tds_analog').innerHTML = garden.tds_analog;
    document.getElementById('tds_calibrated').innerHTML = tds_calibrated_display;
    document.getElementById('water_state').innerHTML = water_state_display;
  }
  function onLoad(event) {
    initButtons();
    initWebSocket();
  }
  function initButtons() {
    document.getElementById('pump1_button').addEventListener('click', toggle_pump1);
    document.getElementById('pump2_button').addEventListener('click', toggle_pump2);
    document.getElementById('valve1_button').addEventListener('click', toggle_valve1);
    document.getElementById('valve2_button').addEventListener('click', toggle_valve2);
    document.getElementById('fan1_button').addEventListener('click', toggle_fan1);
    document.getElementById('update_fan1_speed_button').addEventListener('click', update_fan1_speed);
    document.getElementById('fan2_button').addEventListener('click', toggle_fan2);
    document.getElementById('light_button').addEventListener('click', toggle_light);
    document.getElementById('update_light_on_button').addEventListener('click', update_light_on);
    document.getElementById('update_light_off_button').addEventListener('click', update_light_off);
    document.getElementById('scheduler_button').addEventListener('click', toggle_scheduler);
    document.getElementById('update_spray_period_button').addEventListener('click', update_spray_period);
    document.getElementById('update_spray_duration_button').addEventListener('click', update_spray_duration);
    document.getElementById('calibrate_ph4_button').addEventListener('click', calibrate_ph4);
    document.getElementById('calibrate_ph7_button').addEventListener('click', calibrate_ph7);
    document.getElementById('calibrate_tds_button').addEventListener('click', calibrate_tds);
  }
  function toggle_pump1() {
    garden_command.type = "toggle";
    garden_command.item = "pump1";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_pump2() {
    garden_command.type = "toggle";
    garden_command.item = "pump2";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_valve1() {
    garden_command.type = "toggle";
    garden_command.item = "valve1";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_valve2() {
    garden_command.type = "toggle";
    garden_command.item = "valve2";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_fan1() {
    garden_command.type = "toggle";
    garden_command.item = "fan1";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function update_fan1_speed() {
    garden_command.type = "update";
    garden_command.item = "fan1_speed";
    garden_command.value = document.getElementById("new_fan1_speed").value
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_fan2() {
    garden_command.type = "toggle";
    garden_command.item = "fan2";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_light() {
    garden_command.type = "toggle";
    garden_command.item = "light";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function update_light_on() {
    garden_command.type = "update";
    garden_command.item = "light_on";
    garden_command.value = document.getElementById("new_light_on").value
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function update_light_off() {
    garden_command.type = "update";
    garden_command.item = "light_off";
    garden_command.value = document.getElementById("new_light_off").value
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function toggle_scheduler() {
    garden_command.type = "toggle";
    garden_command.item = "scheduler";
    garden_command.value = 0;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function update_spray_period() {
    garden_command.type = "update";
    garden_command.item = "spray_period";
    garden_command.value = document.getElementById("new_spray_period").value
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function update_spray_duration() {
    garden_command.type = "update";
    garden_command.item = "spray_duration";
    garden_command.value = document.getElementById("new_spray_duration").value
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function calibrate_ph7() {
    garden_command.type = "toggle";
    garden_command.item = "calibrate_ph";
    garden_command.value = 7;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function calibrate_ph4() {
    garden_command.type = "toggle";
    garden_command.item = "calibrate_ph";
    garden_command.value = 4;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }
  function calibrate_tds() {
    garden_command.type = "toggle";
    garden_command.item = "calibrate_tds";
    garden_command.value = 1413;
    const garden_data = JSON.stringify(garden_command);
    websocket.send(garden_data);
  }

</script>
</body>
</html>
)rawliteral";
