# 2024-2025-4GP-cookingmae-MaouLidaDev

# SOMMAIRE
***
- [2024-2025-4GP-cookingmae-MaouLidaDev](#2024-2025-4gp-cookingmae-maoulidadev)
- [SOMMAIRE](#sommaire)
    - [But du projet](#but-du-projet)
    - [Livrables](#livrables)
    - [Matériel donnée](#matériel-donnée)
    - [Simulation du circuit sous Ltspice](#simulation-du-circuit-sous-ltspice)
    - [Création du Shield sous Kicad](#création-du-shield-sous-kicad)
    - [Code Arduino](#code-arduino)
    - [Application Android sous MIT App Inventor](#application-android-sous-mit-app-inventor)
    - [Banc de test](#banc-de-test)
    - [Datasheet](#datasheet)
    - [Conclusion](#conclusion)
    - [Contact](#contact)
***  

### But du projet

Le but du projet est de pouvoir venir comparer deux technologies de jauge de contrainte.
* Une jauge provenant du commerce : Une "flex sensor" ayant une résistance à plat d'environ 10KOhms
* Une jauge de contrainte en graphite faite par nos soins s'inscrivant dans le cadre des technologies low tech

Pour fabriquer notre jauge de contrainte low tech et low cost, nous nous basons sur un article scientifique intitulé: "Pencil Drawn Strain Gauges and Chemiresistors on Paper" publié par Cheng-Wei Lin, Zhibo Zhao, Jaemyung Kim et Jiaxing Huang dans lequel il a été prouvé que l'on puisse créer une jauge de contraite à partir d'une piste de graphite. Cette piste de graphite peut être dessiner à l'aide d'un crayon à papier sur une feuille de papier tout deux disponibles dans le commerce. En effet, en se basant sur les phénomènes de transport dans les systèmes granulaires, on peut observer une modification de la resistance de la piste en fonction de la flexion de cette dernière. Nous souhaitons observer ce phénomène pour differentes pistes de graphite plus au moins chargées en particles de graphite. Ainsi, nous pourrons étudier la variation de résistance de la piste en fonction de la déformation et ce pour chaque piste obtenue par dépot de crayon à papier differents.

Afin de pouvoir observer ce phénomène, nous avons débuté ce projet et proposons de répondre à la question suivante: "Est ce que la sensibilité d'une jauge de contrainte en graphite peut être comparable à celle d'une jauge dans le commerce et si oui est-il possible de commercialiser cette jauge low cost et low tech à grande echelle?"

Pour repondre à cette question, nous mettons en place un protocole constituer de phase de design, création, programmation et test pour notre jauge de contrainte en graphite.
Voici  un récapitulatif des étapes:
* Designer et créer un montage d'amplifaction nous permettant d'exploiter notre jauge en graphite et de relier sa sortie à une carte arduino
* Intégrer un potentiomètre digitale en entrée de notre montage d'amplifaction pour calibrer les mesures
* Créer un shield sous Kicad permettant d'y intégrer notre montage amplificateur, notre potentiomètre, nos 2 capteurs à comparer, un écran OLED pour voir la mesure que nous effectuons, un module Bluetooth pour exporter les datas vers un telephone android et un encodeur rotatoire
* Préparer le PCB et y integrer tout les composants 
* Créer un code Arduino nous permettant de contrôler tout les éléments dont nous disposons sur notre PCB
* Tester notre code ainsi que notre montage physique 
* Tester les deux capteurs et recenser dans une datasheet les caractéristiques de notre jauge de contrainte low cost
* Répondre à la problèmatique concernant notre jauge de contrainte 
* Préparer nos splendides résutats durant une soutenance début mai


### Livrables

* Un shield PCB conçu sous Kicad pouvant se fixer à une carte Arduino UNO avec dessus un amplificateur transimpédance, un module Bluetooth, un écran OLED et un potentiomètre digital.
* Un code Arduino gérant la mesure de résistance d'une jauge de contrainte faite en graphite, la communication avec un module Bluetooth pour afficher recuillir les valeurs de la mesure sur un portable Android, l'affichage à l'écran OLED pour lire les valeurs en local et la gestion du potentiomètre digital permettant la calibration de notre capteur.
* Un Android APK qui gère l'interfaçage du système de mesure sur un téléphone Android.
* La datasheet du capteur de contrainte en graphite.

### Matériel donnée 

* 1 carte Arduino Uno
* 2 Résistance de 100kΩ
* 1 Résistance de 1kΩ
* 1 Potentiomètre digital MCP-41050
* 2 Condensateurs 100nF
* 1 Condensateurs 1µF
* 1 AOP LTC1050
* 1 Écran OLED01
* 1 Module Bluetooth HC-05

### Simulation du circuit sous Ltspice

Nous souhaitons pouvoir récupérer la resistance de notre capteur de Graphite. Ce dernier, lorsqu'une de ses pins est connectée à une source de tension de (environ 5V) délivre sur sa deuxième pin un très faible courant (de l'ordre du nanoampère). En effet, la resistance de notre jauge de contrainte en graphite est de l'ordre du gigaohm ce qui pour la gamme de tension d'alimentation dont nous disposons pour nos tests se traduit par un courant très faible à exploiter. 

Pour pouvoir tout de même récupérer la variation de resistance de notre capteur, nous proposon un montage d'amplification transimpédence composé de 3 filtres passe bas et d'un AOP. L'AOP se doit de pouvoir amplifier notre signal pour pouvoir donner une tension qui pourra ensuite être analysée par une carte Arduino UNO. Nous avons choisis le LTC 1050.
En plus de l'AOP, nous rajoutons 3 filtres: 
  - Un filtre passe bas passif composé de R1 et C1 de fréquence de coupure de 16Hz pour filtrer les bruits en courant sur le signal d'entrée
  - Une filtre passe bas actif composé de R3 et C4 de fréquence de coupure de 1.6Hz permettane de filtre la composante de bruit à 50Hz provenant du couplage avec le réseau éléctrique 
  - Un filtre passe bas standard composé de R6 et C2 de fréquence de coupure de 1.6kHz qui permet de s'affranchir des bruit crées par l'environnement de l'observation
 Il est possible de venir rajouter une resistance variable R2 de sorte à pouvoir garantir la non saturation de l'entrée analogique de la carte Arduino UNO récupérant le signal de sortie de l'amplificateur transimpédance. En effet, comme la resistance de la piste varie en fonction de sa teneur en particule de graphite, il sera possible de venir faire varier R2 pour modifier le gain de notre amplification. 
 
 Nous avons testé ce montage sur le logiciel LTspice basé sur [ce schéma](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-cookingmae-MaouLidaDev/blob/main/Images/SchemaLTSpice.png) 

Grâce à notre simulation, nous avons pu valider que nos filtres sont bien dimensionnés pour atténuer les bruits (par exemple, dans le cas des simulations en courant alternatif, on constate que notre atténuation est de l'orde 40dB pour la composante à 50Hz).


### Création du Shield sous Kicad

Voici ci-dessous des images des schéma de notre PCB sous Kicad: 
![schematique_kicad](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-cookingmae-MaouLidaDev/blob/main/Images/Sch%C3%A9matique%20Kicak.png)
![routage_kicad](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-cookingmae-MaouLidaDev/blob/main/Images/SHIELD%20.png)
![troisd_kicad](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-cookingmae-MaouLidaDev/blob/main/Images/3D%20du%20SHIELD.png)


Le PCB a été entierement disigné sous le logiciel Kicad et imprimé au sein de deux départements de l’INSA Toulouse, le Génie Électrique et Informatique et le Génie Physique. Il a été fabriqué avec une plaquette d’époxy recouverte d’une couche de cuivre d’environ 60𝝻m sur laquelle nous avons imprimé le circuit. Pour imprimé le circuit, nous utilisons un calque nous permettant d'isoler les pistes des UVs et de venir graver à l'aide d'un bain de percholorure de fer les parties en cuivre non désirées. 

Une fois le PCB tiré, nous sommes passé à la soudure des composants nécéssaires pour pouvoir analyser la réponse de notre capteur graphite. 
Voici ci-dessous une image de notre carte une fois imprimée et une fois tout les composants ajoutés: 



### Code Arduino 

### Application Android sous MIT App Inventor

### Banc de test

### Datasheet 

### Conclusion

### Contact

-Hamadi Maoulida: mhamadi@insa-toulouse.fr
-Tartera Maëlys : tartera@insa-toulouse.fr

