/* 
  File:   stubb_softbuttons.c
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

/** I N C L U D E S **********************************************************/

#include "stubb_softbuttons.h"


/** V A R I A B L E S ********************************************************/

uint16_t _color_text, _color_text_inactive, _color_background, _color_border, _color_hilite;
uint8_t  *_font_text, *_font_symbol;
button_type	buttons[MAX_BUTTONS];


/** F U N C T I O N S ********************************************************/

//void SoftButtons_initButtons(UTFT *ptrUTFT, UTouch *ptrUTouch)
void SoftButtons_initButtons(void)
{
	_deleteAllButtons();
	_color_text				= RA8875_WHITE;
	_color_text_inactive	= RA8875_GREY;
	_color_background		= RA8875_BLUE;
	_color_border			= RA8875_WHITE;
	_color_hilite			= RA8875_LTBLUE;
	_font_text				= NULL;
	_font_symbol			= NULL;
}


int SoftButtons_addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags)
{
    int btcnt = 0;
  
	while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) && (btcnt < MAX_BUTTONS))
		btcnt++;
  
	if (btcnt == MAX_BUTTONS)
		return -1;
	else
	{
		buttons[btcnt].pos_x  = x;
		buttons[btcnt].pos_y  = y;
		buttons[btcnt].width  = width;
		buttons[btcnt].height = height;
		buttons[btcnt].flags  = flags | BUTTON_BITMAP;
		buttons[btcnt].label  = label;
//		buttons[btcnt].data   = NULL;
        
        /* we don't pass in color values, so we'll
         * set some default button colors  here
        */
        buttons[btcnt].fill_color = _color_background;
        buttons[btcnt].border_color = _color_border;
        buttons[btcnt].hilite_color = _color_hilite;
        buttons[btcnt].text_color = _color_text;
		return btcnt;
	}
    
}


/* Draws all buttons at once */
void SoftButtons_drawButtons(void)
{
	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		if ((buttons[i].flags & BUTTON_UNUSED) == 0)
			SoftButtons_drawButton(i);
	}
}


/* Draws a specific button */
void SoftButtons_drawButton(int8_t buttonID)
{
    int text_x, text_y;
    
    // fill a rounded rectangle
    RA8875_fillRoundedRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, 15, 15, buttons[buttonID].fill_color);
    // draw a rounded rectangle
    RA8875_drawRoundedRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, 15, 15, buttons[buttonID].border_color);
    
    // draw the button highlight
    RA8875_fillRoundedRect(buttons[buttonID].pos_x + 5, buttons[buttonID].pos_y + 5, buttons[buttonID].width - 10, buttons[buttonID].height / 2, 15, 15, buttons[buttonID].hilite_color);
    
    // set Text Mode
    RA8875_textMode();
    // set the text color (can be checked for active / inactive)
    if (buttons[buttonID].flags & BUTTON_DISABLED)
        RA8875_textTransparent(_color_text_inactive);
    else
        RA8875_textTransparent(buttons[buttonID].text_color);
    /* set the text size */
    RA8875_textEnlarge(RA8875_TEXT_LARGE);
    
    // set button icon / image and / or text
    text_x = ((buttons[buttonID].width / 2) - ((strlen(buttons[buttonID].label) * 24)/2)) + buttons[buttonID].pos_x;
    text_y = (buttons[buttonID].height / 2) - (24 / 2) + buttons[buttonID].pos_y;
    
    RA8875_textSetCursor(text_x, text_y);
    RA8875_textWrite(buttons[buttonID].label, 0);
    
    // return to Graphics Mode
    RA8875_graphicsMode();

}


void SoftButtons_enableButton(int buttonID, uint8_t redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_DISABLED;
		if (redraw)
			SoftButtons_drawButton(buttonID);
	}
}

void SoftButtons_disableButton(int buttonID, uint8_t redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_DISABLED;
		if (redraw)
			SoftButtons_drawButton(buttonID);
	}
}

void SoftButtons_relabelButton(int buttonID, char *label, uint8_t redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].label = label;
		if (redraw)
			SoftButtons_drawButton(buttonID);
	}
}

uint8_t SoftButtons_buttonEnabled(int buttonID)
{
	return !(buttons[buttonID].flags & BUTTON_DISABLED);
}


void SoftButtons_deleteButton(int buttonID)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
		buttons[buttonID].flags = BUTTON_UNUSED;
}


void SoftButtons_invertButton(int buttonID, uint8_t redraw)
{
    uint16_t fill, outline, text, hilite;
    
    if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
        fill = buttons[buttonID].fill_color;
        outline = buttons[buttonID].border_color;
        text = buttons[buttonID].text_color;
        hilite = buttons[buttonID].hilite_color;
        
        buttons[buttonID].fill_color = outline;
        buttons[buttonID].border_color = fill;
        buttons[buttonID].text_color = fill;
        
        switch (buttons[buttonID].fill_color)
        {
            case RA8875_WHITE:
                buttons[buttonID].hilite_color = RA8875_LTGREY;
                break;
                
            case RA8875_RED:
                buttons[buttonID].hilite_color = RA8875_LTRED;
                break;
                
            default:
                buttons[buttonID].hilite_color = RA8875_LTBLUE;
                break;
        }
    }
    
    if (redraw)
			SoftButtons_drawButton(buttonID);
}

void SoftButtons_setButtonColors(int buttonID, uint16_t fill, uint16_t border, uint16_t hilite, uint16_t text_color)
{
    if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
        buttons[buttonID].fill_color = fill;
        buttons[buttonID].border_color = border;
        buttons[buttonID].hilite_color = hilite;
        buttons[buttonID].text_color = text_color;
    }
}


/**************************************************************************/
/*!
    @brief  Scans the array of soft buttons to detect buttons presses

    @param[in] displayPtr  Pointer to the tsPoint_t object that holds
                           the compensated pixel location on the display
    @returns   -1 = no button press, otherwise we return the button ID 
                                     (0 - (MAX_BUTTONS -1))
 */
int8_t SoftButtons_checkButtons(uint16_t cal_x, uint16_t cal_y)
{
    for (uint8_t i = 0; i < MAX_BUTTONS; i++) {    
        /* Make sure the button is being used and is enabled */
        if (!(buttons[i].flags & (BUTTON_UNUSED | BUTTON_DISABLED))) {
            /* Are we in the horizontal (X) range? */
            if (((uint16_t)cal_x > buttons[i].pos_x) && ((uint16_t)cal_x < buttons[i].pos_x + buttons[i].width)) {
                /* Yes, are we in the vertical (Y) range? */
                if (((uint16_t)cal_y > buttons[i].pos_y) && ((uint16_t)cal_y < buttons[i].pos_y + buttons[i].height)) {
                    /* We have a button press, return its ID */
                    return i;
                }
            }
        }
    }
    return -1;      // no button found
}

/* Private */

void _deleteAllButtons()
{
	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		buttons[i].pos_x = 0;
		buttons[i].pos_y = 0;
		buttons[i].width = 0;
		buttons[i].height = 0;
		buttons[i].flags = BUTTON_UNUSED;
		buttons[i].label = (unsigned char *)"";
        buttons[i].fill_color = 0;
        buttons[i].border_color = 0;
        buttons[i].hilite_color = 0;
        buttons[i].text_color = 0;
	}
}

