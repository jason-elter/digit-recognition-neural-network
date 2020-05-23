/**
 * @file Matrix.h
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Header file for the Matrix class which represents a 2D matrix or 1D vector.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * The Matrix class- represents a 2D matrix or 1D vector.
 */
class Matrix
{
public:
    // Constructors.
    /**
     * Constructs Matrix rows * cols.
     * Inits all elements to 0.
     *
     * @param rows Number of rows the matrix will have.
     * @param colsNumber of columns the matrix will have.
     */
    Matrix(int rows, int cols);

    /**
     * Constructs 1*1 Matrix.
     * Inits the single element to 0.
     */
    Matrix();

    /**
     * Constructs Matrix from another Matrix m.
     *
     * @param m The Matrix to copy.
     */
    Matrix(const Matrix &m);

    /**
     * Destroys the Matrix and frees the memory occupied by it.
     */
    ~Matrix();

    // Methods.
    /**
     * Returns the amount of rows as int.
     *
     * @return The amount of rows as int.
     */
    int getRows() const;

    /**
     * Returns the amount of columns as int.
     *
     * @return The amount of columns as int.
     */
    int getCols() const;

    /**
     * Transforms a matrix into a column vector.
     * Supports function calling concatenation.
     * i.e.(1) Matrix m(5,4);... m.vectorize()
     * m.getCols() == 1
     * m.getRows() == 20
     * i.e.(2) Matrix m(5,4), b(20, 1); then
     * m.vectorize() + b should be a valid expression.
     */
    void vectorize();

    /**
     * Prints matrix elements, no return value.
     * prints space after each element (incl. last element in the row).
     * prints newline after each row (incl. last row).
     */
    void plainPrint() const;

    // Operators.
    /**
     * Matrix copy constructor (Matrix a,b; ... a = b;)
     *
     * @param other The matrix to copy.
     * @return A reference to this Matrix after copying the other Matrix.
     */
    Matrix &operator=(const Matrix &other);

    /**
     * Matrix a,b; -> a * b
     *
     * @param other The other matrix.
     * @return A reference to the result as a Matrix.
     */
    Matrix operator*(const Matrix &other) const;

    /**
     *  Matrix m; float c; -> m * c
     *
     * @param matrix A matrix.
     * @param scalar A scalar (float).
     * @return A reference to the result as a Matrix.
     */
    Matrix operator*(float scalar) const;

    /**
     *  Matrix m; float c; -> c * m
     *
     * @param matrix A matrix.
     * @param scalar A scalar (float).
     * @return A reference to the result as a Matrix.
     */
    friend Matrix operator*(float scalar, const Matrix &matrix);

    /**
     * Matrix a,b; -> a += b
     *
     * @param other The other matrix.
     * @return A reference to this Matrix after addition.
     */
    Matrix &operator+=(const Matrix &other);

    /**
     * Matrix a,b; -> a + b
     *
     * @param other The other matrix.
     * @return A reference to the result as a Matrix.
     */
    Matrix operator+(const Matrix &other) const;

    /**
     * For i,j indices, Matrix m:
     * m(i,j) will return the i,j element.
     *
     * @param i The row index.
     * @param j The column index.
     * @return The i,j element in this Matrix.
     */
    float operator()(int i, int j) const;

    /**
     * For i,j indices, Matrix m:
     * m(i,j) will return the i,j element.
     *
     * @param i The row index.
     * @param j The column index.
     * @return The i,j element in this Matrix.
     */
    float &operator()(int i, int j);

    /**
     * For i index, Matrix m:
     * m[i] will return the i'th element.
     *
     * @param i The index in the Matrix.
     * @return The i'th element in this Matrix.
     */
    float operator[](int i) const;

    /**
     * For i index, Matrix m:
     * m[i] will return the i'th element.
     *
     * @param i The index in the Matrix.
     * @return The i'th element in this Matrix.
     */
    float &operator[](int i);

    /**
     * Fills matrix elements.
     * Has to read input stream fully, otherwise, that's an error.
     * istream is; Matrix m(rows, cols); ... is >> m;
     *
     * @param is The input stream.
     * @param matrix The Matrix.
     * @return A reference to the input stream.
     */
    friend std::istream &operator>>(std::istream &is, Matrix &matrix);

    /**
     * Pretty export of the matrix.
     *
     * @param os The output stream.
     * @param matrix The matrix.
     * @return A reference to the output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

private:
    int _rows, _cols;
    float **_matrix, *_vector;

    void _copyMatrix(const Matrix &other); // Copies another matrix into this one.
    void _freeArrays(); // Frees the memory occupied by the arrays.
};

#endif //MATRIX_H
