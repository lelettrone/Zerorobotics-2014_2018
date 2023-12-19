//Begin page CONSTANT
#define SPSPLACEMENT 0
#define DOCK 1
#define DROP 10


#define FLOATSIZE3 12
//End page CONSTANT
//Begin page CORE
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}

bool isFacingPos(float myState[12], float lookPos[3], float tollerance){
    float realPos[3];    
    mathVecSubtract(realPos, lookPos, myState, 3); 
    mathVecNormalize(realPos,3); mathVecNormalize(myState+6,3);
    return acosf(mathVecInner(realPos, myState+6, 3))<tollerance;
}

/*float getAngleBetweenVector(float vectorA[3], float vectorB[3], int len){
    return acosf(mathVecInner(vectorA, vectorB, len) / (mathVecMagnitude(vectorA, len) * mathVecMagnitude(vectorB, len)));
}*/

//#define newIsInBound(itemState) !(fabsf(itemState[0]) > 0.56 || fabsf(itemState[1]) > 0.72f || fabsf(itemState[2]) > 0.56f)

bool newIsInBound(float *itemState){
    return !(fabs(itemState[0]) > 0.54 || fabs(itemState[1]) > 0.7f || fabs(itemState[2]) > 0.54f);
}

float getTriangleAreaByVertex(float *A, float *B, float *C){
    float AB[3], AC[3], crossBetween[3];
    mathVecSubtract(AB, B, A, 3);
    mathVecSubtract(AC, C, A, 3);
    mathVecCross(crossBetween, AB, AC);
    return (mathVecMagnitude(crossBetween, 3)/2);
}

//End page CORE
//Begin page Comments
/*void NLerp(float whereToGo[3], float start[3], float end[3], float percent){
    for(int i=0;i<3;i++){
        whereToGo[i] = (start[i] + percent*(end[i] - start[i]));
    }
    mathVecNormalize(whereToGo, 3);
}*/

/*void calculateRightSecondSPS(float myState[12], float *itemPos, float radius, float *secondSPS, float *A, float *CVERA, bool couldCollide){
    DEBUG(("couldCollide: %d", couldCollide));
    #define howIncrease 0.01f
    #define minArea 0.12500f
    float attitudeVector[3], lastDist = 3.00f;
    bool isAGoodPoint = false;
    for(int i=0;i<3;i++) attitudeVector[i] = itemPos[i] + (itemPos[6+i]*(radius));
    while(newIsInBound(attitudeVector)){
        for(float i = 0.0f; i<=3.0f; i+=0.01f){
            int axis = 0;
            for(int j = 0;j<6;j++){
                float actualPoint[3];// = {attitudeVector[0], attitudeVector[1], attitudeVector[2]};
                memcpy(actualPoint, attitudeVector, sizeof(float)*3);
                // 0 0 1 1 2 2
                axis+= (j==2 || j==4);
                actualPoint[axis] += (j%2 == 0) ? i : -i;
                float nowArea = getTriangleAreaByVertex(A, actualPoint, CVERA);
                float dist = getDistance(itemPos, actualPoint);
                if(dist<lastDist && (nowArea > minArea) &&  ((!couldCollide) ? !willCollide(myState, actualPoint, itemPos, (radius*1.4142f)+0.1*1.4142f) && !willCollide(actualPoint, CVERA, itemPos, (radius*1.4142f)+0.1*1.4142f) : true) && newIsInBound(actualPoint)){
                    isAGoodPoint = true;
                    memcpy(secondSPS, actualPoint, sizeof(float)*3);
                    lastDist = dist;
                }
            } 
            
            
        }
        for(int i=0;i<3;i++) attitudeVector[i] = attitudeVector[i] + (itemPos[6+i] * howIncrease);
    }
    DEBUG(("TROVATO PUNTO: %d", isAGoodPoint));
}*/
//End page Comments
//Begin page gugliottaScelta
void chooseID(float lowerDistance, short itemID, float distanceBetweenZones){
        actualID = 1-Color;
        
        
        if(distanceBetweenZones>1.15f){
            float range = (distanceBetweenZones>1.5f) ? 0.45f : 0.33f;
        
            if((droppedItems<2 && lowerDistance<range) || (droppedItems>1 && !game.itemInZone(itemID))) 
                actualID = itemID;
            
            
        }
        else{
            if(droppedItems>1 && !game.itemInZone(Color))
                actualID = Color;
         
        }
        
        
        Phase = DOCK;
        if(droppedItems>=3) Phase = DROP;
    }
