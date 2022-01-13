//
// Created by Ilir on 2022-01-13.
//

#ifndef PROJECTMCODES_EFFECTMODEHANDLER_H
#define PROJECTMCODES_EFFECTMODEHANDLER_H

#include "EXIPacket.h"

void resetEffectMode();
void checkEffectModeDurationFinished();

EXIStatus effectModeFlight(u16 duration, u16 x_maxspeed, u16 y_maxspeed, s16 x_accel, s16 y_accel);
EXIStatus effectModeBorderless(u16 duration);

#endif //PROJECTMCODES_EFFECTMODEHANDLER_H
