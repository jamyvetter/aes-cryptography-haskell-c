module Euclide where

import Anneau
import GF28
import Polynome
import Z2

instance Corps GF28 where
  inv (GF28 p) = GF28 (inv_p p)

-- Fonction d'inversion de polynomes
inv_p :: GF2X -> GF2X
inv_p p = u where (pgcd, u, v) = euclide (p, m)
--N.B: On pourrait changer le type et mettre ::GF28->Gf28 car les polynomes qu'on veut inverser sont ceux du corps GF28 (sauf zero)

-- Prend deux polynômes A,B et renvoie les polynomes d'Euclide etendu: (pgcd, U, V)
euclide :: (GF2X, GF2X) -> (GF2X, GF2X, GF2X)
euclide (a, b) = (r, u, v)
  where
    (r, _, u, _, v, _) = aux (a, b, un, zero, zero, un) -- fait l'initialisation

aux :: (GF2X, GF2X, GF2X, GF2X, GF2X, GF2X) -> (GF2X, GF2X, GF2X, GF2X, GF2X, GF2X)
aux (r', r, u', u, v', v)
  | r == zero = (r', r, u', u, v', v) -- on revoie tous les coeffs, les primes seront utilises par euclide
  | otherwise = aux (r_prec, reste, u_prec, u_nv, v_prec, v_nv)
  where
    (q, reste) = de_pol r' r -- de_p(r',r)
    r_prec = r
    tempu = u
    u_nv = add u' (mul q u)
    u_prec = tempu
    tempv = v
    v_nv = add v' (mul q v)
    v_prec = tempv
