const int MAX_LEVEL = 10;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;

int velocity = 1000;

void setup() {
  pinMode(0, INPUT);  //azul
  pinMode(2, INPUT);  //verde
  pinMode(4, INPUT);  //rojo (infrarojo)
  
  pinMode(12, OUTPUT);  //azul
  pinMode(13, OUTPUT);  //verde
  pinMode(14, OUTPUT);  //rojo
  
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);

}

void loop(){
  simon();
}
void simon() {
  if (level == 1)
    generate_sequence();//generate a sequence;

  if (digitalRead(0) == LOW || level != 1) {
    while (digitalRead(0) == LOW){
    }
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
        while (digitalRead(0) == LOW){
        }
        your_sequence[i] = 12;
        flag = 1;
        delay(200);
        digitalWrite(12, LOW);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        
      }

      if (digitalRead(2) == LOW){
       digitalWrite(13, HIGH);
        while (digitalRead(2) == LOW){
        }
        your_sequence[i] = 13;
        flag = 1;
        delay(200);
        digitalWrite(13, LOW);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        
      }

      if (digitalRead(4) == LOW){
       digitalWrite(14, HIGH);
        while (digitalRead(4) == LOW){
        }
       delay(50);
        your_sequence[i] = 14;
        flag = 1;
        delay(200);
        digitalWrite(14, LOW);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        
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
