//Begin page ItemFunctions
void takeMirror()//Cesa-Riccio
{
    memcpy(whereToGo, ITEM_LOC[Color+7],sizeof(float)*3);
    if(game.hasItem(Color+7)==0)
    {
       Phase=2;
    }
}

//End page ItemFunctions
//Begin page MovementFunctions
bool isFacingEarth()//Cesa-Riccio       //get vector from we and earth
{                                       //and get our attitude vector,
                                        //if angle lower of tolerance (cosf(0.25f)
                                        //return true,else return false
    float Earth[3] = {myState[0],myState[1],myState[2]+0.05f};
    float realEarth[3];
    mathVecSubtract(realEarth, Earth, myState, 3);
    mathVecNormalize(realEarth, 3);
   // DEBUG(("INNER:,%f",angleBeetween(realEarth,myState+6)));
    return angleBetween(realEarth,myState+6)<acosf(MAX_FACING_ANGLE);
}

bool CanAnotherPic()
{//Cesa-Riccio
    if(facing_enemy //verify if we sphere is oriented to enemy
            && myEnergy>=1.55f //1.0 should be ok //2.0
            && game.posInArea(enemyState)!=-1 //we can take pic only when enemy zone is different from DARK
            && getDistance(myState,enemyState)>=0.5f) //we can take pic only when enemy distance is greate or ideam 0.5
            { 
                return true;
            }
    return false;
}

void whenUseMirror()//Cesa-Riccio
{
    if(game.posInArea(myState)!=-1 && game.posInArea(enemyState)==-1 && timeToSwitch>=6)
    {
        if(game.getNumMirrorsHeld()>0)
        {
            game.useMirror();
        }
    }
}

void attitudeToEnemy()//Cesa-Riccio
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
      //  DEBUG(("STO GUARDANDO LA TERRA %d",isFacingEarth()));
        if(isFacingEarth())
        {
            if((myEnergy-1.0f)>0.5f || (myEnergy>=1.00 && Time>170))
            {//1.5
                if(myRotationVelocity<=0.05f)//0.05
                {
                   // DEBUG(("***TRY TO UPLOAD***"));
                    game.uploadPics();
                    attitudeToEarth=false; 
                }
            }
        }
        else
        {
            float earth[3]={myState[0],myState[1],myState[2]+0.05f};
            mathVecSubtract(whereAttitude,earth,myState,3);
            mathVecNormalize(whereAttitude,3);
        }
    }
}

void takeAndUploadPic()
{//Cesa-Riccio
    if(game.getMemoryFilled()<2 && CanAnotherPic() && Time<171){
        if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
            //DEBUG((" mammt cos %f",game.getPicPoints()));
            if(game.hasItem(8-Color)==1){
                if(game.getPicPoints()>=0.0f){
                    //DEBUG(("FACCIO VERIFICA"));
                    game.takePic();
                }
            }else{
                //DEBUG(("NON FACCIO VERIFICA"));
                game.takePic();
            }
            /*if(valuePic>=0 || lastTime+24==Time){
            valuePic = game.takePic();
            DEBUG(("value: %f",valuePic));
            lastTime=Time;
            }else{
                DEBUG(("non scattare"));
            }*/
        }
    }
    else if(game.getMemoryFilled()>=1 || (Time>=171 && game.getMemoryFilled()>=1)){
	    if(myEnergy>=1.0f){
			attitudeToEarth=true;
        }
    } 
}

/*void getNextPosEnemy(float vOut[3], int Sec)
{ //Cesa-Riccio
    //Predict position of enemy between n seconds
    vOut[0]=enemyState[0]+(enemyState[3]*Sec);
    vOut[1]=enemyState[1]+(enemyState[4]*Sec);
    vOut[2]=enemyState[2]+(enemyState[5]*Sec);
}*/

