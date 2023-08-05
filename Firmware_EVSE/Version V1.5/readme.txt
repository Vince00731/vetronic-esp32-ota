INSTALLATION
------------

Dézarchiver le fichier stm32-update-package.zip dans votre dossier personnel sous mac ou Linux,
ou sur votre Bureau sous windows

Le programme pour flasher le microcontroleur est en fait un script python, il faut donc
dans un 1er temps installer l'interpreteur python.

Installation de Python pour Windows :
-------------------------------------

- Dans le dossier stm32-update-package/install/windows, executer python-2.7.2.msi et procéder
  à l'installation

Installation de Python pour Linux ou Mac :
------------------------------------------

- Python est déjà probablement installé, si ce n'est pas le cas, l'installer avec le gestionnaire de logiciel integré
  à votre distribution



Ensuite il faut installer l'extension python "pySerial" permettant de communiquer sur un port série.


Installation de l'extension pySerial pour Windows :
---------------------------------------------------

- Aller dans le dossier stm32-update-package/install/pyserial-2.5
- Executer le script install.bat



Installation de l'extension pySerial pour Linux ou mac :
--------------------------------------------------------

- Ouvrir une ligne de commande (Application/Utilitaires/Terminal sous Mac)
- Aller dans le dossier stm32-update-package, puis dans le sous dossier install/pyserial-2.5 :
  - cd stm32-update-package
  - cd install
  - cd pyserial-2.5
- Puis installer l'extension en executant la commande suivante : python setup.py install
- Fermer l'invite de commande


Ensuite il faut configurer le n° du port série qui va être utilisé pour communiquer avec la carte



Configuration du port série sous Windows :
------------------------------------------

- Editer le fichier stm32-update-package/load_windows.bat (clic droit / modifier) et sur la
  1ère ligne remplacer COM1 par le bon COM si c'est nécessaire
- Si une interface USB/RS232 est utilisée, aller dans le gestionnaire de périphérique
  windows pour déterminer le bon N° de COM. 
- Enregistrer le fichier une fois la configuration effectuée.

Configuration du port série sous Linux ou Mac :
-----------------------------------------------

- Editer le fichier stm32-update-package/load_linux_mac.bash et sur la 3ème ligne indiquer
  le chemin du port série à utiliser
- Sous linux ça va ressembler à port=/dev/ttyS0 ou port=/dev/ttyUSB0 par exemple
- Sous Mac ça va ressembler à ?????



PROCEDURE DE REFLASH
--------------------

- Placer le fichier firmware.bin à flasher dans le dossier stm32-update-package
- Couper l'alimentation du système
- Connecter le système au PC avec le port série
- Sur la carte mère positionner le jumper de mise en mode boot
- Remettre l'alimentation du système
- Lancer la mise à jour en double cliquant sur le fichier load_windows.bat ou load_linux_mac.bash
- Lorsque la mise à jour fonctionne, une série de ligne de programmation défile dans la console
- Une fois la mise à jour effectuée, couper l'alimentation du système
- Retirer le jumper de mise en mode boot
- Remettre l'alimentation


