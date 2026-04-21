{- HLINT ignore "Use camelCase" -}
module AES where

import Anneau
import Euclide
import GF28
import Polynome
-- import SBox
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

-- Fonction GF28 vers Entier
gf28ToInt :: GF28 -> Int
gf28ToInt (GF28 (Polynome coeffs)) = sum [fromIntegral val * (2 ^ i) | (Z2 val, i) <- zip coeffs [0 ..]]

-- Fonction Entier vers GF28
intToGf28 :: Int -> GF28
intToGf28 n = GF28 (Polynome (toZ2List n 8))
  where
    toZ2List _ 0 = []
    toZ2List x count = Z2 (toInteger (x `mod` 2)) : toZ2List (x `div` 2) (count - 1)

-- SUBBYTES

-- calcule inverse multiplicatif de l'octet (polynome dans GF28)
inv_gf28 :: GF28 -> GF28
inv_gf28 octet
  | octet == zero = zero -- inverse de {00} est {00}
  | otherwise = inv octet -- utilise la fonction inv_p de Euclide.hs

-- transformation affine
-- bi' = bi + b(i+4) [modulo 8] + b(i+5) [modulo 8] + b(i+6) [modulo 8] + b(i+7) [modulo 8] + ci
-- c = {63} = 01100011 ([c7,c6,c5,c4,c3,c2,c1,c0] pour nous)

