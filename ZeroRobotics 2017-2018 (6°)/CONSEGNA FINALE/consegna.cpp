//Begin page DNS
#define CHOOSESTRATEGY 0
#define CHOOSESAMPLE 2
#define STARTDRILL 3
#define STOPDRILL 4
// #define STUDYZONE 5
 #define DROP 10
// #define PULA 11
//#define PUSHBOTTOM 12

//End page DNS
//Begin page Schema
typedef struct s_square{
    float squareDrilled[3];
    int actualDrillIndex;
    float conc;
}Square;
//End page Schema
//Begin page functions

/*void getBestPath(float *myState, int resSquare[3]){//questa funzione cerca nei punti con maggiore interesse dove ci sonopiù SQUARE alti a minima distanza
     int frequencySquare[] = {3, 3, 1,5, -4, 7,-6, 1};
     int index = 0;
     int lastComfortableIndex = 0.0f;
     int bestSquare[3];
     for(int i = 0; i< 4; i++){
         int actualStudyPoint[3] = {frequencySquare[i * 2 + 0], frequencySquare[i * 2 + 1], 0};
         float actualSquareCoord[3];
         game.square2pos(actualStudyPoint, actualSquareCoord);
         actualSquareCoord[2] = game.getTerrainHeight(actualSquare);
         
         float distance = getDistance(myState, actualSquareCoord, 3);
         float highSquare = analyzeAroundSquare(actualStudyPoint);
         if((highSquare / distance) > lastComfortableIndex  && !game.isGeyserHere(actualStudyPoint)){
             lastComfortableIndex = (highSquare / distance);
             index = i;
             memcpy(bestSquare, actualStudyPoint, sizeof(int)*3);
         }
     }

     DEBUG(("indexFound: %d", index));
     //DEBUG(("X:%d Y:%d", bestSquare[0], bestSquare[1]));
     
}*/



/*int analyzeAroundSquare(int square[2]){
    int allDiff[] = {-2, -1 , 0, 1, 2};
    int howManyHighSquare = 0;
    for(int i = 0; i<5; i++){
        for(int j = 0; j < 5; j++){
            int actualSquare[3] = {square[0] + allDiff[i], square[1] + allDiff[j], 0};
            float squareHeigth = game.getTerrainHeight(actualSquare);
            if(squareHeigth == 0.40f){ howManyHighSquare++;}
            DEBUG(("%d %d", square[0] + allDiff[i], square[1] + allDiff[j]));
        }
    }
    
    DEBUG(("howManyHighSquare: %d", howManyHighSquare));
    return howManyHighSquare;
}*/

/*int analyzeAroundSquare(int square[3], float *myState){
    int allDiff[] = {-3, -2, -1 , 0, 1, 2, 3};
    // int allDiff[3] = {-1,0,1}; 
    int howManyHighSquare = 0;
    float lastIndex = 0.0f;
    for(int i = 0; i<7; i++){
        for(int j = 0; j<7; j++){
            int actualStudySquare[3] = {square[0] + allDiff[i], square[1] + allDiff[j], 0};
            float squareHeigth = game.getTerrainHeight(actualStudySquare);
            
            if(fabs(actualStudySquare[0]) <= 2 && fabs(actualStudySquare[1]) <= 2) continue;
            if((squareHeigth == 0.40f || (_Time > 125 && squareHeigth <= 0.48f))  && game.getDrills(actualStudySquare)==0){ 
                howManyHighSquare++;
                float squareCoord[3];
                game.square2pos(actualStudySquare, squareCoord);
                squareCoord[2] = squareHeigth;
                
                
                
               // float futureMyState[3];
                //mathVecAdd(futureMyState, myState, myState+3, 3);
                float actualIndex = (squareHeigth) / ((getDistance(myState, squareCoord, 3) + mathVecMagnitude(squareCoord, 3)) );
                
                if(actualIndex > lastIndex && !game.isGeyserHere(actualStudySquare) && (-8<=actualStudySquare[0] && actualStudySquare[0]<=8 && -10<=actualStudySquare[1] && actualStudySquare[1]<=10)){
                    memcpy(actualSquare, actualStudySquare, sizeof(int)*3);
                    lastIndex = actualIndex;
                }
                
                
            }
            //DEBUG(("%d %d", square[0] + allDiff[i], square[1] + allDiff[j]));
        }
    }
    
   // DEBUG(("howManyHighSquare: %d", howManyHighSquare));
    return howManyHighSquare;
}*/