void doMovement()
{//Cesa-Riccio //go to "Strategic" point
     float StrategyPic[]={Color==0?-0.408f:0.408, 0.604f, -0.596f};
     memcpy(whereToGo, StrategyPic, sizeof(float)*3);
   
   /* if(((10*enemyState[5])+enemyState[2])>-0.04f)
    {
         float toGo[3]; //-0.55
        getNextPosEnemy(toGo,10);
        toGo[2]-=0.55;
    //    if(mathVecMagnitude(toGo,3)<1.10f && game.posInArea(myState)==game.posInArea(enemyState))//1.21f
        if(mathVecMagnitude(toGo,3)<1.10f)//1.21f
        {
            memcpy(whereToGo,toGo,sizeof(float)*3);
        }
    }
    else{
            if(game.posInArea(myState)!=game.posInArea(enemyState))
            {
                if(game.hasItem(5-Color)!=0)
                {
                    memcpy(whereToGo,allItem[5-Color],sizeof(float)*3);
                     DEBUG(("OFRA"));
                } 
            }
            else
            {
                Phase = 0;
            }
      //  }
    }*/
    
}
//End page MovementFunctions
//Begin page basicFunction
float angleBetween(float *vectorA, float *vectorB)
{//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}

float getDistance(float a[3], float b[3]) //get distance from 2 point
{
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}

void setConstantVelocityTo(float point[], float k)
{//our function for movement... that use setVelocity api, give by platform
		float velocityVector[3];
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
		api.setVelocityTarget(velocityVector);
}
//End page basicFunction
//Begin page main
float myState[12],enemyState[12],myEnergy,velocity;
float myRotationVelocity;
short timeToSwitch, Phase, Color,Time;
float whereToGo[3],whereAttitude[3];
bool attitudeToEarth,facing_enemy;
//int lastTime;
float valuePic;

void init(){
    attitudeToEarth=false;
    Time = 0;
    Phase = valuePic = 1;
    DEBUG(("Greetings from ZRighi Team!"));
    //DEBUG(("\nGiovanni Cesarano, Emanuele Riccio"));
}

void loop()
{
    api.getMyZRState(myState); //get info of my sphere
    api.getOtherZRState(enemyState); //get info of enemy sphere
    if(Time<1)
        Color = ((myState[0] < 0) ? 0 : 1); //use this for index in matrix,
         //0 == Red | 1 == Blue              //then, go to correct point, indifferently
                                            //if we are red or blue

    Time++;  //our WAY TO get Time and optimize codesize
    myRotationVelocity = mathVecMagnitude(myState+9,3);
    facing_enemy = game.isFacingOther(); //   TRUE if oriented to Enemy, otherwise FALSE
    timeToSwitch = game.getLightSwitchTime(); //get time to the next Switch
    myEnergy = game.getEnergy(); //get my Energy
    
    //General functionS for strategy AHAHHAAHAHAH
    attitudeToEnemy();
    takeAndUploadPic();
    //whenUseMirror();
    
    if(Phase==0)
    {                   //phase IN WICH we take mirror,in the current strategy is not used
     //  takeMirror();
    }
    
    if(Phase==1) //The game start with Phase = 1; in this phase the sphere go to the score in
    {            //negative Z axis
    
        //  velocity = (((getDistance(myState,whereToGo)*0.68f)/ 0.0477148161220323f)/100.0f);
        if(game.hasItem(6)==-1)//verify if item(score in this case) is not already picked
        {
            memcpy(whereToGo, ITEM_LOC[6],sizeof(float)*3);
            //DEBUG(("OFRA"));
        }
        else if(game.hasItem(5-Color)==-1 //when our and their sphere, is in different zone
                                        //verify if item is not already picked and go on that
            )
           // && game.posInArea(myState)!=game.posInArea(enemyState)
        {
            memcpy(whereToGo, ITEM_LOC[5-Color],sizeof(float)*3);
           // DEBUG(("OFRA1"));
        }
        /*else if(){
            
        }*/
        else
        {
            //when both score (predict by strategy) are picked, change phase to 2;
            Phase = 2;
        }
    }
    
    if(Phase==2)
    {
       doMovement(); //go to Strategic point
    }
    
   if(myEnergy<=1.00f) //alarm control
   {
       //velocity=0.001f; //Instantly reduce velocity
       velocity=0.000f;
   }
    else
    {
        if(Phase==1)
        {
            
            //change velocity according to the movement (that is the current Phase)
            
           velocity = (((getDistance(myState,whereToGo)*0.68f)/ 0.0663f)/100.0f); 
         
         
           //velocity = (((getDistance(myState,whereToGo)*0.77336925203941f)/ 0.055217750769114095f)/100.0f);
           
           //+0.02 = +4 sec  0.04624f
        }//0.0457148161220323f
        else
        {
            velocity = (((getDistance(myState,whereToGo)*0.68f)/ 0.0677148161220323f)/100.0f); 
        }
    }
    
    if(game.getScore() == game.getOtherScore() && Time>175){
        game.takePic();
    }
   // DEBUG(("DISTANZA: %f",getDistance(myState,whereToGo)));
   // DEBUG(("Velocity: %f",velocity));
   // DEBUG(("Phase: %d",Phase));
    //DEBUG(("Velocity: %f",velocity));
    //DEBUG(("%f %f %f",whereToGo[0], whereToGo[1], whereToGo[2]));
   // DEBUG(("PhaseMAMT: %d",Phase));
  //  DEBUG((" mia rotazione %f",myRotationVelocity));
    setConstantVelocityTo(whereToGo,velocity); //one command for movement
        api.setAttitudeTarget(whereAttitude); //one command for attitud
}
//End page main
