//Begin page funzioni


void goTarget(float Target[3]){
    float myState[12];
    api.getMyZRState(myState);
    float myVelocity[]={myState[3],myState[4],myState[5]};
    float myPosition[]={myState[0],myState[1],myState[2]};
    float idealVector[3],realVector[3];
    mathVecSubtract(idealVector,myPosition,Target,3);
    mathVecMult(idealVector,idealVector,-1,3);
    mathVecSubtract(realVector,idealVector,myVelocity,3);
    mathVecNormalize(realVector,3);
    mathVecMult(realVector,realVector,0.07f,3);
    api.setVelocityTarget(realVector);
}




void stampaarray(float s[]){
    int length = sizeof(s)/sizeof(s[0]);
    for(int i = 0;i<length;i++){
        DEBUG(("%f",s[i]));
    }
    DEBUG(("\n"));
}
void setConstantVelocityTo(float point[], float k){
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
float getPercent(float max,int percent){
    return (max*percent)/100;
}

void goOnCircle(float radius,bool var){
    float alpha=0.14f;// 0.14
    float cosalpha=cosf(alpha),sinalpha=sinf(alpha); 
    float stato[12];
    api.getMyZRState(stato);
    float v[3], w[3];
    mathVecMult(v,stato,1,3);
    mathVecNormalize(v,3);
    if(!var){
    w[0]=radius*(v[0]*cosalpha-v[1]*sinalpha);
    w[1]=radius*(v[0]*sinalpha+v[1]*cosalpha);
    }else{
    w[0]=radius*(v[1]*sinalpha+v[0]*cosalpha);
    w[1]=radius*(v[1]*cosalpha-v[0]*sinalpha);
    }
    w[2]=0;
    mathVecSubtract(v,w,stato,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
float getDistance(float a[13], float b[13])
{
	float distance = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return distance;
}
bool getColor(){
    float Blue[]={0.00f, -0.5f, 0.00f};
    float Red[]={0.00f, 0.5f, 0.00f};
    if(getDistance(myState,Blue)<=0.05f){
        return 0;
    }else if(getDistance(myState,Red)<=0.05f){
        return 1;
    }
}

//End page funzioni
//Begin page main

//Begin page main
//tutti i valori presenti sono messi adcazzum AMTERDAM YEP
enum Axis{x,y,z};
float myState[12];
float Points[2][4][3];
float forza[3];
float Radius;
int Index;
int step;
float Omega;
bool fatto;
bool distanzaminore;
void init(){
    fatto=false;
    distanzaminore=false;
    
    Omega=0.1356f;
    Radius=0.646f; //0.65 0.646 6435
    Points[0][1][x]=0.5f;
    Points[0][1][y]=-0.5f;//riduzione raggio sphere blue
    Points[0][1][z]=0.00f; 
    
  //  -0,64278760968653932632264340990726
    Points[0][2][x]=-0.0f;
    Points[0][2][y]=1.0f;//49489841458940237f
    Points[0][2][z]=0.0f;
    
    Points[1][1][x]=-0.5f; 
    Points[1][1][y]=0.5f;//riduzione raggio sphere rosso
    Points[1][1][z]=0.00f;
    
    Points[0][3][x]=0.06f;
    Points[0][3][y]=-0.485f;
    Points[0][3][z]=0.0f;
    
    Points[1][3][x]=-0.08f;
    Points[1][3][y]=0.485f;
    Points[1][3][z]=0.0f;
    
    Points[1][2][x]=-0.0f;
    Points[1][2][y]=-1.0f;//49489841458940237f
    Points[1][2][z]=0.0f;
    
    Points[0][0][x]=-0.19f; //terzo punto sphere blue
    Points[0][0][y]=0.70f;
    Points[0][0][z]=0.00f;
     
    Points[1][0][x]=0.19f;  //terzo punto sphere rosso
    Points[1][0][y]=-0.70f;
    Points[1][0][z]=0.00f;
    
    api.getMyZRState(myState); 
    Index = getColor(); //get color sphere
    step=0;
  //  DEBUG(("Color: %d",Index));
    
}
void loop(){
    api.getMyZRState(myState);
     if(getDistance(myState,Points[Index][step])<0.05f){ //nel terzo punto cambia il raggio per effettuare il passaggio nel punto previsto dal regolamento
       Radius=0.67f; //0.67
     // DEBUG(("Cambio Terzo Punto: %d \n",Index));
    }
 
      if(getDistance(myState,Points[Index][step+1])<0.115f){// 125  0.078   diminuisce il raggio nell'ultimo punto per tornare a quello di partenza
    /* Omega=0.158f;
      Radius=0.366f; //0.45 oppure 0.457 Ps: consigliato 0475*/
      //Radius=0.45f;
      
       //   if(getDistance(myState,Points[Index][step+1])<0.3f){
              fatto=true;
         //     DEBUG(("E' UN'ADRENALINA"));
        //  }
        /* else{
              api.setPositionTarget(Points[Index][step+1]);
              DEBUG(("E' UN'ADRENALINA POSITION"));
          }*/
          //fatto=true;
       //   DEBUG(("Cambio: %d",Index));
    }
    
  
    
   /* if(getDistance(myState,Points[Index][step+4])<0.05f){ //ho messo 4 solo per evitarlo, ma è 3
        distanzaminore=true;
      //  DEBUG(("CI SONOOOO"));
    }*/
    
    
    if(!fatto){
        goOnCircle(Radius,true); //movimento generale :)
    }else{
        
        if(!distanzaminore){
            //setConstantVelocityTo(Points[Index][step+3],0.06f);
            goTarget(Points[Index][step+3]);
            //setConstantVelocityTo(Points[Index][step+3],0.07f);
            //api.setForces(Points[Index][step+2]);
         //    DEBUG(("CIAO2"));
            
        }
    }
}


//End page main
