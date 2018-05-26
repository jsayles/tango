#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_NeoPixel.h>


// Hardware
#define PIXEL_PIN       8
#define BUTTON1_PIN     0
#define BUTTON2_PIN     5
#define LED_PIN         13

// Preferences
#define PIXEL_BRIGTNESS 42
#define INTERVAL_MS     100

// States
#define CALIBRATING     0
#define STOPPED         1
#define TRACKING        2
#define RECORDING       3


// Bosch BNO55 Sensor
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Adafruit NeoPixel LED
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// System State
uint8_t state = CALIBRATING;


unsigned long previousMillis = 0;

float starting_x_position = 0;
float starting_y_position = 0;
float starting_z_position = 0;

int ledState = LOW; 

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
/*    Button Methods                                                      */
/**************************************************************************/

void button1_press(void) {
  digitalWrite(LED_PIN, HIGH);
  if (bno.isFullyCalibrated()) {
    Serial.println("Switching to TRACKING");
    state = TRACKING;
    pixel_white();
    sensors_event_t event;
    bno.getEvent(&event);
    starting_x_position = event.orientation.x;
    starting_y_position = event.orientation.y;
    starting_z_position = event.orientation.z;
  }
  delay(1000);
}

void button2_press(void) {
  digitalWrite(LED_PIN, LOW);  
  if (state == TRACKING) {
    Serial.println("Switching to RECORDING");
    state == RECORDING;
  } else if (state == RECORDING) {
    Serial.println("Switching to STOPPED");
    state == STOPPED;
  }
  delay(1000);
}


/**************************************************************************/
/*    State Methods                                                       */
/*                                                                        */
/**************************************************************************/

void calibrate(void) {
  // There are four calibration values (0..3) 
  // Any sensor data reporting 0 should be ignored
  // 3 means 'fully calibrated" 
  uint8_t sys, gyro, accel, mag;
  sys = gyro = accel = mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  // The data should be ignored until the system calibration is > 0
  if (sys == 0 || gyro == 0 || accel == 0 || mag == 0) {
    Serial.print("! ");
    pixel_red();
  } else if (sys >= 2 && gyro >=2 && accel >=2 && mag >=2) {
    pixel_green();
  } else {
    pixel_orange();
  }

  // Display the individual values
  Serial.print("System:");
  Serial.print(sys, DEC);
  Serial.print(" Gyroscope:");
  Serial.print(gyro, DEC);
  Serial.print(" Accelerometer:");
  Serial.print(accel, DEC);
  Serial.print(" Magnetometer:");
  Serial.print(mag, DEC);
  Serial.println(); 
}

void track(void) {
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print("X: ");
  Serial.print(event.orientation.x - starting_x_position, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y - starting_y_position, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z - starting_z_position, 4); 
  Serial.println(); 
}

void record(void) {
  Serial.println("recording blah blah blah.....");
}

/**************************************************************************/
/*    Arduino Methods                                                     */
/*                                                                        */
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

  // Setup our neopixel
  pixel.begin();

  // Setup our status LED
  pinMode(LED_PIN, OUTPUT);

  // Setup our buttons
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), button1_press, CHANGE);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), button2_press, CHANGE);
}

void loop(void) {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > INTERVAL_MS) {
    previousMillis = currentMillis;  

    // Run the appropriate method for the state
    switch(state) {
      case CALIBRATING:
        calibrate();
        break;
      case TRACKING:
        track();
        break;
      case RECORDING:
        record();
        break;
    }

    // Toggle our LED
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(LED_PIN, ledState);
  }
}
