// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// =================================================================
// Median Filter Configuration
// 이 값을 3, 10, 30으로 변경하며 테스트하세요.
#define WINDOW_SIZE 3
// =================================================================

// Configurable parameters
#define SND_VEL 346.0      // Sound velocity at 24 Celsius degree (unit: m/sec)
#define INTERVAL 25        // Sampling interval (unit: msec)
#define PULSE_DURATION 10  // Ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100      // Minimum distance to be measured (unit: mm)
#define _DIST_MAX 300      // Maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // Maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // Coefficient to convert duration to distance

// Global variables for Median Filter
float samples[WINDOW_SIZE]; // Array to store recent N samples
int sample_index = 0;       // Current index for the samples array

// Global variables
unsigned long last_sampling_time; // unit: msec

// Function to get a distance reading from USS (unit: mm)
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}

// =================================================================
// Median Filter Function
// =================================================================
float median_filter(float new_sample) {
  // 1. Add the new sample to the circular array
  samples[sample_index] = new_sample;
  sample_index = (sample_index + 1) % WINDOW_SIZE;

  // 2. Create a temporary array to sort the data
  // (Do not sort the original 'samples' array)
  float sorted_samples[WINDOW_SIZE];
  for (int i = 0; i < WINDOW_SIZE; i++) {
    sorted_samples[i] = samples[i];
  }

  // 3. Sort the temporary array (using a simple bubble sort)
  for (int i = 0; i < WINDOW_SIZE - 1; i++) {
    for (int j = 0; j < WINDOW_SIZE - i - 1; j++) {
      if (sorted_samples[j] > sorted_samples[j + 1]) {
        float temp = sorted_samples[j];
        sorted_samples[j] = sorted_samples[j + 1];
        sorted_samples[j + 1] = temp;
      }
    }
  }

  // 4. Return the median value (the middle element)
  return sorted_samples[WINDOW_SIZE / 2];
}

void setup() {
  // Initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // Initialize serial port
  Serial.begin(57600);

  // Initialize the median filter's sample array with the first reading
  // This helps stabilize the filter at the beginning.
  float first_sample = USS_measure(PIN_TRIG, PIN_ECHO);
  for (int i = 0; i < WINDOW_SIZE; i++) {
    samples[i] = first_sample;
  }
}

void loop() {
  float dist_raw, dist_median;
  
  // Wait until next sampling time.
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // Get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  // Apply the Median Filter
  dist_median = median_filter(dist_raw);
  
  // =================================================================
  // [수정됨] 시리얼 플로터 출력을 위해 raw 값과 median 값만 쉼표로 구분하여 전송합니다.
  // =================================================================
  Serial.print(dist_raw);
  Serial.print(",");
  Serial.println(dist_median);

  // LED Control (based on the filtered value)
  // 측정 실패(0) 또는 범위 밖의 값일 경우 LED를 끕니다.
  if (dist_median <= _DIST_MIN || dist_median >= _DIST_MAX)
    digitalWrite(PIN_LED, LOW);   // LED OFF if outside the range
  else
    digitalWrite(PIN_LED, HIGH);  // LED ON if inside the range

  // Update last sampling time
  last_sampling_time += INTERVAL;
}
