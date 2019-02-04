#include <SoftwareSerial.h>
#define DirPWM 5
#define DirDig 7
#define EsqPWM 6
#define EsqDig 8
#define sensor1 2
#define sensor2 3
int fw=0,bw=0,lt=0,rt=0;
int velEsq,velDir,vel=75;
bool y=HIGH;
bool debug = true;
unsigned long prevMillis = 0;
const long waitMillis = 5000; //ms
char c[4];

SoftwareSerial ESP(10, 9); //TX, RX

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DirDig,OUTPUT);
  pinMode(DirPWM,OUTPUT);
  pinMode(EsqDig,OUTPUT);
  pinMode(EsqPWM,OUTPUT);
 
  ESP.begin(9600);
  Serial.begin(9600);
  SendData("AT+RST",1000,debug);
  SendData("AT+CIOBAUD=9600",1000,debug);
  SendData("AT+CWMODE=3",1000,debug);
  //SendData("AT+CWJAP=\"OnePlus5\",\"noisnois\"",5000,debug);
  SendData("AT+CWJAP=\"Guilherme-2.4G\",\"abc13579\"",5000,debug);
  SendData("AT+CIFSR",5000,debug);
  SendData("AT+CIPMUX=1",1000,debug);
  SendData("AT+CIPSTART=4,\"UDP\",\"192.168.1.102\",2222,1112,0",5000,debug);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop () {
  if(ESP.available()) {
    if(ESP.find("+IPD,")) {
      if(ESP.find("p=")) {
        velEsq = 0;
        velDir = 0;
        ESP.readBytes(c,4);
        fw = c[0]-48; 
      }
    }
    defineVelocidade();
    Serial.println();
    ESP.readString();
  }
  anda();
}

void defineVelocidade() {
  if(c[0]-48 == 1) {
    Serial.println("FRENTE");
    y = HIGH;  
    velEsq+=vel;
    velDir+=vel;
    c[0]=48;
  }
  if(c[1]-48 == 1) {
    Serial.println("RÉ");
    y = LOW;
    velEsq+=vel;
    velDir+=vel;
    c[1]=48;
  }
  if(c[2]-48 == 1) {
    Serial.println(" ESQUERDA");
    velDir+=10;
    if(velEsq==75)
      velEsq-=15;
    c[2]=48;
  }
  if(c[3]-48 == 1) {
    Serial.println(" DIREITA");
    velEsq+=10;
    if(velDir==75)
      velDir-=15;
    c[3]=48;
  }
}

double proximidade() {
  double valorVolts = analogRead(A0) * 0.0048828125;
  double distancia = 4800/(valorVolts*200 - 20 );
  return distancia;
}

void anda() {
  if(proximidade() > 10) {
    Serial.println("maior que 10!");
    digitalWrite(DirDig,y); 
    analogWrite(DirPWM,velDir);
    digitalWrite(EsqDig,y); 
    analogWrite(EsqPWM,velEsq);
  } else {
    if(velEsq==vel && velDir==vel) {
      Serial.println("pra frente n anda!");
      digitalWrite(DirDig,y); 
      analogWrite(DirPWM,0);
      digitalWrite(EsqDig,y); 
      analogWrite(EsqPWM,0);
    } else {
      Serial.println("lados ou ré!");
      digitalWrite(DirDig,y); 
      analogWrite(DirPWM,velDir);
      digitalWrite(EsqDig,y); 
      analogWrite(EsqPWM,velEsq);
    }
  }
}

String SendData(String command, const int timeout, boolean debug)
  {
    String response = "";
    ESP.println(command);
    long int time = millis();
    while ( (time + timeout) > millis())
      {
        while (ESP.available())
          {
            char c = ESP.read();
            response += c;
          }
      }
    if (debug)
      {
        Serial.print(response);
      }
    return response;
  }
