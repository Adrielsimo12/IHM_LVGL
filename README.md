
# 🖥️ IHM_LVGL – Interface Homme-Machine avec LVGL et ESP32

Ce projet présente une interface graphique développée avec **LVGL** (Light and Versatile Graphics Library) pour un écran **TFT Waveshare 7 pouces** piloté par un **ESP32-S3**. Il s'inscrit dans le cadre d'une solution de contrôle pour dispositifs de soins lumineux (LEDs, IR, etc.).

---

## 🚀 Fonctionnalités principales

- Affichage d’une interface utilisateur moderne avec **LVGL**
- Support de l’écran **TFT 7” avec contrôleur ILI9488 (SPI)**
- Intégration d’un écran tactile **XPT2046**
- Contrôle de LEDs via boutons
- Commandes via Bluetooth BLE (ArduinoBLE)
- Lecture et affichage du phototype et du soin sélectionné
- Multilingue : Français, Anglais, Espagnol, Allemand (extensible)

---

## 🛠️ Technologies utilisées

- 🧠 **ESP32-S3**
- 🎨 **LVGL 8.x**
- 🔌 **Arduino IDE**
- 📟 **TFT ILI9488 + XPT2046 (SPI)**
- 📡 **Bluetooth BLE**
- 📁 **I2C pour communication inter-ESP**

## 🌐 Langues supportées

- 🇫🇷 Français
- 🇬🇧 English
- 🇪🇸 Español
- 🇩🇪 Deutsch

---

## 📅 État du projet

✅ Version fonctionnelle en cours de test  
🛠️ Optimisation de la gestion mémoire et BLE en cours  
📦 Préparation d’un boîtier physique à venir

---

## Prérequis

* ESP32 (idéalement modèle ESP32-S3)
* Bibliothèques Arduino LVGL, TFT\_eSPI, Bluetooth (ArduinoBLE)
* Outils de compilation Arduino IDE ou PlatformIO

## Installation

1. Cloner le dépôt :

```bash
git clone https://github.com/Adrielsimo12/IHM_LVGL.git
cd IHM_LVGL
```

2. Ouvrir le projet dans Arduino IDE ou PlatformIO.
3. Installer les dépendances nécessaires via le gestionnaire de bibliothèques.
4. Configurer le fichier `config.h` selon votre matériel (pins, type d’écran).
5. Compiler et téléverser sur l’ESP32.

## 🤝 Contributeurs

- 👨‍💻 **Adriel Tatchum Simo** – Développement, architecture logicielle, interface
- Les contributions sont les bienvenues. Veuillez soumettre des issues ou des pull requests via GitHub.

## Licence

Ce projet est sous licence MIT.



