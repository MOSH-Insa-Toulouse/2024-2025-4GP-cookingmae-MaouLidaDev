# 2024-2025-4GP-cookingmae-MaouLidaDev

### But du projet

Le but du projet est de pouvoir venir comparer deux technologies de jauge de contrainte.
* Une jauge provenant du commerce : Une "flex sensor" ayant une résistance à plat d'environ 10KOhms
* Une jauge de contrainte en graphite faite par nos soins s'inscrivant dans le cadre des technologies low tech

Pour fabriquer notre jauge de contrainte low tech et low cost, nous nous basons sur un articcle scientifique intitulé! "Pencil Drawn Strain Gauges and Chemiresistors on Paper" publié par Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim et Jiaxing Huang dans lequel il est prouvé que l'on puisse créer une jauge de contraite à partir d'une piste de graphite créer par un crayon un papier sur une feuille de papier. En effet, en se basant sur les phénomènes de transport dans les systèmes granulaires, on peut observer une modification de la resistance de la piste en fonction de la flexion de cette dernière. 

Afin de repondre à la question "est ce qu'il est possible d'obtenir les mêmes resultats qu'une jauge commerciale avec une jauge low cost en graphite?" nous avons: 
* Designer et créer un montage d'amplifaction nous permettant d'exploiter notre jauge en graphite et de relier sa sortie à une carte arduino
* Intégrer un potentiomètre digitale en entrée de notre montage d'amplifaction pour calibrer les mesures
* Créer un shield sous Kicad permettant d'y intégrer notre montage amplificateur, notre potentiomètre, nos 2 capteurs à comparer, un écran OLED pour voir la mesure que nous effectuons, un module Bluetooth pour exporter les datas vers un telephone android et un encodeur rotatoire
* Préparer le PCB et y integrer tout les composants 
* Créer un code Arduino nous permettant de contrôler tout les éléments dont nous disposons sur notre PCB
* Tester notre code ainsi que notre montage physique 
* Tester les deux capteurs et recenser dans une datasheet les caractéristiques de notre jauge de contrainte low cost
* Répondre à la question 
* Préparer nos splendides résutats durant une soutenance début mai


### Livrables

* Un shield PCB conçu sous Kicad pouvant se fixer à une carte Arduino UNO avec dessus un amplificateur transimpédance, un module Bluetooth, un écran OLED et un potentiomètre digital.
* Un code Arduino gérant la mesure de résistance d'une jauge de contrainte faite en graphite, la communication avec un module Bluetooth pour afficher recuillir les valeurs de la mesure sur un portable Android, l'affichage à l'écran OLED pour lire les valeurs en local et la gestion du potentiomètre digital permettant la calibration de notre capteur.
* Un Android APK qui gère l'interfaçage du système de mesure sur un téléphone Android.
* La datasheet du capteur de contrainte en graphite.

## Matériel donnée 

* 1 carte Arduino Uno
* 2 Résistance de 100kΩ
* 1 Résistance de 1kΩ
* 1 Potentiomètre digital MCP-41050
* 2 Condensateurs 100nF
* 1 Condensateurs 1µF
* 1 AOP LTC1050
* 1 Écran OLED01
* 1 Module Bluetooth HC-05

### Code Arduino 


### Création du Shield sous Kicad

Voici ci-dessus le circuit imprimé réalisé à partir d’un calque de PCB.

Le PCB a été réalisé au sein de deux départements de l’INSA Toulouse, le Génie Électrique et Informatique et le Génie Physique. Il a été fabriqué avec une plaquette d’époxy recouverte d’une couche de cuivre d’environ 60𝝻m. Nous avons imprimé sur du calque le circuit ci-dessus, puis nous avons insolé aux UVs notre plaquette avec ce calque pendant une dizaine de minutes. Après avoir retiré la partie de résine non isolée avec un révélateur, nous avons plongé notre plaquette dans un bain de percholorure de fer. Cela a permis de retirer le cuivre qui n’était pas protégé et de révéler les pistes de notre circuit.

### Simulation du circuit sour Ltspice

### Banc de test

### Datasheet 

