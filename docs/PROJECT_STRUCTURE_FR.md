
# Structure du projet (FR)

> **La documentation technique complète de chaque module est disponible dans [ModularDocs/README_FR.md](../ModularDocs/README_FR.md). Toutes les fiches sont pédagogiques et incluent une section spéciale pour les débutants.**

## Arborescence
```
00-Template-ESP32-Multi/
├── platformio.ini              # Configuration projet, environnements, version
├── .gitignore                  # Ignore Git (inclut secrets.h)
│
├── README.md                   # Vue rapide (EN)
├── README_FR.md                # Vue rapide (FR)
├── CHANGELOG.md                # Historique versions (EN)
├── CHANGELOG_FR.md             # Historique versions (FR)
│
├── docs/
│   ├── GETTING_STARTED.md      # Démarrage pas-à-pas (EN)
│   ├── GETTING_STARTED_FR.md   # Démarrage pas-à-pas (FR)
│   ├── PIN_MAPPING.md          # Référence GPIO (EN)
│   ├── PIN_MAPPING_FR.md       # Référence GPIO (FR)
│   ├── LCD_SYSTEM.md           # Personnalisation écran (EN)
│   ├── LCD_SYSTEM_FR.md        # Personnalisation écran (FR)
│   ├── WEB_UI.md               # Serveur web & API (EN)
│   ├── WEB_UI_FR.md            # Serveur web & API (FR)
│   ├── ADD_MODULES.md          # Intégration capteurs (EN)
│   ├── ADD_MODULES_FR.md       # Intégration capteurs (FR)
│   ├── USER_GUIDE.md           # Guide général (EN)
│   ├── USER_GUIDE_FR.md        # Guide général (FR)
│   └── [autres guides]
│
├── include/
│   ├── config.h                # Paramètres utilisateur
│   ├── board_config.h          # Définitions GPIO (spécifique environnement)
│   ├── secrets.h               # Identifiants WiFi (créé par utilisateur, ignoré)
│   ├── secrets_example.h       # Modèle WiFi (ne pas éditer)│   ├── secrets_wrapper.h       # API sécurisée pour accès aux credentials│   └── [autres headers]
│
├── src/
│   ├── main.cpp                # Point d'entrée (setup, loop)│   ├── secrets_wrapper.cpp     # Wrapper sécurisé (seul fichier incluant secrets.h)│   └── [modules feature]       # Fichiers .cpp additionnels
│
├── lib/
│   └── README                  # Placeholder librairies locales
│
├── test/
│   └── README                  # Placeholder tests unitaires
│
├── .pio/                       # Sortie build PlatformIO (git-ignoré)
└── .vscode/                    # Paramètres VS Code (git-ignoré)
```

## Fichiers clefs expliqués

### platformio.ini
Définit :
- **Environnements** (`esp32devkit`, `esp32s3_n16r8`)
- **Métadonnées projet** (`PROJECT_NAME`, `PROJECT_VERSION`)
- **Flags build communs** (optimisation, flags compilateur)
- **Librairies** (`lib_deps` pour Adafruit, etc.)
- **Paramètres upload/monitor**

À mettre à jour lors d'ajout d'environnements ou librairies.

### include/board_config.h
**À NE PAS ÉDITER** sauf ajouter nouvelles GPIO. Contient :
- Définitions broches boutons (BOOT, USER1, USER2)
- Définitions broches écran (SPI : SCK, MOSI, MISO, CS, DC, RST, BL)
- Broche NeoPixel (S3 seulement, GPIO48)
- Configurations spécifiques environnement

Toutes références GPIO viennent d'ici. Mettre à jour docs après changements.

### include/config.h
**Éditable par l'utilisateur**. Paramètres centralisés :
- Nom mDNS (`mdnsHost`)
- Rotation LCD, niveau rétroéclairage
- Timeouts WiFi, tentatives connexion
- Visibilité barre de boot
- Web UI on/off
- Toggles fonctionnalités

Éditer ici pour personnalisation sans toucher le firmware.

### include/secrets.h
**Git-ignoré** (voir `.gitignore`). L'utilisateur crée en dupliquant `secrets_example.h`.
Contient réseaux WiFi (paires SSID/mot-de-passe). Ne jamais commiter.

