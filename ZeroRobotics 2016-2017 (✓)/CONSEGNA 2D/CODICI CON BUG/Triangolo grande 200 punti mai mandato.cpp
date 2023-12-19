//Begin page commenti
//OUR FUNCTION FOR DETERMINATE ITEMTYPE
/*float allItemTypeRange[3] = {0,2,4};//RANGE PER OGNI TIPO DI ITEM
    int itemType = 0;
    while(!(itemID >= allItemTypeRange[itemType] && itemID < allItemTypeRange[itemType+1]) && (itemType < (sizeof(allItemTypeRange)/sizeof(allItemTypeRange[0]))-1))
             itemType++;*/
             

   
             
//End page commenti
//Begin page const
static const short SMALLITEM = 1 >> 1;
static const short MEDIUMITEM = 2 >> 1;
static const short LARGEITEM = 1 << 1;
//End page const
//Begin page core
float setConstantVelocityTo(float whereToGo[3], float velocity){
    float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);
    mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++)
		velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
}

bool in_circle2D(float centerX, float centerY, float radius, float x, float y){
    float square_dist = (((centerX - x) * (centerX - x)) + ((centerY - y) * (centerY - y)));
    DEBUG(("squaredirt: %f radiusquare: %f",square_dist,radius*radius));
    return square_dist <= (radius*radius);
}

bool in_circle3D(float centerX, float centerY, float centerZ, float radius, float x, float y, float z){
    float square_dist = (((centerX - x) * (centerX - x)) + ((centerY - y) * (centerY - y)) + ((centerZ - z) * (centerZ - z)));
    DEBUG(("squaredirt: %f radiusquare: %f",square_dist,radius*radius));
    return square_dist <= (radius*radius);
}


