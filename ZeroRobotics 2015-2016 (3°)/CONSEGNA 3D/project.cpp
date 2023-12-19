//Begin page ItemFunctions
void takeMirror()//prende il mirro all'inizio del game
{
    memcpy(whereToGo,allItem[Color+7],sizeof(float)*3);
    if(game.hasItem(Color+7)==0)
    {
       Phase=3;
    }
}

//End page ItemFunctions
//Begin page MovementFunctions
float angleBeetween(float *vectorA, float *vectorB)//prende l'angolo tra 2 vettori
{
 return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}

bool isFacingEarth() //verifica se siamo orientati verso la terra
{
    float Earth[3] = {myState[0],myState[1],myState[2]+0.05f};
    float realEarth[3];
    mathVecSubtract(realEarth, Earth, myState, 3);
    mathVecNormalize(realEarth, 3);
   // DEBUG(("INNER:,%f",angleBeetween(realEarth,myState+6)));
    return angleBeetween(realEarth,myState+6)<acosf(MAX_FACING_ANGLE);
}

bool CanAnotherPic(){//verifica se è possibile scattare altre foto
    if(facing_enemy
            && myEnergy>=1.55f //1.0 should be ok //2.0
            && game.posInArea(enemyState)!=-1 
            && getDistance(myState,enemyState)>=0.5f)
            { 
                return true;
            }
    return false;
}

/*void whenUseMirror(){
if(game.posInArea(myState)!=-1
    && game.posInArea(enemyState)==-1
    ){
       game.useMirror();
    }
}*/




void attitudeToEnemy()//si orenta verso il nemico per fare le foto, e verso la terra per fare l'upload
{ 
    if(!attitudeToEarth)
    {
        if(!facing_enemy)
        {
            float futPos[3], futEnemyPos[3];
            mathVecAdd(futPos,myState,myState+3,3);
            mathVecAdd(futEnemyPos,enemyState,enemyState+3,3);
            mathVecSubtract(whereAttitude, futEnemyPos, futPos, 3);
            mathVecNormalize(whereAttitude,3);
        }
    }
    else
    {
        DEBUG(("STO GUARDANDO LA TERRA %d",isFacingEarth()));
        if(isFacingEarth())
        {
            if((myEnergy-1.0f)>1.5f || (myEnergy>=1.00 && Time>170))
            {//1.5
                DEBUG(("***TRY TO UPLOAD***"));
                attitudeToEarth=false;
                game.uploadPics();
                
            }
        }
        else
        {
            float earth[3]={myState[0],myState[1],myState[2]+0.05f};
            mathVecSubtract(whereAttitude,earth,myState,3);
            mathVecNormalize(whereAttitude,3);
            //game.uploadPics();
        }
    }
}


void takeAndUploadPic()
{//in base alle foto in memoria..deside se scattare o caricare
      if(game.getMemoryFilled()<2 && CanAnotherPic()  && Time<178 && myEnergy>1.55f)
      {
          if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){ //dobbiamo farle noi queste funzioni
                 game.takePic();
          }
      }
      
     /* if(){
          
      }*/
      
      else if(game.getMemoryFilled()>=1
            || (Time>=178 && game.getMemoryFilled()>=1)){
           
               /** if(game.posInArea(myState)==1 || Time>=178 || myEnergy>=1.0f || 
                (game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.60f))
                {
                   attitudeToEarth=true;
                }*/
                 if(game.posInArea(myState)==1 || myEnergy>=1.0f || 
                (game.getMirrorTimeRemaining()==0 && (myEnergy-1)>0.60f)){
                   attitudeToEarth=true;
                }
          
    } 
}


/*int getColor(){
    api.getMyZRState(myState);
    return ((myState[0] < 0) ? 0 : 1);
}*/

float getDistance(float a[3], float b[3])//restituisce la distanza tra due punti
{
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}

void setConstantVelocityTo(float point[], float k)//funzione per il movimento
{
		float velocityVector[3];
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
		api.setVelocityTarget(velocityVector);
}


void doMovement()//ci mettiamo sopra il nemico e scattiamo se le condizioni sono favorevoli
{
    if(enemyState[2]>-0.04f){
        float toGo[3]={enemyState[0],enemyState[1],enemyState[2]-0.55f}; //-0.55
        memcpy(whereToGo,toGo,sizeof(float)*3);
    }
    else{
            if(game.getNumMirrorsHeld()==1){
            game.useMirror();
        }
            memcpy(whereToGo,allItem[6],sizeof(float)*3);
    }
}
//End page MovementFunctions
//Begin page main
float myState[12],enemyState[12],myEnergy,velocity;
float allItem[9][3];
short timeToSwitch, Phase, Color,Time;
float whereToGo[3],whereAttitude[3];
bool attitudeToEarth,facing_enemy;

/*float support[3];
short temp;*/


void init()
{
    attitudeToEarth=false;
    for(int i=0;i<9;i++)
        game.getItemLoc(allItem[i],i);
    
    Time = Phase = 0;
    DEBUG(("\n Greetings from the school Righi, Naples (Italy) :)"));
}

void loop()
{
    
    api.getMyZRState(myState);
    api.getOtherZRState(enemyState);
    if(Time<1)
        Color = ((myState[0] < 0) ? 0 : 1);

    Time++;
    facing_enemy=game.isFacingOther();
    timeToSwitch = game.getLightSwitchTime();
    myEnergy = game.getEnergy();
    //HowMuchSwitch = getCounterSwitch(HowMuchSwitch);
    //  enemyZone = game.posInArea(enemyState);
    //  myZone= game.posInArea(myState);
    
    //CONTROL FOR ATTITUDE TO ENEMY BEFORE THE SWITCH OF LIGHT ZONE
    attitudeToEnemy();
    takeAndUploadPic();
    //whenUseMirror();
    
    if(Phase==0){
       takeMirror();
    }
  
   /* if(Phase==1){
        takeScore(2,myZone);
    }
    if(Phase==2){
        takeEnergy(3,myZone);
    }*/
    if(Phase==3){
       doMovement();
    }
   
   
   //proporzione per la velocità...quando invece l'energia diventa poca, e quindi il movimento è a rischio...andiamo verso il punto da noi indicato per inerzia
   velocity = ((myEnergy<=0.65f) ? getDistance(myState,whereToGo)/(timeToSwitch) : ((getDistance(myState,whereToGo)*0.68f)/ 0.0377148161220323f)/100.0f);
   //0.737837f, 0.06713208364316785f
   
   /* if(myEnergy<=0.65f){//0.85f //0.6
         velocity= getDistance(myState,whereToGo)/(timeToSwitch);//0.01 //0.001
    }else{
       velocity = ((getDistance(myState,whereToGo)*0.737837f)/ 0.06713208364316785)/100.0f;
    }*/
 /*   DEBUG(("DISTANZA: %f",getDistance(myState,whereToGo)));
    DEBUG(("Velocity: %f",velocity));
    DEBUG(("Phase: %d",Phase));*/
    setConstantVelocityTo(whereToGo,velocity); //decide dove muoversi
    api.setAttitudeTarget(whereAttitude); //decide dove orienstarsi
}
//End page main
