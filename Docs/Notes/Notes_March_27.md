#geom_t

#Partie 1 : Profil de vitesse trapézoïdal
##API :
##safe_acc(V0, acc, distance, OPTION)
__INTRO__ Vérifie si la distance est suffisante pour atteindre V0 avec une accélération de acc et freiner avec un accélération -acc. Optimise le profil pour atteindre la destination avec une accélération et une décélération symétriques.

__PARAMS__ 

* ```V0``` : vitesse constante à atteindre
* ```acc``` : Profil d'accélération à obtenir (symétrique en accélération et en décélération)
* ```distance``` : longueur du parcours. Peut-être différente de la distance euclidienne dans le cas d'une trajectoire courbe.
* ```OPTION``` : Définit le comportement si la distance est trop courte pour réaliser le profil souhaité.

__OPTIONS__

* ```SYMMETRIC_PROFILE``` : Calcule une nouvelle accélération qui permet d'avoir un profil symétrique en accélération et en décélération (la plupart du temps, pas de plateau à V0)
* ```CONST_SPEED``` : Annule le profil de vitesse pour avoir un profil rectangulaire classique à la vitesse V0
* ```ACC_UNTIL``` : Accélère jusqu'à atteindre la destination puis s'arrête
* ??

##unsafe_acc(V0, acc)
Applique le profil sans vérification. Si la destination est trop proche, le profil n'est pas optimisé : le robot peut donc partir trop loin.

##Notes

###Relations
$v(t)= \Delta t * acc + v(t-1)$

###Décélération
* Méthode 1 __Discrétiser__ : On a compté n points lors de la phase d'accélération, on devra décélérer à n points de la fin de la trajectoire.
* Méthode 2 __Temps__ : Il faut le même temps pour accélérer que pour décélérer ($t=\frac{V0}{acc}$)
* Méthode 3 __Distance__ : Il faut la même distance pour accélérer que pour décélérer 


#Partie 2 : Formes géométriques
##API :
* ```e_motor_exec_shape(shape_t shape, speed_t speed)```
* ```e_motor_exec_sequence(trajectory_t trajectory, speed_t speed)```
* ```e_motor_idk```

##Structs
* ```typedef struct movement_t shape_t[VERTICES];```
* ```typedef struct trajectory_t movement_t[LENGTH]```
* etc


###Méthode de déplacement entre les points
Il faut déterminer une grille assez fine pour que les pas entre les points de la trajectoire ne soient pas trop visibles. On précalcule le nouveau vecteur direction pour le point n+1 afin qu'il soit prêt lorsqu'on atteint de le point n.

Entre 2 points on calculera :

* angle du vecteur direction (origine -> dest) : $\theta = arctan(\frac{y}{x})$
* norme du vecteur direction : $||\vec{x}|| = \sqrt{(x_{n}-x_{n-1})^2 + (y_{n}-y_{n-1})^2}$