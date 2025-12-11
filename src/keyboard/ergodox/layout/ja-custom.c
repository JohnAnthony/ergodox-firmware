#include <stdint.h>
#include <stddef.h>
#include <avr/pgmspace.h>
#include "../../../lib/data-types/misc.h"
#include "../../../lib/usb/usage-page/keyboard--short-names.h"
#include "../../../lib/key-functions/public.h"
#include "../matrix.h"
#include "../layout.h"

#define FREE 0

// basic
#define  kprrel  &kbfun_press_release
#define  ktog    &kbfun_toggle
#define  ktrans  &kbfun_transparent
#define  lpush1   &kbfun_layer_push_1
#define  lpop1    &kbfun_layer_pop_1
// device
#define  dbtldr  &kbfun_jump_to_bootloader
// special
#define  sshprre  &kbfun_shift_press_release
#define  s2kcap   &kbfun_2_keys_capslock_press_release
#define  slpunum  &kbfun_layer_push_numpad
#define  slponum  &kbfun_layer_pop_numpad

// Key values
///////
const uint8_t PROGMEM _kb_layout[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
	KB_MATRIX_LAYER(  // layout: layer 0: default
// unused
0,
// left hand
    _grave,               _1,    _2,    _3,     _4,    _5, FREE,
      _tab,               _Q,    _W,    _E,     _R,    _T,    1,
      _esc,               _A,    _S,    _D,     _F,    _G,
   _shiftL,               _Z,    _X,    _C,     _V,    _B, FREE,
    _ctrlL, _backslash_nonUS, _altL, _dash, _equal,
                                                  _del,   _home,
                                                  0, 0,   _end,
                                           _space, _bs,  _guiL,
// right hand
  FREE,    _6,      _7,      _8,      _9,           _0,      FREE,
_bracketL, _Y,      _U,      _I,      _O,           _P,      FREE,
           _H,      _J,      _K,      _L,   _semicolon,    _quote,
  FREE,    _N,      _M,  _comma, _period,       _slash,   _shiftR,
             _bracketL, _bracketR, _altR, _pound_nonUS,    _ctrlR,
 _home, _insert,
  _end, 0, 0,
 _guiR,  _enter, _space ),


	KB_MATRIX_LAYER(  // layout: layer 1: function and symbol keys
// unused
0,
// left hand
  FREE,     _F1,     _F2,     _F3,  _F4,  _F5, _F11,
  FREE,    FREE, _arrowU,    FREE, FREE, FREE,    1,
  _esc, _arrowL, _arrowD, _arrowR, FREE, FREE,
  FREE,    FREE,    FREE,    FREE, FREE, FREE, FREE,
  FREE,    FREE,    FREE,    FREE, FREE,
                                        FREE, _pageU,
                                       0,  0, _pageD,
                               _space,  FREE,   FREE,
// right hand
_F12,     _F6,     _F7,     _F8,     _F9, _F10, FREE,
FREE,    FREE,    FREE,    FREE,    FREE, FREE, FREE,
      _arrowL, _arrowD, _arrowU, _arrowR, FREE, FREE,
FREE,    FREE,    FREE,    FREE,    FREE, FREE, FREE,
                  FREE,    FREE,    FREE, FREE, FREE,
FREE,  FREE,
FREE,  0,  0,
FREE,  _enter,  _space )
};

// Functions for keypress
///////
const void_funptr_t PROGMEM _kb_layout_press[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
	KB_MATRIX_LAYER(  // press: layer 0: default
// unused
NULL,
// left hand
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, lpush1,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 s2kcap, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel,
                                                 kprrel, kprrel,
                                           NULL,   NULL, kprrel,
                                         kprrel, kprrel, kprrel,
// right hand
         kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
         kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
                 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
         kprrel,   kprrel, kprrel, kprrel, kprrel, kprrel, s2kcap,
                         kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel,
 kprrel,   NULL,   NULL,
 kprrel, kprrel, kprrel ),


	KB_MATRIX_LAYER(  // press: layer 1: function and symbol keys
// unused
NULL,
// left hand
   NULL, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel,   NULL,  lpop1,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel,
                                                 kprrel, kprrel,
                                         kprrel, kprrel, kprrel,
                                         kprrel, kprrel, kprrel,
// right hand
        kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
        kprrel,   NULL, kprrel, kprrel, kprrel, kprrel, kprrel,
                kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
        kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
                        kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel,
 kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel )
};

// Functions for key release
///////
const void_funptr_t PROGMEM _kb_layout_release[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
	KB_MATRIX_LAYER(  // press: layer 0: default
// unused
NULL,
// left hand
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, lpush1,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 s2kcap, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel,
                                                 kprrel, kprrel,
                                           NULL,   NULL, kprrel,
                                         kprrel, kprrel, kprrel,
// right hand
         kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
         kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
                 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
         kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, s2kcap,
                         kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel,
 kprrel,   NULL,   NULL,
 kprrel, kprrel, kprrel ),


	KB_MATRIX_LAYER(  // press: layer 1: function and symbol keys
// unused
NULL,
// left hand
   NULL, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel,   NULL,  lpop1,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel, kprrel, kprrel,
                                                 kprrel, kprrel,
                                         kprrel, kprrel, kprrel,
                                         kprrel, kprrel, kprrel,
// right hand
        kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
        kprrel,   NULL, kprrel, kprrel, kprrel, kprrel, kprrel,
                kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
        kprrel, kprrel, kprrel, kprrel, kprrel, kprrel, kprrel,
                        kprrel, kprrel, kprrel, kprrel, kprrel,
 kprrel, kprrel,
 kprrel, kprrel, kprrel,
 kprrel, kprrel, kprrel )
};
