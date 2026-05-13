#include <Arduino.h>

#define pinoRELE 4

#define pinoLDR 34

// CALIBRAÇÂO DAS VARIAVES LDR

const int LDR_NOpeople = 2200;

const int LDR_YESpeople = 1800;

// --------------------------------

unsigned long CLOCKanterior = 0;

const int CLOCKwait_test = 1000;


enum states{
  Iniciado, 
  Aron, 
  Aroff, 
  Listening
  };

states state_now = Iniciado;

bool relay_state = false;

void setup() {

  digitalWrite(pinoRELE, LOW);

  pinMode(pinoRELE, OUTPUT);

  pinMode(pinoLDR, INPUT);

  Serial.begin(115200);
  delay(1000); //tem que tirar esse delay >:( 

}

// ---- Principal ---

void Aroff_line(){ // abstrair mais essa parte

  const int proof_person = analogRead(pinoLDR);

  unsigned long CLOCKtime = millis(); 

// Parte de Depuração para o SERIAL >:) //

  static unsigned long lastDATA_SENSOR = 0;
    if (CLOCKtime - lastDATA_SENSOR > 500){
    Serial.print("Sensor LDR : "); 
    Serial.print(proof_person);
    Serial.print(" | State: "); 
    Serial.println(state_now == Aron ? "ARON (RELAY ON/NO)": state_now == Aroff ? "AROFF (RELAY OFF/NC)" : "LISTENING");
    lastDATA_SENSOR = CLOCKtime;
}
/////////////////////////////////////////

  switch(state_now){

    case Iniciado:
      
      Serial.println("Aroff system initialized.");
      Serial.println("System produced by LAARC 2026");

      if(CLOCKtime - CLOCKanterior >= CLOCKwait_test){
        Serial.println("Relay is Online and Listening ... ");

        state_now = Listening;
}
      break;

  case Listening:

    if(proof_person < LDR_YESpeople){
      Serial.println("Person Detected");
      state_now = Aron;

    } else if(proof_person < LDR_NOpeople){
      Serial.println("no person detected");
      state_now = Aroff;
}
    break;

  case Aron:
    digitalWrite(pinoRELE, HIGH);

    if(proof_person >= LDR_NOpeople){
      digitalWrite(pinoRELE, LOW);
     state_now = Aroff;
}

    break;

  case Aroff:
    digitalWrite(pinoRELE, LOW);
   
    if(proof_person < LDR_YESpeople){
      digitalWrite(pinoRELE, HIGH);
      state_now = Aron;

  }
    break;
 }
}
void loop() {
 Aroff_line();
 //depois detalhar mais para condição caso a inicialização falhe 
 //ou a conexão entre as outras esp seja notada
}
