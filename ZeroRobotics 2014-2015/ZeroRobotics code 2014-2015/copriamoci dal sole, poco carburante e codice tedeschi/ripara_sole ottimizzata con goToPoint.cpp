void uno(){
        float pos_1[3]={0.43,0,0};
        if(isGoodVector(pos_1)==true){
            goToPoint(pos_1,0.04);
        }else{
            float pos_11[3]={0.40,0.40,0};
            goToPoint(pos_11,0.04);
        }
        
    }
    void due(){
        float pos_2[3]={0.5,0.5,0};
        api.setPositionTarget(pos_2);
    }
    void tre(){
        float pos_3[3]={0.5,-0.5,0};
        api.setPositionTarget(pos_3);
        DEBUG(("TRE"));
    }
    void quattro(){
        float pos_4[3]={0.43,0,0};
        if(isGoodVector(pos_4)==true){
            goToPoint(pos_4,0.04);
        }else{
            float pos_44[3]={-0.40,-0.40,0};
            goToPoint(pos_44,0.04);
        }
    }
void ripara_sole(){
    float pos_1[3]={0.43,0,0};
    float stato[12];
    api.getMyZRState(stato);
    if(stato[0]<0 && stato[1]<0){
        tre();
    }else if(stato[0]>0 && stato[1]>0){
        uno();
    }else if(stato[0]>0 && stato[1]<0){
        due();
    }else if(stato[0]<0 && stato[1]>0){
        quattro();
    }
}
