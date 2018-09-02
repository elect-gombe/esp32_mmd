/**
Copyright (c) 2018 Gombe.

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#ifndef _3DCONFIG
#define _3DCONFIG

const static int window_width = 320;
const static int window_height = 240;

//#define ENDIAN_LITTLE

#define ESP32
#ifdef ESP32
  #define OMIT_ZBUFFER_CONFLICT
//comment out if U use M5Stack.
  #define ILI9341
  #define PROCESSNUM 2
  #define DRAW_NLINES (20)
  #define MAXPROC_POLYNUM (920)
#else
//  #define OUTPUTTERMINAL
  #define USE_SDL
  #define PROCESSNUM 1
  #define DRAW_NLINES (window_height)
  #define MAXPROC_POLYNUM (2000)
#endif



// DO NOT CHANGE HERE
#ifdef OUTPUTTERMINAL
#undef ENDIAN_LITTLE
#endif

#endif
