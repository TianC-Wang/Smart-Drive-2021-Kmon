#ifdef WIN32
#include "Kmon.h"
#endif

#define Increase 1
#define Decrease 2

//For the use of adcTreshold
const int adcTreshold = 180;

int adcSensor[6];

void UpdateadcSensor(void) {
	for (int a = 0; a < 6; a++) adcSensor[a] = getadc(a + 1);
}

//return 1==Success; 0==Failed
int PassWhiteLine(int TriesLimited) {
	for (int a = 0; a < TriesLimited; a++) {
		UpdateadcSensor();
		int count = 0;
		if (adcSensor[1] <= adcTreshold) count++;
		if (adcSensor[2] <= adcTreshold) count++;
		if (adcSensor[3] <= adcTreshold) count++;
		if (adcSensor[4] <= adcTreshold) count++;
		//Can edit the Sensor nunmber of detected
		if (count >= 1) {
			return 1;
		}
	}
	return 0;
}

//return 1==Success; 0==Failed
int LookupWhiteLineWithDistance(void) {
	//Stop both Motor
	motor(1, 0);
	motor(2, 0);
	int eadc3 = 0;
	int eadc3p = 0;
	int status;
	int flag = 0;
	int ReturnValue = 0;
	for (int tries = 0; tries < 1000; tries++) {
		if (eadc3 == 0) {
			//Mark the Current Value
			eadc3 = geteadc(3);
			//Here should have A TRY that if Value=100 Could WORK!
			motor(1, 100);
		}
		//Judge Increase or Decrease
		else if (eadc3 != 0 && eadc3p == 0) {
			eadc3p = eadc3;
			eadc3 = geteadc(3);
			if (eadc3 > eadc3p) flag = Increase;
			if (eadc3 < eadc3p) flag = Decrease;
			if (eadc3 = eadc3p) eadc3p = 0;
		}
		else if (flag==Increase) {
			//Here should have A TRY that if Value=100 Could WORK!
			motor(1, -100);
			eadc3p = eadc3;
			eadc3 = geteadc(3);
			flag = Decrease;
		}
		else if (flag == Decrease) {
			eadc3p = eadc3;
			eadc3 = geteadc(3);
			if (eadc3p < eadc3) {
				ReturnValue = 1;
				break;
			}
		}
	}
	return ReturnValue;
}


// 他也曾鲜衣怒马，望尽长安花
int LookupWhiteLineWithSenior(void) {
	motor(1, 0); motor(2, 0);
	UpdateadcSensor();
	if (adcSensor[0] > adcTreshold && adcSensor[1] > adcTreshold && adcSensor[2] > adcTreshold &&
		adcSensor[3] > adcTreshold && adcSensor[4] > adcTreshold && adcSensor[5] > adcTreshold) {
		return 0;
	}
LookupStarted:
	if (adcSensor[1] <= adcTreshold) {
		if (adcSensor[4] > adcTreshold) {
			motor(2, 100);
			//Tries should have A TEST! (Value of a)
			for (int a = 0; a < 50; a++) {
				if (getadc(5) <= adcTreshold) {
					motor(2, 0);
					return 1;
				}
			}
			motor(2, -100);
			for (int a = 0; a < 100; a++) {
				if (getadc(5) <= adcTreshold) {
					motor(2, 0);
					return 1;
				}
			}
		}
	}
	else if (adcSensor[4] <= adcTreshold) {
		if (adcSensor[1] > adcTreshold) {
			motor(1, 100);
			//Tries should have A TEST! (Value of a)
			for (int a = 0; a < 50; a++) {
				if (getadc(2) <= adcTreshold) {
					motor(1, 0);
					return 1;
				}
			}
			motor(2, -100);
			for (int a = 0; a < 100; a++) {
				if (getadc(2) <= adcTreshold) {
					motor(1, 0);
					return 1;
				}
			}
		}
	}
	else {
		go(100, 100);
		for (int a = 0; a < 50; a++) {
			UpdateadcSensor();
			if (adcSensor[1] <= adcTreshold || adcSensor[4] <= adcTreshold) {
				goto LookupStarted;
			}
		}
	}
	motor(1, 0); motor(2, 0);
	return 0;
}