#include "stackcomm.hpp"

enum Position{
    PhotoOne,
    PhotoTwo,
    PhotoThree,
    PhotoFour
};

Position& operator++( Position &c ) {
  using IntType = typename std::underlying_type<Position>::type;
  if ( c == Position::PhotoFour )
    c = static_cast<Position>(0);
  else
    c = static_cast<Position>( static_cast<IntType>(c) + 1 );
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

        std::string positionMap[4] = {"PhotoOne","PhotoTwo","PhotoThree","PhotoFour"};

    private:
        int stackPort = -1;

        Position state = PhotoOne;

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
    
    if (state == PhotoFour){
        this->returnToStart();
        return this->state;
    }

    stackMove(this->stackPort, 0.25, 1.0, 1, 0);
    this->state ++;
    return this->state;
}

Position RigController::returnToStart(){

    switch(this->state){
        case PhotoOne:

        break;
        case PhotoTwo:
        stackMove(this->stackPort, 0.25, 1.0, 0, 0);
        break;
        case PhotoThree:
        stackMove(this->stackPort, 0.5, 1.0, 1, 0);
        break;
        case PhotoFour:
        stackMove(this->stackPort, 0.25, 1.0, 1, 0);
        break;
    }

    this->state = PhotoOne;
    return this->state;
}