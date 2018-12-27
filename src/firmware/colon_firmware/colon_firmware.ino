#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Colon.h>

Colon aControl;

union{
    byte byteVal[4];
    float floatVal;
    long longVal;
}val;

union{
  byte byteVal[8];
  double doubleVal;
}valDouble;

union{
  byte byteVal[2];
  short shortVal;
}valShort;

#if defined(__AVR_ATmega32U4__) 
  int analogs[12]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11};
#endif

#if defined(__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)
  int analogs[16]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};
#endif
String mVersion = "1.1.103";
boolean isAvailable = false;
boolean isBluetooth = false;

int len = 52;
char buffer[52];

byte index = 0;
byte dataLen;
byte modulesLen=0;
boolean isStart = false;
unsigned char irRead;
char serialRead;
#define VERSION 0
#define INIT_HW 1
#define TEMPERATURE_SENSOR 2
#define LIGHT_SENSOR 3
#define POTENTIONMETER 4
#define JOYSTICK 5
#define GYRO 6
#define SOUND_SENSOR 7
#define RGBLED 8
#define SEVSEG 9
#define MOTOR 10
#define SERVO 11
//#define ENCODER 12
#define IR 13
#define PIRMOTION 15
#define INFRARED 16
#define LINEFOLLOWER 17
#define SHUTTER 20
#define LIMITSWITCH 21
#define BUTTON 22
#define DIGITAL 30
#define ANALOG 31
#define PWM 32
#define SERVO_PIN 33
#define TONE 34
#define LED 40
#define ENCODER 41
#define TIMER 50
#define BUTTERY 60
#define PSD 61
#define LED_MATRIX_A  62
#define LED_MATRIX_B  63
#define LED_MATRIX_C  64
#define LED_MATRIX_D  65
#define SERIAL_READ 66
#define SERIAL_WRITE 67
#define STOP_ALL 68
#define CDS 69

#define MOTER_CALIBRATION 70
#define TEMP_CALIBRATION 71

#define COLON_LED 80
#define COLON_RGBLED  81
#define COLON_RGBLED2 82
#define COLON_VSS 83
#define COLON_SSS 84
#define COLON_CDS 85
#define COLON_N_SERVO 88
#define COLON_E_SERVO 89
#define LED_MATRIX_E  90
#define COLON_TEMP_STATUS 91
#define COLON_MIC 92
#define COLON_RIP 93
#define COLON_EXT_POW 94






#define GET 1
#define RUN 2
#define RESET 4
#define START 5
float angleServo = 90.0;
unsigned char prevc=0;
double lastTime = 0.0;
double currentTime = 0.0;

void setup(){
  Serial.begin(115200);
  
  Serial1.begin(9600);
  aControl.init();
}

void loop(){

  digitalWrite(17, 0);

  currentTime = millis()/1000.0-lastTime;
  readSerial();
  if(isAvailable){
    unsigned char c = serialRead&0xff;
    if(c==0x55&&isStart==false){
     if(prevc==0xff){
      index=1;
      isStart = true;
     }
    }else{
      prevc = c;
      if(isStart){
        if(index==2){
         dataLen = c; 
        }else if(index>2){
          dataLen--;
        }
        writeBuffer(index,c);
      }
    }
     index++;
     if(index>51){
      index=0; 
      isStart=false;
     }
     if(isStart&&dataLen==0&&index>3){ 
        isStart = false;
        parseData(); 
        index=0;
     }
  }
}

void serialEvent1() {
    aControl.serialEventHandler() ;
}

unsigned char readBuffer(int index){
 return buffer[index]; 
}
void writeBuffer(int index,unsigned char c){
  buffer[index]=c;
}
void writeHead(){
  writeSerial(0xff);
  writeSerial(0x55);
}
void writeEnd(){
 Serial.println(); 
 #if defined(__AVR_ATmega32U4__) 
//   Serial1.println();
 #endif
}

