//Begin page geometry
// ================================================================================================

// float getAngleBetween(float pointA[3], float secondPointA[3], float pointB[3], float secondPointB[3])
// {
//     float vectorA[3];
//     float vectorB[3];

//     mathVecSubtract(vectorA,pointA,secondPointA,3);     // Vector from A --> secondA 
//     mathVecSubtract(vectorB,pointB,secondPointB,3);     // Vector from B --> SecondB 

//     return getAngleBetweenVector(vectorA, vectorB);
// }

// ================================================================================================

// same thing but shorter -Christopher
float getAngleBetweenVector(float vectorA[3], float vectorB[3])
{
    return acosf(mathVecInner(vectorA, vectorB, 3) / (mathVecMagnitude(vectorA, 3) * mathVecMagnitude(vectorB, 3)));
}


// Note: this function breaks if you give it points within asteroid_radius of the origin
bool line_of_sight_between(float x[3], float y[3])
{
    float v[3];
    mathVecSubtract(v, y, x, 3);
    

    //float d = sqrtf(mathVecMagnitude(x,3)*mathVecMagnitude(x,3) - asteroid_radius*asteroid_radius);

    if (mathVecMagnitude(v, 3) < sqrtf(mathSquare(mathVecMagnitude(x, 3)) - mathSquare(0.33f)))
        return true;

    float cross[3];
    mathVecCross(cross, x, v);
    
    return ((mathVecMagnitude(cross, 3) / mathVecMagnitude(v, 3)) > 0.33f);
}

void calculate_intermediate_point(float ret[3], float a[3], float b[3])
{
    //float radius = 0.33f;
    float zero[3] = {0, 0, 0};
    // I'm defining a right-hand rule system.
    // i is the unit vector perpendicular to j and k given by the right-hand rule (j x k)
    // j is the unit vector in the direction from the SPHERE to the origin
    // k is the unit normal vector to the plane we're in
    float j[3];
    mathVecSubtract(j, zero, a, 3);
    mathVecNormalize(j, 3);
    
    float k[3];
    float ab[3];
    mathVecSubtract(ab, b, a, 3);
    mathVecCross(k, ab, j);
    mathVecNormalize(k, 3);
    
    float i[3];
    mathVecCross(i, j, k);
    
    // debug_vector("i", i);
    // debug_vector("j", j);
    // debug_vector("k", k);
    
    // So vectors defined in this system will have variable names starting with rh_
    
    // Let O be the origin (same in both systems)
    // P be sphere
    // float rh_P[3];
    // rh_P[0] = 0.0f;
    // rh_P[1] = -mathVecMagnitude(a, 3);
    // rh_P[2] = 0.0f;
    // I be point we're trying to find
    
    // theta is angle between OP and OI
    // Since PIO is a right angle, cos(theta) = |OI|/|OP|
    // |OI| is radius, and |OP| is just |P|, thus
    // float theta = acosf(radius / mathVecMagnitude(rh_P, 3));
    float theta = acosf(0.33f / mathVecMagnitude(a, 3));
    
    // // Use pythagorean thm to get magnitude of PI
    // // |OP|^2 = |PI|^2 + |OI|^2
    // // thus |PI| = sqrt(|OP|^2 - |OI|^2)
    // if (mathSquare(mathVecMagnitude(rh_P, 3)) - mathSquare(radius) < 0)
    //     DEBUG(("WWWWWWWWTTTTTTTTTTTTFFFFFFFFF???????????"));
    // float rh_PI_mag = sqrtf(mathSquare(mathVecMagnitude(rh_P, 3)) - mathSquare(radius));
    
    // Magnitude of OI is just the radius
    // Use magnitude and angle to find vector (we know it's in rh x-y plane)
    // Draw out the triangle if you have to
    float rh_OI[3];
    rh_OI[0] = 0.33f * sinf(theta);
    rh_OI[1] = 0.33f * -cosf(theta);
    rh_OI[2] = 0.0f;

    // I - O = OI
    // Since O is origin, I = OI

    // Convert back to original ZR system, and add a little buffer room.
    float buffer_scale = 1.1f;
    ret[0] = ((rh_OI[0] * i[0]) + (rh_OI[1] * j[0]) + (rh_OI[2] * k[0])) * buffer_scale;
    ret[1] = ((rh_OI[0] * i[1]) + (rh_OI[1] * j[1]) + (rh_OI[2] * k[1])) * buffer_scale;
    ret[2] = ((rh_OI[0] * i[2]) + (rh_OI[1] * j[2]) + (rh_OI[2] * k[2])) * buffer_scale;
    
    // If I screwed this up I'm gonna kill someone.
    // EDIT: Someone is officially going to die.
    // EDIT2: Fixed, people can live now.
    
}

