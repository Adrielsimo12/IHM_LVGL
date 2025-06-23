#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>
#include "ESP_Panel_Board_Custom.h"
#include "ESP_Panel_Board_Custom_conf.h"
#include "ESP_Panel_Conf.h"
#include <esp_display_panel.hpp>
#include "esp_log.h"
#include <lvgl.h>
#include "lvgl_v8_port.h"
#include "ui.h"
#include <ArduinoBLE.h>

#define BUZZER_PIN 6
#define UART Serial  // Redefine serial port name for RS485/UART

// BLE
bool ble_connecte = false;
#define BLE_UUID_FACELEDS              "10004000-5EC4-4083-81CD-A10B8D5CF6EC"
#define BLE_UUID_COMMANDES             "10004001-5EC4-4083-81CD-A10B8D5CF6EC"
BLEService faceledsService(BLE_UUID_FACELEDS);
BLEStringCharacteristic commandesCharacteristic(BLE_UUID_COMMANDES, BLERead | BLEWrite, 30);
String fileName = "";
BLEDevice central;
bool ble_deja_verifie = false;

// Buzzer séquences
void demarrageSoin() {
  // Mélodie de démarrage (type notification ascendante)
  tone(BUZZER_PIN, 440, 200); delay(250);  // A4
  tone(BUZZER_PIN, 554, 200); delay(250);  // C#5
  tone(BUZZER_PIN, 659, 300); delay(350);  // E5
  noTone(BUZZER_PIN);
}

void soinBientotFini() {
  // Petit bip double rapide
  tone(BUZZER_PIN, 880, 100); delay(150);
  tone(BUZZER_PIN, 880, 100); delay(200);
  noTone(BUZZER_PIN);
}

void soinTermine() {
  // Mélodie descendante (type "fin")
  tone(BUZZER_PIN, 659, 200); delay(250);  // E5
  tone(BUZZER_PIN, 554, 200); delay(250);  // C#5
  tone(BUZZER_PIN, 440, 400); delay(500);  // A4
  noTone(BUZZER_PIN);
}

#define TAILLE_COMMANDE_ROUGE (3)
#define TAILLE_COMMANDE_BLEU  (4)
#define TAILLE_COMMANDE_VERT  (5)
#define TAILLE_COMMANDE_JAUNE (6)
#define TAILLE_COMMANDE_STOP  (4)



using namespace esp_panel::drivers;
using namespace esp_panel::board;


Phase* phase_en_cours = NULL;
uint32_t temps_phase1_restants = 0;
uint32_t temps_phase2_restants = 0;

// Fonction pour envoyer une commande via UART (Serial1)
void envoyerCommandeLED_UART(const uint8_t* commande, size_t taille) {
  UART.write(commande, taille);   // Envoie des caractères de la commande
  UART.write('\n');               // Fin de commande (important pour la détection côté esclave)
}

void lv_tick_task(void *arg) {
  lv_tick_inc(1);
}

void setup() {

  UART.begin(115200);
  while (!UART) {
    delay(10);
  }
  UART.print("MAITRE_ACTIVE");
  UART.write('\n');

  String title = "LVGL porting example";
  pinMode(BUZZER_PIN, OUTPUT);

     // BLE
  
      BLE.begin();
      BLE.setDeviceName("Faceleds4000");
      BLE.setLocalName("Faceleds4000");
      BLE.setAdvertisedService(faceledsService);
      faceledsService.addCharacteristic(commandesCharacteristic);
      BLE.addService(faceledsService);
      BLE.advertise();
    

  //pb de lvgl bloque
  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &lv_tick_task,
    .name = "lv_tick"
  };
  esp_timer_handle_t lvgl_tick_timer;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, 1000);  // 1 ms
  //Lvgl initialisation

  Board *board = new Board();
  board->init();

#if LVGL_PORT_AVOID_TEARING_MODE
  auto lcd = board->getLCD();
  // When avoid tearing function is enabled, the frame buffer number should be set in the board driver
  lcd->configFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
#if ESP_PANEL_DRIVERS_BUS_ENABLE_RGB && CONFIG_IDF_TARGET_ESP32S3
  auto lcd_bus = lcd->getBus();

  if (lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) {
    static_cast<BusRGB *>(lcd_bus)->configRGB_BounceBufferSize(lcd->getFrameWidth() * 10);
  }
