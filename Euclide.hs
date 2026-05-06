module Euclide where

import Anneau
import GF28
import Polynome
import Z2

instance Corps GF28 where
  inv (GF28 p) = GF28 (inv_p p m)

-- Fonction d'inversion de polynomes modulo un polynome m
inv_p :: (Corps a, Eq a) => Polynome a -> Polynome a -> Polynome a
inv_p p m = u_norm
  where
    (pgcd, u, _) = euclide (p, m)
    (Polynome l) = pgcd
    l_net = nettoyer l
    c = if null l_net then un else last l_net -- le scalaire est le dernier element si non vide
    u_norm = mul u (Polynome [inv c])

-- N.B: On pourrait changer le type et mettre ::GF28->Gf28 car les polynomes qu'on veut inverser sont ceux du corps GF28 (sauf zero)

-- Prend deux polynômes A,B et renvoie les polynomes d'Euclide etendu: (pgcd, U, V)
euclide :: (Corps a, Eq a) => (Polynome a, Polynome a) -> (Polynome a, Polynome a, Polynome a)
euclide (a, b) = (r, u, v)
  where
    (r, _, u, _, v, _) = aux (a, b, un, zero, zero, un) -- fait l'initialisation

aux :: (Corps a, Eq a) => (Polynome a, Polynome a, Polynome a, Polynome a, Polynome a, Polynome a) -> (Polynome a, Polynome a, Polynome a, Polynome a, Polynome a, Polynome a)
aux (r', r, u', u, v', v)
  | r == zero = (r', r, u', u, v', v) -- on revoie tous les coeffs, les primes seront utilises par euclide
  | otherwise = aux (r_prec, reste, u_prec, u_nv, v_prec, v_nv)
  where
    (q, reste) = de_pol r' r -- de_p(r',r)
    r_prec = r
    tempu = u
    u_nv = sub u' (mul q u)
    u_prec = tempu
    tempv = v
    v_nv = sub v' (mul q v)
    v_prec = tempv
