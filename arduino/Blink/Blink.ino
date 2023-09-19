/*
The SD card circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
*/

#include <SD.h>
#include <SPI.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define PIN_SPI_CS 4
struct Vec3
{
    float x;
    float y;
    float z;
};

class MPU6050
{
public:
    Adafruit_MPU6050 device;
    Vec3 offset = {0, 0, 0};
    Vec3 reference = {0, 0, 9.82};

    Vec3 getAcceleration()
    {
        sensors_event_t a, g, temp;
        device.getEvent(&a, &g, &temp);
        return {
            a.acceleration.x + this->offset.x,
            a.acceleration.y + this->offset.y,
            a.acceleration.z + this->offset.z};
    }

    void calibrate()
    {
        Vec3 current = getAcceleration();
        this->offset = {
            reference.x - current.x,
            reference.y - current.y,
            reference.z - current.z};
    }
};

MPU6050 mpu;

const int MAX_RANDOM = 99999999;
const int LED_PIN = 2;

struct Button
{
    int pin;
    int state;
    int lastState;
};

Button startButton;

void blink(int n, int delayMillis = 200)
{
    for (int i = 0; i < n; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(delayMillis);
        digitalWrite(LED_PIN, LOW);
        delay(delayMillis);
    }
}

void writeRandom()
{
    String filename = String(random(MAX_RANDOM)) + ".txt";

    if (SD.exists(filename))
    {
        Serial.println(filename + " exists.");
        filename = String(random(MAX_RANDOM)) + ".txt";
        Serial.println("New filename: " + filename);
    }
    else
    {
        Serial.println(filename + " doesn't exist.");
    }

    File myFile = SD.open(filename, FILE_WRITE);
    if (myFile)
    {
        Serial.println("Writing to " + filename + "...");
        for (int i = 0; i < 500; i++)
        {
            myFile.println(
                String(random(99)) +
                "," +
                String(random(99)) +
                "," +
                String(random(99)));
        }
        myFile.close();
        Serial.println("Write done.");
        blink(2, 150);
    }
    else
    {
        Serial.println("error opening " + filename);
    }
}

void setup()
{
    // Pin setup
    pinMode(LED_PIN, OUTPUT);

    startButton.pin = 7;
    pinMode(7, INPUT);
    startButton.lastState = LOW;
    startButton.state = LOW;

    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // Initialize SD card
    if (!SD.begin(4))
    {
        Serial.println("SD initialization failed!");
        digitalWrite(LED_PIN, HIGH);
        while (1)
        {
            ;
        }
    }
    else
    {
        Serial.println("SD initialization done.");
        blink(5);
    }

    delay(500);

#pragma region accelerometer_calibration
    // Init accelerometer
    if (!mpu.device.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        digitalWrite(LED_PIN, HIGH);
        while (1)
        {
            ;
        }
    }
    else
    {
        Serial.println("MPU6050 Found!");
        blink(3);
    }

    // Select accelerometer range
    mpu.device.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.device.getAccelerometerRange())
    {
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
    mpu.device.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.device.getGyroRange())
    {
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

    mpu.device.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.device.getFilterBandwidth())
    {
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
    mpu.calibrate();
#pragma endregion

    // Get random
    randomSeed(analogRead(A0));
}

void record()
{
    String filename = String(random(MAX_RANDOM)) + ".csv";
    if (SD.exists(filename))
    {
        blink(1, 1000);
        Serial.println(filename + " invalid or exists.");
        return;
    }

    blink(2, 100);
    // File f = SD.open(filename, FILE_WRITE);
    Serial.println("Recording to: " + filename);

    // if (!f)
    // {
    //     blink(1, 1000);
    //     Serial.println("error opening " + filename);
    //     return;
    // }

    while (!(startButton.state == HIGH && startButton.lastState == LOW))
    {
        Vec3 a = mpu.getAcceleration();
        Serial.println(
            String(a.x) +
            "," +
            String(a.y) +
            "," +
            String(a.z));
        delay(50);
        startButton.lastState = startButton.state;
        startButton.state = digitalRead(startButton.pin);
    }
    // f.close();
    Serial.println("Recording done.");
    blink(4, 100);
    return;
}

void loop()
{
    // Get button states
    startButton.state = digitalRead(startButton.pin);
    // If button is pressed, write random
    if (startButton.state == HIGH && startButton.lastState == LOW)
    {
        startButton.state = LOW;
        startButton.lastState = LOW;
        delay(500);
        record();
    }

    delay(50);
    startButton.lastState = startButton.state;
}
