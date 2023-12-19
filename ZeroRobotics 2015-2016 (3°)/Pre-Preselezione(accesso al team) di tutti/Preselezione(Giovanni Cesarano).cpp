//Begin page Funzioni
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
void mathVecMult (float *v, float *a, float k, int dim)
{
    int i;
    for (i=0; i<dim; i++)
    {
        v[i]=a[i]*k;
    }
}

void goOnCircle(float radius,bool var){
    float Omega=0.16f;
    float alpha=0.14f;
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
//End page Funzioni
//Begin page main
void init(){

}
void loop(){
    goOnCircle(0.5f,true);
}
//End page main
