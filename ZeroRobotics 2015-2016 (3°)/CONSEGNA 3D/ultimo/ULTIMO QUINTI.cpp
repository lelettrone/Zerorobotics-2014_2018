//Begin page ItemFunctions
/*void takeMirror()//Cesa-Riccio
{
    memcpy(whereToGo, ITEM_LOC[Color+7],sizeof(float)*3);
    if(game.hasItem(Color+7)==0)
    {
       Phase=2;
    }
}*/

//End page ItemFunctions
//Begin page MovementFunctions
bool isFacingEarth(){ //Cesa-Riccio
    float Earth[3] = {myState[0],myState[1],myState[2]+0.05f};  //get vector from we and earth
    float realEarth[3];                                         //and get our attitude vector,
    mathVecSubtract(realEarth, Earth, myState, 3);              //if angle lower of tolerance (cosf(0.25f)
    mathVecNormalize(realEarth,3);                              //return true,else return false
    //DEBUG(("INNER:,%f",angleBeetween(realEarth,myState+6)));
    //return angleBetween(realEarth,myState+6)<acosf(MAX_FACING_ANGLE);
    float myAngle = acosf(mathVecInner(realEarth, myState+6, 3) / (mathVecMagnitude(realEarth, 3) * mathVecMagnitude(myState+6, 3)));
    DEBUG(("MyAngle: %f",myAngle));
    return  myAngle<acosf(MAX_FACING_ANGLE);
}

