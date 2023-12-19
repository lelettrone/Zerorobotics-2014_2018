//Begin page commentS
/*
bool isGoing(float itemPos[3],float objectState[12]){
    float nextPos[3],myVett[3],itemVett[3];
    for(int i=0;i<3;i++)
        nextPos[i]=objectState[i]+objectState[i+3];
    mathVecSubtract(myVett,nextPos,objectState,3);
    mathVecNormalize(myVett,3);
    mathVecSubtract(itemVett,itemPos,objectState,3);
    mathVecNormalize(itemVett,3);
    return angleBetween(myVett,itemVett)<0.15f;
}
*/
 /*float getSituation(){
    float standardPoint[3]={0.00f,0.500f,0.00f};
    float scoreDistances[2];
    int freeIndex = 0;
    for(int i=0;i<4;i++){
        float nowScore[3];
        game.getItemLoc(nowScore,i+3);
        if(nowScore[0]>0.00f){
            scoreDistances[freeIndex]=getDistance(nowScore,standardPoint);
            freeIndex++;
        }
    }
    return ((scoreDistances[0]+scoreDistances[0])/2.00f);
}*/
 
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
}*/




/*bool getNextObjectArea(float objectState[12], float actualY, int nextTime){
    float objectNextY = objectState[1]+(objectState[4]*nextTime);
    float nextYBack = actualY + (0.025f * nextTime);
    if(nextYBack>=0.8f){
        nextYBack=(-0.8f)+(nextYBack-0.8f);
    }
    float nextYFront = nextYBack + 0.9f;
    if(nextYFront>=0.8f){
        nextYFront=(-0.8f)+(nextYFront-0.8f);
    }
    nextYBack = nextYFront-0.9f;
    if(objectNextY<nextYFront && objectNextY>nextYBack){
        return true;//Light\Grey
    }
return false;//Dark
}*/

/*float getLightNexPos(float actualY,int nextTime){
    float nextYBack = actualY + (0.025f * nextTime);
    nextYBack = ((nextYBack>=0.8f)?(-0.8f)+(nextYBack-0.8f):nextYBack);
    float nextYFront = nextYBack + 0.9f;
    nextYFront = ((nextYFront>=0.8f)?(-0.8f)+(nextYFront-0.8f):nextYFront);
    return nextYFront;
}*/
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
//Begin page controllerZoneScrolling
float getLightFrontActualPos(float actualY){
    float frontInterface = actualY+0.85f;
    return ((frontInterface>0.8)?frontInterface-1.6f:frontInterface);
}
void getLightNexPos(float actualY,int nextTime, float outY[2]){
    float nextYBack = actualY + (0.025f * nextTime);
    outY[0]=((nextYBack>=0.8f)?(-0.8f)+(nextYBack-0.8f):nextYBack);
    float nextYFront = outY[0] + 0.9f;
    outY[1]=((nextYFront>=0.8f)?(-0.8f)+(nextYFront-0.8f):nextYFront);
}
bool getNextObjectArea(float objectState[12], float actualY, int nextTime){
    float objectNextY = objectState[1]+(objectState[4]*nextTime);
    float nextYBack = actualY + (0.025f * nextTime);
    nextYBack=((nextYBack>=0.8f)?(-0.8f)+(nextYBack-0.8f):nextYBack);
    float nextYFront = nextYBack + 0.9f;
    nextYFront=((nextYFront>=0.8f)?(-0.8f)+(nextYFront-0.8f):nextYFront);
    nextYBack = nextYFront-0.9f;
    return ((objectNextY<nextYFront && objectNextY>nextYBack)?true:false);
    //1 = Light\Grey
    //0 Dark
}

