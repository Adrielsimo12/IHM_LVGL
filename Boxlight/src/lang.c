#include "lang.h"
#include "ui.h"
#include <string.h>

// Texte pour chaque langue
static const char *translations[LANG_COUNT][TEXT_COUNT] = {
    [LANG_EN] = {
        [TEXT_NOM_PHOTOTYPE]         = "Phototype Name : ",
        [TEXT_PHOTOTYPE]             = "Phototype",
        [TEXT_NOM_SOIN]              = "Name of the treatment : ",
        [TEXT_SOIN]                  = "Care",
        [TEXT_TOTAL_SOIN]            = "Total Care Time : ",
        [TEXT_RESTANT_SOIN]          = "Remaining Time Care : ",
        [TEXT_START]                 = "Play",
        [TEXT_PAUSE]                 = "Break",
        [TEXT_STOP]                  = "Stop",

        [TEXT_SOIN_ACNE]             = "Pimple",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Open Skin",
        [TEXT_SOIN_SOLEIL]           = "Sun",
        [TEXT_SOIN_BOUTON]           = "Lip Blister",
        [TEXT_SOIN_MASQUE]           = "Mask",
        [TEXT_SOIN_ROUGEUR]          = "Redness",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Aging",
    },
    [LANG_FR] = {
        [TEXT_NOM_PHOTOTYPE]         = "Nom du Phototype : ",
        [TEXT_PHOTOTYPE]             = "Phototypes",
        [TEXT_NOM_SOIN]              = "Nom du soin : ",
        [TEXT_SOIN]                  = "Soin",
        [TEXT_TOTAL_SOIN]            = "Temps total Soins : ",
        [TEXT_RESTANT_SOIN]          = "Temps restant Soins : ",
        [TEXT_START]                 = "Débuter",
        [TEXT_PAUSE]                 = "Pause",
        [TEXT_STOP]                  = "Arrêter",

        [TEXT_SOIN_ACNE]             = "Acnée",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Peau ouverte",
        [TEXT_SOIN_SOLEIL]           = "Soleil",
        [TEXT_SOIN_BOUTON]           = "Bouton de lèvre",
        [TEXT_SOIN_MASQUE]           = "Masque",
        [TEXT_SOIN_ROUGEUR]          = "Rougeur",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Vieillissement",
    },
    [LANG_ES] = {
        [TEXT_NOM_PHOTOTYPE]         = "Nombre del Fototipo : ",
        [TEXT_PHOTOTYPE]             = "Fototipo",
        [TEXT_NOM_SOIN]              = "Nombre del tratamiento : ",
        [TEXT_SOIN]                  = "Tratamiento",
        [TEXT_TOTAL_SOIN]            = "Tiempo total del cuidado : ",
        [TEXT_RESTANT_SOIN]          = "Tiempo restante del cuidado : ",
        [TEXT_START]                 = "Iniciar",
        [TEXT_PAUSE]                 = "Pausa",
        [TEXT_STOP]                  = "Detener",

        [TEXT_SOIN_ACNE]             = "Acné",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Piel Abierta",
        [TEXT_SOIN_SOLEIL]           = "Sol",
        [TEXT_SOIN_BOUTON]           = "Herpes Labial",
        [TEXT_SOIN_MASQUE]           = "Máscara",
        [TEXT_SOIN_ROUGEUR]          = "Enrojecimiento",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Envejecimiento",
    },
    [LANG_DE] = {
        [TEXT_NOM_PHOTOTYPE]         = "Fototypname : ",
        [TEXT_PHOTOTYPE]             = "Fototypen",
        [TEXT_NOM_SOIN]              = "Behandlungsname : ",
        [TEXT_SOIN]                  = "Pflege",
        [TEXT_TOTAL_SOIN]            = "Gesamte Behandlungszeit : ",
        [TEXT_RESTANT_SOIN]          = "Verbleibende Behandlungszeit : ",
        [TEXT_START]                 = "Start",
        [TEXT_PAUSE]                 = "Pause",
        [TEXT_STOP]                  = "Stopp",

        [TEXT_SOIN_ACNE]             = "Akne",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Offene Haut",
        [TEXT_SOIN_SOLEIL]           = "Sonne",
        [TEXT_SOIN_BOUTON]           = "Lippenbläschen",
        [TEXT_SOIN_MASQUE]           = "Maske",
        [TEXT_SOIN_ROUGEUR]          = "Rötung",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Alterung",
    },
    [LANG_IT] = {
        [TEXT_NOM_PHOTOTYPE]         = "Nome del fototipo : ",
        [TEXT_PHOTOTYPE]             = "Fototipi",
        [TEXT_NOM_SOIN]              = "Nome del trattamento : ",
        [TEXT_SOIN]                  = "Cura",
        [TEXT_TOTAL_SOIN]            = "Tempo totale trattamento : ",
        [TEXT_RESTANT_SOIN]          = "Tempo rimanente trattamento : ",
        [TEXT_START]                 = "Avvia",
        [TEXT_PAUSE]                 = "Pausa",
        [TEXT_STOP]                  = "Ferma",

        [TEXT_SOIN_ACNE]             = "Acne",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Pelle Aperta",
        [TEXT_SOIN_SOLEIL]           = "Sole",
        [TEXT_SOIN_BOUTON]           = "Herpes Labiale",
        [TEXT_SOIN_MASQUE]           = "Maschera",
        [TEXT_SOIN_ROUGEUR]          = "Rossore",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Invecchiamento",
    },
    [LANG_PT] = {
        [TEXT_NOM_PHOTOTYPE]         = "Nome do fototipo : ",
        [TEXT_PHOTOTYPE]             = "Fototipos",
        [TEXT_NOM_SOIN]              = "Nome do tratamento : ",
        [TEXT_SOIN]                  = "Cuidado",
        [TEXT_TOTAL_SOIN]            = "Tempo total de tratamento : ",
        [TEXT_RESTANT_SOIN]          = "Tempo restante do tratamento : ",
        [TEXT_START]                 = "Iniciar",
        [TEXT_PAUSE]                 = "Pausa",
        [TEXT_STOP]                  = "Parar",

        [TEXT_SOIN_ACNE]             = "Acne",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Pele Aberta",
        [TEXT_SOIN_SOLEIL]           = "Sol",
        [TEXT_SOIN_BOUTON]           = "Herpes Labial",
        [TEXT_SOIN_MASQUE]           = "Máscara",
        [TEXT_SOIN_ROUGEUR]          = "Vermelhidão",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Envelhecimento",
    },
    [LANG_NL] = {
        [TEXT_NOM_PHOTOTYPE]         = "Naam van het fototype : ",
        [TEXT_PHOTOTYPE]             = "Fototypen",
        [TEXT_NOM_SOIN]              = "Naam van de behandeling : ",
        [TEXT_SOIN]                  = "Zorg",
        [TEXT_TOTAL_SOIN]            = "Totale behandeltijd : ",
        [TEXT_RESTANT_SOIN]          = "Resterende behandeltijd : ",
        [TEXT_START]                 = "Start",
        [TEXT_PAUSE]                 = "Pauze",
        [TEXT_STOP]                  = "Stop",

        [TEXT_SOIN_ACNE]             = "Puistje",
        [TEXT_SOIN_PEAU_OUVERTE]    = "Open huid",
        [TEXT_SOIN_SOLEIL]           = "Zon",
        [TEXT_SOIN_BOUTON]           = "Lippenblaar",
        [TEXT_SOIN_MASQUE]           = "Masker",
        [TEXT_SOIN_ROUGEUR]          = "Roodheid",
        [TEXT_SOIN_VIEILLISSEMENT]  = "Veroudering",
    }
};

