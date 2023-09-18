// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define PIN_SPI_CS 4

Adafruit_MPU6050 mpu;

float x_acc_offset;
float y_acc_offset;
float z_acc_offset;

const float x_acc_ref = 0;
const float y_acc_ref = 0;
const float z_acc_ref = 9.82;

int reset_last_state = HIGH;
int start_stop_last_state = HIGH;


File data_file;

bool started = true;



void setup(void) {
  Serial.begin(115200);
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  randomSeed(analogRead(A0));

  // Init SD Card Reader
  if (!SD.begin(PIN_SPI_CS)) {
    Serial.println(F("SD CARD FAILED."));
    while (1) {
      delay(10);
    }
  } else {
    Serial.println("SD Card Initialized!");
  }

  data_file = SD.open("test.txt", FILE_WRITE);

  if (data_file) {
    Serial.println("File opened!");
  } else {
    Serial.println("File unopened!");
  }

  // Calibrate sensor
  sensors_event_t init_a, init_g, init_temp;
  mpu.getEvent(&init_a, &init_g, &init_temp);

  x_acc_offset = x_acc_ref - init_a.acceleration.x;
  y_acc_offset = y_acc_ref - init_a.acceleration.y;
  z_acc_offset = z_acc_ref - init_a.acceleration.z;
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  int reset_current_state = digitalRead(8);
  int start_stop_current_state = digitalRead(7);

  // Serial.print("Reset state: ");
  // Serial.println(reset_current_state);
  // Serial.print("Start state: ");
  // Serial.println(start_stop_current_state);

  if (reset_last_state == LOW && reset_current_state == HIGH) {
    Serial.println("Reset!");
    
    x_acc_offset = x_acc_ref - a.acceleration.x;
    y_acc_offset = y_acc_ref - a.acceleration.y;
    z_acc_offset = z_acc_ref - a.acceleration.z;
  }

  if (start_stop_last_state == LOW && start_stop_current_state == HIGH) {
    if (started && data_file) {
      Serial.println("Stopped!");

      data_file.close();
      started = false;
    }

    if (!started) {
      Serial.println("Starting!");
      data_file = SD.open(String(random()) + ".txt");
      started = true;
    }
  }

  if (started && data_file) {
    float x_acc = a.acceleration.x + x_acc_offset;
    float y_acc = a.acceleration.y + y_acc_offset; 
    float z_acc = a.acceleration.z + z_acc_offset; 

    data_file.print(x_acc);
    data_file.print(",");
    data_file.print(y_acc);
    data_file.print(",");
    data_file.print(z_acc);
    data_file.println("");

    Serial.print(x_acc);
    Serial.print(",");
    Serial.print(y_acc);
    Serial.print(",");
    Serial.print(z_acc);
    Serial.println("");
  }


  /* Plotter printing */
  // Serial.print(a.acceleration.x + x_acc_offset);
  // Serial.print(",");
  // Serial.print(a.acceleration.y + y_acc_offset);
  // Serial.print(",");
  // Serial.print(a.acceleration.z + z_acc_offset);

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  // Serial.print(a.acceleration.x + x_acc_offset);
  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y + y_acc_offset);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z + z_acc_offset);
  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print(g.gyro.x);
  // Serial.print(", Y: ");
  // Serial.print(g.gyro.y);
  // Serial.print(", Z: ");
  // Serial.print(g.gyro.z);
  // Serial.println(" rad/s");

  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  // Serial.println(" degC");


  reset_last_state = reset_current_state;
  start_stop_last_state = start_stop_current_state;
  // Serial.println("");
  delay(50);
}