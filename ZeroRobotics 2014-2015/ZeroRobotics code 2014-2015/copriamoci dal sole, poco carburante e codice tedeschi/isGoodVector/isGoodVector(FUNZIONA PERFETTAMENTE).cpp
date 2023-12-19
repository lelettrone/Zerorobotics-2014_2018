bool isGoodVector(float point[3]){
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
