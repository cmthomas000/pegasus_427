/*
 * bunkers.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef BUNKERS_H_
#define BUNKERS_H_



#include "screen.h"
#include "globals.h"

void bunkers_draw_initial();

bool bunkers_check_hit(int x, int y);

void bunkers_update();

void bunker_damage(int bunkerNum, int quadrant);
#endif /* BUNKERS_H_ */
