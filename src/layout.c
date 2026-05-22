#include <stddef.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "layout.h"
#include "key_functions.h"

#define FREE 0

#define kprrel  &kbfun_press_release
#define lpush1  &kbfun_layer_push_1
#define lpop1   &kbfun_layer_pop_1
#define s2kcap  &kbfun_2_keys_capslock_press_release

const uint8_t PROGMEM _kb_layout[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
	KB_MATRIX_LAYER(  // layer 0: default
// unused
0,
// left hand
    _grave,               _1,    _2,    _3,     _4,    _5, _arrowU,
      _tab,               _Q,    _W,    _E,     _R,    _T,       1,
      _esc,               _A,    _S,    _D,     _F,    _G,
   _shiftL,               _Z,    _X,    _C,     _V,    _B, _arrowL,
    _ctrlL, _backslash_nonUS, _altL, _dash, _equal,
                                              _del,   _home,
                                           0, 0,      _end,
                                    _space, _bs,    _guiL,
// right hand
  _arrowD,    _6,      _7,      _8,      _9,           _0,      FREE,
     FREE,    _Y,      _U,      _I,      _O,           _P,      FREE,
              _H,      _J,      _K,      _L,   _semicolon,    _quote,
  _arrowR,    _N,      _M,  _comma, _period,       _slash,   _shiftR,
             _bracketL, _bracketR, _altR, _pound_nonUS,    _ctrlR,
_pageU, _insert,
_pageD, 0, 0,
 _guiR,  _enter, _space ),


	KB_MATRIX_LAYER(  // layer 1: function and symbol keys
// unused
0,
// left hand
  FREE,     _F1,     _F2,     _F3,  _F4,  _F5, _F11,
  FREE,    FREE, _arrowU,    FREE, FREE, FREE,    1,
  _esc, _arrowL, _arrowD, _arrowR, FREE, FREE,
  FREE,    FREE,    FREE,    FREE, FREE, FREE, FREE,
  FREE,    FREE,    FREE,    FREE, FREE,
                                        FREE,   FREE,
                                       0,  0,   FREE,
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

const void_funptr_t PROGMEM _kb_layout_press[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
	KB_MATRIX_LAYER(  // press: layer 0
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


	KB_MATRIX_LAYER(  // press: layer 1
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

const void_funptr_t PROGMEM _kb_layout_release[KB_LAYERS][KB_ROWS][KB_COLUMNS] = {
	KB_MATRIX_LAYER(  // release: layer 0
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


	KB_MATRIX_LAYER(  // release: layer 1
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
