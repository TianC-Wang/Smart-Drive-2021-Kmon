#ifdef WIN32
#include "Kmon.h"
#endif

void PassWhiteLine();
int LookupWhiteLine();//return 1==Success; 0==Failed

#define Increase 1
#define Decrease 2

const int eadcTreshold = 180;

void PassWhiteLine() {
	while (1) {
		int count = 0;
		for (int a = 2; a <= 5; a++) {
			if (geteadc(a) <= eadcTreshold) count++;
		}
		if (count >= 4) break;
	}
	return;
}

//return 1==Success; 0==Failed
int LookupWhiteLine() {
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