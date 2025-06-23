// --- Structures ---

#define NB_SOIN 7
#define NB_Photo 3

typedef struct {
    const char* nom;
    float coef_soins;
} Phototype;

typedef struct {
    const char* nom;
    int tps_rouge;
    int tps_bleu;
} Soin;

Phototype phototypes[] = {
    { .nom = "Type 1-2", .coef_soins = 1.0f },
    { .nom = "Type 3-4", .coef_soins = 1.25f },
    { .nom = "Type 5-6", .coef_soins = 1.5f }
};

Soin soins[] = {
    {"Acnnée", 3 * 60, 9 * 60},
    {"Peau ouverte", 6 * 60, 0},
    {"Soleil", 6 * 60, 0},
    {"Bouton lèvre", 4 * 60, 4 * 60},
    {"Masque", 6 * 60, 0},
    {"Rougeur", 6 * 60, 0},
    {"Vieillisement", 6 * 60, 0}
};

// --- Clés de traduction ---

#define TEXT_SOIN_ACNE "TEXT_SOIN_ACNE"
#define TEXT_SOIN_PEAU_OUVERTE "TEXT_SOIN_PEAU_OUVERTE"
#define TEXT_SOIN_SOLEIL "TEXT_SOIN_SOLEIL"
#define TEXT_SOIN_BOUTON "TEXT_SOIN_BOUTON"
#define TEXT_SOIN_MASQUE "TEXT_SOIN_MASQUE"
#define TEXT_SOIN_ROUGEUR "TEXT_SOIN_ROUGEUR"
#define TEXT_SOIN_VIEILLISSEMENT "TEXT_SOIN_VIEILLISSEMENT"

// --- Langues ---

enum Langue {
    LANG_FR,
    LANG_EN,
    LANG_ES,
    LANG_DE,
    LANG_IT,
    LANG_PT,
    LANG_NL
};

int current_lang = LANG_FR;

// --- Traductions ---

const char* translate_fr(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Acné";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Peau ouverte";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Soleil";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Bouton de lèvre";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Masque";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Rougeur";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Vieillissement";
    return key;
}

const char* translate_en(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Pimple";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Open Skin";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Sun";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Lip Blister";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Mask";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Redness";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Aging";
    return key;
}

const char* translate_es(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Acné";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Piel Abierta";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Sol";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Herpes Labial";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Máscara";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Enrojecimiento";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Envejecimiento";
    return key;
}

const char* translate_de(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Akne";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Offene Haut";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Sonne";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Lippenbläschen";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Maske";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Rötung";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Alterung";
    return key;
}

const char* translate_it(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Acne";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Pelle Aperta";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Sole";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Herpes Labiale";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Maschera";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Rossore";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Invecchiamento";
    return key;
}

const char* translate_pt(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Acne";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Pele Aberta";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Sol";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Herpes Labial";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Máscara";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Vermelhidão";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Envelhecimento";
    return key;
}

const char* translate_nl(const char* key) {
    if (strcmp(key, TEXT_SOIN_ACNE) == 0) return "Acne";
    if (strcmp(key, TEXT_SOIN_PEAU_OUVERTE) == 0) return "Open Huid";
    if (strcmp(key, TEXT_SOIN_SOLEIL) == 0) return "Zon";
    if (strcmp(key, TEXT_SOIN_BOUTON) == 0) return "Koortslip";
    if (strcmp(key, TEXT_SOIN_MASQUE) == 0) return "Masker";
    if (strcmp(key, TEXT_SOIN_ROUGEUR) == 0) return "Roodheid";
    if (strcmp(key, TEXT_SOIN_VIEILLISSEMENT) == 0) return "Veroudering";
    return key;
}

const char* translate(const char* key) {
    switch (current_lang) {
        case LANG_FR: return translate_fr(key);
        case LANG_EN: return translate_en(key);
        case LANG_ES: return translate_es(key);
        case LANG_DE: return translate_de(key);
        case LANG_IT: return translate_it(key);
        case LANG_PT: return translate_pt(key);
        case LANG_NL: return translate_nl(key);
        default: return key;
    }
}

