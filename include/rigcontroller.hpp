#include "stackcomm.hpp"

class RigController{
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

        Position returnToStart();

        int CloseStack();

        int getState(){ return this->state;}

    private:
        int stackPort = -1;

        Position state = CardOne;
};

int RigController::OpenStack(){
    int serial_port = stackOpen();

    if (serial_port < 0) return -1;

    this-> stackPort = serial_port;

    return serial_port;
}

int RigController::CloseStack() {
    stackClose(this->stackPort);
    return 0;
}

RigController::Position RigController::nextPosition(){
    std::vector<RigController::Position> upDiagVec = {BugOne, BugTwo, BugFour, BugFive};
    std::vector<RigController::Position> downVec = {CardOne, CardTwo, CardThree, CardFour, CardFive, CardSix};

    auto upDiagIt = std::find(upDiagVec.begin(), upDiagVec.end(), this->state);
    auto downIt = std::find(downVec.begin(), downVec.end(), this->state);
    if(upDiagIt != upDiagVec.end()){
        //Move up and to the right one box
        stackMoveDiagonal(this->stackPort, 3.0, 0.5, 0, 6.0, 1.0, 0);
    } else if (downIt != downVec.end()){
        //Move down one box
        stackMove(this->stackPort, 3.0, 1.0, 1, 0);
    } else if (state == BugThree){
        //Move down one and left two boxes
        stackMoveDiagonal(this->stackPort, 3.0, 0.25, 1, 12.0, 1.0, 1);
    } else if (state == BugSix){
        //Move up three and left two boxes
        stackMoveDiagonal(this->stackPort, 9.0, 0.75, 0, 12.0, 1.0, 1);
    }

    this->state ++;
    return this->state;
}

rigcontroller::Position RigController::returnToStart(){

    switch(this->state){
        case: CardOne
            break;
        case: BugOne
            //Move up one
            stackMove(this->stackPort, 3.0, 1.0, 0, 0);
            break;
        case: CardTwo
            //Move left one
            stackMove(this->stackPort, 6.0, 1.0, 1, 1);
            break;
        case: BugTwo
            //Move up one and left one
            stackMoveDiagonal(this->stackPort, 3.0, 1.0, 0, 6.0, 1.0, 1);
            break;
        case: CardThree
            //Move left two
            stackMove(this->stackPort, 12.0, 1.0, 1, 1);
            break;
        case: BugThree
            //Move up one and left two
            stackMoveDiagonal(this->stackPort, 3.0, 1.0, 0, 12.0, 1.0, 1);
            break;
        case CardFour
            //Move up two
            stackMove(this->stackPort, 6.0, 1.0, 0, 0);
            break;
        case BugFour
            //Move up three
            stackMove(this->stackPort, 9.0, 1.0, 0, 0);
            break;
        case CardFive
            //Move up two and left one
            stackMoveDiagonal(this->stackPort, 6.0, 1.0, 0, 6.0, 1.0, 1);
            break;
        case BugFive
            //Move up three and left one
            stackMoveDiagonal(this->stackPort, 9.0, 1.0, 0, 6.0, 1.0, 1);
            break;
        case CardSix
            //Move up two and left two
            stackMoveDiagonal(this->stackPort, 6.0, 1.0, 0, 12.0, 1.0, 1);
            break;
        case BugSix
            //Move up three and left two
            stackMoveDiagonal(this->stackPort, 9.0, 1.0, 0, 12.0, 1.0, 1);
            break;
    }

    this->state = CardOne;
    return this->state;
}