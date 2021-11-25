#include <M5StickC.h>
#include <WiFi.h>

const char* ssid     = "HUAWEI-F4E1";//WIFI名
const char* password = "64141340";//wifiパスワード

void wifi_conect(void);
void led_blink(void);

void setup()
{
  M5.begin();
  // LED ON(GPIO_NUM_10 or M5_LED)
  M5.Axp.ScreenBreath(10);                        //画面の輝度を少し下げる
  M5.Lcd.setRotation(3);                          //画面を("x*90°")回転させる
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(YELLOW);                    //LCDテキストの色を黄色に変更
  pinMode(GPIO_NUM_10, OUTPUT);
}

void loop() 
{
  wifi_conect();
}

void wifi_conect(void)
{
    WiFi.begin(ssid, password);  //  Wi-Fi APに接続 
    while (WiFi.status() != WL_CONNECTED) //  Wi-Fi AP接続待ち
    {
       digitalWrite(GPIO_NUM_10, LOW);
       delay(250);
       digitalWrite(GPIO_NUM_10, HIGH);
       delay(250);
    }
    Serial.print("WiFi connected\r\nIP address: ");
    Serial.println(WiFi.localIP());
}
