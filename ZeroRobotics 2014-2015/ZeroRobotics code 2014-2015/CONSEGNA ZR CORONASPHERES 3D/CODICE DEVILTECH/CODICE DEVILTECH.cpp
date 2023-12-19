//Begin page geometry
// Note: this function breaks if you give it points within asteroid_radius of the origin
bool line_of_sight_between(float x[3], float y[3], float asteroid_radius) {
    
    float v[3];
    mathVecSubtract(v, y, x, 3);
    
    float d = sqrtf(mathSquare(mathVecMagnitude(x, 3)) - mathSquare(asteroid_radius));
    float xy[3];
    mathVecSubtract(xy, x, y, 3);
    
    if (mathVecMagnitude(xy, 3) < d)
        return true;

    float cross[3];
    mathVecCross(cross, x, v);
    
    return ((mathVecMagnitude(cross, 3) / mathVecMagnitude(v, 3)) > asteroid_radius);

}

void calculate_intermediate_point(float ret[3], float a[3], float b[3], float radius) {
    
    // I'm defining a right-hand rule system.
    // i is the unit vector perpendicular to j and k given by the right-hand rule (j x k)
    // j is the unit vector in the direction from the SPHERE to the origin
    // k is the unit normal vector to the plane we're in
    float j[3];
    mathVecSubtract(j, center_of_asteroid, a, 3);
    normalize_vector(j);
    
    float k[3];
    float ab[3];
    mathVecSubtract(ab, b, a, 3);
    mathVecCross(k, ab, j);
    normalize_vector(k);
    
    float i[3];
    mathVecCross(i, j, k);
    
    // debug_vector("i", i);
    // debug_vector("j", j);
    // debug_vector("k", k);
    
    // So vectors defined in this system will have variable names starting with rh_
    
    // Let O be the origin (same in both systems)
    // P be sphere
    float rh_P[3];
    rh_P[0] = 0.0f;
    rh_P[1] = -mathVecMagnitude(a, 3);
    rh_P[2] = 0.0f;
    // I be point we're trying to find
    
    // theta is angle between OP and OI
    // Since PIO is a right angle, cos(theta) = |OI|/|OP|
    // |OI| is radius, and |OP| is just |P|, thus
    float theta = acosf(radius / mathVecMagnitude(rh_P, 3));
    
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
    rh_OI[0] = radius * sinf(theta);
    rh_OI[1] = radius * -cosf(theta);
    rh_OI[2] = 0.0f;

    // I - O = OI
    // Since O is origin, I = OI

    // Convert back to original ZR system, and add a little buffer room.
    float buffer_scale = 1.1;
    ret[0] = ((rh_OI[0] * i[0]) + (rh_OI[1] * j[0]) + (rh_OI[2] * k[0])) * buffer_scale;
    ret[1] = ((rh_OI[0] * i[1]) + (rh_OI[1] * j[1]) + (rh_OI[2] * k[1])) * buffer_scale;
    ret[2] = ((rh_OI[0] * i[2]) + (rh_OI[1] * j[2]) + (rh_OI[2] * k[2])) * buffer_scale;
    
    // If I screwed this up I'm gonna kill someone.
    // EDIT: Someone is officially going to die.
    // EDIT2: Fixed, people can live now.
    
}

float adjusted_distance_to_target_position() {
    
    // for now
    return distance_to_target_position();
    
}
//End page geometry
//Begin page main
void init()
{

	state_init();
	movement_init();
	
}

