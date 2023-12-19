//Begin page Constants
//Begin page Constants
static const int TAKE_MIRROR = 0;
static const int TAKE_SCORE = 1;
static const int TAKE_ENERGY = 2;
static const int DO_MOVEMENT = 3;
static const int LIGHT = 1;
static const int GREY = 0;
static const int DARK = -1;
static const int FUN_TIME = 15;
static const int FUN_TIME_SWITCH = 5;
static const float FUN_ENERGY = 1.00f;
static const float FUN_ENERGY_TWO = 3.5f;
//End page Constants
//End page Constants
//Begin page itemFunction
void DEBUG_VECTOR(char *s, float *v){
    DEBUG(("%s: [%f],[%f],[%f]",s,v[0],v[1],v[2]));
}
void DEBUG_VECTOR(char *s, int *v){
    DEBUG(("%s: [%d],[%d],[%d]",s,v[0],v[1],v[2]));
}
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
//   getDistance(myState,allItem[Color+7])<=0.05f, setMovementPosition(allItem[Color+7],0.10f,0.03f);//0.34 //0.06
        memcpy(whereToGo,allItem[Color+7],sizeof(float)*3);
    if(game.hasItem(Color+7)==0){ //0.136 //con lo switch <=0.138f
    //old values 0.0f, 0.058, distance=<=0.134 without switch
        Phase=TAKE_SCORE; //when mirror is picked up, go to first energy
        
      //  DEBUG(("PRENDO MIRROR"));
    }
}
/////////////////////
void takeEnergy(int doAfter,int zone = 2){
    if(MUST_ENERGY_TAKE-numberEnergy()>availableEnergy()){
       MUST_ENERGY_TAKE = availableEnergy();
    }
    if(numberEnergy()!=MUST_ENERGY_TAKE){
        int index = getNearestEnergyID(zone);
        if(index!=-1){
      //     setMovementPosition(allItem[index],0.01f,0.03f);//0.1F,0.03 :) //0.13F //0.05
      if(index ==0 || index==1){
          operationInGrey=true;
      }
      memcpy(whereToGo,allItem[index],sizeof(float)*3);
           //last values 27/09/2015 0.178 0.06
        }
        else{
           Phase = doAfter;
        }
    }else{
        Phase = doAfter;
    }
 //   DEBUG(("PRENDO ENERGY"));
}
////////////////////////////
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
void takeScore(int doAfter,int zone = 2){
    if(MUST_SCORE_TAKE-numberScore()>availableScore()){
       MUST_SCORE_TAKE = availableScore();
    }
    if(numberScore()!=MUST_SCORE_TAKE){
        int index = getNearestScoreID();
        if(index!=-1){
      //    setMovementPosition(allItem[index],0.01f,0.03f);//0.1F,0.03 :) //0.13F //0.05
            memcpy(whereToGo,allItem[index],sizeof(float)*3);
      //      DEBUG(("SCORE ID: %d",index));
           //last values 27/09/2015 0.178 0.06
        }else{
            Phase = doAfter;
        }
    }else{
        Phase = doAfter;
    }
 //   DEBUG(("PRENDO ENERGY"));
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
          //  DEBUG(("DISTANZA: %f",getDistance(enemyState,allItem[EnergyIDS[i]])));
        }
    }
    return -1;
}   

