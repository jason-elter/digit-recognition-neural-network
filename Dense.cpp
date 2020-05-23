/**
 * @file Dense.cpp
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Implementation file for the Dense class which represents a layer in a MlpNetwork.
 */

#include "Dense.h"
#include "Matrix.h"
#include "Activation.h"

/**
 * Inits a new layer with given parameters.
 *
 * @param w The weights Matrix for this layer.
 * @param bias The bias Matrix (Vector) for this layer.
 * @param actType The activation type to be used in this layer.
 */
Dense::Dense(const Matrix &w, const Matrix &bias, ActivationType actType) : _weights(w),
                                                                            _bias(bias),
                                                                            _activation(actType)
{}

/**
 * Returns the weights of this layer.
 * Forbids modification.
 *
 * @return The weights of this layer.
 */
const Matrix &Dense::getWeights() const
{
    return _weights;
}

/**
 * Returns the bias of this layer.
 * Forbids modification.
 *
 * @return The bias of this layer.
 */
const Matrix &Dense::getBias() const
{
    return _bias;
}

/**
 * Returns the activation function of this layer.
 * forbids modification.
 *
 * @return The activation function of this layer.
 */
const Activation &Dense::getActivation() const
{
    return _activation;
}

/**
 * Applies the layer on input and returns output Matrix.
 *
 * @param input The Matrix to apply this layer on.
 * @return The input Matrix after applying this layer on it (new Matrix).
 */
Matrix Dense::operator()(const Matrix &input) const
{
    return _activation((_weights * input) + _bias);
}
