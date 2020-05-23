/**
 * @file MlpNetwork.h
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Header file for the MlpNetwork class which represents 
 * a multi-layered neural network for digit recognition in images.
 */

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
                                  {64,  128},
                                  {20,  64},
                                  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
                               {64,  1},
                               {20,  1},
                               {10,  1}};

/**
 * The MlpNetwork class- represents a multi-layered neural network for digit recognition in images.
 */
class MlpNetwork
{
public:
    // Constructors.
    /**
     * Accepts 2 arrays, size 4 each.
     * One for weights and one for biases.
     * Constructs the network.
     *
     * @param weights
     * @param biases
     */
    MlpNetwork(const Matrix weights[MLP_SIZE], const Matrix biases[MLP_SIZE]);

    // Operators.
    /**
     * Applies the entire network on the input.
     * Returns Digit struct.
     * MlpNetwork m(...); ... Digit output = m(img);
     *
     * @param input The input Matrix.
     * @return Digit struct that represents the most likely digit in the image.
     */
    Digit operator()(const Matrix &input) const;

private:
    const Matrix *_weights, *_biases;
};

#endif // MLPNETWORK_H
