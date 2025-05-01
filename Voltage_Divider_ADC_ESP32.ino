// Note, you can comment out the shunt resistor sections, I just thought that we were calculating the current using another circuit made from the battery cell.


#define VOUT_PIN 34   // ADC pin for voltage divider output
#define VSHUNT_PIN 35 // ADC pin for shunt resistor voltage drop

// Constants for calculation
const float R1 = 100000.0; // 100kΩ     - Can be any resistor, this is just a placeholder
const float R2 = 33000.0;  // 33kΩ      - Can be any resistor, this is just a placeholder
const float ADC_REF_VOLTAGE = 3.3;  // ESP32 ADC reference voltage
const int ADC_RESOLUTION = 4095;   // ESP32 ADC is 12-bit (0-4095)
const float R_SHUNT = 0.1; // 0.1Ω shunt resistor     -   This should be as low as possible

void setup() {
    Serial.begin(115200);
}

void loop() {
    // Read ADC values
    int adc_vout = analogRead(VOUT_PIN);
    int adc_vshunt = analogRead(VSHUNT_PIN);
    
    // Convert ADC reading to voltage
    float vout = (adc_vout * ADC_REF_VOLTAGE) / ADC_RESOLUTION;          // for voltage calculation
    float vshunt = (adc_vshunt * ADC_REF_VOLTAGE) / ADC_RESOLUTION;      // for current calculation

    // Calculate actual battery voltage
    float batteryVoltage = vout * ((R1 + R2) / R2);

    // Calculate current using Ohm’s Law
    float batteryCurrent = vshunt / R_SHUNT;

    // Print results
    Serial.print("Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.println(" V");

    Serial.print("Battery Current: ");
    Serial.print(batteryCurrent);
    Serial.println(" A");

    delay(1000); // Update every second
}
