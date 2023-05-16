#include<iostream>
#include<cmath>
#include<CL/cl.hpp>
#include"tempDrawFont.h"
#include"../rend.h"
using namespace std;
using namespace cl;

uint32_t nums[10][25] = {
    {0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000},
    {0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000},
    {0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000},
    {0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000},
    {0x00ffffff,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000},
    {0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000},
    {0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000},
    {0x00ffffff,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0xff000000},
    {0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000},
    {0xff000000,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0x00ffffff,0x00ffffff,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000,0x00ffffff,0xff000000,0xff000000,0xff000000}
};
Buffer* nums_d[10];

void InitFont() {
    nums_d[0] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[0], CL_TRUE, 0, 25*4, nums[0]);
    nums_d[1] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[1], CL_TRUE, 0, 25*4, nums[1]);
    nums_d[2] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[2], CL_TRUE, 0, 25*4, nums[2]);
    nums_d[3] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[3], CL_TRUE, 0, 25*4, nums[3]);
    nums_d[4] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[4], CL_TRUE, 0, 25*4, nums[4]);
    nums_d[5] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[5], CL_TRUE, 0, 25*4, nums[5]);
    nums_d[6] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[6], CL_TRUE, 0, 25*4, nums[6]);
    nums_d[7] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[7], CL_TRUE, 0, 25*4, nums[7]);
    nums_d[8] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[8], CL_TRUE, 0, 25*4, nums[8]);
    nums_d[9] = new Buffer(GetContext(), CL_MEM_READ_ONLY, 25*4);
    GetQueue().enqueueWriteBuffer(*nums_d[9], CL_TRUE, 0, 25*4, nums[9]);
}
void KillFont() {
    clReleaseMemObject((*nums_d[0])());
    delete nums_d[0];
    clReleaseMemObject((*nums_d[1])());
    delete nums_d[1];
    clReleaseMemObject((*nums_d[2])());
    delete nums_d[2];
    clReleaseMemObject((*nums_d[3])());
    delete nums_d[3];
    clReleaseMemObject((*nums_d[4])());
    delete nums_d[4];
    clReleaseMemObject((*nums_d[5])());
    delete nums_d[5];
    clReleaseMemObject((*nums_d[6])());
    delete nums_d[6];
    clReleaseMemObject((*nums_d[7])());
    delete nums_d[7];
    clReleaseMemObject((*nums_d[8])());
    delete nums_d[8];
    clReleaseMemObject((*nums_d[9])());
    delete nums_d[9];
}
void drawDigit(int num, int x, int y, int size, uint32_t color) {
    drawImageHue(nums_d[num],5,5,x,y,size/5,color);
}
void drawNumber(int num, int x, int y, int size, uint32_t color) {
    int count = 0;
    size += 4;
    size -= size%5;
    int numForCounter = num;
    while (numForCounter > 0) {
        numForCounter /= 10;
        count++;
    }
    for (int i = 0; i < count; i++) {
        int digit = (num/(int)pow(10,count-i-1))%10;
        drawDigit(digit, x+(i*size), y, size, color);
    }
}
