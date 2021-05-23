setup(){}

led

loop {

    wait(1 minuto)

    coneção servidor()

    if http.response(201){
        pega instrução()

    }

}




coneção servidor(){

    dados

    dado.humidade = humidade()
    dado.ledOn = led
    dado.ledValor

    json() 

    manda pro server
    

}

pega instrução(){

    json = intruçãoServidor()

    config = decodificar o json()

    if config.ligarled
    if config.bomba

}