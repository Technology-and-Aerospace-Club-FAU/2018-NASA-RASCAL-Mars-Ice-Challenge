//This setup assumes that the left joystick (joystick 1) controls x and y movement (horizontal and vertical on the joystick respectively) and joystick 2 controls z movement(vertical) and z rotation(horizontal)

//pin 13 is a bad pin to use with pull up resistor because it also powers on-board led leading to voltage high=1.7v instead of 5v
//speed control will be PWM because of its greater and constant torque, and because it is more efficient
//pins 5 and 6 will have higher than expected duty cycles when used with PWM
//analogWriteResolution of Mega is 8 bits (255)	
//pwm is supported for arduino mega on pins 2-13, 44-46 at approximately 490 Hz

const int SW_PIN1=0;                            //Input setup, SW_PIN is joystick push button
const int SW_PIN2=1;
const int X_PIN1=A0;
const int X_PIN2=A2;
const int Y_PIN1=A1;
const int Y_PIN2=A3;

const int motorX=2;                             //output setup (motor setup)
const int motorY=3;
const int motorZ=4;
const int motorZrotation=5;


bool emergencyMode = false;                     //emergency ability set up, emergency mode can be initiated and ended by pressing down both joysticks
bool emergencyInitiatePreviously= false;

void setup(){
	pinMode(SW_PIN1, INPUT_PULLUP);               //input set up, to learn what pull up resistors do ask Ahsan
	pinMode(SW_PIN2, INPUT_PULLUP);
	pinMode(X_PIN1, INPUT);
	pinMode(X_PIN2, INPUT);
	pinMode(Y_PIN1, INPUT);
	pinMode(Y_PIN2, INPUT);

	pinMode(motorX, OUTPUT);
	pinMode(motorY, OUTPUT);
	pinMode(motorZ, OUTPUT);
	pinMode(motorZrotation, OUTPUT);
	

	emergencyMode=false;
  emergencyInitiatePreviously=false;
	Serial.begin(9600);
}


void loop(){
	if (digitalRead(SW_PIN1)== LOW && digitalRead(SW_PIN2)== LOW){                      //emergency code, stability needs to be tested because reliability of joystick button is unknown
		if (!emergencyInitiatePreviously){
			String message;
			if(emergencyMode){
				message= ("Emergency over %d from time of program start\n",millis()/1000);
				Serial.print(message);
			}else{
				message= "Emergency started %d from time of program start\n", millis()/1000;
				Serial.print(message);
			}
		}
		emergencyInitiatePreviously=true;
	}
  if (emergencyMode){                                                                 //no inputs are read and all outputs are zero when in emergency mode
    analogWrite(motorX, 0);
    analogWrite(motorY, 0);
    analogWrite(motorZ, 0);
    analogWrite(motorZrotation, 0);
    
		return;
	}

	analogWrite(motorX, map(analogRead(X_PIN1), 0, 1023, 0, 255));                      //sets motor speeds using PWM, talk to Ahsan if you want to learn more
	analogWrite(motorY, map(analogRead(Y_PIN1), 0, 1023, 0, 255));
	analogWrite(motorZrotation, map(analogRead(X_PIN2), 0, 1023, 0, 255));
	analogWrite(motorZ, map(analogRead(Y_PIN2), 0, 1023, 0, 255));


}
