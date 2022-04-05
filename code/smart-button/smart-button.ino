void setup() {
  Serial.begin(9600);

  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
}

void loop() {

  int sensorVal = digitalRead(12); // 14
  int value = digitalRead(13);
  int v = digitalRead(14);
  Serial.println(sensorVal + value + v);

}