sub_affine :: GF28 -> GF28
sub_affine (GF28 p) = GF28 (Polynome b')
  where
    -- la matrice est circulante, l'opération est une multiplication polynomiale modulo X^8 + 1 suivie d'une addition de c.
    a = Polynome [un, un, un, un, un]
    c = Polynome [un, un, zero, zero, zero, un, un, zero]
    m8 = Polynome [un, zero, zero, zero, zero, zero, zero, zero, un] -- X^8 + 1
    (_, r) = de_pol (mul p a) m8
    (Polynome coeffs_rc) = add r c

    b' = take 8 (coeffs_rc ++ repeat zero)

-- applique inverse puis transfo affine a un octet

subByte :: GF28 -> GF28
subByte octet = sub_affine (inv_gf28 octet)

-- applique subByte a chaque octet de la colonne
subBytesCol :: Colonne -> Colonne
subBytesCol (GF28X (Polynome [a0, a1, a2, a3])) = GF28X (Polynome [subByte a0, subByte a1, subByte a2, subByte a3])

-- applique SubBtesCOL a chaque colonne du state

subBytes :: State -> State
subBytes state = map subBytesCol state

-- SHIFTROWS

-- décale circulairement les lignes de la matrice 4x4
shiftRows :: State -> State
shiftRows [c0, c1, c2, c3] = [nouvelle_c0, nouvelle_c1, nouvelle_c2, nouvelle_c3]
  where
    -- on déballe nos 4 colonnes pour accéder aux 16 octets
    (GF28X (Polynome [s00, s10, s20, s30])) = c0
    (GF28X (Polynome [s01, s11, s21, s31])) = c1
    (GF28X (Polynome [s02, s12, s22, s32])) = c2
    (GF28X (Polynome [s03, s13, s23, s33])) = c3

    -- on applique la règle de décalage des lignes
    nouvelle_c0 = GF28X (Polynome [s00, s11, s22, s33])
    nouvelle_c1 = GF28X (Polynome [s01, s12, s23, s30])
    nouvelle_c2 = GF28X (Polynome [s02, s13, s20, s31])
    nouvelle_c3 = GF28X (Polynome [s03, s10, s21, s32])
shiftRows state = state -- si le state ne fait pas 4 colonnes on fait rien

{-
VERSION ALTERNATIVE AVEC SBOX.hs

--  applique la sbox à un octet
subByte :: GF28 -> GF28
subByte octet = intToGf28 valeurSubstituee
  where
    index = gf28ToInt octet
    valeurSubstituee = sbox !! index

-- applique une fonction à chaque octet de la colonne
mapGF28X :: (GF28 -> GF28) -> Colonne -> Colonne
mapGF28X f (GF28X (Polynome coeffs)) = GF28X (Polynome (map f coeffs))

-- applique la fonction subByte au state ([Colonne])
subBytes :: State -> State
subBytes state = map (mapGF28X subByte) state

-}

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
subWord :: Colonne -> Colonne
subWord col = subBytesCol col

-- la constante de tour (Rcon)
-- rconList !! i donne le Rcon du tour i
rconList :: [GF28X]
rconList =
  [ -- Rcon n'est pas utilisé dans l'algorithme (l'index commence à 1)
    GF28X (Polynome [intToGf28 0x00, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x01, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x02, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x04, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x08, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x10, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x20, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x40, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x80, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x1B, zero_gf28, zero_gf28, zero_gf28]),
    GF28X (Polynome [intToGf28 0x36, zero_gf28, zero_gf28, zero_gf28])
  ]
  where
    zero_gf28 = intToGf28 0x00


octets_vers_state:: [GF28]->[GF28X]-- prend b_in qui contient 16 GF28 et donne State (p9), pour le code en C il y aura une formule avec les indices
octets_vers_state [a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p] = [GF28X (Polynome [a,b,c,d]),GF28X (Polynome [e,f,g,h]) ,GF28X (Polynome [i,j,k,l]) ,GF28X (Polynome [m,n,o,p])]


state_vers_octets :: [GF28X] -> [GF28] -- inverse de octets_vers_state
state_vers_octets [c0, c1, c2, c3] = [s00,s01,s02,s03,s10,s11,s12,s13,s20,s21,s22,s23,s30,s31,s32,s33]
  where
    -- on déballe nos 4 colonnes pour accéder aux 16 octets
    (GF28X (Polynome [s00, s10, s20, s30])) = c0
    (GF28X (Polynome [s01, s11, s21, s31])) = c1
    (GF28X (Polynome [s02, s12, s22, s32])) = c2
    (GF28X (Polynome [s03, s13, s23, s33])) = c3


--Note: dans pseudo code y'a une variable out qui ici est le retour de cette fonction
--J'ai fait les mêmes types que dans le pseudo code mais peut être juste State sans les traductions est plus simple
--cipher dépend de nr et j'ai fixé Nb=4

cipher:: Int->[GF28]-> [GF28X] -> [GF28]
cipher nr b_in w= final where state0= octets_vers_state b_in; state1=addRoundKey state0 (intervale_4 0 w); state2= for_round 1 nr state1 w; state3=addRoundKey ((shiftRows . subBytes) state2) (intervale_4 (nr*4) w) ;final=state_vers_octets state3

for_round ::Int->Int-> State -> State->State
for_round round nr state w = if nr==round then state else for_round (round+1) nr state' w where state'= addRoundKey (mixColumns ( shiftRows ( subBytes state))) (intervale_4 round w)

--ordre du cypher: bit octets_vers_state donne 'job'
--faire bob= addRoundKey job (intervale_4 0 w)
--faire la boucle for sur bob : alex= for_round 1 nr bob
--faire tom=  addRoundKey ((shiftRows . subBytes) alex) (intervale_4 nr*nb)
--faire le resultat qui vaut sate_vers_bytes tom


--il faut une fonction qui recupère l'intervalle demandée d'une liste. On utilise drop et take:
-- Prend les 4 premiers elements d'une liste à partir de l'indice i*4
intervale_4 :: Int -> [a] -> [a]
intervale_4 i l = take 4 (drop (i*4) l)


--  key expansion
{-
Code de keyExpansion adapté de la documantation p20
Nr: Nombre de ronde, Nr=f(Nk,Nb) Nr=10,12 ou 14
Nb: nb de colonnes (==nombre de GF28X) dans State fixé ici Nb=4 
Nk:nombre de GF28X dans la Cipher Key. ici, Nk=4,6 ou 8
byte:octet, donc un GF28
word: 4 octets(32-bit), donc un GF28X
State=[GF28X]

key::[GF28]
w::[GF28X]

Accéder à un élément en position i: l !! i
-}

--fonction d'affectation (change l[i]=elem et renvoie la liste modifiée), doit exister qq part-> oui avec drop et take
af::[l]->Int->l->[l]
af liste i elem= take i liste ++ [elem] ++ drop (i+1) liste

--KeyExpansion(byte key[4*Nk] , Nk,Nr) -> word w[Nb*(Nr+1)]
keyExpansion:: [GF28] -> Int ->Int -> [GF28X]
keyExpansion key nk nr = wfinal where w1= f1 0 nk key []; wfinal=g1 nk nk nr w1 ;


--f1(i,Nk,key)--pour coder le premier while, modifie notre résultat w et le renvoie
f1:: Int-> Int-> [GF28] ->[GF28X] -> [GF28X]
f1 i nk key w= if i>=nk then w else f1 (i+1) nk key w'
        where w'=af w i (GF28X (Polynome [ key !! (4*i),key !! (4*i+1),key !! (4*i+2),key !! (4*i+3)]))
--(key !! (4*i)) lui est de type GF28, on prend des elements de GF28 du tableau key et on en fait un polynome qu'on place dans w à l'indice i

--fait le deuxieme while
g1:: Int->Int->Int->[GF28X]-> [GF28X]
g1 i nk nr w= if i<4*(nr+1) then f2 i nk nr w temp else w where temp= w !! (i-1)

--f2::  --modifie w apres le while
f2:: Int->Int->Int->[GF28X]->GF28X -> [GF28X]
f2 i nk nr w temp = g1 (i+1) nk nr w' where temp'= if (mod i nk ==0) then add (subWord (rotWord temp)) (rconList !! (div i nk))else if (nk>6 && ((mod i nk)==4 )) then subWord temp  else temp;w'=af w i (add (w !! (i-nk)) temp') --fait la ligne avant i++s




-- Transformations inverses :

invShiftRows :: State -> State
invShiftRows [c0, c1, c2, c3] = [nouvelle_c0, nouvelle_c1, nouvelle_c2, nouvelle_c3]
  where
    -- on déballe nos 4 colonnes pour accéder aux 16 octets
    (GF28X (Polynome [s00, s10, s20, s30])) = c0
    (GF28X (Polynome [s01, s11, s21, s31])) = c1
    (GF28X (Polynome [s02, s12, s22, s32])) = c2
    (GF28X (Polynome [s03, s13, s23, s33])) = c3

    -- on applique la décalage inverse
    nouvelle_c0 = GF28X (Polynome [s00, s13, s22, s31])
    nouvelle_c1 = GF28X (Polynome [s01, s10, s23, s32])
    nouvelle_c2 = GF28X (Polynome [s02, s11, s20, s33])
    nouvelle_c3 = GF28X (Polynome [s03, s12, s21, s30])
invShiftRows state = state -- si le state ne fait pas 4 colonnes on fait rien