**Important**: Depuis v0.3.2, ce fichier doit UNIQUEMENT être inclus par `secrets_wrapper.cpp`.
Les autres fichiers doivent utiliser l'API de `secrets_wrapper.h` pour accéder aux credentials de manière sécurisée.

### src/main.cpp
**Point d'entrée**. Contient :
- Initialisation objets globaux (WiFiMulti, WebServer, écran, NeoPixel)
- `setup()` — appelé une fois au boot (init hardware, WiFi, serveur web)
- `loop()` — appelé répétément (~1000 fois/sec)
- Fonctions auxiliaires (écran boot, écran statut, gestion boutons)

Garder focalisé sur setup. Mettre code fonctionnalités dans `.cpp` séparés.

### docs/
Documentation complète bilingue :
- **GETTING_STARTED** — Configuration première utilisation
- **PIN_MAPPING** — Référence GPIO
- **LCD_SYSTEM** — Personnalisation écran
- **WEB_UI** — Serveur, API, réseau
- **ADD_MODULES** — Ajouter capteurs et fonctionnalités
- **USER_GUIDE** — Vue d'ensemble générale

Tous les guides existent en anglais (`*.md`) et français (`*_FR.md`).

## Flux de travail typique

1. **Dupliquer ce template** pour démarrer nouveau projet
2. **Mettre à jour platformio.ini** : Changer `PROJECT_NAME`, `PROJECT_VERSION`, environnement par défaut
3. **Créer `include/secrets.h`** à partir de `secrets_example.h`, lister vos réseaux WiFi
4. **Personnaliser `include/config.h`** : Nom mDNS, rotation écran, etc.
5. **Flasher et tester** : `pio run -t upload`, monitorer à 115200 bauds
6. **Ajouter fonctionnalités** : Créer `.cpp` dans `src/`, référencer GPIO de `board_config.h`
7. **Mettre à jour docs** : Garder README et CHANGELOG en sync

## Ajouter nouvelles fonctionnalités

### Ajouter un capteur simple
1. Ajouter définition GPIO à `board_config.h`
2. Créer `src/mon_capteur.cpp` et `include/mon_capteur.h`
3. Initialiser dans `setup()`
4. Lire/mettre à jour dans `loop()` ou fonctions tâches
5. Documenter dans [ADD_MODULES_FR.md](ADD_MODULES_FR.md)

### Ajouter nouvel environnement (ex. ESP32-WROOM-32)
1. Mettre à jour `platformio.ini` avec nouvelle section `[env:name]`
2. Ajouter définitions GPIO à `board_config.h` avec `#if defined(ENV_NAME)` correspondant
3. Mettre à jour [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) avec référence nouvelle carte

### Étendre interface Web
1. Éditer `buildRootPage()` dans `src/main.cpp` pour ajouter HTML/CSS
2. Ajouter nouveaux endpoints `server.on()` pour routes API
3. Documenter changements dans [WEB_UI_FR.md](WEB_UI_FR.md)

## Versioning

Suivre **SEMVER** (Semantic Versioning) :
- **MAJEUR.MINEUR.PATCH** (ex. 1.2.3)
- MAJEUR : changements incompatibles
- MINEUR : nouvelles fonctionnalités (compatible)
- PATCH : corrections bugs

À chaque changement de version :
1. Mettre à jour `PROJECT_VERSION` dans `platformio.ini`
2. Mettre à jour `CHANGELOG.md` et `CHANGELOG_FR.md`
3. Commiter avec message : `chore: bump to v1.2.3`

## Tâches courantes

### Changer nom/version projet
Éditer `platformio.ini` :
```ini
build_flags =
    -D PROJECT_VERSION='"1.0.0"'
    -D PROJECT_NAME='"Mon Projet IoT"'
```

### Changer environnement par défaut
Éditer `platformio.ini` :
```ini
default_envs = esp32s3_n16r8  # ou esp32devkit
```

### Ajouter dépendance librairie
Éditer `platformio.ini` dans `[env]` :
```ini
lib_deps =
    ... existantes ...
    adafruit/Adafruit BME280@^2.2.2
```

### Tester compilation
```bash
pio run -e esp32devkit
pio run -e esp32s3_n16r8
```

### Nettoyer build
```bash
pio run -t clean
pio run
```

Voir [GETTING_STARTED_FR.md](GETTING_STARTED_FR.md) pour première utilisation.
