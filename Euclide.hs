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

{-

type GF2X_8= Polynome Z2 -- ces polynomes sont de degres < a 8

m = Polynome [Z2 1,Z2 1,Z2 0,Z2 1,Z2 1,Z2 0,Z2 0,Z2 0,Z2 1]
instance Anneau GF2X_8 where
    zero = Polynome [zero]
    un = Polynome [un]
    add = addp
    mul = mulp -- apres c'est division euclidienne avec m et on prend le reste
-}

{--
Pseudo code Euclide etendu:

Entrée : a,b Polynomes de Z2
Sortie : (r,u,v) Polynome de Z2 et  u, v Polynomes de Z2 tels que r = pgcd(a, b) et r = a*u+b*v

Initialisation : r := a, r' := b, u := 1, v := 0, u' := 0, v' := 1
                  q  quotient entier
                  rs, us, vs  variables de stockage intermédiaires

les égalités r = a*u+b*v et r' = a*u'+b*v' sont des invariants de boucle

tant que (r' ≠ 0) faire
    q := r÷r'
    rs := r, us := u, vs := v,
    r := r', u := u', v := v',
    r' := rs - q*r', u' := us - q*u', v' := vs - q*v'
   fait
renvoyer (r, u, v)

(source wikipedia)

En Haskell:

Euclide (a,b)= initalisation puis appel de aux()

fonction aux: les ' représentent l'etat précédent de la variable (u_prec=u')
aux(r',r,u',u,v',v) | r==0 retourner (r',u',v')
                    | otherwise retourner aux(r',u',v') where (les variables changent une fois puis appel recursif pour check si r==0)

                    (q,reste)= de_p(r',r)
                    r' = r; r=reste
                    maj coeff Bezout: tempU= u; u=u'+ (q*u); u'=tempu
                                     tempV= v; v=v'+ (q*v); v'=tempu

--}

-- {-

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