void loop()
{
    
	// Keep this at the beginning of the loop.
	update_state();
	
	// DEBUG(("score afaik %f\n", score));
 	// DEBUG(("zone is %i\n", zone()));
	
	if (game.getNextFlare() == 2) {
	    
	    off = true;
	    game.turnOff();
	    
	} else if (((game.getNextFlare() == -1) || (game.getNextFlare() > 2)) && off) {
	    
	    off = false;
	    game.turnOn();
	    
	}
	
	if (attempted_pic[1]) {
	    
        if ((score + 0.10) <= game.getScore()) {

            POI_status[attempted_pic[0]][attempted_pic[1] - 1] = true;
            DEBUG(("PIC SUCCESS!\n"));
            
        } else {
            
            DEBUG(("PIC FAIL.\n"));
            
        }
        
        attempted_pic[1] = 0;
        
	}
	
    // update, now that we've done any comparisons
    score = game.getScore();
	
	if (get_next_POI_update() == 60)
	    reset_POI_status();
	    
// 	DEBUG(("%i %i\n%i %i\n%i %i\n",
// 	       POI_status[0][0], POI_status[0][1],
// 	       POI_status[1][0], POI_status[1][1],
// 	       POI_status[2][0], POI_status[2][1]));

    if (zone() == ALMOST_DANGER_ZONE || zone() == DANGER_ZONE) {
        
        // move away from asteroid, I'm lazy
        go_to_upload();
	    DEBUG(("ASTEROID AVOIDANCE\n"));
    
        
    } else if (zone() == ALMOST_OUT_OF_BOUNDS_ZONE) {
        
        go_to_neutral_position();
        DEBUG(("OUT OF BOUNDS AVOIDANCE\n"));
        
    } else if ((zone() == UPLOAD_ZONE) && game.getMemoryFilled()) {
        
        game.uploadPic();
        DEBUG(("UPLOADING NOW\n"));
        
    } else if ((game.getNextFlare() < 20) && (game.getNextFlare() != -1) && game.getMemoryFilled()) {
        
        go_to_upload();
        DEBUG(("FLARE INCOMING, MOVING TO UPLOAD T-%i\n", game.getNextFlare()));
    
        
    } else if ((game.getNextFlare() < 8) && (game.getNextFlare() != -1)) {
        
        stay_put = true;
        DEBUG(("FLARE INCOMING, STAYING PUT T-%i\n", game.getNextFlare()));
        
    } else if (((240 - api.getTime()) < 20) && game.getMemoryFilled()) {
        
        go_to_upload();
        DEBUG(("END IS NEAR, MOVING TO UPLOAD T-%i\n", 240 - api.getTime()));
    
    } else if (game.getMemoryFilled() < game.getMemorySize()) {
        
        int POI = locate_best_POI(true);
        int closest_POI = locate_best_POI(false);
        
        if ((POI != last_POI) && game.getMemoryFilled()) {
            
            go_to_upload();
            
        } else {
            
            last_POI = POI;
        
            if (!worth_movement_to_POI(POI) && (POI != closest_POI)) {
                
                // align with the closest POI (outer) 
                // and hope next round has one near there
                DEBUG(("not worth it\n"));
                
                if (game.getMemoryFilled()) {
                    
                    go_to_upload();
                    DEBUG(("SO UPLOADING\n"));
                    
                } else {
                    
                    go_to_neutral_position();
                    DEBUG(("SO VISITING NEUTRAL POS\n"));
                
                }
                
            } else {
                
                attempting_valid_POI = true;
                align_with_POI(POI, outer(POI));
                
                if (line_of_sight(POI) && (zone() == (outer(POI) ? OUTER_ZONE : INNER_ZONE))) {
                    
                    game.takePic(POI);
                    attempted_pic[0] = POI;
                    attempted_pic[1] = (zone() == OUTER_ZONE) ? 2 : 1;
                    DEBUG(("I TRIED TO TAKE A PICTURE!!!\n"));
                
                    
                }
                
                DEBUG(("MOVING TO POI %i, %s\n", POI, outer(POI) ? "outer" : "inner"));
            
            }
        
        }
    
    } else {
        
        go_to_upload();
        DEBUG(("MEM FULL, MOVING TO UPLOAD\n"));
        
    }
    
    // take invalid pics if we aren't doing anything
    if ((distance_to_target_position() > 0.1) || !attempting_valid_POI)
    {
        
        int POI = locate_best_POI(false);
        game.takePic(POI);
        
        if (line_of_sight(POI)) {
        
            attempted_pic[0] = POI;
            attempted_pic[1] = (zone() == OUTER_ZONE) ? 2 : 1;
        
        }
        
    }
    
    // reset every second
    attempting_valid_POI = false;
    
    // DEBUG(("dist: %f\n", mathVecMagnitude(position, 3)));

	// Keep this at the end of the loop.
	move();
	
}
//End page main
//Begin page misc
bool in_range(float value, float min, float max)
{
    
    return (value > min && value <= max);

}

