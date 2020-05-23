/**
 * @file Matrix.cpp
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Implementation file for the Matrix class which represents a 2D matrix or 1D vector.
 */

#define ERROR_BAD_MATRIX_INPUT "Error: Invalid Matrix input."
#define ERROR_NOT_MATRIX "Error: Can't preform Matrix operation on Vector."
#define ERROR_MATRIX_DIMS "Error: Can't use operation on two Matrices with incompatible dimensions."
#define ERROR_BAD_MATRIX_INDEX "Error: Invalid index to access matrix."

#define PRINT_MATRIX_MESSAGE "Image processes:"
#define NO_PIXEL "  "
#define YES_PIXEL "**"

#define DEFAULT_SIZE 1
#define PRINT_THRESHOLD 0.1f

#include <iostream>
#include "Matrix.h"

// Returns pointer to a malloced array of the given length, filled with 0.
float *_createZeroVector(int length)
{
    auto *vector = new float[length];
    for (int i = 0; i < length; i++)
    {
        vector[i] = 0.0f;
    }
    return vector;
}

// Returns pointer to a malloced copy of the given array.
float *_copyVector(int length, const float vector[])
{
    auto *newVector = new float[length];
    for (int i = 0; i < length; i++)
    {
        newVector[i] = vector[i];
    }
    return newVector;
}

Matrix::Matrix(int rows, int cols) : _rows(rows), _cols(cols)
{
    if (rows <= 0 || cols <= 0)
    {
        std::cerr << ERROR_BAD_MATRIX_INPUT << std::endl;
        exit(EXIT_FAILURE);
    }

    if (cols == DEFAULT_SIZE)
    {
        _matrix = nullptr;
        _vector = _createZeroVector(rows);
    }
    else
    {
        _vector = nullptr;
        _matrix = new float *[rows];
        for (int i = 0; i < rows; i++)
        {
            _matrix[i] = _createZeroVector(cols);
        }
    }
}

Matrix::Matrix() : _rows(DEFAULT_SIZE), _cols(DEFAULT_SIZE), _matrix(nullptr)
{
    _vector = _createZeroVector(DEFAULT_SIZE);
}

// Copies another Matrix into this one.
void Matrix::_copyMatrix(const Matrix &other)
{
    _rows = other._rows;
    _cols = other._cols;
    if (other._matrix == nullptr)
    {
        _matrix = nullptr;
        _vector = _copyVector(_rows, other._vector);
    }
    else
    {
        _vector = nullptr;
        _matrix = new float *[_rows];
        for (int i = 0; i < _rows; i++)
        {
            _matrix[i] = _copyVector(_cols, other._matrix[i]);
        }
    }
}

Matrix::Matrix(const Matrix &m)
{
    _copyMatrix(m);
}

// Frees the memory occupied by the arrays.
void Matrix::_freeArrays()
{
    if (_matrix == nullptr)
    {
        delete[] _vector;
    }
    else
    {
        for (int i = 0; i < _rows; i++)
        {
            delete[] _matrix[i];
        }
        delete[] _matrix;
    }
}

Matrix::~Matrix()
{
    _freeArrays();
}

int Matrix::getRows() const
{
    return _rows;
}

int Matrix::getCols() const
{
    return _cols;
}

void Matrix::vectorize()
{
    if (_matrix == nullptr)
    {
        std::cerr << ERROR_NOT_MATRIX << std::endl;
        exit(EXIT_FAILURE);
    }

    int newRows = _rows * _cols;
    _vector = new float[newRows];
    for (int y = 0, i = 0; y < _rows; y++)
    {
        float *row = _matrix[y];
        for (int x = 0; x < _cols; x++, i++)
        {
            _vector[i] = row[x];
        }
        delete[] row;
    }
    delete[] _matrix;
    _matrix = nullptr;
    _rows = newRows;
    _cols = DEFAULT_SIZE;
}

