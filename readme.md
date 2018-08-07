# MMD for M5Stack
## MMDモデルの表示 MMD model display test

<img src="https://lh3.googleusercontent.com/CfsFpF1MhOcnrgnABca7k-EsoFonvBLJA3BOnfVFR0FiwxhOHpxkPoMvOpD5GfA8d-dARM-pNvBUc6i3-nSx8GvD7bVHpg1M1KT93jbowudFrzyxb0Um3RTR90J8hpn1OHO_K6AWmWHzHkEP7OM1YY5Ckt4dA5lodj6oooT1OIoCVsC9hT3AsJEINiaKqlrhwQdvnKwxjs7P9pAxdhU8UQiNRvAB4VtYZrA_9A02EiRhHxB1omY4Npvdn2MxCaFfQQ00DjgMg2vJwICzy-SIWZq810hPwLTMML2QRXEIAwHMbHToqN_s0LQng1TNdWkuEJEJN5eLsfKds6yScie-krt1hPqcYJhafeMG7EM2J5NUe8L15AExTkF6dGWr6dz5ReULGtk7vBt6Z1OAr5nJT7xMLnI8NwjqAgsWoT7JsuYsUpg_GOgl2KK4TUBrecuZVFgjj7A6VduML3lz7UQAupS1oYc_umhyB_agm3R-ndskHMhzHvyh5Qq9cSBNqrdZP_eKiT2GOLNsgXNxHjukra4H_kduR0LeF8u3FXhoplPShSsTy_izgSReSrryW8Ai0LDfLWxFTQ1vxgnK5BX79fYca8jky2it1ky8wnErf01bd0uV0xHzuFK3RKtdpeuxOg-SwxuMWg_CKRtG_rKtrkKUeq0bpSk=w753-h951-no" alt="サンプル" width="300">
Video:(twitter) https://twitter.com/elect_gombe/status/1025893781864833024

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
