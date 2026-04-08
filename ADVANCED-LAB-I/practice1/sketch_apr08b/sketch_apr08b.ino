#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

int total_muestras = 300;
int contador = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("No se encontró el MPU6050");
    while (1);
  }

  Serial.println("Inicio de adquisición...");
  delay(2000); // tiempo para prepararte (ej: hacer el gesto)
}

void loop() {
  if (contador < total_muestras) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Enviar datos en formato CSV: x,y,z
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.println(a.acceleration.z);

    contador++;
    delay(50); // controla la frecuencia (~20 Hz)
  } else {
    Serial.println("FIN");
    while (1); // detener el programa
  }
}
