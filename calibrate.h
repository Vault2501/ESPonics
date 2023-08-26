const char calibrate_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Geiger Counter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
    margin: 0;
    }
    html {
      color: #143642;
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
  </style>
  <title>Garden</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
</head>

<body>

    <div class="topnav">
      <h1>Garden</h1>
    </div>

    <div id="Calibration" class="content">
      <h2>Calibration</h2>
      <p><button id="calibration_button" class="button">Start Calibration</button></p>
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
    }

    function onLoad(event) {
      initButtons();
      initWebSocket();
    }

    function initButtons() {
      document.getElementById('calibration_button').addEventListener('click', toggle_calibration);
    }

    function toggle_calibration() {
      console.log('Starting calibration');
      garden_command.type = "toggle";
      garden_command.item = "calibration";
      garden_command.value = 0;
      const garden_data = JSON.stringify(garden_command);
      websocket.send(garden_data);
    }

  </script>
</body>
</html>
)rawliteral";
