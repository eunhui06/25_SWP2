const int ledPin = 7;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 연결 방식이 반대이므로 LOW가 켜짐, HIGH가 꺼짐.

  // 1. 처음 1초 동안 LED 켜기
  digitalWrite(ledPin, LOW); // LOW로 켜기
  delay(1000);
  
  // 2. 다음 동작을 위해 1초 동안 LED 끄기
  digitalWrite(ledPin, HIGH); // HIGH로 끄기
  delay(1000);

  // 3. 0.2초 간격으로 LED 5번 깜빡이기
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, LOW);  // LOW로 켜기
    delay(200);
    digitalWrite(ledPin, HIGH); // HIGH로 끄기
    delay(200);
  }

  // 4. 모든 동작이 끝난 후 LED를 확실히 끄고 프로그램을 정지
  digitalWrite(ledPin, HIGH); // HIGH로 최종 끄기

  while (1) {
    // 이 무한 루프로 loop() 함수가 다시 시작되는 것을 막아
    // LED가 꺼진 상태로 유지.
  }
}
