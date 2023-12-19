//Begin page commenti
//OUR FUNCTION FOR DETERMINATE ITEMTYPE
/*float allItemTypeRange[3] = {0,2,4};//RANGE PER OGNI TIPO DI ITEM
    int itemType = 0;
    while(!(itemID >= allItemTypeRange[itemType] && itemID < allItemTypeRange[itemType+1]) && (itemType < (sizeof(allItemTypeRange)/sizeof(allItemTypeRange[0]))-1))
             itemType++;*/
             
             
             
//vecchia funzione per il dock dell'item

/* SMALL  [.124 : .146]
   MEDIUM [.138 : .160]
   LARGE  [.151 : .173]*/
//Calcola il punto sul bordo dell'item che vogliamo prendere
/*float adjustedEdgePoint(float *myState, float *whereToGo ,float *whereAttitude ,int itemID){//Cesa-Riccio
    //ORDINE: SMALL, MEDIUM, LARGE.
    //float distanceAvarage[3] = {(.124 + .146)/2, (.138 + .160)/2, (.151 + .173)/2}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
    //quello di sopra ÃÂ¨ giusto per il manuale, ma sbagliato per la piattaforma, ÃÂ¨ un bug che devono risolvere
    float distanceAvarage[3] = {(.151 + .173)/2, (.138 + .160)/2, (.124 + .146)/2}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
    int itemType = game.getItemType(itemID); 
    //DEBUG(("tipo: %d",itemType));
    game.getItemLoc(whereToGo,itemID);
    memcpy(actualItemCenter, whereToGo, sizeof(float)*3);
    memcpy (whereAttitude, whereToGo,sizeof(float)*3);
    float idealVector[3];
    mathVecSubtract(idealVector, myState, whereToGo, 3);   
    mathVecNormalize(idealVector,3);
    for(int i=0;i<3;i++) whereToGo[i] += (idealVector[i]*= distanceAvarage[itemType]);//168 large
    return distanceAvarage[itemType];
}*/

/*
//r = 0.133
void poopSPS(float *myState){
    float r = 0.133*1.8;
    if(!isSetStart){
        centerVector[0] = myState[0]-r;
        centerVector[1] = myState[1];
        centerVector[2] = 0.00f;
        mathVecSubtract(startVector, centerVector, myState, 3);
        isSetStart = true;
        game.dropSPS();
        memcpy(firstP,myState,sizeof(float)*3);
    }
    float nowVec[3];
    mathVecSubtract(nowVec, centerVector, myState, 3);
    float angle = angleBetween360(nowVec, startVector);
    DEBUG(("ANGLE: %f",angle));
    
    if(angle>=1.5707963268f*1 && game.getNumSPSHeld()==2){
        game.dropSPS();
        memcpy(secondP,myState,sizeof(float)*3);
    }else if(angle>=1.5707963268f*2 && game.getNumSPSHeld()==1){
        game.dropSPS();
        memcpy(thirdP,myState,sizeof(float)*3);
    }
    
    if(game.getNumSPSHeld()==0){
        isSetStart = false;
        float zoneInfo[4];
        game.getZone(zoneInfo);
        float area = getTriangleArea(firstP, secondP, thirdP);
        DEBUG(("X: %f, Y: %f, Z: %f, Diametro: %f, AreaTriangolo: %f",zoneInfo[0],zoneInfo[1],zoneInfo[2],zoneInfo[3],area));
    }else{
        goOnCircle(r,true,centerVector[0],centerVector[1], false);
    }
    
}
*/
//End page commenti
//Begin page const
/*static const short SMALLITEM = 1 >> 1;
static const short MEDIUMITEM = 2 >> 1;
static const short LARGEITEM = 1 << 1;*/
//End page const
//Begin page core
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}

