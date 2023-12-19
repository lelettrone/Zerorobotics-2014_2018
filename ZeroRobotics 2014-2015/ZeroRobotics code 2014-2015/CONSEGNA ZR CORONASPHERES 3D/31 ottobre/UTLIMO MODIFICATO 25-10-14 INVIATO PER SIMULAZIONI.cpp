//Begin page Funzioni

// MADE BY RUSSO
// LAST CHANGES BY RUSSO 15.02 - 25/10

float getRTNR()    //Remaining Time to the Next Respawn.
{
    float remTime;
    float arrRefresh[5]={0,60,120,180,240};
    int pointer=1;
    float ourTime;
    
    ourTime=api.getTime();
    
    for(int i=0;i<4;i++)
    {
        if(ourTime>arrRefresh[i])
        {
            pointer=i+1;
        }
    }
    
    remTime=arrRefresh[pointer]-ourTime;
    DEBUG(("Tempo Rimasto %f \n",remTime));
    return remTime;
}

// ================================================================================================

//  MADE BY RUSSO-BARUFFINI
//  LAST CHANGES BY RUSSO 20.27 - 24/10

void avoidCollision()
{
    state_vector state;
    float zero[3]={0,0,0};
    float destination[3];
    bool risk;
    risk= false;
    
    api.getMySphState(state);
    
    if(getRadius()<0.335)
    {
        //mathVecSubtract(vectorToZero,zero,state,3);
        mathVecNormalize(state,3);
        risk=true;
        for (int i=0;i<3;i++)
        {
            destination[i]=state[i]*0.80;
        }
    }
    
    if((fabs(state[0])>0.60)||(fabs(state[1])>0.77)||(fabs(state[2])>0.60))
    {
        DEBUG(("TROPPO VICINI AL BORDO!\n"));
        risk=true;
        for (int i=0;i<3;i++)
        {
            destination[i]=zero[i];
        }
    }

   
    if(risk)
    {
        alarm=true;
        goToPoint(destination,1);
    }
    else
    {
        alarm=false;
    }

}

// ================================================================================================

//  MADE BY RUSSO
// LAST CHANGES BY RUSSO 21.17 - 22/20

bool alignPOI()
{
    bool aligned=false; 		//variabile di stato
    float myAtt[3];
    float rightAttitude[3];
    float myRadius;
    float zero[3]={0,0,0};
    float angleMePoi;
    float angleAttitude;
    float anglePosMax, angleAttMax, radiusMin, radiusMax;
    int i;
    
    anglePosMax=0;
    angleAttMax=0.05;
    radiusMin=0;
    radiusMax=0;
    
    api.getMyZRState(myState);
    myRadius=getRadius();
    
    for(i=0;i<3;i++)
    {   myAtt[i]=myState[i+6]; }
    
    //Angle Between Sphere-Centre-POI
    angleMePoi=getAngleBetween(zero, myState, zero, itemPos[0]);
    
    //Angle between ourAttitude-POI-idealAttitude
    mathVecSubtract(rightAttitude,itemPos[0],myState,3);
    angleAttitude=getAngleBetweenVector(myAtt,rightAttitude);
    
    if(Radius[Step]<0.41)
    {
        //We have to take a picture from INNER ZONE
        radiusMin=0.31;
        radiusMax=0.42;
        anglePosMax=0.8;
    }
    else
    {
        //We have to take a picture from OUTER ZONE
        radiusMin=0.42;
        radiusMax=0.53;
        anglePosMax=0.4;
    }

    
	if
	(
	    (getVelocity()<0.01)
	&&  (myRadius>radiusMin)
	&&  (myRadius<radiusMax)
	&&  (angleMePoi<anglePosMax)    
	&&  (angleAttitude<angleAttMax)
	)
	{
	    aligned=true;
    }
	return aligned;
}


// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 21.17 - 22/20
float getAngleBetween(float pointA[3], float secondPointA[3], float pointB[3], float secondPointB[3])
{
    float vectorA[3];
    float vectorB[3];

    mathVecSubtract(vectorA,pointA,secondPointA,3);     // Vector from A --> secondA 
    mathVecSubtract(vectorB,pointB,secondPointB,3);     // Vector from B --> SecondB 

    return getAngleBetweenVector(vectorA, vectorB);
}

// ================================================================================================

//  MADE BY RUSSO 22-10
//  LAST CHANGES BY RUSSO 21.17 - 22/20
float getAngleBetweenVector(float vectorA[3], float vectorB[3])
{
    float modulA, modulB, modulProd, angleBetween;

    modulA=sqrtf(vectorA[0]*vectorA[0]+vectorA[1]*vectorA[1]+vectorA[2]*vectorA[2]);   
    modulB=sqrtf(vectorB[0]*vectorB[0]+vectorB[1]*vectorB[1]+vectorB[2]*vectorB[2]);   
    
    modulProd=vectorA[0]*vectorB[0]+vectorA[1]*vectorB[1]+vectorA[2]*vectorB[2];  
    
    angleBetween=acosf(modulProd/(modulA*modulB));
    
    return angleBetween;
}


// ================================================================================================

//  MADE BY RUSSO 22-10
//  LAST CHANGES BY RUSSO 21.17 - 22/20
void goodDestination(float Radius, float destMoment[3])
{
    int i;
    
    while(!(isGoodVector(destMoment, 0.33)))
    {
        Radius=Radius+0.02;
        
        for(i=0;i<3;i++)
            {
                destMoment[i]= 1 * itemPos[0][i];
            }
        mathVecNormalize(destMoment,3);
            
        for(i=0;i<3;i++)
            {
                destMoment[i]= destMoment[i] * Radius;
            }
    }
}

// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 11.17 - 22/10

void goodDestination2()
{
    if(!isGoodVector(momentDest,0.34))
    
            {
            for(i=0;i<3;i++)
            momentDest[i]=itemSetPos[1][i];
            }
            
            if(!isGoodVector(momentDest,0.34))
            {
            for(i=0;i<3;i++)
            momentDest[i]=itemSetPos[2][i];
            }
            
            if(!isGoodVector(momentDest,0.34))
            {
            momentDest[0]=-0.5;
            momentDest[1]=0;
            momentDest[2]=0;
            }
}

// ================================================================================================

//  MADE BY RICCIO-CESARANO 22-10
//  LAST CHANGES BY RICCIO-CESARANO 21.17 - 22/20
bool isGoodVector(float point[3],float distMin)
{
   float stato[12];
   api.getMyZRState(stato);
   bool is_good=false;
   float middlePoint[3];
   float _point1[3];
   float _point2[3];
   float coordinate_asteroide[3]={0,0,0};
   float distanza1,distanza2,distanza3,distanza4,distanza5;
   getMiddleCoordinates(point,stato,middlePoint);
   getMiddleCoordinates(middlePoint,stato,_point1);
   getMiddleCoordinates(point,middlePoint,_point2);
   distanza1=getDistance(_point1,coordinate_asteroide);
   distanza2=getDistance(middlePoint,coordinate_asteroide);
   distanza3=getDistance(_point2,coordinate_asteroide);
   distanza4=getDistance(point,coordinate_asteroide);
   distanza5=getDistance(stato,coordinate_asteroide);
   if(distanza1>=distMin && distanza2>=distMin && distanza3>=distMin && distanza4>=distMin && distanza5>=distMin)     
    {
      is_good=true;
    }
   else
   {
      is_good=false;
   }
   return is_good;
}

// ================================================================================================

//  MADE BY RICCIO-CESARANO
//  LAST CHANGES BY RICCIO-CESARANO 21.17 - 22/20
void getMiddleCoordinates(float point1[3], float point2[3], float *middlePoint){
    middlePoint[0]=((point1[0]+point2[0])/2);
    middlePoint[1]=((point1[1]+point2[1])/2);
    middlePoint[2]=((point1[2]+point2[2])/2);
}

