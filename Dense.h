/**
 * @file Dense.h
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Header file for the Dense class which represents a layer in a MlpNetwork.
 */

#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * The Dense class- represents a layer in a MlpNetwork.
 */
class Dense
{
public:
    // Constructors.
    /**
     * Inits a new layer with given parameters.
     *
     * @param w The weights Matrix for this layer.
     * @param bias The bias Matrix (Vector) for this layer.
     * @param actType The activation type to be used in this layer.
     */
    Dense(const Matrix &w, const Matrix &bias, ActivationType actType);

    // Methods.
    /**
     * Returns the weights of this layer.
     * Forbids modification.
     *
     * @return The weights of this layer.
     */
    const Matrix &getWeights() const;

    /**
     * Returns the bias of this layer.
     * Forbids modification.
     *
     * @return The bias of this layer.
     */
    const Matrix &getBias() const;

    /**
     * Returns the activation function of this layer.
     * forbids modification.
     *
     * @return The activation function of this layer.
     */
    const Activation &getActivation() const;

    // Operators.
    /**
     * Applies the layer on input and returns output Matrix.
     *
     * @param input The Matrix to apply this layer on.
     * @return The input Matrix after applying this layer on it (new Matrix).
     */
    Matrix operator()(const Matrix &input) const;

private:
    const Matrix& _weights, _bias;
    const Activation _activation;
};

#endif //DENSE_H