//End page gugliottaScelta
//Begin page itemAvoid
bool willCollide(float x[3], float y[3], float itemPos[3], float r){
    //x = myState
    //y = whereToGo
    //itemPos
    //The cross - it's the area of the parallelogram of vec1 and vec2
    //area = h * a
    //area = mathVecMagnitude(cross, 3)
    //a = mathVecMagnitude(xToY, 3)
    // h = area / a
    float xToY[3];
    mathVecSubtract(xToY, y, x, 3);
    float xToYMagnitude = mathVecMagnitude(xToY, 3);
    float xToItem[3];
    mathVecSubtract(xToItem, itemPos, x, 3);
    if (xToYMagnitude < mathVecMagnitude(xToItem, 3)) return false;
    float cross[3];
    mathVecCross(cross, xToItem, xToY);
    return !( (mathVecMagnitude(cross, 3) / xToYMagnitude ) > r);
}
/*void slerpFunction(float whereToGo[3], float start[3], float end[3], float percent){
   //whereToGo = array di output
   //start = subtract(myPos, itemPos)
   //end = subtract(endPos, itemPos)
   //RITORNA UN VETTORE SPOSTAMENTO
   float dot = mathVecInner(start, end, 3)/ (mathVecMagnitude(start, 3) * mathVecMagnitude(end, 3)); 
   // |A| |B| cosf(alpha) = A.B
   // in questo caso dot = cosf(alpha), perche' facendo \ |A| |B|
   // |A| |B| cosf(alpha)
   // ------------------
   // |A| |B|
   // = cosf(alpha)
   float theta = acosf(dot)*percent;
   float rVec[3];
   for(int i=0;i<3;i++) rVec[i] = end[i] - (start[i]*dot);
   mathVecNormalize(rVec, 3);
   float cost = cosf(theta);
   float sint = sinf(theta);
   for(int i=0;i<3;i++) whereToGo[i] = (start[i]*cost) + (rVec[i]*sint);
   //for(int i=0;i<3;i++) whereToGo[i] = ( (start[i]*sinf((1-percent)*theta)) + (rVec[i]*sinf(theta*percent)) )/sinf(theta);
   //(sin((1-t)a)v1 + sin(ta)v2)/sin(a)
}*/
//End page itemAvoid
//Begin page itemFunctions
void calculateRightSecondSPS(float myState[12], float itemPos[12], float radius, float secondSPS[3], float A[3], float CVERA[3], bool couldCollide){
    DEBUG(("couldCollide: %d", couldCollide));
    #define howIncrease 0.01f
    #define minArea 0.12500f
    float attitudeVector[3], lastDist = 3.00f;
    bool isAGoodPoint = false;
    memcpy(attitudeVector, itemPos, FLOATSIZE3);
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
                //float nowArea = getTriangleAreaByVertex(A, actualPoint, CVERA);
                float dist = getDistance(itemPos, actualPoint);
                if(dist<lastDist && (getTriangleAreaByVertex(A, actualPoint, CVERA) > minArea) &&  ((!couldCollide) ? !willCollide(myState, actualPoint, itemPos, (radius*1.4142f)+0.1*1.4142f) && !willCollide(actualPoint, CVERA, itemPos, (radius*1.4142f)+0.1*1.4142f) : true) && newIsInBound(actualPoint)){
                    isAGoodPoint = true;
                    memcpy(secondSPS, actualPoint, FLOATSIZE3);
                    lastDist = dist;
                }
            } 
        }
    }while(newIsInBound(attitudeVector));
    DEBUG(("TROVATO PUNTO: %d", isAGoodPoint));
    if(!isAGoodPoint) actualID = Color + 2;
}

