#include "stackcomm.hpp"

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

Position& operator++( Position &c ) {
  using IntType = typename std::underlying_type<Position>::type;
  c = static_cast<Position>( static_cast<IntType>(c) + 1 );
  if ( c == Position::BugSix )
    c = static_cast<Position>(0);
  return c;
}

Position operator++( Position &c, int ) {
  Position result = c;
  ++c;
  return result;
}

class RigController{
    public:
        int OpenStack();

        Position nextPosition();

        Position returnToStart();

        int CloseStack();

        int getState(){ return this->state;}

    private:
        int stackPort = -1;

        Position state = CardOne;

        const float CELL_LENGTH = 3.5; //revolutions per cell
        const float CELL_HEIGHT = 2.1; //revolutions per cell
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

Position RigController::nextPosition(){
    std::vector<Position> upDiagVec = {BugOne, BugTwo, BugFour, BugFive};
    std::vector<Position> downVec = {CardOne, CardTwo, CardThree, CardFour, CardFive, CardSix};

    auto upDiagIt = std::find(upDiagVec.begin(), upDiagVec.end(), this->state);
    auto downIt = std::find(downVec.begin(), downVec.end(), this->state);
    if(upDiagIt != upDiagVec.end()){
        //Move up and to the right one box
        stackMoveDiagonal(this->stackPort, CELL_LENGTH, 1.0, 0, CELL_HEIGHT, 0.6, 0);
    } else if (downIt != downVec.end()){
        //Move down one box
        stackMove(this->stackPort, CELL_HEIGHT, 1.0, 1, 1);
    } else if (state == BugThree){
        //Move down one and left two boxes
        stackMoveDiagonal(this->stackPort, CELL_LENGTH*2, 1, 1, CELL_HEIGHT, 0.3, 1);
    } else if (state == BugSix){
        //Move up three and left two boxes
        stackMoveDiagonal(this->stackPort, CELL_LENGTH * 2, 1.0, 0, CELL_HEIGHT * 3, 0.9, 1);
    }

    this->state ++;
    return this->state;
}

Position RigController::returnToStart(){

    switch(this->state){
        case CardOne:
            break;
        case BugOne:
            //Move up one
            stackMove(this->stackPort, CELL_HEIGHT, 1.0, 0, 1);
            break;
        case CardTwo:
            //Move left one
            stackMove(this->stackPort, CELL_LENGTH, 1.0, 1, 0);
            break;
        case BugTwo:
            //Move up one and left one
            stackMoveDiagonal(this->stackPort, CELL_LENGTH, 1.0, 0, CELL_HEIGHT, 0.6, 1);
            break;
        case CardThree:
            //Move left two
            stackMove(this->stackPort, CELL_LENGTH*2, 1.0, 1, 0);
            break;
        case BugThree:
            //Move up one and left two
            stackMoveDiagonal(this->stackPort, CELL_LENGTH*2, 1.0, 0, CELL_HEIGHT, 0.3, 1);
            break;
        case CardFour:
            //Move up two
            stackMove(this->stackPort, CELL_HEIGHT*2, 1.0, 0, 1);
            break;
        case BugFour:
            //Move up three
            stackMove(this->stackPort, CELL_HEIGHT*3, 1.0, 0, 1);
            break;
        case CardFive:
            //Move up two and left one
            stackMoveDiagonal(this->stackPort, CELL_LENGTH, 0.83, 0, CELL_HEIGHT*2, 1.0, 1);
            break;
        case BugFive:
            //Move up three and left one
            stackMoveDiagonal(this->stackPort, CELL_LENGTH, 0.55, 0, CELL_HEIGHT*3, 1.0, 1);
            break;
        case CardSix:
            //Move up two and left two
            stackMoveDiagonal(this->stackPort, CELL_LENGTH*2, 1.0, 0, CELL_HEIGHT*2, 0.59, 1);
            break;
        case BugSix:
            //Move up three and left two
            stackMoveDiagonal(this->stackPort, CELL_LENGTH*2, 1.0, 0, CELL_HEIGHT*3, 0.87, 1);
            break;
    }

    this->state = CardOne;
    return this->state;
}