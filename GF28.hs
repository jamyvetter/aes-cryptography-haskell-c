{- HLINT ignore "Use newtype instead of data" -}
module GF28 where

import Anneau
import Polynome
import Z2
import qualified Anneau as GF28

type GF2X = Polynome Z2 -- raccourci/ allias pour les polynomes d'elements Z2

data GF28 = GF28 GF2X deriving (Show, Eq) -- definit un nouveau type avec constructeur pour les polynomes de Z/2Z de degré au plus 7.
--Cette structure est le corps GF(2⁸)

instance Anneau GF28 where
  zero = GF28 zero
  un = GF28 un
  add = add_gf28
  mul = mul_gf28

m :: Polynome Z2
m = Polynome [Z2 1, Z2 1, Z2 0, Z2 1, Z2 1, Z2 0, Z2 0, Z2 0, Z2 1]

add_gf28 :: GF28 -> GF28 -> GF28
add_gf28 (GF28 a) (GF28 b) = GF28 (add a b)

mul_gf28 :: GF28 -> GF28 -> GF28
mul_gf28 (GF28 a) (GF28 b) = GF28 r where (_, r) = de_pol (mul a b) m


--Maintenant, on peut définir l'Anneau GF28X == GF(2⁸)[X]: c'est l'anneau quotient GF(2⁸)/(X⁴+1) 
--Les éléments de cet anneau sont des polynômes d'éléments de GF28, 4 coeffs de type GF28(==octet)
data GF28X = GF28X (Polynome GF28) deriving (Show)

instance Anneau GF28X where
  zero= GF28X zero
  un = GF28X un
  add= add_gf28x
  mul=mul_gf28x

add_gf28x:: GF28X -> GF28X -> GF28X
add_gf28x (GF28X a) (GF28X b) = GF28X (add a b) -- on sort de l'anneau GF28X pour faire l'addition dans l'Anneau GF28

m_GF28X :: Polynome GF28 -- le polynome X^4 +1
m_GF28X = Polynome [un, zero,zero, zero, un]

mul_gf28x :: GF28X -> GF28X -> GF28X
mul_gf28x (GF28X a) (GF28X b) = GF28X r where (_, r) = de_pol (mul a b) m_GF28X

--Définition de State

type Colonne = GF28X -- une colonne est composée de 4 octets (donc 4 éléments de GF28), représenté sous forme de polynome. On utilise donc GF28X
type State = [Colonne]-- State: la matrice des colonnes, pour AES c'est 4*4


--Fonctions de chiffrement:
--Prend la matrice d'état, puis la clef de chiffrement et renvoie les nouveaux états
--fonctionnement: XOR case par case (donc d'abord entre éléments de type GF28X puis entre éléments de GF28)
addRoundKey:: State->State->State
addRoundKey [] [] = []
addRoundKey (s:ss) (k:ks) = add_gf28x s k: addRoundKey ss ks --techniquement add_gf28x=add mais comme ça on voit que c'est de elements de GF28X

--subBytes::State->State
--shiftRows::State->State
--mixColumns::State->State