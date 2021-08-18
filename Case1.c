#ifdef WIN32
#include "Kmon.h"
#endif

#define Increase 1
#define Decrease 2

//For the use of eadcTreshold1
const int eadcTreshold1 = 400;

int eadcSensor[6];

void UpdateeadcSensor(void) {
	for (int a = 0; a < 6; a++) eadcSensor[a] = geteadc(a + 1);
	//printf("%d %d %d %d %d %d\n", eadcSensor[0], eadcSensor[1], eadcSensor[2], eadcSensor[3], eadcSensor[4],eadcSensor[5]);
}


int ToBlackRoad(int TriesLimited) {
	for (int a = 0; a < TriesLimited; a++) {
		cls();
		UpdateeadcSensor();
		int count = 0;
		if (eadcSensor[1] > eadcTreshold1) count++;
		if (eadcSensor[2] > eadcTreshold1) count++;
		if (eadcSensor[3] > eadcTreshold1) count++;
		if (eadcSensor[4] > eadcTreshold1) count++;
		//Can edit the Sensor nunmber of detected
		if (count >= 2) {
			return 1;
		}
		//}
		//cls();
		//UpdateeadcSensor();
		//int count = 0;
		//if (eadcSensor[0] > 190) count++;
		//if (eadcSensor[5] > 235) count++;
		////Can edit the Sensor nunmber of detected
		//if (count >= 1) {
		//	return 1;
		//}
	}
	return 0;
}

//return 1==Success; 0==Failed
int PassWhiteLine(int TriesLimited) {
	ToBlackRoad(TriesLimited);
	for (int a = 0; a < TriesLimited; a++) {
		cls();
		UpdateeadcSensor();
		int count = 0;
		if (eadcSensor[1] <= eadcTreshold1) count++;
		if (eadcSensor[2] <= eadcTreshold1) count++;
		if (eadcSensor[3] <= eadcTreshold1) count++;
		if (eadcSensor[4] <= eadcTreshold1) count++;
		//Can edit the Sensor nunmber of detected
		if (count >= 1) {
			return 2;
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
//int LookupWhiteLineWithSenior(void) {
//	motor(1, 0); motor(2, 0);
//	UpdateeadcSensor();
//	if (eadcSensor[0] > eadcTreshold1 && eadcSensor[1] > eadcTreshold1 && eadcSensor[2] > eadcTreshold1 &&
//		eadcSensor[3] > eadcTreshold1 && eadcSensor[4] > eadcTreshold1 && eadcSensor[5] > eadcTreshold1) {
//		return 0;
//	}
//LookupStarted:
//	if (eadcSensor[2] <= eadcTreshold1) {
//		if (eadcSensor[4] > eadcTreshold1) {
//			motor(2, 180);
//			//Tries should have A TEST! (Value of a)
//			for (int a = 0; a < 10000; a++) {
//				if (geteadc(5) <= eadcTreshold1) {
//					motor(2, 0);
//					return 1;
//				}
//			}
//			motor(2, -180);
//			for (int a = 0; a < 200; a++) {
//				if (geteadc(5) <= eadcTreshold1) {
//					motor(2, 0);
//					return 1;
//				}
//			}
//		}
//	}
//	else if (eadcSensor[4] <= eadcTreshold1) {
//		if (eadcSensor[1] > eadcTreshold1) {
//			motor(1, 200);
//			//Tries should have A TEST! (Value of a)
//			for (int a = 0; a < 50; a++) {
//				if (geteadc(2) <= eadcTreshold1) {
//					motor(1, 0);
//					return 1;
//				}
//			}
//			motor(2, -200);
//			for (int a = 0; a < 100; a++) {
//				if (geteadc(2) <= eadcTreshold1) {
//					motor(1, 0);
//					return 1;
//				}
//			}
//		}
//	}
//	else {
//		go(100, 100);
//		for (int a = 0; a < 50; a++) {
//			UpdateeadcSensor();
//			if (eadcSensor[1] <= eadcTreshold1 || eadcSensor[4] <= eadcTreshold1) {
//				goto LookupStarted;
//			}
//		}
//	}
//	motor(1, 0); motor(2, 0);
//	return 0;
//}


//那年我们错过的爱情，是我们如今才会不去的曾经
int LookupWhiteLineWithSensor(void) {
	UpdateeadcSensor();
	if (eadcSensor[0] > eadcTreshold1 && eadcSensor[1] > eadcTreshold1 && eadcSensor[2] > eadcTreshold1 &&
		eadcSensor[3] > eadcTreshold1 && eadcSensor[4] > eadcTreshold1 && eadcSensor[5] > eadcTreshold1) {
		return 0;
	}
	motor(1, 220); motor(2, 220);
	char flagL = 0;
	char flagR = 0;
	while (1) {
		UpdateeadcSensor();
		if (eadcSensor[0] <= 190) {
			flagL = 1;
			motor(1, -180);
		}
		if (eadcSensor[5] <= 235) {
			flagR = 1;
			motor(2, -180);
		}
		if (flagL == 1 && flagR == 1) {
			wait(0.1);
			motor(1, 0); motor(2, 0);
			UpdateeadcSensor();
			printf("%d %d", eadcSensor[0], eadcSensor[5]);
			return 1;
		}
	}
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

	//Display();

	motor(1, 200); motor(2, 200);
	int debug = PassWhiteLine(10000);
	if (debug == 1)printf("已过白线\n");
	if (debug == 0)printf("滚！");
	motor(1, 0); motor(2, 0);
	debug = LookupWhiteLineWithSensor();
	if (debug == 1)printf("已校准\n");
	if (debug == 0)printf("滚！");
	return 0;
}