/*void getBestPath2(float *myState){
    
    //int frequencySquare[] = {3, 3, 1, 5, -4, 7,-6, 1};
    int frequencySquare[] = {3, 3,5, 1, 1, 5, -4, 7,-6, 1};
    //int i = 0;
    int numHighSquare = 0;
    do{
        int actualStudyPoint[3] = {frequencySquare[counterDrop * 2 + 0] * isBlue, frequencySquare[counterDrop * 2 + 1] * isBlue, 0};
         //DEBUG(("ANALYZING X: %d Y: %d---------------------------", actualStudyPoint[0], actualStudyPoint[1]));
        float actualSquareCoord[3];
        game.square2pos(actualStudyPoint, actualSquareCoord);
        actualSquareCoord[2] = game.getTerrainHeight(actualStudyPoint);
        numHighSquare = analyzeAroundSquare(actualStudyPoint ,myState);
        //if(i++ > 0) counterDrop++; // if fails
    }while(numHighSquare < 3);
    
   // DEBUG(("CALCULATION FINISHED---------------------------")); 
    
    
}*/












//End page functions
//Begin page main
int _Phase, _Time, isBlue, actualSquare[3];
float whereToGo[3];
bool zoneFound, /*continuingToDrill,*/ stopVelAfterGeyser;
bool centerFound;
Square drilledSquare[10], maxSquareConcFound;  // check this for the final
float lastDrilled[3]; int drillIndex;

float lastEnemyScore, myLastScore;


bool heIsDoingPula;
//float centerZone[3];

void init(){
    _Phase = drillIndex = stopVelAfterGeyser = _Time = zoneFound = centerFound = /*heIsDoingPula =*//*continuingToDrill =*/ CHOOSESTRATEGY;
    memset(maxSquareConcFound.squareDrilled, 0, sizeof(int)*3);
    maxSquareConcFound.conc = 0.1f;
    lastDrilled[0] = -20.0;
    lastEnemyScore = myLastScore = 0.0f;
}

