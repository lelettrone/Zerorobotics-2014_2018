//Begin page Constants
static const int TAKE_MIRROR = 0;
static const int TAKE_ENERGY = 1;
static const int DO_MOVEMENT = 2;
static const int LIGHT = 1;
static const int GREY = 0;
static const int DARK = -1;
static const int FUN_TIME = 15;
static const int FUN_TIME_SWITCH = 5;
static const float FUN_ENERGY = 1.00f;
/*FRA QUANTO CAZZO MI PIACCIONO STE COSTANTI AHHAHAAH, IL CODE Ã¨ MOLTO MENO INCASINATO 
<3
*/

//End page Constants
//Begin page itemFunction
int getLightPointStrategy(){
    if(game.posInArea(allItem[2])==-1){
        return 3;
    }
return 2;
}
//////////////////////////////
int getDarkPointStrategy(){
    if(game.posInArea(allItem[2])==-1){
        return 2;
    }
return 3;
}
///////////////////////
bool enemyHaveMirror(){
    int idEnemyMirror = ((Color == 0) ? 8 : 7);
    if(game.hasItem(idEnemyMirror)==1){
        return true;
    }
return false;  
}
/////////////////////////////
void getItemsPos(){
    for(int i=0;i<9;i++){
        game.getItemLoc(allItem[i],i);
    }
}
//////////////////////
void takeMirror(){
    setMovementPosition(allItem[Color+7],0.0f,0.058f);//0.34 //0.06
    if(getDistance(myState,allItem[Color+7])<=0.134f){ //0.136 //con lo switch <=0.138f
        Phase=TAKE_ENERGY; //when mirror is picked up, go to first energy
        DEBUG(("PRENDO MIRROR"));
    }
}
/////////////////////
void takeEnergy(int zone = 2){
    if(MUST_ENERGY_TAKE-numberEnergy()>availableEnergy()){
       MUST_ENERGY_TAKE = availableEnergy();
    }
    if(numberEnergy()!=MUST_ENERGY_TAKE){
        int index = getNearestEnergyID(zone);
        if(index!=-1){
           setMovementPosition(allItem[index],0.178f,0.06f);//0.1F,0.03 :) //0.13F //0.05
        }
        else{
           Phase=DO_MOVEMENT;
        }
    }else{
        Phase = DO_MOVEMENT;
    }
    DEBUG(("PRENDO ENERGY"));
}
///////////////////////////
int numberEnergy(){
    int var = 0;
    for(int i=0;i<4;i++){
        if(game.hasItem(i)==0){
            var++;
        }
    }
    return var;
}
///////////////////////////
int availableEnergy(){
    int var = 0;
    for(int i=0;i<4;i++){
        if(game.hasItem(i)==-1){
            var++;
        }
    }
    return var;
}
///////////////////////////////
bool availableEnergy(int itemZone){
    for(int i=0;i<4;i++){
        if(game.posInArea(allItem[i])==itemZone){
            return true;
        }
    }
    return false;
}
//////////////////////////
int getNearestEnergyID(int zone = 2){
    float allIDS[4];
    int EnergyIDS[4];
    for(int i=0;i<4;i++){
        allIDS[i]=getDistance(myState,allItem[i]);
        EnergyIDS[i]=i;
    }
    bubbleSort(allIDS,EnergyIDS,4);
    for(int i=0;i<4;i++){
        if(game.hasItem(EnergyIDS[i])==-1){
            if(zone!=2){
               if(game.posInArea(allItem[EnergyIDS[i]])==zone){
                   return EnergyIDS[i]; 
               }
            }else{
               return EnergyIDS[i];  
            }
            DEBUG(("DISTANZA: %f",getDistance(enemyState,allItem[EnergyIDS[i]])));
        }
    }
    return -1;
}   

//////////////////////
void bubbleSort(float v[],int EnergyIDS[4], int n){
    float temp;
    int tempI;
    for(int i = 0; i<n-1; i++){
        for(int k = 0; k<n-1-i; k++){
            if(v[k] > v[k+1]){
                temp = v[k];
                v[k] = v[k+1];
                v[k+1] = temp;
                tempI = EnergyIDS[k];
                EnergyIDS[k] = EnergyIDS[k+1];
                EnergyIDS[k+1] = tempI;
            }
        }
    }
}
//End page itemFunction
//Begin page main
float myState[12],enemyState[12], allItem[9][3],myEnergy;
int timeToSwitch, Phase, Color, MUST_ENERGY_TAKE,ACTUAL_PROCEDURE,HowMuchSwitch;

/////////////////////////////////////////////
/////////////////////////////////////////////
////                                     ////
////    Author: Cesy-Riccio              ////
////    Last release: 24/09/2015 19.36   ////
////    Game: SpySPHERES 2D              ////
////    Team: ZRighi                     ////
////                                     ////
/////////////////////////////////////////////
/////////////////////////////////////////////

