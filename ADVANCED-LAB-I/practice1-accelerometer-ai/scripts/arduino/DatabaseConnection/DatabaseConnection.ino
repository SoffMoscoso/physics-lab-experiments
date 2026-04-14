#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "../../models/model.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "config.h"

// 🔥 NUEVO: WiFi + Firebase
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>

Adafruit_MPU6050 mpu;

// Firebase objetos
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Define input and output dimensions
const int kInputSize = 3;
const int kOutputSize = 3;

// TensorFlow Lite
tflite::MicroInterpreter* interpreter = nullptr;

constexpr int kTensorArenaSize = 80 * 1024;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

static tflite::MicroMutableOpResolver<3> resolver;

// Función para obtener índice máximo
int max_index(float* arr, int size) {
  int max_i = 0;
  for (int i = 1; i < size; i++) {
    if (arr[i] > arr[max_i]) {
      max_i = i;
    }
  }
  return max_i;
}

void setup() {
  Serial.begin(115200);

  // CONECTAR WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi conectado");

  // CONFIGURAR FIREBASE
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase listo");

  // MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 no encontrado");
    while (1) delay(10);
  }
  Serial.println("MPU6050 OK");

  // TensorFlow ops
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddSoftmax();

  const tflite::Model* model = tflite::GetModel(gesture_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Modelo incompatible");
    while (1) delay(10);
  }

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("Error en AllocateTensors");
    while (1) delay(1000);
  }

  Serial.println("Modelo listo");
}

void loop() {
  // Leer sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float sensor_data[kInputSize] = {
    a.acceleration.x,
    a.acceleration.y,
    a.acceleration.z
  };

  // Input
  float* input = interpreter->input(0)->data.f;
  for (int i = 0; i < kInputSize; i++) {
    input[i] = sensor_data[i];
  }

  // Inferencia
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Error en inferencia");
    return;
  }

  // Output
  float* output = interpreter->output(0)->data.f;
  int predicted_gesture = max_index(output, kOutputSize);

  Serial.print("Gesto: ");
  Serial.println(predicted_gesture);

  // ENVIAR A FIREBASE
  if (Firebase.RTDB.setInt(&fbdo, "/movimiento", predicted_gesture)) {
    Serial.println("Enviado a Firebase");
  } else {
    Serial.print("Error Firebase: ");
    Serial.println(fbdo.errorReason());
  }

  delay(1000);
}