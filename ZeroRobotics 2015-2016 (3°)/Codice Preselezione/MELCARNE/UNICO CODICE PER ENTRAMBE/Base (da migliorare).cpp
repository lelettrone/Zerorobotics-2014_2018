//Begin page funzioni
 void goTarget(float Target[3]){ //MADE BY RICCIO-CESARANO 01/07/2015
    float myState[12];
    api.getMyZRState(myState);
    float myVelocity[]={myState[3],myState[4],myState[5]};
    float myPosition[]={myState[0],myState[1],myState[2]};
    float idealVector[3],realVector[3];
    mathVecSubtract(idealVector,Target,myPosition,3);
    mathVecSubtract(realVector,idealVector,myVelocity,3);
    mathVecNormalize(realVector,3);
    mathVecMult(realVector,realVector,0.075f,3);
    api.setVelocityTarget(realVector);
}  
   
float getDistance(float a[13], float b[13]){//MADE BY RICCIO-CESARANO 01/07/2015
	float distance = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return distance;
}
   
void MotoCircolare(float r){ //MADE BY RICCIO-CESARANO-ALBERTO 08/07/2015
   // alfa+=7.95*PI/180;
    float point[3];
    
    point[0]=r*sinf(alfa);
    point[1]=r*cosf(alfa);
    point[2]=0;
    
    if(Index<1){
        for(int i = 0;i<3;i++){
            point[i]*=-1;
        }
    }
    api.getMyZRState(myState);
    float vectorBetween[3];
    mathVecSubtract(vectorBetween,point,myState,3);
    mathVecNormalize(vectorBetween,3);
    for(int i=0;i<3;i++){
        vectorBetween[i]=vectorBetween[i]*0.084;//0.078 //0.80
    }
    alfa += 0.10;
    api.setVelocityTarget(vectorBetween);
}


void setConstantVelocityTo(float point[], float k)
{
	    float myState[12];
		float velocityVector[3];
		api.getMyZRState(myState);
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
		api.setVelocityTarget(velocityVector);
}

void mathVecMult (float *v, float *a, float k, int dim){
    int i;
    for (i=0; i<dim; i++){
        v[i]=a[i]*k;
    }
}
bool getColor(){//MADE BY CESARANO
    return ((myState[1] <0.0f) ? 0 : 1);
}
//End page funzioni
//Begin page main
float alfa,myState[12],myCoordinates[12],Radius,RoCircle,myRo;
float pointoncircle[3],Point[2][5][3];
int Index,Step;
bool gotocircle,ritorna;
void init(){
    gotocircle=false;
    ritorna=false;
    Radius = 0.683f; //0.701 //0.69  //0.67 migliore  //0.678
    alfa = 0.28;//-0.28f;
    api.getMyZRState(myState);
    Index = getColor();
    Step=0;
    RoCircle = mathVecMagnitude(pointoncircle,3);
    
    pointoncircle[0]=0.5f;
    pointoncircle[1]=0.5f;
    pointoncircle[2]=0.0f;
    
    Point[0][0][0]=-0.327f; //doveandare
    Point[0][0][1]=-0.627f;
    Point[0][0][2]=0.0f;
    
    Point[0][1][0]=-0.50f; //primo cambio raggio
    Point[0][1][1]=0.50f;
    Point[0][1][2]=0.0f;
    
    Point[0][2][0]=0.50f; //secondo cambio raggio
    Point[0][2][1]=0.50f;
    Point[0][2][2]=0.0f;
    
    Point[0][3][0]=0.50f; //verifica per cambiare legge orario
    Point[0][3][1]=-0.50f;//penultimo punto
    Point[0][3][2]=0.0f;
    
    Point[0][4][0]=0.0f; //punto di ritorno
    Point[0][4][1]=-0.48f;
    Point[0][4][2]=0.0f;
    
    for(int i = 0;i<5;i++){ //MADE BY CESARANO-RICCIO 9/07/2015
        for(int j = 0;j<3;j++){
            Point[1][i][j]=(Point[0][i][j])*-1;
        }
    }
}

void loop(){
    api.getMyZRState(myState);
    myRo = mathVecMagnitude(myState,3);
    DEBUG(("Raggio circonferenza:  %f \n  Raggio Sphere: %f",RoCircle,myRo));
    
    if(getDistance(myState,Point[Index][Step+1])<0.05){
        Radius=0.673f;
        DEBUG(("LELLO1"));
    }
    if(getDistance(myState,Point[Index][Step+2])<0.05){
       //Radius=0.675f+0.0075+0.006762f;
        Radius = 0.698f;
        DEBUG(("LELLO2"));
    }
   /* for(int i = 0;i<3;i++){
        myCoordinates[i] = myState[i];
    }*/
     
    
    if(getDistance(Point[Index][Step],myState)<0.33f){ //0.334
       gotocircle=true;
    }
    
    if(getDistance(Point[Index][Step+3],myState)<0.33f){
        ritorna=true;
    }
    
   // (gotocircle ? (ritorna ? goTarget(Point[Index][Step+4]) : MotoCircolare(Radius)) : setConstantVelocityTo(Point[Index][Step],0.06f));
    if(!gotocircle){
       //  goTarget(doveandare);
       setConstantVelocityTo(Point[Index][Step],0.06f); //0.06
    }else{
        if(!ritorna){
        MotoCircolare(Radius);
        }else{
            DEBUG(("ci sono"));
            goTarget(Point[Index][Step+4]);
        }
    }
}

//End page main
//End page main