void init(){
    getItemsPos();
    Phase = TAKE_MIRROR;
    Color = getColor();
    MUST_ENERGY_TAKE = 1; //3
    ACTUAL_PROCEDURE = DARK;
    HowMuchSwitch = 0;
}
void loop(){
    if(game.posInArea(myState)!=-1 && game.getCurrentTime()>157 && game.getScore()>game.getOtherScore())
       game.useMirror();
     
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    timeToSwitch = game.getLightSwitchTime();
    HowMuchSwitch = getCounterSwitch(HowMuchSwitch);
    myEnergy = game.getEnergy();
    //CONTROL FOR ATTITUDE TO ENEMY BEFORE THE SWITCH OF LIGHT ZONE
    attitudeToEnemy();
    takeAndUploadPic();
    if(Phase==TAKE_MIRROR){
        takeMirror();
    }
    if(Phase==TAKE_ENERGY){
        takeEnergy();
    }
    
    if(Phase==DO_MOVEMENT){
        do_Movement();
        DEBUG(("PHASE 2"));
    }
    
    
}
//End page main
//Begin page movimentFunction
int getCounterSwitch(int Switch){
    return ((timeToSwitch >59) ? Switch+1 : Switch);
}
//////////////////////////
float getTimetoArrive(float distance,float velocity){
    return distance/velocity;
}
///////////////////////////
bool CanAnotherPic(){
    if(game.isFacingOther()
            && myEnergy>=2.0f //1.0 should be ok
            && game.posInArea(enemyState)!=-1 
            && getDistance(myState,enemyState)>=0.5f)
            { 
                return true;
            }
    return false;
}
/////////////////////////////
void attitudeToEnemy(){
     if(game.posInArea(enemyState)!=-1 || (game.posInArea(enemyState)==-1 && timeToSwitch<27) ){
        float result[3], futPos[3], futEnemyPos[3];
        mathVecAdd(futPos,myState,myState+3,3);
        mathVecAdd(futEnemyPos,enemyState,enemyState+3,3);
        mathVecSubtract(result, futEnemyPos, futPos, 3);
        mathVecNormalize(result,3);
        api.setAttitudeTarget(result);
    }
}
////////////////////////
int getColor(){
    api.getMyZRState(myState);
    return ((myState[0] < 0) ? 1 : 0);
}
///////////////////////
float getDistance(float a[3], float b[3]){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
//////////////////////////
bool ruNearMe(float you[13], float me[13], float r){
    float vPath[13];
    mathVecSubtract(vPath,you,me,3);
    return (mathVecMagnitude(vPath,3) <= r ) ? true : false;
}
/////////////////////////
void setConstantVelocityTo(float point[], float k){
	    float myState[12];
		float velocityVector[3];
		api.getMyZRState(myState);
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
		api.setVelocityTarget(velocityVector);
}
/////////////////////////////
void setMovementPosition(float Stop[], float dist,float k){
   if (ruNearMe(Stop,myState,dist)){
        api.setPositionTarget(Stop);
    }else{
        setConstantVelocityTo(Stop,k);
    }
}
/////////////////////////////////
void takeAndUploadPic(){
      if(game.getMemoryFilled()!=2 && CanAnotherPic() && game.getCurrentTime()<178 && myEnergy>1.55f){
          if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
                 game.takePic();
          }
      }else{
          if(game.getMemoryFilled()==1 && !CanAnotherPic()//i have one pic in memory and not be able to make another pic 
            || game.getMemoryFilled() == 2 //memory is full...upload in each circumstances
            || game.getCurrentTime()>=178 && game.getMemoryFilled()>0){
            //if(game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.7f){
                if(game.posInArea(myState)==1 || game.getCurrentTime()>=178 || myEnergy>4.0f || 
                game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.7f){
                    game.uploadPics();
                }
          }
      }
}
///////////////////////////////////

