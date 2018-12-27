// arduino.js

(function(ext) {
    var device = null;
    var _rxBuf = [];
    var target_PA = null; // for controlMotor
    var target_PB = null; // for controlMotor
    var target_PS = null; // for controlMotor
    
    // Sensor states:
    var ports = {
	LEFT_LED: 9,
	RIGHT_LED: 10,
	LEFT_SERVO: 9,
	RIGHT_SERVO: 10,
	LEFT_SWITCH: 12,
	RIGHT_SWITCH: 13,
	LEFT_CDS: 0,
	RIGHT_CDS: 1,
	LEFT_LTS: 2,
	RIGHT_LTS: 3,
        Port1: 1,
        Port2: 2,
        Port3: 3,
        Port4: 4,
        Port5: 5,
        Port6: 6,
        Port7: 7,
        Port8: 8,
        Port9: 9,
        Port10: 10,
        Port11: 11,
        Port12: 12,
        Port13: 13,
	M1:9,
	M2:10
    };

	var p_stat = {
		P_ON:1,
		P_OFF:0
	};
	var slots = {
		Slot1:1,
		Slot2:2
	};
	var switchStatus = {
		On:1,
		Off:0
	};
	var levels = {
		HIGH:1,
		LOW:0,
		LED_HIGH:1,
		LED_LOW:0
	};
	var axis = {
		'X-Axis':1,
		'Y-Axis':2,
		'Z-Axis':3
	}
	var tones ={"ST":1,"B0":31,"C1":33,"D1":37,"E1":41,"F1":44,"G1":49,"A1":55,"B1":62,
			"C2":65,"D2":73,"E2":82,"F2":87,"G2":98,"A2":110,"B2":123,
			"C3":131,"D3":147,"E3":165,"F3":175,"G3":196,"A3":220,"B3":247,
			"C4":262,"D4":294,"E4":330,"F4":349,"G4":392,"A4":440,"B4":494,
			"C5":523,"D5":587,"E5":659,"F5":698,"G5":784,"A5":880,"B5":988,
			"C6":1047,"D6":1175,"E6":1319,"F6":1397,"G6":1568,"A6":1760,"B6":1976,
			"C7":2093,"D7":2349,"E7":2637,"F7":2794,"G7":3136,"A7":3520,"B7":3951,
	"C8":4186,"D8":4699};
	var beats = {"Half":500,"Quarter":250,"Eighth":125,"Whole":1000,"Double":2000,"Zero":0};
	var values = {
		LEFT_LTS: 1,
		RIGHT_LTS: 2,
		LEFT_MOTOR: 1,
		RIGHT_MOTOR: 2,
		FORWARD: 1,
		REVERSE: 2
	};
	
	var lm_pos = {"0":0,"1":1,"2":2,"3":3,"4":4,"5":5,"6":6,"7":7}
	
	var lm_code = {"LM_A":0,"LM_B":1,"LM_C":2,"LM_D":3,"LM_E":4,"LM_F":5,"LM_G":6,"LM_H":7,"LM_I":8,"LM_J":9,"LM_K":10,"LM_L":11,"LM_M":12,"LM_N":13,"LM_O":14,"LM_P":15,
"LM_Q":16,"LM_R":17,"LM_S":18,"LM_T":19,"LM_U":20,"LM_V":21,"LM_W":22,"LM_X":23,"LM_Y":24,"LM_Z":25,"LM_a":26,"LM_i":27,"LM_u":28,"LM_e":29,"LM_o":30,
"LM_ka":31,"LM_ki":32,"LM_ku":33,"LM_ke":34,"LM_ko":35,"LM_sa":36,"LM_shi":37,"LM_su":38,"LM_se":39,"LM_so":40,"LM_ta":41,"LM_chi":42,"LM_tsu":43,"LM_te":44,"LM_to":45,
"LM_na":46,"LM_ni":47,"LM_nu":48,"LM_ne":49,"LM_no":50,"LM_ha":51,"LM_hi":52,"LM_hu":53,"LM_he":54,"LM_ho":55,"LM_ma":56,"LM_mi":57,"LM_mu":58,"LM_me":59,"LM_mo":60,
"LM_ya":61,"LM_yu":62,"LM_yo":63,"LM_ra":64,"LM_ri":65,"LM_ru":66,"LM_re":67,"LM_ro":68,"LM_wa":69,"LM_wo":70,"LM_nn":71,"LM_bar":72,"LM_small tu":73,"LM_small ya":74,"LM_small yu":75,
"LM_small yo":76,"LM_sonant mark":77,"LM_p-sound consonant mark":78,"LM_zero":79,"LM_one":80,"LM_two":81,"LM_three":82,"LM_four":83,"LM_five":84,"LM_six":85,"LM_seven":86,"LM_eight":87,"LM_nine":88,"LM_Surprise":89,"LM_Smile":90,
"LM_plus":91,"LM_percent":92,"LM_at-mark":93,"LM_Question":94,"LM_c-degree":95,"LM_angle":96,"LM_heart":97,"LM_note":98,"LM_Email":99,"LM_clock":100,"LM_prohibit":101,"LM_flower":102,"LM_umbrella":103,"LM_and":104,"LM_sharp":105};

	var indexs = [];
	
	var startTimer = 0;
	var versionIndex = 0xFA;
	
	var rgbled_status = {"RGBLED_HIGH":1,RGBLED_LOW:0};
	var rgbled_color = {"WHITE":1,"RED":2,"GREEN":3,"BLUE":4,"YELLOW":5,"CYAN":6,"MAGENTA":7};
	var setting_sw ={"LEFT_SWITCH":1,"RIGHT_SWITCH":2};
	var mic_port ={"LEFT_MIC":1,"RIGHT_MIC":2};
	
    ext.resetAll = function(){
//    	device.send([0xff, 0x55, 2, 0, 4]);
    };
	
	ext.runArduino = function(){
		responseValue();
	};
	
	//LED操作
	ext.controlLED = function(level) {
		var deviceId = 80;
		runPackage(deviceId, 0, level);
	};

	//RGBLED操作
	ext.controlRGBLED = function(status, color) {
		var deviceId = 81;
		runPackage(deviceId, 0, status, typeof color=="string"?rgbled_color[color]:new Number(color));
	};

	//RGBLED2操作
	ext.controlRGBLED2 = function(status, red, green, blue) {
		var deviceId = 82;
		runPackage(deviceId, 0, status, red, green, blue);
	};


	//振動スイッチ状況取得
	ext.getVibrationSwatchStatus = function(nextID) {
		var deviceId = 83;
		getPackage(nextID,deviceId, 0);
	};

	//設定スイッチ状況取得
	ext.getSettingSwatchStatus = function(nextID, pin) {
		var deviceId = 84;
		getPackage(nextID,deviceId, typeof pin=="string"?setting_sw[pin]:new Number(pin));
	};

	//光センサー状況取得
	ext.getCDSStatus = function(nextID) {
		var deviceId = 85;
		getPackage(nextID,deviceId, 0);
	};


	//ブザー操作
	ext.controlBuzzer = function(tone,beat) {
		var deviceId = 34;
		runPackage(deviceId, 0, short2array(typeof tone=="number"?tone:tones[tone]),short2array(typeof beat=="number"?beat:beats[beat]));
	};
	
	//シリアル読み込み
	ext.serialStringRead =  function(nextID) {
		var deviceId = 66;
		getPackage(nextID,deviceId);
	};
	
	//シリアル書き込み
	ext.serialStringWrite = function(message) {
		var deviceId = 67;
		message = message.toString();
		runPackage(deviceId, bytes2(message),string2array(message));
	};

	//首SERVO操作
	ext.controlNeckServo = function(angle) {
		var deviceId = 88;
		runPackage(deviceId, 0, angle);
	};

	//外部SERVO操作
	ext.controlExtServo = function(angle) {
		var deviceId = 89;
		runPackage(deviceId, 0, angle);
	};
	
	//LED Matrix操作
	ext.controlLedmatrix = function(row, col, level) {
		var deviceId = 62;
		runPackage(deviceId, 0, typeof row=="string"?lm_pos[row]:new Number(row), typeof col=="string"?lm_pos[col]:new Number(col), typeof level=="string"?levels[level]:new Number(level));
	};
		
	//LED Matrix操作
	ext.controlLedmatrixByChar1 = function(code) {
		var deviceId = 63;
		runPackage(deviceId, 0, typeof code=="string"?lm_code[code]:new Number(code));
	};

	//LED Matrix操作
	ext.controlLedmatrixByChar2 = function(code) {
		var deviceId = 63;
		runPackage(deviceId, 0, typeof code=="string"?lm_code[code]:new Number(code));
	};
	
	//LED Matrix操作
	ext.controlLedmatrixByChar3 = function(code) {
		var deviceId = 63;
		runPackage(deviceId, 0, typeof code=="string"?lm_code[code]:new Number(code));
	};
	
	//LED Matrix操作
	ext.controlLedmatrixByChar4 = function(code) {
		var deviceId = 63;
		runPackage(deviceId, 0, typeof code=="string"?lm_code[code]:new Number(code));
	};
	
	//LED Matrix操作
	ext.controlLedmatrixByRowData = function(row, value) {
		var deviceId = 64;
		runPackage(deviceId, 0, typeof row=="string"?lm_pos[row]:new Number(row), value);
	};
	
	//LED Matrix操作
	ext.controlLedmatrixByColData = function(col, value) {
		var deviceId = 90;
		runPackage(deviceId, 0, typeof col=="string"?lm_pos[col]:new Number(col), value);
	};
	
	//LED Matrix操作
	ext.controlLedmatrixClear = function() {
		var deviceId = 65;
		runPackage(deviceId);
	};
		
	//温度センサー状況取得
	ext.getTemperatureStatus = function(nextID) {
		var deviceId = 91;
		getPackage(nextID,deviceId, 0);
	};

	//マイクセンサー状況取得
	ext.getMICStatus = function(nextID, pin) {
		var deviceId = 92;
		getPackage(nextID,deviceId, typeof pin=="string"?mic_port[pin]:new Number(pin));
	};

	//人感センサー状況取得
	ext.getRIPStatus = function(nextID) {
		var deviceId = 93;
		getPackage(nextID,deviceId, 0);
	};

	//外部電源操作
	ext.controlExtPower = function(psw) {
		var deviceId = 94;
		runPackage(deviceId, 0, typeof psw=="string"?p_stat[psw]:new Number(psw));
		
	};
	
	//温度計キャリブレーション
	ext.calibrationTemperature = function(val1) {
		var deviceId = 71;
		runPackage(deviceId, 0, val1);	
	}
	
	//停止（無動作）
	ext.breakProgram = function() {
		var deviceId = 1;
		runPackage(deviceId);
	};
	
	//リセット
	ext.initProgram = function() {
		var deviceId = 1;
		runPackage(deviceId);
	};
	
	ext.runDigital = function(pin,level) {
        runPackage(30,pin,typeof level=="string"?levels[level]:new Number(level));
    };
    ext.runPwm = function(pin,pwm) {
        runPackage(32,pin,pwm);
    };
	ext.runTone = function(pin,tone,beat){
		runPackage(34,pin,short2array(typeof tone=="number"?tone:tones[tone]),short2array(typeof beat=="number"?beat:beats[beat]));
	};
	ext.runServoArduino = function(pin, angle){
		runPackage(33,pin,angle);
	};
	ext.resetTimer = function(){
		startTimer = new Date().getTime();
		responseValue();
	};
	ext.getDigital = function(nextID,pin){
		var deviceId = 30;
		getPackage(nextID,deviceId,pin);
	};
	ext.getAnalog = function(nextID,pin) {
		var deviceId = 31;
		getPackage(nextID,deviceId,pin);
    };
	ext.getPulse = function(nextID,pin,timeout) {
		var deviceId = 37;
		getPackage(nextID,deviceId,pin,short2array(timeout));
    };
	ext.getUltrasonicArduino = function(nextID,trig,echo){
		var deviceId = 36;
		getPackage(nextID,deviceId,trig,echo);
	}
	ext.getTimer = function(nextID){
		if(startTimer==0){
			startTimer = new Date().getTime();
		}
		responseValue(nextID,new Date().getTime()-startTimer);
	}



	function sendPackage(argList, type){
		var bytes = [0xff, 0x55, 0, 0, type];
		for(var i=0;i<argList.length;++i){
			var val = argList[i];
			if(val.constructor == "[class Array]"){
				bytes = bytes.concat(val);
			}else{
				bytes.push(val);
			}
		}
		bytes[2] = bytes.length - 3;
		device.send(bytes);
	}
	
	function runPackage(){
		sendPackage(arguments, 2);
	}
	function getPackage(){
		var nextID = arguments[0];
		Array.prototype.shift.call(arguments);
		sendPackage(arguments, 1);
	}

    var inputArray = [];
	var _isParseStart = false;
	var _isParseStartIndex = 0;
    function processData(bytes) {
		var len = bytes.length;
		if(_rxBuf.length>30){
			_rxBuf = [];
		}
		for(var index=0;index<bytes.length;index++){
			var c = bytes[index];
			_rxBuf.push(c);
			if(_rxBuf.length>=2){
				if(_rxBuf[_rxBuf.length-1]==0x55 && _rxBuf[_rxBuf.length-2]==0xff){
					_isParseStart = true;
					_isParseStartIndex = _rxBuf.length-2;
				}
				if(_rxBuf[_rxBuf.length-1]==0xa && _rxBuf[_rxBuf.length-2]==0xd&&_isParseStart){
					_isParseStart = false;
					
					var position = _isParseStartIndex+2;
					var extId = _rxBuf[position];
					position++;
					var type = _rxBuf[position];
					position++;
					//1 byte 2 float 3 short 4 len+string 5 double
					var value;
					switch(type){
						case 1:{
							value = _rxBuf[position];
							position++;
						}
							break;
						case 2:{
							value = readFloat(_rxBuf,position);
							position+=4;
							if(value<-255||value>1023){
								value = 0;
							}
						}
							break;
						case 3:{
							value = readInt(_rxBuf,position,2);
							position+=2;
						}
							break;
						case 4:{
							var l = _rxBuf[position];
							position++;
							value = readString(_rxBuf,position,l);
						}
							break;
						case 5:{
							value = readDouble(_rxBuf,position);
							position+=4;
						}
							break;
						case 6:
							value = readInt(_rxBuf,position,4);
							position+=4;
							break;
					}
					if(type<=6){
						responseValue(extId,value);
					}else{
						responseValue();
					}
					_rxBuf = [];
				}
			} 
		}
    }
	function readFloat(arr,position){
		var f= [arr[position],arr[position+1],arr[position+2],arr[position+3]];
		return parseFloat(f);
	}
	function readInt(arr,position,count){
		var result = 0;
		for(var i=0; i<count; ++i){
			result |= arr[position+i] << (i << 3);
		}
		return result;
	}
	function readDouble(arr,position){
		return readFloat(arr,position);
	}
	function readString(arr,position,len){
		var value = "";
		for(var ii=0;ii<len;ii++){
			value += String.fromCharCode(_rxBuf[ii+position]);
		}
		return value;
	}
    function appendBuffer( buffer1, buffer2 ) {
        return buffer1.concat( buffer2 );
    }

    // Extension API interactions
    var potentialDevices = [];
    ext._deviceConnected = function(dev) {
        potentialDevices.push(dev);

        if (!device) {
            tryNextDevice();
        }
    }

    function tryNextDevice() {
        // If potentialDevices is empty, device will be undefined.
        // That will get us back here next time a device is connected.
        device = potentialDevices.shift();
        if (device) {
            device.open({ stopBits: 0, bitRate: 115200, ctsFlowControl: 0 }, deviceOpened);
        }
    }

    var watchdog = null;
    function deviceOpened(dev) {
        if (!dev) {
            // Opening the port failed.
            tryNextDevice();
            return;
        }
        device.set_receive_handler('arduino',processData);
    };

    ext._deviceRemoved = function(dev) {
        if(device != dev) return;
        device = null;
    };

    ext._shutdown = function() {
        if(device) device.close();
        device = null;
    };

    ext._getStatus = function() {
        if(!device) return {status: 1, msg: 'Arduino disconnected'};
        if(watchdog) return {status: 1, msg: 'Probing for Arduino'};
        return {status: 2, msg: 'Arduino connected'};
    }


    function bytes2(str) {
        return(encodeURIComponent(str).replace(/%../g,"x").length);
    }

    var descriptor = {};
	ScratchExtensions.register('Arduino', descriptor, ext, {type: 'serial'});
})({});
