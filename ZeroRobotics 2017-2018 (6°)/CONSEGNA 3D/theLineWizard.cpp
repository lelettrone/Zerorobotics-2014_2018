//Begin page DNS
#define CHOOSESAMPLE 0
#define CHOOSESTRATEGY 1
#define STARTDRILL 3
#define STOPDRILL 4
#define DROP 10
#define PULA 11

//#define RESETROTATION 21







//End page DNS
//Begin page Schema
/*typedef struct s_Sample{
   int indexInMemory;
   float concentration;
   int squarePos[2];
  // bool thereIsConcentration;
  bool isDropped;
}Sample;*/

//End page Schema
//Begin page allFailedRotation
/*
        float rightPoint[3];
        memcpy(rightPoint, myState+6, sizeof(float) * 3);
        rightPoint[2] = 0.0f;
        
        float angleY = getAngleBetween(myState+6, rightPoint);
        DEBUG(("angle: %f", angleY));
        
        float rad = (angleY * 0.6f)/ PI;
        float w = mathVecMagnitude(myState+9, 3);
        DEBUG(("rad vel : %f ", rad));
        int where = (myState[8] > 0.0f)? -1 : 1;
        
        
        
        float att[3] = {0.0, rad * where, 0.0f};
        api.setAttRateTarget(att);
        if(angleY < 0.1){ 
            memcpy(restoreRot, myState+6, sizeof(float) * 3);
            _Phase = 21;
        }else{
           float where[3] = {myState[6],myState[7],0};
          // rightPoint[1] = 0.0f;
          // api.setAttitudeTarget(rightPoint);
        }
        
        
         float state13[13];
        api.getMySphState( state13 );
    
        float myQuattAtt[4];
        memcpy(state13+6, myQuattAtt, sizeof(float)*4);
    
        float finalAtt[3] =  {0, -1, 0};
        float refVecc[3] =   {-1, 0, 0};//questo l'avevo fatto io (mauro)
        float baseQuat[4] =  {0,0,0,1} ;
        float quatOut[4];               
    
        api.attVec2Quat( refVecc, finalAtt, baseQuat, quatOut );

        quatRotation(myQuattAtt, quatOut, 0.3f, quatOut);

        api.setQuatTarget(quatOut);

       //  api.setAttitudeTarget(whereAttitude);
      
      float res[3];
      float where[3] = {1,0,0};
      slerp(res, myState+6, where, 0.6f);
      //api.setAttRateTarget(res);
      // mathVecNormalize(res, 3);
      api.setAttitudeTarget(res);
      
      
      
      float where[3] = {0,1,0};
      mathVecAdd(where, where, myState, 3);
      mathVecNormalize(where, 3);
      api.setAttitudeTarget(where);
      
*/
//End page allFailedRotation
//Begin page collision
bool willCollide(float x[3], float y[3], float itemPos[3], float r){//p1, p2, sfera, raggio
    //willCollide(enemyState, enemyY, zeroPos, 0.26f);
    
    //x = myState
    //y = whereToGo
    //itemPos
    //Il cross it's the area of the parallelogram of vec1 and vec2
    //area = h * a
    //area = mathVecMagnitude(cross, 3)
    //a = mathVecMagnitude(xToY, 3)
    // h = area / a
    
    float xToY[3];
    mathVecSubtract(xToY, y, x, 3);
    float xToYMagnitude = mathVecMagnitude(xToY, 3);
    float xToItem[3];
    mathVecSubtract(xToItem, itemPos, x, 3);
    float xToItemMagnitude = mathVecMagnitude(xToItem, 3);
        
    if (xToYMagnitude < xToItemMagnitude && xToItemMagnitude > r) //Se la dist da percorrere è minore di quella dall'item AND non sono in collisione
        return false;
    float cross[3];
    mathVecCross(cross, xToItem, xToY);
    return !( (mathVecMagnitude(cross, 3) / xToYMagnitude ) > r);
}
//End page collision
//Begin page function
/*void goTowards(float nextPosition[3],float myPosition[3],float myVelocity[3], float k){
    float velocity[3];
    mathVecSubtract(velocity,nextPosition,myPosition,3);
    //static float actualVel = -20.0f;
    float actualVel = mathVecMagnitude(myState+3, 3);
    //DEBUG(("vel Applied: %f", actualVel));
    mathVecSubtract(velocity,velocity,myVelocity,3);
    mathVecNormalize(velocity, 3);
    mathVecMult(velocity, velocity, actualVel * 1.5f, 3);
    //mathVecMult(velocity,velocity,k,3);
    api.setVelocityTarget(velocity);
}*/