#endif
#endif
  assert(board->begin());

  //Serial.println("Initializing LVGL");
  lvgl_port_init(board->getLCD(), board->getTouch());

  // Lock the mutex due to the LVGL APIs are not thread-safe
  lvgl_port_lock(-1);
  ui_init();
  lvgl_port_unlock();   // Release the mutex

  UART.print("MAITRE_ACTIVE");
  UART.write('\n');

}

///Gestion commande LEds

void afficher_infos_soin() {
  if (soin_selectionne != NULL && phototype_selectionne != NULL) {
    //Serial.printf("Phototype: %s (coef: %.2f)\n",phototype_selectionne->nom,phototype_selectionne->coef_soins);
  }
}

void start_soin_timer() {
    // Utilisation directe du pointeur pour éviter la copie locale
    if (!soin_selectionne) return;

    phase_en_cours = &soin_selectionne->phase1;
    temps_phase1_restants = tps1;
    temps_phase2_restants = tps2;
    temps_total_restants = tps1 + tps2;

    // Supprimer le timer s’il existe déjà
    if (soin_timer) {
        lv_timer_del(soin_timer);
        soin_timer = NULL;
    }

    // Création ou réinitialisation de la barre de progression
    if (bar_progression == NULL) {
        bar_progression = lv_bar_create(ui_Panel1);
        lv_obj_set_size(bar_progression, 680, 15);
        lv_obj_align(bar_progression, LV_ALIGN_BOTTOM_MID, 0, 10);
    }

    // Configuration de la barre
    lv_bar_set_range(bar_progression, 0, temps_total_restants);
    lv_bar_set_value(bar_progression, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(
        bar_progression,
        lv_color_hex(phase_en_cours->couleur_Bar),
        LV_PART_INDICATOR
    );

    // Création du timer principal (appel toutes les secondes)
    soin_timer = lv_timer_create(soin_timer_callback, 1000, NULL);
    soin_en_cours = true;

    // Démarrage du soin (ex: envoi UART ou BLE)
    demarrageSoin();
}

/////Gestion timer soin

static Phase* phase_precedente = NULL;

void soin_timer_callback(lv_timer_t *timer) {
    if (soin_en_pause) return;

    if (temps_total_restants > 0) {
        temps_total_restants--;

        // Calcul progression barre
        int progress = tps1 + tps2 - temps_total_restants;
        lv_bar_set_value(bar_progression, progress, LV_ANIM_ON);

        // Décrément temps phases et update phase en cours
        if (temps_phase1_restants > 0) {
            temps_phase1_restants--;
            phase_en_cours = &soin_selectionne->phase1;
        } else if (temps_phase2_restants > 0) {
            temps_phase2_restants--;
            phase_en_cours = &soin_selectionne->phase2;
        }

        // Si la phase a changé, on met à jour couleur + envoie commande UART
        if (phase_en_cours != phase_precedente) {
            lv_obj_set_style_bg_color(bar_progression, lv_color_hex(phase_en_cours->couleur_Bar), LV_PART_INDICATOR);
            envoyerCommandeLED_UART(phase_en_cours->commande, phase_en_cours->commande_taille);
            phase_precedente = phase_en_cours;
        }

        // Pré-alerte à 10s
        /*if (temps_total_restants == 10) {
            envoyerCommandeLED_UART(phase_en_cours->commande, phase_en_cours->commande_taille);
            soinBientotFini();
        }*/

        // Mise à jour affichage temps
        lv_label_set_text_fmt(ui_TempsrestantsoinsLabel, "%02d min %02d s", temps_total_restants / 60, temps_total_restants % 60);
    } else {
        lv_timer_del(soin_timer);
        soin_timer = NULL;
        soin_en_cours = false;
        envoyerCommandeLED_UART(commandeStop, TAILLE_COMMANDE_STOP);
        temps_total_restants = 0;
        StopButtonevent(NULL);
        soinTermine();
        phase_precedente = NULL;  // reset pour prochain soin
    }
}

bool verifierConnexionBLE()
{
  BLEDevice central = BLE.central();

  if (central) {
    ble_connecte = true;
    return true;
  } else {
    ble_connecte = false;
    return false;
  }
}

void gererConnexionBLE() {
    // Vérifier si un soin est en cours (temps_total_restants != 0)
    if (temps_total_restants != 0 ) {
        // Si le soin est en cours, empêcher la connexion BLE
        //Serial.println("Déconnexion forcée du BLE, un soin est en cours.");
        central.disconnect(); 
        ble_connecte = false;
        ble_deja_verifie = false;
        return; 
    }
    // Connexion BLE seulement si le soin n'est pas en cours
    if (!ble_connecte && !ble_deja_verifie && temps_total_restants == 0 && verifierConnexionBLE()) {
        central = BLE.central();
        if (central) {
            ble_connecte = true;
            ble_deja_verifie = true;
            // Transition écran
            _ui_screen_change(&ui_BLEScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_BLEScreen_screen_init);
            //Serial.println("BLE connecté !");
            delay(2000);

            // Éteindre les LEDs par défaut
            envoyerCommandeLED_UART(commandeStop, TAILLE_COMMANDE_STOP);
        }
    }

    // Déconnexion BLE 
    if (ble_connecte && !central.connected()) {

        ble_connecte = false;
        ble_deja_verifie = false;
        _ui_screen_change(&ui_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_HomeScreen_screen_init);
        delay(1000);
        envoyerCommandeLED_UART(commandeStop, TAILLE_COMMANDE_STOP);
    }
}



String last_ble_command = "";

void gererCommandesBLE() {
    if (ble_connecte && central.connected()) {
        String nouvelleCommande = commandesCharacteristic.value();
        nouvelleCommande.trim();

        // Si la commande a changé depuis la dernière fois
        if (nouvelleCommande != last_ble_command && nouvelleCommande.length() > 0) {
            last_ble_command = nouvelleCommande;

            if (nouvelleCommande == "rouge") {
                envoyerCommandeLED_UART(commandeRouge, TAILLE_COMMANDE_ROUGE);
            }
            else if (nouvelleCommande == "bleu") {
                envoyerCommandeLED_UART(commandeBleu, TAILLE_COMMANDE_BLEU);
            }
            else if (nouvelleCommande == "stop") {
                envoyerCommandeLED_UART(commandeStop, TAILLE_COMMANDE_STOP);
            }
        }
    }
}


void ui_event_ExitButton(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        UART.print("RUNNING");
        UART.write('\n');
        if (ble_connecte && central.connected()) {
            central.disconnect();
            ble_connecte = false;
            ble_deja_verifie = false;

            envoyerCommandeLED_UART(commandeStop, TAILLE_COMMANDE_STOP);
        }
        // Retour à l'écran principal
        lv_scr_load_anim(ui_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}

static const uint8_t* derniere_commande_envoyee = NULL;
String dernierEtatEnvoye = "";


void control_leds() {
    String nouvelEtat = "";

    // Déterminer l'état actuel
    if (!soin_en_cours || temps_total_restants == 0 || phase_en_cours == NULL) {
        nouvelEtat = "IDLE";
    } else {
        nouvelEtat = "RUNNING";
    }

    // Envoyer l'état uniquement s’il a changé
    if (nouvelEtat != dernierEtatEnvoye) {
        UART.print(nouvelEtat);
        UART.write('\n');
        dernierEtatEnvoye = nouvelEtat;

        delay(10);  // Laisse le temps à l'esclave de traiter l'état
    }

    // Envoyer la commande LED si elle a changé ET si l'état est "RUNNING"
    if (nouvelEtat == "RUNNING" && phase_en_cours) {
        if (phase_en_cours->commande != derniere_commande_envoyee) {
            central.disconnect();
            envoyerCommandeLED_UART(phase_en_cours->commande, phase_en_cours->commande_taille);
            derniere_commande_envoyee = phase_en_cours->commande;
        }
    }

    // Éteindre si IDLE
    if (nouvelEtat == "IDLE" && derniere_commande_envoyee != commandeStop) {
        envoyerCommandeLED_UART(commandeStop, TAILLE_COMMANDE_STOP);
        derniere_commande_envoyee = commandeStop;
    }
}



void loop() {
  BLE.poll();
  gererConnexionBLE();
  gererCommandesBLE();
  delay(5);
}
