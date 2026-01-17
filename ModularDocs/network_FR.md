# Module : network (WiFi et réseau)

## Rôle du module
Ce module gère toutes les connexions réseau : WiFi, mDNS, mises à jour OTA et état réseau. Il permet à l'ESP32 de se connecter à Internet, de communiquer avec d'autres appareils et de fournir des services web.

## Pourquoi séparer le réseau ?
- Pour isoler la logique réseau de l'affichage, des boutons, etc.
- Pour pouvoir modifier les réglages réseau ou ajouter des fonctions (OTA) sans casser le reste.
- Pour réutiliser facilement le code réseau dans d'autres projets.

## Où se trouvent les fichiers ?
- `include/network.h` : déclare les fonctions et constantes publiques.
- `src/network.cpp` : contient le code pour le WiFi, mDNS, OTA, etc.

## Fonctions principales
- `bool connectWifiWithFeedback()` : Connecte au WiFi et fournit une progression pour l'affichage.
- `bool initMdns()` : Démarre le service mDNS (accès par nom).
- `void initOta()` : Active la mise à jour OTA (Over-The-Air).
- `bool isMdnsRunning()` : Retourne vrai si mDNS est actif.
- `bool isConnected()` : Retourne vrai si le WiFi est connecté.

## Paramètres importants
- Les identifiants WiFi sont dans `secrets.h` (jamais en dur !).
- Le nom mDNS est défini dans `config.h`.
- Les délais et timeouts sont configurables dans `config.h`.

## Interactions avec les autres modules
- Fournit l'état réseau à l'affichage (SSID, IP, etc.).
- Utilisé par le serveur web pour savoir s'il peut servir des pages.
- Peut déclencher un changement de couleur de la LED (NeoPixel) selon l'état de connexion.

## Bonnes pratiques
- Ne jamais mettre les identifiants WiFi en dur dans le code.
- Toujours vérifier la connexion avant de lancer les services réseau.
- Utiliser mDNS pour un accès facile sans connaître l'IP.

## Pour les débutants
- Si le WiFi ne se connecte pas, vérifiez votre fichier `secrets.h` et votre box.
- L'OTA permet de mettre à jour le firmware sans câble—très pratique !
- mDNS permet d'accéder à l'ESP32 par nom (`http://esp32-template.local/`).

---

> Le réseau est essentiel pour tout objet connecté. Ce module garde la gestion propre et facile à comprendre !
