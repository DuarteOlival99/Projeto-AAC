//desativar inputs apos carregar A sem meter a pass
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

const int azul = 47;
const int verde = 49;
const int vermelho = 51;
const int pir = 52;
int motor = 21;//servo para porta = 21 ; servo para janela = 22;
const int buzzer = 3;
const int agua = 38;
const int luzesHall = 31;//amarela
const int luzesCozinha = 33;//vermelha
const int luzesSala = 35;//verde
const int luzesWC = 37;//azul
const int ventoinha = 2;


//Gas  
const int gas = A2;//PINO UTILIZADO PELO SENSOR DE GÁS MQ-2
boolean sensorGas = false;
int valorLidoGas = 400;
int leitura_sensor = 300;//DEFININDO UM VALOR LIMITE (NÍVEL DE GÁS NORMAL)

//ventoinhas
boolean ventoinhaBoolean= false;

//MENUS
int menu=0;
boolean menuSensores = true;
boolean menuLuzesLigadas = true;
boolean menuLuzesDesligadas = true;
boolean menuAnteriorA = false;
boolean menuAnteriorB = false;

//pir
int val = 0;
int pirState = LOW; 
boolean sensorPir = false;
boolean pirDetetado = false;

//Agua
int rainsense= A0; // analog sensor input pin 0
int countAgua= 0;  
boolean aguaLer = false;

//LCD
//LiquidCrystal lcd(34, 32, 30, 28, 26, 24);


//motor
int posMotor = 0;
Servo servo;
boolean portaAberta=false;
boolean abrirPorta=false;
boolean abrirJanela=false;

//temperatura
int temperaturaUser = 16; 
int valT;
int tempPin = A1;
float cel = 0.0;
int t = 0;

boolean sensorTemperatura = false;

//Pass+
String password="1234";
String passwordTemporaria;
int count=0;
int tentativas=0;
boolean ativo = false;
boolean ativarAlarme = false;
boolean alarmeAtivado = false;
boolean passAtiva = false;
boolean UserAtivoAlarme = false;
boolean mudarPass=false;
boolean modoMudarPass=false;
boolean passAlterada=false;
boolean tocarBuzzerTecla = false;

const byte linhas = 4; //four rows
const byte coluna = 4; //three columns
char keys[linhas][coluna] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[linhas] = {13,12,11,10}; //connect to the row pinouts of the keypad
byte colPins[coluna] = {9, 8, 7,6}; //connect to the column pinouts of the keypad

Keypad teclado = Keypad( makeKeymap(keys), rowPins, colPins, linhas, coluna );

char sequencia[100];


void setup(){
 Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(pir, INPUT); 
  pinMode(rainsense, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(agua,OUTPUT);
  digitalWrite(agua,HIGH);
  pinMode(luzesHall,OUTPUT);
  pinMode(luzesCozinha,OUTPUT);
  pinMode(luzesSala,OUTPUT);
  pinMode(luzesWC,OUTPUT);
  pinMode(ventoinha, OUTPUT);
}

// ***** LED RGB *******

void vermelhoFuncao(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, LOW);
  digitalWrite(vermelho, HIGH);
}
void vermelhoFuncaoDelay(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, LOW);
  digitalWrite(vermelho, HIGH);
  delay (100);
}
void azulFuncao(){
  digitalWrite(azul, HIGH);
  digitalWrite(verde, LOW);
  digitalWrite(vermelho, LOW);
}
void verdeFuncao(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, HIGH);
  digitalWrite(vermelho, LOW);
}
void amareloFuncao(){
  analogWrite(azul, 0);
  analogWrite(verde, 50);
  analogWrite(vermelho, 255);
}
void roxoFuncao(){
  analogWrite(azul, 207);
  analogWrite(verde, 0);
  analogWrite(vermelho, 255);
}
void brancoFuncao(){
  digitalWrite(azul, HIGH);
  digitalWrite(verde, HIGH);
  digitalWrite(vermelho, HIGH);
}
void desligaRGB(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, LOW);
  digitalWrite(vermelho, LOW);  
  }

