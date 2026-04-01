{- HLINT ignore "Use camelCase" -}
{- HLINT ignore "Use newtype instead of data" -}
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


data Polynome a = Polynome [a] deriving (Eq) -- definit le type des polynomes de coefficients de type a :(a0,a1,a2...an) correspond au
-- polynome a0 + a1X+ a2X² ...

affichage_polynome :: (Anneau a, Eq a) => Polynome a -> [Int]
affichage_polynome (Polynome liste) = map versInt liste
  where
    versInt x
        | x == un   = 1
        | otherwise = 0

instance (Anneau a, Eq a) => Show (Polynome a) where 
    show p = show (affichage_polynome p)




addp :: (Anneau a) => Polynome a -> Polynome a -> Polynome a
addp (Polynome l1) (Polynome l2) = Polynome (addL l1 l2)

addL :: (Anneau a) => [a] -> [a] -> [a]
addL [] ys = ys
addL xs [] = xs
addL (x:xs) (y:ys) = (add x y) : addL xs ys


mulL::(Anneau a ) => [a]->[a]->[a]
mulL [] _ = []
mulL _ [] = []
mulL (x:xs) ys = addL (map (\y -> mul x y) ys) (zero : mulL xs ys)

mulp ::(Anneau a ) => Polynome a -> Polynome a -> Polynome a
mulp (Polynome l1) (Polynome l2) = Polynome (mulL l1 l2)
{-mulp (Polynome l1) (Polynome l2) | diff>0 = Polynome (mulL l1 l2')
                                 | diff <0 = Polynome (mulL l1' l2)
                                 | otherwise = Polynome (mulL l1 l2)  
                                  where l2'= completeListe l2 diff; l1' = completeListe l1 (-diff); diff=(length l1) - (length l2) 
-}

--On met les elements de type Polynome a dans la classe Anneau, avec les operations associées
instance (Anneau a) => Anneau (Polynome a) where
    zero = Polynome [zero]
    un = Polynome [un]
    add = addp 
    mul = mulp



type GF2X=Polynome Z2 -- raccourci/ allias pour les polynomes d'elements Z2

data GF28 = GF28 GF2X deriving (Show, Eq)-- definit un nouveau type avec constructeur pour les polynomes de Z/2Z de degré 7.

instance Anneau GF28 where
    zero= GF28 zero
    un = GF28 un
    add= add_gf28
    mul = mul_gf28 


m :: Polynome Z2
m = Polynome [Z2 1,Z2 1,Z2 0,Z2 1,Z2 1,Z2 0,Z2 0,Z2 0,Z2 1]

add_gf28:: GF28 -> GF28 -> GF28
add_gf28 (GF28 a) (GF28 b)= GF28 (add a b)

mul_gf28 :: GF28 -> GF28 -> GF28
mul_gf28 (GF28 a) (GF28 b)= GF28 r where (_,r)= (de_pol (mul a b) m )


class Anneau a => Corps a where
    inv :: a->a

instance Corps GF2X where
    inv = inv_p


--Fonction d'inversion de polynomes, quand ceux-ci sont premiers avec m
inv_p :: GF2X-> GF2X
inv_p p= u where (pgcd,u,v)= euclide (p ,m) 


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

--{-

-- Prend deux polynômes A,B et renvoie les polynomes d'Euclide etendu: (pgcd, U, V)
euclide:: (GF2X,GF2X) -> (GF2X,GF2X,GF2X)
euclide (a,b) = (r,u,v) where
                                (r,_,u,_,v,_)= aux (a,b,un,zero,zero,un) -- fait l'initialisation


aux :: (GF2X,GF2X,GF2X,GF2X,GF2X,GF2X)->(GF2X,GF2X,GF2X,GF2X,GF2X,GF2X)
aux (r',r,u',u,v',v)| r==zero = (r',r,u',u,v',v) -- on revoie tous les coeffs, les primes seront utilises par euclide
                    | otherwise = aux (r_prec,reste,u_prec,u_nv,v_prec,v_nv) where
                    
                    (q,reste)= de_pol r' r--de_p(r',r)
                    r_prec = r;
                    tempu= u; u_nv= add u' (mul q u); u_prec=tempu
                    tempv= v; v_nv= add v' (mul q v); v_prec=tempv

--Calcule le degré d'un polynome
degre :: (Anneau a, Eq a) => Polynome a -> Int
degre (Polynome l) = simeon (reverse l) (length l - 1)
  where
    simeon [] _ = -1
    simeon (x:xs) d
        | x == zero = simeon xs (d - 1)
        | otherwise = d


completeListe :: (Anneau a )=> [a] ->Int-> [a]
completeListe l 0 = l
completeListe l n = completeListe ([zero]++l) (n-1)

--Effectue la division euclidienne de polynomes
de_pol :: (Anneau a, Eq a) => Polynome a -> Polynome a -> (Polynome a, Polynome a)
de_pol a b 
    | degre b == -1 = error "Division par un polynôme nul impossible"
    | degre a == -1 = (zero, zero)
    | dA < dB       = (zero, a)
    | otherwise     = (add qActuel qSuite, rFinal)
  where
    dA = degre a
    dB = degre b
    diff = dA - dB
    
    -- q_actuel = X^diff
    qActuel = Polynome (replicate diff zero ++ [un])
    
    -- b_decale = B * X^diff (on ajoute 'diff' zéros au début de la liste)
    (Polynome listB) = b
    bDecale = Polynome (completeListe listB diff)
    
    -- Dans Z2, l'addition de a et bDecale équivaut à une soustraction, ce qui annule le terme de plus haut degré
    aReduit = add a bDecale
    
    -- Appel récursif sur le reste
    (qSuite, rFinal) = de_pol aReduit b


