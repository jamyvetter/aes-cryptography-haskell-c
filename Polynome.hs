module Polynome where

import Anneau

data Polynome a = Polynome [a] -- definit le type des polynomes de coefficients de type a :(a0,a1,a2...an) correspond au
-- polynome a0 + a1X+ a2X² ...

-- on modifie l'egalite pour enlever les zeros de tete
instance (Anneau a, Eq a) => Eq (Polynome a) where
  (Polynome l1) == (Polynome l2) = nettoyer l1 == nettoyer l2

-- enleve les zeros de tete
nettoyer :: (Anneau a, Eq a) => [a] -> [a]
nettoyer l = reverse $ dropWhile (== zero) $ reverse l

{-
affichage_polynome :: (Anneau a, Eq a) => Polynome a -> [Int]
affichage_polynome (Polynome liste) = map versInt liste
  where
    versInt x
      | x == un = 1
      | otherwise = 0

instance (Anneau a, Eq a) => Show (Polynome a) where
  show p = show (affichage_polynome p)
-}
--NB: A supprimer à long terme, il y avait un problème d'affichage sur les GF28X qui mettaient que des zéros


instance Show a => Show (Polynome a) where
  show = show_p

show_p :: Show a => Polynome a -> String
show_p (Polynome liste) = show liste
--On utilise le fait que les listes savent s'afficher avec show, donc on a juste à retirer
--le constructeur pour que la liste s'affiche

addp :: (Anneau a) => Polynome a -> Polynome a -> Polynome a
addp (Polynome l1) (Polynome l2) = Polynome (addL l1 l2)

addL :: (Anneau a) => [a] -> [a] -> [a]
addL [] ys = ys
addL xs [] = xs
addL (x : xs) (y : ys) = (add x y) : addL xs ys

mulL :: (Anneau a) => [a] -> [a] -> [a]
mulL [] _ = []
mulL _ [] = []
mulL (x : xs) ys = addL (map (\y -> mul x y) ys) (zero : mulL xs ys)

mulp :: (Anneau a) => Polynome a -> Polynome a -> Polynome a
mulp (Polynome l1) (Polynome l2) = Polynome (mulL l1 l2)

-- On met les elements de type Polynome a dans la classe Anneau, avec les operations associées
instance (Anneau a) => Anneau (Polynome a) where
  zero = Polynome [zero]
  un = Polynome [un]
  add = addp
  mul = mulp

-- Calcule le degré d'un polynome
degre :: (Anneau a, Eq a) => Polynome a -> Int
degre (Polynome l) =
  let l' = nettoyer l
   in if null l' then -1 else length l' - 1

completeListe :: (Anneau a) => [a] -> Int -> [a]
completeListe l 0 = l
completeListe l n = completeListe ([zero] ++ l) (n - 1)

-- Effectue la division euclidienne de polynomes
de_pol :: (Anneau a, Eq a) => Polynome a -> Polynome a -> (Polynome a, Polynome a)
de_pol a b
  | degre b == -1 = error "Division par un polynôme nul impossible"
  | degre a == -1 = (zero, zero)
  | dA < dB = (zero, a)
  | otherwise = (add qActuel qSuite, rFinal)
  where
    dA = degre a
    dB = degre b
    diff = dA - dB

    -- on récupère le coefficient de plus haut degré de A
    (Polynome listA) = a
    coeffA = listA !! dA

    -- le quotient prend ce coefficient
    qActuel = Polynome (replicate diff zero ++ [coeffA])

    -- on utilise la multiplication de l'anneau pour décaler ET mettre à l'échelle B
    bDecale = mul b qActuel

    -- l'addition (XOR) de a et bDecale annule le terme de plus haut degré
    aReduit = add a bDecale

    -- appel récursif sur le reste
    (qSuite, rFinal) = de_pol aReduit b
