#include <DS18B20.h>
#include <OneWire.h>
#include <TaskManager.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Potentiometer.h>
#include <Light_sensor.h>

#include "config.h"

typedef TaskManager<OS_QUEUE_SIZE> TaskManager_;
TaskManager_ OS;

DS18B20 inbox_tempC(ONE_WIRE_PIN);

typedef Potentiometer<ADC_bit_depth> Potentiometer10bit;
typedef Light_sensor<ADC_bit_depth> Light_sensor10bit;

Potentiometer10bit rpm_regulator(rpm_regulator_pin);
Light_sensor10bit smoke_sensor(smoke_sensor_digital_pin);

float start_tempC = 0;
float curr_tempC = 0;

#include "Tasks.h"

ISR (TIMER2_OVF_vect){
  OS.TimerTaskService_();
  //Serial.println("Processing timer...");
}

void setup() {

  noInterrupts();
  TCCR2B |= (1<<CS20);    // (clk/1)
  TIMSK2 |= (1<<TOIE2);   // ovf interrupt enabled
  interrupts();
  
  pinMode(fan_pin1,OUTPUT);
  pinMode(fan_pin2,OUTPUT);
  digitalWrite(fan_pin1,LOW);
  digitalWrite(fan_pin2,LOW);
  // motor is stopped
  
  start_tempC = inbox_tempC.getTempC();

#ifdef DEBUG_ENABLED
  Serial.begin(9600);
  Serial.print(F("Checking start temp...\0"));
  Serial.print(start_tempC);
  Serial.println(F("*C"));
  Serial.println();
#endif  //DEBUG_ENABLED

  //OS.SetTask_(Tasks::checkTemp, null_delay);
  OS.SetTask_(Tasks::startFiring, null_delay);
}

void loop() {
  OS.ProcessTaskQueue_();
  //Serial.println(F("Processing..."));

}
