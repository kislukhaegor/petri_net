#include "petri_net_base.hpp"

PetriNetBase::PetriNetBase(const Matrix<MarkType>& sMatrix, const std::vector<MarkType>& sInitState)
: m_Matrix(sMatrix)
, m_State(sInitState)
{}

void PetriNetBase::applyTransition(size_t sTransition)
{
    for (size_t i = 0; i < m_Matrix.rowsCnt(); ++i)
    {
        m_State[i] += m_Matrix(i, sTransition);
    }
}

bool PetriNetBase::isAvailable(size_t aTransition) const
{
    for (size_t i = 0; i < m_Matrix.rowsCnt(); ++i)
        if (m_Matrix(i, aTransition) < 0 && -m_Matrix(i, aTransition) > m_State[i])
            return false;
    return true;
}

const std::vector<PetriNetBase::MarkType>& PetriNetBase::state() const noexcept
{
    return m_State;
}


std::ostream& operator<<(std::ostream& aOs, const PetriNetBase& aNet)
{
    return aOs << aNet.m_Matrix;
}
