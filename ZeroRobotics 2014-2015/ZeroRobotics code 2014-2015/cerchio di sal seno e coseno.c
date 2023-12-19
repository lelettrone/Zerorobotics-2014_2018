//Begin page main
float stato[12];
float cer[3];
int a;
float r;

void init(){
    a = 0;        //alpha, angolo!
    r = 0.4;      //raggio della circonferenza
    cer[0] = 0.4; //stabilisco il primo
    cer[1] = 0;   //punto della circonf
}

void loop(){
    api.getMyZRState(stato);
    
    api.setPositionTarget(cer); //vado al punto
    
    if(isNear(cer,stato,0.05)){ //quando arrivo, imposto un nuovo punto
        a++;                   //aumentando alpha di uno e 
        cer[0] = r * cos(a);   //ricalcolando x ed y
        cer[1] = r * sin(a);
    }
}

bool isNear(float tar[12], float me[12], float r){
    float vPath[12];
    float d;
    mathVecSubtract(vPath,tar,me,3); //vettore tra i due punti dati

    d = mathVecMagnitude(vPath,3);
    DEBUG(("%f", d));
    
    if( d <= r ) // se siamo vicini
        return true;
    else
        return false;
}
//End page main
