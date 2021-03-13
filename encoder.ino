const int encoder_a = 3; // Pin 3
const int encoder_b = 2; // Pin 5
const int hysteresis = 5; //hysteresis 5 readings, sensitivity 2mm ((wheelDia*PI)/600)*hysteresis
const int pi = 3.14;

volatile long flag_A = 0;  //Assign a value to the token bit
volatile long encoder_pulse_counter = 0;
volatile long direction = 1;
volatile int mps = 0;

const float wheelDia = 50.0;  //wheel diameter in mm
const float singleTick = (wheelDia*PI)/600; //one tick lenght in mm
short count = 0; // unsigned long keytime = 0;



void measure() // Interrupt function
{ 
  char i;
  i = digitalRead( encoder_b );
  if (i == 1){
    count +=1;
  } 
  else {
    count -=1;
  }
  if (abs(count) >= hysteresis)  
  {
    flag_A = flag_A+count;
    count = 0;
  }
} // end measure interrupt function

void encoderPinChangeA() //RPM check pinA
{
    encoder_pulse_counter += 1;
    direction = digitalRead(encoder_a) == digitalRead(encoder_b) ? -1 : 1;
} //end interrupt

void encoderPinChangeB() //RPM check pinB
{
    encoder_pulse_counter += 1;
    direction = digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
} //end interrupt

void setup() 
{
    Serial.begin(9600);
    pinMode(encoder_a, INPUT_PULLUP);
    pinMode(encoder_b, INPUT_PULLUP);
    attachInterrupt(0, encoderPinChangeA, CHANGE);
    attachInterrupt(1, encoderPinChangeB, CHANGE);
    attachInterrupt(digitalPinToInterrupt( encoder_a), measure, RISING); //Interrupt trigger mode: RISING for measurement
}

void loop()
{
    displayUnits(flag_A);
    delay(1024);
}

void displayUnits(long ticks) 
{
      long speed = encoder_pulse_counter/600.00*60; // For encoder plate with 600 Pulses per Revolution
      mps = ( speed * pi * wheelDia ) / 60;
      Serial.print(float((float(ticks)/1000)*singleTick),3);
      Serial.print(" meters  |  ");
      Serial.print(ticks*singleTick/25.4/12);
      Serial.print(" feet  |  ");
      Serial.print(direction*speed,DEC);
      Serial.print(" RPM  |  ");
      Serial.print(mps*.0010,2);
      Serial.print(" m/S ");
      Serial.println(" ");
      Serial.println(" ");
      encoder_pulse_counter = 0; // Clear variable just before counting again 

}
