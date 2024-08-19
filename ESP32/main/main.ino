#include "DHT.h"
#include "config.h"

// DHTセンサー 初期化
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  delay(2000);
  run();
}

void run()
{
  // センサーの値を取得
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // もしセンサーの値の取得に失敗したら
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // 温度
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
  // 湿度
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}