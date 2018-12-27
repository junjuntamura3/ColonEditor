/**
 Rom24lcxxx.h


**/

#ifndef Rom24lcxxx_H
#define Rom24lcxxx_H

#include <Arduino.h>


#define CTRL_CODE  0b01010000  // EEPROM 24xxシリーズの制御コード("1010")

class Rom24lcxxx {
	public:
		Rom24lcxxx(int A0,int A1,int A2) ;
		int Read(long address,char *data,int num) ;
		int Write(long address,char *data,int num) ;
	private:
} ;

#endif // Rom24lcxxx_H
