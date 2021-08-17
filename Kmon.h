#define bool char
#define true 1
#define false 0

// @brief Clear LCD Screen.
extern void cls(void);

// @brief Locate LCD Cursor to Target Pos.
// @param x X Position.
// @param y Y Position.
extern void locate(unsigned char x, unsigned char y);

// @brief Normal Printf Function.
// @param format Format String.
// @param ... Format Parameters.
extern void printf(const char* format, ...);

// @brief Beep at a Frequency for a Period of Time.
// @param fre Frequency.
// @param tim Time.
extern void sound(unsigned long int fre, float tim);

// @brief Run a Motor at a Speed.
// @param channel Motor Channel(1~2).
// @param speed Motor Speed(-1000~1000).
extern void motor(int channel, int speed);

// @brief Run both Motors once.
// @param x Speed of Motor on Channel No.1.
// @param y Speed of Motor on Channel No.2.
extern void go(int x, int y);

// @brief Run a Smaller Motor.
// @param mode 0:Stop; 1:Forward; 2:Backward.
extern void exmotor(unsigned char mode);

// @brief Set a Servo.
// @param svo_num Index of Servo(1~4).
// @param svo_val Position of Servo(0~255).
extern void servo(int svo_num, int svo_val);

// @brief Servos Drive.
// @param svo_num Index of Servo(1~4).
// @param ... End with -1;
extern void s_servo(int svo_num, ...);

// @brief Servo Drive with a Time Delay.
// @param svo_number Index of Servo(1~4).
// @param ... End with -1;
extern void st_servo(int svo_number, ...);

// @brief Delay the Program for Seconds.
// @param nTime Time.
extern void wait(double nTime);

// @brief Get an EADC Analog Input.
// @param x Port Index.
// @return Value(0~1023).
extern int geteadc(int x);

// @brief Get an ADC Analog Input.
// @param x Port Index.
// @return Value(0~1023).
extern int getadc(int x);

// @brief Get a Digital Input.
// @param port Port Index.
// @return Level(0~1).
extern int getport(unsigned char port);

// @brief Set a Digital Output.
// @param port Port Index.
// @param val Level(0~1).
extern void setport(unsigned char port, unsigned char val);

// @brief Get the Start Button Status.
// @return 0:Releasing; 1:Pressing.
extern int trigger(void);

// @brief Get the Wheel Button Status.
// @return 0:Releasing; 1:Pressing.
extern int sel_but(void);

// @brief Get the Compass Sensor Value.
// @return The Compass Value(0~359).
extern int compass(void);

// @brief Get the Battery Voltage.
// @return The Battery Voltage.
extern float battery(void);

// @brief Read a Value from a Slot.
// @param index The Index of The Slot(0~49).
// @return The Value.
extern int EE_ReadVariable(int index);

// @brief Write a Value to a Slot.
// @param index The Index of The Slot(0~49).
// @param val The Value.
extern void EE_WriteVariable(int index, int val);