/*void calculateRightSecondSPS(float myState[12], float *itemPos, float radius, float *secondSPS, float *A, float *CVERA, bool couldCollide){
    DEBUG(("couldCollide: %d", couldCollide));
    float attitudeVector[3], howIncrease = 0.01f, minArea = 0.12500f, lastDist = 3.00f;
    bool isAGoodPoint = false;
    for(int i=0;i<3;i++) attitudeVector[i] = itemPos[i] + (itemPos[6+i]*(radius));
    while(newIsInBound(attitudeVector) && !isAGoodPoint){
        for(float i = 0.0f; i<=3.0f; i+=0.01f){
            float pointMatrix[6][3] = {
                                            {attitudeVector[0]-i, attitudeVector[1], attitudeVector[2]}, 
                                            {attitudeVector[0]+i, attitudeVector[1], attitudeVector[2]}, 
                                            {attitudeVector[0], attitudeVector[1]-i, attitudeVector[2]}, 
                                            {attitudeVector[0], attitudeVector[1]+i, attitudeVector[2]},
                                            {attitudeVector[0], attitudeVector[1], attitudeVector[2]-i},
                                            {attitudeVector[0], attitudeVector[1], attitudeVector[2]+i},
                                       };
            for(int j = 0;j<6;j++){
                float nowArea = getTriangleAreaByVertex(A, pointMatrix[j], CVERA);
                float dist = getDistance(itemPos, pointMatrix[j]);
                if(dist<lastDist && (nowArea > minArea) &&  ((!couldCollide) ? !willCollide(myState, pointMatrix[j], itemPos, (radius*1.4142f)+0.1*1.4142f) && !willCollide(pointMatrix[j], CVERA, itemPos, (radius*1.4142f)+0.1*1.4142f) : true) && newIsInBound(pointMatrix[j])){
                    isAGoodPoint = true;
                    memcpy(secondSPS, pointMatrix[j], sizeof(float)*3);
                    lastDist = dist;
                }
            }    
        }
        for(int i=0;i<3;i++) attitudeVector[i] = attitudeVector[i] + (itemPos[6+i] * howIncrease);
    }
    DEBUG(("TROVATO PUNTO: %d", isAGoodPoint));
}*/


bool isAnItemInMovement(int itemID){
    float actualItemState[12];
    game.getItemZRState(actualItemState, itemID);
    DEBUG(("MAGNITUDO VEL: %f",mathVecMagnitude(actualItemState+3,3)));
    return mathVecMagnitude(actualItemState+3,3) > 0.0f;
}

void getNextPositionByTime(int itemID, int seconds){
    float actualItemState[12];
    game.getItemZRState(actualItemState, itemID);
    //seconds *= !(seconds>howManySecondsToReset(itemID));
    for(int i = 0; i<3; i++) actualItemState[i] += (actualItemState[3+i] * seconds);
    DEBUG(("FUTURE: %f, %f, %f", actualItemState[0], actualItemState[1], actualItemState[2] ));
}

int howManySecondsToReset(int actualID){
    float realItemState[12]; int seconds = 0;
    game.getItemZRState(realItemState, actualID);
    float calculatedPos[3] = {realItemState[0], realItemState[1], realItemState[2]};
    while(newIsInBound(calculatedPos) && mathVecMagnitude(realItemState+3,3)>0.0f){
        seconds++;
        for(int i = 0;i<3;i++){ calculatedPos[i] = realItemState[i] + (realItemState[3+i] * seconds); }
    }
    return seconds;
}
//End page itemFunctions
//Begin page main
float whereToGo[3], realItemState[12], zoneInfo[4];
int Phase, actualID, Time, Color, droppedItems, idealID;
float A[3], calcSPS[3];/* C[3];*/
bool canPickFirst;
void init(){
    Phase = actualID = droppedItems = Time = 0;
    game.dropSPS();
    calcSPS[0] = -20.0f;
}

