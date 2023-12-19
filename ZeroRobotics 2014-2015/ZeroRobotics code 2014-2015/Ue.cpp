//Begin page functions
int getQuadranteAvversario(){
    float stato_avversario[12];
    int n_quadrante=0;
    api.getOtherZRState(stato_avversario);
    if(stato_avversario[0]>0 && stato_avversario[1]>0){
        n_quadrante=1;
    }
    if(stato_avversario[0]<0 && stato_avversario[1]>0){
        n_quadrante=2;
    }
    if(stato_avversario[0]<0 && stato_avversario[1]<0){
        n_quadrante=3;
    }
    if(stato_avversario[0]>0 && stato_avversario[1]<0){
        n_quadrante=4;
    }
    return n_quadrante;
}
int getQuadrante(){
    float stato[12];
    int n_quadrante=0;
    api.getMyZRState(stato);
    if(stato[0]>0 && stato[1]>0){
        n_quadrante=1;
    }
    if(stato[0]<0 && stato[1]>0){
        n_quadrante=2;
    }
    if(stato[0]<0 && stato[1]<0){
        n_quadrante=3;
    }
    if(stato[0]>0 && stato[1]<0){
        n_quadrante=4;
    }
    return n_quadrante;
}
void getMiddleCoordinates(float point1[3], float point2[3], float *middlePoint){
    middlePoint[0]=((point1[0]+point2[0])/2);
    middlePoint[1]=((point1[1]+point2[1])/2);
    middlePoint[2]=((point1[2]+point2[2])/2);
}
float getDistance(float a[], float b[]){
	float lol = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return lol;
}
int dist(double dX0, double dY0, double dX1, double dY1){
    return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
}
bool isGoodVector(float point[]){
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
   if(distanza1>=0.33 && distanza2>=0.33 && distanza3>=0.33 && distanza4>=0.33 && distanza5>=0.33){
      is_good=true;
   }else{
      is_good=false;
   }
   // DEBUG(("d1: %f d2: %f d3: %f \n ",distanza1,distanza2,distanza3));
   // DEBUG(("p1 x: %f p2 y: ",_point1[0],_point1[1]));
   return is_good;
}


bool isGoodVector2(float point[],float coordinate_asteroide[]){
   float stato[12];
   api.getMyZRState(stato);
   bool is_good=false;
   float middlePoint[3];
   float _point1[3];
   float _point2[3];
   float distanza1,distanza2,distanza3,distanza4,distanza5;
   getMiddleCoordinates(point,stato,middlePoint);
   getMiddleCoordinates(middlePoint,stato,_point1);
   getMiddleCoordinates(point,middlePoint,_point2);
   distanza1=getDistance(_point1,coordinate_asteroide);
   distanza2=getDistance(middlePoint,coordinate_asteroide);
   distanza3=getDistance(_point2,coordinate_asteroide);
   distanza4=getDistance(point,coordinate_asteroide);
   distanza5=getDistance(stato,coordinate_asteroide);
   if(distanza1>=0.33 && distanza2>=0.33 && distanza3>=0.33 && distanza4>=0.33 && distanza5>=0.33){
      is_good=true;
   }else{
      is_good=false;
   }
   // DEBUG(("d1: %f d2: %f d3: %f \n ",distanza1,distanza2,distanza3));
   // DEBUG(("p1 x: %f p2 y: ",_point1[0],_point1[1]));
   return is_good;
}
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
//End page Funzioni
//End page functions
//Begin page main

//Begin page main
float myState[12];
float myPos[3];
float itemPos[2][3];
float itemSetPos[2][3];
float vectorBetween[3];
int counter;
bool posi;
int step;
int nPhoto;
int temp;
int solarFlare;
float stepPosition[3];
void init(){
    for(int i=0;i<3;i++){
	    itemPos[0][i]=0.0;
	    itemPos[1][i]=0.0;
	}
	counter=0;
	step=0;
	posi=true;
	temp=0;
	stepPosition[0]=-0.40;
	stepPosition[1]=0;
	stepPosition[2]=0;
}

