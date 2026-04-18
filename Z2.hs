module Z2 where

import Anneau

instance Anneau Integer where
  zero = 0
  un = 1
  add = (+)
  mul = (*)

data Z2 = Z2 Integer deriving (Show, Eq)

instance Anneau Z2 where
  zero = Z2 0
  un = Z2 1
  add (Z2 a) (Z2 b) = Z2 $ mod (a + b) 2
  mul (Z2 a) (Z2 b) = Z2 $ a * b
