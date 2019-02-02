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

SoftwareSerial ESP(10, 9); //TX, RX

unsigned long prevMillis = 0;
const long waitMillis = 5000; //ms

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DirDig,OUTPUT);
  pinMode(DirPWM,OUTPUT);
  pinMode(EsqDig,OUTPUT);
  pinMode(EsqPWM,OUTPUT);
 
  ESP.begin(9600);
  Serial.begin(9600);
  
  Serial.println("Testando comando AT");
  SendData("AT",2000,debug);
  Serial.println("Resetando modulo");
  SendData("AT+RST",2000,debug);
  Serial.println("Mudando baudrate");
  SendData("AT+CIOBAUD=9600",2000,debug);
  
  Serial.println("softAP+station mode");
  SendData("AT+CWMODE=3",1000,debug);

  /*
  Serial.println("connecting...");
  SendData("AT+CWJAP=\"OnePlus5\",\"noisnois\"",10000,debug);
  */
  Serial.println("connecting...");
  SendData("AT+CWJAP=\"Guilherme-2.4G\",\"abc13579\"",10000,debug);
  
  Serial.println("ip info");
  SendData("AT+CIFSR",5000,debug);
  
  Serial.println("multiple connections");
  SendData("AT+CIPMUX=1",1000,debug);
  
  Serial.println("opening connection...");
  SendData("AT+CIPSTART=4,\"UDP\",\"192.168.4.2\",2222,1112,0",10000,debug);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop () {
  String msg="";
  if(ESP.available()) {
    if(ESP.find("+IPD,")) {
      if(ESP.find("fw=")) {
        char c[1];
        ESP.readBytes(c,1);
        fw = c[0]-48; 
      }
      if(ESP.find("bw=")) {
        char c[1];
        ESP.readBytes(c,1);
        bw = c[0]-48; 
      }
      if(ESP.find("lt=")) {
        char c[1];
        ESP.readBytes(c,1);
        lt = c[0]-48; 
      }
      if(ESP.find("rt=")) {
        char c[1];
        ESP.readBytes(c,1);
        rt = c[0]-48; 
      }
    }
    velEsq = 0;
    velDir = 0;
    if(fw==1) {
      Serial.println("FRENTE");
        y = HIGH;
       velEsq+=vel;
      velDir+=vel;
      fw=0;
      }
    if(bw==1) {
           Serial.println("RÉ");
      y = LOW;
      velEsq+=vel;
      velDir+=vel;
      bw=0;
    }
    if(lt==1) {
           Serial.println(" ESQUERDA");
      velDir+=10;
      velEsq-=15;
      lt=0;
    }
    if(rt==1) {
           Serial.println(" DIREITA");
      velEsq+=10;
      velDir-=15;
      rt=0;
    }
    Serial.println();
    ESP.readString();
  }
  
  digitalWrite(DirDig,y); 
    analogWrite(DirPWM,velDir);
    digitalWrite(EsqDig,y); 
    analogWrite(EsqPWM,velEsq);
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
