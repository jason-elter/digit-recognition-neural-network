/**
 * @file Activation.cpp
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Implementation file for the Activation class which an activation function to apply to a Matrix.
 */

#define ERROR_NOT_VECTOR "Error: Can only activate Vector, not Matrix."

#define IS_VECTOR 1
#define VECTOR_COLS 0

#include <math.h>
#include "Activation.h"
#include "Matrix.h"

/**
 * Accepts activation type (Relu/Softmax)
 * and defines the instance's activation accordingly.
 *
 * @param actType The type of activation function to use.
 */
Activation::Activation(ActivationType actType) : _type(actType)
{
    _activate = (actType == Relu) ? _relu : _softmax;
}

/**
 * Returns this activation's type (Relu/Softmax).
 *
 * @return This activation's type (Relu/Softmax).
 */
ActivationType Activation::getActivationType() const
{
    return _type;
}

/**
 * Applies activation function on input.
 * (Does not change input)
 * Matrix output = act(input);
 *
 * @param input The matrix to activate.
 * @return A reference to the activated matrix (which is new).
 */
Matrix Activation::operator()(const Matrix &input) const
{
    if (input.getCols() != IS_VECTOR)
    {
        std::cerr << ERROR_NOT_VECTOR << std::endl;
        exit(EXIT_FAILURE);
    }

    Matrix output(input.getRows(), IS_VECTOR);
    _activate(input, output);
    return output;
}

// Relu activation function.
void Activation::_relu(const Matrix &input, Matrix &output)
{
    int rows = input.getRows();
    for (int i = 0; i < rows; i++)
    {
        float value = input(i, VECTOR_COLS);
        if (value > 0.0f)
        {
            output(i, VECTOR_COLS) = value;
        }
    }
}

// Softmax activation function.
void Activation::_softmax(const Matrix &input, Matrix &output)
{
    int rows = input.getRows();
    float sum = 0.0f;
    for (int i = 0; i < rows; i++)
    {
        sum += output(i, VECTOR_COLS) = exp(input(i, VECTOR_COLS));
    }
    output = output * (1.0f / sum);
}