// ****** Alarme *****

void inserirPass(){
  int pos = 0;
  passwordTemporaria = "";
  ativo = true;
  Serial.println("Introduza a pass\n");
   while(ativo){
    
     char tecla2 = teclado.getKey();
     if (tecla2 != NO_KEY){ 
      
       if(tecla2 == '1' || tecla2 == '2' || tecla2 == '3' || tecla2 == 'A' ||
          tecla2 == '4' || tecla2 == '5' || tecla2 == '6' || tecla2 == 'B' ||
          tecla2 == '7' || tecla2 == '8' || tecla2 == '9' || tecla2 == 'C' ||
          tecla2 == '0' || tecla2 == 'D'){
         
         passwordTemporaria += tecla2;
         Serial.print("*");
         if(tocarBuzzerTecla==false){
          tone (buzzer,4000);
          delay(40);
          noTone(buzzer);
         }
         pos++;
       }
       
       if( pos > 4){
          Serial.println("Limite excedido!\n Tente novamente!\n");           
          pos = 0;
          passwordTemporaria = ""; 
          Serial.println("Introduza a password.\n");
         
         }
       if(tecla2 == '*'){
          Serial.println("Tentativa apagada!\n Introduza novamente.\n");
          pos = 0;
          passwordTemporaria = ""; 
          Serial.println("Introduza a password.\n");

           }
       if(tecla2 == '#' && pos==4){
            
              if(password == passwordTemporaria ){
            
                  ativo = false; 
                  alarmeAtivado = false;
                  noTone(buzzer);
                   verdeFuncao();
                   delay(50);
                   menu = 0;
                  tentativas=0;
                  passAtiva=true;
                  tocarBuzzerTecla = false;
                  ativarAlarme=false;
                  abrirPorta=true;
                  abrirJanela=true;
                                              
                  }else if(password != passwordTemporaria){
                
                  Serial.println("Password errada!\n Tente novamente.\n");
                 pos = 0;
                 passwordTemporaria = ""; 
                  tentativas++;
                  if(tentativas>=3){  
                  tone (buzzer,1000);
                  tocarBuzzerTecla=true;
                    vermelhoFuncao();             

                  }
                    Serial.println("Introduza a password:\n");

                }
       }      
     }
   }
}


void introduzirPassAntiga(){

  int pos=0;
  
  passwordTemporaria= ""; 
  Serial.println("Password antiga:\n");
  mudarPass = true;
  modoMudarPass = true;
  while(mudarPass){
    char tecla2 = teclado.getKey();
     if (tecla2 != NO_KEY){ 
      
       if(tecla2 == '1' || tecla2 == '2' || tecla2 == '3' || tecla2 == 'A' ||
          tecla2 == '4' || tecla2 == '5' || tecla2 == '6' || tecla2 == 'B' ||
          tecla2 == '7' || tecla2 == '8' || tecla2 == '9' || tecla2 == 'C' ||
          tecla2 == '0' || tecla2 == 'D'){
         
         passwordTemporaria += tecla2;
         Serial.print("*");
         if(tocarBuzzerTecla==false){
         tone (buzzer,4000);
          delay(40);
          noTone(buzzer);
         }
         pos++;
       }
       
        if( pos > 4 || tecla2 == '*'){

          Serial.println("Password antiga:\n");           
          pos = 0;
          passwordTemporaria = "";        
         }
        
         if(  tecla2 == '#' && pos==4){
           pos=0;
           passNova();
           
         } 
     }           
  }
}


