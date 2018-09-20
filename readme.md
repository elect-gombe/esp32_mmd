# MMD for M5Stack, esp32
Realtime rendering and skeletal animation on your esp32 devices. Now miku start dancing!
<img src="https://raw.githubusercontent.com/elect-gombe/esp32_mmd/master/image/miku.gif" alt="Miku" width="300">
Video:(twitter) https://twitter.com/elect_gombe/status/1035888138051190784

## How to
1. Download binary file from [release page](https://github.com/elect-gombe/esp32_mmd/releases).
2. write to your device.
### Windows
<img src="https://raw.githubusercontent.com/elect-gombe/esp32_mmd/master/image/howto_flash.png" alt="how to flash" width="300">

### using `esptool`
```
python esptool.py --chip esp32 --port <your usb device> --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0x1000 bootloader.bin 0x10000 spi_master.bin 0x8000 partitions.bin
```

## Performance
@240MHz using dual-core 320x240 2500 triangles 16.5~18fps

## License
### `poly.h`, `texture-body` and `texture-head`,
キャラクター部はPCLライセンスに基づき配布します。
一番の注意点は**初音ミクのモデルは非営利の範囲でのみ利用可能です。** 
```
「初音ミク」はクリプトン・フューチャー・メディア株式会社の著作物です。
© Crypton Future Media, INC. www.piapro.net
```
それ以外はMITライセンスに基づき利用を許可します。ただ、初音ミクのデータをリンクした時点で非営利利用に限る点に注意してください。その部分を外ればMITに基づき自由に利用できます。

A 3d model of Hatsune Miku is distributed under the PCL. **NON-COMMERCIAL USE!**
Other files are distributed under the [MIT License](http://opensource.org/licenses/mit-license.php).

## Feature
このシステムはラインスキャン法を使っています。高速かつ少ないメモリで処理可能で、数ライン分のみのｚバッファを使っているため解像度を上げることも可能です。
ただしパフォーマンスは低下します。

using scanline rendering. only one line z-buffer, fast drawing and just few memory. So you can increase resolution and get more worse performance! :) 

というか、esp32って340kBのメモリしかない。少なすぎて死にそうになった。アプリケーションかけるように、メモリを節約したいなぁ

## その他、PC上で試すとき（開発用）
SDL2が必要です。

```sh
cd ./main/
g++ *.cpp *.c -lSDL2 -DPC
./a.out
```
