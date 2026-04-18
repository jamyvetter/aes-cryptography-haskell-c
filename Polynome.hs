module Polynome where

import Anneau

data Polynome a = Polynome [a] deriving (Eq) -- definit le type des polynomes de coefficients de type a :(a0,a1,a2...an) correspond au
-- polynome a0 + a1X+ a2X² ...

affichage_polynome :: (Anneau a, Eq a) => Polynome a -> [Int]
affichage_polynome (Polynome liste) = map versInt liste
  where
    versInt x
      | x == un = 1
      | otherwise = 0

instance (Anneau a, Eq a) => Show (Polynome a) where
  show p = show (affichage_polynome p)

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

{-mulp (Polynome l1) (Polynome l2) | diff>0 = Polynome (mulL l1 l2')
                                 | diff <0 = Polynome (mulL l1' l2)
                                 | otherwise = Polynome (mulL l1 l2)
                                  where l2'= completeListe l2 diff; l1' = completeListe l1 (-diff); diff=(length l1) - (length l2)
-}

-- On met les elements de type Polynome a dans la classe Anneau, avec les operations associées
instance (Anneau a) => Anneau (Polynome a) where
  zero = Polynome [zero]
  un = Polynome [un]
  add = addp
  mul = mulp

-- Calcule le degré d'un polynome
degre :: (Anneau a, Eq a) => Polynome a -> Int
degre (Polynome l) = simeon (reverse l) (length l - 1)
  where
    simeon [] _ = -1
    simeon (x : xs) d
      | x == zero = simeon xs (d - 1)
      | otherwise = d

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

    -- q_actuel = X^diff
    qActuel = Polynome (replicate diff zero ++ [un])

    -- b_decale = B * X^diff (on ajoute 'diff' zéros au début de la liste)
    (Polynome listB) = b
    bDecale = Polynome (completeListe listB diff)

    -- Dans Z2, l'addition de a et bDecale équivaut à une soustraction, ce qui annule le terme de plus haut degré
    aReduit = add a bDecale

    -- Appel récursif sur le reste
    (qSuite, rFinal) = de_pol aReduit b
