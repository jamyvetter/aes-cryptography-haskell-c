module AES where

import Anneau
import Euclide
import GF28
import Polynome
import SBox
import Z2

-- Maintenant, on peut définir l'Anneau GF28X == GF(2⁸)[X]: c'est l'anneau quotient GF(2⁸)/(X⁴+1)
-- Les éléments de cet anneau sont des polynômes d'éléments de GF28, 4 coeffs de type GF28(==octet)
data GF28X = GF28X (Polynome GF28) deriving (Show, Eq)

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

-- Transformations

-- SUBBYTES

-- Fonction GF28 vers Entier
gf28ToInt :: GF28 -> Int
gf28ToInt (GF28 (Polynome coeffs)) = sum [fromIntegral val * (2 ^ i) | (Z2 val, i) <- zip coeffs [0 ..]]

-- Fonction Entier vers GF28
intToGf28 :: Int -> GF28
intToGf28 n = GF28 (Polynome (toZ2List n 8))
  where
    toZ2List _ 0 = []
    toZ2List x count = Z2 (toInteger (x `mod` 2)) : toZ2List (x `div` 2) (count - 1)

--  applique la sbox à un octet
subByte :: GF28 -> GF28
subByte octet = intToGf28 valeurSubstituee
  where
    index = gf28ToInt octet
    valeurSubstituee = sbox !! index

-- applique la fonction subByte à chaque octet de la colonne
mapGF28X :: (GF28 -> GF28) -> GF28X -> GF28X
mapGF28X f (GF28X (Polynome coeffs)) = GF28X (Polynome (map f coeffs))

-- applique la fonction subByte au state ([Colonne])
subBytes :: State -> State
subBytes state = map (mapGF28X subByte) state

-- SHIFTROWS

-- décale circulairement les lignes de la matrice 4x4
shiftRows :: State -> State
shiftRows [c0, c1, c2, c3] = [nouvelle_c0, nouvelle_c1, nouvelle_c2, nouvelle_c3]
  where
    -- 1. on déballe nos 4 colonnes pour accéder aux 16 octets
    (GF28X (Polynome [s00, s10, s20, s30])) = c0
    (GF28X (Polynome [s01, s11, s21, s31])) = c1
    (GF28X (Polynome [s02, s12, s22, s32])) = c2
    (GF28X (Polynome [s03, s13, s23, s33])) = c3

    -- 2. on applique la règle de décalage des lignes
    nouvelle_c0 = GF28X (Polynome [s00, s11, s22, s33])
    nouvelle_c1 = GF28X (Polynome [s01, s12, s23, s30])
    nouvelle_c2 = GF28X (Polynome [s02, s13, s20, s31])
    nouvelle_c3 = GF28X (Polynome [s03, s10, s21, s32])
shiftRows state = state -- si le state ne fait pas 4 colonnes on fait rien

-- MIXCOLUMNS

-- le polynôme multiplicateur fixe pour MixColumns
-- a(x) = {03}x^3 + {01}x^2 + {01}x + {02}

a_mix :: GF28X
a_mix = GF28X (Polynome [intToGf28 0x02, intToGf28 0x01, intToGf28 0x01, intToGf28 0x03])

-- multiplie modulo m_GF28X (X^4 +1) chaque colonne par a_mix
mixColumns :: State -> State
mixColumns state = map (\col -> a_mix `mul` col) state

-- KEY EXPANSION

-- prend un mot (une colonne GF28X) et fait une rotation vers le haut
rotWord :: GF28X -> GF28X
rotWord (GF28X (Polynome [a0, a1, a2, a3])) = GF28X (Polynome [a1, a2, a3, a0])
rotWord col = col -- Sécurité

-- applique la S-Box à chaque octet du mot
subWord :: GF28X -> GF28X
subWord col = mapGF28X subByte col

-- la constante de tour (Rcon) pour briser les symétries lors de l'expansion de clé.
-- rconList !! i donne le Rcon du tour i (de 1 à 10 pour AES-128).
rconList :: [GF28X]
rconList =
  [ -- Rcon n'est pas utilisé dans l'algorithme (l'index commence à 1)
    GF28X (Polynome [intToGf28 0x00, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x01, zero_gf28, zero_gf28, zero_gf28]), -- Tour 1
    GF28X (Polynome [intToGf28 0x02, zero_gf28, zero_gf28, zero_gf28]), -- Tour 2
    GF28X (Polynome [intToGf28 0x04, zero_gf28, zero_gf28, zero_gf28]), -- Tour 3
    GF28X (Polynome [intToGf28 0x08, zero_gf28, zero_gf28, zero_gf28]), -- Tour 4
    GF28X (Polynome [intToGf28 0x10, zero_gf28, zero_gf28, zero_gf28]), -- Tour 5
    GF28X (Polynome [intToGf28 0x20, zero_gf28, zero_gf28, zero_gf28]), -- Tour 6
    GF28X (Polynome [intToGf28 0x40, zero_gf28, zero_gf28, zero_gf28]), -- Tour 7
    GF28X (Polynome [intToGf28 0x80, zero_gf28, zero_gf28, zero_gf28]), -- Tour 8
    GF28X (Polynome [intToGf28 0x1b, zero_gf28, zero_gf28, zero_gf28]), -- Tour 9
    GF28X (Polynome [intToGf28 0x36, zero_gf28, zero_gf28, zero_gf28]) -- Tour 10
  ]
  where
    zero_gf28 = intToGf28 0x00
