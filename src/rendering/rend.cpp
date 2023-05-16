#include<Windows.h>
#include<CL/cl.hpp>
#include<iostream>
#include<stdint.h>
#include<math.h>
#include"rend.h"
#include"rendSettings.h"
#include"lighting.h"
#include"rendclString.h"
#include"../vec.h"
#include"fontLoading/tempDrawFont.h"
using namespace std;
using namespace cl;

void* BitmapMemory;
Buffer* Pixels;
Buffer* BigPixels;
Buffer* depthBuffer;
Context context;
Device device;
CommandQueue queue;

Kernel ImageCopyK;
Kernel ImageCopyHueK;
Kernel ClearScreenK;
Kernel DrawLineK;
Kernel DrawLineInvK;
Kernel ScaleUpToK;
Kernel FillTriK;
Kernel TextureTriK;

int BitmapWidth;
int BitmapHeight;

int ClientWidth;
int ClientHeight;
HDC DeviceContext;

vec3 camPos(0,0,0);
vec3 camRot(0,0,0);

BITMAPINFO BitmapInfo;

vec3& GetCamPos() {
    return camPos;
}
vec3& GetCamRot() {
    return camRot;
}

Context& GetContext() {
    return context;
}

Device& GetDevice() {
    return device;
}

CommandQueue& GetQueue() {
    return queue;
}
void DrawPixel(int X, int Y, uint32_t Color) {
    DrawLine(X,Y,X,Y,Color);
}
void DrawLine(int x1, int y1, int x2, int y2, uint32_t Color) {
    if (x1 == x2 && y1 == y2) {//TODO: draw pixel better (probably make new kernel)
        return;
    }
    if (abs(((double)y2-y1)/(x2-x1)) <= 1) {
        cl_int error;
        error = DrawLineK.setArg(0, *Pixels);
        error |= DrawLineK.setArg(1, GAM_WIDTH);
        error |= DrawLineK.setArg(2, GAM_HEIGHT);
        error |= DrawLineK.setArg(3, x1);
        error |= DrawLineK.setArg(4, y1);
        error |= DrawLineK.setArg(5, x2);
        error |= DrawLineK.setArg(6, y2);
        error |= DrawLineK.setArg(7, Color);
        int workSize;
        error |= DrawLineK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
        if (error != CL_SUCCESS) {
            cout << "Error setting kernel arguements!" << endl;
            cout << getErrorString(error) << endl;
            exit(3);
        }
        NDRange offset(0);
        int dif = abs(x1-x2)+1;
        NDRange l_range(dif>workSize?workSize:dif);
        NDRange g_range(dif>workSize?dif+workSize-(dif%workSize):dif);
        error = queue.enqueueNDRangeKernel(DrawLineK,offset,g_range,l_range);
        if (error != CL_SUCCESS) {
            cout << "Error enqueueing kernel!" << endl;
            cout << getErrorString(error) << endl;
            exit(4);
        }
        return;
    }
    cl_int error;
    error = DrawLineInvK.setArg(0, *Pixels);
    error |= DrawLineInvK.setArg(1, GAM_WIDTH);
    error |= DrawLineInvK.setArg(2, GAM_HEIGHT);
    error |= DrawLineInvK.setArg(3, x1);
    error |= DrawLineInvK.setArg(4, y1);
    error |= DrawLineInvK.setArg(5, x2);
    error |= DrawLineInvK.setArg(6, y2);
    error |= DrawLineInvK.setArg(7, Color);
    int workSize;
    error |= DrawLineInvK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    NDRange offset(0);
    int dif = abs(y1-y2)+1;
    NDRange l_range(dif>workSize?workSize:dif);
    NDRange g_range(dif>workSize?dif+workSize-(dif%workSize):dif);
    error = queue.enqueueNDRangeKernel(DrawLineInvK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
}

void DrawTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color) {
    vec3 v1(x1-GAM_WIDTH/2,GAM_HEIGHT/2-y1,0);
    vec3 v2(x2-GAM_WIDTH/2,GAM_HEIGHT/2-y2,0);
    vec3 v3(x3-GAM_WIDTH/2,GAM_HEIGHT/2-y3,0);
    if ((v2+(v1*-1)).cross(v3+(v2*-1)).z > 0) {
        DrawLine(x1,y1,x2,y2,Color);
        DrawLine(x2,y2,x3,y3,Color);
        DrawLine(x3,y3,x1,y1,Color);
    }
}

