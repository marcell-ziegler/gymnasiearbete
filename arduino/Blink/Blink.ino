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
        Serial.println("initialization failed!");
        while (1)
        {
            ;
        }
    }
    else
    {
        Serial.println("initialization done.");
        blink(5);
    }

    // Get random
    randomSeed(analogRead(A0));
}

void loop()
{
    // Get button states
    startButton.state = digitalRead(startButton.pin);
    // If button is pressed, write random
    if (startButton.state == LOW && startButton.lastState == HIGH)
    {
        writeRandom();
    }

    delay(50);
    startButton.lastState = startButton.state;
}
