const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Geiger Counter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
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
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
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
  </style>
<title>Garden</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>Garden</h1>
  </div>
  <div class="content">
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
      <h2>Scheduler</h2>
      <p class="scheduler_active">State: <span id="scheduler_active">%SCHEDULER_ACTIVE%</span></p>
      <p><button id="scheduler_button" class="button">Toggle</button></p>
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
<script>
  var gateway = `ws://${window.location.hostname}:8080/ws`;
  var websocket;
  var garden_command = {
    type: "unset",
    item: "unset",
    value: 0
  };

  window.addEventListener('load', onLoad);
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
    if (garden.scheduler_active == "1"){
      scheduler_active_display = "ACTIVE";
    }
    else{
      scheduler_active_display = "INACTIVE";
    }
    document.getElementById('pump1_state').innerHTML = pump1_state_display;
    document.getElementById('pump2_state').innerHTML = pump2_state_display;
    document.getElementById('valve1_state').innerHTML = valve1_state_display;
    document.getElementById('valve2_state').innerHTML = valve2_state_display;
    document.getElementById('scheduler_active').innerHTML = scheduler_active_display;
    document.getElementById('flow_rate').innerHTML = garden.flow_rate;
    document.getElementById('flow_quantity').innerHTML = garden.flow_quantity;
    document.getElementById('spray_period').innerHTML = garden.spray_period;
    document.getElementById('spray_duration').innerHTML = garden.spray_duration;
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
    document.getElementById('scheduler_button').addEventListener('click', toggle_scheduler);
    document.getElementById('update_spray_period_button').addEventListener('click', update_spray_period);
    document.getElementById('update_spray_duration_button').addEventListener('click', update_spray_duration);
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
</script>
</body>
</html>
)rawliteral";