// ================================================================================================

//  MADE BY RICCIO-CESARANO
//  LAST CHANGES BY RUSSO 21.17 - 22/20
int getQuadrante()
{
    float stato[12];
    int n_quadrante=0;
    api.getMySphState(stato);
    if(stato[0]>0 && stato[1]>0)
    { n_quadrante=1; }
    if(stato[0]<0 && stato[1]>0)
    { n_quadrante=2; }
    if(stato[0]<0 && stato[1]<0)
    { n_quadrante=3; }
    if(stato[0]>0 && stato[1]<0)
    { n_quadrante=4; }
    return n_quadrante;
}

// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 21.17 - 22/20
float getVelocity()
{
    float total;
    
    total=sqrtf(myState[3]*myState[3]+myState[4]*myState[4]+myState[5]*myState[5]);
    
    return total;
}

// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 21.17 - 22/20
void firmtMommo()
{
    int i;
    
    if (!takenPos)
    {
        takenPos=true;
        for (i=0;i<3;i++)
        {
            stopPosition[i]=myState[i]-0.001;
        }
    }

    api.setPositionTarget(stopPosition);
}

// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 01.57 - 23/10
int getNearestPOI(int *outVector, int numberState)
{
    float posPoi[3][3];
    float dist[3][2];
    int otherList[3], otherMin;
    int min,med,max,temp;
    state_vector state;
    
    switch(numberState)
    {
        case 0:
        api.getMySphState(state);
        break;
        
        case 1:
        api.getOtherSphState(state);
        break;
    }
    numberState++;
    
    
    for(int i=0;i<3;i++)
        {
            game.getPOILoc(posPoi[i],i);
            mathVecNormalize(posPoi[i],3);
            for (int j=0;j<3;j++)
            {
                posPoi[i][j]= posPoi[i][j] * Radius[0];
            }
        }
    
    for(int i=0;i<3;i++)
    {
        dist[i][0]=getDistance(posPoi[i],state);
        dist[i][1]=i;
    }
    
    increasingOrd(dist,3,2);
    
    if(numberState==2)
    {
        return dist[0][1];
    }
    
    otherMin=getNearestPOI(otherList, 1);   // 1 gets Other Sphere State
    
    min=dist[0][1];
    med=dist[1][1];
    max=dist[2][1];
    
    if(min==otherMin)
    {
        temp=max;
        max=min;
        min=med;
        med=temp;
    }
    if(med==otherMin)
    {
        temp=max;
        max=med;
        med=temp;
    }
    
    
    outVector[0]=min;
    outVector[1]=med;
    outVector[2]=max;

    return 0;
}

//  &&&&&&&&&&&&&&&&&

void increasingOrd(float vector[3][2], int numOrd, int secDim)
{
    float temp;
    for(int i=0;i<numOrd-1;i++)
    {      
          for(int j=i+1;j<numOrd;j++)
          {
                if(vector[i][0] > vector[j][0]) 
                {
                    for(int k=0;k<secDim;k++)
                    {
                        temp=vector[i][k];
                        vector[i][k]=vector[j][k];
                        vector[j][k]=temp;
                    }
                }     
          }
    }
}

// ================================================================================================

float getDistance(float a[13], float b[13])
{
	float dist = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return dist;
}

// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 21.17 - 22/20
bool flareGone(int &number, int &timer)
{
    int nextFlare;
    nextFlare=game.getNextFlare();
    switch(number)
    {
        case 0:
        // "Case" become 1 if solarFlare is coming.
        if(nextFlare!=-1)
        {
            number=1;
        }
        return false;
        break;
    
        case 1:
        
        // Timer is increased if nextFlare is "-1" again
        // Solar flare lasts 3 seconds, so if timer=3, the functions returns TRUE
        
        if(nextFlare==-1)
        {
            timer++;
        }
        
        if(timer==1)
        {
            number=0;
            timer=0;
            return true;
        }
        else
        {   return false;   }
        
        break;
        
    }       //Close switch
    
}

