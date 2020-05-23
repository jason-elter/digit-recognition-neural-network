/**
 * @file MlpNetwork.cpp
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Implementation file for the MlpNetwork class which represents 
 * a multi-layered neural network for digit recognition in images.
 */

#define ERROR_BAD_MLP_DIMS "Error: You have given MlpNetwork matrices with improper dimensions"

#define IS_MLP_VECTOR 1
#define RESULT_LENGTH 10

#include "MlpNetwork.h"

MlpNetwork::MlpNetwork(const Matrix weights[MLP_SIZE], const Matrix biases[MLP_SIZE]) : _weights(
        weights), _biases(biases)
{
    for (int i = 0; i < MLP_SIZE; i++)
    {
        if (weights[i].getRows() != weightsDims[i].rows ||
            weights[i].getCols() != weightsDims[i].cols ||
            biases[i].getRows() != biasDims[i].rows ||
            biases[i].getCols() != biasDims[i].cols)
        {
            std::cerr << ERROR_BAD_MLP_DIMS << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

Digit MlpNetwork::operator()(const Matrix &input) const
{
    if (input.getRows() != (imgDims.rows * imgDims.cols) || input.getCols() != IS_MLP_VECTOR)
    {
        std::cerr << ERROR_BAD_MLP_DIMS << std::endl;
        exit(EXIT_FAILURE);
    }

    // Apply the layers.
    Matrix result(Dense(_weights[0], _biases[0], Relu)(input));
    for (int i = 0; i < (MLP_SIZE - 2); i++)
    {
        Dense currentLayer(_weights[i], _biases[i], Relu);
        result = currentLayer(result);
    }
    result = Dense(_weights[0], _biases[0], Softmax)(result);

    // Find Most likely digit.
    Digit digit = {0, result[0]};
    for (int i = 1; i < RESULT_LENGTH; i++)
    {
        float newProbability = result[i];
        if (newProbability > digit.probability)
        {
            digit.value = i;
            digit.probability = newProbability;
        }
    }
    return digit;
}

