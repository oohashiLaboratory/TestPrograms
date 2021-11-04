#include <M5StickC.h>

void led_blink(void);

void setup(){
  M5.begin();

  // LED ON(GPIO_NUM_10 or M5_LED)
  pinMode(GPIO_NUM_10, OUTPUT);
  
}

void loop() 
{
  led_blink();
}

void led_blink()
{
  digitalWrite(GPIO_NUM_10, LOW);
  delay(500);
  digitalWrite(GPIO_NUM_10, HIGH);
  delay(1000);
  
}
