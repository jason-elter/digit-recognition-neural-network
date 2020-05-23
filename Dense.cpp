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

Dense::Dense(const Matrix &w, const Matrix &bias, ActivationType actType) : _weights(w),
                                                                            _bias(bias),
                                                                            _activation(actType)
{}

const Matrix &Dense::getWeights() const
{
    return _weights;
}

const Matrix &Dense::getBias() const
{
    return _bias;
}

const Activation &Dense::getActivation() const
{
    return _activation;
}

Matrix Dense::operator()(const Matrix &input) const
{
    return _activation((_weights * input) + _bias);
}
