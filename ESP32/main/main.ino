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
  connect();

  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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

  // もし 分の値が 30で割り切れる場合 なら run() を実行
  if (timeinfo.tm_min % 30 == 0)
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

  // 現在のエポックタイムを取得
  time_t now;
  time(&now);

  // 今回リクエストする URL を生成
  char url[150];
  char buf_t[5];
  char buf_h[5];
  dtostrf(t, 0, 1, buf_t);
  dtostrf(h, 0, 1, buf_h);
  sprintf(url, "%s?t=%s&h=%s&d=%ld", URL, buf_t, buf_h, now);

  // WiFi に接続できなかったら
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed to connect to WiFi!");
    return false;
  }

  HTTPClient http;
  http.begin(url); // URLにアクセス
  int httpCode = http.GET();
  Serial.print("StatusCode: ");
  Serial.println(httpCode);
  http.end(); // 接続を閉じる

  // もし http ステータスコードが 400番 以上だったら false
  if (httpCode >= 400)
    return false;
  return true;
}

void connect()
{
  // 試行時間
  unsigned long attemptTime = millis();

  // 接続が成功するか、5秒経過するまで待つ
  while (WiFi.status() != WL_CONNECTED && millis() - attemptTime < 5000)
  {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }

  // もし接続できなかったら
  if (WiFi.status() != WL_CONNECTED)
  {
    // 再接続を試みる
    WiFi.disconnect();
    WiFi.reconnect();
    connect();
  }
}