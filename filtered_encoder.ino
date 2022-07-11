void setup() {
  // put your setup code here, to run once:
  pinMode(A5,INPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);

  pinMode(6,INPUT_PULLUP);
  Serial.begin(9600);
}

int _2A = 0;
int _1A = 0;
int A = 0;
int a1 = 0;
int a2 = 0;

int _2B = 0;
int _1B = 0;
int B = 0;
int b1 = 0;
int b2 = 0;

//inputs
float potentiometer;
float value; //input velocity
int input_position = 0; //integral of velocity, theoretical position

//outputs
float angle = 0; //calculated angle, from the processed encoder signal

int round_(int value){
  if (value>900){
    return 1;
  } else if (value<100){
    return 0; }}

void loop() {

//Store the surrounding signal values to try to round out the noise. A and B are the actual "current" signal values
  _2A = _1A;
  _1A = A;
  A = a1;
  a1 = a2;
  a2 = round_(analogRead(A1)); //all of these values will be rounded to 1 or 0

  _2B = _1B;
  _1B = B;
  B = b1;
  b1 = b2;
  b2 = round_(analogRead(A0));

  //goal: filter out noise, where it spikes for just one or two values
  //filtering is a bit complicated because the ones in front haven't been processed yet, could be false
  //if you disagree with -1 and you don't have +2 backing you up, or you're 10101, flip (00101 is ok, +1 is the error)
  //00110 is likely a spike  
  if ((A!=_1A && A!=a2) || (_2A==A && A==a2 && _1A==a1 && A!=a1)){
    A = abs(A-1);
  }

  if ((B!=_1B && B!=b2) || (_2B==B && B==b2 && _1B==b1 && B!=b1)){
    B = abs(B-1); //flip from 0 to 1, or vise versa
  }

//turn the motor
  potentiometer = analogRead(A5);
  value = potentiometer/2.1 -255; //ranges from -256 to +256
  if(value>0){
    analogWrite(11,value);
    analogWrite(10,0);
  } else{
    analogWrite(11,0);
    analogWrite(10,value*-1);
  }
  input_position = input_position + value *0.1;

//detect rises/falls in the signals, and check which way the motor is turning
  if(_1A!=A || _1B!=B){
    if (value>0){
      angle += 75;
    }
    if (value<0){
      angle -= 75;
    }
  }

//display values
//  Serial.print("A:");
//  Serial.print(A);
//  Serial.print(",");
//  Serial.print("B:");
//  Serial.print(B); 
//  Serial.print(",");

  Serial.print("input_velocity:");
  Serial.print(value);
  Serial.print(",");
  Serial.print("input_position:");
  Serial.print(input_position);
  Serial.print(",");

  Serial.print("detected_angle:");
  Serial.print(angle);

  Serial.println();

}



// https://www.amazon.com/25GA370-Encoder-Metal-Gearmotor-150RPM/dp/B07GNFYGYQ
// 408 pulses per rotation
//https://en.wikipedia.org/wiki/Incremental_encoder#Position_reporting

//see whether A or B signal changes first to determine direction of movement
//  if (_1A==_1B && _1B==B && _1A!=A){
//    angle = angle + 100;
//  }
//  if (_1A==_1B && _1B!=B && _1A==A){
//    angle = angle - 100;
//  }

