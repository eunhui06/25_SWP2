#define PIN_LED 13
unsigned int count = 0;
int toggle = 0; // 변수 선언 시 초기화

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // 시리얼 포트가 연결될 때까지 대기
  }
  Serial.println("Hello World!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  Serial.println(++count);

  // if문을 사용하여 toggle 변수의 값을 0이면 1로, 1이면 0으로 변경
  if (toggle == 0) {
    toggle = 1;
  } else {
    toggle = 0;
  }
  
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}
