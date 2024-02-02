# CY-Trucks-
##### Mariam Kardallas
##### Marwa ZAGLIZ
##### Préing2 MEF1

En tant que responsables d'une société nationale de transport routier, nous disposons d'un outil de gestion logistique. Cependant, les données recueillies sont bien trop volumineuses et mélangées pour être traitées manuellement. Notre objectif est donc de développer un programme afin de présenter de manière globale les activités de Cy Trucks, en fournissant des analyses claires et des graphiques récapitulatifs pour faciliter la compréhension de notre activité logistique.

# Instructions pour compiler
- Si vous utilisez les systèmes Ubuntu ou bien Debian, veuillez mettre à jour la liste des paquets disponibles à partir des dépôts logiciels configurés sur votre système :


                            `sudo apt-get update`

- Pour mettre en oeuvre Cy trucks,  vous aurez besoin de quelques outils dans votre terminal :

                               `sudo apt-get install gnuplot`(Gnuplot est utilisé pour générer des graphiques à partir des données)
                               `sudo apt-get install build-essential` (Pour compiler le programme en C, vous aurez besoin d'un compilateur C)
                               `sudo apt-get install make` ( utiliser l'utilitaire Make)
                               `sudo apt-get install imagemagick`(ImageMagick est utilisé pour manipuler des images, générer des graphiques)
                               `sudo apt-get install dos2unix` (dos2unix est un utilitaire qui convertit les fins de ligne des fichiers texte entre les formats DOS et Unix )

  ## Consigne d'éxécution :

 - Après avoir récupéré le dossier, donné lui les droits d'éxécution :

                                `sudo chmod +x CyTrucks.sh`
 - Ensuite :
   
                                `sudo dos2unix CyTrucks.sh`

 - Enfin :
     
                                `./CyTrucks.sh` (à partir de là vous pouvez lui donner n'imporque quel traitement)
  

  ## Différents traitements disponible :

  - d1: Affiche les 10 premiers conducteurs avec le plus de trajets.
  - d2: Affiche les 10 premiers conducteurs avec la plus grande distance parcourue.
  - l: Montre les 10 trajets les plus longs.
  - t: Liste les 10 villes les plus traversées.
  - s: Génère des statistiques sur les étapes.
  - h: Affiche l'aide, expliquant les options disponibles.

  ## Apercu de la création des différents dossiers:

![Uploading creation_dossier.PNG…]()




  ## Apercu de l'affichage help :

  - Afin d'afficher la fonction d'aide, il suffit d'éxecuter sur le terminal `./CyTrucks.sh -h`

 ![Uploading fonction_aide.PNG…]()






## Répartition des tâches au sein du groupe

| Membre du groupe                                                | Description de la tâche à faire                  |
| ----------------------------------------------------            | ------------------------------------             |
| `Mariam`                                                        | -> Traitement T       
|                                                                 | -> Traitement S                                    
|                                                                 | -> Responsable du code en C (makefile inclus) comme elle ne peut pas utiliser shell sur  son pc dû à un problème.  
|                                                                 | -> Gestion des erreurs : Le programme C doit renvoyer un code d'erreur approprié en cas de problème détecté pendant le traitement des données. Assurez-vous de libérer les allocations mémoires avant la fin du programme.                     
| `Marwa`                                                         | -> Traitement d1
|                                                                 | -> Traitement d2
|                                                                 | -> Responsable de la partie shell 
|                                                                 | -> Génération des graphiques
| `Mariam et Marwa`                                               | -> Gestion du temps d'éxécution
|                                                                 | -> Optimisation mémoire
|                                                                 | -> Gestion des erreurs dans la globalité des erreurs
|                                                                 | -> Réaliser ensemble les traitements où nous avons rencontré des difficultés
|                                                                 | -> Readme


## Planning de réalisation 

* 06/12/2023 : Lancement du projet, c'est-à-dire comprendre les consignes du projet. Découverte du fichier data.csv et compréhension du fichier. (mariam et marwa)
* 13/12/2023 : Commencement du projet, essayez de créer les dossiers etc... (mais ce n'était pas au top) (mariam et marwa)
* 17/12/2023 : Réussit à creer les fichiers temp, images, data, demo, progc. Commencement du traitement d1 (mariam et marwa)
* 20/12/2023 : On arrive pas à afficher le graphique du traitement d1. Les données sont correct mais il y a une erreur dans l'affichage. (mariam et marwa)
* 28/12/2023 : Le même problème persiste. Je suis passée au traitement d2. Le même problème aussi au niveau du graphique mais les données sont correct.(marwa). Commencemment et comprehension traitement t ( mariam)
* 29/12/2023 : Readme + porblème affichage de la fonction aide. (marwa)    Problème avec le data.csv il ne m'affiche qu 1\6 des données. (mariam)
* 30/12/2023 : toujours en train de régler les problèmes. (marwa).  De même pour moi (mariam)
* 31/12/2023 : Le problème avec la fonction d'affichage d'aide est réglé. (marwa)    Le fichier re fonctionne normalement je reprends la traiotement T. 
* 01/01/2024 : Traitement l fait et l'affichage du graphique est correct. (Il faut corriger l'affichage du fichier txt et le temps d'exécution) (marwa)
* 03/01/2024 : Traitement -d1 réssuit le graphique s'affiche correctement (il faut juste afficher le prénom en même temps que le nom) + commencement du traitement s (marwa)
* 04/01/2024 : Traitement -d2 réussit le graphique s'affiche correctement. (il faut juste afficher le prénom en même temps que le nom + le temsp d'execution). J'ai apporté quelque modif au -l mais le temps reste aussi long.
* 10/01/2024 : Commence de la structure du traitement t.
* 13/01/2024 : Déblocage des bugs de d2 et l. (marwa)
* 17/01/2024 : Reprise du traitement t, premier essaie mais erreur. (mariam et marwa)
* 27/01/2024 : Traitement t, il y a un problème il trie tous le fichier. (marwa)
* 28/01/2024 : Toujours sur traitement t. (marwa)
* 29/01/2024 : Je n'arrive pas à trouver mon erreur concernant du traitement t. (marwa)
* 30/01/2024 : Enorme difficulté avec le traitement t il m'affiche des valeurs mais pas les mêmes que les résultats. (marwa)
* 31/01/2024 : On a commencé le traitement s on a eu quelques resultats mais les valeurs ne correspondent pas. (marwa et mariam) J'ai reussis à faire afficher une petite partie du graphique malgré le problème avec les données. (marwa)
* 01/02/2024 : On a refait le menu principal on y a rajouté le traitement -d1 -d2 -l -t -s en shell. (marwa et mariam) On essaye de trouver l'erreur du code en c du traitement t et s. ( marwa et mariam)  Traitement s, je le teste mais il met plus de 30 minutes afin qu'il s'affiche correctement avec le graphique.
* 02/02/2024 : On a réussit à rectifier les erreurs qui étaient présentes dans les traitemnts -t et -s. 
## Ce qu'il reste à faire
**Script Shell :**
- [x] Prendre en paramètres le chemin du fichier CSV d'entrée et les choix de traitement.
- [x] Afficher un message d'aide si l'argument -h est présent.
- [ ] Vérifier la présence de l'exécutable C sur le disque, le compiler si nécessaire, et traiter les erreurs.
- [x] Vérifier la présence des dossiers temp et images, les créer s'ils n'existent pas.
- [x] Vider le dossier temp avant l'exécution des traitements.
- [x] Afficher la durée de chaque traitement en secondes.
- [ ] Créer un graphique à partir des résultats obtenus du programme C.
- [x] Regrouper les traitements effectuer dans le menu principale
- [x] Dans le CyTrucks.sh mettre une fonction qui vérifie l'existence du fichier.csv
- [x] Traitements possibles :
   - [x] **D1** Conducteurs avec le plus de trajets :
   - [x] créer un histogramme horizontal du -d1. (manque juste à afficher le prénom en meme temps que le nom)
   - [x] **D2** Conducteurs et la plus grande distance :
   - [x] créer un histogramme horizontal du -d2. (manque juste à afficher le prénom en meme temps que le nom + temps d'éxecution)
   - [x] **L** Les 10 trajets les plus longs : (corriger le problème avec l'id route et la distance, les chiffres après la virgule, les colonnes du fichier s'affiche pas dans le même ordre que le prof + temps d'éxecution)
   - [x] créer un histogramme vertical du -l.
   - [ ] **T** Les 10 villes les plus traversées : créer un histogramme regroupé.
   - [ ] **S** Statistiques sur les étapes : créer un graphique de courbes min-max-moyenne.

**Programme C :**
- [ ] Compiler le programme C avec un Makefile.
- [x] Définir les fonctionnalités du programme C en fonction des besoins du script Shell.
- [x] Prendre en charge les fichiers d'entrée et de sortie en tant qu'arguments (ou lire depuis l'entrée standard/sortie standard).
- [x] Retourner un code d'erreur strictement positif en cas de problème, 0 sinon.
- [x] Assurer la robustesse du code, libérer les allocations mémoires dans le cas nominal.
- [ ] Limiter la taille mémoire utilisée en définissant des structures avec une empreinte minimale.
- [ ] **T** Les 10 villes les plus traversées : créer un histogramme regroupé.
- [ ] **S** Statistiques sur les étapes : créer un graphique de courbes min-max-moyenne.

## Aperçu des traitements : 

- Pour lancer le traitement -d1, vous devez saisir votre terminal `./CyTrucks.sh data.csv -d1` (il s'affiche entre 3 à 15 secondes)
#### Traitement D1 
![top_10_drivers](https://github.com/Marwa-zag/CY-Trucks-/assets/131532539/98330d24-6d9a-4e27-ac16-f9ed8cd70c10)

- Pour lancer le traitement -d2, vous devez saisir votre terminal `./CyTrucks.sh data.csv -d2` (il s'affiche au bout de 30 secondes)
#### Traitement D2
  ![top_10_driver_distances](https://github.com/Marwa-zag/CY-Trucks-/assets/131532539/2df92570-9325-4b9a-92ce-0fb074d6841d)

Pour lancer le traitement -l, vous devez saisir votre terminal `./CyTrucks.sh data.csv -l` (il s'affiche au bout de 40 secondes)
#### Traitement L:
![traitement_l](https://github.com/Marwa-zag/CY-Trucks-/assets/131532539/651d2c88-fd7d-40e7-8fac-1d3e7a37c087)

Pour lancer le traitement -t, vous devez saisir votre terminal `./CyTrucks.sh data.csv -t`
#### Traitement t : 

Pour lancer le traitement -s, vous devez saisir votre terminal `./CyTrucks.sh data.csv -s`
#### Traitement s: 


## Remarques: 
- Pour le traitement S, dans le programme s'il affiche erreur lors de l'ouverture du fichier, il ne faut pas prendre en compte cette annotation car il execute tout de même le traitement.
- En fonction de chaque traitement, le code génère des fichiers dans le dossiers temp où il y a les résultats attendus .
- Lorsque l'on execute les options -d1, -d2, -l, nous avons les résultats sur le terminal, il faut ainsi aller dans le dossier image pour voir les résulats du graphique.
  
## Difficultés rencontrées : 

- Avant de commencer les traitements, nous avons mit un certain temps avant de comprendre le fichiers data.csv
- Nous avons eu du mal a exécuter et compiler les traitemntrs -t et -s. Nous avons eu du mal à collecter les bonnes données.
- Les traitements -d2 et -l fonctionnement mais ne respectent pas vraiment les 7 secondes ( à tester sur un ordinateur plus performent le temps sera sans doute respecté).
- Nous avons eu du mal à générer les graphiques, nous avons puis seulement afficher les noms de familles sur le graphique -d1 et -d2.
- Les traitements -t et -s ont beaucoup de mal à fonctionner avec la makefile c'est pourquoi pour qu'ils soient opérationnels nous avions dû les executer sans la makefile mais avec gcc.

## Objectifs réussis : 

- A deux c'était compliqué de réaliser tous les traitements, la partie graphique avec gnu plot ... , autrement dit tout le projet en amont des partiels mais nous avons réussis à bien nous répartir les     tâches et travailler dans les temps.
- - Les traitements et options affichent bien ce qui a été demandée.
- Les traitements -t et -s ont éte les plus durs à réaliser mais nous avons finalement réussis à les programmer.

  ## Sources :
  - Vidéos youtube pour gnu plot.
  - TD et CM pour les différentes fonctions des traitements tels que les AVL, les différentes rotations, Shell...
  

