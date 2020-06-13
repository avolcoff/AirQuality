/*
  AirQuality Demo V1.0.
  connect to A1 to start testing. it will needs about 20s to start 
* By: http://www.seeedstudio.com
*/
#include"AirQuality.h"
#include"Arduino.h"
AirQuality airqualitysensor;
int current_quality =-1;

//Sensor Setup
#define airquality_sensor_pin 0

int airQualityThreshold = 40;
 
int buzzer = 7;
const int redPin = 10;
const int greenPin = 8;
const int bluePin = 12;

void setup()
{
    Serial.begin(9600);
    airqualitysensor.init(A0);
    pinMode(buzzer, OUTPUT);
}
void loop()
{
    current_quality=airqualitysensor.slope();
    
    int airquality_value = analogRead(airquality_sensor_pin);
    Serial.println(airquality_value);
    
    if (current_quality >= 0)// if a valid data returned.
    {
        if (airquality_value/airQualityThreshold >= 3){
            Serial.println("High pollution! Force signal active");
            analogWrite(redPin, 255);
            analogWrite(greenPin, 0);
            analogWrite(bluePin, 0);
            for (int i = 0; i < 80; i++) {  // make a sound
              digitalWrite(buzzer, HIGH); // send high signal to buzzer 
              delay(1); // delay 1ms
              digitalWrite(buzzer, LOW); // send low signal to buzzer
              delay(1);
            }
            delay(20);
            for (int j = 0; j < 100; j++) { //make another sound
              digitalWrite(buzzer, HIGH);
              delay(2); // delay 2ms
              digitalWrite(buzzer, LOW);
              delay(2);
            }
            delay(20);
        }
        else if (airquality_value/airQualityThreshold >= 2){
            Serial.println("High pollution!");
            analogWrite(redPin, 255);
            analogWrite(greenPin, 0);
            analogWrite(bluePin, 0);
        }
        else if (airquality_value/airQualityThreshold >= 1){
            Serial.println("Low pollution!");
            analogWrite(redPin, 0);
            analogWrite(greenPin, 0);
            analogWrite(bluePin, 255);
        }
        else {
            Serial.println("Fresh air");
            analogWrite(redPin, 0);
            analogWrite(greenPin, 255);
            analogWrite(bluePin, 0);
        }
    }
}

ISR(TIMER2_OVF_vect)
{
    if(airqualitysensor.counter==122)//set 2 seconds as a detected duty
    {
        airqualitysensor.last_vol=airqualitysensor.first_vol;
        airqualitysensor.first_vol=analogRead(A0);
        airqualitysensor.counter=0;
        airqualitysensor.timer_index=1;
        PORTB=PORTB^0x20;
    }
    else
    {
        airqualitysensor.counter++;
    }
}
