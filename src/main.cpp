#include <algorithm>
#include <iostream>
#include <vector>

#include "matrix.hpp"
#include "petri_net.hpp"
#include "events.hpp"


int main(/*int argc, char* argv[]*/)
{
    std::vector<EventType> sEvents{CONTROLLABLE, EXPECTED, CONTROLLABLE, EXPECTED,
                                   CONTROLLABLE, EXPECTED, CONTROLLABLE, EXPECTED};
    Matrix<PetriNet::MarkType> sMatr(std::vector<PetriNet::MarkType>{
        -1,  0,  0,  1,  0,  0,  0,  0,
         1, -1,  0,  0,  0,  0,  0,  0,
         0,  1, -1,  0,  0,  0,  0,  0,
         0,  0,  1, -1,  0,  0,  0,  0,
         0,  0,  0,  0, -1,  0,  0,  1,
         0,  0,  0,  0,  1, -1,  0,  0,
         0,  0,  0,  0,  0,  1, -1,  0,
         0,  0,  0,  0,  0,  0,  1, -1
        }, 8, 8);
    PetriNet sNet(sMatr, {1, 0, 0, 0, 1, 0, 0, 0}, {0, 1, 4, 5, 2, 3, 6, 7, 0});
    sNet.registerEvents(sEvents);
    std::cout << "Initial state: [ ";
    for (auto el : sNet.state())
        std::cout << el << " ";
    std::cout << "]\n\nPetri net:\n";
    std::cout << sNet << std::endl;
    bool isDone = false;
    while (!isDone)
    {
        while (sNet.nextState())
        {
            std::cout << "Current state : [ ";
            for (auto el : sNet.state())
                std::cout << el << ' ';
            std::cout << ']' << std::endl;
            std::cout << "Output events: [ ";
            auto sOutEvents = sNet.popEvents();
            for (auto sEvent : sOutEvents)
                std::cout << sEvent << ' ';
            std::cout << "]\n";
        }
        size_t sEvent = 0;
        bool isEntered = false;
        while (!isEntered)
        {
            std::cout << "Enter event: ";
            if (std::cin >> sEvent)
            {
                if (sEvent < sEvents.size())
                {
                    if (sEvents[sEvent] > CONTROLLABLE)
                        isEntered = true;
                    else
                        std::cout << "This event is controllable\n";
                }
                else
                {
                    std::cout << "This event is not supported\n";
                }
            }
            else if (!std::cin.eof())
            {
                std::cout << "This is not valid event\n";
                std::cin.clear();
            }
            else
            {
                isDone = true;
                isEntered = true;
            }
        }
        if (!isDone)
        {
            sNet.applyEvent(sEvent);
        }
    }
}
