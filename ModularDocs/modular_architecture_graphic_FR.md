# Schéma d’architecture modulaire

Voici un schéma simplifié de l’architecture modulaire de ce projet ESP32. Chaque module est indépendant et communique via des interfaces bien définies.

```
+-------------------+
|    main.cpp       |
+-------------------+
         |
         v
+-------------------+
|   ui_state        |
+-------------------+
   |     |      |   
   v     v      v   
tft  web_ui  network
  |      |      |
  v      v      v
neopixel web_server psram
         |
         v
   secrets_wrapper
```

**Légende :**
- Les flèches indiquent la dépendance ou le flux de données.
- Chaque bloc est un module (voir ModularDocs/ pour le détail).

## Comment lire ce schéma
- `main.cpp` est le point d’entrée et orchestre le système.
- `ui_state` gère ce qui est affiché et les interactions utilisateur.
- Les modules d’affichage (`tft_display`), web UI et réseau dépendent de `ui_state`.
- Les modules de plus bas niveau (neopixel, web_server, psram) sont utilisés par les modules supérieurs.
- `secrets_wrapper` n’est accédé que par les modules ayant besoin de données sensibles.

---

> Pour une explication détaillée de chaque module, voir le fichier correspondant dans ModularDocs/.
