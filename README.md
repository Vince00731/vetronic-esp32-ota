
# VETRONIC ESP32 OTA

[![GitHub release](https://img.shields.io/github/v/release/Vince00731/vetronic-esp32-ota)](https://github.com/Vince00731/vetronic-esp32-ota/releases)
[![arduino-library-badge](https://www.ardu-badge.com/badge/AutoConnect.svg?)](https://www.ardu-badge.com/AutoConnect)
[![License](https://img.shields.io/github/license/Vince00731/vetronic-esp32-ota)](https://github.com/Vince00731/vetronic-esp32-ota/blob/master/LICENSE)

Permet le contrôle à distance de la borne de recharge [VE-TRONIC WB-01](http://ve-tronic.fr/store/wallbox) par [Jeedom](https://www.jeedom.com) (ou tout autre domotique) via un ESP32.

## Présentation

Ce projet permet de communiquer en RS232 avec la borne VE-TRONIC par l'intermédiaire d'une requête HTTP. 
La commande à envoyer à la borne est intégrée à la requête HTTP et l'ESP32 retourne un résultat en XML que Jeedom (ou autre logiciel de domotique) pourra facilement interpréter. 
Côté Jeedom, il faut juste utiliser le plugin [Script](https://market.jeedom.com/index.php?v=d&p=market_display&id=20) et d'utiliser le parser XML intégré pour récupérer les valeurs. Pour envoyer une commande à la borne, le même plugin permet d'envoyer une requête HTTP simplement.

Pour faciliter la configuration et la maintenance logicielle du module ESP32, un serveur Web est intégré ainsi que le manager WiFi [AutoConnect](https://github.com/Hieromon/AutoConnect) permettant la configuration du WiFi mais également la mise à jour du firmware de l'ESP32 par OTA.
Le serveur web intégré à l'ESP32 permet d'envoyer des commandes manuellement à la borne et d'afficher la réponse retournée via une IHM simple.
La première programmation de l'ESP32 devra être réalisé par USB, les mises à jours pourront être faites directement depuis la page Web en sélectionnant le fichier binaire à charger.

![Portail Web](./Images/portail1.png "Portail Web")

## Materiels

- [ESP32 NodeMCU](https://www.amazon.fr/dp/B071P98VTG) (ou un de ses nombreux clones)

![NodeMCU](./Images/nodemcu.jpg)

- [Une Board MAX3232](https://www.amazon.fr/gp/product/B07ZDK4BLH)

![Board RS232](./Images/board_rs232.jpg)
- [Une alimentation 230V vers 5V](https://www.amazon.fr/dp/B00WKKG7CI), j'ai utilisé un bloc secteur de raspberry que j'avais en stock.

![alim](./Images/alim.jpg)
- [un Câble SUBD 9 points mâle / mâle croisé](https://www.amazon.fr/dp/B08LPT8RMF?th=1) pour ma part je l'ai fabriqué en achetant 2 prises SUBD 9 points mâle et du câble électrique pour le faire à la bonne longueur. 

## Câblage

Le câblage de l'ESP32 vers la board RS232 est assez simple, 4 fils à raccorder comme suit:

| ESP32 |     | MAX3232 |
| :---- | :-: | ------: |
| 3V3  | ->  |     VCC |
| GND     | ->  |     GND |
| D15    | ->  |      RX |
| D4    | ->  |      TX |

Côté borne, on récupére l'alimentation 230V sur le bornier du bas le plus à gauche pour y connecter l'alimentation 5V.
On raccorde ensuite le câble RS232 croisé sur le connecteur SUBD en haut à droite.

Si vous voulez connaitre le courant de charge réel envoyé au véhicule, il faudra y raccoder un tore et le monter sur le fil de phase qui part au VE. 
Voir la documentation de la borne: [Documentation WB-01](./docs/wallbox_WB-01_V15A.pdf)

![alim](./Images/cablage.jpg)

## Intégration dans la borne

Voici mon installation provisoire, j'ai prévu de mettre l'ESP32 et la board RS232 sur un support en impression 3D que je ferais plus tard.
Sur la photo, on voit l'alimentation de la raspberry qui a été ouverte pour y souder les 2 fils 230V.

![integration](./Images/integration.jpg)

## Installation du programme dans l'ESP32

1. Installer [l'IDE Arduino](https://www.arduino.cc/en/software) 
2. Dans le gestionnaire de carte, rechercher et installer les cartes [esp32 par Espressif Systems](https://github.com/espressif/arduino-esp32 "esp32 par Espressif Systems")
3. Dans le gestionnaire de librairie, rechercher et installer les librairies suivantes:
	- [PageBuilder](https://github.com/Hieromon/PageBuilder "PageBuilder")
	- [ArduinoJson](https://github.com/bblanchon/ArduinoJson "ArduinoJson")
	- [AutoConnect](https://github.com/Hieromon/AutoConnect "AutoConnect")
4. Redémarrer l'IDE Arduino après avoir installé les packages
5. Charger le projet VETRONIC_ESP32_OTA.ino
6. Sélectionner la carte "ESP32-WROOM-DA" ou autre en fonction de votre module ESP32
7. Raccorder votre ESP32 à votre ordinateur et installer les drivers USB / RS232 correspondant
8. Dans l'IDE Arduino, sélectionner le port COM correspondant à l'ESP32
9. Compiler puis téléverser le code dans l'ESP32

## 1er démarrage du module ESP32

Au 1er démarrage, le module ESP32 va démarrer en point d'accès WiFi.
1. Rechercher et connectez vous au WiFi: ***VETRONIC_ESP32_OTA***, le mot de passe est ***12345678***
2. Normalement votre smartphone ou tablette devrait vous demander de vous connecter à la page du serveur, si ce n'est pas le cas, connectez vous à l'adresse IP suivante: ***172.0.0.1***
3. Cliquer sur ***Configure new AP*** et sélectionner votre borne WiFi.
4. Le module redémarre et bascule sur le WiFi que vous avez configuré, il vous faudra alors récupérer son adresse IP dans votre routeur ou votre box internet. Je vous conseille de le mettre en IP fixe pour ne pas à reconfigurer Jeedom en permanence.

## Mise à jour du firmware du module ESP32

Une fois que le module ESP32 est programmé et configuré une première fois, vous pouvez mettre à jour le firmware depuis la page web du module.
1. Connecter vous au module, l'adresse IP dans mon exemple est http://192.168.0.63, elle sera différente chez vous !
2. Sélectionner le fichier .bin se trouvant dans le répertoire Firmware_ESP32 de se dépot. 
3. Cliquer sur Update
4. A la fin de l'update, patienter jusqu'au rechargement de la page web.

![update](./Images/portail_update.png)

## Change log

### [v1.0] 5 août 2023
- Première version


## License

License under the [MIT license](LICENSE).
