#pragma once

#include <ostream>
#include <vector>

#include "events.hpp"
#include "matrix.hpp"
#include "petri_net_base.hpp"

class PetriNet : public PetriNetBase
{
public:

    PetriNet(const Matrix<MarkType>& sMatrix,
             const std::vector<MarkType>& aProcessInitState,
             const std::vector<size_t>& aTransients);

    void regenerateController(const std::vector<size_t>& aTransients);

    bool registerEvents(const std::vector<EventType>& sEvents);

    void applyEvent(size_t sEvent);

    std::vector<MarkType> popEvents();

private:
    size_t m_ProcessRows;
    size_t m_ControllerRows = 0;
    EventsMap m_EventsMap;

    void generateInitState(const std::vector<size_t>& aTransients);

    void generateController(const std::vector<size_t>& aTransients);
};
