# Module : web_server (Serveur Web)

## Rôle du module
Ce module gère le serveur web HTTP embarqué sur l’ESP32. Il permet d’accéder à une interface web, de consulter l’état du système et d’interagir avec l’ESP32 via un navigateur.

## Pourquoi séparer le serveur web ?
- Pour isoler la logique web du reste du code (matériel, logique principale).
- Pour faciliter l’ajout de nouvelles fonctionnalités web sans impacter le cœur du projet.
- Pour réutiliser facilement ce module dans d’autres projets connectés.

## Où se trouvent les fichiers ?
- `include/web_server.h` : déclare les fonctions publiques et les endpoints.
- `src/web_server.cpp` : contient le code du serveur HTTP.

## Fonctions principales
- `void init()` : Démarre le serveur web et enregistre les endpoints.
- `void handleClient()` : Gère les requêtes HTTP entrantes.

## Paramètres importants
- Le port et les endpoints sont définis dans le code.
- Les fonctionnalités de l’interface web sont activées/désactivées dans `config.h`.

## Interactions avec les autres modules
- Utilise l’état du réseau pour servir les pages uniquement si connecté.
- Peut afficher des données d’autres modules (statut, logs, etc.).

## Bonnes pratiques
- Garder les endpoints simples et bien documentés.
- Toujours vérifier la connexion réseau avant de servir une page.
- Utiliser JSON pour les API de statut si possible.

## Pour les débutants
- Accédez à l’interface web sur `http://<ip-esp32>/` ou `http://<mdns>.local/`.
- Si la page ne s’affiche pas, vérifiez le WiFi et le mDNS.

---

> Le serveur web est la vitrine de votre ESP32. Ce module le rend robuste et facile à faire évoluer !
