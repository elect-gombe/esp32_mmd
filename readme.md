# MMD with ESP32
## MMDモデルの表示 MMD model display test

<img src="http://i.imgur.com/9LDjwK9.jpg" alt="サンプル" width="300">
Video:(twitter) https://twitter.com/elect_gombe/status/872766291135692800

Mはげさんのソースコードから拝借して表示しました。
# How to
M5Stackにバイナリを書き込んでください。

## Performance
@240MHz using dual-core 320x240 300 triangles 14fps~21fps.

## License
### `poly.h`, `texture-body` and `texture-head`,
キャラクター部はPCLライセンスに基づき配布します。
一番の注意点は**初音ミクのモデルは非営利の範囲でのみ利用可能です。** 

>> 「初音ミク」はクリプトン・フューチャー・メディア株式会社の著作物です。
>> © Crypton Future Media, INC. www.piapro.net

A 3d model of Hatsune Miku is distributed under the PCL. NON-COMMERCIAL USE ONLY!

それ以外はMITライセンスに基づき利用を許可します。ただ、初音ミクのデータをリンクした時点で非営利利用に限る点に注意してください。その部分を外ればMITに基づき自由に利用できます。

Other files are distributed under the MIT License.
http://opensource.org/licenses/mit-license.php

## Feature
このシステムはラインスキャン法を使っています。高速かつ少ないメモリで処理可能で、数ライン分のみのｚバッファを使っているため解像度を上げることも可能です。
ただしパフォーマンスは低下します。

using scanline rendering. only one line z-buffer, fast drawing and just few memory. So you can increase resolution and get more worse performance! :) 


http://gombe.azurewebsites.net/2017/05/14/%e3%83%9e%e3%82%a4%e3%82%b3%e3%83%b3%e3%81%a73d%e8%a1%a8%e7%a4%ba%e5%9b%b3%e8%a7%a3/
