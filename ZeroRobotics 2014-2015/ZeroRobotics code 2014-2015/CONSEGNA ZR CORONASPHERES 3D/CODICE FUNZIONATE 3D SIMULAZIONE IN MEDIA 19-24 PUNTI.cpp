//Begin page Functions
bool alignPOI(float maxAtt,float maxPos)
{
	/*se la torsione del nostro satellite
	non e' uguale alla torsione di setAttitude
	restituisce false altrimenti true*/

    bool aligned=false; 		//variabile di stato
    float myAttitude[3];        //torsione del nostro satellite
    float myPos[3];
    api.getMyZRState(myState);
    
	for(int i=0;i<3;i++){
	    myAttitude[i]=myState[i+6];
	    myPos[i]=myState[i];
	}
	
	DEBUG(("DISTANZA: %f",(getDistance(myState,itemSetPos[Step])) ));
	if(
	    (simili(myAttitude[0],vectorBetween[0],maxAtt))
	&&  (simili(myAttitude[1],vectorBetween[1],maxAtt))
	&&  (simili(myAttitude[2],vectorBetween[2],maxAtt))
	
	/*&&  (simili(myPos[0],itemSetPos[Step][0],0.05))
	&&  (simili(myPos[1],itemSetPos[Step][1],0.05))
	&&  (simili(myPos[2],itemSetPos[Step][2],0.05))*/
	
	&&  (getDistance(myState,itemSetPos[Step])<maxPos)
	)
	{
	    aligned=true;
    }
	return aligned;
}
/*bool alignPOI(float attitudeMax, float distMax)
{
	se la torsione del nostro satellite
	non e' uguale alla torsione di setAttitude
	restituisce false altrimenti true

    bool aligned=false; 		//variabile di stato
    float myAttitude[3];        //torsione del nostro satellite
    float myPos[3];
    api.getMyZRState(myState);
    
	for(int i=0;i<3;i++)
	{
	    myAttitude[i]=myState[i+6];
	    myPos[i]=myState[i];
	}

	if(
        (simili(myAttitude[0],vectorBetween[0],attitudeMax))
	&&  (simili(myAttitude[1],vectorBetween[1],attitudeMax))
	&&  (simili(myAttitude[2],vectorBetween[2],attitudeMax))
	&&  (simili(myPos[0],itemSetPos[Step][0],distMax))
	&&  (simili(myPos[1],itemSetPos[Step][1],distMax))
	&&  (simili(myPos[2],itemSetPos[Step][2],distMax))
	  )
	{
	    aligned=true;
    }
	return aligned;
}*/

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void attaccaAvversario()
{
    float myState[12],otherState[12];
    api.getMyZRState(myState);
    api.getOtherZRState(otherState);
    if((game.getScore()-game.getOtherScore())>=2.12 && getDistance(myState,otherState)<0.3 && isGoodVector(otherState,0.37))
    {
        api.setPositionTarget(otherState);
        attack=true;
    }
    else
    {
        attack=false;
    }
}

