/*
 * interpolation.h
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_INTERPOLATION_H_
#define SOURCE_ADSP_INTERPOLATION_H_

#include <cmath>

namespace Adsp {
/**
 * @defgroup interpolation
 * @ingroup Adsp
 * Contains several interpolation algorithms.
 * shape.
 * @{
 */

/**
 * Performs linear interpolation. This is done via a simple
 * equation: \f$ v_1*(1-b)+v_2*b\f$
 * @param val1 The value to blend from.
 * @param val2 The value to blend to.
 * @param blend A double between 0 and 1 which controls the blend between val1 and val2.
 * @return The blended value.
 */
constexpr double linearInterp(double val1, double val2, double blend) {
	return val2 * blend + val1 * (1.0 - blend);
}

#ifdef USE_STRICT_CONSTEXPR
double smoothInterp(double val1, double val2, double blend);
#else
/**
 * Performs 'smooth' interpolation. It's not really smooth,
 * it just has a bendy shape. The shape is defined by a
 * polynomial: \f$ 6x^5-15x^4+10x^3\f$
 * @param val1 The value to blend from.
 * @param val2 The value to blend to.
 * @param blend A double between 0 and 1 which controls the blend between val1 and val2.
 * @return The blended value.
 */
constexpr double smoothInterp(double val1, double val2, double blend) {
	return linearInterp(val1, val2, (6 * pow(blend, 5)) - (15 * pow(blend, 4)) + (10 * pow(blend, 3)));
}
#endif

/**
 * Lookup table defining the shape used in fastEnvelopeInterp().
 * The shape is defined by the function
 * \f$ 1-\sqrt{1-x^2}\f$ for the range 0 - 1.
 */
const double ENVELOPE_POSITIVE_SHAPE[101] = { 0, 5.00013e-05, 0.00020002, 0.000450101, 0.00080032, 0.00125078, 0.00180162, 0.00245301, 0.00320514, 0.00405823, 0.00501256, 0.00606841, 0.00722611, 0.00848601, 0.0098485, 0.011314, 0.012883, 0.0145559, 0.0163334, 0.0182159, 0.0202041, 0.0222986, 0.0245001, 0.0268094, 0.0292271, 0.0317542, 0.0343914, 0.0371397, 0.04, 0.0429734, 0.0460608, 0.0492634, 0.0525825, 0.0560191, 0.0595746, 0.0632503, 0.0670477, 0.0709682, 0.0750135, 0.0791851, 0.0834849, 0.0879145, 0.0924759, 0.0971711, 0.102002, 0.106971, 0.112081, 0.117334, 0.122732, 0.128278, 0.133975, 0.139826, 0.145834, 0.152002, 0.158335, 0.164835, 0.171507, 0.178355, 0.185384, 0.192597, 0.2, 0.207599, 0.215398, 0.223405, 0.231625, 0.240066, 0.248734, 0.257639, 0.266788, 0.276191, 0.285857, 0.295798, 0.306026, 0.316553, 0.327393, 0.338562, 0.350077, 0.361956, 0.37422, 0.386893, 0.4, 0.41357, 0.427636, 0.442237, 0.457414, 0.473217, 0.489706, 0.506948, 0.525026, 0.544039, 0.56411, 0.585392, 0.608082, 0.63244, 0.658826, 0.68775, 0.72, 0.756895, 0.801003, 0.858933, 1 };
/**
 * Lookup table defining the shape used in fastEnvelopeInterp().
 * The shape is defined by the function
 * \f$ \sqrt{2x-x^2}\f$ for the range 0 - 1.
 */
const double ENVELOPE_NEGATIVE_SHAPE[101] = { 0, 0.141067, 0.198997, 0.243105, 0.28, 0.31225, 0.341174, 0.36756, 0.391918, 0.414608, 0.43589, 0.455961, 0.474974, 0.493052, 0.510294, 0.526783, 0.542586, 0.557763, 0.572364, 0.58643, 0.6, 0.613107, 0.62578, 0.638044, 0.649923, 0.661438, 0.672607, 0.683447, 0.693974, 0.704202, 0.714143, 0.723809, 0.733212, 0.742361, 0.751266, 0.759934, 0.768375, 0.776595, 0.784602, 0.792401, 0.8, 0.807403, 0.814616, 0.821645, 0.828493, 0.835165, 0.841665, 0.847998, 0.854166, 0.860174, 0.866025, 0.871722, 0.877268, 0.882666, 0.887919, 0.893029, 0.897998, 0.902829, 0.907524, 0.912086, 0.916515, 0.920815, 0.924986, 0.929032, 0.932952, 0.93675, 0.940425, 0.943981, 0.947418, 0.950737, 0.953939, 0.957027, 0.96, 0.96286, 0.965609, 0.968246, 0.970773, 0.973191, 0.9755, 0.977701, 0.979796, 0.981784, 0.983667, 0.985444, 0.987117, 0.988686, 0.990152, 0.991514, 0.992774, 0.993932, 0.994987, 0.995942, 0.996795, 0.997547, 0.998198, 0.998749, 0.9992, 0.99955, 0.9998, 0.99995, 1.0 };

/**
 * Performs 'envelope interpolation' between -1.0 and 1.0.
 * Envelope interpolation has an extra parameter, shape.
 * When shape is 0.0, it acts like linear interpolation.
 * However, as the shape goes to -1 or 1, it takes on
 * a circular shape, which is retrieved from a lookup.
 * table. See ENVELOPE_POSITIVE_SHAPE and
 * ENVELOPE_NEGATIVE_SHAPE for the exact formulas.
 * @param blend A double between 0 and 1 which controls the blend between -1.0 and 1.0.
 * @param shape A double between -1 and 1 which works as previously described.
 * @return
 */
double fastEnvelopeInterp(double blend, double shape);

/**
 * Like fastEnvelopeInterp(), but with extra inputs
 * to interpolate between values other than -1 and 1.
 * @param val1 The value to blend from.
 * @param val2 The value to blend to.
 * @param blend A double between 0 and 1 which controls the blend between -1.0 and 1.0.
 * @param shape See fastEnvelopeInterp().
 * @return
 */
double envelopeInterp(double val1, double val2, double blend, double shape);

/** @} */

} /* namespace Adsp */

#endif /* SOURCE_ADSP_INTERPOLATION_H_ */
