/****************************************************
name:IR Obstacle Avoidance Sensor
function:place a piece of paper in front of the Obstacle Avoidance Sensor,
and the LED attached to pin 13 on the SunFounder Uno board will light up. 
******************************************************/
//Email:support@sunfounder.com
const int keyPin = 11; //the number of the key pin
const int avoidPin = 7; //the ir obstacle sensor attach to pin 7
const int buzzerPin = 8;//the buzzer pin attach to

const int redPin = 9; // R petal on RGB LED module connected to digital pin 11 
const int greenPin = 10;
void setup()
{
  Serial.begin(9600);
  pinMode(avoidPin, INPUT); //set the avoidPin as INPUT
  pinMode(redPin, OUTPUT); // sets the redPin to be an output 
  pinMode(greenPin, OUTPUT); // sets the greenPin to be an output 
  pinMode(keyPin,INPUT);//initialize the key pin as input 
  color(0,255); // turn the RGB LED red 
}
  
void loop()
{

  boolean avoidVal;
  boolean  Value=digitalRead(keyPin);//read the state of the key value
  
  int flag=0;
  if(Value == LOW && flag==0)
  {
   color(255,0);
   flag=1;
  }
  
  while(flag==1){
    avoidVal = digitalRead(avoidPin); //read the value of pin7
    if(avoidVal == LOW) //if the value is LOW level
    {
       tone(8,2000, 100);
       color(0, 255); 
       flag=0;
    }
  }
 
}

void color (unsigned char red, unsigned char green) // the color generating function 
{ 
  analogWrite(redPin, red); 
  analogWrite(greenPin, green); 
}

