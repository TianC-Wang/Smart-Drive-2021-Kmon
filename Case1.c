#ifdef WIN32
#include "Case1.h"
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
		if count >= 4 break;
	}
	return;
}

//return 1==Success; 0==Failed
int LookupWhiteLine() {
	//Stop both Motor
	motor(1, 0);
	motor(2, 0);
}