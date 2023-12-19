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
    int squareDrilled[3];
    int actualDrillIndex;
    float conc;
}Square;

/*typedef struct S { 
    int x; 
} T;*/
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
int _Phase, _Time, isBlue;
float whereToGo[3];
bool zoneFound, continuingToDrill;

int actualSquare[3];
bool afterGeyserVel;
bool stopVelAfterGeyser;

Square maxSquareConcFound; // check this for the final

int drillIndex;
Square drilledSquare[10];
float lastDrilled[3];

//int distanceToDrill;
bool nearest;



void init(){
    //api.setPosGains(0.20f, 0, 2.0f);
    _Phase = drillIndex = afterGeyserVel = stopVelAfterGeyser = _Time = zoneFound  = continuingToDrill = CHOOSESTRATEGY;
//   afterGeyserVel = false;
//   stopVelAfterGeyser = false;
//   drillIndex = 0;
    //maxSquareConcFound = {.conc = 0};
    memset(maxSquareConcFound.squareDrilled, 0, sizeof(int)*3);
    maxSquareConcFound.conc = 0.0f;
   lastDrilled[0] = -20.0;
   //distanceToDrill = 0.0f;
   nearest = false;
   
   

   //DEBUG(("HELLO! v2.17"));
}

void loop(){
    float myState[13], enemyState[12], kVel = 0.048f; 
    float whereAttitude[3] = {.0f, .0f, -1.0f};
    api.getMySphState(myState);
    api.getOtherZRState(enemyState);
    if(_Time++<1){ isBlue = (myState[0] > 0.0f) ? 1 : -1; }
    //float enemyDistanceToCenter = mathVecMagnitude(enemyState, 3);
    float rotVel=mathVecMagnitude(myState+10,3);    

    if(_Phase == STARTDRILL){
        // bool isDrillEnabled = ;
        if(game.getDrillEnabled() == false){ game.startDrill();  }
        continuingToDrill = false;
        /*float myState[12];
        api.getMyZRState(myState);*/
        whereAttitude[2] = -.5 * isBlue;
        api.setAttRateTarget(whereAttitude);

        if(game.checkSample()){//controlla se abbiamo drillato abbastanza
            game.pickupSample();
            memcpy(drilledSquare[drillIndex].squareDrilled, actualSquare, sizeof(int)*3);
            drilledSquare[drillIndex].actualDrillIndex = drillIndex;
            game.square2pos(actualSquare, lastDrilled);
            lastDrilled[2] = game.getTerrainHeight(actualSquare);
            //memcpy(lastDrilled, actualSquare);
            drillIndex++;
        }
        _Phase = STOPDRILL;
    }
    
    int sampleHeld = game.getNumSamplesHeld();
    bool isGeyserHere = game.isGeyserHere(myState);
    if(_Phase == STOPDRILL){
        if((game.getDrills(myState) == 2 && sampleHeld >=5) /*|| sampleHeld >=5*/){ game.dropSample(sampleHeld - 1); sampleHeld--; }//drop l'ultima
        if((game.getDrillError() || isGeyserHere || game.getDrills(myState)>=3 || sampleHeld == 5)){ 
            game.stopDrill();
            _Phase = CHOOSESTRATEGY;
            continuingToDrill = true;
            //_Phase = DROP;
        }else{ _Phase = STARTDRILL; }
    }
    
    if(_Phase == CHOOSESTRATEGY){
        if(sampleHeld>=4 ){
            _Phase = DROP;
        }else{
            float maxConcPointCoord[3];
            game.square2pos(maxSquareConcFound.squareDrilled, maxConcPointCoord);
            maxConcPointCoord[2] = game.getTerrainHeight(maxSquareConcFound.squareDrilled);
            getNearSquare(myState, (zoneFound ? maxConcPointCoord : myState),enemyState);
            //getNearSquare(myState, myState,enemyState);
            _Phase = CHOOSESAMPLE; 
        }
    }
    
    float distanceMeToEnemy = getDistance(myState, enemyState, 3);
    if(_Phase == CHOOSESAMPLE){
        kVel = 0.045f;//0.045
        //here avoid zarosphere
        /*float enemyVel[3] = {enemyState[3],enemyState[4],enemyState[5]};
        float tempPos[3] = {-myState[0], myState[1], myState[2]};
        if(_Time<4 && fabsf(getAngleBetween(enemyVel,myState))<1.57f) //1.57~pi/2
        {
            getNearSquare(tempPos);
        }*/
        if(distanceMeToEnemy < 0.38 /*&& mathVecMagnitude(enemyState, 3) > 0.01f*/){ kVel = 0.08f; }
        game.square2pos(actualSquare, whereToGo);
        whereToGo[2] = game.getTerrainHeight(actualSquare) - (0.11f + 0.025);
        
        if(getDistance(myState, whereToGo, 3) <=  0.015f/*0.015*/ && mathVecMagnitude(myState+3,3) < 0.01f && rotVel < 0.04f){ // range 0.33-0.37
            _Phase = STARTDRILL;
            stopVelAfterGeyser = afterGeyserVel = false;
        }
    }
    
    float remainingFuel = game.getFuelRemaining();
    
    if(_Phase == DROP){
        //kVel = 0.055f;
        stopVelAfterGeyser = afterGeyserVel = false;
        if(mathVecMagnitude(enemyState, 3) < 0.24f && distanceMeToEnemy < 0.3f){ kVel = 0.07f;  }//when he is nearest to out drop targt we reduce the vel
        memcpy(whereToGo, myState, sizeof(float)*3);
        mathVecNormalize(whereToGo, 3);
        continuingToDrill = false;
        for(int i = 0; i<3; i++){ whereToGo[i] *=/* mathVecMagnitude(myState, 3)*/0.235f; }
        // for(int i = 0; i<3; i++){ whereToGo[i] *= 0.249f; }
        if(game.atBaseStation()){
            //if(isBlue)counterDrop++;
            for(int i = 0; i<5; i++){
                bool boolsamples[5];
                game.getSamplesHeld(boolsamples);
                if(boolsamples[i]){
                    float point = game.dropSample(i);
                    Square square = drilledSquare[i];
                    if(point >= 0.3f && point > maxSquareConcFound.conc){ 
                        zoneFound = true;
                        maxSquareConcFound = square;
                        maxSquareConcFound.conc = point;
                        //distanceToDrill = (point == 0.3f) ? 2 : 1
                        nearest = true;
                        
                    }
                    
                    
                    
                    //DEBUG(("MAXSQUARE: X: %d Y: %d CONC: %f", maxSquareConcFound.squareDrilled[0], maxSquareConcFound.squareDrilled[1], maxSquareConcFound.conc));
                    //DEBUG(("SQUARE X:%d Y:%d CONC:%f DRILLINDEX:%d",square.squareDrilled[0],square.squareDrilled[1], point, square.actualDrillIndex ));
                }
                
               
            } 
            //memset(drilledSquare, );
            drillIndex = 0;
            if(remainingFuel > 0.1f/*0.08f*/) _Phase = CHOOSESTRATEGY; //else {_Phase = PULA; }
        }
        //if((getDistance(whereToGo, enemyState, 3) < 0.15f)){_Phase = CHOOSESAMPLE;}
    }
    
    /*if(_Phase == PUSHBOTTOM){
        
    }*/
    
    /*if(_Phase == PULA){
        kVel = 0.1f;
        memcpy(whereToGo, enemyState, sizeof(float)*3);
        mathVecNormalize(whereToGo, 3);
        for(int i=0;i<3;i++) whereToGo[i] *= 0.145f;
        //if((game.getScore() - game.getOtherScore()) < 15.0f){ _Phase = CHOOSESTRATEGY; }
    }*/
    
    if (remainingFuel <= 0.02 ) { memset(whereToGo, 0.0f, sizeof(float)*3); }

    float distance = getDistance(myState, whereToGo, 3);
    if(continuingToDrill && distance < 0.22f) kVel = 0.035f; // 0.038 
    if(afterGeyserVel && _Phase != DROP) kVel = 0.009f;//0.025f;//0.012 // we need distance control because without that the SATELLITE will exceed the trajectory
	float velocity = (((distance*0.0775f)/(kVel))/0.56553f)/14.0f;
	if(velocity > 0.05f){ velocity = 0.05f;}
    DEBUG(("dist: %f calcVel: %f actualVel: %f kVel : %f angularVel: %f", distance, velocity, mathVecMagnitude(myState+3,3), kVel, mathVecMagnitude(myState+10,3)));
	float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);
    mathVecNormalize(velocityVector, 3);
    if(isGeyserHere){ //when we go to drop sometimes there is a geyser between us and the drop zone 
        afterGeyserVel = stopVelAfterGeyser = (distance < 0.21f); // only if the square is really nearest to us
        api.setForces(velocityVector);
	}else{
	    /*float meToSquare[3];
	    mathVecSubtract(meToSquare, whereToGo, myState, 3);*/

	    for(int i = 0; i<3; i++){ myState[i+3]*=3.0f; }
	    
	    if(stopVelAfterGeyser && _Phase != DROP){
	        //float meToSquare[3];
	        mathVecSubtract(velocityVector, whereToGo, myState, 3);
	        //DEBUG(("TRYING TO STOP FORCES"));
	        mathVecSubtract(velocityVector, velocityVector, myState+3, 3);
	        mathVecNormalize(velocityVector, 3);
	        velocity*=3;
	    }
	    
	    for(int i=0;i<3;i++) velocityVector[i]*=velocity;
	    api.setVelocityTarget(velocityVector);
	    
	}

	{//rotation and failover(CLOUD AWS) async block
        if(_Phase == DROP){ // A LOT OF FUEL WHEN THE ROTATION IS FAST
             api.setAttitudeTarget(whereAttitude);
        }else{
            myState[8] = myState[9] = 0.00f;
            api.setQuatTarget(myState+6);
            
            if(afterGeyserVel /*|| continuingToDrill*/){
                //for(int i = 0; i<3; i++){ myState[i+10]*=-1; }
                float nullVector[3] = {0,0, -myState[12] * 1.0f};
                //api.setAttRateTarget(nullVector);
                if(rotVel < 0.04f){
                    nullVector[0] = nullVector[1] = nullVector[2] = 0.0f; 
                }
                
                api.setAttRateTarget(nullVector);
            }
            
        }
       // if(myState[2] > 0.16f && getDistance(myState, enemyState, 3) < 0.35f){  api.setVelocityTarget(new float[3]{0}); }
	}
	//_time-mathVecMagnitude(myState,3)/0.075<=0 davide's formula
	//if(remainingFuel )
	//if(remainingFuel<=0.02){float center[3]; center[0]=0.0f;center[1]=0.0f;center[2]=-1.0f;api.setForces(center);}
	if(_Time >= 160  && _Time < 170 &&  game.getNumSamplesHeld() >=2 && remainingFuel > 0.1f ){ _Phase = DROP; if(game.getDrillEnabled()) game.stopDrill();}
	//DEBUG(("QUAT %f %f %f %f", myState[6], myState[7], myState[8], myState[9]));
}
//End page main
//Begin page map the area
/*void getNearSquare(float target[3]){ // GiovannisTouchy Test Geyser Avoid
    float dist=3.0f;
    //float center[3] = {0.0f,0.0f,0.0f};
    for(int i = -8; i<8; i++){
        for(int j = -10; j < 10; j++){
            int calcSquare[2] = {i*isBlue, j*isBlue};
            float squareHeight = game.getTerrainHeight(calcSquare);
            float pos[3];game.square2pos(calcSquare,pos);pos[2]=squareHeight;
            float angleActualSquareNextSquare = getAngleBetween(target, pos);
            // angleNowThen = angleBetween(whereToGo,pos,2);
            // if(angleNowThen) cambia
            bool geyserHere = game.isGeyserHere(target);
            bool range=(drillMade==0)?(mathVecMagnitude(pos,2)>0.4):(mathVecMagnitude(pos,2)>0.24f);
            if(squareHeight == 0.40f && ( (!geyserHere) || (geyserHere && angleActualSquareNextSquare > 0.187))){
                DEBUG(("X: %d Y: %d ANGLE: %f",calcSquare[0], calcSquare[1], angleActualSquareNextSquare ));
                
                if(getDistance(pos,target,3)<dist && game.getDrills(calcSquare)==0 && (range && mathVecMagnitude(pos,2)<0.5f)){
                    dist=(getDistance(pos,target,3));
                    memcpy(actualSquare,calcSquare,sizeof(int)*2);
                }

            }
        }
    }
    
}*/




