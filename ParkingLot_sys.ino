#include "ParkingLot_sys.h"
#include <Servo.h>

uint16_t in_sensor_threshold = 400;
uint16_t out_sensor_threshold = 130;
uint16_t time_threshold = 1000;
uint8_t num_car = 0;
uint8_t Max_car = 3;
long in_current_time = millis();
long out_current_time = millis();
Servo myServo;  // create a servo object

void yellow_light(void)
{
  analogWrite(RED_LIGHT_PIN, 100);
  analogWrite(GREEN_LIGHT_PIN, 120);
  analogWrite(YELLOW_LIGHT_PIN, 0);
}
void green_light(void)
{
  analogWrite(GREEN_LIGHT_PIN, 110);
  analogWrite(YELLOW_LIGHT_PIN, 0);
  analogWrite(RED_LIGHT_PIN, 0);
}
void red_light(void)
{
  analogWrite(RED_LIGHT_PIN, 110);
  analogWrite(YELLOW_LIGHT_PIN, 0);
  analogWrite(GREEN_LIGHT_PIN, 0);
}

void gate_down(void)
{
  myServo.write(180);
}
void gate_up(void)
{
  myServo.write(90);
}

void light_state(int light_case)
{
  switch(light_case)
  {
    case 0:
    case 1:
    {
      green_light();
      break;
    }
    case 2:
    {
      yellow_light();
      break;
    }
    case 3:
    {
      red_light();
      break;
    }
    default:
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LIGHT_PIN, OUTPUT);
  pinMode(GREEN_LIGHT_PIN, OUTPUT);
  pinMode(YELLOW_LIGHT_PIN, OUTPUT);
  myServo.attach(SERVO_PIN); // attaches the servo on pin 9 to the servo object
  pinMode(IN_SENSOR, INPUT_PULLUP);
  pinMode(OUT_SENSOR, INPUT_PULLUP);
  Serial.begin(9600);   // 與電腦序列埠連線
  in_current_time = millis();
  out_current_time = millis();
  gate_down();
  num_car = 0;
  light_state(num_car);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(num_car < Max_car)
  {
    if(analogRead(IN_SENSOR) > in_sensor_threshold)
    {
      if((millis()-in_current_time) > time_threshold)
      {
        gate_up();
        in_current_time = millis();
        num_car++;

        while(1)
        {
          if(analogRead(OUT_SENSOR) > out_sensor_threshold)
          {
            if((millis()-out_current_time) > time_threshold)
            {
              gate_down();
              out_current_time = millis();
              light_state(num_car);
              break;
            }
          }  
        }
        Serial.println("QQ");
        Serial.println(num_car);
      }
    }
  }
  if(num_car)
  {
    if(analogRead(OUT_SENSOR) > out_sensor_threshold)
    {
      if((millis()-out_current_time) > time_threshold)
      {
        gate_up();
        out_current_time = millis();
        num_car--;

        while(1)
        {
          if(analogRead(IN_SENSOR) > in_sensor_threshold)
          {
            if((millis()-in_current_time) > time_threshold)
            {
              gate_down();
              in_current_time = millis();
              light_state(num_car);
              break;
            }
          } 
        }
        Serial.println("KK");
        Serial.println(num_car);
      }
    }
  }
}
