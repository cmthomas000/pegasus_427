/*
 * text.h
 *
 *  Created on: Sep 29, 2016
 *      Author: superman
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "screen.h"
#include "globals.h"

// Used to write a char to the game screen at given coordinate
void text_write(unsigned char val, point_t coord);

// Draws the scoreboard values to the top of the screen
void text_draw_score();

#endif /* TEXT_H_ */
