#ifndef _TASKS_H
#define _TASKS_H

namespace Tasks
{
	void checkTemp(void);
	void startFiring(void);
  void checkSmokeSensor(void);
	void setFanSpeed(void);
}

//////////////////////////////////////////////////////////////////////////////////////
void Tasks::startFiring(void){
	
		analogWrite(fan_pin1, fan_pwm_max);
    
#ifdef DEBUG_ENABLED    
		Serial.println(F("Firing...\0"));
    Serial.println();
#endif //#ifdef DEBUG_ENABLED

    OS.SetTask_(Tasks::checkSmokeSensor, null_delay);
}

//////////////////////////////////////////////////////////////////////////////////////
void Tasks::checkSmokeSensor(void){
  if (smoke_sensor.getState()== state::sensor_is_opened) {
    OS.SetTask_(startFiring, check_smoke_sensor_delay);
#ifdef DEBUG_ENABLED    
    Serial.println(F("No smoke in chamber\0"));
    Serial.println();
#endif //#ifdef DEBUG_ENABLED
  }
  else {
    OS.SetTask_(Tasks::setFanSpeed, null_delay);
#ifdef DEBUG_ENABLED    
    Serial.println(F("Smoke in chamber\0"));
    Serial.println();
#endif //#ifdef DEBUG_ENABLED
  }
}

//////////////////////////////////////////////////////////////////////////////////////
void Tasks::setFanSpeed(void){
	unsigned char fan_pwm_value;

	if (curr_tempC == max_temp){
    fan_pwm_value = fan_pwm_min;

#ifdef DEBUG_ENABLED
    Serial.println(F("PWM min...\0"));
    Serial.println();
#endif //#ifdef DEBUG_ENABLED
	}
	
	else if (curr_tempC > max_temp){
    fan_pwm_value = fan_pwm_stop;
    
#ifdef DEBUG_ENABLED
    Serial.println(F("PWM stop...\0"));
    Serial.println();
#endif //#ifdef DEBUG_ENABLED
	}
	
	else {
    float temp_reference = rpm_regulator.getValueAsCoeff() * (1 - (curr_tempC / max_temp));

#ifdef DEBUG_ENABLED
    Serial.print(F("rpm regulator coefficient "));
    Serial.println(rpm_regulator.getValueAsCoeff());
    Serial.print(F("temp reference "));
    Serial.println(temp_reference);
    Serial.println();
#endif //#ifdef DEBUG_ENABLED

		fan_pwm_value = (unsigned int)(fan_pwm_max * temp_reference) + (unsigned int)fan_pwm_additive;
		if (fan_pwm_value > fan_pwm_max) fan_pwm_value = fan_pwm_max;
   
#ifdef DEBUG_ENABLED   
    Serial.println(F("PWM calculate...\0"));
#endif //#ifdef DEBUG_ENABLED
	}

  analogWrite(fan_pin1, fan_pwm_value);
  
	OS.SetTask_(checkTemp, temp_check_delay);

#ifdef DEBUG_ENABLED 
	Serial.print(F("setting fan speed...\0"));
  Serial.println(fan_pwm_value);
  Serial.println();
#endif //#ifdef DEBUG_ENABLED
}

//////////////////////////////////////////////////////////////////////////////////////
void Tasks::checkTemp(void){
  
  curr_tempC = inbox_tempC.getTempC();
  
#ifdef DEBUG_ENABLED
  Serial.print(F("Checking temp..."));
  Serial.print(curr_tempC);
  Serial.println(F("*C"));
  Serial.println();
#endif //#ifdef DEBUG_ENABLED
  
  OS.SetTask_(checkSmokeSensor, null_delay);
}
//////////////////////////////////////////////////////////////////////////////////////

#endif //_TASKS_H
