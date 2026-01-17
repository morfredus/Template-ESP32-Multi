
# Guide Utilisateur (FR)

> **Version minimum :** 0.4.3

> **La documentation technique complète de chaque module est disponible dans [ModularDocs/README_FR.md](../ModularDocs/README_FR.md). Toutes les fiches sont pédagogiques et incluent une section spéciale pour les débutants.**

## Dupliquer le template
- Copier le dossier du depot puis le renommer.
- Mettre a jour `PROJECT_NAME` et `PROJECT_VERSION` uniquement dans `platformio.ini`.
- Garder `.pio` hors du depot (deja ignore).

## Configurer
- Secrets : copier `include/secrets_example.h` vers `include/secrets.h` et ajouter vos SSID/mots de passe.
- Options : editer `include/config.h` (nom mDNS, rotation LCD, delais WiFi, NeoPixel on/off).
- Pin map : verifier `include/board_config.h` pour chaque environnement avant de cabler.

## Environnements
- `esp32devkit` : ESP32 DevKit classique, pas de NeoPixel, LCD sur VSPI (SCK 18, MOSI 23, CS 5, DC 16, RST 17, BL 4), boutons 0/32/33.
- `esp32s3_n16r8` : ESP32-S3 DevKitC-1 N16R8, NeoPixel GPIO48, LCD SPI 12/11/15 avec CS 10, DC 17, RST 18, BL 16, boutons 0/16/17.

## Ajouter des modules
- Mettre le code commun dans `src/` ou `lib/`. Les headers partages vont dans `include/`.
- Ajouter les librairies dans `lib_deps` de `[env]` pour eviter les doublons.
- Pour un nouveau capteur, etendre `board_config.h` avec des pins dediees et documenter dans `docs` et README.

## Modifier le pin mapping
- `include/board_config.h` est la source de reference. Mettre a jour par environnement, garder BOOT sur GPIO0 et NeoPixel sur GPIO48 pour la S3.
- Eviter les broches de strapping S3 (45/46) et ESP32 (0/2/4/5/12/15) en sortie sauf necessite.
- Apres modification, aligner la documentation et le cablage.

## WiFi
- Utilise `WiFiMulti` : tous les reseaux de `secrets.h` sont testes en sequence.
- L'ecran de boot affiche la progression; en cas de succes, SSID et IP sont visibles.
- Le nom mDNS vient de `config.h` et declare le service HTTP (port 80).

## LCD
- ST7789 via SPI + Adafruit GFX. Pas d'effacement plein ecran en boucle; mises a jour ciblees pour eviter le scintillement.
- Barre de progression active par defaut; peut etre desactivee via `enableBootBar`.
- Rétroéclairage par PWM LEDC avec deux niveaux :
  - `bootBacklightLevel` (défaut 77/255, ~30%) : luminosité réduite durant le boot pour minimiser le courant et éviter les bootloops sur ports USB faibles
  - `backlightLevel` (défaut 255/255, 100%) : luminosité normale en fonctionnement, atteinte progressivement après le boot via fondu de 500ms
- La gestion progressive de puissance évite les pics de courant d'appel durant l'initialisation.

## UI Web
- Serveur `WebServer` integre sur le port 80.
- Accessible via IP ou `http://<mdns>.local/` si mDNS actif.
- Cartes: CPU, frequence, heap/PSRAM, SSID, IP. JSON dispo sur `/status`.

## Structure du projet
- `platformio.ini` : environnements, flags communs, dependances, metadonnees.
- `include/` : configuration (`config.h`, `board_config.h`, secrets) et headers communs.
- `src/` : firmware principal (`main.cpp`).
- `docs/` : documentation bilingue.
- `test/`, `lib/` : emplacements pour tests et libs locales.

## Bonnes pratiques
- Nom/version du projet uniquement dans `platformio.ini`; utiliser les macros dans le code.
- Favoriser les libs natives; ajouter des deps tierces avec parcimonie.
- Tester les deux environnements apres chaque changement; respecter les notes GPIO.
- Chaque increment de version doit etre documente dans README/CHANGELOG.
