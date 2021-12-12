//Biblioteca para utilização de funções para protocolo I2C
#include <Wire.h>

//ENDEREÇO DO DISPOSITIVO ESCRAVO
const int endereco_escravo = 10;

//PINO DO BOTÃO
const int botao = 7;

//DADOS ENVIADOS PELO MESTRE AO ESCRAVO NESSA COMUNICAÇÃO
const char botao_pressionado_p = '.';
const char botao_pressionado_m = '-';


#define LED_PIN 4
#define CODE_BUTTON 3

bool codeButtonArmed;
bool codeButtonPressed;
unsigned long codeTime;
unsigned long startTime;
unsigned long lastButtonPressTime;
bool letterDecoded;
bool newWord;

// Time range of a dot in milliseconds
const unsigned int dotTimeMillisMin = 40;
const unsigned int dotTimeMillisMax = 180;

// Array to store the times of the code button presses
unsigned long buttonPressTimes[5];
int bptIndex;

int row;
int col;

void setup() {
  
  //INICIALIZANDO BARRAMENTO COMO MESTRE (sem argumento) 
  Wire.begin();
  
  pinMode(CODE_BUTTON, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  // Use full 256 char 'Code Page 437' font
  Serial.println("Iniciando Codigo Morse");
  delay(100);
}

void loop() {
  scanButtons();
  if( millis() - lastButtonPressTime > 1600 && newWord == true ) {
    Serial.print(" ");
    newWord = false;
  }else if( millis() - lastButtonPressTime > 600 && letterDecoded == false) {
   	
    Serial.print("DECODE LETTER: ");
  	for(int i=0; i<bptIndex; i++) {
    	if( isDot(buttonPressTimes[i]) ){
  
      		Serial.print(".");
        }
      
      	else if( isDash(buttonPressTimes[i]) ){
          
      		Serial.print("-");
  			}
    
  	Serial.print("   ");
  	char c = decodeMsg();
  	Serial.print(c);
  	Serial.println();

  	bptIndex = 0;
  	for(int i=0; i<5; i++) {
    	buttonPressTimes[i] = 0;
  	}
    letterDecoded = true;
  }
}
}

void codeButtonDown() {
  digitalWrite(LED_PIN, HIGH);
  codeTime = millis() - startTime;
}

void codeButtonReleased() {
  digitalWrite(LED_PIN, LOW);

  // Most button bounces take less than 25 millis. If the code time
  // is greater than 25 millis then it was probably a legit button press
  if( codeTime > 25 ) {
    

    // Save codeTime
    buttonPressTimes[bptIndex] = codeTime;
    bptIndex++;
    if( bptIndex == 5 ) {
      
      for(int i=0; i<5; i++) {
        buttonPressTimes[bptIndex] = 0;
      }
    }
  }
}

void scanButtons() {
  if( ! codeButtonArmed && digitalRead(CODE_BUTTON) == LOW ) {
    codeButtonArmed = true;
    // start timer
    startTime = millis();
    lastButtonPressTime = startTime;
    codeTime = 0;
    letterDecoded = false;
    newWord = true;
  }
  if( digitalRead(CODE_BUTTON) == LOW ) {
    codeButtonPressed = true;
    codeButtonDown();
  }
  if( codeButtonPressed && digitalRead(CODE_BUTTON) == HIGH ) {
    codeButtonPressed = false;
    codeButtonReleased();
    codeButtonArmed = false;
  }
}

bool isDot(unsigned long t) {
  if( t >= dotTimeMillisMin && t <= dotTimeMillisMax )
    return true;
  return false;
}
bool isDash(unsigned long t) {
  if( t > dotTimeMillisMax )
    return true;
  return false;
}

void drawChar(char c) {
  Serial.print(c);
}

char decodeMsg() {
  char c = '?';
  if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && buttonPressTimes[2] == 0 )
    c = 'A';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'B';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'C';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'D';
  else if( isDot(buttonPressTimes[0]) && buttonPressTimes[1] == 0 )
    c = 'E';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'F';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'G';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'H';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && buttonPressTimes[2]== 0 )
    c = 'I';
  else if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'J';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'K';
  else if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'L';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && buttonPressTimes[2]== 0 )
    c = 'M';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && buttonPressTimes[2]== 0 )
    c = 'N';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'O';
  else if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'P';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'Q';
  else if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'R';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'S';
  else if( isDash(buttonPressTimes[0]) && buttonPressTimes[1]== 0 )
    c = 'T';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'U';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'V';
  else if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && buttonPressTimes[3]== 0 )
    c = 'W';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'X';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'Y';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && buttonPressTimes[4]== 0 )
    c = 'Z';
  else if( isDot(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && isDash(buttonPressTimes[4]))
    c = '1';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && isDash(buttonPressTimes[4]))
    c = '2';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && isDash(buttonPressTimes[4]))
    c = '3';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && isDash(buttonPressTimes[4]))
    c = '4';
  else if( isDot(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && isDot(buttonPressTimes[4]))
    c = '5';
  else if( isDash(buttonPressTimes[0]) && isDot(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && isDot(buttonPressTimes[4]))
    c = '6';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDot(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && isDot(buttonPressTimes[4]))
    c = '7';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDot(buttonPressTimes[3]) && isDot(buttonPressTimes[4]))
    c = '8';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && isDot(buttonPressTimes[4]))
    c = '9';
  else if( isDash(buttonPressTimes[0]) && isDash(buttonPressTimes[1]) && isDash(buttonPressTimes[2]) && isDash(buttonPressTimes[3]) && isDash(buttonPressTimes[4]))
    c = '0';
  return c;
}