void adjusted_target_pos(float ret[3])
{
    if (line_of_sight_between(position, target_position))
    {
        memcpy(ret, target_position, sizeof(float) * 3);
    }
    else
    {
        calculate_intermediate_point(ret, position, target_position);
    }
}

float adjusted_distance_to_point(float x[3])
{
    
    if (line_of_sight_between(position, x))
    {
        return vector_distance(position, x);
    }
    else
    {
        float real[3];
        calculate_intermediate_point(real, position, x);

        float fake[3];
        calculate_intermediate_point(fake, x, position);
        
        return ((getAngleBetweenVector(real, fake) * 0.33f) + 
                vector_distance(position, real) + vector_distance(x, fake));
    }
}
//End page geometry
//Begin page main
float target_position[3];
// target_attitude moved inside main

bool POI_status[3][2];

void init()
{
    //center_of_asteroid[0] = center_of_asteroid[1] = center_of_asteroid[2] =
    //shadow_zone_place[1] = shadow_zone_place[2] =
    //Earth_loc[1]= Earth_loc[2]=
    time_to_active_cam= 0;
    
    isOuter = true;
    
    //Earth_loc[0]=0.64f;
    
    // Distance to (0,0,0) must be > 0.33 for shadow_zone_place
    // shadow_zone_place[0] = 0.4f;
    
    Time = target_POI = target_time = -1;
    
}

