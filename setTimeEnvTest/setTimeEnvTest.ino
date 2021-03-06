//温度測定プログラム
#include <M5StickC.h>
#include "SHT3X.h"   //M5Stack用環境センサユニット ver.2
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

SHT3X sht30;        //M5Stack用環境センサユニット ver.2

//現在時間取得
#include <WiFi.h>
#include "time.h"

//Ambient
#include "Ambient.h"

const char* ssid     = "tst.lsb";//WIFI名
const char* password = "oohashilab@00";//wifiパスワード

WiFiClient client;
Ambient ambient;

unsigned int channelId = 38699; // AmbientチャネルＩＤ
const char* writeKey = "40b82c743231a631"; // Ambientライトキー

#define uS_TO_S_FACTOR 1000000  // マイクロ秒から秒への変換係数Conversion factor for micro seconds to seconds
#define SLEEP_CONST  600       // スリープ時間の基準値

//関数プロトタイプ宣言
void wifi_conect(void);
void ambient_access(void);
void lcd_display(void);
void get_time(void);
void sleep_time_conf(void);
void led_blink(void);

//温度測定用変数
float temp = 0.0;                                 //温度変数
RTC_DATA_ATTR float hAveTemp = 0;                //1時間平均気温変数
float hum = 0.0;                                  //湿度変数

//時間管理用変数
char now[20];
int now_hour;
int now_min;
int now_sec;
RTC_DATA_ATTR int old_hour = 0;
int sleep_time;
int min_to_sec;
int sec;

void setup() 
{
    M5.begin(); 
    M5.Axp.ScreenBreath(10);                        //画面の輝度を少し下げる
    M5.Lcd.setRotation(3);                          //画面を("x*90°")回転させる
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(YELLOW);                    //LCDテキストの色を黄色に変更
    Wire.begin();                                   //I2Cを初期化する
    pinMode(GPIO_NUM_10, OUTPUT);
}

void loop() 
{
  led_blink();
  if(sht30.get() == 0)
  {
    temp = sht30.cTemp;               //温度取り込み
    hum = sht30.humidity;             //湿度取り込み
  }
  
  if(hAveTemp == 0)
  {
    hAveTemp = temp; 
  }
  else
  {
    hAveTemp = (hAveTemp+temp)/ 2;
  }
  wifi_conect();
  get_time();
  sleep_time_conf();
  lcd_display();
  
  if(now_hour != old_hour)
  {
    ambient_access();
    WiFi.disconnect();
    hAveTemp = 0;      //1時間平均気温リセット 
  }
  else
  {
    WiFi.disconnect();  
  }
  old_hour = now_hour;
  esp_deep_sleep(sleep_time * uS_TO_S_FACTOR);
}

//wifi接続関数
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

//時間取得関数
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
    
    now_hour = timeInfo.tm_hour;
    now_min  = timeInfo.tm_min;
    now_sec  = timeInfo.tm_sec;
}

//LCD表示関数
void lcd_display(void)
{
   //データをLCDに表示
   M5.Lcd.setTextSize(1);                        //テキストサイズを２に変更
   M5.Lcd.setTextColor(YELLOW,BLACK);            //テキストの色を黄色に変更
   M5.Lcd.setCursor(20, 0);
   M5.Lcd.println(now_min);//現在分を表示
   M5.Lcd.setCursor(20, 15);
   M5.Lcd.println(min_to_sec);//現在分を10で割った余りを秒に直したものを表示
   M5.Lcd.setCursor(20, 30);
   M5.Lcd.println(sec);//現在秒とmin_to_secを足したもの
   M5.Lcd.setCursor(20, 50);
   M5.Lcd.println(sleep_time);//600からsecを引いたあまり
   M5.Lcd.setCursor(10, 65);
   M5.Lcd.println(now);//現在時刻を表示
}

//スリープ時間決定関数
void sleep_time_conf(void)
{
  min_to_sec = (now_min%10)*60;
  sec = now_sec + min_to_sec;
  sleep_time = SLEEP_CONST - sec;
}

//アンビエントアクセス関数
void ambient_access()
{
    ambient.begin(channelId, writeKey, &client); //チャネルIDとライトキーを指定してAmbientの初期化

    //Ambientに送信するデータをセット 
    ambient.set(1, temp);
    ambient.set(2, hum);
    ambient.set(3, hAveTemp);
    
    ambient.send();                   //ambientにデータを送信
    
}

void led_blink()
{
  digitalWrite(GPIO_NUM_10, LOW);
  delay(500);
  digitalWrite(GPIO_NUM_10, HIGH);
}