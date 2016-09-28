/*
 * aliens.c
 *
 *  Created on: Sep 27, 2016
 *      Author: superman
 */
#include "aliens.h"
#include "screen.h"
#include <stdbool.h>
#include<stdio.h>

#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
		((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
				(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define TOP_ROW 11
#define MIDDLE_ROW 33
#define BOTTOM_ROW 55
#define ALIENS_PER_ROW 11
#define ALIEN_SPACING 16
#define BUFFER_WIDTH 10
#define ALIEN_BLOCK_HEIGHT 5*ALIEN_SPACING
#define ALIEN_BLOCK_WIDTH ALIENS_PER_ROW*ALIEN_SPACING
#define ALIEN_MOVEMENT 4

static bool alien_legs_in[GLOBALS_NUMBER_OF_ALIENS];

static const int alien_explosion_12x10[] =
{
		packword12(0,0,0,0,0,0,1,0,0,0,0,0),
		packword12(0,0,0,1,0,0,1,0,0,0,1,0),
		packword12(1,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,1,0,0,1,0,0,0,1,0,0,0),
		packword12(0,0,0,0,0,0,0,0,0,0,1,1),
		packword12(1,1,0,0,0,0,0,0,0,0,0,0),
		packword12(0,0,0,1,0,0,0,1,0,0,1,0),
		packword12(0,0,1,0,0,0,0,0,1,0,0,1),
		packword12(0,1,0,0,0,1,0,0,1,0,0,0),
		packword12(0,0,0,0,0,1,0,0,0,0,0,0)
};

static const int alien_top_in_12x8[] =
{
		packword12(0,0,0,0,0,1,1,0,0,0,0,0),
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(0,0,1,1,1,1,1,1,1,1,0,0),
		packword12(0,0,0,1,0,1,1,0,1,0,0,0),
		packword12(0,0,1,0,0,0,0,0,0,1,0,0),
		packword12(0,0,0,1,0,0,0,0,1,0,0,0)
};

static const int alien_top_out_12x8[] =
{
		packword12(0,0,0,0,0,1,1,0,0,0,0,0),
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(0,0,1,1,1,1,1,1,1,1,0,0),
		packword12(0,0,0,0,1,0,0,1,0,0,0,0),
		packword12(0,0,0,1,0,1,1,0,1,0,0,0),
		packword12(0,0,1,0,1,0,0,1,0,1,0,0)
};

static const int alien_middle_in_12x8[] =
{
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,0,0,0,1,0,0,0,1,0,0,0),
		packword12(0,0,0,1,1,1,1,1,1,1,0,0),
		packword12(0,0,1,1,0,1,1,1,0,1,1,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,1,0,1,0,0,0,0,0,1,0,1),
		packword12(0,0,0,0,1,1,0,1,1,0,0,0)
};

static const int alien_middle_out_12x8[] =
{
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,1,0,0,1,0,0,0,1,0,0,1),
		packword12(0,1,0,1,1,1,1,1,1,1,0,1),
		packword12(0,1,1,1,0,1,1,1,0,1,1,1),
		packword12(0,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,1,1,1,1,1,1,1,1,1,0),
		packword12(0,0,0,1,0,0,0,0,0,1,0,0),
		packword12(0,0,1,0,0,0,0,0,0,0,1,0)
};

static const int alien_bottom_in_12x8[] =
{
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,0),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(1,1,1,0,0,1,1,0,0,1,1,1),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,1,1,1,0,0,1,1,1,0,0),
		packword12(0,1,1,0,0,1,1,0,0,1,1,0),
		packword12(0,0,1,1,0,0,0,0,1,1,0,0)
};

static const int alien_bottom_out_12x8[] =
{
		packword12(0,0,0,0,1,1,1,1,0,0,0,0),
		packword12(0,1,1,1,1,1,1,1,1,1,1,0),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(1,1,1,0,0,1,1,0,0,1,1,1),
		packword12(1,1,1,1,1,1,1,1,1,1,1,1),
		packword12(0,0,0,1,1,0,0,1,1,0,0,0),
		packword12(0,0,1,1,0,1,1,0,1,1,0,0),
		packword12(1,1,0,0,0,0,0,0,0,0,1,1)
};

