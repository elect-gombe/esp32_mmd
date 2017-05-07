# MMD with ESP32
## MMDモデルの表示
Mはげさんのソースコードから拝借して表示しました。
## 回路
ST7735S(160x128)を使います。Aitendoとかにあったのですが今は知りません。
```c
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
```
このとおりです。DCはA0とかRSとか別名多数ですね。全てきちんと接続してくださいよ。

## Performance
160x128で22fpsです。STM32F4に大きく遅れを取ってます。
最適化レベルは-O3で240MHz駆動です。