float angleBetween(float *vectorA, float *vectorB){//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}  

void mathVecMult (float *v, float *a, float k, int dim){
    for (int i=0; i<dim; i++) v[i]=a[i]*k;
}

void mergeArray(int *outVett, int *toMergeVett, int toMergeLen, int *nextFreeSlot){
    for(int i=*nextFreeSlot;i<toMergeLen;i++){
        outVett[i] = toMergeVett[i];
    }
    *nextFreeSlot = *nextFreeSlot + toMergeLen;
}

/*float angleBetween360(float *vectorA, float *vectorB){
    float dot = vectorA[0]*vectorB[0] + vectorA[1]*vectorB[1]; // dot product
    float det = vectorA[0]*vectorB[1] - vectorA[1]*vectorB[0]; // determinant
    float angle = atan2f(det, dot);
    DEBUG(("Angle func: %f",angle));
    return (angle>=0.00f || angle>=-0.01) ? angle: 6.28-(-angle);
}*/
//End page core
//Begin page functions
float adjustedItemFace(float *myState, float *whereToGo , float *whereAttitude,int itemID){//Cesa-Riccio
    //ORDINE: SMALL, MEDIUM, LARGE.
    float distanceAvarage[3] = {(.151 + .173)/2, (.138 + .160)/2, (.124 + .146)/2}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
    float distanceMax[3] = {.173, .160, .146}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
    float sizeItem[3] = {.1f, .075, .05};//IL DIAMETRO DELLO SPHERES Ã¨ 0.2f
    float percentageItem[3] = {0.75, 0.85, 1.00f};
    int itemType = game.getItemType(itemID); 
    game.getItemLoc(whereToGoItemCenter,itemID);
    float halfSideLenght = (sizeItem[itemType]/2.0f)+(distanceAvarage[itemType]*percentageItem[itemType]);//Meta' del lato, + una percentuale x della media del range
    const int facesLenght = 6;
    float facesItemPos[facesLenght][3] = {
                                            {whereToGoItemCenter[0],whereToGoItemCenter[1]-halfSideLenght,whereToGoItemCenter[2]}, //FRONT
                                            {whereToGoItemCenter[0],whereToGoItemCenter[1]+halfSideLenght,whereToGoItemCenter[2]}, //BACK
                                            {whereToGoItemCenter[0]-halfSideLenght,whereToGoItemCenter[1],whereToGoItemCenter[2]}, //LEFT
                                            {whereToGoItemCenter[0]+halfSideLenght,whereToGoItemCenter[1],whereToGoItemCenter[2]}, //RIGHT
                                            {whereToGoItemCenter[0],whereToGoItemCenter[1],whereToGoItemCenter[2]-halfSideLenght}, //UP
                                            {whereToGoItemCenter[0],whereToGoItemCenter[1],whereToGoItemCenter[2]+halfSideLenght}  //DOWN 
                                          }; //FRONT, BACK, LEFT, RIGHT, UP, DOWN
    float lastDistance = getDistance(myState,facesItemPos[0]);
    int rightFacesIndex = 0;
    for(int i = 1;i<facesLenght;i++){
        float actualDistance = getDistance(myState, facesItemPos[i]);
        if(actualDistance < lastDistance){
            rightFacesIndex = i;
            lastDistance = actualDistance;
        }
    }
    memcpy(whereAttitude, whereToGoItemCenter, sizeof(float)*3);
    memcpy(whereToGo, facesItemPos[rightFacesIndex], sizeof(float)*3);
    //return distanceAvarage[itemType];
    return distanceMax[itemType];
}



float getTriangleAreaByVertex(float *A, float *B, float *C){
    float dArea = ((B[0] - A[0])*(C[0] - A[1]) - (C[0] - A[0])*(B[1] - A[1]))/2.0;
    return (dArea > 0.0) ? dArea : -dArea;
}



//Mette in outVett la lista degli item in base alla distanza
// typeFilter, per dire alla funzione di considerare solo quelli LARGE ad esempio, oppure SMALL-MEDIUM
//0-1 large, 2-3 medium, 4-5 small
//Ritorna come int, il riempimento reale dell'array outVett
int sortItemByDistance(float *myState, int *outVett, bool filterSmall = true, bool filterMed = true, bool filterLarge = true){ 
    int small[2] = {0,1}, med[2] = {2,3}, large[2] = {4,5};
    int itemsToUse[6];
    int nextFreeSlot = 0;
    if(filterSmall /*&& game.hasItem(small[0])==-1 && game.hasItem(small[1])==-1*/){mergeArray(itemsToUse, small, 2, &nextFreeSlot);}
    if(filterMed /*&& game.hasItem(med[0])==-1 && game.hasItem(med[1])==-1*/){mergeArray(itemsToUse, med, 2, &nextFreeSlot);}
    if(filterLarge /*&& game.hasItem(large[0])==-1 && game.hasItem(large[1])==-1*/){mergeArray(itemsToUse, large, 2, &nextFreeSlot);}
    if(nextFreeSlot >= 1){
        int i,j,min;
        float temp;
        for(i=0; i<nextFreeSlot-1; i++){
            min = i;
            for(j=i+1; j<nextFreeSlot; j++){
                float jPos[3];
                float minPos[3];
                game.getItemLoc(jPos, itemsToUse[j]);
                game.getItemLoc(minPos, itemsToUse[min]);
                if(getDistance(myState, jPos) < getDistance(myState, minPos)){
                    min = j;
                    temp=outVett[min];
                    outVett[min]=outVett[i];
                    outVett[i]=temp;
                }
            }
        }
        memcpy(outVett, itemsToUse, sizeof(int)*nextFreeSlot);
        return nextFreeSlot;
    }
    return nextFreeSlot;
}


//End page functions
//Begin page main
float myState[12],whereToGo[3],whereToGoItemCenter[3],zoneInfo[4];
short Phase, Color,Time;

void init(){
    DEBUG(("Greetings from ZRighi - Naples"));
    Phase = Color = Time = 0;
}

void loop(){
    float whereAttitude[3], kVel = 0.06;
    api.getMyZRState(myState);
    if(Time++<1) Color = ((myState[1] < 0) ? 1 : 0);//BLUE = 0, RED = 1
    
    if(Phase==0){
        float rangeDistance = adjustedItemFace(myState,whereToGo, whereAttitude, 2);
        DEBUG(("MY DISTANCE: %f",getDistance(myState,whereToGoItemCenter)));
        DEBUG(("MYDISTANCE < %f",rangeDistance));
        if((getDistance(myState, whereToGoItemCenter) < rangeDistance)){
            if(game.dockItem()) Phase = 1;
        }  
    }
    
    if(Phase==1){
        game.getItemLoc(whereToGo,2);
        if(getDistance(myState,whereToGo)<0.2){
            game.dropItem();
            Phase=2;
        }
    }
    
    if(Phase==2){
        game.getItemLoc(whereToGo,3);
    }
    
    /*int listItems[6];
    int r = sortItemByDistance(myState, listItems);
    DEBUG(("R: %d",r));
    DEBUG(("Phase: %d",Phase));
    game.getItemLoc(whereToGo, listItems[0]);*/
    //poopSPS(myState);
    
    
    DEBUG(("Phase: %d",Phase));
    //START ROTATION
    mathVecSubtract(whereAttitude,whereAttitude,myState,3);
    mathVecNormalize(whereAttitude,3);
    api.setAttitudeTarget(whereAttitude);
    //STOP ROTATION
            
	//START MOVEMENT LOGIC
	float velocity = (((getDistance(myState, whereToGo)*0.68f)/kVel)/100.0f);
	float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++) velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
	//END MOVEMENT
}

//End page main
