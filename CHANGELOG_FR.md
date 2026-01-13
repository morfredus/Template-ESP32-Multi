# Journal des changements

## [0.1.2] - 2026-01-13
- Activation de la PSRAM sur ESP32-S3 N16R8 via flags de build et configuration OPI PSRAM

## [0.1.1] - 2026-01-13
- Forcer l'initialisation de la PSRAM sur ESP32-S3 N16R8 et journaliser son statut au demarrage

## [0.1.0] - 2026-01-12
- Creation du template multi-environnements ESP32 DevKit et ESP32-S3 DevKitC-1
- WiFiMulti avec ecran de boot et barre de progression
- Ecran ST7789 affichant nom et version depuis platformio.ini
- NeoPixel optionnel sur ESP32-S3 (GPIO48)
- Interface Web avec stats systeme et endpoint JSON
- Parametres centralises dans config.h et pin mapping dans board_config.h
- Documentation bilingue et exemple de secrets fournis
