//Begin page controllerZoneScrolling

//Begin page controllerZoneScrolling
float getLightFrontActualPos(float actualY){
    float frontInterface = actualY+0.85f;
    return ((frontInterface>0.8)?frontInterface-1.6f:frontInterface);
}
void getLightNexPos(float actualY,int nextTime, float outY[2]){
    float nextYBack = actualY + (0.025f * nextTime);
    outY[0]=((nextYBack>=0.8f)?(-0.8f)+(nextYBack-0.8f):nextYBack);
    float nextYFront = outY[0] + 0.9f;
    outY[1]=((nextYFront>=0.8f)?(-0.8f)+(nextYFront-0.8f):nextYFront);
}
bool getNextObjectArea(float objectState[12], float actualY, int nextTime){
    float objectNextY = objectState[1]+(objectState[4]*nextTime);
    float nextYBack = (actualY) + (0.025f * nextTime);
    nextYBack=((nextYBack>=0.8f)?(nextYBack-1.6f):nextYBack);
    float nextYFront = nextYBack + 0.9f;
    nextYFront=((nextYFront>=0.8f)?(nextYFront-1.6f):nextYFront);
    return (objectNextY<nextYFront && objectNextY>nextYFront-0.9f)||
        (objectNextY<nextYBack+0.9f && objectNextY>nextYBack);
    //1 = Light\Grey
    //0 Dark
}

//End page controllerZoneScrolling
//End page controllerZoneScrolling
//Begin page functions
int choseScoreNearMirror()
{
    float lastDistance = 50.0f;
    int choosenIndex = -1,i=3;
    while(i<7)
    {
        float actualPosition[3];
        game.getItemLoc(actualPosition,i);
        float nowDistance = getDistance(mirror,actualPosition);//Prendo la distanza dallo score attuale
        if(nowDistance<lastDistance && game.hasItem(i)==-1)
        {
            choosenIndex = i;
            lastDistance = nowDistance;
        }
        
        i++;
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex);
    return choosenIndex;
    
}

int chooseRightEnergy(float myState[12],float whereToGo[3], int numEnergy){
    float lastDistance = 5.00f; 
    int choosenIndex=-1;
    int numItemTaken=0;
    for(int i=0;i<3;i++){
        (game.hasItem(i)==0)?numItemTaken++:0;
    }
    if(numItemTaken<numEnergy){
       for(int i=0;i<3;i++){
        float nowPosition[3];
        game.getItemLoc(nowPosition,i);
        float nowDistance = getDistance(myState,nowPosition);
        if((nowDistance<lastDistance) && (game.hasItem(i)==-1)){
            choosenIndex = i;
            lastDistance = nowDistance;
        }
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex); 
    }
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
}
bool isGoing(float itemPos[3],float objectState[12]){
    float myVett[3],itemVett[3];
    for(int i=0;i<3;i++)
        myVett[i]=objectState[i];
    mathVecSubtract(itemVett,itemPos,objectState,3);
    mathVecNormalize(itemVett,3);
    return angleBetween(myVett,itemVett)<0.15f;
}
int getSituation(){
    float firstPos[3],secondPos[3];
    game.getItemLoc(firstPos,3);
    game.getItemLoc(secondPos,5);
    if(firstPos[2]<=0.00f && secondPos[2]<=0.00f){
        return 0;
    }else if((firstPos[2]<=0.00f && secondPos[2]>=0.00f)||(firstPos[2]>=0.00f && secondPos[2]<=0.00f)){
        if(firstPos[2]>0.200f || secondPos[2]>0.200f){
            return 2;
        }
        return 1;
    }else{
     //   if(firstPos[2]>0.300f || secondPos[2]>0.300f){
          if(firstPos[2]>0.450f && secondPos[2]>0.450f){
            return 4;
        }
        return 3;
    }
return 5;
//0 Sopra
//1 Medio Alto
//2 Medio Basso
//3 Basso Alto
//4 Bassotto
//5 BOH
}
float angleBetween(float *vectorA, float *vectorB){//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}
int chooseRightScore(short actualSituation, float myState[12],float enemyState[12],float whereToGo[3], int numScores,short myZone,short enemyZone){
    float lastDistance = 5.00f; 
    short choosenIndex=-1;
    short numItemTaken=0;
    for(int i=0;i<4;i++){
        (game.hasItem(i+3)==0)?numItemTaken++:0;
    }
    if(numItemTaken<numScores){
        
        for(int i=0;i<4;i++){
            short actualIndex = i+3;
            float nowPosition[3];
            game.getItemLoc(nowPosition,actualIndex);
            float nowDistance = getDistance(myState,nowPosition);
            bool performCondition = false;
            //0 Sopra
            //1 Medio Alto || due sopra e due sotto prendibili
            //2 Medio Basso || due sopra e due sotto troppo bassi, imprendibili
            //3 Basso Alto || 4 sotto, 2 prendibili e 2 no
            //4 Bassotto || tutti non prendibili
        //    DEBUG(("TimeFREE: %f",freeTime));
            
            switch(actualSituation){
                case 0:
                    performCondition = true;
                    break;
                case 1:
                    performCondition = (nowPosition[2]<=0.00f)||(nowPosition[2]<=0.150f&&enemyState[2]>-0.200f);
                    break;
                case 2:
                    performCondition = (nowPosition[2]<=0.00f)||(nowPosition[2]<=0.450f&&enemyState[2]>0.100f);
                    break;
                case 3:
                    performCondition = (nowPosition[2]<=0.450f&&enemyState[2]>0.100f);
                    break;
                case 4:
                    performCondition = false;
                    break;
            }
            if((nowDistance<lastDistance) && (game.hasItem(actualIndex)==-1) && performCondition  && getDistance(enemyState,nowPosition)>0.1f){
                    choosenIndex = actualIndex;
                    lastDistance = nowDistance; 
            } 
        }//
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex); 
    }//
  //  DEBUG(("INDEX: %d",choosenIndex));
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
}