void loop(){
    float myState[13], enemyState[12], kVel = 0.048f; 
    float whereAttitude[3] = {.0f, .0f, -1.0f}, remainingFuel = game.getFuelRemaining();
    api.getMySphState(myState);
    api.getOtherZRState(enemyState);
    float rotVel=mathVecMagnitude(myState+10,3); 
    float myVel = mathVecMagnitude(myState+3, 3);
    if(_Time++<1){ isBlue = (myState[0] > 0.0f) ? 1 : -1; }
    
    float myScore = game.getScore(), enemyScore = game.getOtherScore();
    float diffEnemyScore = enemyScore - lastEnemyScore, myDiffScore = myScore - myLastScore;
    lastEnemyScore = enemyScore, myLastScore = myScore;
    bool enemyCenterFound = (diffEnemyScore >= 3.5f && enemyState[2] > 0.24f);
    bool usCenterFound = (myDiffScore >=3.5f && myState[2] > 0.24f);
    DEBUG(("maxSquareConcFound.conc: %f", maxSquareConcFound.conc));
    DEBUG(("diffEnemyScore: %f enemyFoundCenter: %d usCenterFound %d", diffEnemyScore, enemyCenterFound, usCenterFound));
    
    
    
    float dropPoint[3];
    memcpy(dropPoint, myState, sizeof(float)*3);
    mathVecNormalize(dropPoint, 3);
    for(int i = 0; i<3; i++){ dropPoint[i] *= 0.235f; }
    heIsDoingPula = (getDistance(enemyState, dropPoint, 3)< 0.15f);
    

    if(usCenterFound || enemyCenterFound){ 
        DEBUG(("trovata"));
        centerFound = zoneFound = true;
        maxSquareConcFound.conc = 1.0f;
        
        //game.pos2square((centerFound ? myState : enemyState), maxSquareConcFound.squareDrilled);
        memcpy(maxSquareConcFound.squareDrilled, ((usCenterFound) ? myState : enemyState), sizeof(float)*3);
        //maxSquareConcFound.squareDrilled[2] = game.getTerrainHeight(maxSquareConcFound.squareDrilled);
        if(enemyCenterFound){
            maxSquareConcFound.squareDrilled[0] *=-1;
            maxSquareConcFound.squareDrilled[1] *=-1;
        }
        
        
        /*int tempSquare[3];
        game.pos2square(maxSquareConcFound.squareDrilled, tempSquare);
        DEBUG(("FOUND AT SQUARE: %d %d", tempSquare[0],  tempSquare[1]));
        
        DEBUG(("FOUND AT coord: %f %f", maxSquareConcFound.squareDrilled[0],  maxSquareConcFound.squareDrilled[1]));*/
        
        
        
    }
    
    if(_Phase == STARTDRILL){
        
        //continuingToDrill = false;
        stopVelAfterGeyser = false;
        whereAttitude[2] = -.5 * isBlue;
        api.setAttRateTarget(whereAttitude);
        
        if(game.checkSample()){
            game.pickupSample();
            memcpy(drilledSquare[drillIndex].squareDrilled, myState, sizeof(float)*3);
            drilledSquare[drillIndex].actualDrillIndex = drillIndex;
            memcpy(lastDrilled, myState, sizeof(float)*3);
            /*memcpy(drilledSquare[drillIndex].squareDrilled, actualSquare, sizeof(int)*3);
            drilledSquare[drillIndex].actualDrillIndex = drillIndex;
            game.square2pos(actualSquare, lastDrilled);
            lastDrilled[2] = game.getTerrainHeight(actualSquare);*/
            drillIndex++;
        }
        _Phase = STOPDRILL;
    }
    
    int sampleHeld = game.getNumSamplesHeld();
    bool isGeyserHere = game.isGeyserHere(myState);
    DEBUG(("sampleHeld : %d", sampleHeld));
    bool drillInterrupt = (_Time >= 160  && _Time < 170 &&  sampleHeld >=2 && remainingFuel > 0.1f && !heIsDoingPula);    //interrupt mascherabile
    if(_Phase == STOPDRILL){
        if((game.getDrills(myState) == 2 && sampleHeld >=5 && !isGeyserHere) /*|| sampleHeld >=5*/ || (heIsDoingPula && sampleHeld >=4)){ game.dropSample(sampleHeld - 1); sampleHeld--; }//drop l'ultima
        if((drillInterrupt || game.getDrillError() || isGeyserHere || game.getDrills(myState)>=3 || sampleHeld == 5)){ 
            game.stopDrill();
            _Phase = CHOOSESTRATEGY;
           // heIsDoingPula = false;
            //continuingToDrill = true;
            //stopVelAfterGeyser = false; //check this
        }else{ _Phase = STARTDRILL; }
    }
    
    if(drillInterrupt) _Phase = DROP; //stop wahtever it is doing;
    
    if(_Phase == CHOOSESTRATEGY){
        if(sampleHeld>=4  && !heIsDoingPula/*|| drillInterrupt*/){
            _Phase = DROP;
        }else{
            
            /*float maxConcPointCoord[3];
            game.square2pos(maxSquareConcFound.squareDrilled, maxConcPointCoord);
            maxConcPointCoord[2] = game.getTerrainHeight(maxSquareConcFound.squareDrilled);*/
            
            
            getNearSquare(myState, ((zoneFound) ? maxSquareConcFound.squareDrilled : myState),enemyState);
            _Phase = CHOOSESAMPLE; 
        }
    }
    
    float distanceMeToEnemy = getDistance(myState, enemyState, 3);
    if(_Phase == CHOOSESAMPLE){
        kVel = 0.045f; //0.045
        if(distanceMeToEnemy < 0.38 /*&& mathVecMagnitude(enemyState, 3) > 0.01f*/){ kVel = 0.08f; }
        game.square2pos(actualSquare, whereToGo);
        whereToGo[2] = game.getTerrainHeight(actualSquare) - (0.11f + 0.025);
        
        if(getDistance(myState, whereToGo, 3) <=  0.015f/*0.015*/ && myVel < 0.01f && rotVel < 0.04f){ // range 0.33-0.37
            _Phase = STARTDRILL;
            if(game.getDrillEnabled() == false){ game.startDrill();  }
            //stopVelAfterGeyser = false;
        }
    }
    
    if(_Phase == DROP){
        
        //kVel = 0.055f;
        //stopVelAfterGeyser = false;
        if(mathVecMagnitude(enemyState, 3) < 0.24f && distanceMeToEnemy < 0.3f){ kVel = 0.07f;  }//when he is nearest to out drop targt we reduce the vel
       /* memcpy(whereToGo, myState, sizeof(float)*3);
        mathVecNormalize(whereToGo, 3);
        //continuingToDrill = false;
        
    
        for(int i = 0; i<3; i++){ whereToGo[i] *= 0.235f; }*/
        // for(int i = 0; i<3; i++){ whereToGo[i] *= 0.249f; }
        memcpy(whereToGo, dropPoint, sizeof(float)*3);
        if(game.atBaseStation()){
            //if(isBlue)counterDrop++;
            for(int i = 0; i<5; i++){
                float point = game.dropSample(i);
                if(point != 0.0f){
                    Square square = drilledSquare[i];
                    if(point > maxSquareConcFound.conc){ 
                        DEBUG(("FOUND A BETTER ONE"));
                        zoneFound = true;
                        maxSquareConcFound = square;
                        maxSquareConcFound.conc = point;
                    }
                } 
            }
            drillIndex = 0;
            if(remainingFuel > 0.1f/*0.08f*/) _Phase = CHOOSESTRATEGY; //else {_Phase = PULA; } //only when we have enough fuel
            
           // if(getDistance(myState, whereToGo, 3) < ){ _Phase }
        }
        DEBUG(("d to Drop: %f", getDistance(enemyState, whereToGo, 3)));
        if(heIsDoingPula){ _Phase = CHOOSESTRATEGY; heIsDoingPula = true; DEBUG(("STA FACENDO LAPULA"));}
        //if((getDistance(whereToGo, enemyState, 3) < 0.15f)){_Phase = CHOOSESAMPLE;}
    }
    
    /*if(_Phase == PULA){
        kVel = 0.1f;
        memcpy(whereToGo, enemyState, sizeof(float)*3);
        mathVecNormalize(whereToGo, 3);
        for(int i=0;i<3;i++) whereToGo[i] *= 0.145f;
        //if((game.getScore() - game.getOtherScore()) < 15.0f){ _Phase = CHOOSESTRATEGY; }
    }*/
    
    if (remainingFuel <= 0.02f  && myState[2] > 0.24f ) { memset(whereToGo, 0.0f, sizeof(float)*3); }
    float distance = getDistance(myState, whereToGo, 3);
    //if(continuingToDrill && distance < 0.22f) kVel = 0.035f; // 0.038  //era inutile
    //if(stopVelAfterGeyser && _Phase != DROP) kVel = 0.02f;//0.025f;//0.012 // we need distance control because without that the SATELLITE will exceed the trajectory
	//if(myVel > 0.03f && _Phase != DROP && !stopVelAfterGeyser){ kVel = 0.055;/*DEBUG(("SETPOS")); api.setPositionTarget(myState);*/ }
	//float velocity = (((distance*0.0775f)/(kVel))/0.56553f)/14.0f;
	//float dbrake = (myVel*myVel) / (0.04f/*2*0.02f*/);
    if(fabsf(distance - (myVel*myVel) / (0.04f/*2*0.02f*/)) < 0.05f && distance > 0.015f && myVel > 0.015f && !stopVelAfterGeyser){ kVel = 0.09f; }
    if(stopVelAfterGeyser && distance > 0.011f && myVel < 0.01f) kVel = 0.02f;
    
    //float vBreak =  sqrtf(distance * 0.01f);
    // DEBUG(("myVel: %f dbrake: %f distance: %f vBreak: %f dist-dbrake: %f %d %d",myVel, dbrake, distance, vBreak,fabs(distance - dbrake), dbrake < distance , fabs(distance - dbrake) < 0.1f));
    // if(myVel > vBreak && distance > 0.01f && distance < 0.1f ){ kVel = 0.07f; }
    
    float velocity = (distance * 0.009789f)/kVel;
    DEBUG(("dist: %f calcVel: %f actualVel: %f kVel : %f angularVel: %f stopVelAfterGeyser: %d", distance, velocity, mathVecMagnitude(myState+3,3), kVel, mathVecMagnitude(myState+10,3), stopVelAfterGeyser));
	if(velocity > 0.05f){ velocity = 0.05f;}
	//if(mathVecMagnitude(myState+3,3) > 0.030f && velocity > 0.03f && _Phase != DROP){ velocity = 0.028;/*DEBUG(("SETPOS")); api.setPositionTarget(myState);*/ }
    DEBUG(("AFTER CALCULATION VEL :%f", velocity));
    float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);

    /*if(stopVelAfterGeyser && _Phase != DROP){
        for(int i = 0; i<3; i++){ myState[i+3]*=3.0f; }
        mathVecSubtract(velocityVector,ƒ whereToGo, myState, 3);
        mathVecSubtract(velocityVector, velocityVector, myState+3, 3);
        velocity*=3;
	}*/
	mathVecNormalize(velocityVector, 3);
	
    if(isGeyserHere){ //when we go to drop sometimes there is a geyser between us and the drop zone 
        stopVelAfterGeyser = (distance < 0.21f) && _Phase == CHOOSESAMPLE; // only if the square is really nearest to us
        //for(int i=0;i<3;i++) velocityVector[i]*=4.0f;
	    
        api.setForces(velocityVector);
	}else{
	   
	    
	    for(int i=0;i<3;i++) velocityVector[i]*=velocity;
	    api.setVelocityTarget(velocityVector);
	    
	}

	{//rotation and failover(CLOUD AWS) MIMD CLUSTER async block
        if(_Phase == DROP){
             api.setAttitudeTarget(whereAttitude);
        }else{
            
            /*if(_Phase == STARTDRILL){
                whereAttitude[2] = -.5 * isBlue;
                api.setAttRateTarget(whereAttitude);
            }else{
                whereAttitude[2] = myState[8] = myState[9] = 0.00f;
                api.setQuatTarget(myState+6);
                if(stopVelAfterGeyser){ api.setAttRateTarget(whereAttitude); }
            }*/
            
            whereAttitude[2] = myState[8] = myState[9] = 0.00f;
            api.setQuatTarget(myState+6);
            if(stopVelAfterGeyser){ api.setAttRateTarget(whereAttitude); }
            
            
            
        }
       // if(myState[2] > 0.16f && getDistance(myState, enemyState, 3) < 0.35f){  api.setVelocityTarget(new float[3]{0}); }
	}
	//if(_Time >= 160  && _Time < 170 &&  game.getNumSamplesHeld() >=2 && remainingFuel > 0.1f ){ _Phase = DROP; if(game.getDrillEnabled()) game.stopDrill();}
}
//End page main
//Begin page map the area
//bool geyserCheck(float newSquare[3], float k){
//    //if lastDrilled is not setted it means that the satellite did no drill, so he have to return true
//    return (lastDrilled[0] == -20.0f || continuingToDrill) ? true : (( ((game.isGeyserHere(lastDrilled)) ? getAngleBetween2D(newSquare, lastDrilled) >= k : true)/*canContinue*/ /*&& getDistance(myState, newSquare, 3) > 0.21*/));
//}
void getNearSquare(float myState[13], float target[3],float *enemyState){ //used till now
    float dist=30.0f;
    for(int i = -8; i<=8; i++){
        for(int j = -10; j <=10; j++){
            int maxIndex = (_Time > 125 || zoneFound) ? 2 : 3;
            //int maxIndex = 3;
             // int calcSquare[2] = {i*isBlue + (zoneFound ? maxSquareConcFound.squareDrilled[0] : 0 ), j*isBlue + (zoneFound ? maxSquareConcFound.squareDrilled[1] : 0 )};
            int calcSquare[2] = {i*isBlue , j*isBlue };
            if(abs(calcSquare[0])>maxIndex || abs(calcSquare[1])>maxIndex /*|| nearest*/){ //(Ex. (5,2)) 2 da false, mentre 5 da true, quindi la condizione è verificata senza problemi
                float squareHeight = game.getTerrainHeight(calcSquare);
                
                if((squareHeight <= ((zoneFound)? 0.49 : 0.41f) || centerFound) /*|| (_Time > 125 && squareHeight <= 0.48f)*/){
                    float pos[3];game.square2pos(calcSquare,pos); pos[2]=squareHeight;
                    // DEBUG(("CANIGOTHERE?? :%d", geyserCheck(pos, 0.2f, myState)));
                    float tempDist = getDistance(pos,target,2); /*+ (zoneFound ? mathVecMagnitude(pos, 3) : 0);*/
                    bool geyserCheck = (lastDrilled[0] == -20.0f || myState[2] > 0.24f/*|| continuingToDrill*/) ? true : (( ((game.isGeyserHere(lastDrilled)) ? getAngleBetween2D(pos, lastDrilled) >= 0.2f : true)));
                    //DEBUG(("myState[2] > 0.24f: %d lastDrilled[0] == -20.0f: %d geyserCheck: %d", myState[2] > 0.24f, lastDrilled[0] == -20.0f,geyserCheck));
                    
                    if(tempDist<dist && game.getDrills(calcSquare)==0 && geyserCheck && getDistance(pos,enemyState,3)>0.323f){
                        dist=tempDist;
                        memcpy(actualSquare,calcSquare,sizeof(int)*2);
                    }
                }
            }
        }
    }
}
//End page map the area
//Begin page math
float getDistance(float *a, float *b, int length){
    float distance[3];
    mathVecSubtract(distance,a,b, length);
    return mathVecMagnitude(distance,length);
}

float getAngleBetween2D(float *vectorA, float *vectorB){ //return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 2) / (mathVecMagnitude(vectorA, 2) * mathVecMagnitude(vectorB, 2)));
}


/*public calcPar(float *x0, float *y0, float *a, float *c){
    c =(y1-y2)/(y2-y3);
    x0 =(-x1^2+x2^2+c*(x2^2-x3^2))/(2.0*(-x1+x2+c*x2-c*x3));
    a =(y1-y2)/((x1-x0)^2-(x2-x0)^2);
    y0 = y1-a*(x1-x0)^2;
    //y-y0=a*(x-x0)^2
}*/
//End page math
