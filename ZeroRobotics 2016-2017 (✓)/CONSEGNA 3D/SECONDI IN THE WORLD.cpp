//Begin page functions
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


bool isItemInsideAssembly(float centerX, float centerY, float centerZ, float radius, float x, float y, float z){//Cesa-Riccio
    return (((centerX - x) * (centerX - x)) + ((centerY - y) * (centerY - y)) + ((centerZ - z) * (centerZ + z))) <= (radius*radius);
}

void chooseID(float lowerDistance, short itemID, float distanceBetweenZones){
    actualID = (whichStrategy) ? 1-Color : Color;
    
   float range = (distanceBetweenZones>1.5f) ? 0.45f : 0.33f;
   //DEBUG(("R: %f",range));
    
    if(droppedItems<2 && lowerDistance<range && distanceBetweenZones > 1.15f) 
         actualID = itemID;
    
    if(droppedItems>1 && !game.itemInZone(itemID) && distanceBetweenZones > 1.15f){
        float item[3]; game.getItemLoc(item,itemID);
        //DEBUG(("rapporto: %f",Time*getDistance(item,zoneInfo)));
        if((Time*getDistance(item,zoneInfo))<75){
            actualID = itemID;    
        }
        else{
            droppedItems = 3;    
        }
    }
       
    
    if(droppedItems>1&&distanceBetweenZones<1.15f&&((!game.itemInZone(1-Color)&&whichStrategy==0)||(!game.itemInZone(Color)&&whichStrategy==1)))
        actualID = (whichStrategy) ? Color : 1-Color;
       
        
    if(droppedItems>=3){
        Phase = -1;
        memcpy(whereToGo,zoneInfo,sizeof(float)*3);
        actualID = -1;
    }else
    {
        Phase = 1;
    }
   
}


//End page functions
//Begin page main
float whereToGo[3], whereAttitude[3],zoneInfo[4],enemyAss[3];
short Phase,Time,Color,ID,droppedItems;
int actualID;
bool whichStrategy;
bool enemyDock;
bool otherItem;



void init(){ 
    otherItem = whichStrategy = droppedItems = Phase = actualID = Time = ID = enemyDock = 0; 
    game.dropSPS();
}