// ================================================================================================

//  MADE BY RUSSO
//  LAST CHANGES BY RUSSO 21.17 - 22/20
int chooseRightPOI()
{
    float temp0[3];
    float temp1[3];
    float temp2[3];
    
    game.getPOILoc(temp0,0);
    game.getPOILoc(temp1,1);
    game.getPOILoc(temp2,2);
    
    switch(isBlue)
    {
    case true:
        if((temp0[1]>temp1[1])&&(temp0[1]>temp2[1]))
            {   return 0;   }
        else
        {
            if ((temp1[1]>temp0[1])&&(temp1[1]>temp2[1]))
            {   return 1;   }
            else
            {   return 2;   }
        }
    break;
    
    case false:
        
        if((temp0[1]<temp1[1])&&(temp0[1]<temp2[1]))
            {   return 0;   }
        else
        {
            if ((temp1[1]<temp0[1])&&(temp1[1]<temp2[1]))
            {   return 1;   }
            else
            {   return 2;   }
        }
    
    break;
    } //end switch
}

// ================================================================================================

//  MADE BY SCOTTO
//  LAST CHANGES BY SCOTTO 21.17 - 22/20
float getRadius()
{
	float state[12];
	float radius;
	api.getMyZRState(state);
	radius=sqrt((state[0]*state[0])+(state[1]*state[1])+(state[2]*state[2]));
	return radius;
}

//End page funzioni

//End page Funzioni
//Begin page Movimenti
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

void goToPoint(float *target,float maximumVelocity) {
	float distance[3];
	float myState[12];
    api.getMyZRState(myState);
    mathVecSubtract(distance,target,myState,3);

	float stoppingDistance = maximumVelocity*maximumVelocity*35.0F;		// 35 is a value obtained empirically.

	if(mathVecMagnitude(distance,3) < stoppingDistance+0.01F) {       	// If we need to start curbing?
		api.setPositionTarget(target);  								// We use the setPositionTarget(); function, much more stable
	}											                        // than a function manually-created.
	
	else {
		float forza[3] = {0.0F,0.0F,0.0F};      						// The default value of the force is 0. Like this if the condition
																		// of the below IF the SPHERES move thanks to inertia.

		if(mathVecMagnitude(myState+3,3)<maximumVelocity-0.015F) {      // I evaluate whether speeding up or move thanks to inertia.
			float max =  (distance[0]); 							// This auxiliary variable will contain the maximum component of the
			for (int i = 1; i<3; i++) {									// SPHERES distance between the target position.
				if (fabsf(distance[i])>max) {
					max = fabsf(distance[i]);
				}
			}

			float vettoreRapporti[3];       							// This is an auxiliary vector, useful for the computing of the force vector.
			for (int i = 0; i<3; i++) {
				vettoreRapporti[i] = distance[i]/max;
			}
			for (int i = 0; i<3; i++) {             					// This FOR cicle applies the formula: f[3] = displacement[3] * K.
				forza[i] = vettoreRapporti[i]*0.38F;    				// 0.38 is 0.55 divided by the third root of 3, or rather the maximum
																		// force each propeller can exert.
			}
		}
		api.setForces(forza);   										// If the satellite must accelerate we'll exert the computed force,
	}																	// else the force will be null.
}
//End page Movimenti
//Begin page main
//Begin page main
/* ================================================================================================

*  LAST CHANGES BY RUSSO 21.43 - 24/10
    -   Added the fuction "avoidCollision" to avoid collisions vs Satellite
            and don't go out from the game's area.
    -   Changed time to call the function "firmtMommo()" from 4 to 5
    -   Removed the 3 possibilites to take a picture when we have taken a bad picture.
    
*   LAST CHANGES BY RUSSO 16.45 - 25/10
    -   Added the function to calculate the "RTNR", remaining time to the next points' respawn.
    -   Control the RTNR and the time that we need to upload picture before taking it
    After these changes, we take a picture if RTNR < 17 OR solarFlare < 10,
    therefore we wait only if after the solar Flare we haven't time enough to take the other
    pictures of the same POI, otherwise we take the picture.
*/

