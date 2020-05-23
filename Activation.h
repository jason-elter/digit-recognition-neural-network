/**
 * @file Activation.h
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Header file for the Activation class which an activation function to apply to a Matrix.
 */

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * The activation class- represents an activation function to apply to a Matrix.
 */
class Activation
{
public:
    // Constructors.
    /**
     * Accepts activation type (Relu/Softmax)
     * and defines the instance's activation accordingly.
     *
     * @param actType The type of activation function to use.
     */
    explicit Activation(ActivationType actType);

    // Methods.
    /**
     * Returns this activation's type (Relu/Softmax).
     *
     * @return This activation's type (Relu/Softmax).
     */
    ActivationType getActivationType() const;

    // Operators.
    /**
     * Applies activation function on input.
     * (Does not change input)
     * Matrix output = act(input);
     *
     * @param input The matrix to activate.
     * @return A reference to the activated matrix (which is new).
     */
    Matrix operator()(const Matrix &input) const;

private:
    const ActivationType _type;
    void (*_activate)(const Matrix &input, Matrix &output);
    static void _relu(const Matrix &input, Matrix &output); // Relu activation function.
    static void _softmax(const Matrix &input, Matrix &output); // Softmax activation function.
};

#endif //ACTIVATION_H
