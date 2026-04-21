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

## Séance 3 — *À compléter*

### Travail en autonomie (Hors séance)
- [x] **Jamyson** : Remodelage de la structure du projet (séparation du code en 7 fichiers distincts : `Anneau.hs`, `Z2.hs`, `Polynome.hs`, `GF28.hs`, `Euclide.hs`, `AES.hs`, `Main.hs`).

### A. Tâches à réaliser avant la séance
#### Siméon & Jamyson

- Travail commun
  - [ ] Comprendre les 4 fonctions clés de l’algorithme AES, identifier les types demandés, retour et parametres attendus  
  - [ ] Implémenter au moins une de ces fonctions  



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