bool CanAnotherPic(){//Cesa-Riccio
    if(game.isFacingOther() //verify if we sphere is oriented to enemy
            && ((myZone == -1) ? myEnergy>=1.55 : myEnergy>=1.00) //da vedere
          //  && myEnergy>=1.55f //1.0 should be ok //2.0 //1.55 //1.00f
            && enemyZone!=-1 //we can take pic only when enemy zone is different from DARK game.posInArea(enemyState)!
            && getDistance(myState,enemyState)>=0.5f) //we can take pic only when enemy distance is greate or ideam 0.5
            { 
                return true;
            }
    return false;
}
//End page MovementFunctions
//Begin page basicFunction
float angleBetween(float *vectorA, float *vectorB){//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}

float getDistance(float a[3], float b[3]){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}

/*void setConstantVelocityTo(float point[], float k){
		float velocityVector[3];
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		for(int i=0;i<3;i++)
		    velocityVector[i]*=k;
		api.setVelocityTarget(velocityVector);
}*/
//End page basicFunction
//Begin page main
float myState[12],enemyState[12],whereToGo[3],whereAttitude[3];
float distanceToWhereGo,myEnergy,velocity,enemyVel,valuePic;
short timeToSwitch, Phase, Color,Time,HowMuchSwitch,enemyZone,myZone;
bool attitudeToEarth,useCazzimma;
void init(){
    Phase = HowMuchSwitch = Time = attitudeToEarth = 0;
    valuePic = 1;
    DEBUG(("Greetings from ZRighi Team!"));
   // api.setPosGains(0.25f, 0.0f, 2.5f);
   // api.setAttGains(0.8f, 0.0f, 8.0f);
}
void loop(){
    api.getMyZRState(myState); //get info of my sphere
    api.getOtherZRState(enemyState); //get info of enemy sphere
    if(Time<1)
        Color = ((myState[0] < 0) ? 0 : 1); //use this for index in matrix,
         //0 == Red | 1 == Blue              //then, go to correct point, indifferently
                                            //if we are red or blue
    Time++;
    //myRotationVelocity = mathVecMagnitude(myState+9,3);
    timeToSwitch = game.getLightSwitchTime();
    myEnergy = game.getEnergy(); 
    enemyVel = mathVecMagnitude(enemyState+3,3);
    ((timeToSwitch <2) ? HowMuchSwitch+=1 : HowMuchSwitch);
    //DEBUG(("quanti switch: %d",HowMuchSwitch));
    enemyZone = game.posInArea(enemyState);
    myZone = game.posInArea(myState);
    
   /* if(myZone!=-1 && enemyZone==-1 && HowMuchSwitch>0 && HowMuchSwitch<3 && timeToSwitch>24 ){
        if(game.getNumMirrorsHeld()>0){
            game.useMirror();
        }
    }*/
    
    if((myZone==-1 && enemyZone==-1 && timeToSwitch<2 )||( myZone!=-1 && enemyZone==-1 && HowMuchSwitch>0 && HowMuchSwitch<3 && timeToSwitch>24)||(myZone==-1 && enemyZone!=-1 && HowMuchSwitch>0 && HowMuchSwitch<3 && timeToSwitch<2)){
        if(game.getNumMirrorsHeld()>0){
            game.useMirror();
        }
    }
       /* if(myZone!=-1 && enemyZone==-1 && HowMuchSwitch>0 && HowMuchSwitch<3 && timeToSwitch>24 ){
        if(game.getNumMirrorsHeld()>0){
            game.useMirror();
        }
    }*/
    
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
    }/*else if(game.getMemoryFilled()==1 && myZone==-1 || game.getMemoryFilled()>1){
			attitudeToEarth=true;
    }*/
    
    if(game.getMemoryFilled()>=1 && Time>169 || game.getMemoryFilled()==1 && myZone==-1 && myEnergy<1.5f || game.getMemoryFilled()>1){
        attitudeToEarth=true;
    }
    
    if(!attitudeToEarth){
        mathVecSubtract(whereAttitude, enemyState, myState, 3);
        mathVecNormalize(whereAttitude,3);
    }else{
        
        //(1+cos(0.25),1,0)
        
        float earth[3]={myState[0]+(sinf(0.25)*0.05),myState[1],myState[2]+0.05f};
        mathVecSubtract(whereAttitude,earth,myState,3);
        mathVecNormalize(whereAttitude,3);
        if(isFacingEarth()){
            if((myEnergy-1.0f)>=0.55f || (myEnergy>=1.00 && Time>170)){
                if(mathVecMagnitude(myState+9,3)<=0.05f)//0.05{
                    game.uploadPics();
                    attitudeToEarth=false; 
            }
        }
    }
    
    api.setAttitudeTarget(whereAttitude);
    //DEBUG(("Phase: %d",Phase));
    if(Phase==0){
        memcpy(whereToGo, ITEM_LOC[Color+7],sizeof(float)*3);
        if(myZone!=-1 && enemyZone!=-1 && HowMuchSwitch<1){
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
        /*memcpy(whereToGo, ITEM_LOC[5-Color],sizeof(float)*3);
        if(game.hasItem(5-Color)!=-1){
            Phase=3;
        }*/
        if(myZone!=enemyZone){
           // if(myZone==-1 && game.getMemoryFilled()>1 || Time>165 && myZone==-1 && game.getMemoryFilled()>0){
           //     maggiaMovr=0;
                
           // }else{
              //  maggiaMovr=1;
                if(game.hasItem(Color+7)==-1){
                    memcpy(whereToGo, ITEM_LOC[Color+7],sizeof(float)*3);
                }else if(game.hasItem(5-Color)==-1){
                    memcpy(whereToGo, ITEM_LOC[5-Color],sizeof(float)*3);
                }else if(game.hasItem(4+Color)==-1){
                    memcpy(whereToGo, ITEM_LOC[4+Color],sizeof(float)*3);
                }
                /*else if(game.hasItem(3)==-1){
                    memcpy(whereToGo, ITEM_LOC[3],sizeof(float)*3);
                }*/
                else{
                    whereToGo[2]=-0.596f;//-0.500 ? verificare
                }
          //  }
            
            
        }else{
            if(game.hasItem(5-Color)==-1){
                memcpy(whereToGo, ITEM_LOC[5-Color],sizeof(float)*3);
            }else if(game.hasItem(4+Color)==-1 && game.hasItem(5-Color)!=0){
                memcpy(whereToGo, ITEM_LOC[4+Color],sizeof(float)*3);
            }else{
                //Phase=3;
                whereToGo[2]=-0.596f;   
            }   
        }
         
    }
    
    distanceToWhereGo = getDistance(myState,whereToGo);
    if(myEnergy<=1.00f){ //1.00 //0.65f  //0.70f
        velocity=0.000f;
    }else{
        if(myZone==-1 && myEnergy<=2.3 || Time>165 && game.getMemoryFilled()>0){
            velocity=0.000f;
        }else{
            if(Phase==0){
                //    DEBUG(("velocity per il mirror"));
                velocity = (((distanceToWhereGo*0.68f)/ 0.05459f)/100.0f); // 0.0577148161220323f
            }else if(Phase==1){
                if(!useCazzimma){
                    //    DEBUG(("non uso la cazzimma"));
                    velocity = (((distanceToWhereGo*0.68f)/ 0.0663f)/100.0f);  //0.0663
                }else{
                    if(getDistance(enemyState,whereToGo)>0.23f && enemyVel<=0.02){
                        //   Phase = 7;
                        //     DEBUG(("enemy fermo, vado sciolto sul punto"));
                        velocity = (((distanceToWhereGo*0.68f)/ 0.0677148161220323f)/100.0f);
                    }else{
                        //    if(distanceToWhereGo>=0.23){
                        if(distanceToWhereGo>=0.18f){
                            //    DEBUG(("SSEGUO IL NEMICO"));
                            //       DEBUG(("gli do velocity di cazzimma"));
                            velocity = ((distanceToWhereGo)/(getDistance(enemyState, whereToGo)/enemyVel+(2.6f)));
                            // velocity = (((distanceToWhereGo)/(getDistance(enemyState, whereToGo))/enemyVel+3.0f));//3.0
                        }else{
                            // DEBUG(("arrivo sul punto, diminuisco la velocity"));
                            velocity = (((distanceToWhereGo*0.68f)/ 0.09459f)/100.0f);
                        }  
                    }
                }
            }else{
                velocity = (((distanceToWhereGo*0.68f)/ 0.0677148161220323f)/100.0f);
            }
        }
    }
    
  //  if(maggiaMovr){
      /*  float velocityVector[3];
		mathVecSubtract(velocityVector, whereToGo, myState, 3);
		mathVecNormalize(velocityVector, 3);
		for(int i=0;i<3;i++)
		     velocityVector[i]*=velocity;
		api.setVelocityTarget( velocityVector);*/
  //  }
  		
}
//End page main
