# ProjetLong-Worm
TLS SEC 2023 Projet long 
 
- Developpement d'un serveur vulnerable
  fork sur nouvelle connexion
  Serveur hello : attend un message -> retour ce message
  Vulnerabilite : buffer overflow

- Partie scan / recherche de cible
  Entree : plage d'adresses IP (deux entiers)
  Sortie : liste d'adresses IP potentiellement vulnerables
  A definir : strategie de scan de la plage
  (Au depart : 10.0.0.1 - 10.0.0.255)
  (Replicat : 10.1.0.1 - 10.1.0.255)
- Partie replication
  Message envoye : [code du vers/recopie - data - padding - adresse de Message en memoire]
  Piste : script de linkage (*.ld) : recuperer adresse en memoire du debut du code et l'adresse en memoire de fin du code
  Piste : data doit etre la plage choisie "aleatoirement" que la nouvelle instance doit traiter

- Bootstrap
  Code avec un "main" qui se connecte a un serveur pour copier le vers (la premiere fois)

- Points d'attention
  Eviter d'utiliser les fonctions standard
  Socket

* Etape 1 : developpement du serveur (fork + socket) : regarder sur internet
* Etape 2 : developpement d'un programme malveillant qui scanne une plage, identifie des machines vulnerables, boucle sur la liste de ces machines et exploite la vulnerabilite -> charge malveillante afficher la date
* Etape 3 : modification du programme malveillant pour qu'il s'affiche a l'ecran malveillant - sans exploiter la vulnerabilite - en hexa
* Etape 4 : construction de la charge maleillante en fonction du code malveillant lui-meme (recopie)
