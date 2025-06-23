
#include <ArduinoBLE.h>
#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "esp_wifi.h"
#include "WiFi.h"
#include <WiFiUdp.h>
#include <EEPROM.h>

#define SLAVE_ADDRESS_1 0x20
#define SLAVE_ADDRESS_2 0x21

//-----------------------------------------------------------------------

// definir tous les constante pour la connecxion wifi
int Envoyer=0;   // variable qui passe à 1 quand on envoie le MDP au PhotoLeds apres renisialisation

const char* initialSSID = "Phototype";    
const char* initialPassword = "12345678";
const char* newSSID = "Phototypeauto";  // SSID pour futures connexions (envoyer au photoleds)
const char* newPassword = "Boxlight_3000";  // Mot de passe pour futures connexions  ( numero de serie de la boxlight)
const char* IP_recept = "192.168.4.1";    // IP du ESP32-C3 du PhotoLeds
char savedPassword[32];    // pour stocker le MDP envoyé
char packetBuffer[255];
String receivedPhototype = "";   // variable pour mettre le phototype reçu par le photoleds
String Vphotoconnect="";    // variable qui indique l'etat de connexion avec le photoleds
String Vinitphotoconnect ="";   // variable qui est comparé âr Vphotoconnect a chaque fois si elles ont egaux on n'envoie rien a la tablette pour ne pas la saturé

String receivedCommande = "";  // variable reçu du photoleds qui indique que le photoleds va etre renisialisé
String receivedeteindre= "";

const int udpPort = 4210;
const int udpPort1 = 4211;
const int dpPort2 =4212;
const int dpPort3 =4213;

WiFiUDP udp, udp1, udp2, udp3;


static bool apStarted = false;
static unsigned long lastClientCheck = 0;
static bool boxlightWasConnected = false;





// define ledc function as pwm softaware
const int PWM_CHANNEL = 0;    // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 175;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 
const int DUTY_CYCLE = 50; // Duty cycle depends of irradiance needed


const int thermistorPin = A0;
const float seriesResistor = 10000;
const float nominalResistance = 10000;
const float nominalTemperature = 25;
const float bCoefficient = 3950;
const int adcMaxValue = 4095;
//-----------------------------------------------------------------------
const unsigned long interval = 120000; // 2 minutes in milliseconds
unsigned long previousMillis = 0;

#define ledIR D6
#define ledrouge D1
#define ledbleu D2
#define ledjaune D3

#define ledvisrouge D9
#define ledconnect D10

//#define Switchoff D4  // pour les tests (appairrage)

//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs, UUID BLE & UUID commandes
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_BOXLIGHT               "10003000-5EC4-4083-81CD-A10B8D5CF6EC"
#define BLE_UUID_COMMANDES              "10003001-5EC4-4083-81CD-A10B8D5CF6EC"

//----------------------------------------------------------------------------------------------------------------------
// BLE
//----------------------------------------------------------------------------------------------------------------------

BLEService BOXLIGHTService(BLE_UUID_BOXLIGHT);
BLEStringCharacteristic commandesCharacteristic(BLE_UUID_COMMANDES, BLERead | BLEWrite | BLENotify, 30);

String fileName = "";
byte I2C_byte;
int RESET;
const float temperatureThreshold = 55.0; // Threshold temperature in Celsius