void passNova(){
//por pass erada ao mudar a pass e depois por a certa corrigir erro
  int pos = 0;
  if(password == passwordTemporaria ){
    int count = 0;
    tentativas=0;
    passwordTemporaria = "";
    Serial.println("\nPassword nova:");
              
      while(modoMudarPass){
                
        char tecla = teclado.getKey();
          if (tecla != NO_KEY){ 
            if(tecla == '1' || tecla == '2' || tecla == '3' || tecla == 'A' ||
            tecla == '4' || tecla == '5' || tecla == '6' || tecla == 'B' ||
            tecla == '7' || tecla == '8' || tecla == '9' || tecla == 'C' ||
           tecla == '0' || tecla == 'D'){

                  count++;
              passwordTemporaria += tecla;
              Serial.print("*");
              if(tocarBuzzerTecla==false){
              tone (buzzer,4000);
          delay(40);
          noTone(buzzer);
              } 
              pos++;
            }
                  
            if(tecla == '*' || pos > 4 ){
            count =0;
              pos = 0;
              passwordTemporaria = "";   
              Serial.println("\nPassword nova:");
            }

            if(tecla == '#' && count==4){
              count = 0;
              password = passwordTemporaria;
              verdeFuncao();
              delay(500);
              
               Serial.println("Password alterada com sucesso!\n");
              ativarAlarme=false;
              alarmeAtivado=false;
              passAtiva=true;
              tocarBuzzerTecla = false;
              modoMudarPass=false;
              mudarPass = false;
              menu=0;
              tentativas=0;
            }
          }
        }
  }else{
    Serial.println("Password Errada!\n Tente novamente.\n");
    tentativas++;
    passwordTemporaria = "";   
    pos=0;
      if(tentativas>=3){ 
        tone (buzzer,1000);
        tocarBuzzerTecla=true;
        vermelhoFuncao();
      }
    }
}

//****** Sensores ******
// pir

void funcaoPir(){
  pirDetetado=false;
  val = digitalRead(pir);  
  if (val == HIGH) {            
   
   // if (pirState == LOW) {
       tone (buzzer,1000);
       Serial.println("Movimento detetado no hall!");
        vermelhoFuncao(); 
       pirState = HIGH;
       sensorPir = true;
       pirDetetado=true;
    //}
  } else {
      //if (pirState == HIGH){
         Serial.println("Nao ha movimento!");
        noTone(buzzer);
        verdeFuncao();
        pirState = LOW;
        sensorPir = false;
     // }
    }
}

// motor

void funcaoMotorAbrir(){
servo.attach(motor);
   // sweep the servo from 0 to 180 degrees in steps of 1 degrees
      for (posMotor = 0; posMotor <= 110; posMotor += 1) {
        // tell servo to go to position in variable 'pos'
        servo.write(posMotor);
        delay(15);
      }

}

void funcaoMotorFechar(){
  servo.attach(motor);
  for (posMotor = 110; posMotor >= 0; posMotor -= 1) {
        // tell servo to go to position in variable 'pos'
        servo.write(posMotor);
        delay(15);
      }
  }

void funcaoMotor(){
  
  servo.attach(motor);
   // sweep the servo from 0 to 180 degrees in steps of 1 degrees
      for (posMotor = 0; posMotor <= 110; posMotor += 1) {
        // tell servo to go to position in variable 'pos'
        servo.write(posMotor);
        delay(15);
      }
      delay(2000);
      for (posMotor = 110; posMotor >= 0; posMotor -= 1) {
        // tell servo to go to position in variable 'pos'
        servo.write(posMotor);
        delay(15);
      }
  }

//Ventoinha
void funcaoVentoinha(){
   if(ventoinhaBoolean){//se tiver ligada -> desliga
              digitalWrite(ventoinha,LOW);
              ventoinhaBoolean = false;
              }else{ // se tiver desligada -> liga
                digitalWrite(ventoinha,HIGH);
                ventoinhaBoolean = true;
                }
            
  }

//temperatura

void funcaoTemperatura(){
  
valT = analogRead(tempPin);
float mv = ( valT/1024.0)*5000; 
cel = mv/10;
float farh = (cel*9)/5 + 32;

  }
  