const char* translate_soin(const char* key) {
    if (strcmp(key, "Acnnée") == 0 || strcmp(key, "Acné") == 0) return translate(TEXT_SOIN_ACNE);
    if (strcmp(key, "Peau ouverte") == 0) return translate(TEXT_SOIN_PEAU_OUVERTE);
    if (strcmp(key, "Soleil") == 0) return translate(TEXT_SOIN_SOLEIL);
    if (strcmp(key, "Bouton lèvre") == 0 || strcmp(key, "Bouton de lèvre") == 0) return translate(TEXT_SOIN_BOUTON);
    if (strcmp(key, "Masque") == 0) return translate(TEXT_SOIN_MASQUE);
    if (strcmp(key, "Rougeur") == 0) return translate(TEXT_SOIN_ROUGEUR);
    if (strcmp(key, "Vieillisement") == 0 || strcmp(key, "Vieillissement") == 0) return translate(TEXT_SOIN_VIEILLISSEMENT);
    return key;
}

// --- Mise à jour des dropdowns ---

void update_dropdown_languages() {
    char options_photo[256] = "";
    for (int i = 0; i < NB_Photo; i++) {
        strcat(options_photo, phototypes[i].nom);
        strcat(options_photo, "\n");
    }
    lv_dropdown_set_options(ui_PhototypeLists, options_photo);

    char options_soins[512] = "";
    for (int i = 0; i < NB_SOIN; i++) {
        strcat(options_soins, translate_soin(soins[i].nom));
        strcat(options_soins, "\n");
    }
    lv_dropdown_set_options(ui_SoinsLists, options_soins);
}










    ui_LangDropdown = lv_dropdown_create(ui_MenuSoins);
    lv_dropdown_set_options(ui_LangDropdown,
                                            "English\n"
                                            "French\n"
                                            "Spanish\n"
                                            "German\n"
                                            "Italian\n"
                                            "Portuguese\n"
                                            "Dutch");
    //lv_dropdown_set_text(ui_LangDropdown, "Languages");
    lv_obj_set_size(ui_LangDropdown, 160, LV_SIZE_CONTENT);
    lv_obj_set_pos(ui_LangDropdown, 315, -200);
    lv_obj_set_align(ui_LangDropdown, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_LangDropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    // Styles dropdown principal
    // Masquer l’indicateur
    lv_dropdown_set_symbol(ui_LangDropdown, NULL);
    lv_obj_set_style_pad_right(ui_LangDropdown, 0, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(ui_LangDropdown, LV_OPA_TRANSP, LV_PART_INDICATOR);

    // Couleur plus foncée au clic
    lv_obj_set_style_bg_color(ui_LangDropdown, lv_color_hex(0x444444), LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_LangDropdown, LV_OPA_COVER, LV_STATE_PRESSED);

    lv_obj_set_style_text_color(ui_LangDropdown, lv_color_hex(0x040404), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LangDropdown, &ui_font_Roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LangDropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lv_dropdown_get_list(ui_LangDropdown), &ui_font_Roboto_24,LV_PART_MAIN | LV_STATE_DEFAULT);

    void update_ui_language(void) {
        lv_label_set_text(ui_Labelphoto, translate(TEXT_NOM_PHOTOTYPE)); //phototypes
        lv_dropdown_set_text(ui_PhototypeLists, translate(TEXT_PHOTOTYPE));//phototype
        lv_label_set_text(ui_Label7, translate(TEXT_NOM_SOIN));//soins
        lv_dropdown_set_text(ui_SoinsLists, translate(TEXT_SOIN));//soins
        lv_label_set_text(ui_Labeltps, translate(TEXT_TOTAL_SOIN));//tps total soin
        lv_label_set_text(ui_Label10, translate(TEXT_RESTANT_SOIN));//tps restants soin
        lv_label_set_text(ui_Label2, translate(TEXT_START));//start
        lv_label_set_text(ui_Label3, translate(TEXT_PAUSE));//pause
        lv_label_set_text(ui_Label4, translate(TEXT_STOP));//stop
        update_dropdown_languages();

    }
	
	    void event_lang_dropdown(lv_event_t * e) {
        uint32_t selected = lv_dropdown_get_selected(ui_LangDropdown);
        change_language((language_t)selected);
        update_ui_language();
    }