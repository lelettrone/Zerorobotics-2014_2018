//Begin page Funzioni
bool occupedShadow()
    {
        state_vector otherState;
        
        api.getOtherZRState(otherState);
        
        if ((otherState[0]>0)&&(otherState[1]>-0.35)&&(otherState[1]<0.35))
        {
            DEBUG(("Enemy is in shadow zone"));
            return true;
        }
        else
        {
            return false;
        }
    }

// =====================

bool nearShadowPOI()        //IF the interested POI is near shadow return true
    {
        if(itemPos[0][0]>-0.09)
        {   return true;    }
        else
        {   return false;   }
    }

// =======================================================================================

int underVelocity(float min)
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

// =========================================================================================

void goToUpload(float radiusUpload)
{
    float Point[3];
    
    Point[0] = 0;
    Point[2] = 0;
    
    if (isBlue)
        {
            Point[1] = radiusUpload;
        }
    else
        {
            Point[1] = -radiusUpload;
        }

    posStopRotation(Point, radiusUpload, 1, 0.2, isBlue);  //0.2
    game.takePic(idPOI);
    game.uploadPic();
}

//===============================================

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

// =============================

void goToShadow(float Radius)
{
    float shadowPoint[3];
    shadowPoint[0]=Radius; shadowPoint[1]=0; shadowPoint[2]=0;
    posStopRotation(shadowPoint, Radius, 1, 0.2, isBlue);  //0.2
    game.takePic(idPOI);
    game.uploadPic();
}

// ==================================

int chooseRightPOI()
{
    float temp0[3];
    float temp1[3];
    
    game.getPOILoc(temp0,0);
    game.getPOILoc(temp1,1);
    
    switch(isBlue)
    {
        case TRUE:
        
        if(temp0[1]>temp1[1])
            {   return 0;   }
        else
            {   return 1;   }
        break;
        
        case FALSE:
        
        if(temp0[1]>temp1[1])
            {   return 1;   }
        else
            {   return 0;   }
        break;

    }
    
}

int getNearestPOI()
{
    float temp1[3];
    float temp2[3];
    state_vector state;
    
    api.getMySphState(state);
    game.getPOILoc(temp1,0);
    game.getPOILoc(temp2,1);
    
    if (getDistance(temp1,state) < getDistance(temp2,state)){
        return 0;
    }
    else {
        return 1;
    }
}
    
//-------------------------------------------------------------

bool ruNearMe(float you[13], float me[13], float r)
{
    float vPath[13];
    
    mathVecSubtract(vPath,you,me,3);
    
    return (mathVecMagnitude(vPath,3) <= r ) ? true : false;

}
 
//-------------------------------------------------------------
 
float getDistance(float a[13], float b[13])
{
	float lol = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return lol;
}


// ===============================================================


bool alignPOI()
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
	}
	for(int i=0;i<3;i++){
	    myPos[i]=myState[i];
	}

	if(
	    (simili(myAttitude[0],vectorBetween[0],0.04))
	&&  (simili(myAttitude[1],vectorBetween[1],0.04))
	&&  (simili(myPos[0],itemSetPos[Step][0],0.05))
	&&  (simili(myPos[1],itemSetPos[Step][1],0.05))
	)
	{
	    aligned=true;
    }
	return aligned;
}

// ======================================

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

// =======================================

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

// ===================================================


