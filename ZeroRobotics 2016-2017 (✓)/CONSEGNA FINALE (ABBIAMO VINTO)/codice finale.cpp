//Begin page CONSTANT
#define SPSPLACEMENT 0
#define DOCK 1
#define DROP 10
#define STAY 11
#define FLOATSIZE3 12
//End page CONSTANT
//Begin page CORE
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
bool isFacingPos(float myState[12], float lookPos[3], float tollerance, int valuePos){
    float realPos[3];  
    mathVecSubtract(realPos, lookPos, myState, 3); 
    mathVecNormalize(realPos,3);
    mathVecNormalize(myState+valuePos,3);
    //DEBUG(("TEST: %f",acosf(mathVecInner(realPos, myState+valuePos, 3))));
    return acosf(mathVecInner(realPos, myState+valuePos, 3))<tollerance;
}

bool newIsInBound(float *itemState){
    return !(fabs(itemState[0]) > 0.54 || fabs(itemState[1]) > 0.7f || fabs(itemState[2]) > 0.54f);
}


//End page CORE
//Begin page itemAvoid
bool willCollide(float myState[3], float itemFace[3], float itemPos[3]){
    return getDistance(myState, itemPos) < getDistance(myState, itemFace);
}
//End page itemAvoid
//Begin page itemFunctions
void calculateRightSecondSPS(float myState[12], float itemPos[12], float secondSPS[3], float A[3], float rightDistanceAverage){
    #define howIncrease 0.01f
    #define minArea 0.12500f
    float attitudeVector[3], CVERA[3], lastDist = 3.00f;
    do{
        game.getItemZRState(itemPos, actualID);
        memcpy(attitudeVector, itemPos, FLOATSIZE3);
        for(int i=0;i<3;i++) CVERA[i] = itemPos[i] + (itemPos[6+i] * rightDistanceAverage);
        canPickFirst = !(willCollide(myState, CVERA, itemPos));
        do{
            for(int i=0;i<3;i++) attitudeVector[i] = attitudeVector[i] + (itemPos[6+i] * howIncrease);
            for(float i = 0.0f; i<=3.0f; i+=0.01f){
                int axis = 0;
                for(int j = 0;j<6;j++){
                    float actualPoint[3];// = {attitudeVector[0], attitudeVector[1], attitudeVector[2]};
                    memcpy(actualPoint, attitudeVector, FLOATSIZE3);
                    // 0 0 1 1 2 2
                    axis += (j==2 || j==4);
                    actualPoint[axis] += (j%2 == 0) ? i : -i;
                    float AB[3], AC[3], crossBetween[3];
                    mathVecSubtract(AB, actualPoint, A, 3);
                    mathVecSubtract(AC, CVERA, A, 3);
                    mathVecCross(crossBetween, AB, AC);
                    float dist = getDistance(itemPos, actualPoint);
                    
                    if(getDistance(A, CVERA)>0.2f && dist<lastDist && ((mathVecMagnitude(crossBetween, 3)/2) > minArea) &&  ((!canPickFirst) ?  !willCollide(actualPoint, CVERA, itemPos) : true) ){
                        if(newIsInBound(actualPoint)){
                            memcpy(secondSPS, actualPoint, FLOATSIZE3);
                            lastDist = dist;
                        }
                    }
                } 
            }
        }while(newIsInBound(attitudeVector));
        
        if(lastDist == 3.0f) actualID+=2;
    }while(lastDist == 3.0f && actualID < 6);
}

//End page itemFunctions
//Begin page main
int Phase, actualID, Time, Color, droppedItems, idealID,otherID;
float A[3], calcSPS[3];
float allFacePOI[6][6];
float zoneInfo[4], enemyZone[3];
bool canPickFirst;

int idProva;
void init(){
    idProva = otherID = 10;
    Phase = actualID = droppedItems = Time = 0;
    game.dropSPS();
    calcSPS[0] = -20.0f;
}

