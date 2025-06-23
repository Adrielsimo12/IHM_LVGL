#include <Arduino.h>
#include <ArduinoBLE.h>

// Broches
#define ROUGE  D0     
#define BLEU   D3
#define IR     D1 
#define JAUNE  D2     
#define VERT   D8
#define BUZZER_PIN D10

#define UART_RX_PIN  D7
#define UART_TX_PIN  D6
#define BAUD_RATE 115200

#define PWM_FREQUENCY 175
#define DUTY_CYCLE 50

bool ir_active = false;
String current_command = "";
bool soin_en_cours = false;
bool maitre_actif = false;

BLEDevice central;
#define BLE_UUID_FACELEDS              "10004000-5EC4-4083-81CD-A10B8D5CF6EC"
#define BLE_UUID_COMMANDES             "10004001-5EC4-4083-81CD-A10B8D5CF6EC"

BLEService faceledsService(BLE_UUID_FACELEDS);
BLEStringCharacteristic commandesCharacteristic(BLE_UUID_COMMANDES, BLERead | BLEWrite, 30);

String fileName = "";
bool ble_connecte = false;
bool message_envoye = false;

// PWM IR
void pulseIR() {
  unsigned long period = 1000000UL / PWM_FREQUENCY;
  unsigned long highTime = period * DUTY_CYCLE / 100;
  unsigned long lowTime = period - highTime;
  digitalWrite(IR, HIGH); delayMicroseconds(highTime);
  digitalWrite(IR, LOW);  delayMicroseconds(lowTime);
}

void lancerBLE() {
  BLE.begin();
  BLE.setDeviceName("Faceleds4000");
  BLE.setLocalName("Faceleds4000");
  BLE.setAdvertisedService(faceledsService);
  faceledsService.addCharacteristic(commandesCharacteristic);
  BLE.addService(faceledsService);
  commandesCharacteristic.writeValue(fileName);
  BLE.advertise();
  Serial.println("BLE lancé : mode autonome");
}

void setup() {
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ROUGE, OUTPUT); digitalWrite(ROUGE, LOW);
  pinMode(BLEU, OUTPUT);  digitalWrite(BLEU, LOW);
  pinMode(IR, OUTPUT);    digitalWrite(IR, LOW);
  pinMode(JAUNE, OUTPUT); digitalWrite(JAUNE, LOW);
  pinMode(VERT, OUTPUT);  digitalWrite(VERT, LOW);

  // Attente UART pour détecter le maître actif
  unsigned long debut = millis();
  while (millis() - debut < 3000) {
    if (Serial1.available()) {
      String msg = Serial1.readStringUntil('\n');
      msg.trim();
      if (msg == "MAITRE_ACTIVE") {
        maitre_actif = true;
        Serial.println("Maître détecté via UART");
        break;
      }
    }
  }

  if (!maitre_actif) {
    Serial.println("Maître non détecté -> BLE activé");
    lancerBLE();
  }
}

String lastCommande = "";

void Ble_Commande() {
  if (soin_en_cours) {
    if (ble_connecte && central.connected()) {
      central.disconnect();
    }
    ble_connecte = false;
    return;
  }

  if (!ble_connecte) {
    BLEDevice detected = BLE.central();
    if (detected) {
      central = detected;
      ble_connecte = true;
      lastCommande = "";
    }
  }

  while (ble_connecte && central.connected()) {
    if (commandesCharacteristic.written()) {
      String fileName = commandesCharacteristic.value();
      fileName.trim();

      if (fileName != lastCommande) {
        Serial.print("Commande BLE reçue: ");
        Serial.println(fileName);
        lastCommande = fileName;

        if (fileName == "rouge") {
          digitalWrite(ROUGE, HIGH);
          digitalWrite(BLEU, LOW);
          digitalWrite(IR, LOW);
          ir_active = true;
        } else if (fileName == "bleu") {
          digitalWrite(ROUGE, LOW);
          digitalWrite(BLEU, HIGH);
          digitalWrite(IR, LOW);
          ir_active = false;
        } else if (fileName == "stop") {
          digitalWrite(ROUGE, LOW);
          digitalWrite(BLEU, LOW);
          digitalWrite(IR, LOW);
          ir_active = false;
        }
      }
    }
  }

  if (ble_connecte && !central.connected()) {
    Serial.println("Déconnexion BLE");
    ble_connecte = false;
    message_envoye = false;
    lastCommande = "";

    digitalWrite(ROUGE, LOW);
    digitalWrite(BLEU, LOW);
    digitalWrite(IR, LOW);
    digitalWrite(JAUNE, LOW);
    digitalWrite(VERT, LOW);
  }
}

void loop() {
  digitalWrite(ROUGE, LOW); 
  digitalWrite(BLEU, LOW);
  digitalWrite(IR, LOW);
  digitalWrite(JAUNE, LOW);
  digitalWrite(VERT, LOW);

  if (maitre_actif && Serial1.available()) {
    String uartCommande = Serial1.readStringUntil('\n');
    uartCommande.trim();
    if (uartCommande.length() > 0) {
      Serial.print("UART (maître) : ");
      Serial.println(uartCommande);
      current_command = uartCommande;
    }
  }

  if (current_command == "RUNNING") {
    soin_en_cours = true;
  } else if (current_command == "IDLE") {
    soin_en_cours = false;
  } else if (current_command == "RED") {
    digitalWrite(ROUGE, HIGH);
    digitalWrite(BLEU, LOW);
    ir_active = true;
  } else if (current_command == "BLUE") {
    digitalWrite(ROUGE, LOW);
    digitalWrite(BLEU, HIGH);
    digitalWrite(IR, LOW);
    ir_active = false;
  } else if (current_command == "STOP") {
    digitalWrite(ROUGE, LOW);
    digitalWrite(BLEU, LOW);
    digitalWrite(IR, LOW);
    ir_active = false;
  }

  if (ir_active && (current_command == "RED")) {
    pulseIR();
  }

  if (!maitre_actif) {
    Ble_Commande();
  }

  delay(5);
}
