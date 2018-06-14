

/* Name STTduino
 coded by Tom Kurian
 */


#include <SPI.h>
#include <Wire.h>
#include <dht.h>
#define DHT11_PIN 7
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


int d = 8; //Enter depth of your tank here in centimeters
int sen;
dht DHT;
 
int sensorPin     = A3;     // select the input pin for ldr
int sensorValue   = 0;      // variable to store the value coming from the ldr
int soilmo        = 0;      //variable to store soil moisture
int moi           = A0;     // soil moisture pin
int trig = 11;             // Attach Trig of ultrasonic sensor to pin 11
int echo = 12;             // Attach Echo of ultrasonic sensor to pin 12

int lightrelay = 3;
int tankrelay  = 4;
int solrelay   = 5;
int fanrelay   = 6;

void setup() 
{
pinMode(tankrelay, OUTPUT); //pins connected to the relays
pinMode(solrelay, OUTPUT);
pinMode(lightrelay, OUTPUT);
pinMode(fanrelay, OUTPUT);
Serial.begin(9600);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(1);
display.clearDisplay(); 
display.setCursor(0,0);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
display.print("Welcome"); 
display.display();
delay(2000);
}

void loop()
{

digitalWrite(tankrelay, LOW);
digitalWrite(solrelay, LOW);



 long duration, cm;   
 
      sensorValue = analogRead(sensorPin);  
      soilmo = analogRead(moi);// reads soil moisture value


                ////TEMPERATURE AND HUMIDITY
                  
                  DHT.read11(DHT11_PIN);
                  Serial.print("Temperature = ");                 
                  Serial.println(DHT.temperature);
                  Serial.print("Humidity = ");
                  Serial.println(DHT.humidity);
                           
              // end of temp and humid 

                                                    // SR-04 Ranging
                                                    
                                                    pinMode(trig, OUTPUT);
                                                    digitalWrite(trig, LOW);
                                                    delayMicroseconds(2);
                                                    digitalWrite(trig, HIGH);
                                                    delayMicroseconds(5);
                                                    digitalWrite(trig, LOW);
                                                  
                                                    // The same pin is used to read the signal from the PING: a HIGH
                                                    // pulse whose duration is the time (in microseconds) from the sending
                                                    // of the ping to the reception of its echo off of an object.
                                                    pinMode(echo, INPUT);
                                                    duration = pulseIn(echo, HIGH);
                                                    cm = microsecondsToCentimeters(duration);
                                                    


            display.setCursor(0,0);
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.print("Lux:"); display.print(sensorValue); 
            display.print("\nTmp:");display.print(DHT.temperature); display.print("C");
            display.print("Hum:");display.print(DHT.humidity);
            display.print("\nRange:"); display.print(cm); 
            display.print("\nMoisture:"); display.print(soilmo);
            display.display();
            delay(1000);

// Checking the condition for main tank
if(cm<10)
{
display.setCursor(0,0);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.println("Main Tank Is Empty");
display.display();  
delay(1000);
// Making the relay turn on
display.setCursor(0,0);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.println("Tank Is Filling");
display.display(); 
digitalWrite(tankrelay, HIGH);
delay(60000);
digitalWrite(tankrelay, LOW);
}

// Checking the condition for soil moisture
if(soilmo>960)
{
display.setCursor(0,0);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.println("Soil Is Dry");
display.display();  
delay(1000);
// Making the relay turn on
digitalWrite(solrelay, HIGH);
delay(10000);
digitalWrite(solrelay, LOW);
}

// Checking the condition for light intensity
if(sensorValue<200)
{
/*int in=sensorValue;
      if(in<200 && in>150)
      analogWrite(lightrelay, 24); 
      if(in<150 && in>125)
      analogWrite(lightrelay, 48); 
      if(in<125 && in>50)
      analogWrite(lightrelay, 72); 
      if(in<50)
      analogWrite(lightrelay, 96);*/ 
      digitalWrite(lightrelay,HIGH);
}
else
digitalWrite(lightrelay,LOW);

// Check For temperature
if(DHT.temperature>32)
digitalWrite(fanrelay,HIGH); 
else
digitalWrite(fanrelay,LOW);

delay(100);

}


long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

