/*
 * Sigmoid Servo Control Example
 * Original code by ArduinoGetStarted.com, modified with Sigmoid function.
 */

#include <Servo.h>
#include <math.h> // exp() 함수를 사용하기 위해 math 라이브러리를 포함합니다.

#define PIN_SERVO 10

Servo myServo;
unsigned long MOVING_TIME = 3000; // 이동 시간은 3초
unsigned long moveStartTime;
int startAngle = 30; // 30°
int stopAngle  = 90; // 90°

void setup() {
  myServo.attach(PIN_SERVO);
  moveStartTime = millis(); // 시작 시간 기록

  myServo.write(startAngle); // 초기 위치로 이동
  delay(500);
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    // --- 이 부분이 Sigmoid 로직으로 변경 ---

    // 1. 시간 진행률(0.0 ~ 1.0)을 시그모이드의 입력 범위(-6.0 ~ 6.0)로 변환합니다.
    //    이 범위를 사용하면 함수의 S-커브를 효과적으로 활용할 수 있습니다.
    float x = (float)progress / MOVING_TIME * 12.0 - 6.0;

    // 2. 시그모이드 함수를 계산합니다. 결과는 0.0 ~ 1.0 사이의 값이 나옵니다.
    float sigmoidOutput = 1.0 / (1.0 + exp(-x));

    // 3. 시그모이드 결과(0.0~1.0)를 실제 움직일 각도 범위로 변환합니다.
    float totalAngleRange = stopAngle - startAngle;
    int angle = startAngle + (sigmoidOutput * totalAngleRange);
    
    myServo.write(angle);
    
  } else {
    // 움직임이 끝나면 방향을 전환합니다.
    int temp = startAngle;
    startAngle = stopAngle;
    stopAngle = temp;

    moveStartTime = millis(); // 다음 움직임을 위해 시작 시간 재설정
  }
}
