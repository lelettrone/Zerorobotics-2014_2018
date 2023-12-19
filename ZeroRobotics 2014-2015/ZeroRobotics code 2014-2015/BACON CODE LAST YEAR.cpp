#include "ZRGame.h"
#include "ZR_API.h"
#include "ZRUser.hpp"

#include 

static ZeroRoboticsGame &game = ZeroRoboticsGame::instance();
static ZeroRoboticsAPI &api = ZeroRoboticsAPI::instance();

//Implement your simulation code in init() and loop()
class ZRUser01 : public ZRUser
{
//Begin page main
//HAPPY BIRTHDAY PENNY!
float myPos[3];
float enemyPos[3];
short nextFlare;
short gameTime;
short chosenPOIID;
float zVec[3];
float lattitudeAngle;
float chosenPOILoc[3];
float height;
bool POIPhotoTaken[6];
bool bottom;
short flareCount;
float myHeight;
short timeToSwitch;
short timeToPoint;
bool running;
bool missed;

void init() {
    gameTime = -1;
    flareCount=zVec[0]=zVec[1]=0;
    zVec[2] = -1.0f;
    flushPOIPhotos(); 
    chosenPOIID = 2;
    missed=bottom=false;
}

void loop(){

    gameTime++;
    running = false;
    float zeroVec[3] = {0.0f, 0.0f, 0.0f};
    float darkSpot[3] = {0.39f, 0.0f, 0.0f};
    timeToSwitch = 60 - gameTime % 60;
    short upload = 0;
    float state[12];
    api.getOtherZRState(state);
    enemyPos[0] = state[0];
    enemyPos[1] = state[1];
  enemyPos[2] = state[2];
    api.getMyZRState(state);
    myPos[0] = state[0];
  myPos[1] = state[1];
  myPos[2] = state[2];
  
  myHeight = mathVecMagnitude(myPos, 3); 
  bool inShadowZone = myPos[0] > 0 && myPos[1]*myPos[1] + myPos[2]*myPos[2] < 0.04f; 
  
  if (gameTime == 3 && (enemyPos[0] < -0.012f || enemyPos[2] < -0.005f)) 
      bottom = true;

  if (inShadowZone) 
      bottom = (myPos[2] > enemyPos[2]) ? true:false;
      
  short memSize = game.getMemorySize();
  short preRoundPhotos = game.getMemoryFilled();
  nextFlare = game.getNextFlare();
  if (nextFlare < 0)
      nextFlare = 100;
      
  if (nextFlare == 1) {
      flareCount++;
      if (!inShadowZone) {
            game.turnOff();
            game.turnOn();
      }
    }
  
  if (timeToSwitch == 60) 
        flushPOIPhotos(); 

    getNewChosenPoint();
    
    game.getPOILoc(chosenPOILoc, chosenPOIID);
    lattitudeAngle = (bottom) ? 2.54f : 0.1f;
    if (memSize == 1 && bottom)
        lattitudeAngle += 0.3f;
    float rotAngle = getRotationAngle(chosenPOILoc);
    float futurePOISpot[3];
    float futurePhotoSpot[3];
    float vecToPOI[3];
    POIPredict(futurePOISpot, chosenPOILoc, rotAngle);
    vecManip(futurePhotoSpot, height*5, futurePOISpot);
       
  if (timeToLattitude(chosenPOILoc) >= 6 && memSize - preRoundPhotos == 2) {
        POIPredict(futurePOISpot, chosenPOILoc, rotAngle - 0.6f);
        POIPredict(futurePhotoSpot, chosenPOILoc, rotAngle - 0.3f);
        vecManip(futurePhotoSpot, 1.95f, futurePhotoSpot);
        mathVecSubtract(vecToPOI, futurePOISpot, futurePhotoSpot, 3);
    }
    else 
     mathVecSubtract(vecToPOI, chosenPOILoc, myPos, 3); 
  
  if (memSize == 1 || missed)
      mathVecSubtract(vecToPOI, futurePOISpot, futurePhotoSpot, 3);
  
      
    mathVecNormalize(vecToPOI, 3);

    if ((game.getMemoryFilled() > 0) 
        && (nextFlare < 8 || preRoundPhotos == memSize || gameTime >= 170
        || (timeToLattitude(chosenPOILoc) > 10 
        && distance(myPos, futurePhotoSpot) < 0.05f) || game.getFuelRemaining() <= 1)) {
        uploadPhotos();
        api.setVelocityTarget(zeroVec);
        upload = 1;
    }
    else if (game.alignLine(chosenPOIID)) {
            float vecPOItoSPHERE[3];
            mathVecSubtract(vecPOItoSPHERE, myPos, chosenPOILoc, 3);
            if ((angleCalc(vecPOItoSPHERE, chosenPOILoc) < ((myHeight > 0.42f) ? 0.45f : 0.9f))
                && !(myHeight < 0.42f && POIPhotoTaken[chosenPOIID * 2])) {
              game.takePic(chosenPOIID);
                }
            if (game.getMemoryFilled() > preRoundPhotos)  
                POIPhotoTaken[chosenPOIID * 2 + ((myHeight > 0.42f) ? 1:0)] = true;
    }
  if (running || (timeToLattitude(chosenPOILoc) > 14 && timeToSwitch > 3))
      game.takePic(0);
      
    if ((realTimeToLat(chosenPOILoc, chosenPOIID) + 20 > nextFlare) && flareCount == 0) {
        if (game.getMemoryFilled() > 0)
            uploadPhotos();
        running = true;
        moveToPoint(darkSpot);
    }    
    else if (upload == 0) {
        moveToPoint(futurePhotoSpot);
        api.setAttitudeTarget(vecToPOI);
    }
    
    if (gameTime < 3) 
        moveToPoint(zeroVec);
    
    if (game.getMemoryFilled() == 1 && timeToLattitude(chosenPOILoc) > 10)
        missed = true;
    else if (game.getMemoryFilled() != 1)
        missed = false;
    DEBUG(("timeToLattitude %i\n gameTime %i\n", timeToLattitude(chosenPOILoc), gameTime));
        
}

void uploadPhotos() {
    float vecToEarthPos[3] = {0.64f - myPos[0], -myPos[1], -myPos[2]};
    mathVecNormalize(vecToEarthPos, 3);
    api.setAttitudeTarget(vecToEarthPos);
    game.uploadPic();
}


short realTimeToLat(float givenPOILoc[3], short POIID) {
    float futurePOILoc[3];
    short timeToLat = timeToLattitude(givenPOILoc);
    POIPredict(futurePOILoc, givenPOILoc, (getRotationAngle(givenPOILoc) 
        - ((timeToLat > 6 && game.getMemorySize() != 1) ? 0.3f : 0.0f)));
    timeToPoint = angleCalc(futurePOILoc, myPos) * 12 + (fabs(myHeight - height)*40) + 2;
    if (POIID == chosenPOIID && timeToLat <= 8 && game.getMemoryFilled() != game.getMemorySize())
        timeToPoint -= 10;
    if (timeToPoint > timeToLat)
        timeToLat += 31;
    return timeToLat;
}

short timeToLattitude(float givenPOILoc[3]) {
    float vecCircleToPOILoc[3] = {givenPOILoc[0], 0.0f, givenPOILoc[2]};
    float timeToTop = angleCalc(vecCircleToPOILoc, zVec) * 10 - 1;
    return (bottom) ? ((timeToTop > 25 && timeToTop < 32) ? timeToTop - 25 : timeToTop + 8) : timeToTop;
}

float getRotationAngle(float POILoc[3]) {
    float vecCircToPOI[3] = {POILoc[0], 0.0f, POILoc[2]};
    return angleCalc(vecCircToPOI, zVec) - lattitudeAngle;  
}

void POIPredict(float futurePOILoc[3], float currentPOILoc[3], float theta) {
    float a1 = cosf(theta);
    float a2 = -sinf(theta);
    futurePOILoc[0] = currentPOILoc[0]*a1 + currentPOILoc[2]*a2;
    futurePOILoc[1] = currentPOILoc[1];
    futurePOILoc[2] = -currentPOILoc[0]*a2 + currentPOILoc[2]*a1;
}

void moveToPoint(float targetPos[3]) {
    float velocityTarget[3];
    if (angleCalc(targetPos, myPos) > 1.16f || myHeight < 0.35f) {
        mathVecAdd(targetPos, targetPos, myPos, 3);
        mathVecNormalize(targetPos, 3);
        vecManip(targetPos, (running) ? 0.38f : 0.42f, targetPos);
    }
    mathVecSubtract(velocityTarget, targetPos, myPos, 3);
    float dist = mathVecMagnitude(velocityTarget, 3);
    mathVecNormalize(velocityTarget, 3);
    vecManip(velocityTarget, (dist*dist < 0.02f) ? dist*dist : 0.02f, velocityTarget);
    if (realTimeToLat(chosenPOILoc, chosenPOIID) - timeToPoint < 10 || running) {
      api.setVelocityTarget(velocityTarget);
  }
    api.setPositionTarget(targetPos);
}




void getNewChosenPoint() {
    float POILoc[3];
    short interimPOIID = chosenPOIID;
    short minTimeToLat = 50;
    height = 0.5f;
    for (short POIID = 0; POIID < 3; POIID++) {
        game.getPOILoc(POILoc, POIID);
        short timeToLat = realTimeToLat(POILoc, POIID);
        short weight = (POIPhotoTaken[POIID * 2 + 1]) ? 8 : 0;
        if (POIID == (myPos[1] > 0) ? 1 : 0)
            weight += 3;
        if (enemyPos[1] * POILoc[1] > 0.0001f) {
            weight += 5;
            if (enemyPos[2] * myPos[2] > 0)
                weight += 10;
        }
        if (timeToLat < timeToSwitch
            && timeToLat + weight < minTimeToLat && !(POIPhotoTaken[POIID * 2] && POIPhotoTaken[POIID * 2 + 1])) { 
            interimPOIID = POIID;
            minTimeToLat = timeToLat + weight;
        }
    }
    chosenPOIID = interimPOIID;
    game.getPOILoc(chosenPOILoc, chosenPOIID);
    
    if (minTimeToLat > 49) 
        chosenPOIID = 2;
    
    if (POIPhotoTaken[chosenPOIID * 2 + 1] && timeToLattitude(chosenPOILoc) < timeToSwitch)
        height = 0.39f;
}

void flushPOIPhotos() { 
    POIPhotoTaken[0]=POIPhotoTaken[1]=POIPhotoTaken[2]=POIPhotoTaken[3]=POIPhotoTaken[4]=POIPhotoTaken[5]=false;
}

void vecManip(float outVec[3], float c, float inVec[3])
{   
  outVec[0] = c * inVec[0];
  outVec[1] = c * inVec[1];
  outVec[2] = c * inVec[2];
}

float angleCalc(float vec1[3], float vec2[3])
{
  return acosf(mathVecInner(vec1, vec2, 3)/(mathVecMagnitude(vec1,3) * mathVecMagnitude(vec2,3)));
}

float distance(float pos1[3], float pos2[3]) {
    float differenceVec[3];
    mathVecSubtract(differenceVec, pos1, pos2, 3);
    return mathVecMagnitude(differenceVec, 3);
}
//End page main
};

ZRUser *zruser787 = new ZRUser01;
