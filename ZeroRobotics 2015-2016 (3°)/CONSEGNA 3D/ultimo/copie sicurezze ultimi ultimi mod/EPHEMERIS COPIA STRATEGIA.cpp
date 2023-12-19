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
    if(game.isFacingOther() //verify if we sphere is oriented to enemy
            && myEnergy>=1.00f //1.0 should be ok //2.0 //1.55
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



/*void getNextPosEnemy(float vOut[3], int Sec)
{ //Cesa-Riccio
    //Predict position of enemy between n seconds
    vOut[0]=enemyState[0]+(enemyState[3]*Sec);
    vOut[1]=enemyState[1]+(enemyState[4]*Sec);
    vOut[2]=enemyState[2]+(enemyState[5]*Sec);
}*/
float getVelocityCazzimma(float destPoi[3], float delayTime){
    float otherDistance = getDistance(enemyState, destPoi);
    float otherT = otherDistance/enemyVel;
    return ((getDistance(myState,destPoi))/(otherT+delayTime));
}

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

void getNextPosEnemy(float vOut[3], int Sec)
{ //Cesa-Riccio
    //Predict position of enemy between n seconds
    vOut[0]=enemyState[0]+(enemyState[3]*Sec);
    vOut[1]=enemyState[1]+(enemyState[4]*Sec);
    vOut[2]=enemyState[2]+(enemyState[5]*Sec);
}

void followiamolo(){
     float toGo[3]; //-0.55
      //  getNextPosEnemy(toGo,10);
      getNextPosEnemy(toGo,5);
        toGo[2]-=0.53;
  //  if(((10*enemyState[5])+enemyState[2])>-0.04f && game.posInArea(toGo)==1){
        
    //    if(mathVecMagnitude(toGo,3)<1.10f && game.posInArea(myState)==game.posInArea(enemyState))//1.21f
        if(mathVecMagnitude(toGo,3)<1.10f){      //1.21f
            memcpy(whereToGo,toGo,sizeof(float)*3);
        }
     //   }
  //  }else{
     //   Phase = oldPhase; 
  //  }
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


int getCounterSwitch(int Switch){
    return ((timeToSwitch <2) ? Switch+1 : Switch);
}
//End page basicFunction
//Begin page main
float myState[12],enemyState[12],whereToGo[3],whereAttitude[3];
float myRotationVelocity,distanceToWhereGo,myEnergy,velocity,enemyVel,valuePic;
short timeToSwitch, Phase, Color,Time,HowMuchSwitch,oldPhase;
bool attitudeToEarth,useCazzimma;
void init(){
    Phase = HowMuchSwitch = Time = attitudeToEarth = oldPhase = 0;
    //game.sendMessage('C');
    valuePic = 1;
    DEBUG(("Greetings from ZRighi Team!"));
}
void loop(){
    api.getMyZRState(myState); //get info of my sphere
    api.getOtherZRState(enemyState); //get info of enemy sphere
    if(Time<1)
        Color = ((myState[0] < 0) ? 0 : 1); //use this for index in matrix,
         //0 == Red | 1 == Blue              //then, go to correct point, indifferently
                                            //if we are red or blue

    Time++;
    myRotationVelocity = mathVecMagnitude(myState+9,3);
    timeToSwitch = game.getLightSwitchTime();
    myEnergy = game.getEnergy(); 
    enemyVel = mathVecMagnitude(enemyState+3,3);
    HowMuchSwitch = getCounterSwitch(HowMuchSwitch);
    
    
    
    
    if(game.posInArea(myState)!=-1 && game.posInArea(enemyState)==-1){
        if(game.getNumMirrorsHeld()>0){
            game.useMirror();
        }
    }
        
    
   // if(game.getMemoryFilled()<2 && CanAnotherPic() && Time<171){
   if(game.getMemoryFilled()<2 && CanAnotherPic()){
        if(game.isCameraOn() && game.getMirrorTimeRemaining()==0){
            if(game.hasItem(8-Color)==1){
                if(game.getPicPoints()>=0.0f){
                    game.takePic();
                }
            }else{
                game.takePic();
            }
        }
   // }else if(game.getMemoryFilled()>=1 || (Time>=171 && game.getMemoryFilled()>=1)){
   }else if(game.getMemoryFilled()>=1){
	    if(myEnergy>=1.0f){
			attitudeToEarth=true;
        }
    }
    if(game.getMemoryFilled()>=1 && Time>=171){
        attitudeToEarth=true;
    }
    
    if(!attitudeToEarth){
        /*float futPos[3], futEnemyPos[3];
        mathVecAdd(futPos,myState,myState+3,3);
        mathVecAdd(futEnemyPos,enemyState,enemyState+3,3);*/
        mathVecSubtract(whereAttitude, enemyState, myState, 3);
        mathVecSubtract(whereAttitude, enemyState, myState, 3);
        mathVecNormalize(whereAttitude,3);
    }else{
        float earth[3]={myState[0],myState[1],myState[2]+0.05f};
        mathVecSubtract(whereAttitude,earth,myState,3);
        mathVecNormalize(whereAttitude,3);
        if(isFacingEarth()){
            if((myEnergy-1.0f)>0.5f || (myEnergy>=1.00 && Time>170)){
                if(myRotationVelocity<=0.05f)//0.05{
                    game.uploadPics();
                    attitudeToEarth=false; 
                
            }
        }
    }
    api.setAttitudeTarget(whereAttitude); //one command for attitud
    /*DEBUG(("TO SWITCH: %d",timeToSwitch));
    DEBUG(("ENEMY DISTANCE: %f",getDistance(myState,enemyState)));*/
   // DEBUG(("ENEMY VEL: %f",mathVecMagnitude(enemyState+3,3)));
   // DEBUG(("Phase: %d",Phase));
  //  if(((10*enemyState[5])+enemyState[2])>0.05f && HowMuchSwitch>0 && game.posInArea(enemyState)==1){
      if(((5*enemyState[5])+enemyState[2])>0.05f && HowMuchSwitch>0 && game.posInArea(enemyState)==1){
      
        oldPhase = Phase;
        Phase = 6;
    }else{
        oldPhase = Phase;
       Phase = oldPhase; 
    }
    
    if(Phase==0){
        memcpy(whereToGo, ITEM_LOC[Color+7],sizeof(float)*3);
        if(game.posInArea(myState)!=-1 && game.posInArea(enemyState)!=-1 && HowMuchSwitch < 1){
            Phase=1;
            useCazzimma=true;
        }else{
            if(game.hasItem(Color+7)!=-1){
                Phase=1;
                useCazzimma=false;
            }
        }
    }
    
    if(Phase==1){
        memcpy(whereToGo, ITEM_LOC[6],sizeof(float)*3);
        if(game.hasItem(6)!=-1){
            Phase=2;
        }
    }
     if(Phase==2){
        memcpy(whereToGo, ITEM_LOC[5-Color],sizeof(float)*3);
        if(game.hasItem(5-Color)!=-1){
            Phase=3;
        }
    }
    if(Phase==3){
        float StrategyPic[]={Color==0?-0.408f:0.408, 0.604f, -0.596f};
        memcpy(whereToGo, StrategyPic, sizeof(float)*3);
    }
    if(Phase==6){
       followiamolo();
    }
    
   distanceToWhereGo = getDistance(myState,whereToGo);
   if(myEnergy<=0.65f){ //1.00
       velocity=0.000f;
   }else{
       if(Phase==0){
           velocity = (((distanceToWhereGo*0.68f)/ 0.05459f)/100.0f); // 0.0577148161220323f
        }else if(Phase==1){
            if(!useCazzimma){
                velocity = (((distanceToWhereGo*0.68f)/ 0.0663f)/100.0f);  //0.0663
            }else{
                if(getDistance(enemyState,whereToGo)>0.23f && enemyVel<=0.02){
                    velocity = (((distanceToWhereGo*0.68f)/ 0.0677148161220323f)/100.0f);
                  //  DEBUG(("sono qui"));
                }else{
                   if(distanceToWhereGo>=0.23){
                     //   DEBUG(("ARRIVO DOPO DI LUI"));
                        velocity = getVelocityCazzimma(whereToGo,3.0f);//4 //3 //3.4f
                    }else{
                     //   DEBUG(("MI FERMO"));
                        velocity = (((distanceToWhereGo*0.68f)/ 0.09459f)/100.0f);
                    }  
                }
                      
            }
        }else{
            velocity = (((distanceToWhereGo*0.68f)/ 0.0677148161220323f)/100.0f); 
        }
    }
    
   /* if(game.getScore() == game.getOtherScore() && Time>175){
        game.takePic();
    }*/
    
    // DEBUG(("DISTANZA: %f",getDistance(myState,whereToGo)));
    // DEBUG(("Velocity: %f",velocity));
    // DEBUG(("Phase: %d",Phase));
    // DEBUG(("Velocity: %f",velocity));
    // DEBUG(("%f %f %f",whereToGo[0], whereToGo[1], whereToGo[2]));
    // DEBUG(("PhaseMAMT: %d",Phase));
    // DEBUG((" mia rotazione %f",myRotationVelocity));
    setConstantVelocityTo(whereToGo,velocity); //one command for movement
    
}
//End page main
