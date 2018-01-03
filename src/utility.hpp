#pragma once

//
// 雑多な処理
//


// xをyで丸めこむ
int roundValue(int x, int y) {
  return (x > 0) ? (x + y / 2) / y
                 : (x - y / 2) / y;
}