void loop(){
    float whereToGo[3], whereAttitude[3], realItemState[12];
    float myState[12], enemyState[12];
    
    float kVel = 0.07f;
    
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    
    float distanceAvarage[3] = { .162f, .149f, .135f}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
     
     
    if(Time++<1){ 
        actualID = Color = myState[1] < 0;
        memcpy(A, myState, FLOATSIZE3);
        for(int i = 0;i<6;i++){
            float tempState[12];
            game.getItemZRState(tempState, i);
            for(int j = 0;j<3;j++){
                allFacePOI[i][j] = tempState[j] + (tempState[6+j] * distanceAvarage[game.getItemType(i)]);
                allFacePOI[i][j+3] = tempState[j];
            }
        }
    }
    
    int SPSHeld = game.getNumSPSHeld();
    int itemType = game.getItemType(actualID);
    
    game.getItemZRState(realItemState, actualID);
    
    for(int i=0;i<3;i++){
        whereToGo[i] = realItemState[i] + (realItemState[6+i] * distanceAvarage[itemType]);
        whereAttitude[i] = -realItemState[6+i];
    }
    
    bool isGoingToB = ((SPSHeld==2 && !canPickFirst) || (canPickFirst && SPSHeld==1));
    if(Phase == SPSPLACEMENT){
        if(calcSPS[0] == -20.0f){
            calculateRightSecondSPS(myState, realItemState, calcSPS, A, distanceAvarage[itemType]);
        }
        
        memcpy(whereToGo, (isGoingToB) ? calcSPS : whereToGo, FLOATSIZE3);
        
        if(isGoingToB){ kVel = 0.0435; }
        
        if(getDistance(myState, whereToGo)<0.04f){
            if(canPickFirst && SPSHeld == 1){
                Phase = DROP;
            }
            if(isGoingToB) game.dropSPS();
        }
    }
    
    float realItemVel = mathVecMagnitude(realItemState+3, 3);
    float distanceToItem = getDistance(myState, realItemState);
    float enemyVel = mathVecMagnitude(enemyState+3,3);
    
    if(Phase<2){
        if(droppedItems==1){
            DEBUG(("distaaa : %f",getDistance(enemyState,enemyZone)));
            if(idProva != 10){
                DEBUG(("PROVA: %f",getDistance(myState,enemyZone)));
                if(isFacingPos(enemyState,enemyZone,0.55f,3)){
                    kVel = 0.16f;
                    DEBUG(("FACING"));
                    if(game.hasItem(idProva)==0){
                        actualID = (getDistance(myState,enemyZone)>0.28f) ? idProva : otherID;
                    }
                }
            }
            
            if(enemyVel>0.035f){
                if(isFacingPos(enemyState,zoneInfo,0.3f,3)){
                    Phase = STAY;
                    DEBUG(("It's so cold, I come back home"));
                }
            }
        }
        
        if((((realItemVel <= 0.0f) && realItemState[0] == allFacePOI[actualID][0+3]) || realItemVel > 0.0f) && ((!isGoingToB) || SPSHeld == 0)){
            DEBUG(("I'll wait"));
            memcpy(whereToGo, allFacePOI[actualID], sizeof(float)*3);
        }
        
        
        
        if(game.hasItem(actualID) == 0 && distanceToItem < distanceAvarage[itemType] + 0.011f && distanceToItem > distanceAvarage[itemType] - 0.011f && mathVecMagnitude(myState+3,3) < 0.01f ){
            if( isFacingPos(myState, realItemState, 0.25f, 6)){
                DEBUG(("posso DOCKARE"));
                if(game.dockItem(actualID)){
                    if(SPSHeld >=1) game.dropSPS(); 
                    if(SPSHeld <= 1 ) Phase = DROP;
                }
            }
        }
    }
    
    game.getZone(zoneInfo);
    for(int i = 0;i<3;i++) enemyZone[i] = -zoneInfo[i];
        
     
    if(Phase == DROP){
        mathVecSubtract(whereToGo,myState,zoneInfo,3);
        mathVecNormalize(whereToGo,3);
        
        for(int i = 0;i<3;i++){ whereToGo[i] *= distanceToItem; }
        
        mathVecAdd(whereToGo,zoneInfo,whereToGo,3);
        mathVecSubtract(whereAttitude,zoneInfo,myState,3);
        
        if(droppedItems == 0) kVel = 0.05f;
        
        if(getDistance(realItemState, zoneInfo) < 0.046f){ 
                game.dropItem();
                droppedItems++;
                Phase = 30;
        }
        
    }
    
    if(otherID>5){
        for(int i=0; i<6; i++){
            if(game.hasItem(i)==2)
                otherID = i;
        }
    }
    
    if(Phase==30){
        memcpy(A,whereToGo,sizeof(float)*3); //save item side in A
        
        Phase = STAY;
        float min = 5.0f;
        if(droppedItems<2){   
            for(int i=0; i<4; i++){
                float distance = getDistance(myState,allFacePOI[i]+3); 
                DEBUG(("DISTANCE %f ITEM %d",distance,i));
                if(distance<min && i!=actualID && i!=otherID){
                    min = distance;
                    idealID = i;
                }
            }
            float range = (getDistance(zoneInfo, enemyZone)>1.0f) ? 0.5f : 0.45f;
            
            if(min < range){
                Phase = DOCK;
                actualID = idealID;
                DEBUG(("PRENDO ITEM PIU VICINO"));
            }
            else{
                calcSPS[0] = 46.0f;
            }
            
        }
        
    }
    
    if(Phase==STAY){
        DEBUG(("STAY"));
        bool isWaiting = getDistance(enemyState,zoneInfo)<0.5 && enemyVel < 0.004f;
        if(isWaiting){
            DEBUG(("You can't steal at thief home."));
        }
        else{
            DEBUG(("Enemy is moving"));
        }
        
        for(int i = 0; i<4; i++){
            if( i!=actualID && i!=otherID && calcSPS[0]==46.0f ){
                if(itemType<=game.getItemType(otherID)){    
                    if((isFacingPos(enemyState,allFacePOI[i],0.3f,3) && enemyVel > 0.025f) || (getDistance(enemyState,allFacePOI[i])<0.3f&&enemyVel<0.02f)){    
                        if(getDistance(myState,enemyState)>0.45f){
                            DEBUG(("AVVERSARIO VERSO %d",i));
                            idProva = i;
                            Phase = DOCK;
                            actualID = otherID;
                        }
                    }
                }
            }
        }
        
        memcpy(whereToGo,(droppedItems>1) ? zoneInfo : A ,sizeof(float)*3);
        
        if(Time==134 && !game.itemInZone(otherID) && !isWaiting){
            Phase = DOCK;
            actualID = otherID;
        }

    }
    
    float distance = getDistance(myState, whereToGo);
	
	float velocity = (((distance*0.0775f)/(kVel))/0.56553f)/14.0f;
	if(velocity> 0.08) velocity = 0.08f;
    if(velocity>0.065f && SPSHeld==0) velocity = 0.065f; 
    if(distance>=0.47f && velocity>=0.06 && Phase==SPSPLACEMENT && !isGoingToB) velocity = 0.06f;
    
    DEBUG(("kVEL:    %f",kVel));
    DEBUG(("VEL: %f REALVEL: %f", velocity, mathVecMagnitude(myState+3,3)));
    DEBUG(("actualID: %d",actualID));
    
    float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);
    mathVecNormalize(velocityVector, 3);
    for(int i=0;i<3;i++) velocityVector[i]*=velocity;
    api.setVelocityTarget(velocityVector);
    
    
    mathVecNormalize(whereAttitude, 3);
    api.setAttitudeTarget(whereAttitude);
}



//End page main