bool in_rectangle(float value_x, float value_y, float min_x, float max_x, float min_y, float max_y)
{
    
    return (in_range(value_x, min_x, max_x) && in_range(value_y, min_y, max_y));
    
}

bool in_prism(float value_x, float value_y, float value_z, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z)
{
    
    return (in_range(value_x, min_x, max_x) && in_range(value_y, min_y, max_y) && in_range(value_z, min_z, max_z));
    
}
//End page misc
//Begin page movement
float target_position[3];
float target_attitude[3];

bool stay_put;
bool off;

// POI inner outer
// whether we have taken the picture at inner or outer
bool POI_status[3][2];

bool override_asteroid_avoidance;
bool attempting_valid_POI;
int last_POI;

void movement_init() {
    
    memcpy(target_position, position, 3 * sizeof(float));
    memcpy(target_attitude, attitude, 3 * sizeof(float));
    
    stay_put = false;
    
    off = false;

    reset_POI_status();
    
    attempting_valid_POI = false;
    
    last_POI = -1;

}

void move() {
    
    // debug_vector("pos", target_position);
    // DEBUG(("att: %f %f %f\n", target_attitude[0], target_attitude[1], target_attitude[2]));
    
    float adjusted_target_position[3];
    bool is_nan = false;
    
    // Position
    float radius = 0.33f;
    if (line_of_sight_between(position, target_position, radius) || override_asteroid_avoidance) {
    
        if (isnan(target_position[0])) { // quick non-null test
            
            DEBUG(("WARNING: NULL TARGET!\n"));
            is_nan = true;
            
        } else
            memcpy(adjusted_target_position, target_position, sizeof(float) * 3);
        // debug_vector("n", target_position);
    
        
    } else {
        
        float intermediate[3];
        calculate_intermediate_point(intermediate, position, target_position, radius);
        if (isnan(intermediate[0])) { // quick non-null test
            
            DEBUG(("WARNING: NULL INTERMEDIATE TARGET!\n"));
            is_nan = true;
            
        } else
            memcpy(adjusted_target_position, intermediate, sizeof(float) * 3);
        // debug_vector("y", intermediate);
        
    }
    
    if (!is_nan) {
    
        float max_acceleration = 0.004f;
        
        if (stay_put) {
            
            // cheap way to get [0, 0, 0]
            api.setVelocityTarget(center_of_asteroid);
            
            // reset
            stay_put = false;
            
        } else if(mathVecMagnitude(velocity, 3) > sqrtf(2 * adjusted_distance_to_target_position() * max_acceleration)) {
            
            DEBUG(("position target\n"));
            api.setPositionTarget(adjusted_target_position);
            
        } else {
            
            DEBUG(("velocity target\n"));
            
            float target_velocity[3];
            mathVecSubtract(target_velocity, adjusted_target_position, position, 3);
            normalize_vector(target_velocity);
            
            float max_vel = 0.05f;
            
            scale_vector(target_velocity, max_vel);
            
            api.setVelocityTarget(target_velocity);
            
        }
    }
    
    
    // Old method
    
    // if (adjusted_distance_to_target_position() < 0.05) {
        
    //     DEBUG(("closed-loop\n"));
    //     api.setPositionTarget(adjusted_target_position);
    
        
    // } else {
        
    //     DEBUG(("open-loop\n"));
    //     float target_velocity[3];
    //     mathVecSubtract(target_velocity, adjusted_target_position, position, 3);
        
    //     if (mathVecMagnitude(target_velocity, 3) != 0) {
            
    //         normalize_vector(target_velocity);
            
    //         float acceleration = 0.003f;
    //         float max_vel = 0.05f;
            
    //         float target_vel;
            
    //         float decreasing_vel = sqrtf(2 * adjusted_distance_to_target_position() * acceleration);
    //         float vel_in_direction = mathVecInner(velocity, target_velocity, 3);
            
    //         if (decreasing_vel < max_vel) {
                
    //             target_vel = decreasing_vel;
    //             DEBUG(("decelerating\n"));
            
    //         } else if (vel_in_direction >= max_vel) {
    //             target_vel = max_vel;
    //             DEBUG(("at max\n"));
    //         } else {
                
    //             target_vel = vel_in_direction + acceleration; // a * t = a because t = 1s
    //             DEBUG(("accelerating\n"));
                
    //         }
            
    //         scale_vector(target_velocity, target_vel);
            
    //     }
        
    //     float forces[3];
    //     mathVecSubtract(forces, target_velocity, velocity, 3);
    //     float SPHERES_mass = 4.45;
        
    //     // F = ma
    //     scale_vector(forces, SPHERES_mass);
    //     api.setForces(forces);
        
    // }
    
    // Attitude
    float target_unit_attitude[3];
    memcpy(target_unit_attitude, target_attitude, 3 * sizeof(float));
    normalize_vector(target_unit_attitude);
    api.setAttitudeTarget(target_unit_attitude);
    
    // Reset every second
    override_asteroid_avoidance = false;
    is_nan = false;
    
}

