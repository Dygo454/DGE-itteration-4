#ifndef REND_H
#define REND_H

#include<windows.h>
#include<CL/cl.hpp>
#include<stdint.h>
#include"rendSettings.h"
#include"../vec.h"
#include"../OpenCL/readCL.h"

vec3& GetCamPos();
vec3& GetCamRot();
cl::Context& GetContext();
cl::Device& GetDevice();
cl::CommandQueue& GetQueue();
void DrawPixel(int X, int Y, uint32_t Color);
void DrawLine(int x1, int y1, int x2, int y2, uint32_t Color);
void DrawTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color = 0x00ffffff);
void DrawOpenTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color = 0x00ffffff);
void DrawIncompleteTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color = 0x00ffffff);
void FillTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color = 0x00ffffff, bool safeOutline=true);
void FillOpenTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color = 0x00ffffff, bool safeOutline=true);
void FillIncompleteTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color = 0x00ffffff, bool safeOutline=true);
void DrawWorldSpaceTri(vec3 v1, vec3 v2, vec3 v3, uint32_t Color = 0x00ffffff);
void FillWorldSpaceTri(vec3 v1, vec3 v2, vec3 v3, uint32_t Color = 0x00ffffff, bool safeOutline=false);
void FillWorldSpaceTriOld(vec3 v1, vec3 v2, vec3 v3, uint32_t Color, bool safeOutline=false);
void ShadeWorldSpaceTri(vec3 v1, vec3 v2, vec3 v3, uint32_t Color = 0x00ffffff, bool safeOutline=true);
void TextureWorldSpaceTri(Triangle& tri);
void DrawRect(int xLeft, int yTop, int width, int height, uint32_t Color);
void FillRect(int xLeft, int yTop, int width, int height, uint32_t Color);
void ClearScreen(uint32_t Color = 0x00000000);
void ClearDepth(uint32_t Color = 0x00000000);
void drawImage(cl::Buffer* image, int xSize, int ySize, int xPos=0, int yPos=0, double scale=1);
void drawImageHue(cl::Buffer* image, int xSize, int ySize, int xPos=0, int yPos=0, double scale=1, uint32_t hue=0x00ffffff);
void SetupDraw(HINSTANCE& Instance, HWND& Window, cl::Context& c, cl::Device& d, cl::CommandQueue q);
void KillDraw();
cl::Buffer* getPixels();
void DrawBuffer();

#endif