//End page controllerZoneScrolling
//Begin page functions
bool isGoing(float itemPos[3],float objectState[12]){
    float myVett[3],itemVett[3];
    for(int i=0;i<3;i++)
        myVett[i]=objectState[i];
    mathVecSubtract(itemVett,itemPos,objectState,3);
    mathVecNormalize(itemVett,3);
    return angleBetween(myVett,itemVett)<0.15f;
}
int getSituation(){
    float firstPos[3],secondPos[3];
    game.getItemLoc(firstPos,3);
    game.getItemLoc(secondPos,5);
    if(firstPos[2]<=0.00f && secondPos[2]<=0.00f){
        return 0;
    }else if((firstPos[2]<=0.00f && secondPos[2]>=0.00f)||(firstPos[2]>=0.00f && secondPos[2]<=0.00f)){
        if(firstPos[2]>0.200f || secondPos[2]>0.200f){
            return 2;
        }
        return 1;
    }else{
     //   if(firstPos[2]>0.300f || secondPos[2]>0.300f){
          if(firstPos[2]>0.450f && secondPos[2]>0.450f){
            return 4;
        }
        return 3;
    }
return 5;
//0 Sopra
//1 Medio Alto
//2 Medio Basso
//3 Basso Alto
//4 Bassotto
//5 BOH
}
float angleBetween(float *vectorA, float *vectorB){//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
int chooseRightScore(short actualSituation, float myState[12],float enemyState[12],float whereToGo[3], int numScores,short myZone,short enemyZone){
    float lastDistance = 5.00f; 
    short choosenIndex=-1;
    short numItemTaken=0;
    for(int i=0;i<4;i++){
        (game.hasItem(i+3)==0)?numItemTaken++:0;
    }
    if(numItemTaken<numScores){
        
        for(int i=0;i<4;i++){
            short actualIndex = i+3;
            float nowPosition[3];
            game.getItemLoc(nowPosition,actualIndex);
            float nowDistance = getDistance(myState,nowPosition);
            bool performCondition = false;
            //0 Sopra
            //1 Medio Alto || due sopra e due sotto prendibili
            //2 Medio Basso || due sopra e due sotto troppo bassi, imprendibili
            //3 Basso Alto || 4 sotto, 2 prendibili e 2 no
            //4 Bassotto || tutti non prendibili
        //    DEBUG(("TimeFREE: %f",freeTime));
            
            switch(actualSituation){
                case 0:
                    performCondition = true;
                    break;
                case 1:
                    performCondition = (nowPosition[2]<=0.00f)||(nowPosition[2]<=0.150f&&enemyState[2]>-0.200f);
                    break;
                case 2:
                    performCondition = (nowPosition[2]<=0.00f)||(nowPosition[2]<=0.450f&&enemyState[2]>0.100f);
                    break;
                case 3:
                    performCondition = (nowPosition[2]<=0.450f&&enemyState[2]>0.100f);
                    break;
                case 4:
                    performCondition = false;
                    break;
            }
            if((nowDistance<lastDistance) && (game.hasItem(actualIndex)==-1) && performCondition  && getDistance(enemyState,nowPosition)>0.1f){
                    choosenIndex = actualIndex;
                    lastDistance = nowDistance; 
            } 
        }//
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex); 
    }//
  //  DEBUG(("INDEX: %d",choosenIndex));
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
}
//End page functions
//Begin page main
float whereToGo[3],velocity;
short Phase, Color,Time;
bool attitudeToEarth;
void init(){
    Phase = Time = attitudeToEarth = 0;
  //  DEBUG(("Situation: %d",getSituation()));
  DEBUG(("Greetings from Echelon"));
}
void loop(){
    float myState[12],enemyState[12],whereAttitude[3];
    api.getMyZRState(myState);api.getOtherZRState(enemyState);
    if(Time<1) Color = ((myState[0] < 0) ? 0 : 1);
    short actualSituation = getSituation();
    float kVel = 0.075f;
    bool goingUp = false;
    //float yBoundaries[2];
    
    float actualYBackBoundaries = game.getLightInterfacePosition();
    float actualYFrontBoundaries = getLightFrontActualPos(actualYBackBoundaries);
    //getLightNexPos(actualYBackBoundaries,7,yBoundaries);
    //DEBUG(("yBoundaries: %f",yBoundaries));
    //DEBUG(("Front Actual Interface %f",getLightFrontActualPos(game.getLightInterfacePosition())));
    //DEBUG(("yReal: %f",game.getLightInterfacePosition()));
    bool oStammGuardand = game.isFacingOther(), doTorque=false;
    Time++;
    //DEBUG(("%d",Time));
    float myRotationVelocity = mathVecMagnitude(myState+9,3),myEnergy = game.getEnergy(); 
    short myZone = game.posInArea(myState),enemyZone = game.posInArea(enemyState);
    if(game.getMemoryFilled()<2 && CanAnotherPic(oStammGuardand,myZone,enemyZone,myEnergy,myState,enemyState)){
        if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
            //game.hasItem(7+Color)==1?(game.getPicPoints()>=0?game.takePic():0):game.takePic();
            game.hasItem(7)==1 || game.hasItem(8)==1?(game.getPicPoints()>=0?game.takePic():0):game.takePic();
        }
    }
    if(((game.getMemoryFilled()>=1 && Time>169)
        || (game.getMemoryFilled()==1 && myZone==-1 && enemyZone==-1 && myEnergy>2.3f) 
        || (game.getMemoryFilled()>1))){
            attitudeToEarth=true;
    }
    if(!attitudeToEarth){
        float cathetusAdd = 0.2294237037f*getDistance(myState,enemyState);//tanf(0.253) Adjust point to attitude
        mathVecSubtract(whereAttitude, enemyState, myState, 3);
        whereAttitude[2]+=cathetusAdd;
        mathVecNormalize(whereAttitude,3);
    }else{
        /*for(int i=0;i<3;i++){
            whereAttitude[i]=myState[i]+(myState[i+6]*(0.02731512449));//tanf(0.25)*0.05;
        }
        whereAttitude[2]=myState[2]+0.05f;*/
        float earth[3]={myState[0],myState[1],myState[2]+0.05f};
        mathVecSubtract(whereAttitude,earth,myState,3);
        mathVecNormalize(whereAttitude,3);
        //if(isFacingPos(myState,earth,0.50f)){ //<-Is This right?
        if(myState[8]>0.95533648912f){ //constant from the API  0.95533648912f 0.96891242171f
            if((myEnergy-1.0f)>=0.55f || (myEnergy>=1.00 && Time>169)){
                if(myRotationVelocity<=0.05f){//0.05{
                    game.uploadPics();
                    attitudeToEarth=false;
                }
                doTorque=true;
            }
        }
    }
  
  
  //  bool myAreaIn2Second = getNextObjectArea(myState,actualYBackBoundaries,2),
 //   enemyAreaIn2Second = getNextObjectArea(enemyState,actualYBackBoundaries,2);
    
   /* if((myState[2]>enemyState[2]) || ((myAreaIn2Second) && (enemyAreaIn2Second)) && myEnergy<1.0f || ((myAreaIn2Second) && (!enemyAreaIn2Second))){
     DEBUG(("CI SONO MA LA DISTANZA Ã¨ POCA"));
        if(game.getNumMirrorsHeld()>0 &&  getDistance(myState,enemyState)>=0.5f)//migliorare
            game.useMirror();
    }*/
    //if(Front<myState[1] && (myState[1]-Front)<0.05f && Front<enemyState[1] && (enemyState[1]-Front)<0.0345f)
     //if((actualYFrontBoundaries<myState[1] && (myState[1]-actualYFrontBoundaries)<0.05f && actualYFrontBoundaries<enemyState[1] && (enemyState[1]-actualYFrontBoundaries)<0.0345f
      //   || myZone!=-1 && enemyZone!=-1 && myEnergy<1.0f) 
      /*if(()){
        || myZone!=-1 && enemyZone==-1
        )){*/
    // DEBUG(("CI SONO MA LA DISTANZA Ã¨ POCA"));
       
    //}

    
    if(Phase==0){
        game.getItemLoc(whereToGo,8-Color);
        //kVel = 0.077148161220323f;
        kVel = 0.093148161220323f;
        if(game.hasItem(8-Color)!=-1){
            Phase=1;   
        }
    }
    bool nextMyZone = getNextObjectArea(myState,actualYBackBoundaries,5);
    bool nextEnemyZone = getNextObjectArea(enemyState,actualYBackBoundaries,5);
    if(Phase==1){
        bool condition = ((myZone==-1)?(nextMyZone&&nextEnemyZone):true);
        if((enemyState[2]+(enemyState[5]*2))>=0.15f && condition){//0.10 //0.30
            whereToGo[0]=enemyState[0]+(enemyState[3]*5);
            whereToGo[1]=enemyState[1]+(enemyState[4]*5);
            whereToGo[2]=-0.550;
            kVel = 0.075148161220323f;
            goingUp=true;
        }else{
            whereToGo[0]=((enemyState[0]<=0.0f)? 0.5f: -0.5f);
            whereToGo[1]=(enemyState[1]+(enemyState[4]*5))-(10.5f*0.025f);//(10.5f*0.025f)
            whereToGo[1]=whereToGo[1]>.76f?.76f:whereToGo[1]<-.76f?-.76f:whereToGo[1];
            whereToGo[2]=-0.550;
            chooseRightScore(actualSituation,myState,enemyState,whereToGo,4,myZone,enemyZone);
        }  
    }
    
    nextMyZone = getNextObjectArea(myState,actualYBackBoundaries,1);
    nextEnemyZone = getNextObjectArea(enemyState,actualYBackBoundaries,1);
   // DEBUG(("AREA: %d, %d",nextMyZone,nextEnemyZone));
    if((!goingUp)&&( (nextMyZone && !nextEnemyZone &&game.getOtherEnergy()>1.3f)
          ) ){
             // DEBUG(("ATTIVO"));
        if(game.getNumMirrorsHeld()>0 &&  getDistance(myState,enemyState)>=0.5f){
            game.useMirror();
        }
    }
    float distanceToWhereGo = getDistance(myState, whereToGo);
    if(myEnergy<=1.00f){
        velocity=0.00f;
    }else{
        velocity = (((distanceToWhereGo*0.68f)/kVel)/100.0f);
    }
  //whereToGo[1]=whereToGo[1]>.79f?.79f:whereToGo[1]<-.79f?-.79f:whereToGo[1];
    if(doTorque){
        float torques[3]={-.65f*myState[9],-.65f*myState[10],-.65f*myState[11]}; //-.65f
        if(myRotationVelocity<=.075f){
            if(attitudeToEarth){
                torques[0]=torques[1]=torques[2]=0.0f;
                api.setAttRateTarget(torques);
            }else if(oStammGuardand){
                api.setAttitudeTarget(whereAttitude);
            }
        }else{
            api.setTorques(torques);
        }
    }else{
        api.setAttitudeTarget(whereAttitude);
    }
    
    //	velocity = myEnergy<=1.00f?0.00f:(((distanceToWhereGo*0.68f)/ 0.0977148161220323f)/100.0f);
    //	DEBUG(("attitudeToEarth: %d",attitudeToEarth));
	//  To icrement velocity you must decrement 0.0677148161220323f
	//  To decrement velocity you must increment 0.0677148161220323f
	//  0.0677148161220323f it's a stable value
	
	float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++)
		velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
}
//End page main