/*float getW(float *u, float *v, float *w){ //W = V - U[myVel, meToZone, outW]
    mathVecSubtract(w, v, u, 3);
    float m = mathVecMagnitude(w, 3);
    mathVecNormalize(w, 3);
    return m;
}*/

int whichOne(int nSampleHeld){
    float tDrop = 16;
    float tRim = (176 - (_Time));

    float pNow = ((5*0.1+2)*nSampleHeld); //point goin to drop now
    float ToDrillToDrop5Time = 71;
    float ToDrillToDrop3Time = 43;


    float tToChangeSquare = 9;
    float pHypoteticFromDropWith5Drill = (((tRim-tDrop) / (ToDrillToDrop5Time)) * (5 * 0.1 + 2) ) + pNow;
    float pHypoteticFromDropWith3Drill = (((tRim-tDrop) / (ToDrillToDrop3Time)) * (5 * 0.1 + 2) ) + pNow;
    float pHypoteticContinuingDrill = (((tRim-(tToChangeSquare + tDrop + 10)) / (ToDrillToDrop5Time)) * (5 * 0.1 + 2)   ) + (pNow + ((5 - (nSampleHeld)) * (5*0.1+2)));
    
    float pRealHypotetic = (pHypoteticFromDropWith5Drill > pHypoteticFromDropWith3Drill) ? pHypoteticFromDropWith5Drill : pHypoteticFromDropWith3Drill;
    return (pRealHypotetic > pHypoteticContinuingDrill) ? 1 : 0;
}

float getVelCazz(float myState[12], float enemyState[12], float point[3], float phi){
    return (getDistance(myState, point))/(((getDistance(enemyState, point))/mathVecMagnitude(enemyState+3, 3))+phi);
}

bool willInZone(float *zrState, float s){ //Se entro s secondi sarà in zona
    float nextPos[3];
    for(int i=0;i<3;i++) nextPos[i] = zrState[i] + (zrState[i+3]*s);
    return mathVecMagnitude(nextPos, 3) <= 0.26;
}

bool linePlaneCollision(float rayDirection[3], float rayPoint[3], float collPoint[3]){
    float epsilon=1e-6;
    float planeNormal[3] = {0, 0, 1};
    float planePoint[3] =  {0, 0, 0.48f};
    float ndotu = mathVecInner(planeNormal, rayDirection, 3);
    if (fabs(ndotu) < epsilon){
        return false;
    }
    float w[3];
    mathVecSubtract(w, rayPoint, planePoint, 3);
    float invPlaneNormal[3];
    for(int i=0;i<3;i++) invPlaneNormal[i] = -planeNormal[i];
    float si = mathVecInner(invPlaneNormal, w, 3) / ndotu;
    for(int i=0;i<3;i++) collPoint[i] = w[i] + (si*rayDirection[i]) + planePoint[i];
    return true;
}


/*void quatRotation(float q1[4], float q2[4], float maxAngle, float outQuatt[4]){

	if( maxAngle < 0.001f ){
		// No rotation allowed. Prevent dividing by 0 later.
		memcpy(q1, outQuatt, sizeof(float)*4);
		return;
	}
    
    float dot = 0.0f;
    for(int i=0;i<4;i++) dot+= q1[i] * q2[i];
    
    
	float cosTheta = dot;

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if(cosTheta > 0.9999f){
	    memcpy(q2, outQuatt, sizeof(float)*4);
		return;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0){
	    for(int i=0;i<4;i++)q1[i] = q1[i]*-1.0f;
	    
	    cosTheta *= -1.0f;
	}

	float angle = acosf(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < maxAngle){
	    memcpy(q2, outQuatt, sizeof(float)*4);
		return;
	}

	float fT = maxAngle / angle;
	angle = maxAngle;

	float res[4];
	for(int i=0;i<4;i++) res[i] = (sinf((1.0f - fT) * angle) * q1[i] + sinf(fT * angle) * q2[i]) / sinf(angle);
	
	double n = sqrtf(res[0]*res[0] + res[1]*res[1] + res[2]*res[2] + res[3]*res[3]);
    res[0] /= n;
    res[1] /= n;
    res[2] /= n;
    res[3] /= n;
    
    
	//res = normalize(res);
	
	
	
	memcpy(res, outQuatt, sizeof(float)*4);

}*/