void Matrix::plainPrint() const
{
    if (_matrix == nullptr)
    {
        for (int i = 0; i < _rows; i++)
        {
            std::cout << _vector[i] << " " << std::endl;
        }
    }
    else
    {
        for (int i = 0; i < _rows; i++)
        {
            float *row = _matrix[i];
            for (int j = 0; j < _cols; j++)
            {
                std::cout << row[j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
        _freeArrays();
        _copyMatrix(other);
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (_cols != other._rows)
    {
        std::cerr << ERROR_MATRIX_DIMS << std::endl;
        exit(EXIT_FAILURE);
    }

    Matrix newMatrix(_rows, other._cols);

    if (_matrix == nullptr)
    {
        if (other._matrix == nullptr)
        {
            // Vector * 1*1 Vector.
            float otherVal = other._vector[0];
            for (int i = 0; i < newMatrix._rows; i++)
            {
                newMatrix._vector[i] = _vector[i] * otherVal;
            }
        }
        else
        {
            // Vector * Matrix.
            for (int i = 0; i < newMatrix._rows; i++)
            {
                for (int j = 0; j < newMatrix._cols; j++)
                {
                    newMatrix._matrix[i][j] = _vector[i] * other._matrix[0][j];
                }
            }
        }
    }
    else
    {
        if (other._matrix == nullptr)
        {
            // Matrix * Vector.
            for (int i = 0; i < newMatrix._rows; i++)
            {
                float &sum = newMatrix._vector[i];
                for (int k = 0; k < _cols; k++)
                {
                    sum += _matrix[i][k] * other._vector[k];
                }
            }
        }
        else
        {
            // Matrix * Matrix.
            for (int i = 0; i < newMatrix._rows; i++)
            {
                for (int j = 0; j < newMatrix._cols; j++)
                {
                    float &sum = newMatrix._matrix[i][j];
                    for (int k = 0; k < _cols; k++)
                    {
                        sum += _matrix[i][k] * other._matrix[k][j];
                    }
                }
            }
        }
    }

    return newMatrix;
}

Matrix Matrix::operator*(float scalar) const
{
    Matrix newMatrix(*this);
    if (_matrix == nullptr)
    {
        for (int i = 0; i < newMatrix._rows; i++)
        {
            newMatrix._vector[i] *= scalar;
        }
    }
    else
    {
        for (int i = 0; i < newMatrix._rows; i++)
        {
            for (int j = 0; j < newMatrix._cols; j++)
            {
                newMatrix._matrix[i][j] *= scalar;
            }
        }
    }

    return newMatrix;
}

Matrix operator*(float scalar, const Matrix &matrix)
{
    return (matrix * scalar);
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    if (_cols != other._cols || _rows != other._rows)
    {
        std::cerr << ERROR_MATRIX_DIMS << std::endl;
        exit(EXIT_FAILURE);
    }

    if (_matrix == nullptr)
    {
        for (int i = 0; i < _rows; i++)
        {
            _vector[i] += other._vector[i];
        }
    }
    else
    {
        for (int i = 0; i < _rows; i++)
        {
            float *row = _matrix[i], *otherRow = other._matrix[i];
            for (int j = 0; j < _cols; j++)
            {
                row[j] += otherRow[j];
            }
        }
    }

    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    if (_cols != other._cols || _rows != other._rows)
    {
        std::cerr << ERROR_MATRIX_DIMS << std::endl;
        exit(EXIT_FAILURE);
    }

    Matrix newMatrix(*this);
    return (newMatrix += other);
}

float &Matrix::operator()(int i, int j)
{
    if (i < 0 || j < 0 || i >= _rows || j >= _cols)
    {
        std::cerr << ERROR_BAD_MATRIX_INDEX << std::endl;
        exit(EXIT_FAILURE);
    }

    if (_matrix == nullptr)
    {
        return _vector[i];
    }

    return _matrix[i][j];
}

float &Matrix::operator[](int i)
{
    int y = i % _cols, x = (i - y) / _cols;
    return (*this)(y, x);
}

float Matrix::operator()(int i, int j) const
{
    return (*this)(i, j);
}

float Matrix::operator[](int i) const
{
    return (*this)[i];
}

std::istream &operator>>(std::istream &is, Matrix &matrix)
{
    if (matrix._matrix == nullptr)
    {
        for (int i = 0; i < matrix._rows; i++)
        {
            is >> matrix._vector[i];
            if (is.fail())
            {
                std::cerr << ERROR_BAD_MATRIX_INPUT << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        for (int i = 0; i < matrix._rows; i++)
        {
            float *row = matrix._matrix[i];
            for (int j = 0; j < matrix._cols; j++)
            {
                is >> row[j];
                if (is.fail())
                {
                    std::cerr << ERROR_BAD_MATRIX_INPUT << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    if (!is.eof())
    {
        std::cerr << ERROR_BAD_MATRIX_INPUT << std::endl;
        exit(EXIT_FAILURE);
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    os << PRINT_MATRIX_MESSAGE << std::endl;
    if (matrix._matrix == nullptr)
    {
        for (int i = 0; i < matrix._rows; i++)
        {
            os << ((matrix._vector[i] <= PRINT_THRESHOLD) ? NO_PIXEL : YES_PIXEL) << std::endl;
        }
    }
    else
    {
        for (int i = 0; i < matrix._rows; i++)
        {
            float *row = matrix._matrix[i];
            for (int j = 0; j < matrix._cols; j++)
            {
                os << ((row[j] <= PRINT_THRESHOLD) ? NO_PIXEL : YES_PIXEL);
            }
            os << std::endl;
        }
    }

    return os << std::endl;
}