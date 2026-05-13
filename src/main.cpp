#include <Arduino.h>

#define pinoRELE 2
#define pinoLDR 34

 // CALIBRAÇÂO DAS VARIAVES LDR 
const int LDR_ALIVE = 2200;
const int LDR_KEEP_GOING = 1800;
// --------------------------------


unsigned long CLOCKanterior = 0;
const int CLOCKwait_test = 1000;

enum states{Iniciado, Aron, Aroff, Listening};
states state_now = Iniciado;
bool relay_state = false;


void setup() {
  pinMode(pinoRELE, OUTPUT);
  pinMode(pinoLDR, INPUT);
  Serial.begin(9600);
  Serial.println("Aroff system initialized.");
  Serial.println("System produced by LAARC 2026");
  
}

void Aroff_line(){
  const int proof_person = analogRead(pinoLDR);
  
  unsigned long CLOCKtime = millis(); // abstrair mais essa parte
  
  switch(state_now){
    
    case Iniciado:
  
      if(CLOCKtime - CLOCKanterior >= CLOCKwait_test){
        digitalWrite(pinoRELE, LOW);
        Serial.println("Relay is Online");
        CLOCKanterior = CLOCKtime;
        state_now = Listening;
      }else{
        Serial.println("Relay is Offline");
        digitalWrite(pinoRELE, HIGH);
      }
    break;

    case Listening:
        if(proof_person >= LDR_ALIVE){

          Serial.println("Person Detected");
          state_now = Aron;
      } else if(proof_person < LDR_KEEP_GOING){
          Serial.println("no person detected");
          state_now = Aroff;
      }
      break;

    case Aron:
      digitalWrite(pinoRELE, LOW);

      if(proof_person < LDR_KEEP_GOING){
        digitalWrite(pinoRELE, HIGH);
        state_now = Aroff;
      }
    break;

    case Aroff:
      digitalWrite(pinoRELE, HIGH);
      
      if(proof_person >= LDR_ALIVE){
        digitalWrite(pinoRELE, LOW);
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


