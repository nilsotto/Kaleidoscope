#pragma once
#include <Arduino.h>


#include "EventDispatcher.h"
#include KALEIDOSCOPE_HARDWARE_H

/** KALEIDOSCOPE_HARDWARE_H is expected to define HARDWARE_EVENT_DISPATCHER
 * if it has registered a dispatcher.  The goal is to remove this once
 * all of the hardware implementations have been updated to register
 * their dispatchers. */
#ifndef HARDWARE_EVENT_DISPATCHER
#include "HIDEventDispatcher.h"
extern HIDEventDispatcher defaultHIDDispatcher;
#endif

#include "key_defs.h"
#include "keyswitch_state.h"

extern const Key keymaps[][ROWS][COLS];


// Code can use this macro on injected key events to signal that
// the event isn't tied to a specific physical keyswitch
#define UNKNOWN_KEYSWITCH_LOCATION 255,255

// sending events to the computer
/* The event handling starts with the Scanner calling handleKeyswitchEvent() for
 * every key in the matrix, and it is the task of this method to figure out what
 * to do, it is the main entry point.
 *
 * This function will iterate through an array of handler functions, and stop as
 * soon as one of them signals that the event has been handled. To make it
 * possible to inject synthetic events, one can call handleKeyswitchEvent from
 * within a custom handler (making the event handling recursive), with a
 * different keycode.
 *
 * This is useful for example for one-shot modifiers, where we would like to
 * temporarily disable the one-shot functionality, and have them work as a
 * normal modifier instead. In this case, the keymap would contain a key with
 * OSM flags set, and the event handler would remove the OSM flags, and let the
 * system handle the key as it would have, without the OSM flags. So we simply
 * clear the flags, and call handleKeyswitchEvent again, with the modifier keycode
 * as the first argument. This way, we could insert an event, and have the whole
 * chain re-process it, instead of registering the keycode ourselves with HID
 * ourselves. Injecting allows any and all custom handlers to have a chance,
 * too.
 *
 * For this reason, the handleKeyswitchEvent receives four arguments: the mapped key
 * (or Key_NoKey if we do not want to override what is in the keymap), the row
 * and column of the key, so we can look up the code for it, and the current and
 * previous state of the key, so we can determine what the event is. The
 * currentState may be flagged INJECTED, which signals that the event was
 * injected, and is not a direct result of a keypress, coming from the scanner.
 */
void handleKeyswitchEvent(Key mappedKey, byte row, byte col, uint8_t keyState);

/** Flushes any pending regular key switch events and sends them out */
void sendKeyboardReport();

// A facade on top of our HID implementation
void pressKey(Key mappedKey);
void releaseKey(Key mappedKey);
void pressKeyRaw(Key mappedKey);
void releaseKeyRaw(Key mappedKey);

void pressConsumer(Key mappedKey);
void releaseConsumer(Key mappedKey);

void pressSystem(Key mappedKey);
void releaseSystem(Key mappedKey);

void moveMouse(signed char x, signed char y, signed char wheel);
void clickMouseButtons(uint8_t buttons);
void setMouseButtons(uint8_t buttons);
void pressMouseButtons(uint8_t buttons);
void releaseMouseButtons(uint8_t buttons);
void moveAbsoluteMouse(signed char x, signed char y, signed char wheel);
void moveAbsoluteMouseTo(uint16_t x, uint16_t y, signed char wheel);
void clickAbsoluteMouseButtons(uint8_t buttons);
void setAbsoluteMouseButtons(uint8_t buttons);
void pressAbsoluteMouseButtons(uint8_t buttons);
void releaseAbsoluteMouseButtons(uint8_t buttons);


