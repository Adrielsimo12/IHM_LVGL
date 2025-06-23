SET(SOURCES screens/ui_MenuSoins.c
    screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_Roboto_16.c
    fonts/ui_font_Roboto_24.c)

add_library(ui ${SOURCES})