int scelta_score(float state[12]){
    float lastDistance = 5.00f; //Conterra' l'ultima distanza registrata valida,
                                //E' impostata ad un valore grande 50.00f
                                //Cosi' al primo ciclo si resettera' prendere la distanza
                                //del primo score esaminato
    int choosenIndex = -1,i=0;
    for(i=0;i<4;i++){
        float nowPosition[3];//Posizione dello score attuale esaminato
        game.getItemLoc(nowPosition,i+3);//Prendo la pos e la metto in nowPosition
        float nowDistance = getDistance(state,nowPosition);//Prendo la distanza dallo score attuale
        if((nowDistance<lastDistance)&&game.hasItem(i+3)==-1){
            //Se la (distanza attuale<di quella precedentemente valida) && lo score attuale
            //e' libero 
            choosenIndex = i+3;//L'ID valido viene aggiornato a quello dello score attualmente esaminato
            lastDistance = nowDistance;//Stessa cosa per la distanza
        }
    }
    if(choosenIndex!=-1)
        game.getItemLoc(whereToGo,choosenIndex);
   // DEBUG(("%d",choosenIndex));    
    //Se choosenIndex!=-1, quindi almeno uno score e' risultato valido, copia la sua posizione
    return choosenIndex;
    
}
//End page functions
//End page functions
//Begin page main

float whereToGo[3];
float whereAttitude[3];

float myState[12];
float enemyState[12];
float mirror[3];

float score;
float energy;

float velocity;
float kVel;

    
    

short actualProcedure, Color,actualFrame;

bool attitudeToEarth, takePics;
bool allScore;
    




void init()
{
    actualProcedure = actualFrame = attitudeToEarth  = 0; 
    takePics = true; 
    allScore = false;
}