void DrawOpenTri(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, uint32_t Color) {
    vec3 v1(x1-GAM_WIDTH/2,GAM_HEIGHT/2-y1,0);
    vec3 v2(x2-GAM_WIDTH/2,GAM_HEIGHT/2-y2,0);
    vec3 v3(x3-GAM_WIDTH/2,GAM_HEIGHT/2-y3,0);
    if ((v2+(v1*-1)).cross(v3+(v2*-1)).z > 0) {
        DrawLine(x1,y1,x2,y2,Color);
        DrawLine(x2,y2,x3,y3,Color);
        DrawLine(x3,y3,x4,y4,Color);
    }
}

void DrawIncompleteTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color) {
    vec3 v1(x1-GAM_WIDTH/2,GAM_HEIGHT/2-y1,0);
    vec3 v2(x2-GAM_WIDTH/2,GAM_HEIGHT/2-y2,0);
    vec3 v3(x3-GAM_WIDTH/2,GAM_HEIGHT/2-y3,0);
    if ((v2+(v1*-1)).cross(v3+(v2*-1)).z > 0) {
        DrawLine(x1,y1,x2,y2,Color);
        DrawLine(x2,y2,x3,y3,Color);
    }
}

void FillTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color, bool safeOutline) {
    vec3 v1(x1-GAM_WIDTH/2,GAM_HEIGHT/2-y1,0);
    vec3 v2(x2-GAM_WIDTH/2,GAM_HEIGHT/2-y2,0);
    vec3 v3(x3-GAM_WIDTH/2,GAM_HEIGHT/2-y3,0);
    if ((v2+(v1*-1)).cross(v3+(v2*-1)).z > 0) {
        v1.x = x1;
        v1.y = y1;
        v2.x = x2;
        v2.y = y2;
        v3.x = x3;
        v3.y = y3;
        vec3 vecs[] = {v1,v2,v3};
        int max = (v1.y >= v2.y) ? ((v1.y >= v3.y) ? 0 : 2) : ((v2.y >= v3.y) ? 1 : 2);
        int other1 = (max+1)%3;
        int other2 = (max+2)%3;
        int mid = (vecs[other1].y >= vecs[other2].y) ? other1 : other2;
        int min = (vecs[other1].y < vecs[other2].y) ? other1 : other2;
        if (vecs[min].y == vecs[max].y) {
            return;
        }
        double slopes[] = {
            (vecs[mid].x - vecs[max].x) / (vecs[mid].y - vecs[max].y),
            (vecs[min].x - vecs[mid].x) / (vecs[min].y - vecs[mid].y),
            (vecs[min].x - vecs[max].x) / (vecs[min].y - vecs[max].y)
        };
        for (int y = vecs[max].y-(safeOutline?0:1); y > vecs[min].y; y--) {
            int xx1 = 0;
            int xx2 = 0;
            if (y >= vecs[mid].y) {
                if (vecs[mid].y == vecs[max].y) {
                    continue;
                }
                xx1 = (y-vecs[mid].y)*slopes[0] + vecs[mid].x;
                xx2 = (y-vecs[min].y)*slopes[2] + vecs[min].x;
            }
            else {
                if (vecs[min].y == vecs[mid].y) {
                    continue;
                }
                xx1 = (y-vecs[min].y)*slopes[1] + vecs[min].x;
                xx2 = (y-vecs[min].y)*slopes[2] + vecs[min].x;
            }
            if (y < 0 || y >= GAM_HEIGHT) {
                continue;
            }
            if (xx1 < 0) {
                xx1 = 0;
            }
            if (xx1 >= GAM_WIDTH) {
                xx1 = GAM_WIDTH-1;
            }
            if (xx2 < 0) {
                xx2 = 0;
            }
            if (xx2 >= GAM_WIDTH) {
                xx2 = GAM_WIDTH-1;
            }
            if (xx1 == xx2) {
                continue;
            }
            DrawLine(xx1+(safeOutline?(xx1<xx2?-1:1):(xx1<xx2?1:-1)),y,xx2-(safeOutline?(xx1<xx2?-1:1):(xx1<xx2?1:-1)),y,Color);
        }
        DrawTri(x1,y1,x2,y2,x3,y3,Color);
    }
}