void setup() {
  // put your setup code here, to run once:
  // la partie pour verifier si on a sauvegarder le mot de passe envoyer ou non
  Serial.begin(115200);
  delay(1000);

 // Serial.begin(9600);
  // begin I2C communication
  Wire.begin();

  // Sets up a channel (0-15), a PWM duty cycle frequency, and a PWM resolution (1 - 16 bits) 
  // ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits);
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(ledIR, PWM_CHANNEL);

  // configure LEDs as output
  pinMode(ledrouge, OUTPUT);
  //pinMode(ledIR,OUTPUT);
  pinMode(ledbleu, OUTPUT);
  pinMode(ledjaune, OUTPUT);
  pinMode(ledvisrouge, OUTPUT);
  pinMode(ledconnect, OUTPUT);

  //pinMode(Switchoff,INPUT_PULLUP); // switch pour les tests (pour appairrage)

  // set all LEDs as 0
  ledcWrite(PWM_CHANNEL, 0);
  digitalWrite(ledrouge, LOW);
  //digitalWrite(ledIR,LOW);
  digitalWrite(ledbleu, LOW);
  digitalWrite(ledjaune, LOW);
  digitalWrite(ledvisrouge, LOW);
  digitalWrite(ledconnect, LOW);

  BLE.begin();
  
  // set advertised local name and service UUID:
  BLE.setDeviceName("BOXLIGHT3000");
  BLE.setLocalName("BOXLIGHT3000");
  BLE.setAdvertisedService(BOXLIGHTService);


  // BLE add characteristics
  BOXLIGHTService.addCharacteristic(commandesCharacteristic);

  // add service
  BLE.addService(BOXLIGHTService);

  // set the initial value for the characteristic:
  commandesCharacteristic.writeValue(fileName);

  // start advertising
  BLE.advertise();

  EEPROM.begin(512); 

  /*memset(savedPassword,0, sizeof(savedPassword));
  EEPROM.put(0, savedPassword);
  EEPROM.commit();*/

  // verifier la memoire
  EEPROM.get(0,savedPassword);
  Serial.println("mot de passe stocké");
  Serial.println(savedPassword);
  
  // si la memoir est pleinne "mode AP", on attend 10s si soit le dispositif se connecte avec le photoleds pour la 1ere fois 
  if (savedPassword[0]!='\0' ){
    WiFi.softAP("Phototypeauto",newPassword);     // mettre le master au mode AP
    Serial.println("Boxlight en mode AP");

    unsigned long startAttempt = millis();    // variable qui permet de savoir le temps de debut du mode AP
    bool connected = false;
    // attendre 10s pour la connexion du dispositif
    while (millis() - startAttempt < 10000) {
      int stationCount = WiFi.softAPgetStationNum();
      if (stationCount > 0) {
        connected = true;
        break;
      }
      delay(500);
      Serial.print(".");
    }
    
    if (connected) {
      Vphotoconnect = "CON";
      Serial.println("Boxlight connecté.");
    } else {
      Vphotoconnect = "NCO";
      Serial.println("Boxlight non connecté.");
    }
  }
   else {
    // si la memoire est vide "mode sta" Vphotoconnect= NPA (jamais appairé)
    WiFi.begin("Phototype", "12345678");      // mettre le  master au mode station
    Vphotoconnect="NPA";
    Serial.println("Boxlight jamais appairé");
    WiFi.disconnect(); 
    delay(1000);
  }

  udp.begin(udpPort);
  udp1.begin(udpPort1);
  udp2.begin(dpPort2);


  
}

void resetAllLEDs() {

  I2C_byte = 4; // 4 = stop
  Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
  Wire.write(I2C_byte);
  Serial.println(I2C_byte);
  Wire.endTransmission();
  Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
  Wire.write(I2C_byte);
  Wire.endTransmission();

  digitalWrite(ledrouge, LOW);  // Éteint la LED rouge
  digitalWrite(ledbleu, LOW);   // Éteint la LED bleue
  digitalWrite(ledjaune, LOW);  // Éteint la LED jaune
  ledcWrite(PWM_CHANNEL, 0);    // Met à zéro le signal PWM pour l'IR LED (ledIR)

}

void measureTemperature() {
  int adcValue = analogRead(thermistorPin);
  float voltage = adcValue / float(adcMaxValue) * 3.3;
  float thermistorResistance = (seriesResistor * (3.3 / voltage - 1));
  float steinhart = log(thermistorResistance / nominalResistance) / bCoefficient + 1.0 / (nominalTemperature + 273.15);
  int temperatureC = 1.0 / steinhart - 273.15;

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  if (temperatureC >= temperatureThreshold) {
    Serial.println("Temperature threshold exceeded, turning off all LEDs.");
    resetAllLEDs();
  }
}

