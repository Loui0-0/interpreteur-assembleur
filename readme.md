# Simulateur d'Assembleur  
**Projet d'Architecture des Ordinateurs**  

Victor Cai et Louis de Domingo  

2023-2024  

## Description  
Ce projet est un simulateur de code assembleur permettant de traduire et d'exécuter des programmes assembleur. Il se compose de deux parties principales :  
1. **Traducteur** : Convertit le code assembleur en instructions hexadécimales, générant un fichier `hexa.txt`.  
2. **Machine Virtuelle** : Simule une machine avec des registres, de la mémoire et un processus d'exécution de programme, permettant un traitement pas à pas des instructions.  

## Prérequis  
- **GCC** (GNU Compiler Collection) installé sur votre système.  
- Un terminal pour exécuter les commandes.  

## Installation  
1. Téléchargez ou clonez le dépôt du projet.  
2. Ouvrez un terminal dans le dossier du projet (`CaiDeDomingo`).  

**Si l'exécutable `simulateur` est manquant ou endommagé** :  
Compilez le projet avec la commande suivante : 
```bash 
gcc -Wall -o simulateur -I include src/*.c  
```
## Utilisation  
- Pour exécuter le simulateur, utilisez la commande :  
  ./simulateur [fichier_assembleur]  
  (Remplacez `[fichier_assembleur]` par le chemin de votre fichier.)  
- Si la traduction réussit, le fichier `hexa.txt` est généré et le programme s'exécute automatiquement.  
- Les erreurs (syntaxe, étiquettes manquantes, etc.) sont affichées dans le terminal avant l'exécution.  

## Fonctionnalités  
- **Traduction Assembleur vers Hexadécimal** :  
  - Prise en charge des étiquettes, registres (r0 à r31) et valeurs immédiates.  
  - Détection d'erreurs contextuelles (ex: registres invalides, valeurs hors plage).  
- **Machine Virtuelle** :  
  - 32 registres 16 bits, mémoire de 64 Ko, drapeaux d'état (retenue, overflow, etc.).  
  - Exécution pas à pas des instructions (arithmétiques, sauts, accès mémoire).  
- **Génération de Fichier** : Création automatique de `hexa.txt` après une traduction réussie.  

## Structure du Projet  
### Phase de Traduction  
1. **Extraction des Étiquettes** : La fonction `extractionEtiquette` identifie les étiquettes et leurs adresses.  
2. **Découpage des Lignes** : Chaque instruction est décomposée en tokens via `intructionVersToken`.  
3. **Encodage Hexadécimal** : Conversion des tokens en instructions 32 bits avec vérification des erreurs.  

### Phase de Simulation  
- **Initialisation** : La structure `Machine` contient la mémoire, les registres et le compteur de programme (PC).  
- **Exécution** :  
  - Chargement des instructions depuis `hexa.txt` dans la mémoire.  
  - Décodage dynamique des instructions via une table de fonctions (add, sub, jmp, etc.).  
  - Mise à jour des registres, drapeaux et PC après chaque instruction.  

### Défis Techniques  
- **Gestion des Étiquettes** : Optimisation des recherches d'adresses avec des structures de données efficaces.  
- **Extension de Signe** : Correction de l'extension des valeurs 8 bits pour les instructions `ldb`/`ldw`.  
- **Synchronisation du PC** : Cohérence du compteur de programme lors des sauts et de l'exécution séquentielle.  

## Licence  
Ce projet est distribué sous licence [MIT](https://fr.wikipedia.org/wiki/Licence_MIT).  

## Auteurs  
- **Victor Cai**  
- **Louis De Domingo**  