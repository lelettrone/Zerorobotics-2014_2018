//Begin page funzione
void muovi(float pos[3]){
    static bool arrivato=false;
   float my_vector, total_vector;
   float stato[12];
   api.getMyZRState(stato);
   my_vector = mathVecMagnitude(stato,3);
   total_vector = mathVecMagnitude(pos,3);
   DEBUG(("x velocity: %f x: %f total vector: %f my_vector: %f",stato[3],stato[0],total_vector,my_vector));
   
   if((total_vector-my_vector)<0.5){
       api.setPositionTarget(pos);
       arrivato=true;
   }else if(arrivato==false){
       api.setForces(pos);
   }
   
}
//End page funzione
//Begin page main
float pos[5][3];
float mio_stato[12];
enum {x,y,z};
int step;
void init(){
    pos[0][x]=0.5; //X 
    pos[0][y]=0.5;  //Y
    pos[0][z]=0; //Z
    pos[1][x]=0.5;//x
    pos[1][y]=-0.5;//y
    pos[1][z]=0;//z
    pos[2][x]=-0.5;//x
    pos[2][y]=-0.5;//y
    pos[2][z]=0;//z
    pos[3][x]=-0.1;
    pos[3][y]=0.5;
    pos[3][z]=0;
    pos[4][x]=0;
    pos[4][y]=0.5;
    pos[4][z]=0;
    step=0;
}
void loop(){
    muovi(pos[step]);
    api.getMyZRState(mio_stato);
    if(mio_stato[x]>=0.5-0.2 && mio_stato[y]>=0.5-0.2 && step==0){ //Es. 2.1 , 2.2
        step++;
        //DEBUG(("Stato: %d",stato));
    }
    if(mio_stato[x]>=0.5-0.2 && mio_stato[y]<=-0.5+0.2 && step==1){//Es. -2.1 ,-2,2
        step++;
        //DEBUG(("Stato: %d",stato));
    }
    if(mio_stato[x]<=-0.5+0.2 && mio_stato[y]<=-0.5+0.2 && step==2){
        step++;
        //DEBUG(("Stato: %d",stato));
    }
    if(mio_stato[x]>=-0.5+0.2 && mio_stato[y]>=0.5-0.2 && step==3){
        step++;
        //DEBUG(("Stato: %d",stato));
    }
    if(mio_stato[x]>=0 && mio_stato[y]>=0.5-0.2 && step==4){
        step++;
        //DEBUG(("Stato: %d",stato));
    }
    
    
    
}

//End page main
