# esp32-keyble-homeassistant
Verwende das ESP32-POE IoT Entwicklungsboard von Olimex (https://www.olimex.com/Products/IoT/ESP32/ESP32-POE/open-source-hardware) für das Eqiva Bluetooth-Smartlock, um es in Home Assistant als MQTT-Lock zu integrieren. Das Board bietet 100 Mb Ethernet, Power over Ethernet (PoE), WiFi, BLE und einen integrierten Programmierer.

Wichtige Punkte:
Nutzung der NimBLE-Arduino-Bibliothek (weniger Speicherverbrauch, stabiler!)
Immer aktive Bluetooth-Verbindung (schnelle Steuerung des Schlosses mit minimalem Batterieeinsatz)
Home Assistant MQTT-Schlosseinheit wird automatisch konfiguriert
Optimierungen zur Vermeidung unnötiger Logik
Das Projekt wurde auf dem ESP32-POE erfolgreich getestet und läuft seit einigen Monaten stabil. Es könnte auch mit ähnlicher ESP32-Hardware mit integriertem Ethernet ohne oder mit minimalen Anpassungen funktionieren.

Installation:
Voraussetzungen (nicht hier abgedeckt):

Installiertes VSCode + Platformio Plugin
Installierte Olimex ESP32 USB-Treiber
Konfigurierter und laufender MQTT-Server
Konfigurierter und verbundener HomeAssistant-Server, der mit dem MQTT-Server gekoppelt ist
Eqiva-Lock-Zugangsdaten, die mit der Anleitung im Projekt generiert wurden: https://github.com/oyooyo/keyble
Schritte:

Repository klonen
Projekt mit VSCode + Platformio öffnen
Konfigurationsparameter in src/main.cpp bearbeiten und deine MQTT- und Eqiva-Lock-Zugangsdaten einfügen
Gerät verbinden und auf das ESP32-POE-Board flashen
Eine Schlosseinheit sollte nun auf deinem HomeAssistant-Server erscheinen – viel Spaß!
Basierend auf der großartigen Arbeit von:

tc-maxx/esp32-keyble
lumokitho/esp32-keyble
MariusSchiffer/esp32-keyble
oyooyo/keyble