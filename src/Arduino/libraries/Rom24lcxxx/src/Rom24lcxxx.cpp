/**
 Rom24lcxxx.h


**/

#include <Wire.h>
#include <Arduino.h>

#include "Rom24lcxxx.h"

static int slave_addr ;

#define CTRL_CODE  0b01010000  // EEPROM 24xxシリーズの制御コード("1010")


Rom24lcxxx::Rom24lcxxx(int A0,int A1,int A2) {

	Wire.begin() ;
	slave_addr = CTRL_CODE | (A2<<2) | (A1<<1) | A0 ;
}

/*
*    EEPROMから指定個数だけデータを読み出す処理                                *
*                                                                              *
*    address : 読み出し開始のデータアドレス位置を指定します                    *
*    *data: 読み出したデータを保存する為の配列変数を指定します                 *
*    num  : 読み出すデータの個数を指定します(この個数分だけ配列変数は確保する) *
*    ret  : 戻り値、0=正常終了　それ以外I2C通信エラー下記                      *
*                   1=送ろうとしたデータが送信バッファのサイズを超えた         *
*                   2=スレーブ・アドレスを送信し、NACKを受信した               *
*                   3=データ・バイトを送信し、NACKを受信した                   *
*                   4=その他のエラー                                           *
*                   5=データ受信エラー                                         *

*/

int Rom24lcxxx::Read(long address,char *data,int num) {
	byte x ;
	int i , ret ;

	Wire.beginTransmission(slave_addr) ;
	x = (byte)(address >> 8) ;
	Wire.write(x) ;
	x = (byte)(address & 0x00ff) ;
	Wire.write(x) ;
	ret = Wire.endTransmission() ;
	if (ret == 0) {
		ret = Wire.requestFrom(slave_addr,num) ;
		if (ret == num) {
			for (i=0 ; i<num ; i++ ) {
				*data = Wire.read() ;
				data++ ;
			}
			ret = 0 ;
		} else {
			ret = 5 ;
		}
	}
	return ret ;
}

/*
*    EEPROMに指定個数だけデータを書き込む処理                                  *
*                                                                              *
*    address : 書き込み開始のデータアドレス位置を指定します                    *
*    *data: 書き込むデータを保存した配列変数を指定します                       *
*    num  : 書き込むデータの個数を指定します                                   *
*           デバイスのページサイズ以上は連続書き込みは出来ません               *
*           24xx32/64は32byte   24xx128/256は64byte  24xx512は128byte          *
*           (WireライブラリのバッファがMAX32byteまでと制限あり)                *
*    ret  : 戻り値、0=正常終了　それ以外I2C通信エラー下記                      *
*                   1=送ろうとしたデータが送信バッファのサイズを超えた         *
*                   2=スレーブ・アドレスを送信し、NACKを受信した               *
*                   3=データ・バイトを送信し、NACKを受信した                   *
*                   4=その他のエラー                                           *
*                   5=データ受信エラー                                         *

*/

int Rom24lcxxx::Write(long address,char *data,int num) {
	byte x ;
	int i , ret ;

	Wire.beginTransmission(slave_addr) ;
	x = (byte)(address >> 8) ;
	Wire.write(x) ;
	x = (byte)(address & 0x00ff) ;
	Wire.write(x) ;
	for (i=0 ; i<num ; i++ ) {
		Wire.write(*data) ;
		data++ ;
	}
	ret = Wire.endTransmission() ;
	return ret ;
}

