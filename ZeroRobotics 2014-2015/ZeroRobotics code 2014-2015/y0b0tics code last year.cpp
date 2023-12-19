#include "ZRGame.h"
#include "ZR_API.h"
#include "ZRUser.hpp"

#include 

static ZeroRoboticsGame &game = ZeroRoboticsGame::instance();
static ZeroRoboticsAPI &api = ZeroRoboticsAPI::instance();

//Implement your simulation code in init() and loop()
class ZRUser01 : public ZRUser
{

//Begin page aInit
//Two POI changes :
//Line 8 use picsTaken[3] instead of picsTaken[2][3]
//setAttGains to (0.8, 0.0, 8.0)
struct
{
    float myState[12], otherState[12], POITarget[3], att[3], toEarth[3], zero[3], memPack[3], earth[3];
    int time, stageTime, flareTime, zone, flareCtr, numFlares, numPics, memSize, lastPics, targZone, targID, timeToPoi;
    int side;
    bool picsTaken[3], off, flareActive, shadowed, foundTarget;
    float radius;
} j;

void init()
{
    memset(&j, 0, sizeof(j));
    //j.memPack[0] = -0.5f;
    //j.maxAccel = .00375f;
    j.earth[0] = .64f;
    //j.other = -1;
    //j.pickedUpItem = true;
    api.setPosGains(0.25f, 0.0f, 2.5f);
    api.setAttGains(0.8f, 0.0f, 8.0f);
}
//End page aInit
//Begin page bUtilities
//Two POI Changes: none
void mathVecScale(float *save, float *v, float mag, bool norm)
{
    memcpy(save, v, 3*sizeof(float));
    if(norm) mathVecNormalize(save, 3);
    for(int i = 0; i < 3; ++i) save[i] *= mag;
}
bool notNan(float vec[3])
{
    for(int i = 0; i < 3; i++)
    {
        if(!(vec[i] < 1000))
        return false;
    }
    return true;
}
float mathVecDistance(float vec1[3], float vec2[3])
{
    float diffvec[3];
    mathVecSubtract(diffvec, vec1, vec2, 3);
    return mathVecMagnitude(diffvec, 3);
}
//End page bUtilities
//Begin page cMovement
//Two POI Changes: None
void getAround(float target[3], int recParam)
{
    
    //DEBUG(("\n Magnitude of Target = %f", mathVecMagnitude(target,3)));
    float centerVec[3];
    float radialVec[3];
    float targVec[3];
    float res[3];
    float scalar;
    mathVecSubtract(targVec, target, j.myState, 3);
    mathVecScale(centerVec, j.myState, -1.0f, false);
    mathVecNormalize(targVec,3);
    scalar = mathVecInner(targVec, centerVec, 3);
    mathVecScale(targVec, targVec, scalar, false);
    mathVecSubtract(radialVec, targVec, centerVec, 3);
    //DEBUG(("\n Magnitude of radialVec", mathVecMagnitude(radialVec,3)));
    if(scalar <= 0.0f || mathVecMagnitude(radialVec,3) > 0.33f)
    {
        memcpy(res,target,sizeof(float)*3);
    }
    else
    {
       mathVecScale(res, radialVec, 0.48f, true);
       //memcpy(res,radialVec,sizeof(float)*3); 
    }
    //DEBUG(("\n Magnitude of RES = %f", mathVecMagnitude(res,3)));
    if(!recParam && j.radius < 0.36f)
    {
        getAround(res, 1);
    }
    else
    {
        if(notNan(res))
        api.setPositionTarget(res);
    }
}


void goShadow()
{
    float shadowPoint[3] = {0.4f, 0.0f, 0.0f};
    game.takePic(j.targID);
    if(j.myState[0] > 0.05f) memcpy(j.att, j.toEarth, 3*sizeof(float));
    game.uploadPic();
    //memset(shadowPoint+1, 0, 2*sizeof(float));
    if(j.otherState[0]>0.3f && j.otherState[0]<0.64f)//if enemy is paralel with the shadow
    {

        if(isInShadow(j.otherState) && !j.shadowed)
        {
            shadowPoint[0] = 0.24f;
            for(int i = 1; i < 3; i++)
                shadowPoint[i] = 0.185f*(j.myState[i])/fabsf(j.myState[i]);
            if(j.otherState[0] > .45f)
                shadowPoint[0] = .33f;
            else
                shadowPoint[0] = j.otherState[0] + .15f;
        }
        else
        {   
            shadowPoint[0]=(j.otherState[0]+shadowPoint[0])/2.0f;
        }
    }
    getAround(shadowPoint, 0);
}
void goUpload()
{
    memcpy(j.att, j.toEarth, 3*sizeof(float));
    if(!seeEarth())
    {
        getAround(j.earth, 0);
    }
    else
    {
        api.setVelocityTarget(j.zero);
        if(faceEarth())
        {
            game.uploadPic();
        }
    }
}
//End page cMovement
//Begin page dRotation
//Two POI Changes: None
float angle(float v1[3], float v2[3])
{
    float angle = mathVecInner(v1, v2, 3)/(mathVecMagnitude(v1, 3) * mathVecMagnitude(v2, 3));
    angle = acosf(angle);
    if(!(angle < 1000))
        angle = 0.0f;
    return angle;
}

//End page dRotation
//Begin page eLogic
//Two POI Changes:
//Lines 24 through 33 are different
//Line 43 sets sizeof(bool)*3 rather than sizeof(bool)*6
void stateChecks()
{
    api.getMyZRState(j.myState);
    api.getOtherZRState(j.otherState);
    j.side = j.myState[1] < 0.0f;
    
    
    mathVecSubtract(j.toEarth, j.earth, j.myState, 3);
    j.shadowed = isInShadow(j.myState);
    
    j.numPics = game.getMemoryFilled();
    j.memSize = game.getMemorySize();
    if(j.numPics > j.lastPics)
    {
        j.picsTaken[j.targID] = true;
        if(j.zone == 0)
        {
            j.targZone = 1;
            mathVecScale(j.POITarget, j.POITarget, .52f, true);
        }
    }
    
    j.stageTime = j.time%60;
    j.flareTime = game.getNextFlare();
    
    if(j.stageTime < 2)
    {
        memset(j.picsTaken, 0, sizeof(bool) * 3);
        bestPOI(false);
    }
    
    
    if(j.flareTime < 0) j.flareTime = 31;
    if(j.flareTime == 1)
  {
      j.flareCtr = 4;
      j.numFlares++;
  }
  if(j.flareCtr > 0)
  {
      j.flareActive = true;
      j.flareCtr--;
  }
  else j.flareActive = false;
  if(j.flareActive) j.flareTime = -1;
  
  j.radius = mathVecMagnitude(j.myState, 3);
  j.zone = -1;
    float lim;
    for(lim = 0.31f; j.radius >= lim; lim += 0.11f)
        j.zone++;
    j.lastPics = j.numPics;
    DEBUG(("flares %d", j.numFlares));
}

bool isInShadow(float pos[3])
{
    return ((fabsf(pos[2]) < .2f) && (fabsf(pos[1]) < .2f) && ((pos[0] > 0.0f)));
}

bool seeEarth()
{
    float radius;
    if(j.myState[0] < 0.0f)
    {
        radius = -j.myState[0] * 0.33f + 0.22f;
        return j.myState[1]*j.myState[1] + j.myState[2]*j.myState[2] > radius*radius;
    }
    return true;
}
bool faceEarth()
{
    float tmp[3];
    mathVecSubtract(tmp, j.earth, j.myState, 3);
    return (angle(tmp, &j.myState[6]) <= .25);
}
//End page eLogic
//Begin page fPredictor
//Two POI Changes:
//Get rid of the targZone for loop in bestPOI()
//set targZone equal to zero, like in line 10, in bestPOI()
//on line 17 change timeToPoi to (timeToPoi - 5) in bestPOI()
void bestPOI(bool subOptimal)
{
    float fPoint[3];
    j.foundTarget = false;
    j.targZone = j.memSize < 2;
    
    for(j.timeToPoi=0; j.timeToPoi < (60 - j.stageTime); j.timeToPoi++)
    {
        for(j.targID = 0; j.targID < 2; ++j.targID)
        {
            
            DEBUG(("\npoi ID = %d\n", j.targID));
            
            futurePOIs(fPoint, j.targID, j.timeToPoi);
            mathVecScale(fPoint, fPoint, 0.41f + j.targZone*0.04f, true);
            DEBUG(("\ndistance to poi = %f\n", mathVecDistance(j.myState, fPoint)));
            if ((1066.66f*mathVecDistance(fPoint, j.myState)) < ((j.timeToPoi-3) * (j.timeToPoi-3)) 
               && (!j.picsTaken[j.targID]) 
               && (subOptimal || ((fPoint[2] < 0.33f) 
               && (fPoint[0] > -0.38f || j.numFlares > 0)))
               && (fPoint[0] < -0.1f) 
               && (j.time > 25 || j.targID == j.side)
               )
                {
                    memcpy(j.POITarget, fPoint, sizeof(float)*3);
                    j.foundTarget = true;
                    //j.other = -1;
                    return;
                }
        }
    }
    if(!subOptimal) bestPOI(true);
}

void futurePOIs(float res[3], int id, int steps)
{
    float POILoc[3];
    float angle;
    float radius;
    game.getPOILoc(POILoc, id);
    angle = atan2f(POILoc[2],POILoc[0]);
    radius = sqrtf(POILoc[2]*POILoc[2] + POILoc[0]*POILoc[0]);
    angle += 0.1f * steps;
    if((angle < 1.5708f) && (angle > -1.5708f))
    {
        angle += 3.1415f;
    }
    res[0] = radius*cosf(angle);
    res[1] = POILoc[1];
    res[2] = radius*sinf(angle);
}
//End page fPredictor
//Begin page main
void mainFunction()
{
    if(game.getFuelRemaining() == 0.0f || !j.foundTarget)
    {
        game.takePic(j.targID);
    }
    if(j.memSize == 0)
    {
        game.takePic(j.targID);
        getAround(j.otherState, 0);
    }
    float POI[3];
    float POILoc[3];
    float tmp[3];
    if((j.timeToPoi < -2) || !j.foundTarget)
    {
        bestPOI(false);
    }
    // futurePOIs(POI, j.targID, 1);
    game.getPOILoc(POILoc, j.targID);
    
    
    if(j.timeToPoi > 2)
    {
        futurePOIs(POI, j.targID, j.timeToPoi-4);
    }
    else
    {
        futurePOIs(POI, j.targID, 2);
    }
    
    mathVecSubtract(j.att, POI, j.myState, 3);
    // if(angle(j.myState, POI) > 1.6f && j.myState[2] > 0.0f)
    // {
    //     futurePOIs(POI, j.targID, (j.timeToPoi - 2));
    // }
    // mathVecSubtract(j.att, POI, j.myState , 3);
    mathVecSubtract(tmp, j.myState, POILoc, 3);
    
    if((j.time >= 165)  && (j.numPics > 0))
    {
        j.flareTime = 10;// we treat the end of the round as a solar flare
    }
    if(j.flareActive && !j.shadowed)
    {
        game.turnOff();
        j.off = true;
    }
    if(game.getFuelRemaining() < 15.0f && j.numFlares < 2)
    {
      j.flareTime = 10;
    }
    if(j.off && j.flareTime > 5 )
    {
        game.turnOn();
        bestPOI(false);
        j.off = false;
    }
    else
    {
        if(j.flareTime < 31 && j.numFlares<2)
        {
            goShadow();
        }
        else
        {
            if((j.numPics < j.memSize))
            {
                getAround(j.POITarget, 0);
                if(game.alignLine(j.targID) && j.zone == j.targZone)
                {
                    if(angle(tmp, POILoc) < (2-j.targZone)*.4f)
                    {
                        DEBUG(("\ntaking Picture\n"));
                        game.takePic(j.targID);
                    }
                }
            }
        }
    }
    if(j.numPics == j.memSize || (j.flareTime < 16 && j.numPics > 0))
    {
        goUpload();
    }
    

    
    if(notNan(j.att))
    api.setAttitudeTarget(j.att);
    j.timeToPoi--;
    //DEBUG(("\ntime To POI... might lie here... = %d\n", j.timeToPoi));
    
}
void loop()
{
    stateChecks();
    mainFunction();
    j.time++;
}
//End page main


};

ZRUser *zruser795 = new ZRUser01;
