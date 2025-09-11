#define PIN_LED 13
unsigned int count = 0;
int toggle = 0; //변수 선언 시 초기화

void setup() {
  pinMode(PIN_LED,OUTPUT);
  Serial.begin(115200);
  while (!Serial){
    ;
  }
  Serial.println("Hello World!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  Serial.println(++count);
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}

int toggle_state(int current_state){
  return !current_state; //0이면 1, 1이면 0으로 변환
}
