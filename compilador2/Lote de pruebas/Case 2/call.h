struct call{
    int floor;
    int delay;//=0
    int elevatorCalled;
    int active;//=0
    int floorDest;
    int goingToDest;
    int number;


};

void createCall(struct call *call,int floor,int elevatorCalled,int floorDest,int number){
    call->active=1;
    call->floor=floor;
    call->delay=0;
    call->elevatorCalled=elevatorCalled;
    call->floorDest=floorDest;
    call->goingToDest=0;
    call->number=number;
}
