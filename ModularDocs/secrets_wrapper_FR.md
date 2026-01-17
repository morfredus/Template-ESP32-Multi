# Module : secrets_wrapper (Gestion des secrets)

## Rôle du module
Ce module gère de façon sécurisée les informations sensibles (mot de passe WiFi, clés API, etc.) pour le projet ESP32. Il offre un moyen sûr de stocker et d’accéder aux secrets dans le code.

## Pourquoi séparer la gestion des secrets ?
- Pour éviter de mettre des données sensibles en dur dans le code principal.
- Pour pouvoir modifier les secrets sans toucher à la logique principale.
- Pour garder les secrets hors du contrôle de version (via .gitignore).

## Où se trouvent les fichiers ?
- `include/secrets_wrapper.h` : déclare les fonctions d’accès aux secrets.
- `src/secrets_wrapper.cpp` : contient la logique de récupération des secrets.
- `include/secrets_example.h` : exemple/template pour les secrets utilisateur.

## Fonctions principales
- `const char* getWifiSsid()` : Retourne le SSID WiFi.
- `const char* getWifiPassword()` : Retourne le mot de passe WiFi.
- Ajouter des fonctions similaires pour d’autres secrets si besoin.

## Paramètres importants
- Les secrets sont définis dans `secrets.h` (non suivi par git).
- Les exemples sont dans `secrets_example.h`.

## Interactions avec les autres modules
- Utilisé par network et web_server pour la connexion WiFi et les API.

## Bonnes pratiques
- Ne jamais committer de vrais secrets sur git.
- Utiliser le fichier exemple comme modèle et garder vos vrais secrets en local.
- Régénérez vos secrets en cas de doute sur une fuite.

## Pour les débutants
- Copiez `secrets_example.h` en `secrets.h` et remplissez vos infos.
- Si le WiFi ne se connecte pas, vérifiez d’abord votre fichier de secrets.

---

> Le module secrets_wrapper protège vos données sensibles et sécurise votre projet !