void loop()
{
    float target_attitude[3];
    float zero[3] = {0, 0, 0};
    float shadow_zone_place[3] = {0.4f, 0, 0};
    float Earth_loc[3] = {0.64f, 0, 0};
    // Keep this at the beginning of the loop.
    Time++;

    float *state = (float*)malloc(12 * sizeof(float));
    api.getMyZRState(state);
    position = state;
    velocity = state + 3;
    attitude = state + 6;
    time_to_active_cam--;
    
    int taken_pic = game.getMemoryFilled();
    
    float max_velocity = 0.035f;
    
    
    
    
    // I thought we were losing 1 second of movement per taken picture due the function to take the pictures is at the end of loop
    //      Now it's at the top !
    
    // PS We should try to make the choice of target_POI and target_time preferring the id of the POI
    //      in which we have to take a picture from the outer zone..
    //      something like subtract 5 seconds if that's true (and add it again, later);
    
    // PPS We should also avoid to go to the POI if that will respawn (each 60s)..
    //      so  we can stay put or move to a strategic position (to define)
    
    // PPPS Try to reduce code-size if u have time :)
    
    // PPPPS Don't kill me for so many posts xD
    
    //  - Marco
    
    // Marco we're obviously going to try to kill you over how many posts you make
    
    // PS yeah code size is a bit of a problem
    
    // PPS Don't kill me for not contributing at all to this project
    
    // - Sam
    
    
    
    
    for(int i=0; i<3; i++)
    {
        //if(game.alignLine(i)) 
        //{
            float POI[3];
            game.getPOILoc(POI, i);
            float fromPOI[3];
            mathVecSubtract(fromPOI, position, POI, 3);
            
            float cross[3];
            mathVecCross(cross, fromPOI, attitude);
            if      ((mathVecMagnitude(cross, 3) < 0.05f) && mathVecInner(fromPOI, attitude, 2) < 0)
            //if(getAngleBetweenVector(toPOI, attitude)<0.25f)    // The new tolerance..
            {
             
                float myRadius = mathVecMagnitude(position, 3);
                bool zone = false;  // false for outer, true for inner;
                float anglePosition = getAngleBetweenVector(fromPOI, POI);
                
                
                
                // PLEASE NOTE THAT I DON'T KNOW IF ALSO THESE TOLERANCES HAVE BEEN INCREASED OF 0.15%
                // DON'T WORRY I INCREASED THEM -CHRIS
                
                
                //  supposed to be only one = (not ==)
                //        v
                if(((myRadius<0.53f && !POI_status[i][0] && anglePosition < 0.46f) ||
                    (zone = (myRadius<0.42f && !POI_status[i][1] && anglePosition < 0.92f)))
                   && time_to_active_cam<1)
                {
                    game.takePic(i);
                    time_to_active_cam = 3;     //Changed after the bug deployment 28/12/2014 -Marco
                    //DEBUG(("\n \n POI ID: %d, radius: %f anglePos %f \n Magnitude_vec_cross: %f Vec_inner: %f\n \n", i, myRadius, anglePosition,mathVecMagnitude(cross, 3), mathVecInner(fromPOI, attitude, 2) ));    
                    
                    if(taken_pic != game.getMemoryFilled())
                    {
                        POI_status[i][zone] = true;
                    }
                    target_POI = -1; // reset POI and time
                }
            }
        //}
    }
    
    if((Time % 60) == 0)
    {
        // reset POI every 60 seconds
        target_POI = -1;
        for (int i = 0; i < 3; i++)
            memset(POI_status[i], false, 3 * sizeof(bool));
    }
    
    // start loop stuff
	
	// Time to next solar flare
	int nextF = game.getNextFlare();
	//DEBUG(("\nTime to Flare: %d", nextF));
	// a point closer to us should be selected to be more fancy
	// it'll give us more time to take pictures
	//DEBUG(("next %i time %f\n", nextF, time_to_point(shadow_zone_place)));
	
	// pick a POI and go to it
	
	//  TRYING TO MAKE A MORE ACCURATE ESTIMATE, SECOND BY SECOND CONTROL UNTIL WE ARE VERY NEAR TO THE POI (4s FOR NOW)
	//  -MARCO
	//  NOT WORKING FOR NOW (Caused by getPerfectTimeToPOI)
	//  UP: WORKING NOW -MARCO
	float target_time_less_Time = target_time - Time;
	if ((target_POI == -1) || (Time > target_time + 1) || target_time_less_Time > 5)              //  (count<4))
	{
    	target_time = 10000;
    	target_POI = 0;
    	for (int id = 0; id < 3; id++)
    	{
    	    float POI_pos[3];
    	    game.getPOILoc(POI_pos, id);
    	    int POI_time = ALTgetPerfectTimeToPOI(POI_pos, !POI_status[id][0]);
    	    
    	    if ((POI_time < target_time) && !(POI_status[id][0] && POI_status[id][1]))
    	    {
    	        target_time = POI_time;
    	        target_POI = id;
    	    }
    	}
    	target_time += Time;
    	isOuter = !POI_status[target_POI][0];
    	
	    //DEBUG(("\n CALCULATING THE TIME: %d, Target:%d", Time+4, target_time));
	}
	target_time_less_Time = target_time - Time;
	
	DEBUG(("\nID: %d, target time: %i", target_POI, target_time));
	
	float init_location[3];
	game.getPOILoc(init_location, target_POI);
	float final_location[3];
	getPoiPosAfterTime(final_location, init_location, target_time_less_Time);
	
	//if( (target_time>60 && Time<60) || (target_time>120 && Time<120) )
	//    final_location[0]=10;
	
	//align_with_POI_loc(final_location, isOuter);
	// Attitude
	
    if(target_time_less_Time < 3)
	{
	    float position_after_1[3];
	    for(int i = 0; i<3; i++)
        {
            position_after_1[i] = position[i] + velocity[i];
        }
	    mathVecSubtract(target_attitude, final_location, position_after_1, 3);
	}
	else
	{
        mathVecSubtract(target_attitude, zero, final_location, 3);
	}
    
    
    // Position
    memcpy(target_position, final_location, 3 * sizeof(float));
    scale_vector(target_position, isOuter ? 0.43f : 0.39f);       //0.51f : 0.38f)

	
	// ===================
	
	int time_to_shadow_less_nextF   = time_to_point(shadow_zone_place) - nextF;
	
	
	DEBUG(("\n Time to shadow zone: %f,\n Time to next flare: %d, \n Time to shadow less nextFlare: %d, \n ID: %d.",time_to_point(shadow_zone_place), nextF, time_to_shadow_less_nextF, target_POI ));
	// This must go after so that we know target_position
	if      (
	                (nextF != -1 && time_to_shadow_less_nextF > 0)
	        ||      (taken_pic == game.getMemorySize())
	        ||      ((target_time_less_Time)*2 > nextF && nextF!= -1)
	        ||      (taken_pic && (( Time>165 && target_time_less_Time > 3) || (game.getFuelRemaining() < 5)))
	        )
	// Moreover, we should stay in the shadow zone if we haven't time enough to reach the POI and then come back !
	// UPDATE: I added a prototype of this function
	{
        memcpy(target_position, shadow_zone_place, 3 * sizeof(float));
        
        if(time_to_shadow_less_nextF < -2)
        {
            DEBUG(("\n INCREASED VELOCITY"));
            max_velocity = 0.05f;
        }
        
        
        if(taken_pic) // taken_pic > 0
        {
            mathVecSubtract(target_attitude, Earth_loc, position, 3);
            game.uploadPic();
        }
        else
        {
            game.takePic(0);
            target_attitude[0]=10;      // Just an impossible number to be assigned in diffent way
        }
            
        if(((mathVecMagnitude(position+1, 2) < 0.18f) && (position[0] > 0)))
        // It's better 0.18 than 0.2 cause the other sphere could try to put our sphere out !
        
            target_position[0]=10;      // Just an impossible number to be assigned in diffent way
	}
	
// 	DEBUG(("%i", min_time));
    
     if ((target_time_less_Time) > 6)
     {
         game.takePic(0); // worth .01 points each
     }
    
    
	// Keep this at the end of the loop.
	// ============================= MOVEMENT ==========================================
	
	// =============== POSITION
    float adjusted_target_position[3];
    float radius = 0.33f;
    
    if(target_position[0]==10)
    {
        api.setVelocityTarget(zero);
    }
    else
    {
        adjusted_target_pos(adjusted_target_position);
        
        // DEBUG(("D: %f\n", (adjusted_distance_to_point(target_position) * 0.06f)));
        // DEBUG(("V: %f\n", mathVecMagnitude(velocity, 3)));
        // debug_vector("vel", velocity);
    if(mathVecMagnitude(position, 3) < radius)
    {
        
        DEBUG(("WARNING: INSIDE RADIUS\n"));
        scale_vector(position, 100);
        api.setVelocityTarget(position);
        
    } else if ((mathVecMagnitude(velocity, 3) > sqrtf(adjusted_distance_to_point(target_position) * 0.008f)) 
            || (adjusted_distance_to_point(target_position) < 0.015f)) 
        {
            
            //DEBUG(("position target\n"));
            api.setPositionTarget(adjusted_target_position);
            
        } else {
            
            //DEBUG(("velocity target\n"));
            
            float target_velocity[3];
            mathVecSubtract(target_velocity, adjusted_target_position, position, 3);
            mathVecNormalize(target_velocity, 3);
    
            scale_vector(target_velocity, max_velocity);
            
            api.setVelocityTarget(target_velocity);
            
        }
    }
   
    
    // =============== ATTITUDE
    if(target_attitude[0] != 10)
        api.setAttitudeTarget(target_attitude);
 
}
//End page main
//Begin page misc
// bool in_range(float x, float min, float max)
// {
//     return ((x <= max) && (x >= min));
// }
//End page misc
//Begin page movement


