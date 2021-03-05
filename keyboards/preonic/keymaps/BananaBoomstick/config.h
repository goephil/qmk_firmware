#pragma once

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(MARIO_THEME)
    #define GOODBYE_SONG SONG(MARIO_GAMEOVER)
    #define MUSIC_ON_SONG SONG(MARIO_MUSHROOM)
    #define MUSIC_OFF_SONG SONG(MARIO_GAMEOVER)

    #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND) }
#endif

#define MUSIC_MASK (keycode != KC_NO)

#define AUDIO_CLICKY

#define ENCODER_DIRECTION_FLIP

/*
 * MIDI options
 */

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2