void FillOpenTri(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, uint32_t Color, bool safeOutline) {
    FillTri(x1,y1,x2,y2,x3,y3,Color,safeOutline);
    FillTri(x1,y1,x3,y3,x4,y4,Color,safeOutline);
}

void FillIncompleteTri(double x1, double y1, double x2, double y2, double x3, double y3, uint32_t Color, bool safeOutline) {
    FillTri(x1,y1,x2,y2,x3,y3,Color,safeOutline);
}

void DrawWorldSpaceTri(vec3 v1, vec3 v2, vec3 v3, uint32_t Color) {
    if ((v1.x > -0.001) && (v2.x > -0.001) && (v3.x > -0.001)) {
        return;
    }
    if ((v2-v1).cross(v3-v2).dot(v1*-1) <= 0) {
        return;
    }
    if ((v1.x > -0.001) || (v2.x > -0.001) || (v3.x > -0.001)) {
        vec3 first;
        vec3 sec;
        vec3 third;
        vec3 last;
        if ((v1.x <= -0.001) && (v2.x <= -0.001)) {
            first = (v3-v1)*((v1.x+0.001)/((v1-v3).x))+v1;
            sec = v1;
            third = v2;
            last = (v3-v2)*((v2.x+0.001)/((v2-v3).x))+v2;
            vec3 sv[] = {
                first.toScreenSpace(),
                sec.toScreenSpace(),
                third.toScreenSpace(),
                last.toScreenSpace()
            };
            DrawOpenTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,sv[3].x,sv[3].y,Color);
        }
        else if ((v1.x <= -0.001) && (v3.x <= -0.001)) {
            first = (v2-v3)*((v3.x+0.001)/((v3-v2).x))+v3;
            sec = v3;
            third = v1;
            last = (v2-v1)*((v1.x+0.001)/((v1-v2).x))+v1;
            vec3 sv[] = {
                first.toScreenSpace(),
                sec.toScreenSpace(),
                third.toScreenSpace(),
                last.toScreenSpace()
            };
            DrawOpenTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,sv[3].x,sv[3].y,Color);
        }
        else if ((v2.x <= -0.001) && (v3.x <= -0.001)) {
            first = (v1-v2)*((v2.x+0.001)/((v2-v1).x))+v2;
            sec = v2;
            third = v3;
            last = (v1-v3)*((v3.x+0.001)/((v3-v1).x))+v3;
            vec3 sv[] = {
                first.toScreenSpace(),
                sec.toScreenSpace(),
                third.toScreenSpace(),
                last.toScreenSpace()
            };
            DrawOpenTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,sv[3].x,sv[3].y,Color);
        }
        else if (v1.x <= -0.001) {
            first = (v3-v1)*((v1.x+0.001)/((v1-v3).x))+v1;
            sec = v1;
            third = (v2-v1)*((v1.x+0.001)/((v1-v2).x))+v1;
            vec3 sv[] = {
                first.toScreenSpace(),
                sec.toScreenSpace(),
                third.toScreenSpace()
            };
            DrawIncompleteTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,Color);
        }
        else if (v2.x <= -0.001) {
            first = (v1-v2)*((v2.x+0.001)/((v2-v1).x))+v2;
            sec = v2;
            third = (v3-v2)*((v2.x+0.001)/((v2-v3).x))+v2;
            vec3 sv[] = {
                first.toScreenSpace(),
                sec.toScreenSpace(),
                third.toScreenSpace()
            };
            DrawIncompleteTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,Color);
        }
        else {
            first = (v2-v3)*((v3.x+0.001)/((v3-v2).x))+v3;
            sec = v3;
            third = (v1-v3)*((v3.x+0.001)/((v3-v1).x))+v3;
            vec3 sv[] = {
                first.toScreenSpace(),
                sec.toScreenSpace(),
                third.toScreenSpace()
            };
            DrawIncompleteTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,Color);
        }
    }
    else {
        vec3 sv[] = {
            v1.toScreenSpace(),
            v2.toScreenSpace(),
            v3.toScreenSpace()
        };
        DrawTri(sv[0].x,sv[0].y,sv[1].x,sv[1].y,sv[2].x,sv[2].y,Color);
    }
}