// POI inner outer
// whether we have taken the picture at inner or outer

/*
void align_with_POI_loc(float POI_location[3], bool outer)
{
    // Attitude
    mathVecSubtract(target_attitude, center_of_asteroid, POI_location, 3);
    
    // Position
    memcpy(target_position, POI_location, 3 * sizeof(float));
    // Make POI_location a unit vector so we can multiply and stuff.
    mathVecNormalize(target_position, 3);
    scale_vector(target_position, outer ? 0.43f : 0.39f);       //0.51f : 0.38f)
    
}
*/

// void reset_POI_status()
// {
//     for (int i = 0; i < 3; i++)
//         for (int j = 0; j < 2; j++)
//             POI_status[i][j] = false;
// }
//End page movement
//Begin page state
float *position, *velocity, *attitude;
bool isOuter;

int target_POI, target_time, time_to_active_cam, Time;     // Used to take picture and calculate the min position where we can reach the POI


// void update_state()
// {
//     float state[12];
    
//     Time++;

//     api.getMyZRState(state);
//     memcpy(position, state, 3 * sizeof(float));
//     memcpy(velocity, state + 3, 3 * sizeof(float));
//     memcpy(attitude, state + 6, 3 * sizeof(float));
//     time_to_active_cam--;
    
//     if((Time % 60) == 0)
//     {
//         // reset POI every 60 seconds
//         target_POI = -1;
//         reset_POI_status();
//     }
// }

