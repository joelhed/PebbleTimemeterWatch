#pragma once

#define KEY_COLOR_BACKGROUND 10
#define KEY_COLOR_HOURHAND 14
#define KEY_COLOR_DATEHAND 15

#define COLOR_DEFAULT_BACKGROUND 0x000000
#define COLOR_DEFAULT_HOURHAND 0xffffff
#define COLOR_DEFAULT_DATEHAND 0xe35462

static const GPathInfo HOUR_HAND_POINTS = {
  8, (GPoint []){
    { -4, 10 },
    { -2, 14 },
    {  2, 14 },
    {  4, 10 },
    {  3, -75 },
    {  1,-80 },
    { -1,-80 },
    { -3, -75 },  
  }
};

static const GPathInfo DATE_HAND_POINTS = {
  8, (GPoint []) {
    { -5,   6 },
    { -3, 10 },
    {  3, 10 },
    {  5,   6 },
    {  5,   0 },
    {  1,-40 },
    { -1,-40 },
    { -5,   0 },  
  }
};

