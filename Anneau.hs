class Anneau a where
    zero :: a
    un :: a
    add :: a -> a -> a
    mul :: a -> a -> a

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
    mul (Z2 a) (Z2 b) = Z2 $ a*b


data Polynome a = Polynome [a] deriving (Show, Eq) -- definit le type des polynomes de coefficients de type a :(a0,a1,a2...an) correspond au
-- polynome a0 + a1X+ a2X² ...

--Fonction utilitaire pour compléter les termes de degré manquant pour un polynome
{- completeListe :: (Anneau a )=> [a] ->Int-> [a]
completeListe l 0 = l
completeListe l n = completeListe (l++[zero]) (n-1)
-}

{- addp ::(Anneau a ) => Polynome a -> Polynome a -> Polynome a
addp (Polynome l1) (Polynome l2) | diff>0 = Polynome (addL l1 l2')
                                 | diff <0 = Polynome (addL l1' l2)
                                 | otherwise = Polynome (addL l1 l2)  
                                  where l2'= completeListe l2 diff; l1' = completeListe l1 (-diff); diff=(length l1) - (length l2) 
-}

addp :: (Anneau a) => Polynome a -> Polynome a -> Polynome a
addp (Polynome l1) (Polynome l2) = Polynome (addL l1 l2)

addL :: (Anneau a) => [a] -> [a] -> [a]
addL [] ys = ys
addL xs [] = xs
addL (x:xs) (y:ys) = (add x y) : addL xs ys


mulL::(Anneau a ) => [a]->[a]->[a]
mulL [] _ = []
mulL _ [] = []
--mulL (x:xs) (y:ys) = (mul x y) : mulL xs ys
mulL (x:xs) ys = addL (map (\y -> mul x y) ys) (zero : mulL xs ys)

mulp ::(Anneau a ) => Polynome a -> Polynome a -> Polynome a
{-mulp (Polynome l1) (Polynome l2) | diff>0 = Polynome (mulL l1 l2')
                                 | diff <0 = Polynome (mulL l1' l2)
                                 | otherwise = Polynome (mulL l1 l2)  
                                  where l2'= completeListe l2 diff; l1' = completeListe l1 (-diff); diff=(length l1) - (length l2) 
-}
mulp (Polynome l1) (Polynome l2) = Polynome (mulL l1 l2)

instance (Anneau a) => Anneau (Polynome a) where
    zero = Polynome [zero]
    un = Polynome [un]
    add = addp 
    mul = mulp




{--
type GF2X=Polynome Z2


class Poly a where
    zerop :: Polynome a
    unp:: Polynome a
    addp :: Polynome a -> Polynome a -> Polynome a
    mulp :: Polynome a -> Polynome a -> Polynome a


instance Poly Z2 where
    zerop = Polynome [zero]
    unp = Polynome [un]
    addp (Polynome [a:as]) (Polynome [b:bs]) = (add a b ) : (addp (Polynome as) (Polynome bs))
    mulp (Polynome [a:as]) (Polynome [b:bs]) = (mul a b ): (mulp (Polynome as) (Polynome bs))





--

--}

--Effectue le d.e du premier polynome(P) par le second(M), renvoie le couple (Q,R)
--de_pol::(Anneau a ) => Polynome a -> Polynome a -> (Polynome a, Polynome a)
--de_pol (Polynome P) (Polynome M) = 


--Calcule Euclide etendu pour des polynomes de Z2/Z


{--
Pseudo code Euclide etendu:

Entrée : a,b Polynomes de Z2
Sortie : r Polynome de Z2 et  u, v Polynomes de Z2 tels que r = pgcd(a, b) et r = a*u+b*v

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




{-
Pseudo-code division euclidienne de Polynomes

fonction de_pol(A, B):

    si  B == 0 :
        Erreur("Division par un polynôme nul impossible") -- On refuse immédiatement la division par le polynôme nul.

    
    Si A == 0 :
        Retourner (0, 0) -- si A est nul calcul deja fini


    dA = degre(A)
    dB = degre(B)


    Si dA < dB :
        Retourner (0, A)   on ne peut plus rien diviser donc q = 0 et r=A


    diff = dA - dB

    Q_actuel = creer_monome(diff)   -- X^diff


    B_decale = decaler_polynome(B, diff) --ajouter "diff" zéro au debut

    A_reduit = simplifier(A + B_decale) -- enleve les zeros créés à la fin


    (Q_suite, R_final) = de_pol(A_reduit, B)

    Q_final = Q_actuel + Q_suite

    Retourner (Q_final, R_final)

-}