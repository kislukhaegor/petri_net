#include "petri_net.hpp"

PetriNet::PetriNet(const Matrix<MarkType>& aMatrix,
                   const std::vector<MarkType>& aProcessInitState,
                   const std::vector<size_t>& aTransients)
: PetriNetBase(aMatrix, aProcessInitState)
, m_ProcessRows(aMatrix.rowsCnt())
{
    generateController(aTransients);
    generateInitState(aTransients);
}

void PetriNet::regenerateController(const std::vector<size_t>& aTransients)
{
    m_Matrix.dropLastRows(m_Matrix.rowsCnt() - m_ProcessRows);
    generateController(aTransients);
    generateInitState(aTransients);
}

bool PetriNetBase::nextState()
{
    for (size_t t = 0; t < m_Matrix.colsCnt(); ++t)
    {
        if (isAvailable(t))
        {
            applyTransition(t);
            return true;
        }
    }
    return false;
}

bool PetriNet::registerEvents(const std::vector<EventType>& sEvents)
{
    if (sEvents.size() != m_Matrix.colsCnt())
        return false;
    m_EventsMap = sEvents;
    m_Matrix.addRows(sEvents.size());
    for (size_t i = 0; i < sEvents.size(); ++i)
        m_Matrix(i + m_ControllerRows + m_ProcessRows, i) = sEvents[i] > CONTROLLABLE ? -1 : 1;
    m_State.resize(m_Matrix.rowsCnt());
    return true;
}

void PetriNet::applyEvent(size_t sEvent)
{
    if (sEvent < m_Matrix.rowsCnt())
    {
        m_State[m_ProcessRows + m_ControllerRows + sEvent] += 1;
    }
}

void PetriNet::generateInitState(const std::vector<size_t>& aTransients)
{
    std::vector<int64_t> sParikh(m_Matrix.colsCnt(), -1);
    for (auto t : aTransients)
        ++sParikh[t];
    std::vector<MarkType> sControllerState(m_Matrix.rowsCnt() - m_ProcessRows, 0);
    for (size_t i = 0; i < m_Matrix.rowsCnt(); ++i)
    {
        for (size_t j = 0; j < sControllerState.size(); ++j)
        {
            sControllerState[i] -= m_Matrix(i + m_ProcessRows, j) * sParikh[j];
        }
    }
    std::copy(sControllerState.begin(), sControllerState.end(), std::back_inserter(m_State));
    m_ControllerRows = sControllerState.size();
}

void PetriNet::generateController(const std::vector<size_t>& aTransients)
{
    size_t i = 0;
    while (i < aTransients.size() - 1)
    {
        size_t currentTransient = aTransients[i];
        size_t nextTransient = aTransients[i + 1];
        MarkType prodResult = 0;
        for (size_t j = 0; j < m_Matrix.rowsCnt(); ++j)
            prodResult += m_Matrix(j, currentTransient) * m_Matrix(j, nextTransient);
        if (0 == prodResult)
        {
            m_Matrix.addRows();
            m_Matrix(m_Matrix.rowsCnt() - 1, currentTransient) = 1;
            m_Matrix(m_Matrix.rowsCnt() - 1, nextTransient) = -1;
        }
        else
        {
            ++i;
        }
    }
}

std::vector<PetriNet::MarkType> PetriNet::popEvents()
{
    std::vector<MarkType> sEvents;
    sEvents.reserve(m_Matrix.colsCnt());
    for (size_t i = 0; i < m_Matrix.colsCnt(); ++i)
    {
        auto& sEvent = m_State[i + m_ControllerRows + m_ProcessRows];
        if (sEvent != 0 && m_EventsMap[i] == CONTROLLABLE)
        {
            sEvent = 0;
            sEvents.push_back(i);
        }
    }
    return sEvents;
}
