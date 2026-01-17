

# Journal des changements

## [0.4.3] - 2026-01-17
### Documentation
- Migration complète vers ModularDocs/ (bilingue, modulaire, pédagogique)
- Tous les documents du projet référencent ModularDocs
- Suppression du dossier Tech/
- Incrément de version dans platformio.ini

> **Toute la documentation technique est désormais modulaire, bilingue et pédagogique. Voir [ModularDocs/README_FR.md](ModularDocs/README_FR.md) pour le détail.**

## [0.4.2] - 2026-01-17
### Ajouté
- Gestion progressive de la puissance pendant la séquence de boot pour éviter les bootloops sur ports USB faibles
- Nouveau paramètre de configuration `bootBacklightLevel` pour réduire le rétroéclairage durant le boot
- Fonction `fadeBacklightToNormal()` pour augmenter progressivement le rétroéclairage après le boot

### Modifié
- Le rétroéclairage démarre maintenant à ~30% durant le boot (77/255) au lieu de 100%
- Le rétroéclairage monte progressivement vers la luminosité normale sur 500ms après le boot
- La LED NeoPixel démarre maintenant éteinte (noir) durant le boot au lieu de bleu doux
- Réduction de la consommation de courant durant la phase d'initialisation pour améliorer la stabilité sur ports USB basse puissance

### Corrigé
- Problème de bootloop sur ordinateurs avec ports USB à délivrance de puissance insuffisante
- Pics de courant d'appel lors de l'initialisation simultanée des périphériques

## [0.4.1] - 2026-01-17
### Modifié
- Incrément de version pour publication après corrections de documentation sur le pin mapping
- Synchronisation de dev/maint vers main pour la release

### Documentation
- Aucun changement de contenu depuis 0.4.0 ; les documents restent valides à partir de 0.4.0

## [0.4.0] - 2026-01-17
### Modifié
- Mise à jour du pin mapping pour les deux environnements de développement pour performances optimales
- ESP32 DevKit: Aucun changement (configuration stable et vérifiée)
- ESP32-S3 N16R8: Mise à jour des broches d'interface d'affichage
  - Display DC: GPIO 17 (depuis GPIO 9)
  - Display RST: GPIO 18 (depuis GPIO 8)
  - Display BL: GPIO 16 (depuis GPIO 7)
  - Display CS: GPIO 10, MOSI: GPIO 11, SCK: GPIO 12, MISO: GPIO 15
- Toutes les assignations GPIO optimisées pour éviter conflits PSRAM sur N16R8

### Documentation
- Mise à jour PIN_MAPPING_FR.md avec table ESP32-S3 corrigée et schéma de câblage
- Mise à jour PIN_MAPPING.md avec table ESP32-S3 corrigée et schéma de câblage
- Mise à jour USER_GUIDE_FR.md avec références GPIO précises pour ESP32-S3
- Mise à jour USER_GUIDE.md avec références GPIO précises pour ESP32-S3
- Mise à jour MODULAR_ARCHITECTURE_FR.md numéro de version en 0.4.0
- Mise à jour MODULAR_ARCHITECTURE.md numéro de version en 0.4.0
- Toute la documentation reflète désormais les définitions GPIO de board_config.h

## [0.3.4] - 2026-01-13
### Corrigé
- Statut mDNS incorrectement affiché comme "disabled" sur LCD alors que le service fonctionnait
- Ajout vérification dynamique de l'état mDNS au lieu de valeur mise en cache au boot
- Implémentation `Network::isMdnsRunning()` pour interroger état réel du service mDNS

### Ajouté
- Nouvelle fonction `Network::isMdnsRunning()` - interroge état réel mDNS à l'exécution
- Mise à jour dynamique du statut dans boucle d'affichage - statut mDNS reflète maintenant état réel
- Ajout include network.h dans tft_display.cpp pour appel isMdnsRunning()

### Modifié
- Statut mDNS sur écran TFT vérifie maintenant `Network::isMdnsRunning()` au lieu de `g_uiState.mdnsOk` en cache
- Affichage du hostname se met à jour dynamiquement selon changements réseau

