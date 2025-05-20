/*Ensemble des librairies*/
#include <SoftwareSerial.h>    // Communication avec le Blutooth
#include <SPI.h>               // Communication avec le Potentiomètre digital
#include <Adafruit_SSD1306.h>  // Communication avec l'écran OLED

/*Ensemble des pins*/
#define CLK_pin 2              // Pin pour l'Encodeur rotatoire  
#define DT_pin 4               // Pin pour l'Encodeur rotatoire 
#define SW_pin 5               // Pin pour l'Encodeur rotatoire 
#define Rx_pin 6               // Pin pour le Bluetooth 
#define Tx_pin 7               // Pin pour le Bluetooth 
#define CS_pin 10              // Pin pour le Potentiomètre digital
#define SI_pin 11              // Pin pour le Potentiomètre digital
#define SCK_pin 13             // Pin pour le Potentiomètre digital
#define AmpOut_pin A0          // Pin pour le Capteur 
#define FlexOut_pin A1         // Pin pour le Flex Sensor
#define SDA_pin A4             // Pin pour l'écran OLED
#define SCL_pin A5             // Pin pour l'écran OLED

/*OLED*/
#define nombreDePixelsEnLargeur 128                 
#define nombreDePixelsEnHauteur 64       
#define brocheResetOLED         -1          
#define adresseI2CecranOLED     0x3C        
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

/*Bluetooth*/
SoftwareSerial mySerial(Rx_pin,Tx_pin); // mySerial est l'objet associé au Bluetooth
int Message = 0;                        // Message récupère le byte envoyé par l'appli Android

/*Potentiomètre Digital*/
#define MCP_NOP 0b00000000
#define MCP_WRITE 0b00010001            // Commande pour écrire un byte sur le Potentiomètre digital
#define MCP_SHTDWN 0b00100001

/*Flex Sensor*/
const float VCC = 5.0;      
const float RDiv = 33000.0 ;            // Résistance pour le pont diviseur sur le Flex Sensor
float Rflex;                            // Résistance calculée
float Vflex;                            // Tension mesurée aux bornes du Flex
int Dataflex;                           // Tension mesurée aux bornes du Flex

/*Capteur*/
float VAmpRef = 2.5;
float VAmp;
float RAmp;
int DataVamp;
byte zero = 0;
byte un = 1;
byte max = 255;

/*Encodeur*/
int volatile Choix = 0;               // Action affichée en surbrillance sur l'OLED
int volatile Choix_en_cours = 0;      // Action effectuée
unsigned long LastDebounceTime = 0;   
unsigned long debounceDelay = 500;    // Délai entre chaque rotation

/*Temporisation*/
unsigned long temps_courant;   
unsigned long temps_precedent = 0;
const long intervalle = 500;

/*Baudrate*/
#define baudrate 9600

/*Setup*/
void setup() {
  Setup_Flex();
  Setup_Ampli();
  Setup_Bluetooth();
  Setup_OLED();
  Setup_Encodeur();
  Setup_Pot();
  mySerial.begin(baudrate);
  Serial.begin(baudrate);
}

/*Ensemble des fonctions Setup*/
void Setup_Flex() {
  pinMode(FlexOut_pin,INPUT);
}

void Setup_Pot() {
  pinMode (CS_pin, OUTPUT);
  digitalWrite(CS_pin, HIGH); 
}

void Setup_Ampli() {
  pinMode(AmpOut_pin,INPUT);
}

void Setup_OLED() {
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(2);                  
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE);
  ecranOLED.println("Acquisi-");
  ecranOLED.println("-tion");
  ecranOLED.println("interrom-");
  ecranOLED.println("-pue");
  ecranOLED.display();
  delay(1000);
}

void Setup_Encodeur() {
  pinMode(CLK_pin, INPUT_PULLUP);     
  pinMode(DT_pin, INPUT_PULLUP);       
  pinMode(SW_pin, INPUT_PULLUP);
  attachInterrupt(0, Encodeur, RISING); 
}

void Setup_Pot_Dig() {
  pinMode (CS_pin, OUTPUT);
  digitalWrite(CS_pin, HIGH); 
}

void Setup_Bluetooth() {
  pinMode(Rx_pin,INPUT);
  pinMode(Tx_pin,OUTPUT);
}

/*Boucle principale*/
void loop() {
  temps_courant = millis();
  if (temps_courant - temps_precedent >= intervalle) {  
    temps_precedent = temps_courant;
    if (mySerial.available()) {
      Message = mySerial.read();
    }
    switch (Message) {                                                   // Début des mesures
      case 1 : Menu(Choix);
               if (digitalRead(SW_pin) == 0) {Choix_en_cours = Choix;}
               Taches(Choix_en_cours);
               break;
      case 0 : ecranOLED.clearDisplay();                                 // Fin des mesures
               ecranOLED.setTextSize(2);                  
               ecranOLED.setCursor(0, 0);
               ecranOLED.setTextColor(SSD1306_WHITE);
               ecranOLED.println("Acquisi-");
               ecranOLED.println("-tion");
               ecranOLED.println("interrom-");
               ecranOLED.println("-pue");
               ecranOLED.display();
               Choix_en_cours = 0;
               break;
    }
  }
}

