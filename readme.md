# MMD with ESP32
## MMDモデルの表示 MMD model display test

<img src="http://i.imgur.com/qks27xH.jpg" alt="サンプル" width="200">

Mはげさんのソースコードから拝借して表示しました。

# How to

## Pin assign
ST7735S(160x128)を使います。
for ST7735S (160x128)

```c
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
```


## Performance
@240MHz 160x128 about 30000 triangles/sec. not bad, not good.

## memo
このシステムはラインスキャン法を使っています。高速かつ少ないメモリで処理可能で、一ライン分のみのｚバッファを使っているため解像度を上げることも可能です。
ただしパフォーマンスは低下します。

http://gombe.azurewebsites.net/2017/05/14/%e3%83%9e%e3%82%a4%e3%82%b3%e3%83%b3%e3%81%a73d%e8%a1%a8%e7%a4%ba%e5%9b%b3%e8%a7%a3/
