#pragma once

#include <ostream>
#include <vector>

template <typename T>
class Matrix
{
public:
    explicit Matrix(size_t aRowsCnt = 0, size_t aColsCnt = 0)
            : m_RowsCnt(aRowsCnt), m_ColsCnt(aColsCnt), m_Data(aRowsCnt * aColsCnt)
    {}

    Matrix(const Matrix<T>&) = default;
    Matrix(Matrix<T>&&) noexcept = default;

    Matrix(const std::vector<T>& aData, size_t aRowsCnt, size_t aColsCnt)
            : m_RowsCnt(aRowsCnt), m_ColsCnt(aColsCnt), m_Data(aData)
    {
        m_Data.resize(aRowsCnt * aColsCnt);
    }

    Matrix(std::vector<T>&& aData, size_t aRowsCnt, size_t aColsCnt)
            : m_RowsCnt(aRowsCnt), m_ColsCnt(aColsCnt), m_Data(aData)
    {
        m_Data.resize(aRowsCnt * aColsCnt);
    }

    T rowsCnt() const noexcept { return m_RowsCnt; }
    T colsCnt() const noexcept { return m_ColsCnt; }

    void addRows(size_t aRowsCount = 1)
    {
        m_Data.resize(m_Data.size() + aRowsCount * m_ColsCnt);
        m_RowsCnt += aRowsCount;
    }

    void addCols(size_t aColsCount = 1)
    {
        m_Data.resize(m_Data.size() + m_RowsCnt * aColsCount);
        size_t sColsCountOld = m_ColsCnt;
        m_ColsCnt += aColsCount;
        for (size_t i = m_RowsCnt - 1; i > 0; --i)
        {
            for (size_t j = sColsCountOld; j > 0; --j)
            {
                (*this)(i, j - 1) = m_Data[i * sColsCountOld + j - 1];
            }
            for (size_t j = 1; j <= aColsCount; ++j)
            {
                (*this)(i, m_ColsCnt - j) = 0;
            }
        }
        for (size_t j = 1; j <= aColsCount; ++j)
        {
            (*this)(0, m_ColsCnt - j) = 0;
        }
    }

    void dropLastRows(size_t aRowsCount = 1)
    {
        m_RowsCnt -= aRowsCount;
        m_Data.resize(m_RowsCnt * m_ColsCnt);
    }

    void dropLastCols(size_t aColsCount = 1)
    {
        m_Data.resize(m_Data.size() + m_RowsCnt * aColsCount);
        size_t sColsCountOld = m_ColsCnt;
        m_ColsCnt -= aColsCount;
        for (size_t i = 1; i < m_RowsCnt; ++i)
            for (size_t j = 0; j < m_ColsCnt; ++j)
                (*this)(i, j) = m_Data[i * sColsCountOld + j];
    }

    T operator()(size_t aRow, size_t aCol) const
    {
        return m_Data[aRow * m_ColsCnt + aCol];
    }
    T& operator()(size_t aRow, size_t aCol)
    {
        return m_Data[aRow * m_ColsCnt + aCol];
    }
private:
    size_t m_RowsCnt;
    size_t m_ColsCnt;
    std::vector<T> m_Data;
};


template <typename T>
std::ostream& operator<<(std::ostream& aOs, const Matrix<T>& aMatr)
{
    for (size_t i = 0; i < aMatr.rowsCnt(); ++i)
    {
        for (size_t j = 0; j < aMatr.colsCnt(); ++j)
        {
            aOs << aMatr(i, j) << ' ';
        }
        aOs << '\n';
    }
    return aOs;
}