/*void getNearSquare(float target[3]){ //used till now
    float dist=3.0f;
    //float center[3] = {0.0f,0.0f,0.0f};
    for(int i = -8; i<8; i++){
        for(int j = -10; j < 10; j++){
            int calcSquare[2] = {i*isBlue, j*isBlue};
            float squareHeight = game.getTerrainHeight(calcSquare);
            if(squareHeight == 0.40f || (_Time > 125 && squareHeight <= 0.48f)){
                float pos[3];game.square2pos(calcSquare,pos);pos[2]=squareHeight;
                
                if(getDistance(pos,target,3)<dist && game.getDrills(calcSquare)==0 && mathVecMagnitude(pos,2)>0.24
                && mathVecMagnitude(pos,2)<0.7f){
                    dist=(getDistance(pos,target,3));
                    memcpy(actualSquare,calcSquare,sizeof(int)*2);
                }
                //this checks the distance to go to the point and to return to the base station
                
            }
        }
    }
}*/

bool geyserCheck(float newSquare[3], float k){
    /*float geyser[10][2];
    game.getGeyserLocations(geyser);
    bool thereIsProblem = false;
    int index = 0;
    while(!thereIsProblem && index<10){
        float geyserCoord[3]; game.square2pos(geyserCoord);
        index++;
    }
    return getAngleBetween(newSquare, ) > k  && _Phase==DROP;
    return thereIsProblem;*/
    /*float myState[12];
    api.getMyZRState(myState);*/
    // DEBUG(("ANGLE CAL: %f distMeToSquare: %f",getAngleBetween(newSquare, lastDrilled),getDistance(myState, newSquare, 3)));
    //bool canContinue = ((game.isGeyserHere(lastDrilled)) ? getAngleBetween(newSquare, lastDrilled) >= k : true) ;
    
    return (lastDrilled[0] == -20.0f || continuingToDrill) ? true : (( ((game.isGeyserHere(lastDrilled)) ? getAngleBetween2D(newSquare, lastDrilled) >= k : true)/*canContinue*/ /*&& getDistance(myState, newSquare, 3) > 0.21*/));
    //if lastDrilled is not setted it means that the satellite did no drill, so he have to return true
    //return  // 0.21 is totally empiric

}
void getNearSquare(float myState[13], float target[3],float *enemyState){ //used till now
    float dist=3.0f;
    //float center[3] = {0.0f,0.0f,0.0f};
    //int height = (nearest) ? 1 : 8;
    //int width = (nearest) ? 1 : 10;
    for(int i = -8; i<=8; i++){
        for(int j = -10; j <=10; j++){
            int maxIndex = (_Time > 125) ? 2 : 3;
            int calcSquare[3] = {i*isBlue + (zoneFound ? maxSquareConcFound.squareDrilled[0] : 0 ), j*isBlue + (zoneFound ? maxSquareConcFound.squareDrilled[1] : 0 ), 0};
            
            
            if(abs(calcSquare[0])>maxIndex || abs(calcSquare[1])>maxIndex /*|| nearest*/){
                float squareHeight = game.getTerrainHeight(calcSquare);
                //DEBUG(("SQUARE HEIGHT: %f %d %f", squareHeight, (float)squareHeight <= (float) 0.49f, squareHeight - 0.48f));
                // if(squareHeight-(zoneFound?0.48:0.4)==0)
                if((squareHeight <= (zoneFound? 0.48f : 0.40f))){//essendo lìultimo codice inviato..ho lasciato il bug, nei nuovi codici ovviamente è fixato
                    //DEBUG(("squareHeight :%f  zoneFound : %d",squareHeight,zoneFound));
                    float pos[3];game.square2pos(calcSquare,pos); pos[2]=squareHeight;
                    // DEBUG(("CANIGOTHERE?? :%d", geyserCheck(pos, 0.2f, myState)));
                    float tempDist = getDistance(pos,target,3) + (zoneFound ? mathVecMagnitude(pos, 3) : 0);
                    if(tempDist<dist && game.getDrills(calcSquare)==0 && geyserCheck(pos, 0.2f) && getDistance(pos,enemyState,2)>0.323f){
                        dist=tempDist;
                        memcpy(actualSquare,calcSquare,sizeof(int)*2);
                    }
                }
            }
        }
    }
}

/*

void avoidGeyser(float *myState){
    float forceToAvoid[3];
    int map[10][2];game.getGeyserLocations(map);
    for(int i=0;i<10;i++){
        float mapPos[2];game.square2pos(map[i],mapPos);
        float m = ((whereToGo[1]-myState[1])/(whereToGo[0]-myState[0]));
        float q = -m + whereToGo[1];
        if(mapPos[1] == ((mapPos[0]*m)+q)){
            DEBUG(("Geyser tra di noi"));
        }
    }
}*/
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
