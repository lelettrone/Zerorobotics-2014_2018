bool asteroidCollision(float posx, float posy, float tarx, float tary){
    float coefx, n; //coefficiente e termine noto dell'equazione della retta
    float a,b,c,d;
    bool collisione;
    int tipo;       //se l'equazione è in forma y=mx+q vale 1, se in x=q vale -1, se in y=q vale -2, se punti sovrapposti vale 0
    tipo=calcoloEquazione(posx,posy,tarx,tary,coefx,n);
    switch(tipo){
    case 1:
        a=coefx*coefx+1;            //NON
        b=2*coefx*n;                //CHIEDETEMI
        c=(n*n)-0.09;               //IL
        d=(b*b)-(4*a*c);            //PERCHE'
        if(d<0){
            collisione=false;
        }else{
            collisione=true;
        }
        break;
    case 0:
        collisione=false;
        break;
    case -1:
        if(q<=0.3){
            collisione=true;
        }else{
            collisione=false;
        }
        break;
    return collisione
}
 
int calcoloEquazione(float x1, float y1, float x2, float y2, float cx, float q){
    int esito;
    dy = y2 - y1;
        dx = x2 - x1;
        if ((dx==0)||(dy==0)){
            if((dx==0)&&(dy==0)){
                esito=0;
            }else if(dx==0){
                esito=-1;
                q=x1;
            } else {
                esito=-1;
                q=y1;
            }
        }else{
            esito=1;
            cx=dy/dx;
            q = y1 - cx * x1;
        }
        return esito;
}