//////////////////////
void bubbleSort(float v[],int EnergyIDS[], int n){
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

int numberScore(){
    int var = 0;
    for(int i=0;i<3;i++){
        if(game.hasItem(i+4)==0){
            var++;
        }
    }
    return var;
}
///////////////////////////
int availableScore(){
    int var = 0;
    for(int i=0;i<3;i++){
        if(game.hasItem(i+4)==-1){
            var++;
        }
    }
    return var;
}
///////////////////////////////
bool availableScore(int itemZone){
    for(int i=0;i<3;i++){
        if(game.posInArea(allItem[i+4])==itemZone){
            return true;
        }
    }
    return false;
}
int getNearestScoreID(int zone = 2){
    float allIDS[3];
    int ScoreIDS[3];
    for(int i=0;i<3;i++){
        allIDS[i]=getDistance(myState,allItem[i+4]);
        ScoreIDS[i]=i+4;
    }
    bubbleSort(allIDS,ScoreIDS,3);
//    DEBUG_VECTOR("ScoreIDS: ",ScoreIDS);
 //   DEBUG_VECTOR("Distances: ",allIDS);
    for(int i=0;i<3;i++){
        if(game.hasItem(ScoreIDS[i])==-1){
        //    DEBUG(("ID: %d NON PRESO",ScoreIDS[i]));
            if(zone!=2){
               if(game.posInArea(allItem[ScoreIDS[i]])==zone){
                   return ScoreIDS[i]; 
               }
            }else{
               return ScoreIDS[i];  
            }
          //  DEBUG(("DISTANZA: %f",getDistance(enemyState,allItem[EnergyIDS[i]])));
        }
    }
    return -1;
} 
//End page itemFunction
//Begin page main
//Begin page main
float myState[12],enemyState[12], allItem[9][3],myEnergy,velocity;
int timeToSwitch, Phase, Color, MUST_ENERGY_TAKE,MUST_SCORE_TAKE,HowMuchSwitch,myZone,enemyZone;
float whereToGo[3];
bool operationInGrey;
/*float distToStop = 0.1f;
float velocity = 0.03f;*/

/////////////////////////////////////////////
/////////////////////////////////////////////
////                                     ////
////    Author: Riccio-Cesa              ////
////    Last release: 30/09/2015 22:48   ////
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
    MUST_SCORE_TAKE = 2;
    HowMuchSwitch = 0;
    operationInGrey=false;
    DEBUG(("\n Greetings from the school Righi, Naples (Italy) :)"));
}
void loop(){
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    timeToSwitch = game.getLightSwitchTime();
    HowMuchSwitch = getCounterSwitch(HowMuchSwitch);
    myEnergy = game.getEnergy();
    enemyZone = game.posInArea(enemyState);
    myZone= game.posInArea(myState);
    //CONTROL FOR ATTITUDE TO ENEMY BEFORE THE SWITCH OF LIGHT ZONE
    attitudeToEnemy();
    takeAndUploadPic();
    whenUseMirror();
    
    if(Phase==TAKE_MIRROR){
        takeMirror();
    }
    if(Phase==TAKE_SCORE){
        if(myZone!=enemyZone){
         //   DEBUG(("PIGLIAMl"));
           MUST_SCORE_TAKE=3;
        }else{
            MUST_SCORE_TAKE=2;
        }
         takeScore(TAKE_ENERGY,myZone);
    }
    if(Phase==TAKE_ENERGY){
        takeEnergy(DO_MOVEMENT,myZone);
    }
    if(Phase==DO_MOVEMENT){
        doMovement();
       // DEBUG(("PHASE 2"));
    }
    
    
    
    
     if(myEnergy<=0.60f){//0.85f
         velocity=getRightVelocity(2);//0.01 //0.001
         if(velocity<0.003218f){
             velocity = 0.003218f;
         }
     }else if(Phase == TAKE_MIRROR){
         velocity = (getRightVelocity()/100.0f)*2;
     }else if(Phase == TAKE_ENERGY){
        velocity=getRightVelocity(1)/100.0f;
     }else{
         velocity=getRightVelocity()/100.0f;
     }
         
 /*   DEBUG(("DISTANZA: %f",getDistance(myState,enemyState)));
    DEBUG(("Velocity: %f",velocity));
    DEBUG(("Phase: %d",Phase));*/
    setMovementPosition(whereToGo,0.1f,velocity);
   // DEBUG_VECTOR("Vector: ",whereToGo);
}
//End page main


