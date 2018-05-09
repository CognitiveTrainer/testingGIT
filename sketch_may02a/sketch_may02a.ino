int col[]={2,3,4,5,6};
int fila[]={7,8,9,10,11,12,13};

void setup() {
  // put your setup code here, to run once:
  for(int i=0;i<=4;i++){
    pinMode(col[i], OUTPUT);
    digitalWrite(col[i],HIGH);
  }
  for(int i=0;i<=6;i++){
    pinMode(fila[i], OUTPUT);
    digitalWrite(fila[i],LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //  COLUMNAS EN LOW SE PRENDEN
  //  FILAS EN HIGH SE PRENDEN
    
  cruz();
}

void tick(){
  // TICK
  int j=0;
  boolean flag=false;
  for(int i=4;i>=0;i--){ 
    digitalWrite(col[i],LOW);
    digitalWrite(fila[j],HIGH);
    delay(1);
    digitalWrite(col[i],HIGH);
    digitalWrite(fila[j],LOW);
    if (j==3){
      flag=true;
    }
    if(flag){
      j--;
    } else {
      j++;
    }
  }
}

void cruz(){
  
//CRUZ DIAGONAL HACIA DERECHA
  for(int i=0;i<=4;i++){
    digitalWrite(col[i],LOW);
    digitalWrite(fila[i],HIGH);
    delay(1);
    digitalWrite(col[i],HIGH);
    digitalWrite(fila[i],LOW);
  }

// CRUZ DIAGONAL HACIA IZQUIERDA
  int j=0;
  for(int i=4;i>=0;i--){
    digitalWrite(col[i],LOW);
    digitalWrite(fila[j],HIGH);
    delay(1);
    digitalWrite(col[i],HIGH);
    digitalWrite(fila[j],LOW);
    j++;
  }
  
}
