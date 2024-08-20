# TemperatureHumidity-GoogleCalendarLog

ESP32とGoogle Apps Scriptを使って気温と湿度のデータをGoogleカレンダーに記録する。

DHTセンサー (初期設定ではDHT11) から読み取った 温度と湿度を 30分毎に Googleカレンダーに記録されます。

[./ESP32/main](./ESP32/main) は esp32 側のコードで、
[./GAS](./GAS) が GAS 側のコードです。

esp32 のライブラリは
[DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library)
を使っています。

DHTセンサーの回路については
<https://randomnerdtutorials.com/complete-guide-for-dht11dht22-humidity-and-temperature-sensor-with-arduino/>
この記事を参考にしてください。
