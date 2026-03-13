


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

data Z2 = Z2 Integer deriving (Show)

instance Anneau Z2 where
    zero = Z2 0
    un = Z2 1
    add (Z2 a) (Z2 b) = Z2 $ mod (a + b) 2
    mul (Z2 a) (Z2 b) = Z2 $ a*b


data Polynome a = Polynome [a] deriving (Show) -- definit le type des polynomes de type a :(a0,a1,a2,...)

addp ::(Anneau a ) => Polynome a -> Polynome a -> Polynome a
addp (Polynome l1) (Polynome l2) = Polynome (addL l1 l2)




completeliste :: (Anneau a) => [a] -> Int -> [a]
completeliste l 0 = l 
completeliste l n = completeliste (l++[zero]) (n-1)






addL::(Anneau a ) => [a]->[a]->[a]
addL [] _ = []
addL _ [] = []
addL (x:xs) (y:ys) = (add x y) : addL xs ys


instance (Anneau a) => Anneau (Polynome a) where
    zero = Polynome [zero]
    un = Polynome [un]
    add = addp 
    --mul (Polynome [a:as]) (Polynome [b:bs]) = (mul a b ): (mulp (Polynome as) (Polynome bs))






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





--d_euc_pol:: (Polynome P) -> (Polynome M) -> (Polynome R) 




plusieurs façons de definir les polynomes


ensuite on instancie dans Anneau



--}