#define upl 0.60    //radius upload zone
#define out 0.44    //radius outer zone
#define inn 0.39    //radius inner zone

float maxVelocity;  //It depends on wich Step we are in
float Radius[3];
float itemPos[3][3];             // Position of the POI
float itemSetPos[3][3];         // Position in front of the POI 
float itemSetAtt[3];
float momentDest[3];            //Moment destination to avoid damage
float stopPosition[3];
float vectorBetween[3];
float Time;
int Phase, Step, solarFlare, nPhoto, tent, waitTime;
int numberFlare, TimerFlare; // Used in flareGone
int countPOI,i, j, idPOI, listaPOI[3];
bool alarm, defence, attack, refresh, isBlue, takenPos;

float distStop;

state_vector myState;

void init()
{
    waitTime=0;
    Phase = 0;
    Step = 0; // inner or outer
    countPOI=0;
    numberFlare=0; TimerFlare=0;
    Radius[0]=inn;
    Radius[1]=out;
    Radius[2]=upl;
    alarm=false;
    refresh=false;
    defence=false;
    attack=true;
    takenPos=false;
    tent=0;
    
    if ((getQuadrante()==1)||(getQuadrante()==2))
    {   isBlue=TRUE;}
    else
    {   isBlue=FALSE;}
    
}

