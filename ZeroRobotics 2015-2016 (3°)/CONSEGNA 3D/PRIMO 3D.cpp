//Begin page Constants
static const int TAKE_MIRROR = 0;
static const int TAKE_SCORE = 1;
static const int TAKE_ENERGY = 2;
static const int DO_MOVEMENT = 3;
/*static const int LIGHT = 1;
static const int GREY = 0;
static const int DARK = -1;*/
//End page Constants
//Begin page ItemFunctions
void DEBUG_VECTOR(char *s, float *v){
    DEBUG(("%s: [%f],[%f],[%f]",s,v[0],v[1],v[2]));
}
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
//////////////////////////
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
    memcpy(whereToGo,allItem[Color+7],sizeof(float)*3);
    if(game.hasItem(Color+7)==0){
       Phase=TAKE_SCORE;
    }
}
/////////////////////////
void takeEnergy(int doAfter, int Zone){
    if(MUST_ENERGY_TAKE-myItem(0, 2)>freeItem(0, 2, Zone)){
       MUST_ENERGY_TAKE = freeItem(0, 2, Zone);
    }
    DEBUG(("MIE ENERGY: %d",myItem(0, 2)));
    DEBUG(("Disponibili: %d",freeItem(0, 2, Zone)));
    DEBUG(("index: %d",getNearestItemID(0, 2, Zone)));
    DEBUG(("MUST ENERGY: %d",MUST_ENERGY_TAKE));
    if(myItem(0, 2)!=MUST_ENERGY_TAKE){
        int index = getNearestItemID(0, 2, Zone);
        
        DEBUG(("index: %d",index));
        if(index!=-1){
           memcpy(whereToGo,allItem[index],sizeof(float)*3);
            
        }else{
           Phase = doAfter;
        }
    }else{
        Phase = doAfter;
    }
}
///////////////////////////
int getNearestItemID(int Start, int End,int Zone){
    int Size = (End-Start)+1;
    int ItemIDS[Size];
    float allDistances[Size];
    memset(allDistances,1.00f,Size);
    memset(ItemIDS,1,Size);
    int k=0;
    int idReturn = -1;
    for(int i=Start;i<End+1;i++){
        allDistances[k]=getDistance(myState,allItem[i])*1.00f;
        ItemIDS[k]=i;
        k++;
    }
    bubbleSort(allDistances,ItemIDS,Size);
    for(int j=0;j<Size;j++){
        if((game.hasItem(ItemIDS[j])==-1 && game.posInArea(allItem[ItemIDS[j]])==Zone)
        || (game.hasItem(ItemIDS[j])==-1 && Zone==2)){
           idReturn = ItemIDS[j]; 
           break;
        }
    }
    return idReturn;
} 
///////////////////////
int myItem(int Start, int End){
    int var = 0;
    for(int i=Start;i<End+1;i++){
        if(game.hasItem(i)==0){
            var++;
        }
    }
    return var;
}
//////////////////////////////
int freeItem(int Start, int End, int Zone){
    int var = 0;
    for(int i=Start;i<End+1;i++){
        if(game.hasItem(i)==-1){
            if(game.posInArea(allItem[i])==Zone){
                var++;
            }else if(Zone==2){
                var++;
            }
        }
    }
    return var;
}
//////////////////////////
void takeScore(int doAfter,int Zone){
    if(MUST_SCORE_TAKE-myItem(3, 6)>freeItem(3, 6, Zone)){
       MUST_SCORE_TAKE = freeItem(3, 6, Zone);
    }
    if(myItem(3, 6)!=MUST_SCORE_TAKE){
        int index = getNearestItemID(3, 6, Zone);
        if(index!=-1){
           memcpy(whereToGo,allItem[index],sizeof(float)*3);
        }else{
            Phase = doAfter;
        }
    }else{
        Phase = doAfter;
    }
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
//End page ItemFunctions
//Begin page MovementFunctions
float angleBeetween(float *a, float *b){//Cesa-Riccio
    return acosf((a[0]*b[0]+a[1]*b[1]+a[2]*b[2])/(mathVecMagnitude(a,3)*mathVecMagnitude(b,3)));
}
///////////////////////////////////////
float getRightVelocity(int mustItem){//Cesa-Riccio
    if(mustItem == 1){
        DEBUG(("VEL BOTTARELLA A KATERINA!"));
        return getDistance(myState,whereToGo)/(timeToSwitch); //get velocity to arrive
    }if(mustItem == 0){
       DEBUG(("VEL NORMALE!"));
       return (getDistance(myState,whereToGo)*0.737837f)/ 0.06713208364316785;//0.08302409288875127f;
       
    }//0.06713208364316785f ridurre di 0.01 = -2 secondi
    return -1;
}
//////////////////////////
bool isFacingEarth(){
    float Earth[3] = {myState[0],myState[1],myState[2]+0.05f};
    float relEarth[3];
    mathVecSubtract(relEarth, Earth, myState, 3);
    mathVecNormalize(relEarth, 3);
    DEBUG(("INNER:,%f",angleBeetween(relEarth,myState+6)));
    return angleBeetween(relEarth,myState+6)<MAX_FACING_ANGLE;
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
/////////////////////////
void attitudeToEnemy(){ 
    if(!attitudeToEarth){
        if(!game.isFacingOther()){
        //if(game.posInArea(enemyState)!=-1 || (game.posInArea(enemyState)==-1 && timeToSwitch<27) ){
            /*float vet1[3] = {1,0,0};
            float vet2[3];
            float quat1[4] = {1,0,0,0};
            float quat2[4];
            mathVecSubtract(vet2,myState,enemyState,3);
            mathVecAdd(vet2,vet2,enemyState+3,3);
            mathVecNormalize(vet2,3);
            api.attVec2Quat(vet1,vet2,quat1,quat2);
            memcpy(whereAttitude,quat2,sizeof(float)*4);*/
            float futPos[3], futEnemyPos[3];
            mathVecAdd(futPos,myState,myState+3,3);
            mathVecAdd(futEnemyPos,enemyState,enemyState+3,3);
            mathVecSubtract(whereAttitude, futEnemyPos, futPos, 3);
            mathVecNormalize(whereAttitude,3);
            //  api.setAttitudeTarget(result);
            //  api.setQuatTarget(quat2);
        }
    }else{
        DEBUG(("STO GUARDANDO LA TERRA %d",isFacingEarth()));
        if(isFacingEarth()){
            if((myEnergy-1.0f)>1.5f || (myEnergy>=1.00 && api.getTime()>170)){//1.5
                DEBUG(("***TRY TO UPLOAD***"));
                game.uploadPics();
                attitudeToEarth=false;
            }
        }else{
            float earth[3]={myState[0],myState[1],myState[2]+0.05f};
            mathVecSubtract(whereAttitude,earth,myState,3);
            mathVecNormalize(whereAttitude,3);
            //game.uploadPics();
        }
    }
}


////////////////////////
int getColor(){
    api.getMyZRState(myState);
    return ((myState[0] < 0) ? 0 : 1);
}
///////////////////////
float getDistance(float a[3], float b[3]){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
//////////////////////////

/////////////////////////
void setConstantVelocityTo(float point[], float k){
		float velocityVector[3];
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
		api.setVelocityTarget(velocityVector);
}
/////////////////////////////

/////////////////////////////////
void takeAndUploadPic(){
      if(game.getMemoryFilled()==0 && CanAnotherPic()  && game.getCurrentTime()<178 && myEnergy>1.55f){
          if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
                 game.takePic();
               //  game.uploadPics();
          }
      }
          if(game.getMemoryFilled()==1 //i have one pic in memory and not be able to make another pic 
            || game.getMemoryFilled() == 2 //memory is full...upload in each circumstances
            || (game.getCurrentTime()>=178 && game.getMemoryFilled()>0)){
            //if(game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.7f){
                if(game.posInArea(myState)==1 || game.getCurrentTime()>=178 || myEnergy>=1.0f || 
                (game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.60f))
                {
                   attitudeToEarth=true;
                }
          }
     
}
///////////////////////////////////
void doMovement(){
    float toGo[]={enemyState[0],enemyState[1],enemyState[2]-0.50};
    memcpy(whereToGo,toGo,sizeof(float)*3);
    /*if(myZone!=enemyZone){
       //MUST_SCORE_TAKE=freeItem(3, 5, myZone)+1;
       takeScore(DO_MOVEMENT,myZone);
    }*/
   // MUST_ENERGY_TAKE = myItem(0,2)+1;
  //  takeEnergy(DO_MOVEMENT, LIGHT);
}
//End page MovementFunctions
//Begin page main
float myState[12],enemyState[12], allItem[9][3],myEnergy,velocity;
int timeToSwitch, Phase, Color, MUST_ENERGY_TAKE,MUST_SCORE_TAKE,myZone,enemyZone;
float whereToGo[3],whereAttitude[3];
bool attitudeToEarth;
/////////////////////////////////////////////
/////////////////////////////////////////////
////                                     ////
////    Author: Riccio-Cesa              ////
////    Last release: 04/10/2015 22:48   ////
////    Game: SpySPHERES 3D              ////
////    Team: ZRighi                     ////
////                                     ////
/////////////////////////////////////////////
/////////////////////////////////////////////

void init(){
    attitudeToEarth=false;
    getItemsPos();
    Color = getColor();
    Phase = TAKE_MIRROR;
    MUST_ENERGY_TAKE = 3; //3
    MUST_SCORE_TAKE = 2;//2
    DEBUG(("\n Greetings from the school Righi, Naples (Italy) :)"));
}
void loop(){
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    timeToSwitch = game.getLightSwitchTime();
    //HowMuchSwitch = getCounterSwitch(HowMuchSwitch);
    myEnergy = game.getEnergy();
    enemyZone = game.posInArea(enemyState);
    myZone= game.posInArea(myState);
    
    //CONTROL FOR ATTITUDE TO ENEMY BEFORE THE SWITCH OF LIGHT ZONE
    attitudeToEnemy();
    takeAndUploadPic();
  //  whenUseMirror();
    
    if(Phase==TAKE_MIRROR){
       takeMirror();
    }
  
 if(Phase==TAKE_SCORE){
        /*if(myZone!=enemyZone){
         //   DEBUG(("PIGLIAMl"));
           MUST_SCORE_TAKE=3;//3
        }else{
            MUST_SCORE_TAKE=3;//2
        }*/
        takeScore(TAKE_ENERGY,myZone);
    }
    if(Phase==TAKE_ENERGY){
        takeEnergy(DO_MOVEMENT,myZone);
    }
    if(Phase==DO_MOVEMENT){
       doMovement();
    }
    
     if(myEnergy<=0.65f){//0.85f //0.6
         velocity=getRightVelocity(1);//0.01 //0.001
         
     }else{
        velocity = (getRightVelocity(0)/100.0f);
     }
         
    DEBUG(("DISTANZA: %f",getDistance(myState,whereToGo)));
    DEBUG(("Velocity: %f",velocity));
    DEBUG(("Phase: %d",Phase));
    setConstantVelocityTo(whereToGo,velocity);

    api.setAttitudeTarget(whereAttitude);
    //DEBUG_VECTOR("Vector: ",whereToGo);
}
//End page main