void goodDestination(float Radius, float destMoment[3])
{
    int i;
    
    while(!(isGoodVector(destMoment, 0.37)))
    {
        Radius=Radius+0.01;
        
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

bool underVelocityAtt(float min)
{
    float total;
    int i;
    total = 0;
    for (i=0;i<3;i++)
    {   total=total+myState[i+9];   }
    
    if((total<min)&&(total>-min))
    {   return true;    }
    else
    {   return false;   }
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
    {   total=total+myState[i+3];   }
    
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
            momentDest[i]=myState[i]-0.001;
        }
    }

    api.setPositionTarget(momentDest);
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

void getNearestPOI(int *outVector){
    float POI[3][3],dist[3];
    int min,max,medium;
    state_vector state;
    api.getMySphState(state);
    for(int i=0;i<3;i++){
        game.getPOILoc(POI[i],i);
        dist[i]=getDistance(POI[i],state);
        DEBUG(("i: %d dist: %f\n",i,dist[i]));
    }
    min=indexofSmallestElement(dist,3);
    max=indexofMaxElement(dist,3);
    if(min==0 && max==1){medium=2;}
    if(min==1 && max==2){medium=0;}
    if(min==2 && max==0){medium=1;}
    outVector[0]=min;
    outVector[1]=medium;
    outVector[2]=max;
    DEBUG(("min: %d medium: %d max: %d \n",min,medium,max));
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

//End page Functions
//Begin page Mouvements
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
//End page Mouvements
//Begin page main
//Begin page main


#define upl 0.60    //60  
#define out 0.48   //48    //48
#define inn 0.36    //35    //36

float Radius[3];
float itemPos[3][3];           // Position of the POI
float itemSetPos[3][3];     // Position in front of the POI 
float itemSetAtt[3];
float momentDest[3];    //Moment destination to avoid damage
float vectorBetween[3];
int Phase, Step, Time, solarFlare, nPhoto, tent;
int numberFlare, TimerFlare; // Used in flareGone
int countPOI,i, j, idPOI, listaPOI[3];
bool alarm, defence, attack, refresh, firstLap, isBlue, takenPos;

float lastPOI[3];

float posMax, attMax;

state_vector myState;

void init()
{
    Phase = 0;
    Step = 0; // inner or outer
    countPOI=0;
    numberFlare=0; TimerFlare=0;
    Radius[2]=upl;
    alarm=false;
    refresh=false;
    defence=false;
    attack=true;
    firstLap=true;
    takenPos=false;
    tent=0;
    
    posMax=0.03;
    attMax=0.05;

    if ((getQuadrante()==1)||(getQuadrante()==2))
    {   isBlue=TRUE;DEBUG(("I'm blue !"));  }
    else
    {   isBlue=FALSE;DEBUG(("I'm red !"));  }
    
    
}

void loop()
{
    api.getMySphState(myState);
    solarFlare=game.getNextFlare();
    Time=api.getTime();
    DEBUG(("idPOI:%d",idPOI));
    if(firstLap)
    {
        Radius[0]=inn;
        Radius[1]=out;
    }
    else
    {
        Radius[0]=out;
        Radius[1]=inn;
    }
    
    
    
    
    
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
        Phase=3;
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
        firstLap=false;
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
            
            if(countPOI==0)
            {
                idPOI=chooseRightPOI();
            }
            else
            {
                getNearestPOI(listaPOI);
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
            
            nPhoto=game.getMemoryFilled();
            game.takePic(idPOI);
            
            momentDest[0]=itemSetPos[Step][0];
            momentDest[1]=itemSetPos[Step][1];
            momentDest[2]=itemSetPos[Step][2];
            
            //goodDestination(Radius[Step],momentDest);
            
            setConstantVelocityTo(momentDest,0.05); // 0.05
            
            if (getDistance(myState,itemSetPos[Step])<0.25)//25
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
            //calculate attitude for facing the POI
            nPhoto=game.getMemoryFilled();
            
            api.setPositionTarget(itemSetPos[Step]);
            
            
            if(underVelocity(0.03))
            {
            mathVecSubtract(vectorBetween,itemPos[Step],itemSetPos[Step],3);
            mathVecNormalize(vectorBetween,3);
            api.setAttitudeTarget(vectorBetween);
            }
            
            if(underVelocity(0.01))
            {
                
            if(alignPOI(attMax,posMax)&&(!((solarFlare!=-1)&&(solarFlare<10))))
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
            }
            
            if(nPhoto==2)
            {   
                Phase=3;
                Step=2;
            }
        break;
        
        case 3:
            
            DEBUG(("SONO IN FASE 3"));
            
            firstLap=false;
            
            if (getDistance(myState,itemSetPos[Step])>0.09)
                {
                    setConstantVelocityTo(itemSetPos[Step],0.05);
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
                //countPOI++;
            }
        break;
	   
    } // end switch
    
    }
}

//End page main