void loop()
{
    api.getMySphState(myState);
    solarFlare=game.getNextFlare();
    Time=api.getTime();

    if (Time==2)
    {
        DEBUG(("Greetings from the school Righi, Naples (Italy) :)"));
    }

    //  Stop Sphere if solar flare is coming
    if((solarFlare!=-1)&&(solarFlare<5)&&(Phase!=3))
    {
        alarm=true;
        firmtMommo();
    }
    else
    {
        takenPos=false;
    }
    if((solarFlare!=-1)&&(solarFlare<2))
        {   
            game.uploadPic();
            game.turnOff();
        }
    
    
    //Go to Upload the picture if we have 1 picture at least and solar flare is coming OR time si ending
    if((nPhoto!=0)&&(((solarFlare!=-1)&&(solarFlare<=10))||(Time>=230)))
    {
        game.takePic(idPOI);
        Phase=4;
        Step=2;
    }
    
    if (flareGone(numberFlare, TimerFlare))
    {
        nPhoto=0;
        alarm=false;
        game.turnOn();
    }
    
    //Refresh nearest POI each 60s
    if (    ((int)Time%60 == 0)
        &&  ((int)Time!=0)
       )
    {
        refresh=true;
        Phase=3;
        Step=2;
    }
    
    if(refresh&&(nPhoto==0))
    {
        refresh=false;
        Phase=0;
        Step=0;
        countPOI=0;
    }
    

    if(!alarm)
    {
    switch(Phase)
    {
        // Start;
        case 0:
            
            if(Time<60)
            {
                idPOI=chooseRightPOI();
            }
            else
            {
            getNearestPOI(listaPOI, 0); //  0 gets our SPH State.
            idPOI= listaPOI[countPOI];
            }
            

            for(i=0;i<3;i++)
            {
                game.getPOILoc(itemPos[i],idPOI);
                for(j=0;j<3;j++)
                {
                    itemSetPos[i][j]= 1* itemPos[i][j];
                }
                
                itemSetAtt[i] = itemPos[0][i];
                
                mathVecNormalize(itemSetPos[i],3);      
            }
            
            
            
            mathVecNormalize(itemSetAtt,3);
            
            for(i=0;i<3;i++)
            {
                for(j=0;j<3;j++)
                {
                    itemSetPos[i][j] = itemSetPos[i][j] * Radius[i];
                }
            }
            
            
            // FINE TEST
            
            Phase = 1;
        break;
        
        case 1:
        DEBUG(("DIST: %f \n",getDistance(myState,itemSetPos[Step]) ));
            if (getDistance(myState,itemSetPos[Step])>0.1)
                {
                    game.takePic(idPOI);
                }
                
            nPhoto=game.getMemoryFilled();
            
            for(i=0;i<3;i++)
            {
            momentDest[i]=itemSetPos[Step][i];
            }
            
            if(Step==0)
            {
            //goodDestination(Radius[Step],momentDest);
            
            goodDestination2();
            
            maxVelocity=0.05;
            }
            else
            {
                maxVelocity=0.02;   //0.01
            }
            distStop=120*getVelocity()*getVelocity();   
            
            setConstantVelocityTo(momentDest,maxVelocity);
            
            if (getDistance(myState,itemSetPos[Step])<distStop)
                {   Phase=2;    }
            
            mathVecSubtract(vectorBetween,itemPos[Step],itemSetPos[Step],3);
            mathVecNormalize(vectorBetween,3);
            api.setAttitudeTarget(vectorBetween);
            
            if(nPhoto==2)
            {   
                Phase=3;
                Step=2;
            }
            break;
            
        case 2:
            
            nPhoto=game.getMemoryFilled();
            
            api.setPositionTarget(itemSetPos[Step]);
            
            if(getVelocity()>0.01)
            {
            mathVecSubtract(vectorBetween,itemPos[Step],itemSetPos[Step],3);
            mathVecNormalize(vectorBetween,3);
            }
            else
            {
            mathVecSubtract(vectorBetween,itemPos[Step],myState,3);
            mathVecNormalize(vectorBetween,3);
            }
            
            api.setAttitudeTarget(vectorBetween);
            
            
            if(((api.getTime()-waitTime)>3)&&alignPOI()&&((!((solarFlare!=-1)&&(solarFlare<10)))||(getRTNR()<17)))     
            {
                    waitTime=api.getTime();
                    game.takePic(idPOI);
    	        
    	        if ((nPhoto!=game.getMemoryFilled()))
    	        {
    	        DEBUG(("FOTO FATTA BENE!"));
    	        }
    	        else
    	        {
    	            DEBUG(("FOTO FATTA MALE!\n"));
    	        }
    	        
    	        Step++;
    	        Phase=1;
    	        nPhoto=game.getMemoryFilled();
    	        
    	        if (Step==2)
    	            {
    	                Phase=3;
    	            }
            }
            
            if(nPhoto==2)
            {   
                Phase=3;
                Step=2;
            }
        break;
        
        case 3:
        
        if(getRadius()>0.54)
        {
            firmtMommo();
        }
        else
        {
            if (getDistance(myState,itemSetPos[Step])>0.09)
                {
                    setConstantVelocityTo(itemSetPos[Step],0.06);
                }
            else
            {
                api.setPositionTarget(itemSetPos[Step]);
            }
        }
            
            game.uploadPic();
            nPhoto=game.getMemoryFilled();
            if(nPhoto==0)
            {
                Phase=0;
                Step=0;
                countPOI++;
            }
        
        break;
        
        case 4:
        
        if(getRadius()>0.54)
        {
            firmtMommo();
        }
        else
        {
            if (getDistance(myState,itemSetPos[Step])>0.1)
                {
                    setConstantVelocityTo(itemSetPos[Step],0.5);
                }
            else
            {
                api.setPositionTarget(itemSetPos[Step]);
            }
            game.uploadPic();
            nPhoto=game.getMemoryFilled();
        }
        
            if(nPhoto==0)
            {
                Phase=0;
                Step=0;
                countPOI++;
            }
            
        break;
	   
    } // end switch

    }
    
    if(Time>=239)
    game.takePic(idPOI);
}

//End page main
