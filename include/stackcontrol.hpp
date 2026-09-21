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

StackController::CloseStack()