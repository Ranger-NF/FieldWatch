#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your Wi-Fi credentials
const char* ssid = "Neuronauts";
const char* password = "neuroftw";

const char* serverUrl = "http://ptsv3.com/t/neuronauts/";

HTTPClient http;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(serverUrl);

    http.addHeader("Content-Type", "application/json");

    // Create a simple JSON payload
    String payload = "{\"message\": \"Hello from Xiao ESP32-S3!\"}";

    // Send POST request
    int httpResponseCode = http.POST(payload);

    // Check the response from the server
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response from server:");
      Serial.println(response);  // This will print the server's response
    } else {
      Serial.println("Error in sending POST request");
    }

    // Close the HTTP connection
    http.end();
  } else {
    Serial.println("Wi-Fi not connected");
  }

  delay(10000);  // Wait for 10 seconds before sending the next request
}