// bool in_shadow_zone(float point[3])
// {
//     /*
//       Shadow zone dimensions:
//       0.0 x 0.64
//       -0.2 x 0.2
//       -0.2 x 0.2
//     */
//     return (in_range(point[0], 0.0f, 0.64f) && in_range(point[1], -0.2f, 0.2f) && in_range(point[2], -0.2f, 0.2f));
// }

// void sub_update_state(int i, float array[3], float state[12])
// {
//     for(int j = 0; j++; j < 3)
//         array[j] = state[i + j];
// }

// float distance_from_surface()
// {
//     // Asteroid radius is 0.2
//     return distance_from_center() - 0.2f;
// }

// float distance_to_target_position()
// {
//     return vector_distance(position, target_position);
// }
//End page state
//Begin page timing
// time_to_point updated by Christopher 2014-11-28
// basically it is the result of random guessing and tweaking the constants
float time_to_point(float x[3])
{
    float total = 0.0f;
    
    float target[3];
    adjusted_target_pos(target);
    float dir[3];
    mathVecSubtract(dir, target, position, 3);
    mathVecNormalize(dir, 3);
    float vel = mathVecInner(dir, velocity, 3);
    //DEBUG(("vel %f\n", vel));
    float dv = 0.034f - vel;
    
    if (vel < 0.034f)
    {
        total += dv * ((adjusted_distance_to_point(x) < 0.1f) ? -100 : 108);
    }
    
    return total + (adjusted_distance_to_point(x) / 0.0342f) + 3.5f;
}

// Changes by Marco 10/12..
// timeToArrive can't be lower than time_to_active_cam

// Changes by Christopher 29/11 14:50 UTC
// time_to_point was using initialPOI instead of posPOI
// adjusted depending on inner/outer zone

// ADDED BY MARCO 27/11 - 20.50 UTC
// START 

