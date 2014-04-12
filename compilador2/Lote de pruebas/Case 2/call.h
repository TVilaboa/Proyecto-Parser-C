typedef struct {
    int floor;
    int delay;
    int elevatorCalled;
    int active;
    int floorDest;
    int goingToDest;
    int number;


}call;

void createCall( call *call,int floor,int elevatorCalled,int floorDest,int number){
    call->active=1;
    call->floor=floor;
    call->delay=0;
    call->elevatorCalled=elevatorCalled;
    call->floorDest=floorDest;
    call->goingToDest=0;
    call->number=number;
}