void writeSerial(unsigned char c){
 Serial.write(c);
 #if defined(__AVR_ATmega32U4__) 
//   Serial1.write(c);
 #endif
}

void readSerial(){
  isAvailable = false;
  if(Serial.available()>0){
    isAvailable = true;
    isBluetooth = false;
    serialRead = Serial.read();
  }
  #if defined(__AVR_ATmega32U4__) 
  /*
  if(Serial1.available()>0){
    isAvailable = true;
    isBluetooth = true;
    serialRead = Serial1.read();
  }
  */
 #endif
}
/*
ff 55 len idx action device port  slot  data a
0  1  2   3   4      5      6     7     8
*/
void parseData(){
  isStart = false;
  int idx = readBuffer(3);
  int action = readBuffer(4);
  int device = readBuffer(5);
  switch(action){
    case GET:{
        writeHead();
        writeSerial(idx);
        readSensor(device);
        writeEnd();
     }
     break;
     case RUN:{
       runModule(device);
       callOK();
     }
      break;
      case RESET:{
        //reset
        callOK();
      }
     break;
     case START:{
        //start
        callOK();
      }
     break;
  }
}
void callOK(){
    writeSerial(0xff);
    writeSerial(0x55);
    writeEnd();
}

void sendByte(char c){
  writeSerial(1);
  writeSerial(c);
}

void sendString(String s){
  int l = s.length();
  writeSerial(4);
  writeSerial(l);
  for(int i=0;i<l;i++){
    writeSerial(s.charAt(i));
  }
}

void sendFloat(float value){ 
     writeSerial(0x2);
     val.floatVal = value;
     writeSerial(val.byteVal[0]);
     writeSerial(val.byteVal[1]);
     writeSerial(val.byteVal[2]);
     writeSerial(val.byteVal[3]);
}

void sendShort(double value){
     writeSerial(3);
     valShort.shortVal = value;
     writeSerial(valShort.byteVal[0]);
     writeSerial(valShort.byteVal[1]);
     writeSerial(valShort.byteVal[2]);
     writeSerial(valShort.byteVal[3]);
}

void sendDouble(double value){
     writeSerial(2);
     valDouble.doubleVal = value;
     writeSerial(valDouble.byteVal[0]);
     writeSerial(valDouble.byteVal[1]);
     writeSerial(valDouble.byteVal[2]);
     writeSerial(valDouble.byteVal[3]);
}

short readShort(int idx){
  valShort.byteVal[0] = readBuffer(idx);
  valShort.byteVal[1] = readBuffer(idx+1);
  return valShort.shortVal; 
}

float readLong(int idx){
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.longVal;
}

float readFloat(int idx){
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.floatVal;
}

char _receiveStr[50] = {};
uint8_t _receiveUint8[16] = {};
char* readString(int idx,int len){
  for(int i=0;i<len;i++){
    _receiveStr[i]=readBuffer(idx+i);
  }
  _receiveStr[len] = '\0';
  return _receiveStr;
}
uint8_t* readUint8(int idx,int len){
  for(int i=0;i<len;i++){
    if(i>15){
      break;
    }
    _receiveUint8[i] = readBuffer(idx+i);
  }
  return _receiveUint8;
}


