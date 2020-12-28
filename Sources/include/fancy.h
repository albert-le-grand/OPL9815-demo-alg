/*
 * fancy.h
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#ifndef FANCY_H_
#define FANCY_H_

typedef struct{
	char *item;                 // menu item string
	unsigned char *fnt_picture;	// holds the image to show
	void(* itemfunc)(void);     // function to be executed on selection of item
}fancymenus;

typedef struct{
	char *item;
	long value;
}fancyselmenus;


int fancymenu(  fancymenus  *items, int layers[], int max );
int KAfancymenu(  fancymenus  *items, int layers[], char* elements );
int fancyselmenu( fancyselmenus *items, int layers[], int max, long* value, int more );


#endif /* FANCY_H_ */
