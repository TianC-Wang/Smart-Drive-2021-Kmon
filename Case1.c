#ifdef WIN32
#include "Kmon.h"
#endif

void PassWhiteLine();
int LookupWhiteLine();//return 1==Success; 0==Failed

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
	for (int tries = 0; tries < 1000; tries++) {
		if (eadc3 == 0) {
			eadc3 = geteadc(3);
			//Here should have A TRY that if Value=100 Could WORK!
			motor(1, 100);
		}
		else if (eadc3 != 0 && eadc3p == 0) {

		}
	}
}