//Begin page main
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
   
void MotoCircolare(float r){
   // alfa+=7.95*PI/180;
    
    float point[3];
    point[0]=r*sinf(alfa);
    point[1]=r*cosf(alfa);
    point[2]=0;
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
//Declare any variables shared between functions here
float alfa;
float myState[12];
float myCoordinates[12];
float Radius;
float RoCircle;
float myRo;
float pointoncircle[3];
float doveandare[3];
float penultimopunto[3];
float ultimopunto[3];
bool gotocircle;
bool ritorna;
float Point1[3],Point2[3];
void init(){
    gotocircle=false;
    ritorna=false;
    pointoncircle[0]=0.5f;
    pointoncircle[1]=0.5f;
    pointoncircle[2]=0.0f;
    
    
    penultimopunto[0]=-0.5f;
    penultimopunto[1]=0.5f;
    penultimopunto[2]=0.0f;
    
    ultimopunto[0]=0.0f;
    ultimopunto[1]=0.48f;
    ultimopunto[2]=0.0f;
    
    doveandare[0]=0.327f;
    doveandare[1]=0.627f;
    doveandare[2]=0.0f;
    Radius = 0.683f; //0.701 //0.69  //0.67 migliore  //0.678
    alfa = 0.28f;
    RoCircle = mathVecMagnitude(pointoncircle,3);
    
    Point1[0]=0.50f;
    Point1[1]=-0.50f;
    Point1[2]=0.00f;
    
    Point2[0]=-0.50f;
    Point2[1]=-0.50f;
    Point2[2]=0.00f;

}

void loop(){
    DEBUG(("Raggio circonferenza:  %f \n  Raggio Sphere: %f",RoCircle,myRo));
    api.getMyZRState(myState);
    if(getDistance(myState,Point1)<0.05){
        Radius=0.673f;
        DEBUG(("LELLO1"));
    }
    if(getDistance(myState,Point2)<0.05){
       //Radius=0.675f+0.0075+0.006762f;
        Radius = 0.698f;
        DEBUG(("LELLO2"));
    }
    for(int i = 0;i<3;i++){
        myCoordinates[i] = myState[i];
    }
     myRo = mathVecMagnitude(myCoordinates,3);
    
    if(getDistance(doveandare,myState)<0.33f){ //0.334
       gotocircle=true;
    }
    
    if(getDistance(penultimopunto,myState)<0.33f){
        ritorna=true;
    }
    
    if(!gotocircle){
       //  goTarget(doveandare);
       setConstantVelocityTo(doveandare,0.06f); //0.06
    }else{
        if(!ritorna){
        MotoCircolare(Radius);
        }else{
            DEBUG(("ci sono"));
            goTarget(ultimopunto);
        }
    }
}

//End page main
//End page main
