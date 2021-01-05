#ifndef _TASKS_H
#define _TASKS_H

namespace Tasks
{
	void checkTemp(void);
	void startFiring(void);
	void setFanSpeed(void);
}

void Tasks::checkTemp(void){
	
	curr_tempC = inbox_tempC.getTempC();
  
#ifdef DEBUG_ENABLED
	Serial.print(F("Checking temp..."));
	Serial.print(curr_tempC);
	Serial.println(F("*C"));
#endif //#ifdef DEBUG_ENABLED
	
	OS.SetTask_(setFanSpeed, null_delay);
}

void Tasks::startFiring(void){
	
		analogWrite(fan_pin1, fan_pwm_max);
		
		OS.SetTask_(Tasks::checkTemp, temp_check_delay);
    
#ifdef DEBUG_ENABLED    
		Serial.println(F("Firing...\0"));
#endif //#ifdef DEBUG_ENABLED
}

void Tasks::setFanSpeed(void){
	unsigned char fan_pwm;

	if (curr_tempC == max_temp){
    fan_pwm = fan_pwm_min;

#ifdef DEBUG_ENABLED
    Serial.println(F("PWM min...\0"));
#endif //#ifdef DEBUG_ENABLED
	}
	
	else if (curr_tempC > max_temp){
    fan_pwm = fan_pwm_stop;
    
#ifdef DEBUG_ENABLED
    Serial.println(F("PWM stop...\0"));
#endif //#ifdef DEBUG_ENABLED
	}
	
	else if ((curr_tempC > (norm_temp - 1.0)) && (curr_tempC < (norm_temp + 1.0))){
    fan_pwm = fan_pwm_mid;
    
#ifdef DEBUG_ENABLED
    Serial.println(F("PWM 50...\0"));
#endif //#ifdef DEBUG_ENABLED
	}
	
	else if (curr_tempC < (norm_temp - 1.0)){
    fan_pwm = fan_pwm_max;
	}
	
	else {
    float temp_reference = 1 - (curr_tempC / max_temp);

#ifdef DEBUG_ENABLED
    Serial.print(F("temp reference...\0"));
    Serial.println(temp_reference);
#endif //#ifdef DEBUG_ENABLED

		fan_pwm = (unsigned int)(fan_pwm_max * temp_reference) + (unsigned int)fan_pwm_additive;
		if (fan_pwm > fan_pwm_max) fan_pwm = fan_pwm_max;
   
#ifdef DEBUG_ENABLED   
    Serial.println(F("PWM calculate...\0"));
#endif //#ifdef DEBUG_ENABLED
	}

  analogWrite(fan_pin1, fan_pwm);
  
	OS.SetTask_(checkTemp, temp_check_delay);

#ifdef DEBUG_ENABLED 
	Serial.print(F("setting fan speed...\0"));
  Serial.print(fan_pwm);
  Serial.println(F("%"));
#endif //#ifdef DEBUG_ENABLED
}


#endif //_TASKS_H
