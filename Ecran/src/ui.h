// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: Boxlight_SLS_00

#ifndef _BOXLIGHT_SLS_00_UI_H
#define _BOXLIGHT_SLS_00_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_events.h"
#include <math.h>
#include "lang.h"
#include "text_id.h"

void image_Animation(lv_obj_t * TargetObject, int delay);

// SCREEN: ui_MenuSoins
extern void ui_MenuSoins_screen_init(void);
extern lv_obj_t * ui_MenuSoins;
extern void ui_event_PhototypeDropdown(lv_event_t * e);
extern void ui_event_SoinsDropdown(lv_event_t * e);
extern lv_obj_t * ui_PhototypeLists;
extern lv_obj_t * ui_SoinsLists;
extern lv_obj_t * ui_Panel1;
extern lv_obj_t * ui_Label7;
extern lv_obj_t * ui_Labeltps;
extern lv_obj_t * ui_Label10;
extern lv_obj_t * ui_NomPhototype;
extern lv_obj_t * ui_NomsoinLabel;
extern lv_obj_t * ui_TempstotalsoinsLabel;
extern lv_obj_t * ui_TempsrestantsoinsLabel;
extern lv_obj_t * ui_Labelphoto;
extern void ui_event_PlayButton(lv_event_t * e);
extern lv_obj_t * ui_PlayButton;
extern lv_obj_t * ui_Label2;
extern void ui_event_PauseButton(lv_event_t * e);
extern lv_obj_t * ui_PauseButton;
extern lv_obj_t * ui_Label3;
extern void ui_event_StopButton(lv_event_t * e);
extern lv_obj_t * ui_StopButton;
extern lv_obj_t * ui_Label4;
extern lv_obj_t * ui_bar_progression;

extern lv_obj_t * ui_LangDropdown;
extern void event_lang_dropdown(lv_event_t * e);
extern void update_dropdown_languages(void);
extern void set_dropdown_language_selection(void);

// CUSTOM VARIABLES
extern void update_play_button_state();
extern void update_pause_button_state();
extern void update_stop_button_state();
extern bool play_button_active;
extern bool pause_button_active;

// SCREEN: ui_BLEScreen
extern void ui_BLEScreen_screen_init(void);
extern lv_obj_t * ui_BLEScreen;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Image3;
extern void ui_event_ExitButton(lv_event_t * e);
extern void ui_event_BLEScreen(lv_event_t * e);
extern lv_obj_t * ui_ExitButton;
// CUSTOM VARIABLES

//Phototype
#define NB_Photo 3 
typedef struct {
    const char* nom; //char nom[32];
    float coef_soins; 
} Phototype;

extern Phototype phototypes[];
extern Phototype *phototype_selectionne;


//Soins
#define NB_SOIN 7 
/*typedef struct {
	text_id_t id;
    //const char* nom;         // Nom du soin
    int temps_rouge;         // Temps en rouge
	int temps_bleu;          // Temps en bleu
} Soin;*/
typedef struct {
    uint16_t duree;             // en secondes
    const char* nom_couleur;    // "rouge", "bleu", "vert", "jaune"
    const uint8_t* commande;    // pointeur vers la trame UART (commandeRouge, etc.)
    size_t commande_taille;
    uint32_t couleur_Bar;    // couleur pour progress bar
} Phase;

typedef struct {
    text_id_t id;
    Phase phase1;
    Phase phase2;
} Soin;

// Déclarez le tableau `soins` globalement
extern Soin soins[];
extern Soin *soin_selectionne;

// SCREEN: ui_HomeScreen
void ui_HomeScreen_screen_init(void);
extern void ui_event_HomeScreen(lv_event_t * e);
void ui_event_ImgButton2(lv_event_t * e);
extern lv_obj_t * ui_HomeScreen;
extern lv_obj_t * ui_ImgButton2;
// CUSTOM VARIABLES

// EVENTS

extern lv_obj_t * ui____initial_actions0;

//image and assets
LV_IMG_DECLARE(ui_img_524120212);    // assets/faceleds.png
LV_IMG_DECLARE(ui_img_ble_png);    // assets/ble.png
LV_IMG_DECLARE(ui_img_mostleds_png);    // assets/MOSTLEDs.png
LV_IMG_DECLARE(ui_img_1985134446);    // assets/exit (4).png

// FONTS
LV_FONT_DECLARE(ui_font_Roboto_16);
LV_FONT_DECLARE(ui_font_Roboto_24);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