/*void slerp(float resVect[3],float start[3], float end[3], float percent){
     float dot = getDotBetween(start, end);
     //dot = clamp(dot, -1.0f, 1.0f);
     float theta = acosf(dot)*percent;
     float relativeVec[3];
     for(int i = 0; i<3; i++){ relativeVec[i] = end[i] - start[i] * dot; }
     mathVecNormalize(relativeVec, 3); // Orthonormal basis
     for(int i = 0; i<3; i++){ resVect[i] = (start[i] * cosf(theta)) + (relativeVec[i] * sinf(theta)); }

}*/








//End page function
//Begin page implementazioniDaFare

// 20/10/2017

/*
Enemy capisce che stiamo facendo pula

if(heIsGoingToDrill){//se sta andando a drillare
    if(enemyState[2]<myState[2]-costante){//se sta sopra di noi e sopra all'asse delle ascisse 
        blocca drill avversario
    }else{
        //Andiamo a drillare anche noi
        //_Phase=CHOOSESAMPLE;
    }
}

Enemy sta bloccando la zona

if(mathVecMagnitude(enemyState,3)<=0.26f){//se il nemico sta nella base station
    float posA[3],posB[3];
    memcpy(posA,myState,sizeof(float)*3);
    memcpy(posB,enemyState,sizeof(float)*3);
    if(getAngleBetween(posA,posB)<0.06){//se l'angolo tra noi e lui e minore di un k valore
        //ci sta bloccando
        _Phase=CHOOSESAMPLE;
        for(int i=0;i<sampleHeld;i++)game.dropSample(i);
    }
}

*/


// 21/10/2017

/*
    //se troviamo la zona dobbiamo fottercene dell'avversario - CIAO CIIIIIRO
    //drilare punti equidistanti dalla base station


*/


// 25/10/2017

/*
    // Migliorare movimento PULA
    
    float distancePula = getDistance(myState,enemyState);
    kVel=(((distancPula*0.0775f)/(0.09f))/0.56553f)/14.0f;
    memcpy(whereToGo,myState,sizeof(float)*3);
    mathVecNormalize(whereToGo,3);
    for(int i=0;i<3;i++){
        whereToGo[i]*=0.2;
    }
    
    // Condizione uscita da PULA
    float nextPos[3];memcpy(nextPos,enemyState,sizeof(float)*3);nextPos[2]=-1.0f;
    if(game.getScore()<=game.getOtherScore() || (enemyState[2]>0.31 && !getAngleBetween(enemyState,nextPos))){
        _Phase==CHOOSE_SAMPLE;
    }
*/

// 27/10/2017

/*//NEL DROP
if(zoneFound){
    memcpy(newSquare,actualSquare,sizeof(int)*2);
    if(drillInZone==0){
        newSquare[1]-=0.1f;
    }else if(drillInZone==1){
        newSquare[0]+=0.1f;
    }else if(drillInZone==2){
        newSquare[0]-=0.1f;
    }else{
        newSquare[1]+=0.1f;
    }
    drillInZone++;
}
// NEL CHOOSE SAMPLE

game.square2pos(((zoneFound)?newSquare:actualSquare),whereToGo);
*/





//End page implementazioniDaFare
//Begin page main
int _Phase, _Time, isBlue, actualSquare[3], enemySample, enemySampleBeforeDrop;
float whereToGo[3], enemyScore;
bool zoneFound, isntFirstDrop;

