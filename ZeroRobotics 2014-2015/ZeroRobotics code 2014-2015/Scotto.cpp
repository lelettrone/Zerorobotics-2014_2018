//Begin page main
float myState[12];
float myPos[3];
float itemPos[2][3];
float itemSetPos[2][3];
float vectorBetween[3];
int counter;
bool posi;
int step;
bool camera_enabled;
unsigned int tempo;
void init(){
    for(int i=0;i<3;i++){
	    itemPos[0][i]=0.0;
	    itemPos[1][i]=0.0;
	}
	counter=0;
	step=0;
	posi=true;
	camera_enabled=true;
}

void loop(){
    if(game.getMemoryFilled()==2){
        game.uploadPic();
        camera_enabled=false;
        tempo=api.getTime();
    }
    if(game.getMemoryFilled()==0){
        (api.getTime()-tempo)>=3 ? camera_enabled=true : camera_enabled=false;
    }
	api.getMyZRState(myState);
	for(int i=0;i<3;i++){
	    myPos[i]=myState[i];
	}
	if((counter==0)||((counter-1)%60==0)){
	    //GET INFOs ABOUT POIs
	    game.getPOILoc(itemPos[0],0);
    	game.getPOILoc(itemPos[1],1);
	    DEBUG(("- 1 o %f -2 o %f o",itemPos[0][1],itemPos[1][1]));
	    
	    //get positions for taking photos
	    for(int i=0;i<2;i++){
        	itemSetPos[i][0]=itemPos[i][0]-0.3;
        	if(itemPos[i][1]>0){
        	    itemSetPos[i][1]=itemPos[i][1]+0.3;
        	}else{
         	    itemSetPos[i][1]=itemPos[i][1]-0.3;
          	}
	    }
	    step=1;
	}
	//if we haven't take photos
	if(step>=0){
	    //calculate attitude for aligning to the POI
	    mathVecSubtract(vectorBetween,itemPos[step],myPos,3);
	    mathVecNormalize(vectorBetween,3);
	    api.setAttitudeTarget(vectorBetween);
	    //set position aligned to the poi
    	api.setPositionTarget(itemSetPos[step]);
    	//posi=game.alignLine();
    	//if we are aligned
    	posi=alignPOI();
    	if(posi==true){
    	    DEBUG(("ON LINE"));
    	    step--;
    	    if(game.getMemoryFilled()<2){
    	         //game.takePic(myState,itemPos[step]);
    	    }
    	}
	}
	counter++;
}
bool alignPOI(){
	/*se la torsione del nostro satellite
	non e' uguale alla torsione di setAttitude
	restituisce false altrimenti true*/
	
    bool paciocco=false; 		//variabile di stato
    float myAttitude[3]; 		//torsione del nostro satellite
    api.getMyZRState(myState);
	for(int i=0;i<3;i++){
	    myAttitude[i]=myState[i+6];
	}
	for(int i=0;i<3;i++){
	    myPos[i]=myState[i];
	}
	
	if(
	    (simili(myAttitude[0],vectorBetween[0]))
	&&  (simili(myAttitude[1],vectorBetween[1]))
	&&  (simili(myPos[0],itemSetPos[step][0]))
	&&  (simili(myPos[1],itemSetPos[step][1]))
	){
	    paciocco=true;
    }
	return paciocco;
}

bool simili(float a, float b){
    float risultato;
    risultato=a-b;
    if((risultato<0.05)&&(risultato>-0.05)){
        return true;
    }else{
        return false;
    }
    
}
//End page main
