#include <M5StickC.h>

//現在時間取得
#include <WiFi.h>
#include "time.h"

//Ambientに送信する
#include "Ambient.h"

const char* ssid     = "tst.lsb";
const char* password = "oohashilab@00";

WiFiClient client;
Ambient ambient;

unsigned int channelId = 38698; // Ambient
const char* writeKey = "bcc550e6d9ce9eba"; // ライトキー

#define uS_TO_S_FACTOR 1000000  // マイクロ秒から秒への変換係数Conversion factor for micro seconds to seconds
#define SLEEP_CONST  60       // ESP32がスリープ状態になる時間（秒単位） Time ESP32 will go to sleep (in seconds) */
//TIME_TO_SLEEP 600(10分)、1200（20分）,1800(30分)　3600（60分）

void wifi_conect(void);
void ambient_acosess(void);
void lcd_display(void);
void get_time(void);
void sleep_time_conf(void);
void ambient_acosess(void);

char now[20];
char now_hour;
char now_min;
char now_sec;
RTC_DATA_ATTR char old_hour = 0;

char sleep_time;

void setup() 
{
    M5.begin(); 
    M5.Axp.ScreenBreath(10);                        //画面の輝度を少し下げる
    M5.Lcd.setRotation(3);                          //画面を("x*90°")回転させる
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);                    //LCDテキストの色を黄色に変更
}

void loop() 
{
  wifi_conect();
  get_time();
  WiFi.disconnect();  
  lcd_display();
  if(now_hour != old_hour)
  {
    ambient_acosess();
  }
  old_hour = now_hour;
  esp_deep_sleep(sleep_time * uS_TO_S_FACTOR);
}

void wifi_conect(void)
{
    WiFi.begin(ssid, password);  //  Wi-Fi APに接続 
    while (WiFi.status() != WL_CONNECTED) //  Wi-Fi AP接続待ち
    {
       delay(500);
       Serial.print(".");
    }
    Serial.print("WiFi connected\r\nIP address: ");
    Serial.println(WiFi.localIP());
}

void get_time(void)
{
    configTime(9 * 3600, 0, "ntp.nict.jp"); // NTP時間をローカルに設定　/Set ntp time to local
    struct tm timeInfo;
    getLocalTime(&timeInfo); //ローカル時間を取得
  
    sprintf(now, "%04d/%02d/%02d %02d:%02d:%02d",
      timeInfo.tm_year + 1900,
      timeInfo.tm_mon + 1,
      timeInfo.tm_mday,
      timeInfo.tm_hour,
      timeInfo.tm_min,
      timeInfo.tm_sec
    );
    
    now_hour = timeInfo.tm_mday;
    now_min  = timeInfo.tm_min;
    now_sec  = timeInfo.tm_sec;
}

void lcd_display(void)
{
   //データをLCDに表示
   M5.Lcd.setTextSize(1);                        //テキストサイズを２に変更
   M5.Lcd.setTextColor(YELLOW,BLACK);            //テキストの色を黄色に変更
   M5.Lcd.setCursor(20, 20);
   M5.Lcd.println(now_min);
   M5.Lcd.setCursor(20, 40);
   M5.Lcd.println(old_hour);
   M5.Lcd.setCursor(10, 60);
   M5.Lcd.println(now);
}

void sleep_time_conf(void)
{
//  char min_to_sec;
//  char sec;
//  min_to_sec = now_min%10;
//  sec = now_sec + min_to_sec;
//  sleep_time = SLEEP_CONST - sec;
  sleep_time = SLEEP_CONST - now_sec;
}

void ambient_acosess(void)
{
    //amibientにデータを送信する
    ambient.begin(channelId, writeKey, &client); // チャネルIDとライトキーを指定してAmbientの初期化

    // 積算温度、平均気温の値をAmbientに送信する 
    ambient.set(1,now_min);
    ambient.set(2,old_hour);
    ambient.set(3,5);
 
    ambient.send();
}