void init(){
    _Phase = _Time = enemyScore = isntFirstDrop = enemySample = zoneFound = CHOOSESAMPLE;
}

void loop(){
    float myState[13], enemyState[12], kVel = 0.05f; 
    float whereAttitude[3] = {0.0f, .0f, -1.0f};
    api.getMySphState(myState);
    api.getOtherZRState(enemyState);

    if(_Time++<1){
        isBlue = (myState[0] > 0.0f) ? 1 : -1; 
        actualSquare[0] = 3 * isBlue; // x:3 y:3
        actualSquare[1] = 3 * isBlue;
        actualSquare[2] = 0;
    }
    
    float enemyScoreDiff = fabs(game.getOtherScore() - enemyScore);
    float enemyVel = mathVecMagnitude(enemyState+3, 3);
    float enemyDistanceToCenter = mathVecMagnitude(enemyState, 3);
    bool heIsDrilling = ( enemyScoreDiff <= 3 ) && enemyVel < 0.01f && enemyState[2] > 0.31f;
    enemyScore = game.getOtherScore();
    //DEBUG(("diff: %f HeIsDrilling: %d",enemyScoreDiff,  heIsDrilling));
    //////////////////////////////////////////////////////////////////////
    if(enemyScoreDiff <=3 && enemyScoreDiff > 0) enemySample++;
    else if(enemyDistanceToCenter < 0.26f && enemySample > 0){ enemySampleBeforeDrop = enemySample; enemySample = 0; }
    //DEBUG(("enemySample: %d", enemySample));
     if(enemyScoreDiff > 3){
        float conc = ((enemyScoreDiff/enemySampleBeforeDrop)-2)/5; //
        //DEBUG(("Concentrazine Nemico: %f",conc));
        if(conc >= 0.3){
            //DEBUG(("sta latrina ha trovato la zona"));
        }
    }
    //////////////////////////////////////////////////////////////////////
    float enemToStationTime = (enemyDistanceToCenter - 0.24f) / enemyVel;
    bool heIsGoinToStation = (enemyState[2] > (enemyState[2] + enemyState[5])); //la prendiamo per buona
    bool isPulaSuitable = heIsGoinToStation && (enemToStationTime > 7 && enemToStationTime < 30);
    // bool isPulaSuitable = heIsGoinToStation && (mathVecMagnitude(enemyState,3)>0.4 && mathVecMagnitude(enemyState,3)<mathVecMagnitude(myState,3));
    //DEBUG(("time to station : %f heIsGoinToStation: %d isPulaSuitable: %d", enemToStationTime, heIsGoinToStation, isPulaSuitable));
    //////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////
    bool heIsGoinToDrill = (enemyState[2] < (enemyState[2] + enemyState[5])) && enemySample <=2 ;
    //DEBUG(("he is goin to drill: %d", heIsGoinToDrill));
    //////////////////////////////////////////////////////////////////////
    
    float temp[3];
    memcpy(temp, myState, sizeof(float)*3);
    mathVecNormalize(temp, 3);
    for(int i = 0; i<3; i++){ temp[i] *= 0.237f; }
    bool heIsDoingPula = getDistance(temp, enemyState) < 0.15f;
    
    if(_Phase == STARTDRILL){
        if(game.getDrillEnabled() == false){ game.startDrill();  }
        whereAttitude[2]=0.6f * isBlue;
        api.setAttRateTarget(whereAttitude);
        if(game.checkSample()){
            game.pickupSample();
        }
        _Phase = STOPDRILL;
    }
    int sampleHeld = game.getNumSamplesHeld();
    if(_Phase == STOPDRILL){
        if(sampleHeld >= 4 &&  game.getDrills(myState) == 2 || heIsDoingPula){ game.dropSample(4); sampleHeld--;}
        if((game.getDrillError() || game.isGeyserHere(myState) || sampleHeld == 5 || game.getDrills(myState) > 2 || (_Time > 140 && sampleHeld == 3))){ //condition to stop drill
            game.stopDrill();
            _Phase = CHOOSESTRATEGY;
        }else{ _Phase = STARTDRILL; }
    }
    
    if(_Phase == CHOOSESTRATEGY){
        if((_Time > 140 && sampleHeld == 3)||sampleHeld >= 4 /*|| (whichOne(sampleHeld) == 1 && game.isGeyserHere(myState))*/){ _Phase = DROP; /*actualSquare[0]-=isBlue; */}
        else { _Phase = ((isPulaSuitable || heIsDrilling) && (_Time > 145 && (game.getScore() - game.getOtherScore()) >=6)) ? PULA : CHOOSESAMPLE; }
    }
    
    if(_Phase == PULA){
        kVel = 0.1f;
        DEBUG(("PULA"));
        memcpy(whereToGo, enemyState, sizeof(float)*3);
        mathVecNormalize(whereToGo, 3);
        for(int i=0;i<3;i++) whereToGo[i] *= 0.145f; //POINT CALCULATION
        if(game.getScore() < game.getOtherScore() || (heIsGoinToDrill && getDistance(myState, enemyState) > 0.33f)){ //LA DISTANZA CI SERV
            _Phase = CHOOSESAMPLE;
        }
    }
    
    if(_Phase == CHOOSESAMPLE){
        game.square2pos(actualSquare, whereToGo); 
        whereToGo[2] = 0.35f;
        if(sampleHeld > 0){//we are continuing to drill, Ex. (3 DRILL GEYSER, 2 DRILL);
            kVel = 0.035f;
        }
        
        if(getDistance(myState, whereToGo) < 0.02f && mathVecMagnitude(myState+3,3) < 0.01f && mathVecMagnitude(myState+10, 3) < 0.04f){ // range 0.33-0.37
            _Phase = STARTDRILL;
            actualSquare[0]-=isBlue;
            if(actualSquare[0] == 0) actualSquare[0]-= isBlue; //0 is not part of DOMAIN
        }
    }
    
    if(_Phase == DROP){
        if(enemyDistanceToCenter < 0.24f && getDistance(myState, enemyState) < 0.3f){ kVel = 0.07f;  } // when he is nearest to out drop target we reduce the vel
        memcpy(whereToGo, myState, sizeof(float)*3);
        mathVecNormalize(whereToGo, 3);
        for(int i = 0; i<3; i++){ whereToGo[i] *= 0.237f; }
        if(game.atBaseStation()){
            /*if(!isntFirstDrop)*/ isntFirstDrop = true;
            for(int i = 0; i<sampleHeld; i++){
                float point = game.dropSample(i);
                if(point >= 0.3f){ zoneFound = true; }
            } 
            _Phase = CHOOSESTRATEGY;
        }
        if((getDistance(whereToGo, enemyState) < 0.15f)){_Phase = CHOOSESAMPLE;}
    }

    float distance = getDistance(myState, whereToGo);
	float velocity = (((distance*0.0775f)/(kVel))/0.56553f)/14.0f;
	float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);
    mathVecNormalize(velocityVector, 3);
    if(game.isGeyserHere(myState)){
        float meToZone[3];
        mathVecSubtract(meToZone, whereToGo, myState, 3);
        mathVecSubtract(velocityVector, meToZone, myState+3, 3);
        mathVecNormalize(velocityVector, 3);
	    api.setForces(velocityVector);
	}else{
	    for(int i=0;i<3;i++) velocityVector[i]*=velocity;
        api.setVelocityTarget(velocityVector);
	}

	{//rotation async block
        if(/*mathVecMagnitude(myState, 3) < 0.335f &&*/ _Phase == DROP){
            api.setAttitudeTarget(whereAttitude);
        }else{
            if(_Phase != STARTDRILL){
                float quatRightRot[4];
                memcpy(quatRightRot, myState+6, sizeof(float) * 4);
                quatRightRot[2] = quatRightRot[3] = 0.0f;
                api.setQuatTarget(quatRightRot);
            }
        }
	}
}
//End page main
//Begin page math
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3); //OPPPLAA
}

float getAngleBetween(float *vectorA, float *vectorB){//return the angle between 2 vector
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}

/*float clamp(float x, float a, float b){
    return x < a ? a : (x > b ? b : x);
}*/
//End page math
