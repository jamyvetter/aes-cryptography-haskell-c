module AES where

import Anneau
import Euclide
import GF28
import Polynome
import Z2

-- Maintenant, on peut définir l'Anneau GF28X == GF(2⁸)[X]: c'est l'anneau quotient GF(2⁸)/(X⁴+1)
-- Les éléments de cet anneau sont des polynômes d'éléments de GF28, 4 coeffs de type GF28(==octet)
data GF28X = GF28X (Polynome GF28) deriving (Show)

instance Anneau GF28X where
  zero = GF28X zero
  un = GF28X un
  add = add_gf28x
  mul = mul_gf28x

add_gf28x :: GF28X -> GF28X -> GF28X
add_gf28x (GF28X a) (GF28X b) = GF28X (add a b) -- on sort de l'anneau GF28X pour faire l'addition des polynomes.

m_GF28X :: Polynome GF28 -- le polynome X^4 +1
m_GF28X = Polynome [un, zero, zero, zero, un]

mul_gf28x :: GF28X -> GF28X -> GF28X
mul_gf28x (GF28X a) (GF28X b) = GF28X r where (_, r) = de_pol (mul a b) m_GF28X

-- Définition de State

type Colonne = GF28X -- une colonne est composée de 4 octets (donc 4 éléments de GF28), représenté sous forme de polynome. On utilise donc GF28X

type State = [Colonne] -- State: la matrice des colonnes, pour AES c'est 4*4

-- Fonctions de chiffrement:
-- Prend la matrice d'état, puis la clef de chiffrement et renvoie les nouveaux états
-- fonctionnement: XOR case par case (donc d'abord entre éléments de type GF28X puis entre éléments de GF28)
addRoundKey :: State -> State -> State
addRoundKey [] [] = []
addRoundKey (s : ss) (k : ks) = add_gf28x s k : addRoundKey ss ks -- techniquement add_gf28x=add mais comme ça on voit que c'est de elements de GF28X

-- subBytes::State->State
-- shiftRows::State->State
-- mixColumns::State->State
