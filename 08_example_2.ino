// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  // initialize serial port
  Serial.begin(57600);
}

void loop() { 
  float distance;
  int brightness = 0;

  // wait until next sampling time.
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  // control led brightness based on distance
  if (distance >= 100.0 && distance <= 200.0) {
      // 100mm(brightness 0) -> 200mm(brightness 255) linearly increasing
      brightness = map(distance, 100, 200, 0, 255);
    } else if (distance > 200.0 && distance <= 300.0) {
      // 200mm(brightness 255) -> 300mm(brightness 0) linearly decreasing
      brightness = map(distance, 200, 300, 255, 0);
    } else {
     // Turn off LED if distance is out of range (less than 100mm or more than 300mm)
      brightness = 0;
   }

  // apply the calculated brightness to the LED
  analogWrite(PIN_LED, brightness);

  // Send only the brightness value to the serial plotter
  Serial.println(brightness);
  
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
