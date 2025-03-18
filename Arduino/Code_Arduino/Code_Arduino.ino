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
#define SI_pin 11 
#define SCK_pin 13 
#define AmpOut_pin A0
#define FlexOut_pin A1
#define SDA_pin A4 
#define SCL_pin A5         

#define baudrate 9600
SoftwareSerial mySerial(Rx_pin,Tx_pin); //D�finition du software serial

const float VCC = 5.0;      // voltage at Ardunio 5V line
const float RDiv = 25000.0 ;
float Rflex;
float Vflex;
int valeur_pota;
float VAmp;
int volatile encodeur = 0;
bool enco_plus;
bool enco_moins;
unsigned long temps_courant;   
unsigned long temps_precedent_Flex = 0;
const long intervalle_Flex = 500;
unsigned long temps_precedent_Amp = 0;
const long intervalle_Amp = 500   


void setup() {
  Setup_Bluetooth();
  Setup_Flex();
  Setup_Pota_Dig();
  Setup_Amp();

  mySerial.begin(baudrate);
  Serial.begin(baudrate);
}

void loop() {
  Configuration();
  Menu(choix);
}

void Setup_Bluetooth() {
  pinMode(Rx_pin,INPUT);
  pinMode(Tx_pin,OUTPUT);
}

void Setup_Pota_Dig() {
  pinMode (CS_pin, OUTPUT); //select pin output
  digitalWrite(CS_pin, HIGH); //SPI chip disabled
  SPI.begin(); 
}

void Setup_Ampli() {
  pinMode(AmpOut_pin,INPUT);
}

void Gestion_Pota_Dig() {
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  digitalWrite(CS_pin, LOW); // SS pin low to select chip
  SPI.transfer(MCP_WRITE);        // Send command code
  SPI.transfer(valeur_pota);       // Send associated value
  digitalWrite(CS_pin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction();
}


void Setup_Flex() {
  pinMode(FlexOut_pin,INPUT);
}

void Setup_Encodeur() {
  pinMode(CLK_pin,INPUT)
  digitalWrite(CLK_pin,HIGH)
  pinMode(DT_pin,INPUT)
  pinMode(DT_pin,HIGH)
  attachInterrupt(0, Gestion_Encodeur, RISING);
}

void Gestion_Pota_Dig() {
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  digitalWrite(CS_pin, LOW); // SS pin low to select chip
  SPI.transfer(MCP_WRITE);        // Send command code
  SPI.transfer(valeur_pota);       // Send associated value
  digitalWrite(CS_pin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction();
}

void Gestion_Encodeur() {
  if (digitalRead(CLK_pin)==HIGH && digitalRead(DT_pin)==HIGH) {
    encodeur++;
  } else if (digitalRead(CLK_pin))==HIGH && digitalRead(DT_pin)==LOW) {
    encodeur--;
  }
}

void MesureFlex() {
  temps_courant = millis();
  if (temps_courant - temps_precedent_Flex >= intervalle_Flex) {  
    temps_precedent_Flex = temps_courant;
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);
    ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
    ecranOLED.println(F("Mesure Flex Sensor:"));
    ecranOLED.display();
    Vflex = analogRead(FlexOut_pin) * VCC / 1023.0;
    Rflex = RDiv * (VCC / Vflex - 1.0);
    ecranOLED.println(Rflex);
    Serial.println(Rflex);
    mySerial.wrtite(Rflex);
  }
}

void MesureAmp() {
  temps_courant = millis();
  if (temps_courant - temps_precedent_Amp >= intervalle_Amp) {  
    temps_precedent_Amp = temps_courant;
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);
    ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
    ecranOLED.println(F("Mesure Ampli :"));
    ecranOLED.display();
    VAmp = analogRead(AmpOut_pin) * VCC / 1023.0;
    
    ecranOLED.println(VAmp);
    Serial.println(VAmp);
    mySerial.wrtite(VAmp);
  }
}

void Menu() {
  switch (encodeur) {
    case 0 :
      MesureAmp();
      break;
    case 1 :
      MesureFlex();
      break; 
  }
}

void Configuration() {
  while (mySerial.available()) {
    encodeur = mySerial.read();
  }
  Gestion_Pota_Dig();
  delay(1000);
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