/*Fonction Calibration*/
void Calib() {
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(2);                  
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE);
  ecranOLED.println("Calibra-");
  ecranOLED.println("-tion");
  ecranOLED.println("en");
  ecranOLED.println("cours...");
  ecranOLED.display();
  VAmp = analogRead(AmpOut_pin) * VCC / 1023.0;
  int pot_dig = 127;
  SPIWrite(MCP_WRITE,pot_dig,CS_pin);
  delay(2000);
  if (VAmp > VAmpRef) { 
    while ((VAmp > VAmpRef) && (pot_dig < 255)) { //On augmente la résistance R2 si on mesure une trop grande tension
      pot_dig++;
      SPIWrite(MCP_WRITE,pot_dig,CS_pin);
      delay(100);
      Serial.println(pot_dig);
      VAmp = analogRead(AmpOut_pin) * VCC / 1023.0;
    }    
  }
  else {
    while ((VAmp < VAmpRef) && (pot_dig > 0)) { //On diminue la résistance R2 si on mesure une trop faible tension
      pot_dig--;
      SPIWrite(MCP_WRITE,pot_dig,CS_pin);
      delay(100);
      Serial.println(pot_dig);
      VAmp = analogRead(AmpOut_pin) * VCC / 1023.0;
    }
  }
  mySerial.write(pot_dig);
}

/*Fonction Flex Sensor*/
void MesureFlex() {
    Vflex = analogRead(FlexOut_pin) * VCC / 1023.0;
    Rflex = RDiv * (VCC / Vflex - 1.0);
    Dataflex = map(Rflex,27000.0,100000.0,0,255);
    mySerial.write(Dataflex);
}

/*Fonction Capteur*/
void MesureAmp() {
    VAmp = analogRead(AmpOut_pin) * VCC / 1023.0;
    DataVamp = (VAmp*255.0)/VCC;
    DataVamp = (int) DataVamp;  
    if ((DataVamp > 0) && (256 > DataVamp)) {mySerial.write(DataVamp);}
    else if (DataVamp == 0)  {mySerial.write(un);} // On renvoie un pour ne pas diviser un nombre par zéro
    else {mySerial.write(max);} 
}

/*Fonction pour le potentiomètre digital*/
void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin) {
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(ssPin, LOW); 
  SPI.transfer(cmd);        
  SPI.transfer(data);       
  digitalWrite(ssPin, HIGH);
  SPI.endTransaction();
}

/*Fonction Encodeur*/
void Encodeur() {
  if (digitalRead(CLK_pin) == HIGH) {
    if (millis() - LastDebounceTime > debounceDelay) {
      if (digitalRead(DT_pin)==HIGH) {Choix++;}
      else if (digitalRead(DT_pin)==LOW) {Choix--;}
      LastDebounceTime = millis();
    }
  }
  if (Choix < 0) {Choix = 3;}
  if (Choix > 3) {Choix = 0;}
}

/*Fonction affichant le Menu sur l'OLED*/
void Menu(int choix) {
    ecranOLED.clearDisplay();
    ecranOLED.setTextSize(2);                  
    ecranOLED.setCursor(0, 0);
    switch (choix) {
      case 0 :  ecranOLED.setTextColor(SSD1306_BLACK,SSD1306_WHITE); 
                ecranOLED.println("Pause");
                ecranOLED.setTextColor(SSD1306_WHITE);
                ecranOLED.println("Calibre A");
                ecranOLED.println("Flex");
                ecranOLED.println("Capteur");
                break;
      case 1 :  ecranOLED.setTextColor(SSD1306_WHITE);
                ecranOLED.println("Pause");
                ecranOLED.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
                ecranOLED.println("Calibre A");
                ecranOLED.setTextColor(SSD1306_WHITE);
                ecranOLED.println("Flex");
                ecranOLED.println("Capteur");
                break;
      case 2 :  ecranOLED.setTextColor(SSD1306_WHITE);
                ecranOLED.println("Pause");
                ecranOLED.println("Calibre A");
                ecranOLED.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
                ecranOLED.println("Flex");
                ecranOLED.setTextColor(SSD1306_WHITE);
                ecranOLED.println("Capteur");
                break;
      case 3 :  ecranOLED.setTextColor(SSD1306_WHITE);
                ecranOLED.println("Pause");
                ecranOLED.println("Calibre A");
                ecranOLED.println("Flex");
                ecranOLED.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
                ecranOLED.println("Capteur");
                break;          
    }
    ecranOLED.display();
}

/*Fonction sélectionnant l'action sur le menu*/
void Taches(int action) {
  switch (action) {
    case 0 : break;
    case 1 : Calib();
             Choix_en_cours = 0;
             break;
    case 2 : MesureFlex();
             break;
    case 3 : MesureAmp();
             break;
  }
}