///////////////////////////////////
void do_Movement(){
    int enemyZone = game.posInArea(enemyState),myZone=game.posInArea(myState);
    if(myZone==DARK && enemyZone==DARK){
       DEBUG(("DARK-DARK"));
       if(timeToSwitch>FUN_TIME && HowMuchSwitch>0){
            if(game.getOtherEnergy()>FUN_ENERGY){
                if(availableEnergy(DARK)){
                    MUST_ENERGY_TAKE = 1;
                    takeEnergy(DARK);
                }else{
                    ACTUAL_PROCEDURE = DARK;
                }
            }else{
                if(availableEnergy(LIGHT)){
                    MUST_ENERGY_TAKE = 1;
                    takeEnergy(LIGHT); 
                }else{
                    ACTUAL_PROCEDURE = LIGHT;
                }
                    
            }
        }else{
            if(timeToSwitch<=FUN_TIME_SWITCH){
               ACTUAL_PROCEDURE = DARK;    
               DEBUG(("CHANGE TO DARK"));
            }
        }//
    }
    if(myZone==LIGHT && enemyZone==LIGHT){
       DEBUG(("LIGHT-LIGHT"));
       
    }if(myZone==DARK && enemyZone==LIGHT){
       DEBUG(("DARK-LIGHT"));
       if(timeToSwitch>FUN_TIME && HowMuchSwitch>0){
            if(game.getOtherEnergy()>FUN_ENERGY){
                MUST_ENERGY_TAKE = 1;
                takeEnergy(DARK);             
            }else{
                MUST_ENERGY_TAKE = 1;
                takeEnergy(LIGHT);     
            }
        }else{
            if(timeToSwitch<=FUN_TIME_SWITCH){
               ACTUAL_PROCEDURE = DARK;    
               DEBUG(("CHANGE TO LIGHT"));
            }
        }//
       
    }if(myZone==LIGHT && enemyZone==DARK){
       DEBUG(("LIGHT-DARK"));
       
    }if(myZone==GREY && enemyZone==DARK){
       DEBUG(("GREY-DARK"));
       
    }if(myZone==DARK && enemyZone==GREY){
       DEBUG(("DARK-GREY"));
       
    }if(myZone==GREY && enemyZone==LIGHT){
       DEBUG(("GREY-LIGHT"));
       
    }if(myZone==LIGHT && enemyZone==GREY){
       DEBUG(("LIGHT-GREY"));
       
    }
    /////////////////////////////////
    if(ACTUAL_PROCEDURE==LIGHT){
        setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
    }else if(ACTUAL_PROCEDURE==DARK){
        setMovementPosition(allItem[getDarkPointStrategy()],0.1f,0.04);
    }
}
//End page movimentFunction
//Begin page savedFunction
/*void do_Movement(){
    int enemyZone = game.posInArea(enemyState),myZone=game.posInArea(myState);
    --if(enemyZone!=-1 && myZone !=-1){
       
       setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
    }else{
        setMovementPosition(allItem[getDarkPointStrategy()],0.1f,0.04);
    }--
    if(enemyZone!=-1){
        DEBUG(("CASE 1"));
            if(myEnergy>4.0 && timeToSwitch>10){
                DEBUG(("CHOISE 1"));
                setMovementPosition(allItem[getDarkPointStrategy()],0.1f,0.04);
            }else{
                DEBUG(("CHOISE 2"));
                setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
            }
            
    }else{
        if(myZone!=-1){
            if(timeToSwitch<15 && game.getOtherEnergy()<1.00){
                setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
            }else{
                if(game.getOtherEnergy()>1.00){
                    setMovementPosition(allItem[getDarkPointStrategy()],0.1f,0.04);
                }else if(timeToSwitch>15){
                    setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
                }
                
            }
        }else if(myZone==-1 && enemyZone==-1){
            if(timeToSwitch<15){
               setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
            }else{
               setMovementPosition(allItem[getDarkPointStrategy()],0.1f,0.04);
            }
        }
    }
}*/







/*
if(myZone==DARK && enemyZone==DARK){
       DEBUG(("DARK-DARK"));
       if(timeToSwitch>FUN_TIME && HowMuchSwitch>0){
            if(game.getOtherEnergy()>FUN_ENERGY){
                MUST_ENERGY_TAKE = 1;
                takeEnergy(DARK);             
            }else{
                MUST_ENERGY_TAKE = 1;
                takeEnergy(LIGHT);     
            }
        }else{
            if(timeToSwitch<=FUN_TIME_SWITCH){
               ACTUAL_PROCEDURE = DARK;    
               DEBUG(("CHANGE TO DARK"));
            }
        }//
    }if(myZone==LIGHT && enemyZone==LIGHT){
       DEBUG(("LIGHT-LIGHT"));
       
    }if(myZone==DARK && enemyZone==LIGHT){
       DEBUG(("DARK-LIGHT"));
       if(timeToSwitch>FUN_TIME && HowMuchSwitch>0){
            if(game.getOtherEnergy()>FUN_ENERGY){
                MUST_ENERGY_TAKE = 1;
                takeEnergy(DARK);             
            }else{
                MUST_ENERGY_TAKE = 1;
                takeEnergy(LIGHT);     
            }
        }else{
            if(timeToSwitch<=FUN_TIME_SWITCH){
               ACTUAL_PROCEDURE = LIGHT;    
               DEBUG(("CHANGE TO LIGHT"));
            }
        }//
       
    }if(myZone==LIGHT && enemyZone==DARK){
       DEBUG(("LIGHT-DARK"));
       
    }if(myZone==GREY && enemyZone==DARK){
       DEBUG(("GREY-DARK"));
       
    }if(myZone==DARK && enemyZone==GREY){
       DEBUG(("DARK-GREY"));
       
    }if(myZone==GREY && enemyZone==LIGHT){
       DEBUG(("GREY-LIGHT"));
       
    }if(myZone==LIGHT && enemyZone==GREY){
       DEBUG(("LIGHT-GREY"));
       
    }
    /////////////////////////////////
    if(ACTUAL_PROCEDURE==LIGHT){
        setMovementPosition(allItem[getLightPointStrategy()],0.1f,0.04);
    }else if(ACTUAL_PROCEDURE==DARK){
        setMovementPosition(allItem[getDarkPointStrategy()],0.1f,0.04);
    }*/
//End page savedFunction
