module Z3 where

import Anneau

data Z3 = Z3 Integer deriving (Eq)

instance Show Z3 where
  show (Z3 x) = show x

instance Anneau Z3 where
  zero = Z3 0
  un = Z3 1
  add (Z3 a) (Z3 b) = Z3 $ (a + b) `mod` 3
  sub (Z3 a) (Z3 b) = Z3 $ (a - b) `mod` 3
  mul (Z3 a) (Z3 b) = Z3 $ (a * b) `mod` 3

instance Corps Z3 where
  inv (Z3 1) = Z3 1
  inv (Z3 2) = Z3 2
  inv _ = error "pas d'inverse pour zero dans Z3"