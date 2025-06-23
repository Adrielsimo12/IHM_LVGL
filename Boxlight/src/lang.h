#ifndef LANG_H
#define LANG_H

#include "ui.h"
#include <string.h>
#include "text_id.h"

typedef enum {
    LANG_EN, //anglais
    LANG_FR, //francais
    LANG_ES, //espagnol
    LANG_DE, //allemand
    LANG_IT, //italien
    LANG_PT, //portugais
    LANG_NL, //néerlandais
    LANG_COUNT
} language_t;


// Tableau qui mappe l'ordre des langues dans le dropdown vers l'enum language_t
static const language_t language_ids[] = {
    LANG_EN,
    LANG_FR,
    LANG_ES,
    LANG_DE,
    LANG_IT,
    LANG_PT,
    LANG_NL,
};

#define LANG_DROPDOWN_COUNT (sizeof(language_ids) / sizeof(language_ids[0]))

extern language_t current_language;

// --- Fonctions de traduction ---
const char* translate(text_id_t id);
void change_language(language_t new_lang);
void update_ui_language(void);

// --- Traduction des soins ---

const char* translate_soin(const char* key);

// --- Mise à jour des dropdowns ---
extern void update_dropdown_languages(void);

#endif // LANG_H
