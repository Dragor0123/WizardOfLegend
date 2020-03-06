#pragma once

#ifndef __REPLACECOLOR_H__
#define __REPLACECOLOR_H__

#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)

HBITMAP ReplaceColor(HBITMAP hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC hBmpDC);

#endif