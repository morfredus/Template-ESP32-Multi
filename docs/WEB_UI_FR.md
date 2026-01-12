# Interface Web et réseau (FR)

## Vue d'ensemble
Le template inclut un serveur web embarqué sur le port **80**. Deux endpoints :
- `GET /` — Tableau de bord HTML avec stats système
- `GET /status` — API JSON pour accès programmatique

## Accéder à l'interface

### Par adresse IP
Trouver l'IP sur l'écran LCD ou moniteur série :
```
http://192.168.x.x/
```

### Par nom mDNS
Nom d'hôte par défaut : `esp32-template` (éditable dans `config.h`) :
```
http://esp32-template.local/
```

**Note :** mDNS nécessite support du domaine `.local` (macOS/Linux généralement OK ; Windows peut nécessiter Bonjour).

## Tableau de bord

La page HTML affiche :
- **Processeur** — Modèle de puce (ex. "ESP32-S3")
- **CPU** — Fréquence en MHz
- **Heap** — Mémoire SRAM libre et totale
- **PSRAM** — RAM externe libre et totale (S3 uniquement)
- **SSID** — Nom du réseau WiFi connecté
- **IP** — Adresse IP de l'appareil
- **OTA** — Bouton vers la page de mise à jour OTA

Toutes les valeurs se mettent à jour au chargement. Responsive, fonctionne sur navigateurs mobiles.

## Page de mise à jour OTA (`/update`)
- Accessible via le bouton OTA du tableau de bord ou directement `http://<hote>/update`
- Envoyer un fichier `.bin` compilé ; une barre de progression s'affiche
- Au succès, l'appareil redémarre automatiquement et revient sur le tableau de bord
- Garder le PC et l'ESP32 sur le même WiFi ; utiliser un signal stable pour les gros uploads
- Sécurité : aucune authentification par défaut ; ne pas exposer cet endpoint sur un réseau non fiable

## API JSON Status

Endpoint : `http://esp32-template.local/status`

Exemple de réponse :
```json
{
  "project": "Template ESP32 Multi",
  "version": "0.1.0",
  "chip": "ESP32-S3",
  "cpu_mhz": 240,
  "heap_free": 256000,
  "heap_total": 320000,
  "psram_free": 8000000,
  "psram_total": 8388608,
  "ssid": "MonRéseau",
  "ip": "192.168.1.100"
}
```

Utiliser pour logger des données, monitorer à distance, ou déclencher alertes.

## Personnaliser le tableau de bord

### Changer titre et couleurs
Éditer `buildRootPage()` dans `src/main.cpp` :
```cpp
html += "<title>Mon appareil IoT</title>";
html += "body{background:linear-gradient(135deg,#1a1a2e,#16213e);...}";
```

### Ajouter une nouvelle carte de stats
```cpp
html += "<div class=\"card\">";
html += "<strong>Ma Stat</strong>";
html += "<div>" + String(maValeur) + "</div>";
html += "</div>";
```

### Changer style des cartes
Éditer CSS dans `buildRootPage()` :
```cpp
html += ".card{background:#0b1220;border:2px solid #green;...}";
```

## Monitoring et collecte de données

### Interroger l'endpoint JSON
```bash
# Exemple Bash
while true; do
  curl -s http://esp32-template.local/status | jq .
  sleep 10
done
```

### Envoyer vers le cloud
Ajouter à `loop()` :
```cpp
static unsigned long lastLog = 0;
if (WiFi.isConnected() && millis() - lastLog > 60000) {
    lastLog = millis();
    String json = buildStatusJson();
    // Envoyer json à votre serveur
}
```

### Déclencher selon conditions
```cpp
if (ESP.getFreeHeap() < 50000) {
    Serial.println("Alerte mémoire basse!");
    // Envoyer alerte
}
```

## Configuration WiFi

### Ajouter/supprimer réseaux
Éditer `include/secrets.h` :
```cpp
static const char* WIFI_NETWORKS[][2] = {
    {"Maison", "motdepasse1"},
    {"Bureau", "motdepasse2"},
    {"Mobile", "motdepasse3"}
};
```

### Ajuster délai de connexion
Éditer `include/config.h` :
```cpp
.wifiConnectTimeoutMs = 12000,  // 12 secondes par tentative
.maxWifiAttempts = 10,           // Essayer jusqu'à 10 fois
```

### Connexion manuelle
Le template utilise `WiFiMulti` qui essaie tous les réseaux configurés. Vérifier statut WiFi :
```cpp
if (WiFi.isConnected()) {
    Serial.println("SSID: " + String(WiFi.SSID()));
    Serial.println("IP: " + WiFi.localIP().toString());
}
```

## Dépannage mDNS

### Nom ne résout pas
- Vérifier mDNS activé dans config : `.mdnsHost = "esp32-template"`
- Routeur ne bloque pas mDNS (port 5353 UDP)
- Essayer `ping esp32-template.local` (ajouter `.local` si nécessaire)

### Nom d'hôte différent
Éditer `config.h` :
```cpp
.mdnsHost = "mon-appareil"  // Accessible sur mon-appareil.local
```

## Notes de sécurité

**Le serveur web du template N'A PAS d'authentification.** Si exposé sur un réseau non fiable :
- Désactiver web UI : `enableWebUi = false` dans `config.h`
- OU restreindre accès au routeur/pare-feu
- OU ajouter authentification HTTP (non implémentée par défaut)

## Performance

- Serveur web s'exécute sur **core 0** avec WiFi
- Boucle firmware aussi active ; réactivité dépend de complexité
- Endpoint JSON léger ; utilisable pour interrogation fréquente

## Avancé : Ajouter endpoints personnalisés

```cpp
server.on("/api/temperature", HTTP_GET, []() {
    String json = "{\"temp\":" + String(getTemperature()) + "}";
    server.send(200, "application/json", json);
});
```

Puis appeler pendant `setup()` après `setupWebServer()` :
```cpp
setupWebServer();
server.on("/api/temperature", ...);
```

Voir [documentation Arduino WebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer).
