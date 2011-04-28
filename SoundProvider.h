/*
 * SoundProvider.h
 *
 *  Created on: 28-04-2011
 *      Author: qwak
 */

#ifndef SOUNDPROVIDER_H_
#define SOUNDPROVIDER_H_

#include "libs/vec2d.h"

struct SoundProvider {

	virtual ~SoundProvider() {}

	virtual void playDestroy(const vec2d<double>& where) {}

	virtual void timerEvent(double elasped_time) {}

	virtual void updateListenerPosition(const vec2d<double>& new_listener_pos) {}

};

void initSoundProvider();

void freeSoundProvider();

SoundProvider& soundProvider();

#endif /* SOUNDPROVIDER_H_ */
