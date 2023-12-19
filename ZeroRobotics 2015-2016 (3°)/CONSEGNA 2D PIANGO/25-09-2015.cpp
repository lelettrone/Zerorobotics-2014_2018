//Begin page Constants
static const int TAKE_MIRROR = 0;
static const int TAKE_ENERGY = 1;
static const int DO_MOVEMENT = 2;
//End page Constants
//Begin page itemFunction
int getLightPointStrategy(){
    float Point[2][3];
    Point[0][0]=0.37f;//0.37
    Point[0][1]=-0.135f; //-0.125f
    Point[0][2]=0.0f;
    Point[1][0]=0.1f;
    Point[1][1]=0.135f;
    Point[1][2]=0.0f;
    
    if(game.posInArea(allItem[2])==-1){
        return 3;
    }
return 2;
}
bool availableEnergy(int itemZone){
    for(int i=0;i<4;i++){
        if(game.posInArea(allItem[i])==itemZone){
            return true;
        }
    }
    return false;
}
int getDarkPointStrategy(){
    float Point[2][3];
    Point[0][0]=0.37f;//0.37
    Point[0][1]=-0.135f; //-0.125f
    Point[0][2]=0.0f;
    Point[1][0]=0.1f;
    Point[1][1]=0.135f;
    Point[1][2]=0.0f;
    
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
void takeEnergy(){
    if(MUST_ENERGY_TAKE-numberEnergy()>availableEnergy()){
       MUST_ENERGY_TAKE = availableEnergy();
    }
    if(numberEnergy()!=MUST_ENERGY_TAKE){
        int index = getNearestEnergyID();
        if(index!=-1){
           setMovementPosition(allItem[index],0.178f,0.05f);//0.1F,0.03 :) //0.13F
        }
        else{
           Phase=DO_MOVEMENT;
        }
    }else{
        Phase = DO_MOVEMENT;
    }
}
///////////////////
void do_Movement(){
    int enemyZone = game.posInArea(enemyState),myZone=game.posInArea(myState);
    float Point[2][3];
    Point[0][0]=myState[0];//0.37 //0.1
    Point[0][1]=-0.135f; //-0.125f
    Point[0][2]=0.0f;
    Point[1][0]=myState[0];
    Point[1][1]=0.135f;
    Point[1][2]=0.0f;
    
    if(enemyZone!=-1 && myZone !=-1){
        setMovementPosition(Point[getLightPointStrategy()],0.1f,0.04);
    }else{
        setMovementPosition(Point[getDarkPointStrategy()],0.1f,0.04);
    }
    
}
///////////////////////
int availableEnergy(){
    int var = 0;
    for(int i=0;i<4;i++){
        if(game.hasItem(i)==-1){
            var++;
        }
    }
    return var;
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
int getNearestEnergyID(){
    float allIDS[4];
    int EnergyIDS[4];
    for(int i=0;i<4;i++){
        allIDS[i]=getDistance(myState,allItem[i]);
        EnergyIDS[i]=i;
    }
    bubbleSort(allIDS,EnergyIDS,4);
    for(int i=0;i<4;i++){
        if(game.hasItem(EnergyIDS[i])==-1 ){
               DEBUG(("DISTANZA: %f",getDistance(enemyState,allItem[EnergyIDS[i]])));
            return EnergyIDS[i];
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
int timeToSwitch, Phase, Color, MUST_ENERGY_TAKE,HowMuchSwitch;

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
    MUST_ENERGY_TAKE = 2; //3
}
void loop(){
    if(game.posInArea(myState)!=-1 && game.getCurrentTime()>157)
     game.useMirror();
     
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    timeToSwitch = game.getLightSwitchTime();
    HowMuchSwitch = getCounterSwitch(HowMuchSwitch);
    myEnergy = game.getEnergy();
    //CONTROL FOR ATTITUDE TO ENEMY BEFORE THE SWITCH OF LIGHT ZONE
    attitudeToEnemy();
    
    if(Phase==TAKE_MIRROR)
        takeMirror();
    if(Phase==TAKE_ENERGY)
        takeEnergy();
    if(Phase==DO_MOVEMENT){
        do_Movement();
        DEBUG(("PHASE 2"));
    }
    
    takeAndUploadPic();
}
//End page main
//Begin page movimentFunction
int getCounterSwitch(int Switch){
    return ((timeToSwitch >59) ? Switch+1 : Switch);
}
float getTimetoArrive(float distance,float velocity){
    return distance/velocity;
}
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
////////////////////////
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
                if(game.posInArea(myState)==1 || game.getCurrentTime()>=178 || myEnergy>4.0f || game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.7f){
                    game.uploadPics();
                }
          }
      }
}
//End page movimentFunction