//End page main
//Begin page movimentFunction
//Begin page movimentFunction
int getCounterSwitch(int Switch){
    return ((timeToSwitch >59) ? Switch+1 : Switch);
}
///////////////////////////
float getRightVelocity(int mustItem = 0){//Cesa-Riccio
    if(mustItem==1 && getDistance(enemyState,whereToGo)<0.737837f){
        return (getDistance(myState,whereToGo)*0.737837f)/ 0.08302409288875127f;
       //return (getRightVelocity()/100.0f)+(0.08302409288875127f-(((getDistance(enemyState,whereToGo)*0.737837f)/0.08302409288875127f)/100.0f));
    }else if(mustItem == 2){
        return getDistance(myState,whereToGo)/timeToSwitch; //get velocity to arrive
    }else{
       return (getDistance(myState,whereToGo)*0.737837f)/ 0.08302409288875127f;
    }
    return -1.0f;
}
//////////////////////////
float getTimetoArrive(float distance,float velocity){
    return distance/velocity;
}
///////////////////////////
bool CanAnotherPic(){
    if(game.isFacingOther()
            && myEnergy>=1.55f //1.0 should be ok //2.0
            && game.posInArea(enemyState)!=-1 
            && getDistance(myState,enemyState)>=0.5f)
            { 
                return true;
            }
    return false;
}
/////////////////////////////
void whenUseMirror(){
if(
    /*game.posInArea(myState)!=-1 
   && game.getCurrentTime()>157 
   && game.getScore()>game.getOtherScore()
    ||*/
    game.posInArea(myState)!=-1
    && game.posInArea(enemyState)==-1
    ){
       game.useMirror();
    }
}
/////////////////////////////
/*void attitudeToEnemy(){
     if(game.posInArea(enemyState)!=-1 || (game.posInArea(enemyState)==-1 && timeToSwitch<27) ){
        float result[3], futPos[3], futEnemyPos[3];
        mathVecAdd(futPos,myState,myState+3,3);
        mathVecAdd(futEnemyPos,enemyState,enemyState+3,3);
        mathVecSubtract(result, futEnemyPos, futPos, 3);
        mathVecNormalize(result,3);
        api.setAttitudeTarget(result);
    }
}*/

