# CY-Trucks-

# Instructions pour compiler


rajouter ce qui faut installer ce qui faut ecrire pour executer mettre des images pour faire des exemples 










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
## Ce qu'il reste à faire
**Script Shell :**
- [ ] Prendre en paramètres le chemin du fichier CSV d'entrée et les choix de traitement.
- [x] Afficher un message d'aide si l'argument -h est présent.
- [ ] Vérifier la présence de l'exécutable C sur le disque, le compiler si nécessaire, et traiter les erreurs.
- [x] Vérifier la présence des dossiers temp et images, les créer s'ils n'existent pas.
- [x] Vider le dossier temp avant l'exécution des traitements.
- [x] Afficher la durée de chaque traitement en secondes.
- [ ] Créer un graphique à partir des résultats obtenus du programme C.
- [ ] Regrouper les traitements effectuern dans le menu principale
- [ ] Dans le CyTrucks.sh mettre une fonction qui vérifie l'existence du fichier.csv
- [ ] Traitements possibles :
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
- [ ] Définir les fonctionnalités du programme C en fonction des besoins du script Shell.
- [ ] Prendre en charge les fichiers d'entrée et de sortie en tant qu'arguments (ou lire depuis l'entrée standard/sortie standard).
- [ ] Retourner un code d'erreur strictement positif en cas de problème, 0 sinon.
- [ ] Assurer la robustesse du code, libérer les allocations mémoires dans le cas nominal.
- [ ] Limiter la taille mémoire utilisée en définissant des structures avec une empreinte minimale.
- [ ] **T** Les 10 villes les plus traversées : créer un histogramme regroupé.
- [ ] **S** Statistiques sur les étapes : créer un graphique de courbes min-max-moyenne.


