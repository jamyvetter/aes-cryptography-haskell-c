module GF28 where

import Anneau
import Polynome
import Z2

type GF2X = Polynome Z2 -- raccourci/ allias pour les polynomes d'elements Z2

data GF28 = GF28 GF2X deriving (Show, Eq) -- definit un nouveau type avec constructeur pour les polynomes de Z/2Z de degré au plus 7.
-- Cette structure est le corps GF(2⁸)

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
