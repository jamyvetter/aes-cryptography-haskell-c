module Anneau where

-- Classe définissant la structure algébrique d'Anneau

class Anneau a where
  zero :: a
  un :: a
  add :: a -> a -> a
  sub :: a -> a -> a
  mul :: a -> a -> a

class (Anneau a) => Corps a where
  inv :: a -> a
