#pragma once

#include <ostream>
#include <vector>

#include "events.hpp"
#include "matrix.hpp"

class PetriNetBase
{
public:
    using MarkType = int64_t;

    PetriNetBase(const Matrix<MarkType>& sMatrix, const std::vector<MarkType>& sInitState);

    const std::vector<MarkType>& state() const noexcept;

    bool nextState();

    friend std::ostream& operator<<(std::ostream& aOs, const PetriNetBase& aNet);

protected:
    Matrix<MarkType> m_Matrix;
    std::vector<MarkType> m_State;

private:
    bool isAvailable(size_t aTransition) const;

    void applyTransition(size_t sTransition);

};

std::ostream& operator<<(std::ostream& aOs, const PetriNetBase& aNet);
