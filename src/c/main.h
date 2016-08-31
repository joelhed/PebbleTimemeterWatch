#pragma once

#define KEY_COLOR_BACKGROUND 10
#define KEY_COLOR_HOURHAND 14
#define KEY_COLOR_DATEHAND 15

#define COLOR_DEFAULT_BACKGROUND 0x000000
#define COLOR_DEFAULT_HOURHAND 0xffffff
#define COLOR_DEFAULT_DATEHAND 0xff0000

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
    { -4,   6 },
    { -2,   8 },
    {  2,   8 },
    {  4,   6 },
    {  4,   0 },
    {  1,-40 },
    { -1,-40 },
    { -4,   0 },  
  }
};

