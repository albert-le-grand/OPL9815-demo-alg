/*
 * fancy.c
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include\lib.h"
#include "include\fancy.h"
#include "include\utils.h"

// check uncheck images
#include "fnt\checked.fnt"
#include "fnt\uncheck.fnt"
#include "fnt\exitsign.fnt"

// special sign icons
#include "fnt\signs.fnt"

#define DISP_YOFFSET  12
#define DISP_XMAX	112
#define DISP_YMAX	64

#define IMG_X_OFF 	( (DISP_XMAX /2 ) -16 )

extern int battery_type;

//------------------------------------------------------------------
//	printinputbar
//  ============================
//	Displays scroll and center icon bar at selected row.
//------------------------------------------------------------------
void printinputbar(int y, int type) {
	setfont(USER_FONT, (const unsigned char*)_signs);
	gotoxy(0, y);
	printsymbol(3); // arrow up

	gotoxy(5, y);
	switch (type) {
	case 0: // select
		printsymbol(4);
		gotoxy(6, y);
		printsymbol(5);
		gotoxy(7, y);
		printsymbol(6);
		gotoxy(8, y);
		printsymbol(7);
		break;
	case 1: // BACK EXIT
		printsymbol(12);
		gotoxy(6, y);
		printsymbol(13);
		gotoxy(7, y);
		printsymbol(14);
		gotoxy(8, y);
		printsymbol(15);
		break;
	}

	gotoxy(12, y);
	printsymbol(2); // arrow down
}

//------------------------------------------------------------------
//	show_image
//  ============================
//	Displays image at selected x, y
//------------------------------------------------------------------
void show_image(int x, int y) {
	y--;
	setoffset(14);
	display_icon(x, y, 0);
	setoffset(0);
}

//------------------------------------------------------------------
//	showprogressbar
//  ============================
//	Displays progress bar
//------------------------------------------------------------------
static void showprogressbar(int curr, int max) {
	int stages;
	static int gap;
	curr--;
	stages = (DISP_YMAX - DISP_YOFFSET - 1 - 5) / (max - 1);
	drawline(DISP_XMAX - 2, gap + 1, DISP_XMAX - 2, gap + 5, 0); // remove prev gapline to the left
	setpixel(DISP_XMAX - 3, gap, 0);
	setpixel(DISP_XMAX - 3, gap + 6, 0);

	drawline(DISP_XMAX - 4, DISP_YOFFSET, DISP_XMAX - 4, DISP_YMAX - 1, 1); // top bottom line
	gap = (curr != (max - 1)) ?
			((curr * stages) + DISP_YOFFSET) : (DISP_YMAX - 7);
	drawline(DISP_XMAX - 4, gap + 1, DISP_XMAX - 4, gap + 5, 0); // small gap in line
	drawline(DISP_XMAX - 2, gap + 1, DISP_XMAX - 2, gap + 5, 1); // draw gapline a bit to the right
	setpixel(DISP_XMAX - 3, gap, 1);
	setpixel(DISP_XMAX - 3, gap + 6, 1);
}

//------------------------------------------------------------------
//	fancymenu
//  ============================
//	Menu for display and control of menu items.
//------------------------------------------------------------------
int fancymenu(fancymenus *items, int layers[], int max) {
	static int i;
	int key;
	char* ptr;

	static char layer[25];
	static char tmp[3];

	cursor(NOWRAP);
	memset(layer, '\0', sizeof(layer));
	// create string to show in what menu you are
	for (i = 0; i < 5; i++) {
		if (layers[i] == 0) {
			i--;
			break; // no more sub menus
		}

		if (strlen(layer) > 0)
			strcat(layer, "-"); // a submenu add a separator marker

		sprintf(tmp, "%d", layers[i]);
		strcat(layer, tmp);
	}

	if ((ptr = strrchr(layer, '-')))
		layer[ptr - layer] = '\0';
	else
		layer[0] = '\0';
	putchar('\f');
	resetkey();
	for (;;) {
		gotoxy(0, 0);
		setfont(SMALL_FONT, NULL);
		printf("%11.11s%c%-2d", layer, layer[0] ? '-' : ' ', layers[i]);

		setfont(LARGE_FONT, NULL);
		gotoxy(0, 0);
		setoffset(8);
		printf("%-13.13s ", items[(layers[i] - 1)].item);
		setoffset(0);

		if (items[(layers[i] - 1)].itemfunc == NULL) //Back-Exit
		{
			printinputbar(7, 1);

			setfont(USER_FONT, (const unsigned char *)_exitsign);
			show_image(5, 2);
		} else if (items[(layers[i] - 1)].fnt_picture) {
			printinputbar(7, 0);

			setfont(USER_FONT, (const unsigned char *)items[(layers[i] - 1)].fnt_picture);
			show_image(5, 2);
		}

		setfont(LARGE_FONT, NULL);
		showprogressbar(layers[i], max);

		switch ((key = KAwaitkey(battery_type))) //KA changed waitkey()
		{
		case CLR_KEY: // want to quit menu
			layers[i] = 0; // clear this menu layer
			return (-1);
		case ENT_KEY:
		case TRIGGER_KEY: // select item
			if (items[layers[i] - 1].itemfunc == NULL) // exit
			{
				layers[i] = 0;
				return (-1);
			}

			return (layers[i] - 1);
		case UP_KEY:
			if ((layers[i] - 1) > 0)
				layers[i]--;
			else
				layers[i] = max;
			break;
		case DOWN_KEY:
			if (layers[i] < max)
				layers[i]++;
			else
				layers[i] = 1;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if ((key - 0x30) <= max && (key - 0x30) < 0x0A) {
				layers[i] = (key - 0x30);
				if (items[layers[i] - 1].itemfunc == NULL) // back item
				{
					break; // stay on the last item this is the back item
				}
				return (layers[i] - 1);
			}
			errkeybeep();
			break;
		case TIMER_TIMEOUT:
			break;
		default:
			errkeybeep();
			break;
		}
	}
}

//------------------------------------------------------------------
//	KAfancymenu
//  ============================
//	Similar to fancymenu but with masking based on elements parameter.
//------------------------------------------------------------------
int KAfancymenu(fancymenus *items, int layers[], char* elements) {
#define LEFT   1
#define RIGHT  2
	static int i;
	int key;
	char* ptr;
	int index = 0; //KA
	int max; //KA
	int direction; //KA

	static char layer[25];
	static char tmp[3];

	cursor(NOWRAP);
	memset(layer, '\0', sizeof(layer));

	max = strlen(elements); //number of items in the menu

	// create string to show in what menu you are
	for (i = 0; i < 5; i++) {
		if (layers[i] == 0) {
			i--;
			break; // no more sub menus
		}

		if (strlen(layer) > 0)
			strcat(layer, "-"); // a submenu add a separator marker

		sprintf(tmp, "%d", layers[i]);
		strcat(layer, tmp);
	}

	if ((ptr = strrchr(layer, '-')))
		layer[ptr - layer] = '\0';
	else
		layer[0] = '\0';
	putchar('\f');
	resetkey();
	index = 0;
	direction = RIGHT;

	for (;;) {
		//display menu items

		if (elements[index] == '0') //skip to next element
				{
			if (direction == LEFT) {
				if ((layers[i] - 1) > 0) {
					layers[i]--;
					index = layers[i] - 1;
				} else {
					layers[i] = max;
					index = layers[i] - 1;
				}
			} else {
				if (layers[i] < max) {
					layers[i]++; //move to next element in list
					index = layers[i] - 1; //move index as well
				} else //at max
				{
					layers[i] = 1;
					index = 0;
				}
			}
			continue;
		} //if element = 0

		gotoxy(0, 0);
		setfont(SMALL_FONT, NULL);
		//	printf("%d",index);   //KA
		printf("%11.11s%c%-2d", layer, layer[0] ? '-' : ' ', layers[i]);

		setfont(LARGE_FONT, NULL);
		gotoxy(0, 0);
		setoffset(8);
		printf("%-13.13s ", items[(layers[i] - 1)].item);
		setoffset(0);

		if (items[(layers[i] - 1)].itemfunc == NULL) //Back-Exit
		{
			printinputbar(7, 1); //UP BACK DWN

			setfont(USER_FONT, (const unsigned char *)_exitsign);
			show_image(5, 2);
		} else if (items[(layers[i] - 1)].fnt_picture) {
			printinputbar(7, 0); //UP SELECT DWN

			setfont(USER_FONT, (const unsigned char *)items[(layers[i] - 1)].fnt_picture);
			show_image(5, 2);
		}

		setfont(LARGE_FONT, NULL);
		showprogressbar(layers[i], max);

		switch ((key = KAwaitkey(battery_type))) //KA changed waitkey
		{
		case CLR_KEY: // want to quit menu
			layers[i] = 0; // clear this menu layer
			return (-1);
		case ENT_KEY:
		case TRIGGER_KEY: // select item
			if (items[layers[i] - 1].itemfunc == NULL) // exit
			{
				layers[i] = 0;
				return (-1);
			}

			return (layers[i] - 1);
		case UP_KEY:
			direction = LEFT;
			if ((layers[i] - 1) > 0) {
				layers[i]--;
				index = layers[i] - 1;
			} else {
				layers[i] = max;
				index = layers[i] - 1;
			}
			break;
		case DOWN_KEY:
			direction = RIGHT;
			if (layers[i] < max) {
				layers[i]++;
				index = layers[i] - 1;
			} else {
				layers[i] = 1;
				index = layers[i] - 1;
			}
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if ((key - 0x30) <= max && (key - 0x30) < 0x0A) {
				layers[i] = (key - 0x30);
				if (items[layers[i] - 1].itemfunc == NULL) // back item
				{
					break; // stay on the last item this is the back item
				}
				return (layers[i] - 1);
			}
			errkeybeep();
			break;
		default:
			errkeybeep();
			break;
		} //switch

	} //main for
}

//------------------------------------------------------------------
//	fancyselmenu
//  ============================
//	Selectable list menu.
//------------------------------------------------------------------
int fancyselmenu(fancyselmenus *items, int layers[], int max, long* value,
		int more) {
	static int i;
	char* ptr;
	int key;

	static char layer[25];
	static char tmp[3];

	cursor(NOWRAP);
	memset(layer, '\0', sizeof(layer));
	// create string to show in what menu you are
	for (i = 0; i < 5; i++) {
		if (layers[i] == 0) {
			i--;
			break; // no more sub menus
		}

		if (strlen(layer) > 0)
			strcat(layer, "-"); // a submenu add a separator marker

		sprintf(tmp, "%d", layers[i]);
		strcat(layer, tmp);
	}

	if ((ptr = strrchr(layer, '-')))
		layer[ptr - layer] = '\0';
	else
		layer[0] = '\0';
	putchar('\f');
	resetkey();
	for (;;) {

		gotoxy(0, 0);
		setfont(SMALL_FONT, NULL);
		printf("%11.11s%c%-2d", layer, layer[0] ? '-' : ' ', layers[i]);

		setfont(LARGE_FONT, NULL);
		gotoxy(0, 0);
		setoffset(8);
		printf("%-13.13s ", items[(layers[i] - 1)].item);
		setoffset(0);

		if (items[layers[i] - 1].value == -1) { // display exit sign
			printinputbar(7, 1);
			setfont(USER_FONT, (const unsigned char *)_exitsign);
		} else if ((more && (items[layers[i] - 1].value & *value))
				|| (!more && (items[layers[i] - 1].value == *value))) {
			printinputbar(7, 0);
			setfont(USER_FONT, (const unsigned char *)_checked);
			//display_icon( IMG_X_OFF, 24, (char*)_checked );
		} else {
			printinputbar(7, 0);
			setfont(USER_FONT, (const unsigned char *)_uncheck);
		}

		show_image(5, 2);
		setfont(LARGE_FONT, NULL);

		showprogressbar(layers[i], max);

		switch ((key = KAwaitkey(battery_type))) //KA changed waitkey()
		{
		case CLR_KEY: // want to quit menu
			layers[i] = 0; // clear this menu layer
			return (-1);
		case ENT_KEY:
		case TRIGGER_KEY: // select item
			if (items[layers[i] - 1].value == -1) {
				layers[i] = 0;
				return (-1);
			}

			if (more)
				*value ^= items[layers[i] - 1].value;
			else
				*value = items[layers[i] - 1].value;

			break;
		case UP_KEY:
			if ((layers[i] - 1) > 0)
				layers[i]--;
			else
				layers[i] = max;
			break;
		case DOWN_KEY:
			if (layers[i] < max)
				layers[i]++;
			else
				layers[i] = 1;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':

			if ((key - 0x30) <= max && (key - 0x30) < 0x0A) {
				layers[i] = (key - 0x30);
				if (layers[i] == 1)
					break;

				if (more)
					*value ^= items[layers[i] - 1].value;
				else
					*value = items[layers[i] - 1].value;
				break;

			}
			errkeybeep();
			break;

		default:
			errkeybeep();
			break;
		}
	}
}

//------------------------------------------------------------------
//	KAfancyselmenu
//  ============================
//	Selectable list menu with masking based on elements parameter.
//------------------------------------------------------------------
int KAfancyselmenu(fancyselmenus *items, int layers[], char* elements, long* value, int more) 
{
	#define LEFT   1
	#define RIGHT  2

	static int i;
	char* ptr;
	int key;
	int max; //KA
	int index; //KA
	int direction; //KA

	static char layer[25];
	static char tmp[3];

	cursor(NOWRAP);
	memset(layer, '\0', sizeof(layer));

	max = strlen(elements); //number of items in the menu
	// create string to show in what menu you are
	for (i = 0; i < 5; i++) {
		if (layers[i] == 0) {
			i--;
			break; // no more sub menus
		}

		if (strlen(layer) > 0)
			strcat(layer, "-"); // a submenu add a separator marker

		sprintf(tmp, "%d", layers[i]);
		strcat(layer, tmp);
	}

	if ((ptr = strrchr(layer, '-')))
		layer[ptr - layer] = '\0';
	else
		layer[0] = '\0';
	putchar('\f');
	resetkey();

	direction = RIGHT;
	index = 0;
	for (;;) {

		//display menu items

		if (elements[index] == '0') //skip to next element
				{
			if (direction == LEFT) {
				if ((layers[i] - 1) > 0) {
					layers[i]--;
					index = layers[i] - 1;
				} else {
					layers[i] = max;
					index = layers[i] - 1;
				}
			} else {
				if (layers[i] < max) {
					layers[i]++; //move to next element in list
					index = layers[i] - 1; //move index as well
				} else //at max
				{
					layers[i] = 1;
					index = 0;
				}
			}
			continue;
		} //if element = 0

		gotoxy(0, 0);
		setfont(SMALL_FONT, NULL);
		printf("%11.11s%c%-2d", layer, layer[0] ? '-' : ' ', layers[i]);

		setfont(LARGE_FONT, NULL);
		gotoxy(0, 0);
		setoffset(8);
		printf("%-13.13s ", items[(layers[i] - 1)].item);
		setoffset(0);

		if (items[layers[i] - 1].value == -1) { // display exit sign
			printinputbar(7, 1);
			setfont(USER_FONT, (const unsigned char *)_exitsign);
		} else if ((more && (items[layers[i] - 1].value & *value))
				|| (!more && (items[layers[i] - 1].value == *value))) {
			printinputbar(7, 0); //UP SELECT DWN
			setfont(USER_FONT, (const unsigned char *)_checked);
			//display_icon( IMG_X_OFF, 24, (char*)_checked );
		} else {
			printinputbar(7, 0);
			setfont(USER_FONT, (const unsigned char *)_uncheck);
		}

		show_image(5, 2);
		setfont(LARGE_FONT, NULL);

		showprogressbar(layers[i], max);

		switch ((key = KAwaitkey(battery_type))) //KA changed waitkey
		{
		case CLR_KEY: // want to quit menu
			layers[i] = 0; // clear this menu layer
			return (-1);
		case ENT_KEY:
		case TRIGGER_KEY: // select item
			if (items[layers[i] - 1].value == -1) {
				layers[i] = 0;
				return (-1);
			}

			if (more)
				*value ^= items[layers[i] - 1].value;
			else
				*value = items[layers[i] - 1].value;

			break;
		case UP_KEY:
			direction = LEFT;
			if ((layers[i] - 1) > 0) {
				layers[i]--;
				index = layers[i] - 1;
			} else {
				layers[i] = max;
				index = layers[i] - 1;
			}
			break;
		case DOWN_KEY:
			direction = RIGHT;
			if (layers[i] < max) {
				layers[i]++;
				index = layers[i] - 1;
			} else {
				layers[i] = 1;
				index = layers[i] - 1;
			}
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':

			if ((key - 0x30) <= max && (key - 0x30) < 0x0A) {
				layers[i] = (key - 0x30);
				if (layers[i] == 1)
					break;

				if (more)
					*value ^= items[layers[i] - 1].value;
				else
					*value = items[layers[i] - 1].value;
				break;

			}
			errkeybeep();
			break;

		default:
			errkeybeep();
			break;
		}
	}
}