// void stay_put() {
    
//     float epsilon = 0.1;
    
//     float d[3];
//     mathVecSubtract(d, stay_put_position, position, 3);
    
//     if (mathVecMagnitude(d, 3) > epsilon)
//         memcpy(stay_put_position, position, 3 * sizeof(float));
    
//     memcpy(target_position, stay_put_position, 3 * sizeof(float));

// }

void collect_memory_pack(int id) {
    
    // Memory pack location is [-0.5, +/- 0.6, 0.0]
    
}

void align_with_POI(int POI_id, bool outer) {
    
    // Should be vector from POI to center of asteroid.
    float POI_location[3];
    game.getPOILoc(POI_location, POI_id);
    // debug_vector("POI", POI_location);
    
    // Attitude
    mathVecSubtract(target_attitude, center_of_asteroid, POI_location, 3);
    
    // Position
    // Make POI_location a unit vector so we can multiply and stuff.
    normalize_vector(POI_location);
    // debug_vector("normalized POI", POI_location);
    float distance = outer ? 0.51f : 0.38f;
    scale_vector(POI_location, distance);
    memcpy(target_position, POI_location, 3 * sizeof(float));
    
}

void go_to_shadow_zone() {
    
    // Aim for some point in the shadow zone.
    // May need to go around asteroid.

}

void go_to_upload() {
    
    float radius = 0.57f;

    memcpy(target_position, position, 3 * sizeof(float));
    
    normalize_vector(target_position);
    scale_vector(target_position, radius);
    override_asteroid_avoidance = true;
    
}

void go_to_neutral_position() {
    
    target_position[0] = -0.5f;
    target_position[1] = 0.0f;
    target_position[2] = 0.0f;
    
}

int closest_POI(bool fancy, bool us, int ignore_id, bool debug) {
    
    int min_id = -1;
    float min_value = 100000;
    
    int id;
    for (id = 0; id < game.numActivePOIs(); id++) {
        
        float POI_loc[3];
        game.getPOILoc(POI_loc, id);
        
        // if (debug) DEBUG(("checking %i\n", id));
        
        if (fancy && ((POI_status[id][0] && POI_status[id][1]) || id == ignore_id)) {
            
            // if (debug) DEBUG(("continue %i\n", id));
            continue;
            
        }
            
        float d[3];
        float mag;
        
        if (!us) {
            
            float enemy_data[12];
            api.getOtherZRState(enemy_data);
            
            float enemy_position[3];
            enemy_position[0] = enemy_data[0];
            enemy_position[1] = enemy_data[1];
            enemy_position[2] = enemy_data[2];
            
            mathVecSubtract(d, enemy_position, POI_loc, 3);
            
        } else {
            
            mathVecSubtract(d, position, POI_loc, 3);
            
        }
        
        mag = mathVecMagnitude(d, 3);
        
        if (mag < min_value) {
            
            min_id = id;
            min_value = mag;
            // if (debug) DEBUG(("choose %i,%i\n", id, min_id));
        
        }
        
    }
    
    // if (debug) DEBUG(("ret %i\n", min_id));
    return min_id;

}

