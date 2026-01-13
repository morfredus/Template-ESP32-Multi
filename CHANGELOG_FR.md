# Journal des changements

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