//  It returns the ideal time to reach the POI..
/*int getPerfectTimeToPOI(float initialPOI[3], bool outer)
{
    int timeToArrive = 0, timeRotPOI = 0;
    float posPOI[3];
    int i = 0;
    do
    {
        timeRotPOI = timeToArrive;
        getPoiPosAfterTime(posPOI, initialPOI, timeRotPOI);
        
        scale_vector(posPOI, outer ? 0.43f : 0.39f);        //0.51f(0.44) : 0.38f
        
        timeToArrive = (int)time_to_point(posPOI) + 1; //ceilf(time_to_point(posPOI))
        i++;
    }
    while (timeToArrive != timeRotPOI && (i < 1000));
    
    if(timeToArrive < time_to_active_cam)
        timeToArrive = time_to_active_cam;
    
    //DEBUG(("\nTIME: %d", timeToArrive));
    //DEBUG(("\n %i", i));
    
    return timeToArrive;
}*/

// Alternate version, hopefully fixed (No Chris, it doesn't work yet -Marco)
// It's working now. -Marco
int ALTgetPerfectTimeToPOI(float initialPOI[3], bool outer)
{   
    int timeToArrive = 1000;
    float posPOI[3];
    
    for(int i = 0; !(timeToArrive < i) || (i<100); i++)
    {
        getPoiPosAfterTime(posPOI, initialPOI, i);
        scale_vector(posPOI, outer ? 0.43f : 0.39f);        //0.51f(0.44) : 0.38f
        timeToArrive = (int)time_to_point(posPOI) + 1; //ceilf(time_to_point(posPOI))
        
    }
    
    if(timeToArrive < time_to_active_cam)
        timeToArrive = time_to_active_cam;
    
    return timeToArrive;
}



// It returns the position of the POI (finalPos) after "dT" seconds.
void getPoiPosAfterTime(float finalPos[3], float startPos[3], float dT )
{
    
    // getRadiusToY
    float Y[3] = {0, startPos[1], 0};
    float radius = vector_distance(startPos, Y);
    //end
    float v[3];
    
    memcpy(v, startPos, 3*sizeof(float));
    mathVecNormalize(v,3);
    
    finalPos[0]=radius*(v[0]*cosf(0.1f*dT)-v[2]*sinf(0.1f*dT));
    finalPos[1]=startPos[1];
    finalPos[2]=radius*(v[0]*sinf(0.1f*dT)+v[2]*cosf(0.1f*dT));
    
    if(finalPos[0]>0)
    {
        // getPoiPosAfterTime(finalPos, startPos, dT+30.4, false);
        finalPos[0] *= -1;
        finalPos[2] *= -1;
    }
    
}

// float getRadiusToY(float item[3])
// {
//     float Y[3] = {0, item[1], 0};
    
//     return vector_distance(item, Y);
// }



//  It returns after how many seconds the POI will be in the bottom side..

short int getTimeToSwap(float pos[3])
{
    short int i = 0;
    float future1[3], future2[3];
    
    do
    {
        i++;
        getPoiPosAfterTime(future1, pos, i);
        getPoiPosAfterTime(future2, pos, i+1);
    }
    while (future1[2] > future2[2]);
    
    return i;
}

//END
//End page timing
//Begin page vector
// void normalize_vector(float vec[3])
// {
//     float mag = sqrtf(mathSquare(vec[0]) + mathSquare(vec[1]) + mathSquare(vec[2]));
//     scale_vector(vec, (1 / mag));
//     // mathVecNormalize(vec, 3);
// }

void scale_vector(float vec[3], float scale)
{
    mathVecNormalize(vec, 3);
    vec[0] *= scale;
    vec[1] *= scale;
    vec[2] *= scale;
}

// void debug_vector(char *name, float x[3])
// {
//     DEBUG(("%s: %f %f %f\n", name, x[0], x[1], x[2]));
// }

float vector_distance(float position[3], float other[3])
{
    float d[3];
    mathVecSubtract(d, position, other, 3);
    return mathVecMagnitude(d, 3);
}
//End page vector
