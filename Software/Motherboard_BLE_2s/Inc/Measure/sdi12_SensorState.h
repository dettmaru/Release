/*
 * sdi12_SensorState.h
 *
 *  Created on: May 2, 2019
 *      Author: Daniel Lohmann
 */

#ifndef MEASURE_SDI12_SENSORSTATE_H_
#define MEASURE_SDI12_SENSORSTATE_H_

enum class SDI12_SensorState {
	OFF=0,
	Init,
	Measure,
	Finished
};



#endif /* MEASURE_SDI12_SENSORSTATE_H_ */
