<p align="center">
  <img src="https://github.com/user-attachments/assets/99f9b4e0-36b0-408a-a19b-68bbb6272378" alt="Template ESP32 Multi" width="800">
</p>


# Template ESP32 Multi-Environnements

**Version actuelle : 0.4.3**
*Ce projet met l'accent sur l'accessibilité aux débutants grâce à une documentation pédagogique et modulaire.*

Template PlatformIO pour débutant couvrant ESP32 DevKit et ESP32-S3 DevKitC. Configuration centralisée, WiFiMulti, écran, boutons, NeoPixel optionnel et interface Web embarquée.

## 📚 Documentation technique et pédagogique
- Toute la documentation technique de chaque module (EN/FR) est disponible dans le dossier [ModularDocs/README_FR.md](ModularDocs/README_FR.md).
- Chaque fiche explique le rôle, les fonctions, les paramètres et les bonnes pratiques, avec une section spéciale "Pour les débutants".
- Un schéma d'architecture modulaire est inclus pour visualiser les interactions.

> **Pour les débutants :** Suivez les guides pas-à-pas et consultez les fiches ModularDocs pour comprendre chaque composant du projet.

## Environnements
- `esp32devkit` (ESP32 DevKit)
- `esp32s3_n16r8` (ESP32-S3 DevKitC-1 N16R8)

## Demarrage rapide
1. Copier ce dossier pour creer un nouveau projet.
2. Dupliquer `include/secrets_example.h` en `include/secrets.h`, puis lister vos reseaux WiFi.
3. Ajuster `include/config.h` (nom mDNS, rotation ecran, delais).
4. Verifier le pin mapping dans `include/board_config.h` selon votre carte.
5. Compiler et flasher avec `pio run -t upload` (choisir `esp32devkit` ou `esp32s3_n16r8`).
6. Ouvrir le moniteur serie a 115200 bauds.

## Contenu
- WiFiMulti avec barre de progression et affichage IP
- Ecran de statut (nom/version depuis platformio.ini)
- Resolvedur mDNS pour acces simplifie
- Web UI simple: CPU, memoire, WiFi, IP
- Gestion du bouton BOOT (appui long = reboot) et deux boutons utilisateur
- NeoPixel optionnel sur ESP32-S3 (GPIO48)
- Configuration et pin mapping centralises

## Fichiers clefs
- `include/config.h` — options utilisateur (mDNS, timeouts, affichage)
- `include/board_config.h` — correspondance des broches par environnement
- `include/secrets.h` — identifiants WiFi (hors VCS) ; modele dans `include/secrets_example.h`

## Ecran et Web UI
- LCD pilote Adafruit ST7789 + GFX via SPI. Maintenir la coherence des pins et de la rotation.
- UI Web accessible sur `http://<ip-appareil>/` et `http://<mdns>.local/` si mDNS actif.

## Boutons
- BOOT (GPIO0) appui long -> barre de reboot; relacher pour annuler.
- Deux boutons user exposes dans `board_config.h`.

## NeoPixel (ESP32-S3 uniquement)
- GPIO48 reserve au NeoPixel (1 LED par defaut). Couleurs = etat WiFi.

## Metadonnees projet
Nom et version viennent de `platformio.ini` via `PROJECT_NAME` et `PROJECT_VERSION`. Ne pas les coder en dur dans le code.

## Licence
Ce code template est fourni pour réutilisation dans vos projets. Ajoutez votre licence lors de la duplication.
