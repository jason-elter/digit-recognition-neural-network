/**
 * @file Matrix.cpp
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Implementation file for the Matrix class which represents a 2D matrix or 1D vector.
 */

#define ERROR_BAD_MATRIX_INPUT "Error: Invalid Matrix input."
#define ERROR_MATRIX_DIMS "Error: Can't use operation on two Matrices with incompatible dimensions."
#define ERROR_BAD_MATRIX_INDEX "Error: Invalid index to access matrix."

#define NO_PIXEL "  "
#define YES_PIXEL "**"

#define DEFAULT_SIZE 1
#define PRINT_THRESHOLD 0.1f

#include <iostream>
#include "Matrix.h"

// Returns pointer to a malloced array of the given length, filled with 0.
static float *_createZeroVector(int length)
{
    auto *vector = new float[length];
    for (int i = 0; i < length; i++)
    {
        vector[i] = 0.0f;
    }
    return vector;
}

// Returns pointer to a malloced copy of the given array.
static float *_copyVector(int length, const float vector[])
{
    auto *newVector = new float[length];
    for (int i = 0; i < length; i++)
    {
        newVector[i] = vector[i];
    }
    return newVector;
}

/**
 * Constructs Matrix rows * cols.
 * Inits all elements to 0.
 *
 * @param rows Number of rows the matrix will have.
 * @param colsNumber of columns the matrix will have.
 */
Matrix::Matrix(int rows, int cols) : _rows(rows), _cols(cols)
{
    if (rows <= 0 || cols <= 0)
    {
        std::cerr << "creation fail" << std::endl;
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

/**
 * Constructs 1*1 Matrix.
 * Inits the single element to 0.
 */
Matrix::Matrix() : _rows(DEFAULT_SIZE), _cols(DEFAULT_SIZE), _matrix(nullptr)
{
    _vector = _createZeroVector(DEFAULT_SIZE);
}

/**
 * Copies another Matrix into this one. (Private method)
 *
 * @param other The matrix to copy.
 */
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

/**
 * Constructs Matrix from another Matrix m.
 *
 * @param m The Matrix to copy.
 */
Matrix::Matrix(const Matrix &m)
{
    _copyMatrix(m);
}

/**
 * Frees the memory occupied by the arrays. (private)
 */
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

/**
 * Destroys the Matrix and frees the memory occupied by it.
 */
Matrix::~Matrix()
{
    _freeArrays();
}

/**
 * Returns the amount of rows as int.
 *
 * @return The amount of rows as int.
 */
int Matrix::getRows() const
{
    return _rows;
}

/**
 * Returns the amount of columns as int.
 *
 * @return The amount of columns as int.
 */
int Matrix::getCols() const
{
    return _cols;
}

/**
 * Transforms a matrix into a column vector.
 * Supports function calling concatenation.
 * i.e.(1) Matrix m(5,4);... m.vectorize()
 * m.getCols() == 1
 * m.getRows() == 20
 * i.e.(2) Matrix m(5,4), b(20, 1); then
 * m.vectorize() + b should be a valid expression.
 */
Matrix &Matrix::vectorize()
{
    if (_matrix != nullptr)
    {
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
    return *this;
}

/**
 * Prints matrix elements, no return value.
 * prints space after each element (incl. last element in the row).
 * prints newline after each row (incl. last row).
 */
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

/**
 * Matrix copy constructor (Matrix a,b; ... a = b;)
 *
 * @param other The matrix to copy.
 * @return A reference to this Matrix after copying the other Matrix.
 */
Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
        _freeArrays();
        _copyMatrix(other);
    }
    return *this;
}

/**
 * Matrix a,b; -> a * b
 *
 * @param other The other matrix.
 * @return A reference to the result as a Matrix.
 */
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
            return newMatrix;
        }

        // Vector * Matrix.
        for (int i = 0; i < newMatrix._rows; i++)
        {
            for (int j = 0; j < newMatrix._cols; j++)
            {
                newMatrix._matrix[i][j] = _vector[i] * other._matrix[0][j];
            }
        }
        return newMatrix;
    }

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
        return newMatrix;
    }

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
    return newMatrix;
}

/**
 *  Matrix m; float c; -> m * c
 *
 * @param matrix A matrix.
 * @param scalar A scalar (float).
 * @return A reference to the result as a Matrix.
 */
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

/**
 *  Matrix m; float c; -> c * m
 *
 * @param matrix A matrix.
 * @param scalar A scalar (float).
 * @return A reference to the result as a Matrix.
 */
Matrix operator*(float scalar, const Matrix &matrix)
{
    return (matrix * scalar);
}

/**
 * Matrix a,b; -> a += b
 *
 * @param other The other matrix.
 * @return A reference to this Matrix after addition.
 */
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

/**
 * Matrix a,b; -> a + b
 *
 * @param other The other matrix.
 * @return A reference to the result as a Matrix.
 */
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

/**
 * Double index access. (private)
 */
float &Matrix::_accessCell(int i, int j) const
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

/**
 * Single index access. (private)
 */
float &Matrix::_accessCell(int i) const
{
    int x = i % _cols;
    int y = (i - x) / _cols;
    return _accessCell(y, x);
}

/**
 * For i,j indices, Matrix m:
 * m(i,j) will return the i,j element.
 *
 * @param i The row index.
 * @param j The column index.
 * @return The i,j element in this Matrix.
 */
float &Matrix::operator()(int i, int j)
{
    return _accessCell(i, j);
}

/**
 * For i index, Matrix m:
 * m[i] will return the i'th element.
 *
 * @param i The index in the Matrix.
 * @return The i'th element in this Matrix.
 */
float &Matrix::operator[](int i)
{
    return _accessCell(i);
}

/**
 * For i,j indices, Matrix m:
 * m(i,j) will return the i,j element.
 *
 * @param i The row index.
 * @param j The column index.
 * @return The i,j element in this Matrix.
 */
float Matrix::operator()(int i, int j) const
{
    return _accessCell(i, j);
}

/**
 * For i index, Matrix m:
 * m[i] will return the i'th element.
 *
 * @param i The index in the Matrix.
 * @return The i'th element in this Matrix.
 */
float Matrix::operator[](int i) const
{
    return _accessCell(i);
}

/**
 * Fills matrix elements.
 * Has to read input stream fully, otherwise, that's an error.
 * istream is; Matrix m(rows, cols); ... is >> m;
 *
 * @param is The input stream.
 * @param matrix The Matrix.
 * @return A reference to the input stream.
 */
std::istream &operator>>(std::istream &is, Matrix &matrix)
{
    if (matrix._matrix == nullptr)
    {
        for (int i = 0; i < matrix._rows; i++)
        {
            is.read((char *)&(matrix._vector[i]), sizeof(float));
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
                is.read((char *)&(row[j]), sizeof(float));
                if (is.fail())
                {
                    std::cerr << ERROR_BAD_MATRIX_INPUT << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Check if can read anymore.
    float test;
    if (is.read((char *)&test, sizeof(float)))
    {
        std::cerr << ERROR_BAD_MATRIX_INPUT << std::endl;
        exit(EXIT_FAILURE);
    }
    is.clear();
    return is;
}

/**
 * Pretty export of the matrix.
 *
 * @param os The output stream.
 * @param matrix The matrix.
 * @return A reference to the output stream.
 */
std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
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

    return os;
}