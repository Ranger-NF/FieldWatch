#include "pitches.h"
#include "esp_camera.h"
#include <Arduino.h>
#include <driver/i2s.h>

// I2S configuration
#define I2S_DATA_IN_PIN 34  // Adjust to your microphone input pin
#define I2S_CLOCK_PIN 0
#define I2S_LRCK_PIN 26
#define I2S_SCK_PIN 27

#include "edge_impulse_sdk.h"  // Include Edge Impulse SDK


// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// Camera configuration settings (adjust according to your needs)
camera_config_t config = {
  .pin_pwdn = GPIO_NUM_32,
  .pin_reset = GPIO_NUM_NC,
  .pin_xclk = GPIO_NUM_0,
  .pin_sda = GPIO_NUM_26,
  .pin_scl = GPIO_NUM_27,
  .pin_d0 = GPIO_NUM_5,
  .pin_d1 = GPIO_NUM_18,
  .pin_d2 = GPIO_NUM_19,
  .pin_d3 = GPIO_NUM_21,
  .pin_d4 = GPIO_NUM_36,
  .pin_d5 = GPIO_NUM_39,
  .pin_d6 = GPIO_NUM_34,
  .pin_d7 = GPIO_NUM_35,
  .pin_vsync = GPIO_NUM_25,
  .pin_href = GPIO_NUM_23,
  .pin_pclk = GPIO_NUM_22,
  .xclk_freq_hz = 10000000,  // 10 MHz
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  .brightness = 0,
  .contrast = 0,
  .saturation = 0,
  .whitebal = 0,
  .gainceiling = (gainceiling_t)0,
  .pixel_format = PIXFORMAT_JPEG,
  .frame_size = FRAMESIZE_SVGA,  // Choose a frame size
  .jpeg_quality = 12,
  .fb_count = 2
};


// Your inference function
void run_inference(uint8_t *image_buffer, size_t image_size) {
  // Prepare the input for Edge Impulse model
  // This might include resizing or other preprocessing if necessary

  // Run the inference
  ei_impulse_result_t result = ei_run_impulse(image_buffer, image_size);

  // Handle the result (e.g., print the classification result)
  Serial.print("Inference Result: ");
  Serial.println(result.classification[0].value); // This is just an example
}

void play_note() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(1, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(1);
  }
}

void setup() {
  Serial.begin(115200);
    // Initialize camera
  esp_camera_init(&config);

    i2s_config_t i2s_config = {
    .mode = I2S_MODE_MASTER | I2S_MODE_RX,  // Receive mode
    .sample_rate = 16000,  // 16 kHz sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // 16-bit samples
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,  // Mono audio
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Interrupt flag
    .dma_buf_count = 8,  // Buffer count
    .dma_buf_len = 1024,  // Buffer length
    .use_apll = false,
  };
  
  i2s_pin_config_t pin_config = {
    .ws_io_num = I2S_LRCK_PIN,
    .bck_io_num = I2S_SCK_PIN,
    .data_in_num = I2S_DATA_IN_PIN,
    .data_out_num = I2S_PIN_NO_CHANGE,  // Output not used
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}
void loop() {
  int16_t buffer[1024];  // Buffer to hold the audio data
  size_t bytes_read;
  
  // Read audio data from the I2S bus
  i2s_read(I2S_NUM_0, (void*)buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);

  // Capture a frame from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Send the frame to Edge Impulse model (inference step)
  // Example: Use a C++ Edge Impulse library to process the frame data
  run_inference(fb->buf, fb->len);

  // Return the frame buffer to the camera driver
  esp_camera_fb_return(fb);

  delay(100); // Adjust delay for real-time processing
}