void runModule(int device){
  //0xff 0x55 0x6 0x0 0x1 0xa 0x9 0x0 0x0 0xa 
  int port = readBuffer(6);
  int pin = port;
  String retBuf;
  
  switch(device){
    //初期化
    case INIT_HW:{
      aControl.init();
    }
    break;

    //LED操作
    case COLON_LED:{
      int level = readBuffer(7);
      aControl.controlLED(level) ;
    }
    break;

    //RGBLED操作
    case COLON_RGBLED:{
      int st = readBuffer(7);
      int cl = readBuffer(8);
      aControl.controlRGBLED(st, cl) ;
    }
    break;

    //RGBLED2操作
    case COLON_RGBLED2:{
      int st = readBuffer(7);
      int r = readBuffer(8);
      int g = readBuffer(9);
      int b = readBuffer(10);
      aControl.controlRGBLED2(st, r, g, b) ;
    }
    break;

    //ブザー操作
    case TONE:{
      int hz = readShort(7);
      int ms = readShort(9);
      if (ms>0){
        aControl.controlBuzzer(hz, ms) ;
      }else{
        aControl.controlBuzzer(hz, 0) ;
      }
    }
    break;

    //シリアル書き込み
    case SERIAL_WRITE:{
      int len = port;
      retBuf = readString(7, len) ;
      aControl.serialStringWrite(retBuf);
    }
    break;

    //首SERVO操作
    case COLON_N_SERVO:{
      int angle = readBuffer(7);
      aControl.controlNeckServo(angle) ;
    }
    break;

    //外部SERVO操作
    case COLON_E_SERVO:{
      int angle = readBuffer(7);
      aControl.controlExtServo(angle) ;
    }
    break;

    //LED Matrix操作
    case LED_MATRIX_A:{
      int row = readBuffer(7);
      int col = readBuffer(8);
      int bl = readBuffer(9);
      if (bl == 1) {
        aControl.controlLedmatrix(row, col, true) ;
      }else{
        aControl.controlLedmatrix(row, col, false) ;
      }
    }
    break;

    //LED Matrix操作
    case LED_MATRIX_B:{
      int code = readBuffer(7);
      aControl.controlLedmatrixByChar(code) ;
    }
    break;

    //LED Matrix操作
    case LED_MATRIX_C:{
      int row = readBuffer(7);
      int value = readBuffer(8);
      aControl.controlLedmatrixByRowData(row, value) ;
    }
    break;

    //LED Matrix操作
    case LED_MATRIX_D:{
      aControl.controlLedmatrixClear() ;
    }
    break;

    //LED Matrix操作
    case LED_MATRIX_E:{
      int col = readBuffer(7);
      int value = readBuffer(8);
      aControl.controlLedmatrixByColData(col, value) ;
    }
    break;

    //外部電源操作
    case COLON_EXT_POW:{
      int sw = readBuffer(7);
      aControl.controlExtPower(sw) ;
    }
    break;

   case TEMP_CALIBRATION:{
     int t_c = readBuffer(7);
      aControl.calibrationTemperature(t_c);
   }
   break;


    //停止
    case STOP_ALL:{
//      aControl.stopAll();
    }
    break;
  }
}
void readSensor(int device){
  /**************************************************
      ff 55 len idx action device port slot data a
      0  1  2   3   4      5      6    7    8
  ***************************************************/
  float value=0.0;
  int port,slot,pin;
  int b_value;
  port = readBuffer(6);
  pin = port;
  switch(device){
    //バージョン
    case  VERSION:{
      sendString(mVersion);
    }
    break;

    //振動スイッチ状況取得
    case COLON_VSS:{
      sendFloat(aControl.getVibrationSwatchStatus());
    }
    break;

    //設定スイッチ状況取得
    case COLON_SSS:{
      sendFloat(aControl.getSettingSwatchStatus(pin));
    }
    break;

    //光センサー状況取得
    case  COLON_CDS:{
      sendFloat(aControl.getCDSStatus()) ;
    }
    break;

    //シリアル読み込み
    case SERIAL_READ:{
      sendString(aControl.serialStringRead());     
    }
    break;

    //温度センサー状況取得
    case  COLON_TEMP_STATUS:{
      sendFloat(aControl.getTemperatureStatus()) ;
    }
    break;

    //マイクセンサー状況取得
    case  COLON_MIC:{
      sendFloat(aControl.getMICStatus(pin)) ;
    }
    break;

    //人感センサー状況取得
    case  COLON_RIP:{
      sendFloat(aControl.getRIPStatus()) ;
    }
    break;
  }
}
