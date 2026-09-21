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

        Position state;

        Position nextPosition();

        int CloseStack();

    private:
        int stackPort;

};