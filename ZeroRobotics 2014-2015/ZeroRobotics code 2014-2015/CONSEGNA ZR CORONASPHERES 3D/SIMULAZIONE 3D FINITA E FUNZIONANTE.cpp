//Begin page Funzioni
bool alignPOI()
{
    bool aligned=false; 		//variabile di stato
    api.getMyZRState(myState);

	DEBUG(("DISTANZA: %f",(getDistance(myState,itemSetPos[Step])) ));
	DEBUG(("Differenza attitudine: %f",(getDiffAttitude(myState,vectorBetween)) ));
	
	if
	(
	    (getDistance(myState,itemSetPos[Step])<0.025)
	&&  (getDiffAttitude(myState,vectorBetween)>-0.03
	&&  (getDiffAttitude(myState,vectorBetween)<0.03))
	)
	{
	    aligned=true;
    }
	return aligned;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void goodDestination(float Radius, float destMoment[3])
{
    int i;
    
    while(!(isGoodVector(destMoment, 0.37)))
    {
        Radius=Radius+0.02;
        DEBUG(("AUMENTO IL RADIUS: %f", Radius));
        
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
    
    DEBUG(("DIREZIONE BUONA!"));
}


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

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

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void getMiddleCoordinates(float point1[3], float point2[3], float *middlePoint){
    middlePoint[0]=((point1[0]+point2[0])/2);
    middlePoint[1]=((point1[1]+point2[1])/2);
    middlePoint[2]=((point1[2]+point2[2])/2);
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

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

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

bool underVelocity(float min)
{
    float total;
    int i;
    total = 0;
    for (i=0;i<3;i++)
    {   total=total+sqrtf(myState[i+3]*myState[i+3]);   }
    
    if((total<min)&&(total>-min))
    {   return true;    }
    else
    {   return false;   }
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

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

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

int getPosition(float *array,int size,float element){
    int position=0;
    for(int i=0;i<size;i++){
        if(array[i]==element){
            position=i;
            break;
        }
    }
    return position;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void getNearestPOI(int *outVector)
{
    float POI[3][3],dist[3];
    int min,max,medium,nPOI;
    state_vector state;
    api.getMySphState(state);
    for(int i=0;i<3;i++){
        game.getPOILoc(POI[i],i);
        dist[i]=getDistance(POI[i],state);
        DEBUG(("i: %d dist: %f\n",i,dist[i]));
    }
    min=indexofSmallestElement(dist,3);
    max=indexofMaxElement(dist,3);
    nPOI=game.numActivePOIs();
    
    DEBUG(("NUMERO DI POI: %d,nPOI"));
    
    if(min==0 && max==1){medium=2;}
        if(min==1 && max==2){medium=0;}
        if(min==2 && max==0){medium=1;}
    
    /*
    switch (nPOI)
    {
        case 2:
        medium=max;
        break;
        
        case 3:
        
        if(min==0 && max==1){medium=2;}
        if(min==1 && max==2){medium=0;}
        if(min==2 && max==0){medium=1;}
        break;
    }*/
    outVector[0]=min;
    outVector[1]=medium;
    outVector[2]=max;
    DEBUG(("min: %d medium: %d max: %d \n",min,medium,max));
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

int indexofSmallestElement(float array[], int size){
  int index = 0 ;
  double n = array[0] ;
  for (int i = 1; i < size; ++i)
  {
    if (array[i] < n)
    {
        n = array[i] ;
        index = i ;
    }
  }
 return index;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

int indexofMaxElement(float array[], int size){
    double max=array[0];
    int max_pos=0;

    int i;
    for (i=1; i<size; i++)
    {
        if (max<array[i])
        {
            max=array[i];
            max_pos=i;
        }
    }

    return max_pos;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float getDistance(float a[13], float b[13])
{
	float lol = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return lol;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float getDiffAttitude(float a[13], float b[3])
{
	float lol = (
	            (   (sqrt(a[6]*a[6])) - (sqrt(b[0]*b[0])) ) +
	            (   (sqrt(a[7]*a[7])) - (sqrt(b[1]*b[1])) ) +
	            (   (sqrt(a[8]*a[8])) - (sqrt(b[2]*b[2])) )
	            );

	return lol;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

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

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

bool ruNearMe(float you[13], float me[13], float r)
{
    float vPath[13];
    
    mathVecSubtract(vPath,you,me,3);
    
    return (mathVecMagnitude(vPath,3) <= r ) ? true : false;

}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

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

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

bool simili(float a, float b,float max)
{
    float risultato;
    risultato=a-b;
    if((risultato<max)&&(risultato>-max)){
        return true;
    }else{
        return false;
    }
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

#define upl 0.60    //60  
#define out 0.49   //48    //48
#define inn 0.38    //35    //36

float maxVelocity;
float Radius[3];
float itemPos[3][3];           // Position of the POI
float itemSetPos[3][3];     // Position in front of the POI 
float itemSetAtt[3];
float momentDest[3];    //Moment destination to avoid damage
float stopPosition[3];
float vectorBetween[3], rightAttitude[3];
int Phase, Step, Time, solarFlare, nPhoto, tent;
int numberFlare, TimerFlare; // Used in flareGone
int countPOI,i, j, idPOI, listaPOI[3];
bool alarm, defence, attack, refresh, isBlue, takenPos;

float distStop;

state_vector myState;

void init()
{
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
    {   isBlue=TRUE;DEBUG(("I'm blue !"));  }
    else
    {   isBlue=FALSE;DEBUG(("I'm red !"));  }
    
    
}

void loop()
{
    DEBUG(("COUNT POI= %d", countPOI));
    api.getMySphState(myState);
    solarFlare=game.getNextFlare();
    Time=api.getTime();

    if((solarFlare!=-1)&&(solarFlare<4)&&(Phase!=3))
    {
        alarm=true;
        firmtMommo();
    }
    else
    {
        takenPos=false;
    }
    if((solarFlare!=-1)&&(solarFlare<1))
        {   
            game.uploadPic();
            game.turnOff();
        }
    
    
    if((nPhoto!=0)&&(((solarFlare!=-1)&&(solarFlare<=10))||(Time>=230)))
    {
        DEBUG(("SCAPPA !"));
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
    
    if(Time%60 == 0)
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
            
            //if((Time<60)||(countPOI==0))
            if(Time<60)
            {
                idPOI=chooseRightPOI();
            }
            else
            {
            getNearestPOI(listaPOI);
            idPOI= listaPOI[countPOI];
            }
            
            DEBUG(("Il POI di ora Ã¨: %d",idPOI));

            for(i=0;i<3;i++)
            {
                game.getPOILoc(itemPos[i],idPOI);
                /*if(itemPos[i][2]>0)
                    {   Radius[1]=0.475;   }
                else    {   Radius[1]=0.48;    }*/
                
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
            
            nPhoto=game.getMemoryFilled();
            game.takePic(idPOI);
            
            for(i=0;i<3;i++)
            momentDest[i]=itemSetPos[Step][i];
            
            if(Step==0)
            {
            //goodDestination(Radius[Step],momentDest);
            
            if(!isGoodVector(momentDest,0.32))
            {
            for(i=0;i<3;i++)
            momentDest[i]=itemSetPos[1][i];
            }
            
            if(!isGoodVector(momentDest,0.32))
            {
            for(i=0;i<3;i++)
            momentDest[i]=itemSetPos[2][i];
            }
            
            if(!isGoodVector(momentDest,0.32))
            {
            momentDest[0]=-0.5;
            momentDest[1]=0;
            momentDest[2]=0;
            }
            
            maxVelocity=0.05;
            distStop=0.25;
            }
            else
            {
                //distStop=100*((sqrtf(myState[3]*myState[3])+sqrtf(myState[4]*myState[4])+sqrtf(myState[5]*myState[5]))*(sqrtf(myState[3]*myState[3])+sqrtf(myState[4]*myState[4])+sqrtf(myState[5]*myState[5])));   
                maxVelocity=0.009;
            }
            
            setConstantVelocityTo(momentDest,maxVelocity); // 0.05

            
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
            
            mathVecSubtract(vectorBetween,itemPos[Step],itemSetPos[Step],3);
            mathVecNormalize(vectorBetween,3);
            api.setAttitudeTarget(vectorBetween);
            
            if(underVelocity(0.0099)&&alignPOI()&&(!((solarFlare!=-1)&&(solarFlare<10))))
            {
    	        game.takePic(idPOI);
    	        if ((nPhoto!=game.getMemoryFilled())||(nPhoto==2)||(tent==2))
    	        {
    	        tent=0;
    	        Step++;
    	        Phase=1;
    	        nPhoto=game.getMemoryFilled();
    	        }
    	        else
    	        {
    	            tent++;
    	        }
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

            if (getDistance(myState,itemSetPos[Step])>0.09)
                {
                    setConstantVelocityTo(itemSetPos[Step],0.06);
                }
            else
            {
                api.setPositionTarget(itemSetPos[Step]);
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

            if (getDistance(myState,itemSetPos[Step])>0.1)
                {
                    setConstantVelocityTo(itemSetPos[Step],0.08);
                }
            else
            {
                api.setPositionTarget(itemSetPos[Step]);
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
	   
    } // end switch
    
    }
}

//End page main
