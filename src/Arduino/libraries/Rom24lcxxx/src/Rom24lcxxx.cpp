/**
 Rom24lcxxx.h


**/

#include <Wire.h>
#include <Arduino.h>

#include "Rom24lcxxx.h"

static int slave_addr ;

#define CTRL_CODE  0b01010000  // EEPROM 24xx�V���[�Y�̐���R�[�h("1010")


Rom24lcxxx::Rom24lcxxx(int A0,int A1,int A2) {

	Wire.begin() ;
	slave_addr = CTRL_CODE | (A2<<2) | (A1<<1) | A0 ;
}

/*
*    EEPROM����w��������f�[�^��ǂݏo������                                *
*                                                                              *
*    address : �ǂݏo���J�n�̃f�[�^�A�h���X�ʒu���w�肵�܂�                    *
*    *data: �ǂݏo�����f�[�^��ۑ�����ׂ̔z��ϐ����w�肵�܂�                 *
*    num  : �ǂݏo���f�[�^�̌����w�肵�܂�(���̌��������z��ϐ��͊m�ۂ���) *
*    ret  : �߂�l�A0=����I���@����ȊOI2C�ʐM�G���[���L                      *
*                   1=���낤�Ƃ����f�[�^�����M�o�b�t�@�̃T�C�Y�𒴂���         *
*                   2=�X���[�u�E�A�h���X�𑗐M���ANACK����M����               *
*                   3=�f�[�^�E�o�C�g�𑗐M���ANACK����M����                   *
*                   4=���̑��̃G���[                                           *
*                   5=�f�[�^��M�G���[                                         *

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
*    EEPROM�Ɏw��������f�[�^���������ޏ���                                  *
*                                                                              *
*    address : �������݊J�n�̃f�[�^�A�h���X�ʒu���w�肵�܂�                    *
*    *data: �������ރf�[�^��ۑ������z��ϐ����w�肵�܂�                       *
*    num  : �������ރf�[�^�̌����w�肵�܂�                                   *
*           �f�o�C�X�̃y�[�W�T�C�Y�ȏ�͘A���������݂͏o���܂���               *
*           24xx32/64��32byte   24xx128/256��64byte  24xx512��128byte          *
*           (Wire���C�u�����̃o�b�t�@��MAX32byte�܂łƐ�������)                *
*    ret  : �߂�l�A0=����I���@����ȊOI2C�ʐM�G���[���L                      *
*                   1=���낤�Ƃ����f�[�^�����M�o�b�t�@�̃T�C�Y�𒴂���         *
*                   2=�X���[�u�E�A�h���X�𑗐M���ANACK����M����               *
*                   3=�f�[�^�E�o�C�g�𑗐M���ANACK����M����                   *
*                   4=���̑��̃G���[                                           *
*                   5=�f�[�^��M�G���[                                         *

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

