# Démarrage rapide (FR)

> **Version minimale valide :** 0.4.4

---

> **Note :** Toutes les références aux boutons et GPIO dans le code sont désormais strictement conformes aux noms définis dans board_config.h (source officielle, immuable). Les constantes d'affichage (DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SPI_FREQ) sont définies pour ESP32 Classic et ESP32-S3.

## 1. Prérequis
- Visual Studio Code + extension PlatformIO
- Python 3.6+
- Câble USB compatible avec votre ESP32

## 2. Créer votre projet
```bash
# Copier le template
cp -r 00-Template-ESP32-Multi mon-projet-esp32
cd mon-projet-esp32
```

## 3. Configurer le WiFi
- Copier `include/secrets_example.h` vers `include/secrets.h`
- Éditer `include/secrets.h` et ajouter vos réseaux :
```cpp
static const char* WIFI_NETWORKS[][2] = {
    {"MonWiFiMaison", "mdpmaison"},
    {"MonHotspotTelephone", "mdptelephone"}
};
```
- **IMPORTANT :** Ne jamais commiter `secrets.h` (c'est dans `.gitignore`)

## 4. Choisir votre carte
Dans `platformio.ini`, mettre `default_envs` :
```ini
default_envs = esp32devkit          # ESP32 classique
; default_envs = esp32s3_n16r8      # ESP32-S3 (décommenter pour utiliser)
```

## 5. Brancher votre carte
- Brancher l'ESP32 en USB
- PlatformIO détecte automatiquement

## 6. Flasher le firmware
- Cliquer sur l'icône **Build & Upload** dans VS Code (`Ctrl+Alt+U`)
- Attendre la fin (la barre de statut affiche "SUCCESS")

## 7. Monitorer la carte
- Cliquer sur l'icône **Serial Monitor** (`Ctrl+Alt+S`)
- Vitesse : **115200** bauds
- Observer les messages de boot et connexion WiFi

## 8. Accéder à l'interface Web
Une fois connectée au WiFi :
- **Par IP :** http://192.168.x.x (voir votre routeur)
- **Par mDNS :** http://esp32-template.local (nom d'hôte par défaut)

## Ce que vous devez voir
- **Écran LCD** (si câblé) : nom, version, SSID, adresse IP
- **Web UI** : stats du projet, CPU, heap, PSRAM, réseau
- **LED** (S3 seulement) : bleu=boot, vert=WiFi OK, rouge=WiFi échoué

## Dépannage

### Pas de connexion WiFi
- Vérifier SSID et mot de passe dans `secrets.h`
- Le WiFi doit être en 2.4 GHz (ESP32 ne supporte pas 5 GHz généralement)
- Observer le moniteur série pour les tentatives

### Impossible d'accéder à la web UI
- Vérifier l'adresse IP sur l'écran LCD ou le moniteur série
- Essayer mDNS : `ping esp32-template.local`
- Le pare-feu peut bloquer ; vérifier les paramètres du routeur

### Erreurs de compilation
- Vérifier que le bon environnement est sélectionné
- Exécuter "PlatformIO: Clean" puis "PlatformIO: Build"
- Vérifier les chemins des #include

## Étapes suivantes
- Lire [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) pour comprendre les GPIO
- Consulter [ADD_MODULES_FR.md](ADD_MODULES_FR.md) pour étendre les fonctionnalités
- Vérifier [LCD_SYSTEM_FR.md](LCD_SYSTEM_FR.md) pour personnaliser l'écran