void funcaoNovaTemperatura(){

//função keypad mudar temperatura
  Serial.print("Defina uma nova temperatura.\n");
  boolean mudarTemperatura=true;
  int temperaturaNova = 0;
  int count1=0;
  String novaTemperatura = "";
  while(mudarTemperatura){
    
    char tecla2 = teclado.getKey();
     if (tecla2 != NO_KEY){ 
      
       if(tecla2 == '1' || tecla2 == '2' || tecla2 == '3' || tecla2 == 'A' ||
          tecla2 == '4' || tecla2 == '5' || tecla2 == '6' || tecla2 == 'B' ||
          tecla2 == '7' || tecla2 == '8' || tecla2 == '9' || tecla2 == 'C' ||
          tecla2 == '0' || tecla2 == 'D'){

          novaTemperatura+=tecla2;
          count1++;
              Serial.print(tecla2);
              tone (buzzer,4000);
          delay(40);
          noTone(buzzer);
          
              } 

              if(tecla2 == '*' || count1 > 2 ){
            count1 =0;
              novaTemperatura = "";   
             Serial.println("Temperatura Nova:");
              
            }

            if(tecla2 == '#'){
              count1 = 0;
              temperaturaNova=novaTemperatura.toInt();
              if(temperaturaNova>=15&&temperaturaNova<=30){
              temperaturaUser=temperaturaNova;
              verdeFuncao();
              delay(500);
              mudarTemperatura=false;
               novaTemperatura = ""; 
              }else{
              Serial.println("Só são aceites temperaturas entre 15 e 30 graus.\n");
              novaTemperatura = ""; 
              Serial.println("Temperatura Nova:");
            }
          }
        }
      }
  }

  
 void sensorAgua(){
    aguaLer = false;
    countAgua = 0;
    int count = 0;
    while(count < 35 ){
    int rainSenseReading = analogRead(rainsense);
   Serial.println(rainSenseReading); // mensagem no serial monitor
   count++;
   delay(300);
   if (countAgua >= 8){ 
      Serial.print("Tem Agua");
      digitalWrite(buzzer, HIGH);  //ligar o buzzer
      vermelhoFuncao(); // ligar led vermelho
      aguaLer = true;
      
   }
   if (rainSenseReading <300){ 
      countAgua++; // aumentar o valor do count
   }
   else if (rainSenseReading >300) { // se nao tiver a chover
      digitalWrite(buzzer, LOW); // buzzer desligado
      verdeFuncao();//ligar led verde
      countAgua = 0; // dar reset ao count
      aguaLer = false;
   }
    }

   delay (1000);
    }

 //Sensor de Gas MQ2

void funcaoGas(){

valorLidoGas = analogRead(gas); //VALOR LIDO NO PINO ANALÓGICO A2
// leitura de sensor definido como 300;
 if (valorLidoGas > leitura_sensor){//SE VALOR LIDO NO PINO ANALÓGICO FOR MAIOR QUE O VALOR LIMITE, FAZ 
  sensorGas = true;
  tone (buzzer,7000);
 }else{ //SENÃO, FAZ
 sensorGas = false;
 digitalWrite(buzzer, LOW);
 }
  delay(200);
  }
    

