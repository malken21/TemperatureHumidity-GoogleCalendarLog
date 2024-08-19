#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <time.h>
#include "config.h"

// DHTセンサー 初期化
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);

  // WiFi 接続開始
  WiFi.begin(SSID, PASSWORD);
  // WiFi 接続待ち
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  // DHTセンサー 初期化
  dht.begin();

  // 時刻取得のため NTP設定
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "time.cloudflare.com");
}

void loop()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    delay(1000);
    return;
  }

  // 毎時00分にGETリクエストを送信
  if (timeinfo.tm_min == 0)
  {
    // 3回 繰り返す (再試行回数 3回)
    for (int i = 0; i < 3; i++)
      if (run())
        break;
      else
        delay(2000);
    // 次のチェックまで1分待機
    delay(60000);
  }

  // 30秒ごとにチェック
  delay(30000);
}

// 温度と湿度を取得して、GAS にデータを送信する
// 成功したら true が返される
// 失敗したら false が返される
bool run()
{
  // センサーの値を取得
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // もしセンサーの値の取得に失敗したら
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }

  // 温度
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
  // 湿度
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  return true;
}
