#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "";
const char* password = "";
AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "angle";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 with Servo</title>
</head>
<style>
    * {
            margin: 0;
            box-sizing: border-box;
        }
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container,form {
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
            row-gap: 20px;
        }
        #pi_input{
            width: 300px; 
            height: 20px; 
        }
</style>
<body>
    <div class="container">
        <h1>ESP32 with Servo</h1>
        <p>Value: <output id="value"></output></p>
        <form action="/get">
            <input id="pi_input" type="range" min="0" max="180" value="0" step="any" list="values" name="angle"/>
            <input type="submit" value="Send to ESP32">
            <datalist id="values">
                <option value="0" label="0"></option>
                <option value="30" label="30"></option>
                <option value="60" label="60"></option>
                <option value="90" label="90"></option>
                <option value="120" label="120"></option>
                <option value="150" label="150"></option>
                <option value="180" label="180"></option>
            </datalist></form>
    </div>

    <script>
        const value = document.querySelector("#value");
        const input = document.querySelector("#pi_input");

        value.textContent = input.value;
        
        input.addEventListener("input", (event) => {
            value.textContent = event.target.value;
        })
    </script>
</body>
</html>

)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
} 

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
  // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP with value: "+ inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
