# Carnet de Bord : Projet Cryptographie / Polynômes

---

<details>
<summary><b>Séance 1 - (13/03/26)</b></summary>

### A. Tâches à réaliser avant la séance
*(Non spécifié pour le démarrage du projet)*

### B. Tâches effectivement réalisées
- [x] **Équipe** : Implémentation de la classe `Anneau`.
- [x] **Équipe** : Représentation des polynômes (liste de coefficients).
- [x] **Équipe** : Instanciation des polynômes dans la classe `Anneau`.
> **Note** : La multiplication de polynômes restait à finaliser à l'issue de cette séance.

</details>

---

<details>
<summary><b>Séance 2 - (01/04/26)</b></summary>

### A. Tâches à réaliser avant la séance
#### Siméon
- [x] Faire le pseudo-code de l'algorithme d'Euclide étendu.
- [x] Préparer des tests pour la division euclidienne et Euclide étendu.

#### Jamyson
- [x] Coder la multiplication de polynômes en Haskell.
- [X] Faire le pseudo-code de la division euclidienne des polynômes.

### B. Tâches effectivement réalisées en séance
#### Siméon
- [x] Implémentation de l'algorithme d'Euclide étendu (pour les polynômes de Z/2Z).
- [x] Implémentation de l'inversion de polynômes.
- [x] Implémentation de la structure de corps, le type GF28 et la multiplication de ses élements (avec un modulo m)

#### Jamyson
- [x] Implémentation de la division euclidienne de polynômes.
- [x] Implémentation d'un affichage personnalisé (`Show`) pour les polynômes.

> **Note** : La résolution de quelques conflits lors du `pull` nous a permis de mieux prendre en main la gestion des fusions avec Git.

</details>

---

<details>
<summary><b>Séance 3 - (21/04/26)</b></summary>

## Séance 3 —

### Travail en autonomie (Hors séance)
- [x] **Jamyson** : Remodelage de la structure du projet (séparation du code en 7 fichiers distincts : `Anneau.hs`, `Z2.hs`, `Polynome.hs`, `GF28.hs`, `Euclide.hs`, `AES.hs`, `Main.hs`).

### A. Tâches à réaliser avant la séance
#### Siméon & Jamyson

- Travail commun
  - [x] Comprendre les 4 fonctions clés de l’algorithme AES, identifier les types demandés, retour et parametres attendus  
  - [x] Implémenter au moins une de ces fonctions  


### B. Tâches effectivement réalisées
#### Jamyson
- [x] **Implémentation AES** : Codage des transformations *State* : `subBytes`, `shiftRows` et `mixColumns`.
- [x] **Déchiffrement AES** : Implémentation des transformations inverses de l’état (`invAddRoundKey`, `invSubBytes`, `invShiftRows`, `invMixColumns`) et ajout de leurs tests de validation.
- [x] **Validation** : Tests unitaires exécutés en s’appuyant sur des exemples issus de la documentation.
- [x] **Refonte de la gestion des polynômes** :
    - **Correction de bug** : Suppression d’une boucle infinie dans `mixColumns`. La division euclidienne a été adaptée pour fonctionner non seulement sur $Z_2$, mais aussi sur les octets ($GF(2^8)$).
    - **Calculs plus fiables** : Amélioration de `de_pol`.
    - **Comparaison correcte** : Définition d’une instance `Eq` personnalisée.

> **Note** : Une version alternative de subBytes utilisant SBox.hs a été codée mais n'est pas utilisée.

#### Siméon

- [x] **Implémentation AES** : La fonction `addRoundKey`.
- [x] **Génération des clés** : Implémentation de `keyExpansion` et de `cipherKey` pour gérer le planning de clés.
- [x] **Interface de données** : Mise en place des fonctions de conversion (bits vers *State*) et d’affectation pour le traitement des blocs.
- [x] **Validation** : Création et exécution des tests unitaires associés aux nouvelles fonctionnalités.
- [x] **Plan** : Détermination des objectifs du projets, de la démarche future à suivre, étude avancée de la documentation.
- [x] **Restructuration du projet** : GF28X, State et l'Instanciation ont été repensés.


---

<details>
<summary><b>Séance 4 - (06/05/26)</b></summary> *A compléter*

### Travail en autonomie (Hors séance)

#### Siméon
- [x] **Refactorisation et nettoyage du code (`AES.hs`)** : Simplification de plusieurs fonctions (`subBytes`, `mixColumns`, `subWord`, `invSubBytes`, `invMixColumns`) en supprimant les variables intermédiaires superflues.
- [x] **Correction algébrique** : Ajustement de la multiplication polynomiale `mul_gf28x` (`take 4`) afin de garantir la bonne taille des polynômes retournés, ce qui corrige le comportement de `mixColumns`.
- [x] **Amélioration de l'affichage et du débogage** :
    - Refonte des instances `Show` dans `Z2.hs` et `Polynomes.hs` pour obtenir un affichage plus lisible (listes standards plutôt que `Z2 x`).
    - Ajout d’un module d’affichage hexadécimal dans `AES.hs` (`gf28ToHex`, `afficher_key`, `afficher_state`) pour faciliter la lecture des états et des clés.
- [x] **Initialisation des tests** : Création de la structure de base du script de validation globale `tests_chiffrement.ghci`.

#### Jamyson
- [x] **Complétion des tests et validation NIST** : Reprise et complétion du script `tests_chiffrement.ghci`.
    - Ajout de tests de validation automatisés pour valider le chiffrement complet (Forward Cipher), en s’appuyant sur les annexes B et C.1 du standard FIPS 197.
    - Ajout de tests d’involution pour vérifier la validité de chaque transformation inverse (déchiffrement).
- [x] **Débogage matriciel** : Identification et correction d’un problème de transposition dans `state_vers_octets`. La matrice est maintenant reconstruite correctement en lecture colonne par colonne (au lieu de ligne par ligne), ce qui a permis de valider définitivement les tests AES.


### A. Tâches à réaliser avant la séance
#### Siméon & Jamyson

- **Travail commun : Orchestration du Déchiffrement**
  - [x] Analyser la structure de l'algorithme *Equivalent Inverse Cipher* dans le standard NIST (ordre des transformations et des clés).
  - [ ] Implémenter la fonction principale `invCipher` dans `AES.hs` en s'appuyant sur les briques unitaires déjà validées.
  - [ ] Développer la boucle de déchiffrement inverse (`for_round_inv`) pour remonter les tours de 10 à 0.
  - [ ] Finaliser le script `tests_chiffrement.ghci` en ajoutant le test de "Round Trip" (vérifier que `invCipher(cipher(data)) == data`).


### B. Tâches effectivement réalisées
(*À remplir à la fin de notre séance*)


</details>