int locate_best_POI(bool fancy) {
    
    int closest_id = closest_POI(fancy, true, -1, false);
    
    float POI_loc[3];
    game.getPOILoc(POI_loc, closest_id);
    
    if (fancy) {
        
        // Figure out if we should ignore the closest POI.
        int enemy_closest_id = closest_POI(fancy, false, -1, false);
        
        // DEBUG(("their poi: %i\n", enemy_closest_id));
        
        if (closest_id == enemy_closest_id) {
            
            float enemy_data[12];
            api.getOtherZRState(enemy_data);
            
            float enemy_position[3];
            enemy_position[0] = enemy_data[0];
            enemy_position[1] = enemy_data[1];
            enemy_position[2] = enemy_data[2];
            
            if (vector_distance(position, POI_loc) > vector_distance(enemy_position, POI_loc)) {
                
                // Second closest POI.
                closest_id = closest_POI(fancy, true, closest_id, true);
                // DEBUG(("avoid %i\n", closest_id));
                
                if (closest_id == -1)
                {
                    
                    // DEBUG(("no choice\n"));
                    closest_id = closest_POI(fancy, true, -1, false);
                    
                    if (closest_id == -1) {
                        
                        closest_id = closest_POI(false, true, -1, false);
                        
                    }
                    
                }
                
            }
        
        }
    
    }
    
    return closest_id;

}

bool outer(int POI_id) {
    
    if (!POI_status[POI_id][0])
        return false;
    else if (!POI_status[POI_id][1])
        return true;
    else // idk
        return true;
    
}

void reset_POI_status() {
    
    POI_status[0][0] = false;
    POI_status[0][1] = false;
    POI_status[1][0] = false;
    POI_status[1][1] = false;
    POI_status[2][0] = false;
    POI_status[2][1] = false;
    
}
//End page movement
//Begin page notes
/*

DOCUMENTATION

State:
float position[3];
float velocity[3];
float attitude[3];
float rotation[3];
void update_state();
float distance_from_center();
float distance_from_surface();
bool line_of_sight(int POI_id);
int closest_POI();
bool in_shadow_zone();

Timing:
int get_next_POI_update();

Movement:
float target_position[3];
float target_attitude[3];
void set_targets();
void collect_memory_pack(int id);
void align_with_POI(int POI_id, bool outer);
void go_to_shadow_zone();


Items to consider:
Trying to stay in bounds when very close to being out-of-bounds

Strategy for deciding which POI to go to:
AVOIDING GOING TO THE SAME POI AS OPPONENT
run the following constantly:
    IF we are closer to (distance less than) closest POI than opponent
        then go to closest POI
    ELSE
        go for second closest POI


*/
//End page notes
//Begin page state
float center_of_asteroid[3];
float position[3];
float velocity[3];
float attitude[3];
float rotation[3];
float score;

// first is POI id, second is no_attempt:0, inner:1, outer:2
int attempted_pic[2];

typedef enum {

    DANGER_ZONE,
    ALMOST_DANGER_ZONE,
    INNER_ZONE,
    OUTER_ZONE,
    UPLOAD_ZONE,
    ALMOST_OUT_OF_BOUNDS_ZONE,
    IDK_BRO

} zone_t;

void state_init() {
    
    center_of_asteroid[0] = 0.0f;
    center_of_asteroid[1] = 0.0f;
    center_of_asteroid[2] = 0.0f;
    
    score = game.getScore();
    
    attempted_pic[0] = 0;
    attempted_pic[1] = 0;
    
}

void update_state() {
    
    float state[12];
    api.getMyZRState(state);
    
    position[0] = state[0];
    position[1] = state[1];
    position[2] = state[2];
    
    velocity[0] = state[3];
    velocity[1] = state[4];
    velocity[2] = state[5];
    
    attitude[0] = state[6];
    attitude[1] = state[7];
    attitude[2] = state[8];
    
    rotation[0] = state[9];
    rotation[1] = state[10];
    rotation[2] = state[11];
    
}

float distance_from_center() {
    
    float d[3];
    
    mathVecSubtract(d, position, center_of_asteroid, 3);
    
    return mathVecMagnitude(d, 3);

}

float distance_from_surface() {
    
    // Asteroid radius is 0.2
    
    return distance_from_center() - 0.2;

}

float distance_to_target_position() {
    
    float d[3];
    mathVecSubtract(d, position, target_position, 3);
    return mathVecMagnitude(d, 3);
    
}

bool line_of_sight(int POI_id) {
    
    // for now
    return game.alignLine(POI_id);

    
}