void FillWorldSpaceTri(vec3 v1, vec3 v2, vec3 v3, uint32_t Color, bool safeOutline) {
    if ((v1.z > 0.001) && (v2.z > 0.001) && (v3.z > 0.001)) {
        return;
    }
    vec3 vs1 = v1.toScreenSpace();
    vec3 vs2 = v2.toScreenSpace();
    vec3 vs3 = v3.toScreenSpace();
    if ((vs2-vs1).cross(vs3-vs2).z >= 0) {
        return;
    }
    cl_int error;
    error = FillTriK.setArg(0, *Pixels);
    error |= FillTriK.setArg(1, GAM_WIDTH);
    error |= FillTriK.setArg(2, GAM_HEIGHT);
    error |= FillTriK.setArg(3, (int)vs1.x);
    error |= FillTriK.setArg(4, (int)vs1.y);
    error |= FillTriK.setArg(5, (int)vs2.x);
    error |= FillTriK.setArg(6, (int)vs2.y);
    error |= FillTriK.setArg(7, (int)vs3.x);
    error |= FillTriK.setArg(8, (int)vs3.y);
    error |= FillTriK.setArg(9, Color);
    error |= FillTriK.setArg(10, (int)safeOutline);
    int workSize;
    error |= FillTriK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    int minx = (vs1.x<vs2.x?(vs1.x<vs3.x?vs1.x:vs3.x):(vs2.x<vs3.x?vs2.x:vs3.x));
    int miny = (vs1.y<vs2.y?(vs1.y<vs3.y?vs1.y:vs3.y):(vs2.y<vs3.y?vs2.y:vs3.y));
    int maxx = (vs1.x>vs2.x?(vs1.x>vs3.x?vs1.x:vs3.x):(vs2.x>vs3.x?vs2.x:vs3.x));
    int maxy = (vs1.y>vs2.y?(vs1.y>vs3.y?vs1.y:vs3.y):(vs2.y>vs3.y?vs2.y:vs3.y));
    NDRange offset((minx<0?-minx:0),(miny<0?-miny:0));
    minx = (minx<0)?0:minx;
    miny = (miny<0)?0:miny;
    maxx = (maxx>=GAM_WIDTH)?GAM_WIDTH-1:maxx;
    maxy = (maxy>=GAM_HEIGHT)?GAM_HEIGHT-1:maxy;
    if (minx >= GAM_WIDTH || maxx < 0 || miny >= GAM_HEIGHT || maxy < 0) {
        return;
    }
    int xgRange = maxx-minx;
    int ygRange = maxy-miny;
    if (xgRange == 0) {
        DrawLine(minx,miny,maxx,maxy,Color);
        return;
    }
    if (ygRange == 0) {
        DrawLine(minx,miny,maxx,maxy,Color);
        return;
    }
    NDRange l_range(xgRange>workSize?workSize:xgRange,ygRange>workSize?workSize:ygRange);
    NDRange g_range(xgRange>workSize?xgRange+((xgRange%workSize==0)?0:workSize-(xgRange%workSize)):xgRange,ygRange>workSize?ygRange+((ygRange%workSize==0)?0:workSize-(ygRange%workSize)):ygRange);
    error |= queue.enqueueNDRangeKernel(FillTriK,offset,g_range,l_range);
    // queue.finish();
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
    return;
}

void ShadeWorldSpaceTri(vec3 v1, vec3 v2, vec3 v3, uint32_t Color, bool safeOutline) {
    vec3 norm = (v2-v1).cross(v3-v2);
    double coef = (norm.dot(getActualLightList()[0].dir)*-1/norm.magnitude()+1)/2;
    uint32_t r = (Color&0x00ff0000) >> 16;
    uint32_t g = (Color&0x0000ff00) >> 8;
    uint32_t b = (Color&0x000000ff);
    Color &= 0xff000000;
    r *= coef;
    g *= coef;
    b *= coef;
    r &= 0xff;
    g &= 0xff;
    b &= 0xff;
    Color |= (r<<16)|(g<<8)|b;
    FillWorldSpaceTri(v1,v2,v3,Color,safeOutline);
}