void aliens_draw_initial() {
	int i;
	int x, y;
	point_t position;
	for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
		if(!globals_isDeadAlien(i)) { // alien is alive
			position.x = globals_getAlienBlockPosition().x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
			position.y = globals_getAlienBlockPosition().y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
			for(x = 0; x < ALIEN_WIDTH; x++) {
				for(y = 0; y < ALIEN_HEIGHT; y++) {
					alien_legs_in[i] = 1; // 1 for in
					if(i < TOP_ROW) {
						if(alien_top_in_12x8[y] & (1 << x)) {
							screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_WHITE);
						}
					}
					else if (i < MIDDLE_ROW) {
						if(alien_middle_in_12x8[y] & (1 << x)) {
							screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_WHITE);
						}
					}
					else  {//if (i < BOTTOM_ROW) {
						if(alien_bottom_in_12x8[y] & (1 << x)) {
							screen_draw_double_pixel(x+position.x,y+position.y,SCREEN_WHITE);
						}
					}
				}
			}
		}
	}
}

void aliens_update_position() {
	//static bool moved_down = true;
	static bool moving_left = false; // start moving right
	point_t blockposition = globals_getAlienBlockPosition();
	point_t position;
	if ((blockposition.x <= BUFFER_WIDTH && moving_left) || (blockposition.x >= SCREEN_WIDTH-ALIEN_BLOCK_WIDTH-BUFFER_WIDTH && !moving_left)) { // on side, needs to move down;
		//move down
	}
	else if(moving_left) {
		//move left
	}
	else {
		int x, y, i;
		for(i = 0; i < GLOBALS_NUMBER_OF_ALIENS; i++) {
			if(!globals_isDeadAlien(i)) { //alien is alive, so it must move

				position.x = blockposition.x + ALIEN_SPACING*(i%ALIENS_PER_ROW);
				position.y = blockposition.y + ALIEN_SPACING*(i/ALIENS_PER_ROW);
				xil_printf("\r\nposition x = %d y = %d ",position.x,position.y);
				for(y = 0; y < ALIEN_HEIGHT; y++) {
					for(x = 0; x < ALIEN_WIDTH+ALIEN_MOVEMENT; x++) {
						if(i < TOP_ROW) {
							if(alien_legs_in[i]) { // switch from legs in to legs out
								int old_alien_line = alien_top_in_12x8[y] << ALIEN_MOVEMENT;
								int new_alien_line = alien_top_out_12x8[y];
								xil_printf("redraw alien %d\r\n",i);
								xil_printf("x = %d %x ?= %x\r\n",x,(new_alien_line & (1<<x)), (old_alien_line & (1<<x)) );
								if((new_alien_line & (1<<x)) != (old_alien_line & (1<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(1<<x)) {
										xil_printf("\r\nprint %d, %d ", position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y,SCREEN_WHITE);
										xil_printf("after draw x = %d\r\n",x);
										//	xil_printf("\r\nColumn %d",y);

									}
									else {
										xil_printf("\r\nblank %d, %d ", position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y,SCREEN_BLACK);
										xil_printf("after blank x = %d\r\n",x);

									}
									unsigned int i;
									for(i = 0; i < 100000; i++);
								}

								/*if((new_alien_line & (1<<x)) != (old_alien_line & (1<<x))) { //some changes has occured to this pixel
									if((new_alien_line) &(1<<x)) {
										xil_printf("\r\nprint %d, %d ", position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y);
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y,SCREEN_WHITE);
										xil_printf("\r\nColumn %d",y);

									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_MOVEMENT+ALIEN_WIDTH-1-x,y+position.y,SCREEN_BLACK);

									}
									unsigned int i;
																			for(i = 0; i < 100000; i++);
								}*/
							}
							else { // switch from legs out to legs in
								if((alien_top_in_12x8[y] & (1<<x)) != ((alien_top_out_12x8[y]<<ALIEN_MOVEMENT) & (1<<(x)))) { //some changes has occured to this pixel
									if(((alien_top_in_12x8)[y]) &(1<<x)) {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-1-x,y+position.y,SCREEN_WHITE);
									}
									else {
										screen_draw_double_pixel(position.x+ALIEN_WIDTH-1-x,y+position.y,SCREEN_BLACK);
									}
								}
							}
						}
						alien_legs_in[i] = !alien_legs_in[i];
					}
				}
			}
		}
		blockposition.x += ALIEN_MOVEMENT;
		globals_setAlienBlockPosition(blockposition);
		//move right
	}
}