#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_NeoPixel.h>


// Hardware
#define PIXEL_PIN       8
#define BUTTON1_PIN     5
#define BUTTON2_PIN     0
#define LED_PIN         13

// Preferences
#define PIXEL_BRIGTNESS 42
#define INTERVAL_MS     150

// States
#define CALIBRATING     0
#define RUNNING         1
#define TRACKING        2
#define RECORDING       3
#define STOPPED         4

// Bosch BNO55 Sensor
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Adafruit NeoPixel LED
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// State Variables
uint8_t run_state = CALIBRATING;
uint8_t button1_state = HIGH;
uint8_t button2_state = HIGH;
unsigned long previousMillis = 0;


/**************************************************************************/
/*    BNO Sensor Methods                                                  */
/**************************************************************************/

// Displays some basic information on this sensor from the unified
// sensor API sensor_t type (see Adafruit_Sensor for more information)
void displaySensorDetails(void) {
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

// Display some basic info about the sensor status
void displaySensorStatus(void) {
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*    Pixel Methods                                                       */
/*                                                                        */
/*  pixel.Color takes RGB values, from 0,0,0 up to 255,255,255            */
/**************************************************************************/

void set_pixel(uint8_t r, uint8_t g, uint8_t b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show(); 
  
}

void pixel_red(void) {
  set_pixel(PIXEL_BRIGTNESS, 0, 0);
}

void pixel_orange(void) {
  set_pixel(PIXEL_BRIGTNESS, PIXEL_BRIGTNESS, 0);
}

void pixel_green(void) {
  set_pixel(0, PIXEL_BRIGTNESS, 0);
}

void pixel_blue(void) {
  set_pixel(0, 0, PIXEL_BRIGTNESS);
}

void pixel_purple(void) {
  set_pixel(PIXEL_BRIGTNESS, 0, PIXEL_BRIGTNESS);
}

void pixel_white(void) {
  set_pixel(PIXEL_BRIGTNESS, PIXEL_BRIGTNESS, PIXEL_BRIGTNESS);
}

void pixel_off(void) {
  set_pixel(0, 0, 0);
}


/**************************************************************************/
/*    State Methods                                                       */
/**************************************************************************/

void stateToRunning(void) {
  run_state = RUNNING;
  pixel_green();
}

void stateToTracking(void) {
  run_state = TRACKING;
  pixel_white();
}

void stateToRecording(void) {
  run_state = RECORDING;
  pixel_blue();
}

void calibrate(void) {
  // Stop calibrating when we are finished
  if (bno.isFullyCalibrated()) {
    Serial.println("Sensor Fully Calibrated!");
    if (run_state == CALIBRATING) {
      stateToRunning();
    }
    // Exit now if we are already calibrated
    return;
  } else {
    pixel_red();
  }
  
  // There are four calibration values (0..3) 
  // Any sensor data reporting 0 should be ignored
  // 3 means 'fully calibrated" 
  // The data should be ignored until the system calibration is > 0
  uint8_t sys, gyro, accel, mag;
  sys = gyro = accel = mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  // Display the individual values
  Serial.print("System:");
  Serial.print(sys, DEC);
  Serial.print(" Gyroscope:");
  Serial.print(gyro, DEC);
  Serial.print(" Accelerometer:");
  Serial.print(accel, DEC);
  Serial.print(" Magnetometer:");
  Serial.print(mag, DEC);
}

void printRawData(void) {  
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print(event.orientation.x);
  Serial.print(",");
  Serial.print(event.orientation.y);
  Serial.print(",");
  Serial.print(event.orientation.z); 
}

/**************************************************************************/
/*    Arduino Methods                                                     */
/**************************************************************************/

void setup(void) {
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  // Initialise the sensor
  if(!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);

  // Display some basic information on this sensor
  displaySensorDetails();
  displaySensorStatus();

  // This board has an external crystal
  bno.setExtCrystalUse(true);

  // Start our neopixel
  pixel.begin();

  // Setup our status LED and our buttons
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void loop(void) {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > INTERVAL_MS) {
    previousMillis = currentMillis;  
    button1_state = digitalRead(BUTTON1_PIN);    
    button2_state = digitalRead(BUTTON2_PIN);
    
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("] ");

    // Catch a CANCEL request on Button 2
    if (button2_state == LOW) {
      Serial.println("CANCEL");
      stateToRunning();
      return;
    }
    
    // Run the appropriate method for the state
    switch(run_state) {
      case CALIBRATING:
        Serial.print("CALIBRATING | ");
        calibrate();
        break;
      case RUNNING:
        if (button1_state == LOW) {
          stateToTracking();
        } else {
          Serial.print("RUNNING | ");
        }
        break;
      case TRACKING:
        if (button1_state == LOW) {
          stateToRecording();
        } else {
          Serial.print("TRACKING | ");
          printRawData();
        }
        break;
      case RECORDING:
        if (button1_state == LOW) {
          stateToRunning();
        } else {
          Serial.print("RECORDING | ");
          printRawData();
        }
        break;
    }
    
    // Toggle our LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
    // End of the loop 
    Serial.println(); 
  }
}
