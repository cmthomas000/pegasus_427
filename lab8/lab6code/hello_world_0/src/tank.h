/*
 * tank.h
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */

#ifndef TANK_H_
#define TANK_H_

#include "screen.h"
#include "globals.h"

// draws the tank near the center bottom of the screen
void tank_draw_initial();

// draws 3 copies of the tank in the top right corner of the screen
// to represent the number of remaining lives
void tank_draw_lives_initial();

// moves the tank to the left
void tank_move_left();

// moves the tank to the right
void tank_move_right();

bool tank_check_hit(point_t pos);

// Decrements a life and redraws new tank
void tank_respawn();

// erases tank from lives display on screen
void tank_remove_life();

// return true if the tank is dying
bool tank_is_dying();

// draws the next stage of the tank death animation
void tank_update_death();

//void tank_remove_life();
//void tank_add_life();
#endif /* TANK_H_ */
