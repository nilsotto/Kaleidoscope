#include "layers.h"
#include "key_events.h"

static uint8_t DefaultLayer;
static uint32_t LayerState;

static void handle_keymap_key_event(Key keymapEntry, uint8_t keyState) {
    if (keymapEntry.rawKey >= MOMENTARY_OFFSET) {
        if (key_toggled_on(keyState)) {
            if ( keymapEntry.rawKey == KEYMAP_NEXT) {
                Layer.next();
            } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
                Layer.previous();
            } else {
                Layer.on(keymapEntry.rawKey - MOMENTARY_OFFSET);
            }
        }
        if (key_toggled_off(keyState)) {
            if ( keymapEntry.rawKey == KEYMAP_NEXT) {
                Layer.previous();
            } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
                Layer.next();
            } else {
                Layer.off(keymapEntry.rawKey - MOMENTARY_OFFSET);
            }
        }

        // switch keymap and stay there
    } else if (key_toggled_on(keyState)) {
        Layer.on (keymapEntry.rawKey);
    }
}

static bool
layerEventHandler(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.flags != (SYNTHETIC | SWITCH_TO_KEYMAP))
        return false;

    handle_keymap_key_event(mappedKey, keyState);
    return true;
}

Layer_::Layer_ (void) {
}

void Layer_::begin (void) {
    defaultLayer (0);
    event_handler_hook_add (layerEventHandler);
}

Key Layer_::lookup(byte row, byte col) {
    Key mappedKey;
    int8_t layer = 31;

    mappedKey.raw = Key_Transparent.raw;

    while (mappedKey.raw == Key_Transparent.raw &&
           layer >= DefaultLayer) {
        if (Layer.isOn (layer))
            mappedKey.raw = pgm_read_word(&(keymaps[layer][row][col]));
        layer--;
    }

    return mappedKey;
}

uint8_t Layer_::top (void) {
    for (int8_t i = 31; i >= 0; i--) {
        if (bitRead (LayerState, i))
            return i;
    }
    return 0;
}

void Layer_::move (uint8_t layer) {
    LayerState = 0;
    on (layer);
}

void Layer_::on (uint8_t layer) {
    bitSet (LayerState, layer);
}

void Layer_::off (uint8_t layer) {
    bitClear (LayerState, layer);
}

boolean Layer_::isOn (uint8_t layer) {
    return bitRead(LayerState, layer);
}

void Layer_::next (void) {
    on (top () + 1);
}

void Layer_::previous (void) {
    off (top ());
}

void Layer_::defaultLayer (uint8_t layer) {
    move (layer);
    DefaultLayer = layer;
}

uint8_t Layer_::defaultLayer (void) {
    return DefaultLayer;
}

uint32_t Layer_::getLayerState (void) {
    return LayerState;
}

Layer_ Layer;
