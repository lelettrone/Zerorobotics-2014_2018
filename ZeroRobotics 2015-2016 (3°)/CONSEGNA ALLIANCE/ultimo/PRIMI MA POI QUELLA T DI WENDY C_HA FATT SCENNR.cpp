//Begin page commentS
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
//End page commentS
//Begin page controllerEntity
//Begin page controllerEntity
bool isFacingPos(float myState[12], float lookPos[3], float maxTollerance){
    float realPos[3];    
    mathVecSubtract(realPos, lookPos, myState, 3); 
    mathVecNormalize(realPos,3);                              
    return acosf(mathVecInner(realPos, myState+6, 3) / (mathVecMagnitude(realPos, 3) * mathVecMagnitude(myState+6, 3)))<maxTollerance;
}
bool CanAnotherPic(bool oGuardamm,short myZone,short enemyZone,float myEnergy,float myState_enemyState){
    return (oGuardamm //verify if we sphere is oriented to enemy
        && ((myZone == -1) ? ((myEnergy>=1.55&&Phase!=0)||(myEnergy>2.45f&&Phase==0)) : myEnergy>=1.00) //da vedere
        //&& myEnergy>=1.55f //1.0 should be ok //2.0 //1.55 //1.00f
        && enemyZone!=-1 //we can take pic only when enemy zone is different from DARK game.posInArea(enemyState)!
       && myState_enemyState>=0.5f); //we can take pic only when enemy distance is greate or ideam 0.5

}
//End page controllerEntity

bool attivaMirror(float interface, float rearInterface, float enemyState[12]){
    return (enemyState[1]<(rearInterface+0.07f)&&enemyState[1]>(interface-0.07f))
    ||(interface>0.1&&enemyState[1]<(rearInterface+0.07f))
    ||(rearInterface<-0.1f&&enemyState[1]>(interface-0.07f));

}
//End page controllerEntity
//End page controllerEntity
//Begin page controllerZoneScrolling

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
    float nextYBack = (actualY) + (0.025f * nextTime);
    nextYBack=((nextYBack>=0.8f)?(nextYBack-1.6f):nextYBack);
    float nextYFront = nextYBack + 0.9f;
    nextYFront=((nextYFront>=0.8f)?(nextYFront-1.6f):nextYFront);
    return (objectNextY<nextYFront && objectNextY>nextYFront-0.9f)||
        (objectNextY<nextYBack+0.9f && objectNextY>nextYBack);
    //1 = Light\Grey
    //0 Dark
}

//End page controllerZoneScrolling
//End page controllerZoneScrolling
//Begin page functions

//Begin page functions
int scelta_score2(float state[12]){
    float lastZ=50.0;
    int choosenIndex = -1;
    float lastDistance = 5.00f; //Conterra' l'ultima distanza registrata valida,
                                //E' impostata ad un valore grande 50.00f
                                //Cosi' al primo ciclo si resettera' prendere la distanza
                                //del primo score esaminato
    for(int i=0;i<4;i++){
        float nowPosition[3];//Posizione dello score attuale esaminato
        game.getItemLoc(nowPosition,i+3);//Prendo la pos e la metto in nowPosition
        float nowDistance = getDistance(state,nowPosition);//Prendo la distanza dallo score attuale
        if(game.hasItem(i+3)==-1){ //Se la (distanza attuale<di quella precedentemente valida) && lo score attuale e' libero 
            if(nowPosition[2]<lastZ ){ //SE 1 SCORE è SOPRA E 1 è SOTTO = GOtoP 
            //
                if((Color == (nowPosition[0]>0))){
                    lastZ=nowPosition[2];
                    choosenIndex = i+3;//L'ID valido viene aggiornato a quello dello score attualmente esaminato
                    lastDistance = nowDistance;//Stessa cosa per la distanza
                }
            }
        }
     /*   if(game.hasItem(i+3)==-1 && nowPosition[2]<lastZ && (Color == (nowPosition[0]>0))){ //Se la (distanza attuale<di quella precedentemente valida) && lo score attuale e' libero 
            lastZ=nowPosition[2];
            choosenIndex = i+3;//L'ID valido viene aggiornato a quello dello score attualmente esaminato
            lastDistance = nowDistance;//Stessa cosa per la distanza
        }*/
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex);
        //DEBUG(("%d",choosenIndex));    
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
    
}
int chooseRightEnergy(float myState[12],float whereToGo[3], int numEnergy){
    float lastDistance = 5.00f; 
    int choosenIndex=-1;
    int numItemTaken=0;
    for(int i=0;i<3;i++){
        (game.hasItem(i)==0)?numItemTaken++:0;
    }
    if(numItemTaken<numEnergy){
       for(int i=0;i<3;i++){
        float nowPosition[3];
        game.getItemLoc(nowPosition,i);
        float nowDistance = getDistance(myState,nowPosition);
        if((nowDistance<lastDistance) && (game.hasItem(i)==-1)){
            choosenIndex = i;
            lastDistance = nowDistance;
        }
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex); 
    }
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
}
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