void attitudeToEnemy(){ //quaternion mode on
if(!game.isFacingOther()){
  //  if(game.posInArea(enemyState)!=-1 || (game.posInArea(enemyState)==-1 && timeToSwitch<27) ){
        float vet1[3]{1,0,0};
        float vet2[3];
        float quat1[4] = {1,0,0,0};
        float quat2[4];
        mathVecSubtract(vet2,myState,enemyState,3);
        mathVecAdd(vet2,vet2,enemyState+3,3);
        mathVecNormalize(vet2,3);
        api.attVec2Quat(vet1,vet2,quat1,quat2);
        api.setQuatTarget(quat2);
 //   }
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
   /*if (ruNearMe(Stop,myState,dist)){
        api.setPositionTarget(Stop);
    }else{
        setConstantVelocityTo(Stop,k);
    }*/
    setConstantVelocityTo(Stop,k);
  //  DEBUG(("USO SETVEL"));
}
/////////////////////////////////
void takeAndUploadPic(){
      if(game.getMemoryFilled()==0 && CanAnotherPic()  && game.getCurrentTime()<178 && myEnergy>1.55f){
          if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
                 game.takePic();
                 myEnergy-=1.0f;
               //  game.uploadPics();
          }
      }
          if(game.getMemoryFilled()==1 //i have one pic in memory and not be able to make another pic 
            || game.getMemoryFilled() == 2 //memory is full...upload in each circumstances
            || game.getCurrentTime()>=178 && game.getMemoryFilled()>0){
            //if(game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.7f){
                if(game.posInArea(myState)==1 || game.getCurrentTime()>=178 || myEnergy>=1.0f || 
                game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.60f
             //   || game.posInArea(myState)==-1 && myEnergy>=1.0f
             )
                {
                    game.uploadPics();
                }
          }
     
}
///////////////////////////////////
///////////////////////////////////
void doMovement(){
  //  DEBUG(("DO MOVE"));
        if(myZone!=enemyZone){
          //  DEBUG(("PIGLIAMl"));
           MUST_SCORE_TAKE=numberScore()+1;
           takeScore(DO_MOVEMENT,myZone);
        }
   /* if(myZone==DARK && enemyZone==DARK){
        if(timeToSwitch>=14){
            //keep current position
            DEBUG(("dark dark >=14"));
        }else{
            if(game.hasItem(4)==-1){
                if(numberScore()!=MUST_SCORE_TAKE  && numberEnergy()==2){
                    MUST_SCORE_TAKE = numberScore()+1;
                    DEBUG(("prendo score"));
                    takeScore(DARK);
                }
            }else{
                DEBUG(("dark dark <16"));
                memcpy(whereToGo,allItem[getLightPointStrategy()],sizeof(float)*3);
            }
            //i write lightPointStrategy because predict future position of item energy in dark
        }
    }
    
    if(myZone==DARK && enemyZone==LIGHT){
        DEBUG(("dark light"));
        if(game.hasItem(4)==-1){
            if(numberScore()!=MUST_SCORE_TAKE  && numberEnergy()==2){
                MUST_SCORE_TAKE = numberScore()+1;
                takeScore(DARK);
            }
        }else{
        }
    }
    
    if(myZone==LIGHT && enemyZone==LIGHT){
        DEBUG(("light light"));
        if(timeToSwitch<15){
            
        }else{
            memcpy(whereToGo,allItem[getDarkPointStrategy()],sizeof(float)*3);
        }
    }
    
    if(myZone==LIGHT && enemyZone==DARK){
        //DEBUG(("light | dark"));
        if(timeToSwitch<14){
            //keep current position
        }else{
            if(game.getOtherEnergy()<1.0f){
                //keep current position 
            }else{
                memcpy(whereToGo,allItem[getDarkPointStrategy()],sizeof(float)*3);
                game.useMirror();
            }
        }
    }
    
    if(myZone==GREY && enemyZone!=DARK && operationInGrey){
        memcpy(whereToGo,allItem[getDarkPointStrategy()],sizeof(float)*3);
        operationInGrey=false;
       //  DEBUG(("grey dark"));
    }
    
    if(myZone==GREY && enemyZone==DARK  && operationInGrey ){
         memcpy(whereToGo,allItem[getDarkPointStrategy()],sizeof(float)*3);
         operationInGrey=false;
    }
  /*  if(HowMuchSwitch==0){
       DEBUG(("SWITCH 0"));
       if(timeToSwitch<=FUN_TIME){
           if(availableEnergy(LIGHT)){
              MUST_ENERGY_TAKE = numberEnergy()+1;
              takeEnergy(LIGHT);
           }else if(availableEnergy()>0){
              MUST_ENERGY_TAKE = numberEnergy()+1;
              takeEnergy();  
           }else{
               
           }
       }else if(timeToSwitch>FUN_TIME){
           
       }  
    }if(HowMuchSwitch==1 || HowMuchSwitch==2 || HowMuchSwitch==3){
        
        if(myZone!=DARK){
            if(myEnergy<2.0f){
              if(availableEnergy(DARK)){
                 MUST_ENERGY_TAKE = numberEnergy()+1;
                 takeEnergy(DARK);
              }else if(availableEnergy()>0){
                 MUST_ENERGY_TAKE = numberEnergy()+1;
                 takeEnergy();  
              }else{
               
              }   
            }else{
                memcpy(whereToGo,allItem[getDarkPointStrategy()],sizeof(float)*3);
            } 
        }else if(myZone==DARK){
            DEBUG(("SONO IN DARK"));
            if(numberScore()!=MUST_SCORE_TAKE  && numberEnergy()==2){
                    DEBUG(("PRENDO SCORE"));
                    MUST_SCORE_TAKE = numberScore()+1;
                    takeScore(DARK);
            }else{
            if(myEnergy<3.5f){//4.0f
                DEBUG(("NEED ENERGY"));
                DEBUG(("AVAILABLE: %d",availableEnergy()));
                if(availableEnergy()>0){
                    DEBUG(("CI STANNO "));
                    MUST_ENERGY_TAKE = numberEnergy()+1;
                    takeEnergy();  
                }
            }else{
                DEBUG(("O STESS"));
                memcpy(whereToGo,allItem[getDarkPointStrategy()],sizeof(float)*3);
            } 
            }
        }
    }*/
}

 
//End page movimentFunction
//End page movimentFunction
//Begin page savedFunction
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

//End page savedFunction
