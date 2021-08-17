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

void PassWhiteLine(void) {
	while (1) {
		int count = 0;
		for (int a = 2; a <= 5; a++) {
			if (getadc(a) <= adcTreshold) count++;
		}
		if (count >= 2) break;
	}
	return;
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
	if (adcSensor[0] < adcTreshold && adcSensor[1] < adcTreshold && adcSensor[2] < adcTreshold &&
		adcSensor[3] < adcTreshold && adcSensor[4] < adcTreshold && adcSensor[5] < adcTreshold) {
		return 0;
	}
	if (adcSensor[0] >= adcTreshold) {
		if (adcSensor[5] < adcTreshold) {
			motor(2, 100);
			wait(0.2);//Still need TESTING!
			motor(2, 0);
		}
	}
	else if (adcSensor[5] >= adcTreshold) {
		if (adcSensor[0] < adcTreshold) {
			motor(2, 100);
			wait(0.2);//Still need TESTING!
			motor(2, 0);
		}
	}
	else if (adcSensor[3] >= adcTreshold) {
		//Still Porcessing
	}
}