int scelta_score(float state[12]){
    float lastDistance = 5.00f; //Conterra' l'ultima distanza registrata valida,
                                //E' impostata ad un valore grande 50.00f
                                //Cosi' al primo ciclo si resettera' prendere la distanza
                                //del primo score esaminato
    int choosenIndex = -1,i=0;
    for(i=0;i<4;i++){
        float nowPosition[3];//Posizione dello score attuale esaminato
        game.getItemLoc(nowPosition,i+3);//Prendo la pos e la metto in nowPosition
        float nowDistance = getDistance(state,nowPosition);//Prendo la distanza dallo score attuale
        if((nowDistance<lastDistance)&&game.hasItem(i+3)==-1){
            //Se la (distanza attuale<di quella precedentemente valida) && lo score attuale
            //e' libero 
            choosenIndex = i+3;//L'ID valido viene aggiornato a quello dello score attualmente esaminato
            lastDistance = nowDistance;//Stessa cosa per la distanza
        }
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex);
   // DEBUG(("%d",choosenIndex));    
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
    
}
//End page functions
//End page functions
//Begin page main
//Begin page main
float whereToGo[3];
short Phase, Color,Time;
bool attitudeToEarth,takeEnergy;

void init(){
    Phase = Time = attitudeToEarth = takeEnergy = 0; 
    DEBUG(("Greetings from Echelon"));
}
void loop(){
    float myState[12],enemyState[12],whereAttitude[3];
    api.getMyZRState(myState);api.getOtherZRState(enemyState);
    float velocity;
    float myState_enemyState = getDistance(myState,enemyState);
    short MemoryFilled = game.getMemoryFilled();
    if(Time<1) Color = ((myState[0] < 0) ? 0 : 1);
    float kVel = 0.075f;
    float interface=game.getLightInterfacePosition()+0.9f;
    float rearInterface=interface-0.9f;
    if(interface>0.8)
        interface=interface-1.65;
   
   
    bool oStammGuardand = game.isFacingOther(), doTorque=false;
    Time++;
    float myRotationVelocity = mathVecMagnitude(myState+9,3),myEnergy = game.getEnergy(); 
    short myZone = game.posInArea(myState),enemyZone = game.posInArea(enemyState);
    if(MemoryFilled<2 && CanAnotherPic(oStammGuardand,myZone,enemyZone,myEnergy,myState_enemyState)){
        if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
            float aPozzFa = game.getPicPoints();
            if(Phase!=7){
                ((aPozzFa>=0)?game.takePic():0);
              //  enemyHaveMirror7==1 || enemyHaveMirror8==1?(aPozzFa>=0?game.takePic():0):game.takePic();
            }else if((myEnergy>=2.1f&&getDistance(myState,whereToGo)<0.2f)||(myEnergy>2.86f)){
              //  enemyHaveMirror7==1 || enemyHaveMirror8==1?(aPozzFa>=0?game.takePic():0):game.takePic();
            ((aPozzFa>=0)?game.takePic():0);
                
            }
        }
    }
    if(((MemoryFilled>=1 && Time>168)
        || (Phase>7&&myEnergy<3.4f&&MemoryFilled>0)
        || (MemoryFilled==1 && myZone==-1 && enemyZone==-1 && myEnergy>2.3f) 
        || (myZone==1 && game.getMirrorTimeRemaining()>0 && MemoryFilled>0)
        || (MemoryFilled>1))){
            attitudeToEarth=true;
    }
    
        if(!attitudeToEarth){
            mathVecSubtract(whereAttitude, enemyState, myState, 3);
        }else{
            float earth[3]={myState[0],myState[1],myState[2]+0.05f};
            mathVecSubtract(whereAttitude,earth,myState,3);
            if(myState[8]>0.9631533648912f){ //constant from the API  0.95533648912f 0.96891242171f
                if((myEnergy>=1.55f) || (myEnergy>=1.00 && Time>155)){
                    if(myRotationVelocity<=0.05f){
                        game.uploadPics();
                        attitudeToEarth=false;
                    }
                doTorque=true;
                }
            }
        }
    
    mathVecNormalize(whereAttitude,3);
            
    float score=game.getScore();
    if(Phase==0){
        if((scelta_score2(myState)==-1)||(score>1.4&&(whereToGo[2]>-0.05f
        ||myState_enemyState<0.35f)))
            Phase=5;
        
        
        kVel = 0.095077148161220323f;
        
    }
    
    if(Phase==5){
        if(game.hasItem(8-Color)==-1){
            game.getItemLoc(whereToGo,8-Color);
        }else if ((myState[8]>0.955f)||(game.getScore()>4.7) 
        ||(game.hasItem(8-Color)==1)||(Time>95)||(Time>70&&myZone==-1)
        ||(enemyState[1]<(myState[1]-0.1f))){
            Phase=6;
        }                                                                   
        kVel = 0.0647148161220323f;
    }
    
    if(Phase==6){
        if((myState[1]-interface)<0.05f
            &&myState_enemyState>=0.48f&&Time>75){
            game.useMirror();
        }
        whereToGo[0]=(Color==1)?0.3f:-0.3f;
        whereToGo[1]=rearInterface-0.2;
        whereToGo[2]=0.0f;
       
        kVel = 0.1207148161220323f;
        if(rearInterface>myState[1]&&myZone==-1&&Time>114){
            Phase=7;
        }
    }
    
    if(Phase==7){
        if((MemoryFilled==0&&myEnergy>2.5)
        ||(takeEnergy==true)){
            takeEnergy=true;
            chooseRightEnergy(myState,whereToGo,1);
            if(getDistance(myState,whereToGo)<0.4)
                kVel = 0.0607148161220323f;
            else
                kVel = 0.140148161220323f;
        }
        if(myEnergy>4.8&&Time>140&&takeEnergy==true){
            Phase=8;
        }
        
        if(takeEnergy==false&&Time>130&&MemoryFilled<1){
            Phase=8;
        }
    }
    
    if(Phase==8){
        if(scelta_score(myState)==-1){
            whereToGo[0]=(Color==1)?0.4f:-0.4f;
            whereToGo[1]=0.5f;
            kVel = 0.075;
        }
        else{
            kVel = 0.05f;
        }
    }
    if(myEnergy<1) game.takePic();
    
   
    
    velocity = (((getDistance(myState, whereToGo)*0.68f)/kVel)/100.0f);
   
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
        }else if(Phase!=6 || Phase==6 &&(!attitudeToEarth||myZone==1)){
            api.setAttitudeTarget(whereAttitude);
        }
    
    
    float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++)
		velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
	
}
//End page main
//End page main
