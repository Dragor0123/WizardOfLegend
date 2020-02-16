#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 1000
#define WINCY 720

//#define LEFT_CAPTION 100
//#define TOP_CAPTION 100
//#define RIGHT_CAPTION 100
//#define BOTTOM_CAPTION 100

//#define HPBARX 460

// °æ·Î
#define ROOT_PATH	"RootPath"
#define BITMAP_PATH "BitmapPath"

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr;}

#define PI  3.1415926535897f
#define GRAVITY 9.80665f

#endif // !__DEFINE_H__
