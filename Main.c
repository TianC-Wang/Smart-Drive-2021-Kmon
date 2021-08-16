//#include "SmartDrive.h"

const int expect = 550;
const int kp = 3;

int main(void) {
	printf("³É¹¦£¡");
	while (1) {
		/*
		cls();
		printf("%3d, %3d, %3d, %3d, %3d, %3d\n%3d, %3d, %3d", geteadc(1), geteadc(2), geteadc(3),
			geteadc(4), geteadc(5), geteadc(6), getadc(3), getadc(4), getadc(5));
		*/
		int adc3 = getadc(3);
		int p = expect - adc3;
		if (p > 100) p = 100;
		motor(1, p * kp + 500);
		motor(2, p * -kp + 500);
	}
	return 0;
}