void loop() {

  digitalWrite(ledvisrouge, HIGH);
  BLEDevice central = BLE.central();

 
  if (central) {     // si Vphotoconnect= JA ou =NC
  
    measureTemperature();
    // set all LEDs as 0
    ledcWrite(PWM_CHANNEL, 0);
    digitalWrite(ledrouge, LOW);
    digitalWrite(ledbleu, LOW);
    digitalWrite(ledjaune, LOW);



    Serial.print("Connected to central: ");
    Serial.println(central.address()); // Display mac adress BLE

    digitalWrite(ledconnect, HIGH);




    while (central.connected()) {
       // verifier si la variable Vphotoconnect est differente du
      if (Vinitphotoconnect!= Vphotoconnect){
        Vinitphotoconnect=Vphotoconnect;
        Serial.println("envoyer a l'IHM");
        commandesCharacteristic.writeValue(Vphotoconnect);
        Serial.println(Vphotoconnect);
      }  

        
      // si le dispositif est connecté  avec la tablette on envoie le phototype a la tablette
      if (Vphotoconnect=="CON"){

        int packetSize1= udp1.parsePacket();
        if (packetSize1){    
          int len1 = udp1.read(packetBuffer, 255);
          if (len1>0){ 
            packetBuffer[len1]='\0';
          }
          receivedPhototype= String(packetBuffer);
          Serial.println("phototype reçu");
          Serial.println(receivedPhototype);}  // phototype reçu
          commandesCharacteristic.writeValue(receivedPhototype);

          // RECEVOIR UNE COMMANDE DE PHOTOLEDS QUAND ON APPUIE SUR LE RESET pour que le dispositif passe au mode manuelle
          int packetSize2= udp2.parsePacket();
          if (packetSize2){    
            int len2 = udp2.read(packetBuffer, 255);
            if (len2>0){ 
              packetBuffer[len2]='\0';
            }
            receivedCommande= String(packetBuffer);
            Serial.println("commande reçu");
            Serial.println(receivedCommande);}  // commande reçu
            //Serial.println(receivedCommande);
            if (receivedCommande != ""){  // si la variable n'est pas vide on envoie la commande a la tablette 
              commandesCharacteristic.writeValue(receivedCommande);
              Vphotoconnect="NCO";
              receivedCommande = "";
            }
            // Recevoir une commande que le photoleds est eteind pour une le dispositif se cotinue au mode manuelle 
            /*int packetSize3= udp3.parsePacket();// ajouter maintenant
            if (packetSize3){    
              int len3 = udp3.read(packetBuffer, 255);
              if (len3>0){ 
                packetBuffer[len3]='\0';
              }
            receivedeteindre= String(packetBuffer);
            Serial.println("commande reçu");
            Serial.println(receivedeteindre);
            Vphotoconnect="NCO";} */
        
            } 
        
          
  

      if (commandesCharacteristic.written()) {
        fileName = commandesCharacteristic.value();
        Serial.print("Commande reçue: ");
        Serial.println(fileName);
        RESET = 1;


                
         if (fileName == "rouge") {
          I2C_byte = 1; // 1 = rouge
          Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
          Wire.write(I2C_byte);
          Serial.println(I2C_byte);
          Wire.endTransmission();
          Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
          Wire.write(I2C_byte);
          Wire.endTransmission();


          digitalWrite(ledbleu, LOW);
          digitalWrite(ledjaune, LOW);
          digitalWrite(ledrouge, HIGH);
          ledcWrite(PWM_CHANNEL, DUTY_CYCLE);

          unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    measureTemperature();
  }
        }

        if (fileName == "bleu") {
          I2C_byte = 2; // 2 = bleu
          Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
          Wire.write(I2C_byte);
          Serial.println(I2C_byte);
          Wire.endTransmission();
          Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
          Wire.write(I2C_byte);
          Wire.endTransmission();


          ledcWrite(PWM_CHANNEL, 0);
          digitalWrite(ledrouge, LOW);
          digitalWrite(ledjaune, LOW);
          digitalWrite(ledbleu, HIGH);

            unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    measureTemperature();
  }
        }

        if (fileName == "jaune") {      
          I2C_byte = 3; // 3 = jaune
          Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
          Wire.write(I2C_byte);
          Serial.println(I2C_byte);
          Wire.endTransmission();
          Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
          Wire.write(I2C_byte);
          Wire.endTransmission();


          ledcWrite(PWM_CHANNEL, 0);  
          digitalWrite(ledrouge, LOW);
          digitalWrite(ledbleu, LOW);
          digitalWrite(ledjaune, HIGH);

            unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    measureTemperature();
  }
        }

        if (fileName == "Phototype") {      
         // activer la fonction phototype auto où envoie le numero de photoype a la tablette 
          /*commandesCharacteristic.writeValue(receivedPhototype);         
          Serial.println("phototypeauto: ");
          Serial.println(receivedPhototype);*/
  }
        if (fileName== "Reset") {   //Mode appairrage
          // vider la memoire
          memset(savedPassword,0, sizeof(savedPassword));
          EEPROM.put(0, savedPassword);
          EEPROM.commit();

          //PASSER AU MODE STA
          WiFi.disconnect(); 
          delay(1000);

          WiFi.begin("Phototype", "12345678");
          unsigned long startAttempt = millis();
          bool connected = false;
          while (millis() - startAttempt < 15000) { // Attente max 10 secondes
            if (WiFi.status() == WL_CONNECTED) {
            connected = true;
            break;
          }
            delay(500);
            Serial.print(".");
          }
          if (connected==true){
            Vphotoconnect = "CON";
            Serial.println("Boxlight connecté apres appairage");
            unsigned long startUdpWait = millis();
            
            // faire une boucle pour envoyer le mdp (pour etre sur que il va etre recu par le photoleds)
            while (millis() - startUdpWait < 10000) { // attendre 10 secondes max
              udp.beginPacket(IP_recept, udpPort);  // Envoi au ER1 récepteur
              udp.print(newPassword);  // Envoi du mot de passe sous forme de chaîne    
              udp.endPacket();
              Serial.println("Mot de passe envoyé");
              Serial.print(newPassword);
            } 
            // sauvegarder le mot de passe envoyer
            strncpy(savedPassword, newPassword, sizeof(savedPassword));  // Copier le nouveau mot de passe 
            EEPROM.put(0, savedPassword);
            EEPROM.commit();
            delay(500);
          }
          else{
            Vphotoconnect = "NCO";
            Serial.println("Boxlight non connecté apres appairage");
          }

          // PASSER AU MODE AP
          WiFi.disconnect(); 
          delay(1000);
          WiFi.softAP("Phototypeauto",newPassword);     // mettre le master au mode AP
          //delay(1000);
          Serial.println("je suis dans la 2eme partie ");

          unsigned long startAttempt1 = millis();    // variable qui permet de savoir le temps de debut du mode AP apres la renisialisation
          bool connected1 = false;
          // attendre 10s pour la connexion du dispositif
          while (millis() - startAttempt1 < 15000) {
            int stationCount1 = WiFi.softAPgetStationNum();
            if (stationCount1 > 0) {
            connected1 = true;
            break;
            }
            delay(500);
            Serial.print(".");
          } 
    
          if (connected1) {
            Vphotoconnect = "CON";
            Serial.println("Boxlight connecté.");
          } else {
            Vphotoconnect = "NCO";
            Serial.println("Boxlight non connecté.");
            commandesCharacteristic.writeValue("Phototype MANU");    // ajouter maintenant
          }
  }
          


        }
        

        if (fileName == "stop") {
          I2C_byte = 4; // 4 = stop
          Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
          Wire.write(I2C_byte);
          Serial.println(I2C_byte);
          Wire.endTransmission();
          Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
          Wire.write(I2C_byte);
          Wire.endTransmission();


          ledcWrite(PWM_CHANNEL, 0);
          digitalWrite(ledrouge, LOW);
          digitalWrite(ledbleu, LOW);
          digitalWrite(ledjaune, LOW);

        unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    measureTemperature();
  }
        }
      }
    

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());

    ledcWrite(PWM_CHANNEL, 0);
    digitalWrite(ledrouge, LOW);
    digitalWrite(ledbleu, LOW);
    digitalWrite(ledjaune, LOW);

    if (RESET == 1) {
      RESET = 0;
      I2C_byte = 4; // 4 = stop
      Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
          Wire.write(I2C_byte);
          Serial.println(I2C_byte);
          Wire.endTransmission();
          Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
          Wire.write(I2C_byte);
          Wire.endTransmission();


      delay(400);
    }
  }
  

  if (RESET == 2) {
    RESET = 0;
    Wire.beginTransmission(SLAVE_ADDRESS_1);                                               
          Wire.write(I2C_byte);
          Serial.println(I2C_byte);
          Wire.endTransmission();
          Wire.beginTransmission(SLAVE_ADDRESS_2);                                               
          Wire.write(I2C_byte);
          Wire.endTransmission();


    delay(400);
  }


  // blink ledconnect if not connected
  digitalWrite(ledconnect, LOW);
  delay(700);
  digitalWrite(ledconnect, HIGH);
  delay(50);
  digitalWrite(ledconnect, LOW);
}






