#include "mbed.h"
#include "TextLCD.h"

// motor pins
#define motor_pinA D7
#define motor_pinB D8
#define pwm_pin D5

// motor shield and potentiometer
#define enspeed_pin A0
#define pot_pin A2

// active buzzer
#define buzzer_pin D9

// lcd display pins
#define rs D12
#define en D11
#define d4 D6
#define d5 D4 
#define d6 D3
#define d7 D2

// initialize lcd
TextLCD lcd(rs, en, d4, d5, d6, d7);

// constants to calculate the breaths per minute
const double kConst_1 = 0.4204;
const double kConst_2 = -17.414;

// variable to store potentiometer reading
int speedVal = 0;

// 16 byte buffers for lcd display
char buffer[16];

//set the breaths per minute (BPM) to an initial value of 0
double BPM=0;

// state machine with starting state as STOP
enum StateMachine {STOP,RUN,WARNING,LIMIT} ;
StateMachine state = STOP;

// function to map values from one range to another
int map(int x, int in_min, int in_max, int out_min, int out_max);

int main()
{
    // setting up the pin modes
    DigitalOut motor_pin1(motor_pinA);
    DigitalOut motor_pin2(motor_pinB);
    DigitalOut buzzer(buzzer_pin); 
    PwmOut pwm(pwm_pin); 
    DigitalOut enspeed(enspeed_pin);
    AnalogIn pot(pot_pin);

    while (true) {
        // power the motor
        enspeed = 1;
        motor_pin1 = 0;
        motor_pin2 = 1;

        // get potentiometer reading
        speedVal = pot.read(); 
        speedVal = map(speedVal,0,4096,0,1023); // match speedVal to pwm
        
        // value of 55 was established after calibrating the ventilator with a tachometer
        if (speedVal<55)  
        {
            BPM = 0;
        }
        else{
            BPM = kConst_1*speedVal + kConst_2;
        }
        
        // state machine
        switch(state)
        {
            // the motor is initially off
            case STOP:  

                lcd.locate(1,0);
                lcd.printf("VENTILATOR 001");

                lcd.locate(0, 1);

                //print the BPM
                sprintf(buffer,"BPM: %3d",(int)BPM);
                lcd.printf("%s", buffer);
                
                // switch off motor of speed is below the minimum (i.e., 55)
                if(speedVal<55) 
                {
                    enspeed = 0;
                    motor_pin1 = 0;
                    pwm.period(0.001);
                    pwm.write(0);        
                }
                
                // switch state to run if speed is increased
                if (speedVal>=55 && speedVal<120) 
                {
                    state=RUN;
                }       
                break;
            
            case RUN:      

                lcd.locate(1,0);
                lcd.printf("VENTILATOR 001");

                pwm.period(0.0001);
                pwm.write(speedVal/1024);

                sprintf(buffer,"BPM:%3d",(int)BPM);

                // sound buzzer
                buzzer = 1;
                wait_us(1000000);

                lcd.locate(0, 1);
                lcd.printf("%s", buffer);
                    
                // if speed is too high, switch to state "WARNING"
                if(speedVal>=120 && speedVal<160)  
                    {
                    state=WARNING;
                    }
                
                //if speed falls below 55, switch to state "STOP"
                if(speedVal<55)  
                    {
                    state=STOP;
                    }
                break;
            
            case WARNING:

                lcd.locate(1,0);
                lcd.printf("VENTILATOR 001");

                pwm.period(0.0001);
                pwm.write(speedVal/1024);

                sprintf(buffer,"BPM:%3d",(int)BPM);
                
                buzzer = 1;
                wait_us(1000000);

                lcd.locate(0, 1);
                lcd.printf("%s", buffer);

                lcd.locate (8,1);
                lcd.printf("WARNING!");

                // if speed is too high, switch to state "LIMIT'
                if(speedVal>=160)  
                    {
                    state=LIMIT;
                    }

                // if speed is reduced, switch to state "RUN"
                if(speedVal<120 && speedVal>=55) 
                    {
                    lcd.cls();
                    state=RUN;
                    }
                    break;
            
            case LIMIT:
                //if speed is equal or exceeds 160, turn off the motor
                if(speedVal>=160) 
                {
                
                buzzer = 1;
                wait_us(1000000);
                
                pwm.period(0.0001);
                pwm.write(0);
                
                lcd.cls();
                lcd.locate(2,0);
                lcd.printf("REDUCE  SPEED");   

                lcd.locate(0,1);
                lcd.printf( "DANGEROUS BPM!!");

                wait_us(1000000);          
                }

                // if speed is reduced, switch to state "WARNING" and run the motor
                if(speedVal<160 && speedVal>=120) 
                {
                    lcd.cls();
                    state=WARNING;
                }
                break; 
        }
    }
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}