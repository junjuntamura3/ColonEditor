/**
 Colon.h


**/


#ifndef Colon_H
#define Colon_H

#include <Arduino.h>

#define FOR_COLON		/* Colon向け */

/* PIN　アサイン */

#define COLON_V_SW		4		/* バネ振動スイッチ ANALOG(?) IN */
#define COLON_LED_PORT		5		/* 白色LED(2個）PWM OUT */
#define COLON_EXT_POW_PORT	7		/* 外部電源 OUT */
#define COLON_RGBLED_PORT	8		/* RGBLED DIGITAL OUT */
#define COLON_N_SERVO_PORT	9		/* 首サーボ PWM OUT */
#define COLON_E_SERVO_PORT	10		/* 外部サーボ PWM OUT */
#define COLON_BUZZER		11		/* ブザー */
#define COLON_C_SW_LEFT		12		/* 設定スイッチ１（左） DIGITAL IN */
#define COLON_C_SW_RIGHT	13		/* 設定スイッチ２（右） DIGITAL IN */

#define COLON_LC_LOAD		14		/* LED Matrix LOAD */
#define COLON_LC_CLK		15		/* LED Matrix CLK */
#define COLON_LC_DATAIN		16		/* LED Matrix DataIn */

#define COLON_CDS_PORT		A0		/* ライトセンサ */
#define COLON_TMP_PORT		A1		/* サーミスタ */
#define COLON_MIC_LEFT		A2		/* MICセンサ(左） */
#define COLON_MIC_RIGHT		A3		/* MICセンサ(右） */
#define COLON_RIP_PORT		A4		/* 人感センサ */




/* 制御値 */
#define COLON_STATUS_OFF	0
#define COLON_STATUS_ON		1

/* 設定スイッチ */
#define COLON_SETTING_SW_LEFT	1
#define COLON_SETTING_SW_RIGHT	2


/* MIC */
#define COLON_MIC_POS_LEFT	1
#define COLON_MIC_POS_RIGHT	2


/* RGBLED 色 */
#define RGBCOLOR_WHITE		1
#define RGBCOLOR_RED		2
#define RGBCOLOR_GREEN		3
#define RGBCOLOR_BLUE		4
#define RGBCOLOR_YELLOW		5
#define RGBCOLOR_CYAN		6
#define RGBCOLOR_MAGENTA	7


/* ブザー定数 */
#define COLON_TONES_ST		0
#define COLON_TONES_B0		31
#define COLON_TONES_C1		33
#define COLON_TONES_D1		37
#define COLON_TONES_E1		41
#define COLON_TONES_F1		44
#define COLON_TONES_G1		49
#define COLON_TONES_A1		55
#define COLON_TONES_B1		62
#define COLON_TONES_C2		65
#define COLON_TONES_D2		73
#define COLON_TONES_E2		82
#define COLON_TONES_F2		87
#define COLON_TONES_G2		98
#define COLON_TONES_A2		110
#define COLON_TONES_B2		123
#define COLON_TONES_C3		131
#define COLON_TONES_D3		147
#define COLON_TONES_E3		165
#define COLON_TONES_F3		175
#define COLON_TONES_G3		196
#define COLON_TONES_A3		220
#define COLON_TONES_B3		247
#define COLON_TONES_C4		262
#define COLON_TONES_D4		294
#define COLON_TONES_E4		330
#define COLON_TONES_F4		349
#define COLON_TONES_G4		392
#define COLON_TONES_A4		440
#define COLON_TONES_B4		494
#define COLON_TONES_C5		523
#define COLON_TONES_D5		587
#define COLON_TONES_E5		659
#define COLON_TONES_F5		698
#define COLON_TONES_G5		784
#define COLON_TONES_A5		880
#define COLON_TONES_B5		988
#define COLON_TONES_C6		1047
#define COLON_TONES_D6		1175
#define COLON_TONES_E6		1319
#define COLON_TONES_F6		1397
#define COLON_TONES_G6		1568
#define COLON_TONES_A6		1760
#define COLON_TONES_B6		1976
#define COLON_TONES_C7		2093
#define COLON_TONES_D7		2349
#define COLON_TONES_E7		2637
#define COLON_TONES_F7		2794
#define COLON_TONES_G7		3136
#define COLON_TONES_A7		3520
#define COLON_TONES_B7		3951

#define COLON_BEATS_HALF	500
#define COLON_BEATS_QUATER	250
#define COLON_BEATS_EIGHTH	125
#define COLON_BEATS_WHOLE	1000
#define COLON_BEATS_DOUBLE	2000
#define COLON_BEATS_ZERO	0


#define COLON_LC_BRIGHTNESS	4

#define COLON_AVG_PICKUP	3
#define COLON_AVG_COUNT	8
#define COLON_AVG_RANGE_H	1.5
#define COLON_AVG_RANGE_L	0.6

#define RING_BUFFER_SIZE 	100

class Colon {
	public :
		Colon() ;
		void init() ;
		void runwait() ;
		void stopAll();
		void controlBuzzer(uint16_t frequency, uint32_t duration) ;
		void controlLedmatrix(uint8_t row, uint8_t col, boolean value) ;
		void controlLedmatrixClear();
		void controlLedmatrixByChar(uint8_t code);
		void controlLedmatrixByRowData(uint8_t row, uint8_t value);
		void controlLedmatrixByColData(uint8_t col, uint8_t value);
		void serialEventHandler();
		String serialStringRead();
		void serialStringWrite(String value);

		void controlLED(uint8_t level) ;
		void controlRGBLED(uint8_t st, uint8_t cl) ;
		void controlRGBLED2(uint8_t st, uint8_t r, uint8_t g, uint8_t b) ;
		void controlNeckServo(uint8_t angle) ;
		void controlExtServo(uint8_t angle) ;
		int getVibrationSwatchStatus();
		int getSettingSwatchStatus(uint8_t pin);
		int getCDSStatus() ;
		int getTemperatureStatus() ;
		int getMICStatus(uint8_t pin) ;
		int getRIPStatus() ;

		void controlExtPower(uint8_t sw);

		void calibrationTemperature(int cParam) ;

	private :
};

#endif // Colon_H

