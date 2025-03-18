#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

#define MCP_NOP 0b00000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001

#define nombreDePixelsEnLargeur 128       
#define nombreDePixelsEnHauteur 64       
#define brocheResetOLED         -1          
#define adresseI2CecranOLED     0x3C        

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

#define CLK_pin 2  
#define DT_pin 4  
#define SW_pin 5 
#define Tx_pin 6
#define Rx_pin 7  
#define CS_pin 10  
#define SI_pin 11 ?
#define SCK_pin 13 ?
#define AmpOut_pin A0
#define FlexOut_pin A1
#define SDA_pin A4 ?
#define SCL_pin A5 ?        

#define baudrate 9600
SoftwareSerial mySerial(Rx_pin,Tx_pin); //D�finition du software serial

const float VCC = 5.0;      // voltage at Ardunio 5V line
const float RDiv = 25000.0 ;
float Rflex;
float Vflex;
int 
float VAmp;
int volatile encodeur = 0;
bool enco_plus;
bool enco_moins;

const int typeFlex 0;
const int typeAmp 1;

unsigned long temps_courant;
unsigned long temps_precedent_OLED = 0;
const long intervalle_OLED = 2000;   
unsigned long temps_precedent_Flex = 0;
const long intervalle_Flex = 500;
unsigned long temps_precedent_Blue = 0;
const long intervalle_Blue = 3;   


void setup() {
  Setup_Bluetooth();
  Setup_Flex();
  Setup_Pota_Dig();
  Setup_Amp();

  mySerial.begin(baudrate);
  Serial.begin(baudrate);
}

void loop() {
  Gesttion_Bluetooth();
  Gestion_Pota_Dig(MPC_WRITE,);
  Gestion_OLED();
  Gestion_Flex();
}

void Setup_Bluetooth() {
  pinMode(Rx_pin,INPUT);
  pinMode(Tx_pin,OUTPUT);
}

void Gestion_Bluetooth(float data, int type) {
  temps_courant = millis();
  if (temps_courant - temps_precedent_Blue > intervalle_Blue) {  
    temps_precedent_Blue = temps_courant;
    switch (type) {
      case 1 : 
        mySerial.write("La résistance du Flex est de ")
        mySerial.write(data)
        mySerial.write(" Ohms")
      case 2 : 
        mySerial.write("La résistance du capteur est de ")
        mySerial.write(data)
        mySerial.write(" Ohms")
    }
  }
}

void Setup_Pota_Dig() {
  pinMode (CS_pin, OUTPUT); //select pin output
  digitalWrite(CS_pin, HIGH); //SPI chip disabled
  SPI.begin(); 
}

void Gestion_Pota_Dig(uint8_t cmd, uint8_t data) {
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  digitalWrite(CS_pin, LOW); // SS pin low to select chip
  SPI.transfer(cmd);        // Send command code
  SPI.transfer(data);       // Send associated value
  digitalWrite(CS_pin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction();
}

void Gestion_OLED(float data, int type) {
  temps_courant = millis();
  if (temps_courant - temps_precedent_OLED >= intervalle_OLED) { 
    temps_precedent_OLED = temps_courant;
    byte tailleDeCaractere = 2;
    if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
        while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation

        ecranOLED.clearDisplay();                                  
        ecranOLED.setTextSize(tailleDeCaractere);                   
        ecranOLED.setCursor(0, 0);
        switch (type) {                                 
          case 0:
            ecranOLED.print("La résistance du Flex est de ");
            ecranOLED.println(data);
            ecran.OLED.print(" Ohms").
            break;          
          case 1: 
            ecranOLED.print("La résistance du capteur est de ");
            ecranOLED.println(data);
            ecran.OLED.print(" Ohms").
            break;        
        } 
      
      
  }    
}

void Setup_Flex() {
  pinMode(FlexOut_pin,INPUT);
}

void Gestion_Flex() {
  temps_courant = millis();
  if (temps_courant - temps_precedent_Flex >= intervalle_Flex) {  
    temps_precedent_Flex = temps_courant;
    Vflex = analogRead(FlexOut_pin) * VCC / 1023.0;
    Rflex = RDiv * (VCC / Vflex - 1.0);
    Gestion_OLED(Rflex,typeFlex);
    Gestion_Bluetooth(Rflex,typeFlex);
  }
}

void Setup_Encodeur() {
  pinMode(CLK_pin,INPUT)
  digitalWrite(CLK_pin,HIGH)
  pinMode(DT_pin,INPUT)
  pinMode(DT_pin,HIGH)
  attachInterrupt(0, Gestion_Encodeur, RISING);
}

void Gestion_Encodeur() {
  if (digitalRead(CLK_pin)==HIGH && digitalRead(DT_pin)==HIGH) {
    encodeur++;
  } else if (digitalRead(CLK_pin))==HIGH && digitalRead(DT_pin)==LOW) {
    encodeur--;
  }
}

void Setup_Ampli() {
  pinMode(AmpOut_pin,INPUT);
}

void Gestion_Ampli() {
  VAmp=VCC*analogRead(AmpOut_pin)/1023.0
}











// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int compteur_filtre; //compteur filtr&eacute;
int compteur_non_filtre; //compteur non filtr&eacute;

// the following variables are long&#039;s because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin (9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you&#039;ve waited
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
 
  if ((millis() - lastDebounceTime) &gt; debounceDelay)
  {
    // whatever the reading is at, it&#039;s been there for longer
    // than the debounce delay, so take it as the actual current state:
    
     if ((reading == 1) &amp;&amp; (reading != buttonState))
     {
       compteur_filtre = compteur_filtre + 1;
     } 
     buttonState = reading;
  }
 
  // set the LED using the state of the button:
  digitalWrite(ledPin, buttonState);

  // save the reading.  Next time through the loop,
  // it&#039;ll be the lastButtonState:
  lastButtonState = reading;
  
  Serial.print(&quot;Compteurs:&quot;);
  Serial.print(compteur_non_filtre);
  Serial.print(&quot;, &quot;);
  Serial.println(compteur_filtre);