void loop(){
    static float whereAttitude[3];
    float myState[12];//, facePOI[3];
    float kVel = 0.045f;
    api.getMyZRState(myState);
    if(Time++<1){ 
        actualID = Color = myState[1] < 0;
        memcpy(A, myState, FLOATSIZE3);
    }
    int SPSHeld = game.getNumSPSHeld();
    float distanceAvarage[3] = { .162f, .149f, .135f}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
    float itemLength[3] = {0.04125f, 0.0275f,  0.01375f};
    //float itemLength[3] = {0.04125f, 0.0275f,  0.01375f};
    
    game.getItemZRState(realItemState, actualID);
    int itemType = game.getItemType(actualID);
    //float myRealVel = mathVecMagnitude(myState+3, 3);
    for(int i=0;i<3;i++){
        whereToGo[i] = realItemState[i] + (realItemState[6+i] * distanceAvarage[itemType]);
        whereAttitude[i] = -realItemState[6+i];
    }
    if(Phase == SPSPLACEMENT){
        if(calcSPS[0] == -20.0f){
            canPickFirst = !(willCollide(myState, whereToGo, realItemState, (itemLength[itemType]*1.4142f)+0.14142f));
            DEBUG(("canPickFirst: %d", canPickFirst));
            calculateRightSecondSPS(myState, realItemState, itemLength[itemType], calcSPS, A, whereToGo /*CVERA*/, canPickFirst);
        }
        
        /*if(canPickFirst){
            DEBUG(("A C B"));
            memcpy(whereToGo, (SPSHeld==2) ? whereToGo : calcSPS, sizeof(float)*3);
        }else{
            DEBUG(("A B C"));
            memcpy(whereToGo, (SPSHeld==2) ? calcSPS : whereToGo, sizeof(float)*3);
        }*/
        
        memcpy(whereToGo, (SPSHeld==2 && !canPickFirst || canPickFirst && SPSHeld==1) ? calcSPS : whereToGo, FLOATSIZE3);
        
        if(getDistance(myState, whereToGo)<0.05f){
            kVel = 0.035f;
            if(!canPickFirst  && SPSHeld == 2){
                //ABC
                DEBUG(("ABC"));
                game.dropSPS();
            }
            if(canPickFirst && SPSHeld == 1){
                //ACB
                DEBUG(("ACB"));
                game.dropSPS();
                Phase = DROP;
            }
        }
    }
    
    
    bool willCollideBool = false;
    if(Phase == DOCK){
        
        willCollideBool = willCollide(myState, whereToGo, realItemState, (itemLength[itemType]*1.4142f)+0.1*1.4142f);
    
        if(willCollideBool && SPSHeld == 0){
            float radius = distanceAvarage[itemType]*0.6f;
            float start[3];
            mathVecSubtract(start,myState,realItemState,3); 
            mathVecNormalize(start,3);
            //for(int i=0;i<3;i++) start[i]*=radius;
            //itemPos--->myState
            
            float end[3];
            mathVecSubtract(end, whereToGo,realItemState,3); 
            mathVecNormalize(end,3);
            
            for(int i=0;i<3;i++){
                start[i]*=radius;
                end[i]*=radius;  
            } 
            //slerpFunction(whereToGo, centeredVector, aux, 0.35f);
            
            float dot = mathVecInner(start, end, 3)/ (mathVecMagnitude(start, 3) * mathVecMagnitude(end, 3)); 
            float theta = acosf(dot)*0.35f;
            float rVec[3];
            for(int i=0;i<3;i++) rVec[i] = end[i] - (start[i]*dot);
            mathVecNormalize(rVec, 3);
            //float cost = cosf(theta);
            //float sint = sinf(theta);
            for(int i=0;i<3;i++) whereToGo[i] = realItemState[i] + (start[i]*cosf(theta)) + (rVec[i]*sinf(theta));
            
            //for(int i=0;i<3;i++) whereToGo[i] = realItemState[i] + whereToGo[i];
        }
        
        
        //if(actualID==6){ memcpy(whereAttitude,realItemState,sizeof(float)*3); }
        
    }
    
    if(Phase<2){
        float meToItem = getDistance(myState, realItemState);
        //bool rangeDistance[3] = {meToItem > .151f && meToItem <.173f,meToItem > .138f && meToItem <.160f,meToItem > .124f && meToItem < .146f};
        float rangeDistance[6] = {.173f, .160f, .146f, .151f, .138f, .124f};
        if(game.hasItem(actualID)==0  && meToItem > rangeDistance[itemType+3] && meToItem < rangeDistance[itemType] && isFacingPos(myState, realItemState,0.25f) && mathVecMagnitude(myState+3, 3)<0.01f){
            if(game.dockItem(actualID)){
                DEBUG(("ci sono"));
                /*if(SPSHeld == 2){ DEBUG(("ACB")); game.dropSPS(); }
                if(SPSHeld == 1){ DEBUG(("ABC")); game.dropSPS(); Phase = DROP; }
                if(SPSHeld == 0) Phase = DROP;*/
                if(SPSHeld >=1) game.dropSPS(); 
                if(SPSHeld <=1 ) Phase = DROP;
            }
        }
    }
    
    if(Phase == DROP){ //drop Item
        DEBUG(("CI SONO"));
        game.getZone(zoneInfo);
       // for(int i = 0;i<3;i++) enemyAss[i] = -zoneInfo[i];
        //float range = /*(getDistance(enemyAss,zoneInfo)<0.7f || */(getDistance(myState,zoneInfo)>0.16f) ? 0.02f : 0.052f;
        
        
        float meToWhereToGo[3];
        mathVecSubtract(meToWhereToGo,myState,zoneInfo,3);
        mathVecNormalize(meToWhereToGo,3);
        for(int i = 0;i<3;i++){ meToWhereToGo[i] *= (getDistance(myState, realItemState));}
        mathVecAdd(whereToGo,zoneInfo,meToWhereToGo,3);
        memcpy(whereAttitude,zoneInfo,FLOATSIZE3);
        if(getDistance(realItemState, zoneInfo) < 0.02f){ 
                game.dropItem();
                droppedItems++;
                //actualID = 1-Color;
                Phase = 30;
        }
        
    }
    
    /*DEBUG((" zoneinfo X: %f Y: %f Z: %f", zoneInfo[0], zoneInfo[1], zoneInfo[2]));
    DEBUG(("WHERETO GO X: %f Y: %f Z: %f", whereToGo[0], whereToGo[1], whereToGo[2]));*/
    
    
    /*bool isInBound = newIsInBound(myState);
    DEBUG(("whereToGo is in bound: %d", isInBound));*/
    
    
    
    if(Phase==30){
        float min = 5.0f;
        if(droppedItems<2){    
            for(int i=2; i<6; i++){
                float item[3];
                game.getItemLoc(item,i);
                float distance = getDistance(zoneInfo,item); 
                if(((i!=3)&&((distance+0.2f)<min)) || (distance<min&&i==3)){
                    min = distance;
                    idealID = i;
                }
            }
        }
        float enemyAss[3] = {-zoneInfo[0], -zoneInfo[1], -zoneInfo[2]};
        float distanceBetweenZones = getDistance(zoneInfo, enemyAss);
        //chooseID(min, idealID, distanceBetweenZones);
        
        actualID = 1-Color;
        
        if(distanceBetweenZones>1.15f){
            float range = (distanceBetweenZones>1.5f) ? 0.45f : 0.33f;
        
            if(!game.itemInZone(idealID) && ((droppedItems<2 && min<range) || (droppedItems>1 ))) 
                actualID = idealID;
        }
        else{
            if(droppedItems>1 && !game.itemInZone(Color))
                actualID = Color;
        }
        
        Phase = DOCK;
        if(droppedItems>=3) Phase = 46; // end movement
        
        DEBUG(("SCELTO: %d",actualID));
        DEBUG(("ideal: %d",idealID));
        
    }
    
    if(Phase==DROP){
        mathVecSubtract(whereAttitude,whereAttitude,myState,3);
        mathVecNormalize(whereAttitude,3);
    }
    api.setAttitudeTarget(whereAttitude);
    
    float velocity = (((getDistance(myState, whereToGo)*0.7f)/kVel)/100.0f);
    if(velocity>0.075f) velocity = 0.075f;
    if(willCollideBool && Phase==DOCK) velocity = 0.042f;
        
    float actualSpeed = mathVecMagnitude(myState+3,3);
     
    float distanceToPoint = getDistance(myState, whereToGo);

    if(Color && Phase==DOCK){
        if(distanceToPoint>0.1f && distanceToPoint<0.4f)   velocity = 0.03f;
        if(distanceToPoint<0.1f && actualSpeed<0.01f)      velocity = 0.008f;
    }
	    
    DEBUG(("VEL: %f", velocity));
    
    float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);
    mathVecNormalize(velocityVector, 3);
    for(int i=0;i<3;i++) velocityVector[i]*=velocity;
    api.setVelocityTarget(velocityVector);
}
//End page main