bool in_shadow_zone() {
    
    /*
      Shadow zone dimensions:
       0.0 x 0.64
      -0.2 x 0.2
      -0.2 x 0.2
    */
    return (in_range(position[0], 0.0, 0.64) && in_range(position[1], -0.2, 0.2) && in_range(position[2], -0.2, 0.2));

}

bool worth_movement_to(float thing[3]) {
    
    // This needs a lot of work. 
    // I basically just hashed it out in a quick attemp to
    // stop our SPHERES from moving away from its POI in the
    // first 60s window.
    
    float scalar = 15.0f; // arbitrary, theoretically time it takes us to move one unit
    float max_distance = 0.2f; // max distance from POI where we are not allowed to say it's not worth it
    
    float d[3];
    mathVecSubtract(d, thing, position, 3);
    float mag = mathVecMagnitude(d, 3);
    
    return (((mag * scalar) < get_next_POI_update()) && (mag > max_distance));

}

bool worth_movement_to_POI(int POI_id)
{
    
    float loc[3];
    game.getPOILoc(loc, POI_id);

    return worth_movement_to(loc);
    
}

zone_t zone() {
    
    // First check if the SPHERE is almost out of bounds.
    // It is whenever the SPHERE is 0.02 from the edge of the playing field.
    // We will define this as six rectangular prisms.
    // -0.64x-0.80x-0.64 to 0.64x-0.78x0.64
    // -0.64x-0.80x-0.64 to -0.62x0.80x0.64
    // -0.64x-0.80x-0.64 to 0.64x0.80x-0.62
    // 0.64x0.80x0.64 to 0.62x-0.80x-0.64
    // 0.64x0.80x0.64 to -0.64x-0.80x0.62
    // 0.64x0.80x0.64 to -0.64x0.78x-0.64
    float distance_from_edge = 0.04;
    if (in_prism(position[0], position[1], position[2],
                 -0.64, 0.64, -0.8, -0.8 + distance_from_edge, -0.64, 0.64) ||
        in_prism(position[0], position[1], position[2],
                 -0.64, -0.64 + distance_from_edge, -0.8, 0.8, -0.64, 0.64) ||
        in_prism(position[0], position[1], position[2],
                 -0.64, 0.64, -0.8, 0.8, -0.64, -0.64 + distance_from_edge) ||
        in_prism(position[0], position[1], position[2],
                 0.64 - distance_from_edge, 0.64, -0.8, 0.8, -0.64, 0.64) ||
        in_prism(position[0], position[1], position[2],
                 -0.64, 0.64, -0.8, 0.8, 0.64 - distance_from_edge, 0.64) ||
        in_prism(position[0], position[1], position[2],
                 -0.64, 0.64, 0.8 - distance_from_edge, 0.8, -0.64, 0.64))
        return ALMOST_OUT_OF_BOUNDS_ZONE;
    
    if (distance_from_center() < 0.31) {
        return DANGER_ZONE;
    } else if (in_range(distance_from_center(), 0.31, 0.33)) {
        return ALMOST_DANGER_ZONE;
    } else if (in_range(distance_from_center(), 0.33, 0.42)) {
        return INNER_ZONE;
    } else if (in_range(distance_from_center(), 0.42, 0.53)) {
        return OUTER_ZONE;
    } else if (distance_from_center() > 0.53) {
        return UPLOAD_ZONE;
    } else {
        
        DEBUG(("HELP! zone() fails\n"));
        return IDK_BRO;
        
    }
    
}
//End page state
//Begin page timing
int get_next_POI_update() {
    
    return 60 - (api.getTime() % 60);

}
//End page timing
//Begin page vectors
void normalize_vector(float vec[3]) {
    
    float mag = sqrtf(mathSquare(vec[0]) + mathSquare(vec[1]) + mathSquare(vec[2]));
    scale_vector(vec, (1 / mag));
    // mathVecNormalize(vec, 3);

}

void scale_vector(float vec[3], float scale) {
    
    vec[0] *= scale;
    vec[1] *= scale;
    vec[2] *= scale;
    
}

void debug_vector(char *name, float x[3]) {
    
    DEBUG(("%s: %f %f %f\n", name, x[0], x[1], x[2]));
    
}

float vector_distance(float position[3], float other[3]) {
    
    float d[3];
    mathVecSubtract(d, position, other, 3);
    return mathVecMagnitude(d, 3);
    
}
//End page vectors