void loop(){
    if(game.getMemoryFilled()>=1){
        game.uploadPic();
    }
	getMyPos();
	if((counter==0)||((counter-1)%60==0)){
	    //GET INFOs ABOUT POIs
	    game.getPOILoc(itemPos[0],0);
    	game.getPOILoc(itemPos[1],1);
	    DEBUG(("- 1 o %f -2 o %f o",itemPos[0][1],itemPos[1][1]));
	    
	    //get positions for taking photos
	    /*for(int i=0;i<2;i++){
	        findPos(i);
	    }
	    */
	    for(int i=0;i<2;i++){
            itemSetPos[i][0]=-1*(0.39 / sqrt(1+pow(itemPos[i][1]/itemPos[i][0],2)));
            itemSetPos[i][1]=itemSetPos[i][0]*(itemPos[i][1]/itemPos[i][0]);
            itemSetPos[i][2]=0;
        }
	    step=1;
	}
	//if we haven't take photos
	if((step>=0)&&(temp==0)){
	    //calculate attitude for aligning to the POI
	    mathVecSubtract(vectorBetween,itemPos[step],myPos,3);
	    mathVecNormalize(vectorBetween,3);
	    api.setAttitudeTarget(vectorBetween);
	    //set position aligned to the poi
    	//goToPoint(itemSetPos[step],1);
    	if(isGoodVector(itemSetPos[step])==true){
    	setConstantVelocityTo(itemSetPos[step],0.02);
    	}else{
    	    setConstantVelocityTo(stepPosition,0.02);
    	}
    	//DEBUG(("in movimento %d %d",step,counter));
    	//posi=game.alignLine();
    	//if we are aligned
    	posi=alignPOI();
    	//posi=game.alignLine();
    	if(posi==true){
    	    DEBUG(("ON LINE"));
    	    game.takePic(step);
	        nPhoto=game.getMemoryFilled();
	        DEBUG(("%d photos\n",nPhoto));
    	    step--;
    	}
	}
	else if(temp==0){
	    game.uploadPic();
	    getMyPos();
	    setConstantVelocityTo(myPos,0.02);
	    
	    temp=counter;
	}else{
	    if((counter-temp)==3){
	        nPhoto=game.getMemoryFilled();
	        DEBUG(("%d photos\n",nPhoto));
	        temp=0;
	    }
	}
	counter++;
	solarFlare=game.getNextFlare();
	if(solarFlare!=-1)
	DEBUG(("Next solar flare: %d s",solarFlare));
}

//-------------------------------------------------------------


bool alignPOI(){
	/*se la torsione del nostro satellite
	non e' uguale alla torsione di setAttitude
	restituisce false altrimenti true*/
	
    bool paciocco=false; 		//variabile di stato
    float myAttitude[3]; 		//torsione del nostro satellite
    api.getMyZRState(myState);
	for(int i=0;i<3;i++){
	    myAttitude[i]=myState[i+6];
	}
	for(int i=0;i<3;i++){
	    myPos[i]=myState[i];
	}
	
	if(
	    (simili(myAttitude[0],vectorBetween[0]))
	&&  (simili(myAttitude[1],vectorBetween[1]))
	&&  (simili(myPos[0],itemSetPos[step][0]))
	&&  (simili(myPos[1],itemSetPos[step][1]))
	){
	    paciocco=true;
    }
	return paciocco;
}

//-------------------------------------------------------------

bool simili(float a, float b){
    float risultato;
    risultato=a-b;
    if((risultato<0.05)&&(risultato>-0.05)){
        return true;
    }else{
        return false;
    }
    
}

//-------------------------------------------------------------



//-------------------------------------------------------------

void getMyPos(){
    api.getMyZRState(myState);
	for(int i=0;i<3;i++){
	    myPos[i]=myState[i];
	}
}

//-------------------------------------------------------------

void findPos(int i){
    float xo,yo,t0,t;
    
    xo=itemPos[i][0];
    yo=itemPos[i][1];

    t0=yo/xo;
    t=sqrt(1+(t0*t0));
    itemSetPos[i][0]=0.65/t;
    itemSetPos[i][1]=itemSetPos[i][0]*t0;
    itemSetPos[i][0]=itemSetPos[i][0]*-1;
    
    
}
//End page main

//End page main
