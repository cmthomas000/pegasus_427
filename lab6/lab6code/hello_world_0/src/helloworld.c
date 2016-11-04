/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "unistd.h"
#include "screen.h"
#include "globals.h"
#include "core.h"
#include "sound.h"
#include<stdint.h>

#define CHAR_TO_INT '0'
#define times_ten(x) (x)*10
int main()
{
	//int64_t idle_count;
	// calls initializer functions, then runs game core
	init_platform();
	screen_init();
	sound_init();
	globals_init();
	screen_clear();
	core_init();
	core_draw_initial();//run_test();
	core_run();

	char input;


	while(1) {
//		xil_printf("\r\nwhoa");

		input = getchar();
//		switch(input) {
//		case '1':
//			xil_printf("\r\nderp");
//			break;
//		}
		xil_printf("\r\nwhoa");
		xil_printf(input);

	}
	cleanup_platform();

	return 0;
}
