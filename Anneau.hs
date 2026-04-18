module Anneau where

class Anneau a where
  zero :: a
  un :: a
  add :: a -> a -> a
  mul :: a -> a -> a

class (Anneau a) => Corps a where
  inv :: a -> a
