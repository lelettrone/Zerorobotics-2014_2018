//Begin page main

       //---------------------//
      // Giove  (0;0.5)      //
     // Venere (0.5;0)      //
    // Marte  (0;-0.5)    //
   // Satutno (-0.5;0)  //
  //__________________//
float position[13];
float force1[3];//velocità da giove a venere
float force2[3];//velocità da venere a arte
float force3[3];//forza per raggiungere giove passando per saturno
bool touch_v;//quando ha valore alto viene applicata force2
bool touch_m;//quando ha valore alto viene applicata force3
float dist_v;//distanza da venere
float dist_m;//distanza da marte
float dist_s;//distanza da saturno
float dist_g;//distanza da giove

void init(){
	force1[0]=0.25;
    force1[1]=-0.25;
    force1[2]=0;
    
    force2[0]=-0.425;
    force2[1]=0.25;
    force2[2]=0;
    
    force3[0]=1;
    force3[1]=0.10;
    force3[2]=0;
    
    touch_m=false;
    touch_v=false;
}

void loop(){
    api.getMySphState(position);
    
    dist_v=(position[0]-0.5)*(position[0]-0.5)+(position[1]-0)*(position[1]-0);
    if(dist_v<0)
        dist_v=dist_v*-1;//valore assoluto
    dist_v=sqrt(dist_v);//calcolcolo distanza da venere
    if(dist_v<0.05){//margine di errore
     DEBUG(( "Colpito Venere !!"));
     touch_v=true;
    }
    
    dist_m=(position[0]-0)*(position[0]-0)+(position[1]+0.5)*(position[1]+0.5);
    if(dist_m<0)
        dist_m=dist_m*-1;//valore assoluto
    dist_m=sqrt(dist_m);//calcolcolo distanza da marte
    
    if(dist_m<0.20){ //la forza viene applicata prima dell' impattocon marte
     touch_m=true;
     touch_v=false;
     
    }
    if(dist_m<0.05){//margine di errore
     DEBUG(( "Colpito Marte!!"));
    }
    
    dist_s=(position[0]+0.5)*(position[0]+0.5)+(position[1]+0)*(position[1]+0);
    if(dist_s<0)
        dist_s=dist_s*-1;//valore assoluto
    dist_s=sqrt(dist_s);//calcolcolo distanza da saturno
    if(dist_s<0.05){//margine di errore
     DEBUG(( "Colpito Saturno!!"));
     
    }
   
    dist_g=(position[0]-00)*(position[0]-0)+(position[1]-0.5)*(position[1]-0.5);
    if(dist_g<0)
        dist_g=dist_g*-1;//valore assoluto
    dist_g=sqrt(dist_g);//calcolcolo distanza da giove
    
    if(dist_g<0.05){//margine di errore
     DEBUG(( "Colpito Giove!!"));
     
    }
    
    
    if(touch_v){
        api.setVelocityTarget(force2);    
    }
    
    if(touch_m){
	    api.setForces(force3);
    } 
	   
	   
	 if(touch_m==false&&touch_v==false){
	    
	  api.setVelocityTarget(force1);
    }
    
}
//End page main
