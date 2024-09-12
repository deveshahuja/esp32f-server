#include <WiFi.h>                 // WiFi library for ESP32
#include <ESPAsyncWebServer.h>    // Async WebServer library
#include <M5Stack.h>              // Include M5Stack library (if using M5Stack, otherwise remove this line)

// Replace with your network credentials
const char* ssid = "your_ssid";        // Your Wi-Fi SSID
const char* password = "your_password";  // Your Wi-Fi password

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Placeholder for dynamic values
String processor(const String& var) {
  // When {{RSSI}} is found in HTML, replace with Wi-Fi signal strength
  if (var == "RSSI") {
    return String(WiFi.RSSI());
  }
  // Add more dynamic values as needed
  return String();
}

// Define the HTML template with placeholders
const char webpageTemplate[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
    <head>
      <title>ESP32 Dynamic Web Server</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <meta http-equiv="refresh" content="1">
    </head>
    <body>
      <h1>Welcome to M5Stack Web Server for Signal Strenth</h1>
      <p>Wi-Fi Signal Strength (RSSI): {{RSSI}} dBm</p>
    </body>
  </html>
)rawliteral";

void setup() {

  M5.begin();

  // Start Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String htmlContent = webpageTemplate;  // Create a copy of the HTML template
    htmlContent.replace("{{RSSI}}", processor("RSSI"));  // Replace dynamic content
    request->send(200, "text/html", htmlContent);  // Send the HTML page
  });

  // Start server
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  // Nothing to do here since it's asynchronous
}
