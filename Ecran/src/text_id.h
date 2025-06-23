// text_id.h
#ifndef TEXT_ID_H
#define TEXT_ID_H

typedef enum {
	TEXT_NOM_PHOTOTYPE,
    TEXT_PHOTOTYPE,
	TEXT_NOM_SOIN,
    TEXT_SOIN,
    TEXT_TOTAL_SOIN,
	TEXT_RESTANT_SOIN,
	TEXT_START,
	TEXT_PAUSE,
    TEXT_STOP,

    // Noms des soins
    TEXT_SOIN_ACNE,
    TEXT_SOIN_PEAU_OUVERTE,
    TEXT_SOIN_SOLEIL,
    TEXT_SOIN_BOUTON,
    TEXT_SOIN_MASQUE,
    TEXT_SOIN_ROUGEUR,
    TEXT_SOIN_VIEILLISSEMENT,

    TEXT_COUNT
} text_id_t;


extern const uint8_t commandeRouge[];
extern const uint8_t commandeVert[];
extern const uint8_t commandeBleu[];
extern const uint8_t commandeJaune[];
extern const uint8_t commandeStop[];

#endif