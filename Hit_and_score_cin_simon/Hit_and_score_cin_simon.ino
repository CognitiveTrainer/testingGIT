int pines[11]={16,5,4,0,2,14,12,13,15,3,1};
int ledRed1=pines[3];
int ledRed2=pines[4];
int ledRed3=pines[2];
int ledGreen1=pines[0];
int ledGreen2=pines[1];
int ledGreen3=pines[8];
int pinShock1= pines[6];
int pinShock2=pines[7];
int pinShock3=pines[5];

const int MAX_LEVEL = 10;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;
int velocity = 1000;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledRed1, OUTPUT);
  pinMode(ledRed2, OUTPUT);
   pinMode(ledRed3, OUTPUT);
  pinMode(ledGreen1, OUTPUT);
  pinMode(ledGreen2, OUTPUT);
  pinMode(ledGreen3, OUTPUT);
  pinMode(pinShock1, INPUT);
  pinMode(pinShock2, INPUT);
  pinMode(pinShock3, INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if (){
    secuencia();
  } else
    simon();
}

void secuencia (){
  digitalWrite(ledGreen1, HIGH);   // turn the LED on (HIGH is the voltage level)                       // wait for a second
  digitalWrite(ledGreen2, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(ledGreen3, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(ledRed1, LOW);   // turn the LED on (HIGH is the voltage level)                       // wait for a second
  digitalWrite(ledRed2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(ledRed3, HIGH);    // turn the LED off by making the voltage LOW
  int num=random(3);
  prenderLed(num);
  int aux=3;
  Serial.println("Entro al while");
  while (aux == 3){
    int aux1 = digitalRead(pinShock1);
    //Serial.println(aux1);
    int aux2 = digitalRead(pinShock2);
    Serial.println(aux2);
    int aux3 = digitalRead(pinShock3);
    if (aux1 == LOW ){
      aux=0;
    }
    if (aux2 == LOW){
      aux=1;
    }
    if (aux3 == LOW){
      aux=2;
    }
    delay(10);
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

void simon() {
  if (level == 1)
    generate_sequence();//generate a sequence;

  if (digitalRead(0) == LOW || level != 1) {
    show_sequence();    //show the sequence
    get_sequence();     //wait for your sequence
  }

}

void generate_sequence(){
  randomSeed(millis()); //in this way is really random!!!

  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(12,15);
  }
}

void show_sequence(){
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);

    for (int i = 0; i < level; i++){
      digitalWrite(sequence[i], HIGH);
      delay(velocity);
      digitalWrite(sequence[i], LOW);
      delay(500);
    }
}

void get_sequence(){
  int flag = 0; //this flag indicates if the sequence is correct

  for (int i = 0; i < level; i++){
    flag = 0;
    while(flag == 0){
      if (digitalRead(0) == LOW){
        digitalWrite(12, HIGH);
        your_sequence[i] = 12;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        digitalWrite(12, LOW);
      }

      if (digitalRead(2) == LOW){
       digitalWrite(13, HIGH);
        your_sequence[i] = 13;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        digitalWrite(13, LOW);
      }

      if (digitalRead(4) == LOW){
       digitalWrite(14, HIGH);
        your_sequence[i] = 14;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        digitalWrite(14, LOW);
      }
    }
  }
  right_sequence();
}

void wrong_sequence(){ 
  for (int i = 0; i < 3; i++){
   digitalWrite(14, HIGH);
    delay(250);
    digitalWrite(14, LOW);
    delay(250);
    digitalWrite(14, HIGH);
    delay(250);
    digitalWrite(14, LOW);
    delay(250);
    digitalWrite(14, HIGH);
    delay(250);
    digitalWrite(14, LOW);
    delay(250);
  }
  level = 1;
  velocity = 1000;
}

void right_sequence(){
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  delay(250);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(14, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  delay(250);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(14, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  delay(250);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(14, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  delay(250);
 
 
  
  if (level < MAX_LEVEL);
    level++;

  velocity -= 50; //increase difficulty
}
