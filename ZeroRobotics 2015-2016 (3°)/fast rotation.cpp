//Begin page main
//Declare any variables shared between functions here
int t;
void init(){
	t=0;
}

void loop(){
    if(api.getTime()==1){
       game.takePic(); 
    }
    float data[3]={0,-.6f,0};
    float myState[12];
    api.getMyZRState(myState);
    DEBUG(("%f",myState[8]));
    if(myState[8]>.8){
        float zero[3]={-2*data[0],-1.5f*data[1],-2*data[2]};
        if(t<5){
            api.setTorques(zero);
            t++;
        }else{
            zero[0]=-myState[9];
            zero[1]=-myState[10];
            zero[2]=-myState[11];
            api.setAttRateTarget(zero);
        }
    }else{
        if(t<3){
    	    api.setTorques(data);
    	    t++;
        }
    }
    float rvel[3]={myState[9],myState[10],myState[11]};
    if(myState[8]>.97 && mathVecMagnitude(rvel,3)<.05f){
        game.uploadPics();
    }
}
//End page main
