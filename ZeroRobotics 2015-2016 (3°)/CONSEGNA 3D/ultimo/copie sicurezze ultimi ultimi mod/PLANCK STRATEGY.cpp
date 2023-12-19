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

void CesaStrategy(){
    
/*    if(Phase==2){
     //  doMovement(); 

    }*/
}

void GugliottaStrategy(){
    
    
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
short timeToSwitch, Phase, Color,Time,timeToFirstSwitch;
float whereToGo[3],whereAttitude[3],nearEnergy[3];
bool attitudeToEarth;
float valuePic;
bool whichStrategy;
float distanceToNextPoint;
int infoMirror,doveSono;

void init(){
    attitudeToEarth=false;
    Phase = Time = 0;
    valuePic = 1;
    //DEBUG(("Greetings from ZRighi Team!"));
    timeToFirstSwitch=game.getLightSwitchTime();
    //DEBUG(("\nGiovanni Cesarano, Emanuele Riccio, Francesco Gugliotta"));
    
    
}

void loop()
{
    api.getMyZRState(myState); //get info of my sphere
    api.getOtherZRState(enemyState); //get info of enemy sphere
    doveSono=game.posInArea(myState);
    
    if(Time<1){
        Color = ((myState[0] < 0) ? 0 : 1); //use this for index in matrix,
         //0 == Red | 1 == Blue              //then, go to correct point, indifferently
                                            //if we are red or blue
        memcpy(nearEnergy,ITEM_LOC[1-Color],sizeof(float)*3);
        nearEnergy[2]-=0.3f;
        /*nearEnergy[0]=ITEM_LOC[1-Color][0];
        nearEnergy[1]=ITEM_LOC[1-Color][1];
        nearEnergy[2]=ITEM_LOC[1-Color][2]-0.3; */
    }
    infoMirror=game.hasItem(Color+7);

    Time++;  //our WAY TO get Time and optimize codesize
    myRotationVelocity = mathVecMagnitude(myState+9,3);
  //  facing_enemy = ; //   TRUE if oriented to Enemy, otherwise FALSE
    timeToSwitch = game.getLightSwitchTime(); //get time to the next Switch
    myEnergy = game.getEnergy(); //get my Energy
    //DEBUG(("FASE:%d",Phase));
    //Cesa-Riccio
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
        }
    }else if(game.getMemoryFilled()>=1 || (Time>=171 && game.getMemoryFilled()>=1)){
	    if(myEnergy>=1.0f){
			attitudeToEarth=true;
        }
    } 
    //Cesa-Riccio
    if(!attitudeToEarth){
       // if(!facing_enemy)
       // {
            float futPos[3], futEnemyPos[3];
            mathVecAdd(futPos,myState,myState+3,3);
            mathVecAdd(futEnemyPos,enemyState,enemyState+3,3);
            mathVecSubtract(whereAttitude, futEnemyPos, futPos, 3);
            mathVecNormalize(whereAttitude,3);
          //  api.setAttitudeTarget(whereAttitude); //one command for attitud
      //  }
    }else{
        float earth[3]={myState[0],myState[1],myState[2]+0.05f};
        mathVecSubtract(whereAttitude,earth,myState,3);
        mathVecNormalize(whereAttitude,3);
        if(isFacingEarth()){
            if((myEnergy-1.0f)>0.5f || (myEnergy>=1.00 && Time>170)){
                if(myRotationVelocity<=0.05f){
                    game.uploadPics();
                    attitudeToEarth=false; 
                }
            }
        }
    }
   
    //whenUseMirror();
    
    if(Phase==0){     
        //DEBUG(("ooh yeah! all how I want"));
        memcpy(whereToGo,ITEM_LOC[Color+7],sizeof(float)*3);
        if(infoMirror==0){
            if(enemyState[1]<0.0){
               
                whichStrategy=false;
                Phase=1;
            }else{
                whichStrategy=true;
                Phase=10;
                
            }
           /* if(enemyState[1]>0.0f&&Time<timeToFirstSwitch&&infoMirror!=-1){
                if(doveSono==-1&&timeToSwitch<31){
                
                }
            }else{
                
            }*/
        }
    }
    if(Phase>0){
        if(whichStrategy){
            if(Phase==10&&doveSono==-1&&timeToSwitch<31){
                DEBUG(("%d control made",(rand()%18)+1));
                memcpy(whereToGo,nearEnergy,sizeof(float)*3);
            }
            if(Phase==10&&myEnergy>4 && myState[1]<0.1f)
            Phase=11;
            if(Phase==11){
                DEBUG(("%d control made",(rand()%18)+1));
                memcpy(whereToGo,ITEM_LOC[1-Color],sizeof(float)*3);
                //velocity = (((distanceToNextPoint*0.68f)/0.1377148161220323f)/100.0f);
            }
            if(Phase==11&&game.hasItem(1-Color)==0&&game.hasItem(Color)==-1)
                Phase=12;
        
            if(Phase==12){
                DEBUG(("%d control made",(rand()%18)+1));
                memcpy(whereToGo,ITEM_LOC[Color],sizeof(float)*3);
            }
   
            if(doveSono==-1&&timeToSwitch<31&&myState[1]<0.14&&Phase==10)
                game.useMirror();
        
        }else{
            if(Phase==1){
                if(game.hasItem(6)==-1){
                    memcpy(whereToGo, ITEM_LOC[6],sizeof(float)*3);
                }else if(game.hasItem(5-Color)==-1){
                    DEBUG(("%d control made",(rand()%18)+1));
                    memcpy(whereToGo, ITEM_LOC[5-Color],sizeof(float)*3);
                }else{
                    float StrategyPic[]={Color==0?-0.408f:0.408, 0.604f, -0.596f};
                    DEBUG(("%d control made",(rand()%18)+1));
                    memcpy(whereToGo, StrategyPic, sizeof(float)*3);
                }
            } 
        }
    }
    
     distanceToNextPoint = getDistance(myState,whereToGo);
    
    if(myEnergy<=0.55f){
        if(whichStrategy){
          //  if(Phase==12)
              //  velocity=distanceToNextPoint/timeToSwitch-30;
          //  else
              //  velocity=0.07f;
        }else{
            velocity=0.00f;
        }
      // velocity=0.000f;
    }else{
        if(Phase==0){
                velocity = (((distanceToNextPoint*0.68f)/((0.0977148161220323f*timeToFirstSwitch)/48))/100.0f);
        }else{
            if(whichStrategy){
                if(Phase==10){
                   velocity = (((distanceToNextPoint*0.68f)/0.0577148161220323f)/100.0f);  
                }else if(Phase==11){
                    velocity = (((distanceToNextPoint*0.68f)/0.1377148161220323f)/100.0f);
                }else{
                    velocity = (((distanceToNextPoint*0.68f)/0.08277148161220323f)/100.0f);
                }
            }else{
                if(Phase==1){
                    velocity = (((distanceToNextPoint*0.68f)/ 0.0663f)/100.0f); 
                }else{
                    velocity = (((distanceToNextPoint*0.68f)/ 0.0677148161220323f)/100.0f); 
                }
            } 
        }
        
    }
    
   /* if(myEnergy<=1.00f){
       velocity=0.1f;
       //velocity=0.000f;
    }else{
        if(Phase==0){
        num=((0.0977148161220323f*timeToFirstSwitch)/48);
        velocity = (((getDistance(myState,whereToGo)*0.68f)/num )/100.0f);
        }else if(Phase==1){
           velocity = (((getDistance(myState,whereToGo)*0.68f)/ 0.0663f)/100.0f);//+0.02 = +4 sec  0.04624f
        }else{
            velocity = (((getDistance(myState,whereToGo)*0.68f)/ 0.0677148161220323f)/100.0f); 
        }
    }*/
    
    //if(enemyState[0])
    
    
    
    if(game.getScore() == game.getOtherScore() && Time>175){
        game.takePic();
    }
    
    // DEBUG(("DISTANZA: %f",getDistance(myState,whereToGo)));
    // DEBUG(("Velocity: %f",velocity));
    // DEBUG(("Phase: %d",Phase));
    // DEBUG(("Velocity: %f",velocity));
    // DEBUG(("%f %f %f",whereToGo[0], whereToGo[1], whereToGo[2]));
    // DEBUG(("PhaseMAMT: %d",Phase));
    // DEBUG((" mia rotazione %f",myRotationVelocity));
    setConstantVelocityTo(whereToGo,velocity); //one command for movement
    api.setAttitudeTarget(whereAttitude); //one command for attitude
   

}
//End page main
