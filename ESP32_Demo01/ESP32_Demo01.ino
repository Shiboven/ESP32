#define LED_BOARD   2

char Index = 0;
bool Dev;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BOARD, OUTPUT);
  ledcSetup(0, 1000, 8);
  ledcAttachPin(2, 0);
  
}

// the loop function runs over and over again forever
void loop() {
  if((Index==0xFF)||(Index==0))
    Dev ^= 0x01; 
  if(Dev) 
    Index++;
  else
    Index--;
  // analogWrite(LED_BOARD, Index);         // 点亮 LED
  ledcWrite(0,Index);
  delay(5);                           // 延时 1s
}
