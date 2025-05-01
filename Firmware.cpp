#include <math.h>

//Pins
#define BATTERY_VOLT_PIN    33
#define BATTERY_CURRENT_PIN 32
#define TEMP_PIN            36 
#define CHARGE_ENABLE_PIN   2 

// Voltage measurement parameters
const float R4 = 1000.0;
const float R5 = 1000.0;
const float R6 = 1500.0;
const float VOLTAGE_DIVIDER_FACTOR = (R4 + R5 + R6) / (R5 + R6);

// ADC parameters for the ESP32:
const float ADC_REF_VOLTAGE = 3.3;
const int ADC_RESOLUTION = 4095;

// Current sensing parameters:
const float R_SHUNT = 0.1;
const float CURRENT_AMPLIFIER_GAIN = 4.7;

// Thermistor parameters for temperature
const float SERIES_RESISTOR     = 10000.0;
const float THERMISTOR_NOMINAL  = 10000.0;
const float TEMPERATURE_NOMINAL = 25.0;
const float BETA                = 3950.0;

const float SAFE_TEMP_LOW  = 10.0;
const float SAFE_TEMP_HIGH = 40.0;

//convert thermistor ADC reading to temperature
float readTemperature() {
    int rawTemp = analogRead(TEMP_PIN);
    if (rawTemp == 0) rawTemp = 1;
    float voltage = (rawTemp * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    
    // Calculate  thermistor resistance:
    float resistance = SERIES_RESISTOR * ((ADC_REF_VOLTAGE / voltage) - 1);
    
    //calculate temperature:
    float temperatureC = (1.0 / ((log(resistance / THERMISTOR_NOMINAL) / BETA) + (1.0 / (TEMPERATURE_NOMINAL + 273.15)))) - 273.15;
    
    return temperatureC;
}

void setup() {
    Serial.begin(115200);
    
    pinMode(CHARGE_ENABLE_PIN, OUTPUT);
    digitalWrite(CHARGE_ENABLE_PIN, LOW);
}

void loop() {
    // Read ADC values for voltage and current:
    int adcVoltage = analogRead(BATTERY_VOLT_PIN);
    int adcCurrent = analogRead(BATTERY_CURRENT_PIN);
    
    // Convert ADC values
    float measuredVoltage = (adcVoltage * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    float measuredShuntVoltage = (adcCurrent * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    
    // Calculate battery voltage
    float batteryVoltage = measuredVoltage * VOLTAGE_DIVIDER_FACTOR;
    
    // Calculate battery current:
    float batteryCurrent = measuredShuntVoltage / (CURRENT_AMPLIFIER_GAIN * R_SHUNT);
    
    // Read temperature in Celsius:
    float batteryTemperature = readTemperature();
    
    // Output
    Serial.print("Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.println(" V");
    
    Serial.print("Battery Current: ");
    Serial.print(batteryCurrent);
    Serial.println(" A");
    
    Serial.print("Battery Temperature: ");
    Serial.print(batteryTemperature);
    Serial.println(" °C");
    
    delay(1000);
}
