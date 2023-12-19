//Begin page commentS
            /*if(game.hasItem(7+Color)==1){
                if(game.getPicPoints()>=0){
                    game.takePic();
                }
            }else{
                game.takePic();
            }*/
            
            
            
/*   memcpy(myAttProject,myState+6,sizeof(float)*3);
            mathVecNormalize(myAttProject,3);
            for(int i=0;i<3;i++){
                myPointProject[i]=myState[i]+(myAttProject[i]*sinf(0.25)*0.05);//sin(0.25)*0.05;
            }
            myPointProject[2]=myState[2]+0.05f;
            mathVecSubtract(whereAttitude,myPointProject,myState,3);*/
            
/*void getNexPosEnemy(float outPos[3], int nextTime){
    outPos[0]=enemyState[0]+(enemyState[3]*nextTime);
    outPos[1]=enemyState[1]+(enemyState[4]*nextTime);
    outPos[2]=enemyState[2]+(enemyState[5]*nextTime);
}
*/
//End page commentS
//Begin page controllerEntity
//Begin page controllerEntity
bool isFacingPos(float myState[12], float lookPos[3], float maxTollerance){
    float realPos[3];    
    mathVecSubtract(realPos, lookPos, myState, 3); 
    mathVecNormalize(realPos,3);                              
    return acosf(mathVecInner(realPos, myState+6, 3) / (mathVecMagnitude(realPos, 3) * mathVecMagnitude(myState+6, 3)))<maxTollerance;
}
bool CanAnotherPic(bool oGuardamm,short myZone,short enemyZone,float myEnergy,float myState[12],float enemyState[12]){
    if(oGuardamm //verify if we sphere is oriented to enemy
        && ((myZone == -1) ? myEnergy>=1.55 : myEnergy>=1.00) //da vedere
        //&& myEnergy>=1.55f //1.0 should be ok //2.0 //1.55 //1.00f
        && enemyZone!=-1 //we can take pic only when enemy zone is different from DARK game.posInArea(enemyState)!
       && getDistance(myState,enemyState)>=0.5f) //we can take pic only when enemy distance is greate or ideam 0.5
        { 
            return true;
        }
    return false;
}
//End page controllerEntity
//End page controllerEntity
//Begin page functions
float angleBetween(float *vectorA, float *vectorB){//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
int chooseRightScore(float myState[12],float whereToGo[3]){
    float lastDistance = 5.00f; 
    int choosenIndex=-1;
    for(int i=0;i<4;i++){
        float nowPosition[3];
        game.getItemLoc(nowPosition,i+3);
        float nowDistance = getDistance(myState,nowPosition);
        if((nowDistance<lastDistance) && (game.hasItem(i+3)==-1)){
            choosenIndex = i+3;
            lastDistance = nowDistance;
        }
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex);
        
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
}
/*void getNexPosEnemy(float outPos[3], int nextTime){
    
    outPos[0]=enemyState[0]+(enemyState[3]*nextTime);
    outPos[1]=enemyState[1]+(enemyState[4]*nextTime);
    outPos[2]=enemyState[2]+(enemyState[5]*nextTime);
}*/
//End page functions
//Begin page main
float whereToGo[3],velocity,strategyPoint[3];
short Phase, Color,Time;
bool attitudeToEarth;
void init(){
    Phase = Time = attitudeToEarth =  0;
    strategyPoint[1]=0.500;
    strategyPoint[2]=-0.475;
    
}
void loop(){
    float myState[12],enemyState[12],whereAttitude[3];
    api.getMyZRState(myState);api.getOtherZRState(enemyState);
    if(Time<1) Color = ((myState[0] < 0) ? 0 : 1);
    //strategyPoint[0]=enemyState[0]+(enemyState[6]*4);
    strategyPoint[0]=(Color==1)?0.300:-0.300f;
    float kVel = 0.075f;
    bool oStammGuardand = game.isFacingOther(), doTorque=false;
    Time++;
    DEBUG(("%d",Time));
    float myRotationVelocity = mathVecMagnitude(myState+9,3),myEnergy = game.getEnergy(); 
    short myZone = game.posInArea(myState),enemyZone = game.posInArea(enemyState);
    //enemyVel = mathVecMagnitude(enemyState+3,3);
    if(game.getMemoryFilled()<2 && CanAnotherPic(oStammGuardand,myZone,enemyZone,myEnergy,myState,enemyState)){
        if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
          game.hasItem(7+Color)==1?(game.getPicPoints()>=0?game.takePic():0):game.takePic();
        }
    }
    if((game.getMemoryFilled()>=1 && Time>169 
       // || game.getMemoryFilled()==1 && myZone==-1 && myEnergy<1.5f 
        || game.getMemoryFilled()>1)){
            attitudeToEarth=true;
    }
    if(!attitudeToEarth){
        mathVecSubtract(whereAttitude, enemyState, myState, 3);
        mathVecNormalize(whereAttitude,3);
        whereAttitude[2]+=0.24740395925f*getDistance(myState,enemyState);//sinf(0.253) Adjust point to attitude
       // if(oStammGuardand){doTorque=true;}
    }else{
        float earth[3]={myState[0],myState[1],myState[2]+0.05f};
        mathVecSubtract(whereAttitude,earth,myState,3);
        mathVecNormalize(whereAttitude,3);
      // if(isFacingPos(myState,earth,0.50f)){ //<-Is This right?
       if(myState[8]>0.95533648912f){ //constant from the API 0.96891242171
            if((myEnergy-1.0f)>=0.55f || (myEnergy>=1.00 && Time>169)){
                if(myRotationVelocity<=0.05f){//0.05{
                    game.uploadPics();
                }
                DEBUG(("MI TROVO QUI"));
                attitudeToEarth=false;
                doTorque=true;
            }
        }
    }
  //  if(chooseRightScore(myState,whereToGo)==-1){
   //     DEBUG(("Score terminati!"));
  //  }
    if(Phase==0){
        if(game.hasItem(2)==-1){
            game.getItemLoc(whereToGo,2);
            kVel=0.045f;
        }else if(game.hasItem(1-Color)==-1){
            game.getItemLoc(whereToGo,1-Color);
            kVel=0.045f;
        }else{
            Phase = 1;
        }
    }
    if(Phase==1){
       if(game.hasItem(8-Color)==-1){
            game.getItemLoc(whereToGo,8-Color);
            kVel=0.055f;
        }else{
            Phase = 2;
        } 
    }
    if(Phase==2){
       if(chooseRightScore(myState, whereToGo)==-1){
           
        }else{
            Phase = 3;
        } 
    }
    if(Phase == 3){
        memcpy(whereToGo, strategyPoint, sizeof(float)*3);
    }
    DEBUG(("Phase: %d",Phase));
    DEBUG(("kVel: %f",kVel));
    
    
    
    //THIS MUST BE THE END PART OF MAIN PAGE
    float distanceToWhereGo = getDistance(myState, whereToGo);
     if(doTorque){
         float torques[3]={-.42f*myState[9],-.42f*myState[10],-.42f*myState[11]}; //-.65f
        if(myRotationVelocity<=.075f){//0.075 it's stable value
            if(attitudeToEarth){ //no have sense this control, setattrate is never call
              //  memset(torques,0,sizeof(float)*3);
                torques[0]=torques[1]=torques[2]=0.0f;
                DEBUG(("stop rotation"));
                api.setAttRateTarget(torques);
            }
        }else{
            DEBUG(("use torques"));
            api.setTorques(torques);
        }
    }else{
        DEBUG(("setattitude"));
        api.setAttitudeTarget(whereAttitude);
     //   DEBUG(("Mi muovo"));
    }
    
  //  DEBUG(("%f %f %f",whereAttitude[0],whereAttitude[1],whereAttitude[2]));
  if(myEnergy<=1.00f){
	    velocity=0.00f;
	    DEBUG(("Stopping"));
	}else{
	    DEBUG(("Moving"));
	    velocity = (((distanceToWhereGo*0.68f)/kVel)/100.0f);
	}
//	velocity = myEnergy<=1.00f?0.00f:(((distanceToWhereGo*0.68f)/ 0.0977148161220323f)/100.0f);
//	DEBUG(("attitudeToEarth: %d",attitudeToEarth));
	//To icrement velocity you must decrement 0.0677148161220323f
	//To decrement velocity you must increment 0.0677148161220323f
	//0.0677148161220323f it's a stable value
	
	float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++)
		velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
}

//End page main

//End page main