void loop(){
    float myState[12],enemyState[12];
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    
    float kVel = 0.058f;
    int SPSHeld = game.getNumSPSHeld();
    
    for(int i=0;i<6;i++){
        if(game.hasItem(i)==2)
            enemyDock = true;
    }
    
    
    
    
    if(Time++<1) Color = ((myState[1] < 0) ? 1 : 0);//BLUE = 0, RED = 1
    
    if(droppedItems==0) kVel = 0.053f;
   
    float actualItemPosition[3];
        game.getItemLoc(actualItemPosition,actualID); 
    
    
    //*********************SPS_Placement*********************************************************
    if(SPSHeld==2&&Phase!=10){
        whereToGo[0]=((Color) ? 0.5f :   -0.5f);
        whereToGo[1]=((Color) ? -0.46f :    0.46f);
        whereToGo[2]=0.0f;
        kVel = 0.06f;
        float item[3]; game.getItemLoc(item,1-Color);
        if (getDistance(item,enemyState)<0.43f)
            otherItem = true;
        else
            otherItem = false;
        //DEBUG(("dist: %f",getDistance(item,enemyState)));
    
        if(fabs(myState[0])>0.3f)
            Phase = 10;
    } 
    DEBUG(("t : %d",Time));
    if(SPSHeld==1 || Phase==10){
        
        if(Phase==10 && fabs(myState[0])>0.32f){
            game.dropSPS();
            Phase = 0;
        }   
            
        kVel = 0.045f;
        actualID = (!otherItem) ? 1-Color : Color;
        
        //DEBUG(("ACTUAL : %d",actualID));     
        if(actualID==(1-Color)){
            //DEBUG(("SONO5")); 
            whichStrategy = false;    
            whereToGo[0] = (Color) ? 0.365f: -0.365f;
            whereToGo[1] = (Color) ? 0.25 : -0.25f;
            whereToGo[2] = actualItemPosition[2];
            
                
            if(fabs(myState[1])>0.23f && Time>25){
                game.dropSPS();    
                Phase = 1; 
            }     
            
        }
        else{
            whichStrategy = true;
            whereToGo[0] = (Color) ? -0.24   :  0.24f;
            whereToGo[1] = (Color) ? -0.38f : 0.38f;
            whereToGo[2] =  whereToGo[0];
            memset(whereAttitude,0.0f,sizeof(float)*3);
            kVel = 0.045f;
            whereAttitude[1] = (Color)? 1.0f : -1.0f;
            api.setAttitudeTarget(whereAttitude);
            if(SPSHeld==1 && fabs(myState[2])>0.25f){
                game.dropSPS();
                Phase = 1;
            }
            //Phase = 1;
        }
    
         
    }
    
        
    //**********END__SPS_Placement***********************************
    
    if(Phase==1){
        game.getZone(zoneInfo);
        enemyAss[0] = -zoneInfo[0];
        enemyAss[1] = -zoneInfo[1];
        enemyAss[2] = -zoneInfo[2];
        game.getItemLoc(actualItemPosition,actualID); 
        
          
            
        
            float distanceAvarage[3] = { .162f, .149f, .135f}; //DISTANZA GIUSTA PER IL DOCK IN BASE ALL'ITEM PRESO IN CONSIDERAZIONE
            float sizeItem[3] = {.1f, .075f, .05f}; //IL DIAMETRO DELLO SPHERES 0.2f
            int itemType = game.getItemType(actualID); 
        
        
            mathVecSubtract(whereToGo, myState, actualItemPosition, 3);
            mathVecNormalize(whereToGo, 3);
            for(int i=0;i<3;i++) whereToGo[i]*=distanceAvarage[itemType];
            mathVecAdd(whereToGo, whereToGo, actualItemPosition, 3);
        
        
            float attPos[3];
            mathVecSubtract(attPos, myState, actualItemPosition, 3);
            mathVecNormalize(attPos, 3);
            for(int i=0;i<3;i++) attPos[i]*=-distanceAvarage[itemType]*2;
            mathVecAdd(whereAttitude, attPos, actualItemPosition, 3);
            if(game.hasItem(actualID)==0 && getDistance(myState, whereToGo) < 0.01f && isFacingPos(myState, actualItemPosition, 0.25f) && mathVecMagnitude(myState+3,3)<0.01f){
                if(game.dockItem(actualID)){
                    
                    Phase = 2;
                } 
            }
        
    } //Dock Item
    
    
    float distanceBetweenZones = getDistance(enemyAss,zoneInfo);
    
    
    if(Phase==2){
        
        
        
        
        float num = (getDistance(enemyAss,zoneInfo)<0.7f||getDistance(myState,zoneInfo)>0.16f) ? 0.02f : 0.052f;
        float num2 = 0.19f;
        //DEBUG(("num: %f",num));
        if(game.getFuelRemaining()==0.0f){
            num = 0.045f;
            num2 = 0.3f;
            //DEBUG(("STO QUA: %d",isFacingPos(myState,zoneInfo,0.25f)));    
        }
        float meToWhereToGo[3];
        mathVecSubtract(meToWhereToGo,myState,zoneInfo,3);
        mathVecNormalize(meToWhereToGo,3);
        for(int i = 0;i<3;i++){ meToWhereToGo[i] *= (getDistance(myState, actualItemPosition));}
        mathVecAdd(whereToGo,zoneInfo,meToWhereToGo,3);
        memcpy(whereAttitude,zoneInfo,sizeof(float)*3);
        if(getDistance(actualItemPosition, zoneInfo) < num
           && isFacingPos(myState,zoneInfo,num2)){ 
            if(enemyDock){
                droppedItems++;
                game.dropItem();
                Phase = -1;
            }else if(Time>167){
                game.dropItem();
                Phase = -1;
            }
        }
    }// Drop Item
    
     if(Phase==-1){
        float min = 5.0f;
        if(droppedItems<2){    
            for(int i=2; i<6; i++){
                float item[3];
                game.getItemLoc(item,i);
                float distance = getDistance(zoneInfo,item); 
                if(((i!=3)&&((distance+0.2f)<min)) || (distance<min&&i==3)){
                    min = distance;
                    ID = i;
                }
            }
        }
        chooseID(min,ID,distanceBetweenZones);
    }
    
    
    
    
    
    if(SPSHeld==0){
        mathVecSubtract(whereAttitude,whereAttitude,myState,3);
        mathVecNormalize(whereAttitude,3);
        api.setAttitudeTarget(whereAttitude);
    }
    //DEBUG(("FASE: %d",Phase));
    
	
	float distanceFromEnemy = getDistance(myState,enemyState);
    
	bool freno = false;
    float value_ = (distanceBetweenZones<1.3f) ? 0.4f : 0.7f;
	if(((isFacingPos(myState,enemyState,0.8) && distanceFromEnemy<value_ && SPSHeld==0
	   && (getDistance(enemyState,enemyAss)<0.4f || getDistance(enemyState,zoneInfo)<0.4f)) || (Phase==2 && !enemyDock && distanceFromEnemy<0.35f))) {
	    
	    if(SPSHeld==0)   kVel = distanceBetweenZones/9.0f;
	     
	    
	    if(SPSHeld>0||distanceBetweenZones>1.2f) kVel = 0.15f;
	    freno = true;
	    DEBUG(("b_mcdanbrk"));
    }
	
	float num = (getDistance(enemyAss,zoneInfo)<0.9f) ? 0.006f : 0.004f;
        
	float velocity = (((getDistance(myState, whereToGo)*0.68f)/kVel)/100.0f) + ((Phase==2) ? num  : 0.0f);//0.0015
	
	if(velocity>0.08 && SPSHeld==0) velocity = 0.08f;
	//if(!Color && distanceBetweenZones&&!freno && velocity < 0.03f && (getDistance(myState, whereToGo)>0.15f) && Phase!=2) velocity = 0.03;
	//DEBUG(("phase: %d", Phase));
	
	if(!whichStrategy && Time>25 && Time<55 && getDistance(actualItemPosition, enemyState)<0.45 && droppedItems==0 && Phase!=2){
        if(getDistance(myState, whereToGo)>0.09f) velocity = 0.0115f;
    }
	
	//if(doCircle) velocity = 0.05f;
    //DEBUG(("C: %f",kVel));
//	DEBUG(("%f",velocity));
	float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++) velocityVector[i]*=velocity;
    api.setVelocityTarget(velocityVector);
//*********END__MOVEMENT_&_ATTITUDE***********************************************
}
//End page main
