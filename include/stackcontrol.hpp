#include "include/stackcomm.hpp"

class StackController{
    public:
        enum Position{
            CardOne,
            BugOne,
            CardTwo,
            BugTwo,
            CardThree,
            BugThree,
            CardFour,
            BugFour,
            CardFive,
            BugFive,
            CardSix,
            BugSix
        };

        int OpenStack();

        

        Position nextPosition();

        int CloseStack();

    private:
        int stackPort = -1;

        Position state = CardOne;
};

StackController::OpenStack(){
    int serial_port = stackOpen();

    if (serial_port < 0) return -1;

    this-> stackPort = serial_port;

    return serial_port;
}

StackController::CloseStack() {
    stackClose(this->stackPort);
}

Position nextPosition(){
    std::vector<Position> upDiagVec = {BugOne, BugTwo, BugFour, BugFive};
    std::vector<Postion> downVec = {CardOne, CardTwo, CardThree, CardFour, CardFive, CardSix};

    auto upDiagIt = std::find(upDiagVec.begin(), upDiagVec.end(), this->state);
    auto downIt = std::find(downVec.begin(), downVec.end(), this->state);
    if(upDaigIt != upDiagVec.end()){
        //Move up and to the right one box
    } else if (downIt != downVec.end()){
        //Move down one box
    } else if (state == BugThree){
        //Move down one and left two boxes
    } else if (state == BugSix){
        //Move up three and left two boxes
    }
}