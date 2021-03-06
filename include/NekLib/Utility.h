#ifndef NEKLIB_UTILITY_H
#define NEKLIB_UTILITY_H

namespace NekLib
{

//色を返す
unsigned int GetColor(int red, int green, int blue);

//基本図形描画
bool DrawPoint(float x, float y, unsigned int color);
bool DrawLine(float x1, float y1, float x2, float y2, unsigned int color);
bool DrawBox(float x, float y, float width, float height, unsigned int color, bool fillFlag);
bool DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color, bool fillFlag);
bool DrawCircle(float x, float y, float r, unsigned int color, bool fillFlag);//TODO:実装

}//namespace NekLib

#endif