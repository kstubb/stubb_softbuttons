/* 
  File:   stubb_softbuttons.h
  Author: kyle
 
  Created on February 4, 2016, 10:28 AM
*/

/* 
  This is an add-on library for the RA8875 display driver (originally by
  Adafuit, ported to PIC by Kyle Stubbins (https://github.com/kstubb/Stubb_RA8875).
  This is a port of the add-on library for UTFT, by Rinky-Dink Electronics.
  See original header below.
*/
 
/*
  UTFT_Buttons.cpp - Add-on Library for UTFT: Buttons
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library adds simple but easy to use buttons to extend the use
  of the UTFT and UTouch libraries.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef STUBB_SOFTBUTTONS_H
#define	STUBB_SOFTBUTTONS_H

/** I N C L U D E S **********************************************************/

#include "stubb_globals.h"
#include "Stubb_RA8875.h"


/** T Y P E D E F S **********************************************************/

typedef struct
{
    uint16_t			pos_x; 
    uint16_t            pos_y;
    uint16_t            width;
    uint16_t            height;
    uint16_t			flags;
    char				*label;
    uint16_t            fill_color;
    uint16_t            border_color;
    uint16_t            hilite_color;
    uint16_t            text_color;
//  bitmapdatatype      data;
} button_type;

/** D E F I N E S ************************************************************/

#define MAX_BUTTONS	20	// Maximum number of buttons available at one time

// Define presets for button status
#define BUTTON_DISABLED			0x0001
#define BUTTON_SYMBOL			0x0002
#define BUTTON_SYMBOL_REP_3X	0x0004
#define BUTTON_BITMAP			0x0008	
#define BUTTON_NO_BORDER		0x0010
#define BUTTON_UNUSED			0x8000

#define NO_REDRAW               FALSE
#define REDRAW                  TRUE

/** E N U M E R A T I O N S **************************************************/


/** P R O T O T Y P E S ******************************************************/

void SoftButtons_initButtons(void);
int SoftButtons_addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags);
void SoftButtons_drawButtons(void);
void SoftButtons_drawButton (int8_t buttonID);
void SoftButtons_enableButton(int buttonID, uint8_t redraw);
void SoftButtons_disableButton(int buttonID, uint8_t redraw);
void SoftButtons_relabelButton(int buttonID, char *label, uint8_t redraw);
uint8_t SoftButtons_buttonEnabled(int buttonID);
void SoftButtons_deleteButton(int buttonID);
void SoftButtons_invertButton(int buttonID, uint8_t redraw);
void SoftButtons_setButtonColors(int buttonID, uint16_t fill, uint16_t border, uint16_t hilite, uint16_t text_color);
int8_t SoftButtons_checkButtons(uint16_t cal_x, uint16_t cal_y);

/* Private */
void _deleteAllButtons();


#endif	/* STUBB_SOFTBUTTONS_H */

