#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10

Servo myServo;
unsigned long MOVING_TIME = 3000;
unsigned long moveStartTime;
int startAngle = 30;
int stopAngle  = 90;

void setup() {
  myServo.attach(PIN_SERVO);
  moveStartTime = millis();
  myServo.write(startAngle);
  delay(500);
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    // 1. 시간 진행률 계산 (0.0 ~ 1.0)
    float timeRatio = (float)progress / MOVING_TIME;

    // 2. 코사인 함수를 이용해 움직임 진행률 계산
    float easeRatio = cosineEaseInOut(timeRatio);

    // 3. 최종 각도 계산 및 적용
    int totalAngleRange = stopAngle - startAngle;
    int angle = startAngle + (easeRatio * totalAngleRange);
    myServo.write(angle);

  } else {
    // 방향 전환
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;
    moveStartTime = millis();
  }
}

/**
 * @brief 코사인 함수를 이용한 가감속 (Cosine Ease-In/Out)
 * @param t 시간 진행률 (0.0 ~ 1.0)
 * @return 변환된 움직임 진행률 (0.0 ~ 1.0)
 */
float cosineEaseInOut(float t) {
  return 0.5 * (1.0 - cos(t * PI));
}
