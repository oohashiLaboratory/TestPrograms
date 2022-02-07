## setTimeTest
送信時間の調整プログラムのテスト  
スリープ時間　＝ 600　-（（現在分 ％ 10）+現在秒）  

1.wifiに接続し本体の時間を更新する  
2.ローカルタイムを取得し、次の１０で割り切れる分数になるまでの秒数を計算する  
3.計算した時間分スリープする  

## ledTest
起動時状態がわかるようLEDを点滅させるためm5stickの内蔵LEDのテスト

## setTimeEnvTest
温度測定用プログラムにsetTimeおよびledTestの内容を組み込んだもの

## GravityMoisture
グラビティの土壌水分センサでデータ取得を確認するためのプログラム

## GroveCapacitiveMoisture 

Gloveの防腐性の土壌水分センサでデータ取得を確認するためのプログラム