void goOnCircle(float radius)
{
    float v[3], w[3];
    mathVecMult (v,myState,1,3);
    mathVecNormalize(v,3);
    w[0]=radius*(v[0]*cosalpha-v[1]*sinalpha);
    w[1]=radius*(v[0]*sinalpha+v[1]*cosalpha);
    w[2]=0;
    mathVecSubtract(v,w,myState,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
 
void goOnCircleReverse(float radius)
{
    float v[3], w[3];
    mathVecMult (v,myState,1,3);
    mathVecNormalize(v,3);
    w[0]=radius*(v[1]*sinalpha+v[0]*cosalpha);
    w[1]=radius*(v[1]*cosalpha-v[0]*sinalpha);
    w[2]=0;
    mathVecSubtract(v,w,myState,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
 
void mathVecMult (float *v, float *a, float k, int dim)
{
    int i;
    for (i=0; i<dim; i++)
    {
        v[i]=a[i]*k;
    }
}

// ===============================================
//End page Funzioni
//Begin page Movimenti
bool posStopRotation(float Stop[], float r, int Step, float dist, bool isBlue)
{
    game.takePic(idPOI);
    if (ruNearMe(Stop,myState,dist))
    {
        api.setPositionTarget(Stop);
        return true;
    }
    
    else
    {
        if (isBlue)     //If our Sphere is the blue one
        {
            switch(Step)
            {
                case 0:  //Going to POI's outer zone and interest zones(x<0)
                goOnCircle(r);
                break;
                
                case 1:  //Going to POI's inner and shadow zones
                goOnCircleReverse(r);
                break;
            }
        }
            
        else        //If our sphere is the red one
        {
            switch(Step)
            {
                case 0:  //Going to POI's outer zone and interest zones(x<0)
                goOnCircleReverse(r);
                break;
                
                case 1:  //Going to POI's inner and shadow zones
                goOnCircle(r);
                break;
            }
        }
    }
}

// -------------------------------------------------------------------------------------------

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

//-------------------------------------------------------------

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
//End page Movimenti
//Begin page main
#define DEBUG_OFF // Uncomment this to see debug information
#define OUT 0.47
#define IN 0.36
float Radius[2];
state_vector myState;        // Our state
bool alarm, arrivedFlare, isBlue, firstLap;
float itemPos[2][3];           // Position of the POI
float itemSetPos[2][3];     // Position in front of the POI 
float vectorBetween[3];
int Time, Phase, Step, idPOI, solarFlare, number, countTo3;
float Omega, alpha, cosalpha, sinalpha;


/////////////////////////////////////////////
/////////////////////////////////////////////
////                                     ////
////    Last release: 30/09/2014 04.53   ////
////    Game: CoronaSPHERES 2D           ////
////    Team: ZRighi                     ////
////                                     ////
/////////////////////////////////////////////
/////////////////////////////////////////////


//-------------------------------------------------------------

void init()
{
	Phase = 0;
	Omega=0.14;
    alpha=0.14;
    alarm=false; arrivedFlare=false; number=0; countTo3=0;
    Step = 0; // inner or outer
    idPOI = 0;
    cosalpha=cosf(alpha); 
    sinalpha=sinf(alpha);
    firstLap=true;
    
    if ((getQuadrante()==1)||(getQuadrante()==2))
    {   isBlue=TRUE;DEBUG(("I'm blue !"));  }
    else
    {   isBlue=FALSE;DEBUG(("I'm red !"));  }
    
}

//-------------------------------------------------------------

void loop()
{
    Time = api.getTime();
    if(Time%60==0)
    {
        Phase=0;
    }
    
    if ((Time>231)&&(Step>0))
    {
        alarm=true;
        goToShadow(0.55);
    }
    
    api.getMySphState(myState);
    solarFlare=game.getNextFlare();
    
    if(firstLap)
        {
            Radius[0]=OUT;
            Radius[1]=IN;
        }
    else
        {
            Radius[0]=IN;
            Radius[1]=OUT;
        }
    
    
    if((Phase==4)||((solarFlare!=-1)&&((solarFlare<20&&nearShadowPOI())||((solarFlare<30)&&(!nearShadowPOI())))))
 // IF we have already done 3 Phases or solarFlare is coming. Near POI, we have 20sec, the others 30sec.
    {
        firstLap=false;
       
        if(occupedShadow())
        {
            if ((Phase==4)||((solarFlare!=-1)&&((solarFlare<15&&nearShadowPOI())||((solarFlare<25)&&(!nearShadowPOI())))))    
            {
                game.takePic(idPOI);
                alarm=true;
                goToUpload(0.55);
                
                if (solarFlare==1)
                    {   
                        game.turnOff(); 
                    }
            }
        }
        else
        {
            game.takePic(idPOI);
            alarm=true;
            goToShadow(0.38);
        }
    }

    if (flareGone(number, countTo3))
    {
        arrivedFlare=true;
    }
    
    if (arrivedFlare)
    {
        Phase=0;
        Step=0;
        alarm=false;
        arrivedFlare=false;
        game.turnOn();
    }
    
    //#ifdef DEBUG_ON
    if((solarFlare%10==0)&&(solarFlare!=0))
	        {
	            DEBUG(("Next solar flare in %d s \n",solarFlare)); 
	        }
	//#endif
    
    #ifdef DEBUG_ON
    DEBUG(("[%d] Photo: %d\n",Time, game.getMemoryFilled()));
    #endif
    
    if (!alarm)
    {
    switch(Phase)
    {
        // Start;
        case 0:
            
            idPOI= chooseRightPOI();//Quadrante 2 if it's blue sphere, Else 3
            
            #ifdef DEBUG_ON
            DEBUG(("idPOI: %d\n", idPOI));
            #endif
            // Get positions for taking photos
            game.getPOILoc(itemPos[0],idPOI);
	            // Outer Circle
                itemSetPos[0][0] = -1 * ( Radius[0] / sqrt(1 + pow(itemPos[0][1] / itemPos[0][0], 2)));
                itemSetPos[0][1] = itemSetPos[0][0] * (itemPos[0][1] / itemPos[0][0]);
                itemSetPos[0][2] = 0;
            
            game.getPOILoc(itemPos[1],idPOI);
                // Inner Circle
                itemSetPos[1][0] = -1 * ( Radius[1] / sqrt(1 + pow(itemPos[1][1] / itemPos[1][0], 2)));
                itemSetPos[1][1] = itemSetPos[1][0] * (itemPos[1][1] / itemPos[1][0]);
                itemSetPos[1][2] = 0;
            
            #ifdef DEBUG_ON
            DEBUG(("outerPOS[%f][%f][%f]\n", itemSetPos[0][0], itemSetPos[0][1], itemSetPos[0][2]));
            DEBUG(("innerPOS[%f][%f][%f]\n", itemSetPos[1][0], itemSetPos[1][1], itemSetPos[1][2]));
            #endif
            
            
            Phase = 1;
        break;
        
        case 1:
            if (firstLap)  //IF it's the first lap, we can stop late !
            {
                if (ruNearMe(itemSetPos[Step],myState,0.15)) //0.15
                {   Phase = 2;  }
                else
                {   posStopRotation(itemSetPos[Step], Radius[Step], Step, 0.15, isBlue);    } //0.08
            }
            else            // ELSE we need to stop earlier !
            {
                if (ruNearMe(itemSetPos[Step],myState,0.2)) //0.2
                {   Phase = 2;  }
                else
                {   posStopRotation(itemSetPos[Step], Radius[Step], Step, 0.2, isBlue); } //0.2
            }
            
            break;
        
        case 2:
            //stop Sphere
            
            api.setPositionTarget(itemSetPos[Step]);
            if(underVelocity(0.01))
            {   Phase=3;  }
            
            break;
            
        case 3:
            //calculate attitude for facing the POI
            
            api.setPositionTarget(itemSetPos[Step]);
            mathVecSubtract(vectorBetween,itemPos[Step],myState,3);
            mathVecNormalize(vectorBetween,3);
            api.setAttitudeTarget(vectorBetween);
            
            if(alignPOI())
            {
                #ifdef DEBUG_ON
    	        DEBUG(("I'm taking a picture!!\n"));
    	        #endif
    	        game.takePic(idPOI);
    	        #ifdef DEBUG_ON
    	        DEBUG(("Taken? %d", game.getMemoryFilled()));
    	        #endif
    	        Step++;
    	        Phase=1;
    	        if (Step==2)
    	            {
    	                Phase=4;
    	            }
            }
        break;
        
	    
    } // end switch
    
    }
}
//End page main