### Technique
- Cause racine: Statut défini uniquement au boot, jamais mis à jour dans boucle d'affichage
- Solution: Interroger état réel mDNS à chaque cycle de mise à jour d'affichage
- Comportement: Affiche hostname quand mDNS fonctionne, "disabled" sinon
### Corrigé
- Service mDNS ne démarrait pas correctement suite à une stabilisation WiFi insuffisante
- Ajout de vérification de connexion WiFi avant démarrage mDNS
- Ajout délai de 100ms pour assurer que la stack WiFi ESP32 est complètement initialisée
- Amélioration logging des erreurs d'initialisation mDNS

### Modifié
- Amélioration de `Network::initMdns()` avec vérifications de stabilité et sortie debug
- Affichage du nom d'hôte mDNS lors de l'initialisation réussie

### Vérifié
- Service mDNS démarre désormais de manière fiable après connexion WiFi
- Périphérique détectable en tant que `[hostname].local` sur le réseau
- Pas de régression sur les autres services réseau (OTA, WebServer)
### Corrigé
- Erreur de linking causée par définitions multiples de `WIFI_NETWORKS` dans secrets.h
- Création du wrapper de sécurité `secrets_wrapper.h/.cpp` pour isoler l'inclusion de secrets.h
- Adaptation de network.cpp pour utiliser l'API du wrapper (getWifiSsid, getWifiPassword, getWifiNetworksCount)
- Suppression de l'include inutile de secrets.h dans main.cpp

### Ajouté
- Nouveau fichier `include/secrets_wrapper.h` - Interface d'accès sécurisée aux credentials
- Nouveau fichier `src/secrets_wrapper.cpp` - Implémentation du wrapper (seul fichier incluant secrets.h)
- Documentation complète du patch dans `docs/PATCH_001_SECRETS_WRAPPER_FR.md`

### Sécurité
- Les credentials restent centralisés uniquement dans secrets.h
- Validation d'index pour éviter buffer overflow
- Architecture garantissant qu'un seul fichier de compilation inclut secrets.h

## [0.3.1] - 2026-01-13
- Correction de l'initialisation mDNS pour ne s'executer que si la connexion WiFi reussit
- Desactivation des services reseau (mDNS, OTA, serveur web) en cas d'echec de connexion WiFi
- Ajout de message de log coherent quand les services reseau sont desactives

## [0.3.0] - 2026-01-13
- Refactorisation complete du code pour modularite et maintenabilite
- Separation du code en modules fonctionnels :
  - `tft_display.h/cpp` : Affichage ST7789 et fonctions de rendu
  - `web_ui.h/cpp` : Constructeurs de pages HTML et generation de reponses JSON
  - `web_styles.h/cpp` : Styles CSS et JavaScript pour l'interface Web
  - `web_server.h/cpp` : Configuration du serveur HTTP et gestion des routes
  - `network.h/cpp` : Connectivite WiFi, mDNS et OTA
  - `neopixel.h/cpp` : Gestion des LEDs NeoPixel
  - `buttons.h/cpp` : Entree utilisateur et logique de redemarrage
  - `ui_state.h/cpp` : Gestion de l'etat global de l'interface
  - `psram.h/cpp` : Initialisation de la PSRAM
- Simplification du main.cpp pour utiliser les composants modulaires
- Amelioration de l'organisation du code et reduction du main.cpp de 474 a ~70 lignes
- Tous les modules utilisent des namespaces pour une meilleure organisation
- Tous les modules sont independants et peuvent etre testes separement
- Documentation amelioree avec en-tetes de fonction et descriptions de parametres

## [0.2.0] - 2026-01-13
- Reecriture de board_config.h avec commentaires pedagogiques pour debutants
- Ajout de documentation educative pour les declarations GPIO incluant :
  - Tensions requises pour chaque peripherique
  - Valeurs des resistances et condensateurs avec leur puissance
  - Alias courants des broches sur les modules (MISO/SDA, CLK/SCLK, etc.)
  - Schemas de cablage en ASCII art
  - Regles de securite et concepts electriques
- Mise a jour de PIN_MAPPING.md avec composants requis detailles
- Mise a jour de PIN_MAPPING_FR.md avec composants requis detailles
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