float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
float getTriangleAreaByVertex(float *A, float *B, float *C){//Cesa-Riccio
    return fabs(((B[0] - A[0])*(C[0] - A[1]) - (C[0] - A[0])*(B[1] - A[1]))/2.0);
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

float angleBetween360(float *vectorA, float *vectorB){
    float dot = vectorA[0]*vectorB[0] + vectorA[1]*vectorB[1]; // dot product
    float det = vectorA[0]*vectorB[1] - vectorA[1]*vectorB[0]; // determinant
    float angle = atan2f(det, dot);
    DEBUG(("Angle func: %f",angle));
    return (angle>=0.00f || angle>=-0.01) ? angle: 6.28-(-angle);
}
//End page core
//Begin page functions

/* SMALL  [.124 : .146]
   MEDIUM [.138 : .160]
   LARGE  [.151 : .173]*/
//Calcola il punto sul bordo dell'item che vogliamo prendere
float adjustedEdgePoint(float *myState, float *whereToGo ,float *actualChoosenItem,int itemID){//Cesa-Riccio
    //ORDINE: LARGE, MEDIUM, SMALL.
    float distanceAvarage[3] = {(.151 + .173)/2, (.138 + .160)/2, (.124 + .146)/2}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
    int itemType = game.getItemType(itemID); 
    game.getItemLoc(whereToGoReal,itemID);
    memcpy (actualChoosenItem, whereToGoReal,sizeof(float)*3);
    memcpy (whereToGo, whereToGoReal,sizeof(float)*3);
    float idealVector[3];
    mathVecSubtract(idealVector, myState, whereToGo, 3);   
    mathVecNormalize(idealVector,3);
    for(int i=0;i<3;i++) whereToGo[i] += (idealVector[i]*= distanceAvarage[itemType]);//168 large
    return distanceAvarage[itemType];
}

float getTriangleArea(float *A, float *B, float *C){
    float dArea = ((B[0] - A[0])*(C[0] - A[1]) - (C[0] - A[0])*(B[1] - A[1]))/2.0;
    return (dArea > 0.0) ? dArea : -dArea;
}

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

void goOnCircle(float radius,bool var,float centerX,float centerY, bool onZ){
    float Omega=0.14f;  //0.14 //0.20
    float alpha=0.14f;// 0.14 //0.20
    //float Omega = ((radius*2)/sinf(alpha/2));
    float cosalpha=cosf(alpha), sinalpha=sinf(alpha); 
    float stato[12];
    float centro[3] = {centerX,(onZ)? 0.0f : centerY, (onZ) ? centerY : 0.0f};
    api.getMyZRState(stato);
    float v[3], w[3];
    mathVecMult(v,stato,1,3);
    mathVecSubtract(v,v,centro,3);
    mathVecNormalize(v,3);
    int asse = (onZ) ? 2 : 1 ;
    if(!var){
        w[0]=centerX + (radius*(v[0]*cosalpha-v[1]*sinalpha));
        w[asse]=centerY + (radius*(v[0]*sinalpha+v[asse]*cosalpha));
    }else{
        w[0]=centerX + (radius*(v[1]*sinalpha+v[0]*cosalpha));
        w[asse]=centerY + (radius*(v[asse]*cosalpha-v[0]*sinalpha));
    }
    w[(onZ) ? 1 : 2]=0;
    mathVecSubtract(v,w,stato,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}

/*
int sceltaItem(float state[12]){
    float lastZ=50.0;
    int choosenIndex = -1;
    float lastDistance = 5.00f; //Conterra' l'ultima distanza registrata valida,
                                //E' impostata ad un valore grande 50.00f
                                //Cosi' al primo ciclo si resettera' prendere la distanza
                                //del primo score esaminato
    for(int i=0;i<6;i++){
        float nowPosition[3];//Posizione dello score attuale esaminato
        game.getItemLoc(nowPosition,i);//Prendo la pos e la metto in nowPosition
        float nowDistance = getDistance(state,nowPosition);//Prendo la distanza dallo score attuale
       
        if(game.hasItem(i+3)==-1 && nowPosition[2]<lastZ && (Color == (nowPosition[0]>0))){ //Se la (distanza attuale<di quella precedentemente valida) && lo score attuale e' libero 
            lastZ=nowPosition[2];
            choosenIndex = i+3;//L'ID valido viene aggiornato a quello dello score attualmente esaminato
            lastDistance = nowDistance;//Stessa cosa per la distanza
        }
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex);
        DEBUG(("%d",choosenIndex));    
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
    
}*/

int chooseNearerItem(float myState[12],int idToEsclude[6]){
    float lastDistance = 5.00f; 
    int choosenIndex=-1;
    
    
    for(int i=0;i<6;i++){
        float nowPosition[3];
        if(idToEsclude[i]!=i)
            game.getItemLoc(nowPosition,i);
        float nowDistance = getDistance(myState,nowPosition);
        if(nowDistance<lastDistance){
            choosenIndex = i;
            lastDistance = nowDistance;
        }
    }
    
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    DEBUG(("I choose:%d",choosenIndex));
    return choosenIndex;
}

int idDaFottere(){
    for(int i=0;i<6;i++){
        if(Time<2||game.hasItem(i)==2){
            previousState[i]=game.hasItem(i);
            DEBUG(("STATEiD(%d): %d",i,previousState[i]));
        }
        if(previousState[i]==2&&game.hasItem(i)==0){
            DEBUG(("CONTROLLO"));
            float item[3];
            game.getItemLoc(item,i);
            if(isItemInsideAssembly(enemyAss[0],enemyAss[1],enemyAss[2],0.1f,item[0],item[1],item[2]))
                DEBUG((" l'item:%d",i));
                return i;
            
        }
    }
}
    





//End page functions
//Begin page item functions
void updateEachItemsPosition(float allItemsPosition[6][3]){
    for(int i = 0;i<6;i++){ game.getItemLoc(allItemsPosition[i],i); }
}

void dockItem(int id){
    
}

void dropItem(int id){
    
}

bool isItemInsideAssembly(float centerX, float centerY, float centerZ, float radius, float x, float y, float z){//Cesa-Riccio
    return (((centerX - x) * (centerX - x)) + ((centerY - y) * (centerY - y)) + ((centerZ - z) * (centerZ - z))) <= (radius*radius);
}

//End page item functions
//Begin page main
//Declare any variables shared between functions here
float whereToGo[3];
float whereToGoReal[3];
float actualChoosenItem[3];
float allItemsPosition[6][3];
float zoneInfo[4],enemyAss[3];
short Phase, Color,Time;
float myState[12],enemyState[12];
float dist;


bool isSetStart;
float startVector[3];
float centerVector[3];

int id,lastId[6],previousState[6];


float firstP[3];
float secondP[3];
float thirdP[3];
void init(){
    DEBUG(("Greetings from ZRighi - Naples"));
    Phase = Color = Time = dist = 0;
    isSetStart = false;
    
    for(int i=0;i<6;i++){
        lastId[i]=-1;
        previousState[i]=0;
    }

}

void loop(){
    updateEachItemsPosition(allItemsPosition);
    float whereAttitude[3], kVel = 0.09f;
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    if(Time++<1) Color = ((myState[1] < 0) ? 1 : 0);//BLUE = 0, RED = 1
    //if(Color == 0) DEBUG(("SIAMO BLU"));
    
    
    
    int SPSHeld = game.getNumSPSHeld();
    
    
    
    
    
    
    if(SPSHeld==3){
        whereToGo[2]=0;
        whereToGo[0] = ((Color==0) ? 58.0f : -58.0f);
        whereToGo[1] = ((Color==0) ? 200.0f : -200.0f);
        if(myState[1]>2.1f&&!Color)
            Phase=1;
        if(myState[1]<-2.1f&&Color)
            Phase=1;
        
    } 
    
    if(Phase==1&&SPSHeld>1){
        if(myState[4]<0.0f&&!Color&&SPSHeld==3)
            game.dropSPS();
        if(myState[4]>0.0f&&Color&&SPSHeld==3)
            game.dropSPS();
            
        whereToGo[0] = ((Color==0) ? 4.0f : -4.0f);
        whereToGo[1] = ((Color==0) ? -100.0f : 100.0f);
        if(myState[1]<-3.0f&&!Color){
            Phase=2;
            whereToGo[0] =-100.0f;
            whereToGo[1] = -100.0f;
        }
        if(myState[1]>3.0f&&Color){
            Phase=2;
            whereToGo[0] = 100.0f;
            whereToGo[1] = 100.0f;
        }
    }

    if(SPSHeld==2&&Phase==2){
        DEBUG(("ms1:%f",myState[1]));
            /*if(Time==84)
            */if(myState[1]<-3.5f && !Color)
                game.dropSPS();
            if(myState[1]>3.4f && Color)
                game.dropSPS();
            //*/
    
        }
    /*if(SPSHeld==1){
        if(Time>140)
            game.dropSPS();
            
    } */  
    
    DEBUG(("ga:%f",game.getFuelRemaining()));
    if((Time==85&&game.getFuelRemaining()<8.0f) || (Phase==10)){
        Phase=10;
        if(SPSHeld==2)
            game.dropSPS();
        whereToGo[0] = ((Color==0) ? -100.0f : 100.0f);
    }
    
    
    if(Time==160&&SPSHeld>0){
        game.dropSPS();
        game.dropSPS();
        game.dropSPS();
    }
    
    
    if(Time>2){
        float vel = (((getDistance(myState, whereToGo)*0.68f)/0.06f)/100.0f);
        setConstantVelocityTo(whereToGo,vel);
        DEBUG(("GASS A MARTELLOOOOO, TRIANGOLO BELLO"));
    }    
    
    
    /*
    DEBUG(("%F",whereToGo[0]));
    DEBUG(("%F",whereToGo[1]));
    DEBUG(("%F",whereToGo[2]));
    */
    DEBUG(("Fase:%d",Phase));
    
    
    
    
    
    

    /*bool isIn = in_circle3D(zoneInfo[0],zoneInfo[1],zoneInfo[2],0.1f,allItemsPosition[id][0],allItemsPosition[id][1],allItemsPosition[id][2]);
    DEBUG(("c sta: %d",isIn));
    */
    //START ROTATION
    
            
	
	//START MOVEMENT LOGIC
	/*float velocity = (((getDistance(myState, whereToGo)*0.68f)/kVel)/100.0f);
	float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++) velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);*/
	//END MOVEMENT
}






//End page main
