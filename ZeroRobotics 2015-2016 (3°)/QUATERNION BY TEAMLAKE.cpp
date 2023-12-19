//Begin page main
float enemyState[12], myState[12], temp[3], refVec[3], baseQuat[4], quat[4];

void init(){
	//This function is called once when your code is first loaded.

	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
}

void loop(){
	api.getMyZRState(myState);
	api.getOtherZRState(enemyState);
	mathVecSubtract(temp, enemyState, myState, 3);
	mathVecNormalize(temp, 3);
	refVec[0] = -1.0f; refVec[1] = refVec[2] = 0.0f;
	baseQuat[0] = baseQuat[1] = baseQuat[2] = 0.0f; baseQuat[3] = 1.0f;
	api.attVec2Quat(refVec, temp, baseQuat, quat);
	if(game.getPicPoints() > 0.0f)
	    game.takePic();
	if(game.getMemoryFilled() > 1.0f) {
        temp[0] = temp[1] = 0.0f; temp[2] = 1.0f;
    	while(fabsf(myState[6]) < 0.25f && fabsf(myState[7]) < 0.25f && fabsf(myState[8] - 1) < 0.25f && 
    	        fabsf(myState[9]) > 0.05f && fabsf(myState[10]) > 0.05f && fabsf(myState[11]) > 0.05f)
    	    temp[0] *= -1; temp[1] *= -1; temp[2] *= -1;
	}
// 	if(game.getCurrentTime() > 170 && game.getPoints() <= game.getOtherPoints())
// 	    game.takePic();
// 	api.setAttitudeTarget(temp);
    api.setQuatTarget(quat);
}

//End page main
