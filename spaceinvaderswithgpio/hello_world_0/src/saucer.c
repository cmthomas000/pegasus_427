/*
 * saucer.c
 *
 *  Created on: Oct 13, 2016
 *      Author: superman
 */

#include "saucer.h"
#include "screen.h"
#include "bunkers.h"
#include "text.h"
#include "globals.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b15 << 15) |(b14 << 14) |(b13 << 13) |(b12 << 12) |(b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define SAUCER_LEFT_X 0
#define SAUCER_LEFT_Y 15
#define SAUCER_RIGHT_X SCREEN_WIDTH-SAUCER_WIDTH
#define SAUCER_RIGHT_Y 15
#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 7
#define SAUCER_MOVEMENT 4

#define SAUCER_POINT_VAR 4
#define BIT_MASK 1
#define X_OFFSET 1

#define SAUCER_LEFT 0
#define SAUCER_RIGHT 1
#define SAUCER_NO_MOTION 2

static const uint32_t saucer_16x7[] =
{
		packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
		packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
		packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
		packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
		packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
		packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
		packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};

static const uint32_t saucer_ghost_16x7[] =
{
		packword16(1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1),
		packword16(1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1),
		packword16(1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1),
		packword16(1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1),
		packword16(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
		packword16(1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1),
		packword16(1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1)
};

static uint32_t leftBound;

static uint32_t saucerPoints[SAUCER_POINT_VAR] = {50,100,150,300};
uint32_t saucer_randMod2() {
	return rand() % 2;	// Random number generated to add random sequence square
}
uint32_t saucer_randMod3() {
	return rand() % 3;	// Random number generated to add random sequence square
}

void saucer_draw_initial(){
	int32_t x, y;
	for(x = 0; x < SAUCER_WIDTH; x++) {
		for(y = 0; y < SAUCER_HEIGHT; y++) {
			if(saucer_16x7[y] & (BIT_MASK << x)) {
				screen_draw_double_pixel(x+globals_getSaucerPosition(),y+SAUCER_LEFT_Y,SCREEN_RED);
			}
		}
	}
}
void saucer_redraw(uint32_t direction) {
	int32_t x, y;

	uint32_t xBitShift = (direction!=SAUCER_LEFT)? 0:SAUCER_MOVEMENT;
	uint32_t offset = (direction!=SAUCER_NO_MOTION)? SAUCER_MOVEMENT:0;
	uint32_t rightShift = (direction==SAUCER_RIGHT)? SAUCER_MOVEMENT:0;
	uint16_t pos = globals_getSaucerPosition();
	for(y = 0; y < SAUCER_HEIGHT; y++) {
		for(x = 0; x < SAUCER_WIDTH+offset; x++) {
			if((saucer_16x7[y] & (BIT_MASK<<x)) != ((saucer_16x7[y]<<offset) & (BIT_MASK<<(x)))) { //some changes has occured to this pixel
				if((saucer_16x7[y]<<xBitShift) &(BIT_MASK<<x)) {
					screen_draw_double_pixel(pos+rightShift+SAUCER_WIDTH-X_OFFSET-x,y+SAUCER_LEFT_Y,SCREEN_BLACK);
				}
				else {
					screen_draw_double_pixel(pos+rightShift+SAUCER_WIDTH-X_OFFSET-x,y+SAUCER_LEFT_Y,SCREEN_RED);
				}
			}
		}
	}
}

void saucer_erase() {
	int32_t x, y;
	for(x = 0; x < SAUCER_WIDTH; x++) {
		for(y = 0; y < SAUCER_HEIGHT; y++) {
			if(saucer_16x7[y] & (BIT_MASK << x)) {
				screen_draw_double_pixel(x+globals_getSaucerPosition(),y+SAUCER_LEFT_Y,SCREEN_BLACK);
			}
		}
	}
}

void saucer_update() {
	// check to see if it has spawned
	if(globals_saucerSpawned()) {
		// see which side it spawned from

		if(globals_getSaucerPosition()>=0 && globals_getSaucerPosition()<=SAUCER_RIGHT_X-SAUCER_MOVEMENT && leftBound==SAUCER_LEFT) { // MOVE RIGHT

			globals_setSaucerPosition(globals_getSaucerPosition()+SAUCER_MOVEMENT);
			saucer_redraw(leftBound);
		}
		else if(globals_getSaucerPosition()>=SAUCER_MOVEMENT && globals_getSaucerPosition()<=SAUCER_RIGHT_X && leftBound==SAUCER_RIGHT) { // MOVE LEFT
			globals_setSaucerPosition(globals_getSaucerPosition()-SAUCER_MOVEMENT);
			saucer_redraw(leftBound);
		}

		else {
			saucer_erase();
			globals_setSaucerPosition(GLOBALS_NULL_LOCATION);
			 globals_toggleSaucer();

		}
		// if off the screen, raise flag for spawn enable. Make sure saucer doesn't roll around
	}
}


void saucer_spawn() {

	// check to see if the flag is raised
	if(!globals_saucerSpawned()) {
		globals_toggleSaucer();
		// randomly pick left or right
		leftBound =saucer_randMod2();
		int32_t initPos;
		initPos = (leftBound==0)? SAUCER_LEFT_X:
		SAUCER_RIGHT_X;

		globals_setSaucerPosition(initPos);
		// draw alien at that position
		saucer_draw_initial();
	}
}

bool saucer_check_hit(point_t pos){
	if(globals_getSaucerPosition() == GLOBALS_NULL_LOCATION) { // saucer is not currently on screen
		return false;
	}
	// See if the point is in bounds
	if((pos.y>=SAUCER_RIGHT_Y) && (pos.y<=SAUCER_RIGHT_Y+SAUCER_HEIGHT)) {
		if((pos.x>=globals_getSaucerPosition()) && (pos.x<=globals_getSaucerPosition()+SAUCER_WIDTH)){
			// erase the alien
			saucer_erase();
			// print score
			uint32_t score = saucerPoints[saucer_randMod3()];
						// add random score
						text_add_score(score);

						text_begin_saucer_score(score);
				//		text_print_saucer_score(score);
			globals_setSaucerPosition(GLOBALS_NULL_LOCATION);



			// reset flag
			globals_toggleSaucer();
			return true;
		}
	}
	return false;
}

bool saucer_is_spawned() {
	return globals_saucerSpawned();
}
