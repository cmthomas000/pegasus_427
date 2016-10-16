/*
 * core.h
 *
 *  Created on: Oct 13, 2016
 *      Author: superman
 */

#ifndef CORE_H_
#define CORE_H_

// initialize core game elements, such as gpio and timer
void core_init();

// draw initial game elements
void core_draw_initial();

// start your engines! but really, enables interrupts to start the game moving.
void core_run();

void core_end_game();

#endif /* CORE_H_ */