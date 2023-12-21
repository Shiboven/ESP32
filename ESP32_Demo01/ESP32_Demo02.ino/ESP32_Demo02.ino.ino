
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(4));
  analogWrite(2, analogRead(4)/16);
  delay(10);
}
