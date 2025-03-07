#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "Myphone";
const char* password = "avpv0707";

// Google Colab Server URL (Change to your endpoint)
String serverUrl = "https://colab.research.google.com/drive/1mmDAnkva8bVSj5e79Nze9P4u4W80KuY4";

// Camera Configuration
#define PWDN_GPIO    32
#define RESET_GPIO   -1
#define XCLK_GPIO    0
#define SIOD_GPIO    26
#define SIOC_GPIO    27
#define Y9_GPIO      35
#define Y8_GPIO      34
#define Y7_GPIO      39
#define Y6_GPIO      36
#define Y5_GPIO      21
#define Y4_GPIO      19
#define Y3_GPIO      18
#define Y2_GPIO      5
#define VSYNC_GPIO   25
#define HREF_GPIO    23
#define PCLK_GPIO    22

camera_config_t config;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected");

  // Initialize the Camera
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO;
  config.pin_d1 = Y3_GPIO;
  config.pin_d2 = Y4_GPIO;
  config.pin_d3 = Y5_GPIO;
  config.pin_d4 = Y6_GPIO;
  config.pin_d5 = Y7_GPIO;
  config.pin_d6 = Y8_GPIO;
  config.pin_d7 = Y9_GPIO;
  config.pin_xclk = XCLK_GPIO;
  config.pin_pclk = PCLK_GPIO;
  config.pin_vsync = VSYNC_GPIO;
  config.pin_href = HREF_GPIO;
  config.pin_sccb_sda = SIOD_GPIO;
  config.pin_sccb_scl = SIOC_GPIO;
  config.pin_pwdn = PWDN_GPIO;
  config.pin_reset = RESET_GPIO;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Camera Init Failed!");
    return;
  }

  Serial.println("Camera Ready!");
}

void loop() {
  Serial.println("Capturing Image...");
  
  // Do NOT deinitialize the camera here!
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera Capture Failed!");
    return;
  }

  Serial.println("Sending Image to Google Colab...");
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "image/jpeg");

  int httpResponseCode = http.POST(fb->buf, fb->len);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error in sending image.");
  }
  
  http.end();
  esp_camera_fb_return(fb);
  
  delay(5000); // Capture image every 5 seconds
}