void loop()
{
    api.getMyZRState(myState);api.getOtherZRState(enemyState);
    if(actualFrame<1) Color = ((myState[0] < 0) ? 0 : 1); actualFrame++;
    score = game.getScore();energy = game.getEnergy();
    bool watchingEnemy = game.isFacingOther();
    float distanceToEnemy = getDistance(myState,enemyState),
                            actualBackInterface=game.getLightInterfacePosition(),
                            actualFrontInterface=actualBackInterface+0.9f;
                            
    actualFrontInterface-= (actualFrontInterface>0.8)?1.65f:0.00f;
    
    short picInsideMemory = game.getMemoryFilled(),
                            myZone = game.posInArea(myState),
                            enemyZone = game.posInArea(enemyState);
    if(picInsideMemory<2 && conditionToTakePic(watchingEnemy,myZone,enemyZone,distanceToEnemy) && takePics)
    {
        if(game.isCameraOn() && game.getMirrorTimeRemaining()==0)
        {
            (game.getPicPoints()>=0)?game.takePic():0;
        }
    }
    if(((picInsideMemory>=1 && actualFrame>168)
        || (actualProcedure>7 && energy<3.4f && picInsideMemory>0)
        || (picInsideMemory==1 && myZone==-1 && enemyZone==-1 && energy>2.3f) 
        || (myZone==1 && game.getMirrorTimeRemaining()>0 && picInsideMemory>0)
        || (picInsideMemory>1)))
    {
        attitudeToEarth=true;
    }
    
    if(takePics)
    {
        if(!attitudeToEarth)
        {
            mathVecSubtract(whereAttitude, enemyState, myState, 3);
        }
        else
        {
            float earth[3]={myState[0],myState[1],myState[2]+0.05f};
            mathVecSubtract(whereAttitude,earth,myState,3);
            if(myState[8]>0.9631533648912f)
            {
                if((energy>=1.55f) || (energy>=1.00 && actualFrame>140))
                {
                    if(mathVecMagnitude(myState+9,3)<=0.05f)
                    {
                        game.uploadPics();
                        attitudeToEarth=false;
                    }
                    
                }
            }
        }
    
        mathVecNormalize(whereAttitude,3);
        if(actualProcedure!=6 || actualProcedure==6 && (!attitudeToEarth||myZone!=-1)){
            api.setAttitudeTarget(whereAttitude);
        }
    }
    else{
        whereAttitude[0]=myState[6];
        whereAttitude[1]=myState[7];
        whereAttitude[2]=myState[8];
        api.setAttitudeTarget(whereAttitude);
    }
    
    game.getItemLoc(mirror,8-Color);

    if(score>1.4&&enemyState[1]<=(mirror[1]+0.02))
        allScore = true;
        
    DEBUG(("allScore=%d",allScore));
   
    if(actualProcedure==0)
    {
        float distanceToNextScore = getDistance(myState,whereToGo);
        if((choseScoreNearMirror()==-1)||(score>1.4 && (distanceToNextScore>0.05f
        ||distanceToEnemy<0.35f)&&(!allScore)))
            actualProcedure=5;
            
        if(!allScore)
            kVel = 0.095077148161220323f;
        else
            kVel = 0.06f;
    }
    
    if(actualProcedure==5)
    {
        if(game.hasItem(8-Color)==-1)
        {
            game.getItemLoc(whereToGo,8-Color);
        }else if ((myState[8]>0.955f)||(game.getScore()>4.7) 
        ||(game.hasItem(8-Color)==1)||(actualFrame>95)||(actualFrame>70&&myZone==-1)
        ||(enemyState[1]<(myState[1]-0.05f))){
            actualProcedure=6;
        }                                                                   
        kVel = 0.06007148161220323f;
    }
    
    if(actualProcedure==6)
    {
        if((myState[1]-actualFrontInterface)<0.05f && distanceToEnemy>=0.48f && actualFrame>75)
        {
            game.useMirror();
        }
        whereToGo[0]=(Color==1)?0.3f:-0.3f;
        whereToGo[2]=0.0f;
        
        if(choseScoreNearMirror()==-1||actualFrame>120||myState[1]>(actualBackInterface-0.1)&&actualFrame<105)
            whereToGo[1]=actualBackInterface-0.2;
        
        kVel = 0.1207148161220323f;
        
        if(actualBackInterface>myState[1] && myZone==-1 && actualFrame>114)
        {
            actualProcedure=8;
        }
        //if(score>game.getOtherScore() && (myState[1]-0.1)<actualBackInterface && actualFrame>105)
            //takePics = false;
    }
    
    
    
    if(actualProcedure==8)
    {
        if(score>game.getOtherScore()&&picInsideMemory<1)
            takePics = false;
        else 
            takePics = true;
            
        if(scelta_score(myState)==-1)
        {
            whereToGo[0]=(Color==1)?0.4f:-0.4f;
            whereToGo[1]=0.5f;
            kVel = 0.14f;
        }
        else
        {
            kVel = 0.0846f;
        }
        
    }
    if(energy<1) game.takePic();
    
    float distanceToWhereGo = getDistance(myState, whereToGo);
    //if(actualProcedure!=8){
        velocity = (((distanceToWhereGo*0.68f)/kVel)/100.0f);
    //}
    //else if((distanceToWhereGo>0.6f)||(actualFrame>153&&distanceToWhereGo>0.25f))
    /*{
        velocity = 0.034f;
    }
    else if(distanceToWhereGo<=0.6f && distanceToWhereGo>0.17f)
    {
        velocity = 0.02f;
    }
    else
    {
        velocity = 0.008f;
    }*/
       
    DEBUG(("fase:%d",actualProcedure));
    float velocityVector[3];
	mathVecSubtract(velocityVector, whereToGo, myState, 3);
	mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++)
		velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
	
}
//End page main
//Begin page takePic
bool conditionToTakePic(bool myAttitudeToEnemy,short myZone,short enemyZone,float myState_enemyState){
    return (myAttitudeToEnemy && ((myZone != 1) ? ((energy>=1.55 && actualProcedure!=0) || (energy>2.45f && actualProcedure==0)) : energy>=1.00) && enemyZone!=-1 && myState_enemyState>=0.5f); 
}
//End page takePic