void loop(){

if(!passAtiva){
   inserirPass();
  }
  
//ativar alarme -> sensores todos ativos- falta; so funcionar tecla B para desativar sem aparecer no ecra; ficar a aparecer a data e a hora ate o alame ser desativado;

     if(!ativarAlarme && passAtiva){
    if(menu == 0){
    Serial.print("\n*****MENU*****");  
    Serial.print("Ativar alarme: tecla A\n"); // tecla A
    Serial.print("Desativar alarme: tecla B\n"); // tecla B
    Serial.print("Mudar password: tecla C\n"); // tecla C
    Serial.print("Testar sensores: tecla D\n"); // tecla C
    Serial.print("Abrir porta: tecla 1\n"); //tecla 1
    Serial.print("Ligar Luzes: tecla 2\n");//tecla 2   
    Serial.print("Alterar Temperatura : tecla 3\n");
    Serial.print("janela : tecla 4\n");
    menu=1;  
    }
      char tecla = teclado.getKey();
      if (tecla != NO_KEY){
          switch(tecla){
            case 'A':{ // Ativar Alarme
            if(!UserAtivoAlarme){
              ativarAlarme=true;  
              if(ativarAlarme && !UserAtivoAlarme){
                Serial.println("O alarme foi ativado.\n");
               
                UserAtivoAlarme= true;
              sensorPir = false;
                 sensorGas = false;
                 aguaLer = false;                 

            
            
                funcaoPir();

                sensorAgua();         
                
          funcaoGas();                
                
                //fazer condicoes dos sensores
                if((sensorPir || sensorGas || aguaLer) && UserAtivoAlarme){
                    tone (buzzer,1000);
                  }else{
                    noTone(buzzer);
                    sensorPir = false;
                   sensorGas = false;
                    aguaLer = false;
                    }

                
              }
                ativarAlarme = false;
                
            }else{
              Serial.println("O alarme já está ativo.\n");
              menu=0;
              }
              
              break;
            }
            
            case 'B':{ // Desativar Alarme
              
              if(UserAtivoAlarme){
              ativarAlarme=true;
              alarmeAtivado=true;
                if(alarmeAtivado){
                inserirPass();
                }
                if(UserAtivoAlarme){
                Serial.println("O alarme foi desativado.\n");
                  noTone(buzzer); //fazer com que o alarme apenas se desligue apos a password correta ser introduzida.
                  UserAtivoAlarme= false;
                }
              }else{
              Serial.println("O alarme já está desativado.\n");
                menu=0;
              }
                           
              break;
            }
            
            case 'C':{ // Mudar pass
              if(!UserAtivoAlarme){
               introduzirPassAntiga();
              }
              break;
              
            }
            
            case 'D':{ // Testar Alarmes
            
            boolean menuAnterior=false;

        while(!menuAnterior){

            
          if(menuSensores){
              Serial.print("\n***** MENU *****\n");
              Serial.print("*Testar Pir* : A\n");
              Serial.print("*Testar Temperatura* : B\n");
              Serial.print("*Testar Gas* : C\n");
              Serial.print("*Testar Porta* : D\n");
              Serial.print("*Testar SensorAgua* : 1\n");
              Serial.print("*testar Janelas* : 3\n");              
              Serial.print("Testar ventoinha : 5\n");              
              Serial.print("*Voltar ao menu anterior* :*\n");
              menuSensores=false;
            }

            
             char tecla = teclado.getKey();
      if (tecla != NO_KEY){
          switch(tecla){
          menuSensores=true;
          case 'A':{//testar pir
            funcaoPir();
            menuSensores=true;
            break;
          }
          case 'B':{//testar temperatura
            funcaoTemperatura();
            Serial.print(cel);
            menuSensores=true;
            break;
          }
          case 'C':{//testar gas
            funcaoGas();
            Serial.print(valorLidoGas);
            menuSensores=true;
            break;
          }
          case 'D':{//testar porta
         
              motor = 21;
              funcaoMotor();
             
            menuSensores=true;
            break;
          }
          case '1':{//testar sensorAgua
                
              sensorAgua();
              
            menuSensores=true;
            break;
          }
          case '*':{//voltar ao menu anterior
            menuAnterior=true;
            menu=0;
            break;
          }                    
          case '3':{ // Abrir as janelas
            
              motor = 22;
              funcaoMotor();
            
            menuSensores=true;
            break;
          }
          case '5':{
                       
           funcaoVentoinha();
            
            menuSensores=true;
            break;
            }         
          }
        } 
     }
       
       break;
   }
   case '1':{
        
         if(!portaAberta){
          motor = 21;
  funcaoMotor();
    portaAberta=true;
  }
  menu=0;
  break;
        }
        case '2':{
                 
          boolean menuAnteriorA=false;

        while(!menuAnteriorA){
            
          if(menuLuzesLigadas){
              Serial.print("\n***** MENU *****\n");
              Serial.print("*Acender/Desligar Luzes Todas* : A\n");
              Serial.print("*Acender/Desligar Luzes Hall* : B\n");
              Serial.print("*Acender/Desligar Luzes WC* : C\n");
              Serial.print("*Acender/Desligar Luzes Cozinha* : D\n");
              Serial.print("*Acender/Desligar Luzes Sala* : 1\n");              
              Serial.print("*Voltar ao menu anterior* :*\n");
              menuLuzesLigadas=false;
            }

             char tecla = teclado.getKey();
      if (tecla != NO_KEY){
          switch(tecla){
            case 'A':{
          if(luzesHall == LOW || luzesCozinha == LOW || luzesSala == LOW || luzesWC == LOW){//luzes desligadas e queremos ligar
          digitalWrite(luzesHall,HIGH);
          digitalWrite(luzesCozinha,HIGH);
          digitalWrite(luzesSala,HIGH);
          digitalWrite(luzesWC,HIGH);
            }else{//luzes ligadas e pretendemos ligar
          digitalWrite(luzesHall,LOW);
          digitalWrite(luzesCozinha,LOW);
          digitalWrite(luzesSala,LOW);
          digitalWrite(luzesWC,LOW);              
              }
             menuLuzesLigadas=true;
              break;
              }
              case 'B':{//Hall
if(luzesHall == LOW){ // ligar luz do hall
digitalWrite(luzesHall,HIGH);
amareloFuncao();
                }else{ // desligar luz do hall
                  digitalWrite(luzesHall,LOW);
                  desligaRGB();
                  }
                menuLuzesLigadas=true;
                break;
                }
                case 'C':{//WC
                  if(luzesWC == LOW){ // ligar luz do WC
digitalWrite(luzesWC,HIGH);
azulFuncao();
                  }else{
                    digitalWrite(luzesWC,LOW);// Desligar luz do WC
                    desligaRGB();
                    }
                  menuLuzesLigadas=true;
                  break;
                  }
                  case 'D':{//Cozinha
                    if(luzesCozinha == LOW){ // ligar luz da cozinha
digitalWrite(luzesCozinha,HIGH);
vermelhoFuncao();
                  }else{
                    digitalWrite(luzesCozinha,LOW);// Desligar luz da cozinha
                    desligaRGB();
                    };
                    menuLuzesLigadas=true;
                    break;
                    }
                    case '1':{//Sala
if(luzesSala == LOW){ // ligar luz da sala
digitalWrite(luzesSala,HIGH);
verdeFuncao();
                  }else{
                  digitalWrite(luzesSala,LOW);// Desligar luz da sala
                   desligaRGB();
                    }
                      menuLuzesLigadas=true;
                      break;
                      }
                      case '*':{//Menu anterior
                        
                         menuAnteriorA=true;
                         menuLuzesLigadas = true;
                         menu = 0;
                        break;
                        }
                  }
                }
              }
              break;
            }                  
              case '3':{ // Alterar Temperatura
          
            funcaoNovaTemperatura();
            Serial.print(temperaturaUser);
         
            
           menu=0; 
           break;
          }
          case '4':{ // abrir ou fechar a janela
           
          if(!abrirJanela){                   
              motor = 22;
            funcaoMotorAbrir();
            abrirJanela = true;
            }else{                      
              motor = 22;
              funcaoMotorFechar();
              abrirJanela = false;
              }
              menu=0; 
           break;
            }
          
}
      }
     }
}
