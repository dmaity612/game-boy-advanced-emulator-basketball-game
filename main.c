#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/garbage.h"
#include "images/court.h"
#include "images/basketball.h"
#include "images/icon.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  WAIT_FOR_INPUT,
  PLAY,
  RULE,
  WIN,
  LOSE,
  ADD_ICON,
  MOVE,
  MOVED1,
  MOVED2,
  ADD_RECTANGLE,
  MOVED3,
  WIN1
};
int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;
  // const u16 *uga = &(garbage[0]);
  const u16 *courtPic = &(court[0]);
  const u16 *basketballPic = &(basketball[0]);
  const u16 *iconPic = &(icon[0]);
  BALL ball = {80, 115};
  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state) {
      case START:
      drawFullScreenImageDMA(basketballPic);
      drawString(50, 50, "Hit Enter to Start", GREEN);
      state = WAIT_FOR_INPUT;
        break;
      case WAIT_FOR_INPUT:
      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
            {
                state = PLAY;
            }
        // state = ?
        break;
      case PLAY:
      drawFullScreenImageDMA(courtPic);
      state = RULE;
      break;
      case RULE:
      drawString(2, 10, "Reach the green rectangle.", RED);
      drawString (20, 10, "Pixel from x pos: 65", RED);
      drawString (30, 10, "Pixels from y pos: 100", RED);
      state = ADD_ICON;
      break;
      case ADD_ICON:
      drawImageDMA(80, 115, 10, 10, iconPic);
      state = ADD_RECTANGLE;
      break;
      case ADD_RECTANGLE:
      drawRectDMA(15, 215, 20, 20, GREEN);
      state = MOVE;
      break;
      case MOVE:
        if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)&& ball.col < 220)
          {
            ball.col += 10;
            state = MOVED1;
          }
        if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons) && ball.col > 10)
          {
            ball.col -= 10;
            state = MOVED1;
          }
        if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons) && ball.row < 150)
          {
            ball.row += 10;
            state = MOVED1;
          }
        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons) && ball.row > 0)
          {
            ball.row -= 10;
            state = MOVED1;
          }          
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
            {
                state = START;
                ball.row = 80;
                ball.col = 115;
            }
        break;
      case MOVED1:
        drawFullScreenImageDMA(courtPic);
        state = MOVED2;
        break;
      case MOVED2:
        drawImageDMA(ball.row, ball.col, 10, 10, iconPic);
        state = MOVED3;
        break;
      case MOVED3:
      drawRectDMA(15, 215, 20, 20, GREEN);
      int x = ball.row - 15;
      int y = 215 - ball.col;
      char dest[100];
      char dest1[100];
      sprintf(dest, "%d", x);
      sprintf(dest1, "%d", y);
      drawString (20, 10, "Pixel from x pos: ", RED);
      drawString (20, 120, dest, RED);
      drawString (30, 10, "Pixels from y pos: ", RED);
      drawString (30, 120, dest1, RED);

      if (ball.row >= 15 && ball.row <= 25 && ball.col >= 215 && ball.row <= 225) {
        state = WIN;
      } else {
        state = MOVE;
      }
      break;
      case WIN:
      fillScreenDMA(GREEN);
        state = WIN1;
        break;
      case WIN1:
      drawString(50, 50, "You Won the GAME", RED);
      if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
            {
                state = START;
                ball.row = 80;
                ball.col = 115;
            }
      break;
      case LOSE:

        // state = ?
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }
  return 0;
}