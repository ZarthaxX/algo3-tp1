contador para poda de factibilidad y de optimalidad para saber cuantas veces se activaron


ideas de experimentacion
n = #negocios
c = contagio limite
    
comparaciones:
backtrack sin podas vs poda fact vs poda opt
backtrack con dos posibles podas de opt (suma acum vs suma acum refinada)
ejemplo de las dos podas de opt:
1   5   7   21   6   9
49  48  43  36   15  9 <- suma acum
35  35  30  30   9   9 <- suma acum refinada (tomo suma acumulada teniendo en cuenta el maximo de a dos elementos)

dinamica vs bt con podas

tipo de instancias
    -familias
      - relacion contagio maximo y contagio por negocio
        definimos los negocios como (a,b) donde a,b <- [A,B]
        y el c = (media de contagio por negocio) * algo, donde "algo" se puede definir para generar mas posibles soluciones o no
      - relacion beneficio contagio
         beneficio < contagio
         beneficio > contagio
        muchas opciones:  
        pocas opciones: 
      - dinamica
        tomamos n y c del intervalo [A,B]
        y los negocios los tomamos al azar del intervalo [(1,1),(B,B)]
    -hardcoded
        bt mejor-opt:[(n,1),(1,1),(n,1),...,(n,1)] c=n
            la solucion optima se obtiene en la rama de mas a la izq (la primera explorada). luego en el paso recursivo que se decida NO incluir a uno de estos elementos (n,1), cuando se haga el chequeo para poda de optimalidad, la suma va a dar menor al optimo encontrado. Ya que se va a estar sumando todos los (n,1) restantes MAS los (1,1), pero como todos los (1,1) sumados no superan en beneficio a un unico (n,1) (que seguro no forma parte de la solucion parcial actual), se va a activar la poda        
        bt mejor-fact:[(1,c+1),(1,c+1),...,(1,c+1),(1,c)]
            cada vez que intento incluir a un negocio (moverse a la izq en el arbol) se activa la poda
            solo no se va a activar al considerar el ultimo negocio
            entonces termino recorriendo la rama mas a la derecha (no incluir nada)
        bt peor-opt: [(1,1),(1,1)....,(1,1),(2n,c)] ; los (1,1) podrian ser (1,c) 
            el optimo es incluir SOLO al ultimo negocio. este no puede ser incluido con ningun otro, entonces todas las soluciones que vayamos armando van a poder mejorarse, y la poda nunca se va a activar
        bt peor-fact: [(1,1),(1,1),...,(1,1),(1,1)] c = n
            la poda nunca se activa porque todos los negocios podrian incluirse sin superar el contagio limite
-bruteforce
    complejidad = 2^n en todos los casos
-backtrack

-dinamica

-graficos

-bt mejor fact : elegimos graficar nodos promedio en lugar del tiempo, porque no se puede apreciar bien la linealidad
                hay que ver si con n = 10k se aprecia utilizando el tiempo