language_t current_language = LANG_EN;

const char *translate(text_id_t id) {
    if (id >= TEXT_COUNT || current_language >= LANG_COUNT) {
        return "";
    }
    return translations[current_language][id];
}

void change_language(language_t new_lang) {
    current_language = new_lang;
    update_dropdown_languages();
}

// --- Mise à jour des dropdowns ---

void update_dropdown_languages(void) {
    // Construction de la liste des phototypes (en supposant phototypes[i].nom déjà traduits ou à traduire)
    char options_photo[256] = "";
    for (int i = 0; i < NB_Photo; i++) {
        strcat(options_photo, phototypes[i].nom);
        if (i < NB_Photo - 1) strcat(options_photo, "\n");
    }
    lv_dropdown_set_options(ui_PhototypeLists, options_photo);

    // Construction de la liste des soins à partir des traductions
    const text_id_t soins_ids[] = {
        TEXT_SOIN_ACNE,
        TEXT_SOIN_PEAU_OUVERTE,
        TEXT_SOIN_SOLEIL,
        TEXT_SOIN_BOUTON,
        TEXT_SOIN_MASQUE,
        TEXT_SOIN_ROUGEUR,
        TEXT_SOIN_VIEILLISSEMENT
    };
    size_t nb_soins = sizeof(soins_ids) / sizeof(soins_ids[0]);

    char options_soins[256] = "";
    for (size_t i = 0; i < nb_soins; i++) {
        strcat(options_soins, translate(soins_ids[i]));
        if (i < nb_soins - 1) strcat(options_soins, "\n");
    }
    lv_dropdown_set_options(ui_SoinsLists, options_soins);
}
