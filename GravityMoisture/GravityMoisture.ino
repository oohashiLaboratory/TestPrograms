//anarogread でP33から受け取る
//水中及び空気中で出る値を定数として記録しそれから率を求める  //空気中3040(P36で測定)
//LCD下のボタンを押したら値を取得する
#include <M5StickC.h>

//セットアップ
void setup()
{
  M5.begin();                                     //初期化関数
  M5.Lcd.setRotation(3);                          //画面を("x*90°")回転させる
  M5.Lcd.setTextColor(YELLOW,BLACK);              //テキストの色を黄色,背景を黒に変更
  M5.Lcd.setCursor(10, 2);                        //LCDのカーソル位置変更
   M5.Lcd.printf("Moisture Sensor\n");
  //pinMode(32, INPUT);                           //pinモードをインプットに変更
  M5.Lcd.setCursor(10, 30);                       //LCDのカーソル位置変更
  M5.Lcd.print("Moisture = ");
}

int sensorPin = 36;
uint16_t sensorValue = 0;

void loop()
{
    M5.update();        //ボタン情報の更新
    if ( M5.BtnA.wasPressed() )//LCD下部のボタンが押されたとき
     {
     sensorValue = analogRead(sensorPin);//データ取得
     delay(100);
     M5.Lcd.setCursor(10, 30);                  //LCDのカーソル位置変更
     M5.Lcd.print("Moisture = ");
     M5.Lcd.setCursor(80, 30);                       //LCDのカーソル位置変更
     M5.Lcd.printf("%d\n",sensorValue);
     }
}