void TextureWorldSpaceTri(Triangle& tri) {
    if (((tri.getPointRealSpace(1)-GetCamPos()).z > 0.001) || ((tri.getPointRealSpace(2)-GetCamPos()).z > 0.001) || ((tri.getPointRealSpace(3)-GetCamPos()).z > 0.001)) {
        return;
    }
    vec3 vs1 = tri.getPointRealSpace(1);
    vec3 vs2 = tri.getPointRealSpace(2);
    vec3 vs3 = tri.getPointRealSpace(3);
    vs1 = (vs1-GetCamPos()).toScreenSpace();
    vs2 = (vs2-GetCamPos()).toScreenSpace();
    vs3 = (vs3-GetCamPos()).toScreenSpace();
    if ((vs2-vs1).cross(vs3-vs2).z >= 0) {
        return;
    }
    cl_int error;
    error = TextureTriK.setArg(0, *Pixels);
    error |= TextureTriK.setArg(1, *depthBuffer);
    error |= TextureTriK.setArg(2, *tri.getTexture());
    error |= TextureTriK.setArg(3, *tri.getLightText());
    error |= TextureTriK.setArg(4, GAM_WIDTH);
    error |= TextureTriK.setArg(5, GAM_HEIGHT);
    error |= TextureTriK.setArg(6, (tri.getPointRealSpace(1)-GetCamPos()).toStruct());
    error |= TextureTriK.setArg(7, (tri.getPointRealSpace(2)-GetCamPos()).toStruct());
    error |= TextureTriK.setArg(8, (tri.getPointRealSpace(3)-GetCamPos()).toStruct());//TODO: rotate
    error |= TextureTriK.setArg(9, vs1.toStructI());
    error |= TextureTriK.setArg(10, vs2.toStructI());
    error |= TextureTriK.setArg(11, vs3.toStructI());
    error |= TextureTriK.setArg(12, tri.getUVPoint(1).toStructI());
    error |= TextureTriK.setArg(13, tri.getUVPoint(2).toStructI());
    error |= TextureTriK.setArg(14, tri.getUVPoint(3).toStructI());
    error |= TextureTriK.setArg(15, tri.getUVSize().toStructI());
    int workSize;
    error |= TextureTriK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    int minx = (vs1.x<vs2.x?(vs1.x<vs3.x?vs1.x:vs3.x):(vs2.x<vs3.x?vs2.x:vs3.x));
    int miny = (vs1.y<vs2.y?(vs1.y<vs3.y?vs1.y:vs3.y):(vs2.y<vs3.y?vs2.y:vs3.y));
    int maxx = (vs1.x>vs2.x?(vs1.x>vs3.x?vs1.x:vs3.x):(vs2.x>vs3.x?vs2.x:vs3.x));
    int maxy = (vs1.y>vs2.y?(vs1.y>vs3.y?vs1.y:vs3.y):(vs2.y>vs3.y?vs2.y:vs3.y));
    NDRange offset((minx<0?-minx:0),(miny<0?-miny:0));
    minx = (minx<0)?0:minx;
    miny = (miny<0)?0:miny;
    maxx = (maxx>=GAM_WIDTH)?GAM_WIDTH-1:maxx;
    maxy = (maxy>=GAM_HEIGHT)?GAM_HEIGHT-1:maxy;
    if (minx >= GAM_WIDTH || maxx < 0 || miny >= GAM_HEIGHT || maxy < 0) {
        return;
    }
    int xgRange = maxx-minx;
    int ygRange = maxy-miny;
    if (xgRange == 0) {
        xgRange = 1;
    }
    if (ygRange == 0) {
        ygRange = 1;
    }
    NDRange l_range(xgRange>workSize?workSize:xgRange,ygRange>workSize?workSize:ygRange);
    NDRange g_range(xgRange>workSize?xgRange+((xgRange%workSize==0)?0:workSize-(xgRange%workSize)):xgRange,ygRange>workSize?ygRange+((ygRange%workSize==0)?0:workSize-(ygRange%workSize)):ygRange);
    error |= queue.enqueueNDRangeKernel(TextureTriK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
    return;
}

void DrawRect(int xLeft, int yTop, int width, int height, uint32_t Color) {
    DrawLine(xLeft,yTop,xLeft,yTop-height,Color);
    DrawLine(xLeft,yTop-height,xLeft+width,yTop-height,Color);
    DrawLine(xLeft+width,yTop-height,xLeft+width,yTop,Color);
    DrawLine(xLeft+width,yTop,xLeft,yTop,Color);
}

void FillRect(int xLeft, int yTop, int width, int height, uint32_t Color) {
    //fill how you fill triangle
}

void ClearScreen(uint32_t Color) {
    cl_int error;
    error = ClearScreenK.setArg(0, *Pixels);
    error |= ClearScreenK.setArg(1, GAM_WIDTH);
    error |= ClearScreenK.setArg(2, GAM_HEIGHT);
    error |= ClearScreenK.setArg(3, Color);
    int workSize;
    error |= ClearScreenK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    NDRange offset(0,0);
    NDRange l_range(GAM_WIDTH>workSize?workSize:GAM_WIDTH,GAM_HEIGHT>workSize?workSize:GAM_HEIGHT);
    NDRange g_range(GAM_WIDTH>workSize?GAM_WIDTH+workSize-(GAM_WIDTH%workSize):GAM_WIDTH,GAM_HEIGHT>workSize?GAM_HEIGHT+workSize-(GAM_HEIGHT%workSize):GAM_HEIGHT);
    error = queue.enqueueNDRangeKernel(ClearScreenK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
    ClearDepth(0x00000000);
}

void ClearDepth(uint32_t Color) {
    cl_int error;
    error = ClearScreenK.setArg(0, *depthBuffer);
    error |= ClearScreenK.setArg(1, GAM_WIDTH);
    error |= ClearScreenK.setArg(2, GAM_HEIGHT);
    error |= ClearScreenK.setArg(3, Color);
    int workSize;
    error |= ClearScreenK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    NDRange offset(0,0);
    NDRange l_range(GAM_WIDTH>workSize?workSize:GAM_WIDTH,GAM_HEIGHT>workSize?workSize:GAM_HEIGHT);
    NDRange g_range(GAM_WIDTH>workSize?GAM_WIDTH+workSize-(GAM_WIDTH%workSize):GAM_WIDTH,GAM_HEIGHT>workSize?GAM_HEIGHT+workSize-(GAM_HEIGHT%workSize):GAM_HEIGHT);
    error = queue.enqueueNDRangeKernel(ClearScreenK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
}

void drawImage(Buffer* image, int xSize, int ySize, int xPos, int yPos, double scale) {
    cl_int error;
    error = ImageCopyK.setArg(0, *image);
    error |= ImageCopyK.setArg(1, *Pixels);
    error |= ImageCopyK.setArg(2, xPos);
    error |= ImageCopyK.setArg(3, yPos);
    error |= ImageCopyK.setArg(4, xSize);
    error |= ImageCopyK.setArg(5, ySize);
    error |= ImageCopyK.setArg(6, GAM_WIDTH);
    error |= ImageCopyK.setArg(7, GAM_HEIGHT);
    error |= ImageCopyK.setArg(8, scale);
    int workSize;
    error |= ImageCopyK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    int newX = xSize*scale;
    int newY = ySize*scale;
    NDRange offset(0,0);
    NDRange l_range(newX>workSize?workSize:newX,newY>workSize?workSize:newY);
    NDRange g_range(newX>workSize?newX+workSize-(newX%workSize):newX,newY>workSize?newY+workSize-(newY%workSize):newY);
    error = queue.enqueueNDRangeKernel(ImageCopyK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
    return;
}
void drawImageHue(Buffer* image, int xSize, int ySize, int xPos, int yPos, double scale, uint32_t hue) {
    cl_int error;
    error = ImageCopyHueK.setArg(0, *image);
    error |= ImageCopyHueK.setArg(1, *Pixels);
    error |= ImageCopyHueK.setArg(2, xPos);
    error |= ImageCopyHueK.setArg(3, yPos);
    error |= ImageCopyHueK.setArg(4, xSize);
    error |= ImageCopyHueK.setArg(5, ySize);
    error |= ImageCopyHueK.setArg(6, GAM_WIDTH);
    error |= ImageCopyHueK.setArg(7, GAM_HEIGHT);
    error |= ImageCopyHueK.setArg(8, scale);
    error |= ImageCopyHueK.setArg(9, hue);
    int workSize;
    error |= ImageCopyHueK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }
    int newX = xSize*scale;
    int newY = ySize*scale;
    NDRange offset(0,0);
    NDRange l_range(newX>workSize?workSize:newX,newY>workSize?workSize:newY);
    NDRange g_range(newX>workSize?newX+workSize-(newX%workSize):newX,newY>workSize?newY+workSize-(newY%workSize):newY);
    error = queue.enqueueNDRangeKernel(ImageCopyHueK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
    return;
}

void SetupDraw(HINSTANCE& Instance, HWND& Window, Context& c, Device& d, CommandQueue q) {
    context = c;
    device = d;
    queue = q;
    initReadCL();

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    ClientWidth = ClientRect.right - ClientRect.left;
    ClientHeight = ClientRect.bottom - ClientRect.top;
    BitmapWidth = ClientWidth;
    BitmapHeight = ClientHeight;
    BitmapMemory = VirtualAlloc(0,BitmapWidth*BitmapHeight*4,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    Pixels = new Buffer(context, CL_MEM_READ_WRITE, GAM_WIDTH*GAM_HEIGHT*4);
    depthBuffer = new Buffer(context, CL_MEM_READ_WRITE, GAM_WIDTH*GAM_HEIGHT*4);
    BigPixels = new Buffer(context, CL_MEM_READ_WRITE, GAM_WIDTH_ACTUAL*GAM_HEIGHT_ACTUAL*4);
    queue.enqueueWriteBuffer(*Pixels, CL_TRUE, 0, BitmapWidth*BitmapHeight*4, BitmapMemory);
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    DeviceContext = GetDC(Window);
    InitFont();

    ImageCopyK = makeKernel(getRendCL(),"ImageCopy");
    ImageCopyHueK = makeKernel(getRendCL(),"ImageCopyHue");
    ClearScreenK = makeKernel(getRendCL(),"ClearScreen");
    DrawLineK = makeKernel(getRendCL(),"DrawLine");
    DrawLineInvK = makeKernel(getRendCL(),"DrawLineInv");
    ScaleUpToK = makeKernel(getRendCL(),"ScaleUpTo");
    FillTriK = makeKernel(getRendCL(),"FillTri");
    TextureTriK = makeKernel(getRendCL(),"TextureTri");

    ClearDepth(0x00000000);
}

Buffer* getPixels() {
    return Pixels;
}

void KillDraw() {
    KillFont();
    clReleaseMemObject((*Pixels)());
    delete Pixels;
}

void DrawBuffer() {
    cl_int error;
    error = ScaleUpToK.setArg(0, *Pixels);
    error |= ScaleUpToK.setArg(1, *BigPixels);
    error |= ScaleUpToK.setArg(2, GAM_WIDTH);
    error |= ScaleUpToK.setArg(3, GAM_HEIGHT);
    error |= ScaleUpToK.setArg(4, GAM_WIDTH_ACTUAL);
    error |= ScaleUpToK.setArg(5, GAM_HEIGHT_ACTUAL);
    error |= ScaleUpToK.setArg(6, RES_SCALE);
    int workSize;
    error |= ImageCopyHueK.getWorkGroupInfo(device,CL_KERNEL_WORK_GROUP_SIZE,&workSize);
    workSize = sqrt(workSize);
    if (error != CL_SUCCESS) {
        cout << "Error setting kernel arguements!" << endl;
        cout << getErrorString(error) << endl;
        exit(3);
    }

    int newX = GAM_WIDTH;
    int newY = GAM_HEIGHT;
    NDRange offset(0,0);
    NDRange l_range(newX>workSize?workSize:newX,newY>workSize?workSize:newY);
    NDRange g_range(newX>workSize?newX+workSize-(newX%workSize):newX,newY>workSize?newY+workSize-(newY%workSize):newY);
    error = queue.enqueueNDRangeKernel(ScaleUpToK,offset,g_range,l_range);
    if (error != CL_SUCCESS) {
        cout << "Error enqueueing kernel!" << endl;
        cout << getErrorString(error) << endl;
        exit(4);
    }
    queue.enqueueReadBuffer(*BigPixels,CL_TRUE,0,GAM_WIDTH_ACTUAL*GAM_HEIGHT_ACTUAL*4,BitmapMemory);
    error = queue.finish();
    if (error != CL_SUCCESS) {
        cout << "Error synchronyzing queue!" << endl;
        cout << getErrorString(error) << endl;
        exit(5);
    }
    StretchDIBits(DeviceContext, 0, 0, BitmapWidth, BitmapHeight, 0, 0, ClientWidth, ClientHeight, BitmapMemory, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}
