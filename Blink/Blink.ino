
int pines[9]={16,5,4,0,2,14,12,13,15};
int ledRed1=pines[3];
int ledRed2=pines[4];
int ledGreen1=pines[0];
int ledGreen2=pines[1];
int pinShock1= pines[6];
int pinShock2=pines[7];

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledRed1, OUTPUT);
  pinMode(ledRed2, OUTPUT);
  pinMode(ledGreen1, OUTPUT);
  pinMode(ledGreen2, OUTPUT);
  pinMode(pinShock1, INPUT);
  pinMode(pinShock2, INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ledGreen1, HIGH);   // turn the LED on (HIGH is the voltage level)                       // wait for a second
  digitalWrite(ledGreen2, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(ledRed1, LOW);   // turn the LED on (HIGH is the voltage level)                       // wait for a second
  digitalWrite(ledRed2, LOW);    // turn the LED off by making the voltage LOW
  int num=random(2);
  prenderLed(num);
  int aux=3;
  Serial.println("Entro al while");
  while (aux == 3){
    int aux1 = digitalRead(pinShock1);
    //Serial.println(aux1);
    int aux2 = digitalRead(pinShock2);
    //Serial.println(aux2);
    if (aux1 == LOW){
      aux=0;
    }
    if (aux2 == LOW){
      aux=1;
    }
    delay(30);
  }
  if (aux==num){
    Serial.write("Ganaste");
  }
  else 
    Serial.write("Game over");
}




void prenderLed(int num) {
  digitalWrite(pines[num], LOW);
  digitalWrite(pines[num+3], HIGH);
}

