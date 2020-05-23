/**
 * @file Digit.h
 * @author  Jason Elter <jason.elter@mail.huji.ac.il>
 * @version 1.0
 * @date 24 January 2020
 *
 * @brief Header file for Digit struct which is the result of a MlpNetwork. 
 */

#ifndef DIGIT_H
#define DIGIT_H

/**
 * @struct Digit
 * @brief Identified (by Mlp network) Digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct Digit
{
    unsigned int value;
    float probability;
} Digit;

#endif //DIGIT_H
