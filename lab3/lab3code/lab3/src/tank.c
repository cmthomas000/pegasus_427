/*
 * tank.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "tank.h"
#include "screen.h"
#include "globals.h"
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>

#define packWord15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define	TANK_Y 220 // out of a 240 height
#define TANK_HEIGHT 8
#define TANK_WIDTH 15
#define TANK_MOVEMENT 3
#define LIVES_Y 10
#define BIT_MASK 1
#define X_OFFSET 1

#define NUMBER_OF_LIVES 3

static const long tank_15x8[] =
{
packWord15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packWord15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packWord15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};
static const int32_t tank_lives_positions[NUMBER_OF_LIVES] = {250, 270, 290};

// draws the tank near the center bottom of the screen
void tank_draw_initial() {
	int32_t x, y;
	for(x = 0; x < TANK_WIDTH; x++) {
		for(y = 0; y < TANK_HEIGHT; y++) {
			if(tank_15x8[y] & (BIT_MASK << x)) {
				screen_draw_double_pixel(x+globals_getTankPosition(),y+TANK_Y,SCREEN_GREEN);
			}
		}
	}
}

// draws 3 copies of the tank in the top right corner of the screen
// to represent the number of remaining lives
void tank_draw_lives_initial() {
	int32_t x, y, i;
	for(i = 0; i < NUMBER_OF_LIVES; i++) {
		for(x = 0; x < TANK_WIDTH; x++) {
			for(y = 0; y < TANK_HEIGHT; y++) {
				if(tank_15x8[y] & (BIT_MASK << x)) {
					screen_draw_double_pixel(x+tank_lives_positions[i],y+LIVES_Y,SCREEN_GREEN);
				}
			}
		}
	}
}

// moves the tank to the left
void tank_move_left() {

	int32_t x, y;
	if(globals_getTankPosition()-TANK_MOVEMENT < 0) return;
	for(y = 0; y < TANK_HEIGHT; y++) {
		for(x = 0; x < TANK_WIDTH+TANK_MOVEMENT; x++) {
			if((tank_15x8[y] & (BIT_MASK<<x)) != ((tank_15x8[y]<<TANK_MOVEMENT) & (BIT_MASK<<(x)))) { //some changes has occured to this pixel
					if(((tank_15x8)[y]<<TANK_MOVEMENT) &(BIT_MASK<<x)) {
						screen_draw_double_pixel(globals_getTankPosition()+TANK_WIDTH-X_OFFSET-x,y+TANK_Y,SCREEN_GREEN);
					}
					else {
						screen_draw_double_pixel(globals_getTankPosition()+TANK_WIDTH-X_OFFSET-x,y+TANK_Y,SCREEN_BLACK);
					}
			}
		}
	}
	globals_setTankPosition(globals_getTankPosition()-TANK_MOVEMENT);
}

// moves the tank to the right
void tank_move_right() {

	int32_t x, y;
	if(globals_getTankPosition()+TANK_MOVEMENT >= SCREEN_WIDTH - TANK_WIDTH) return;
	for(y = 0; y < TANK_HEIGHT; y++) {
		for(x = 0; x < TANK_WIDTH+TANK_MOVEMENT; x++) {
			if((tank_15x8[y] & (BIT_MASK<<x)) != ((tank_15x8[y]<<TANK_MOVEMENT) & (BIT_MASK<<(x)))) { //some changes has occured to this pixel
					if(((tank_15x8)[y]) &(BIT_MASK<<x)) {
						screen_draw_double_pixel(globals_getTankPosition()+TANK_MOVEMENT+TANK_WIDTH-X_OFFSET-x,y+TANK_Y,SCREEN_GREEN);
					}
					else {
						screen_draw_double_pixel(globals_getTankPosition()+TANK_MOVEMENT+TANK_WIDTH-X_OFFSET-x,y+TANK_Y,SCREEN_BLACK);
					}
			}
		}
	}
	globals_setTankPosition(globals_getTankPosition()+TANK_MOVEMENT);
}
