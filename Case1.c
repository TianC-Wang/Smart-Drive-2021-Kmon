#ifdef WIN32
#include "Kmon.h"
#endif

#define Increase 1
#define Decrease 2

//For the use of adcTreshold
const int adcTreshold = 200;

int eadcSensor[6];

void UpdateeadcSensor(void) {
	for (int a = 0; a < 6; a++) eadcSensor[a] = geteadc(a + 1);
	printf("%d %d %d %d %d %d", eadcSensor[0], eadcSensor[1], eadcSensor[2], eadcSensor[3], eadcSensor[4],
		eadcSensor[5]);
}

//return 1==Success; 0==Failed
int PassWhiteLine(int TriesLimited) {
	for (int a = 0; a < TriesLimited; a++) {
		UpdateeadcSensor();
		int count = 0;
		if (eadcSensor[1] <= adcTreshold) count++;
		if (eadcSensor[2] <= adcTreshold) count++;
		if (eadcSensor[3] <= adcTreshold) count++;
		if (eadcSensor[4] <= adcTreshold) count++;
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
	int adc3 = 0;
	int adc3p = 0;
	int status;
	int flag = 0;
	int ReturnValue = 0;
	for (int tries = 0; tries < 1000; tries++) {
		if (adc3 == 0) {
			//Mark the Current Value
			adc3 = getadc(3);
			//Here should have A TRY that if Value=100 Could WORK!
			motor(1, 100);
		}
		//Judge Increase or Decrease
		else if (adc3 != 0 && adc3p == 0) {
			adc3p = adc3;
			adc3 = getadc(3);
			if (adc3 > adc3p) flag = Increase;
			if (adc3 < adc3p) flag = Decrease;
			if (adc3 = adc3p) adc3p = 0;
		}
		else if (flag == Increase) {
			//Here should have A TRY that if Value=100 Could WORK!
			motor(1, -100);
			adc3p = adc3;
			adc3 = getadc(3);
			flag = Decrease;
		}
		else if (flag == Decrease) {
			adc3p = adc3;
			adc3 = getadc(3);
			if (adc3p < adc3) {
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
	UpdateeadcSensor();
	if (eadcSensor[0] > adcTreshold && eadcSensor[1] > adcTreshold && eadcSensor[2] > adcTreshold &&
		eadcSensor[3] > adcTreshold && eadcSensor[4] > adcTreshold && eadcSensor[5] > adcTreshold) {
		return 0;
	}
LookupStarted:
	if (eadcSensor[1] <= adcTreshold) {
		if (eadcSensor[4] > adcTreshold) {
			motor(2, 100);
			//Tries should have A TEST! (Value of a)
			for (int a = 0; a < 50; a++) {
				if (geteadc(5) <= adcTreshold) {
					motor(2, 0);
					return 1;
				}
			}
			motor(2, -100);
			for (int a = 0; a < 100; a++) {
				if (geteadc(5) <= adcTreshold) {
					motor(2, 0);
					return 1;
				}
			}
		}
	}
	else if (eadcSensor[4] <= adcTreshold) {
		if (eadcSensor[1] > adcTreshold) {
			motor(1, 100);
			//Tries should have A TEST! (Value of a)
			for (int a = 0; a < 50; a++) {
				if (geteadc(2) <= adcTreshold) {
					motor(1, 0);
					return 1;
				}
			}
			motor(2, -100);
			for (int a = 0; a < 100; a++) {
				if (geteadc(2) <= adcTreshold) {
					motor(1, 0);
					return 1;
				}
			}
		}
	}
	else {
		go(100, 100);
		for (int a = 0; a < 50; a++) {
			UpdateeadcSensor();
			if (eadcSensor[1] <= adcTreshold || eadcSensor[4] <= adcTreshold) {
				goto LookupStarted;
			}
		}
	}
	motor(1, 0); motor(2, 0);
	return 0;
}

void Display(void) {
	while (1) {
		cls();
		printf("\n\n\n%3d, %3d, %3d, %3d, %3d, %3d\n%3d, %3d, %3d", geteadc(1), geteadc(2), geteadc(3),
			geteadc(4), geteadc(5), geteadc(6), getadc(3), getadc(4), getadc(5));
		wait(0.5);
	}
}

const int kp = 1.2;
const int expect = 450;

void RoadPlay(void) {
	int adc3 = getadc(3);
	int p = expect - adc3;
	if (p > 100) p = 100;
	motor(1, p * kp + 500);
	motor(2, p * -kp + 500);
}

/// @brief Entrance of The Program.
/// @return Status Code.
int main(void) {
	printf("成功！\n");

	Display();

	motor(1, 200); motor(2, 200);
	int debug = PassWhiteLine(10000);
	if (debug == 1)printf("已过白线\n");
	if (debug == 0)printf("滚！");
	motor(1, 0); motor(2, 0);

	return 0;
}