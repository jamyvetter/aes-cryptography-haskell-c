module Z2 where

import Anneau

instance Anneau Integer where
  zero = 0
  un = 1
  add = (+)
  mul = (*)

data Z2 = Z2 Integer deriving (Eq)

instance Show Z2 where 
  show= show_z2

show_z2:: Z2->String
show_z2 (Z2 x) = show x
--de même ici, on apprend à affiche un Z2 sans écrire la notation Z2 x, ce qui simplifie pour l'affichage des polynomes

instance Anneau Z2 where
  zero = Z2 0
  un = Z2 1
  add (Z2 a) (Z2 b) = Z2 $ mod (a + b) 2
  mul (Z2 a) (Z2 b) = Z2 $ a * b
