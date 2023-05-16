void alphaColor(__global unsigned int* screen, int x, int y, int xSize, unsigned int Color) {
    unsigned int newColor = screen[x+(y*xSize)];
    unsigned int a = (Color & 0xff000000) >> 24;
    unsigned int r = (Color & 0x00ff0000) >> 16;
    unsigned int g = (Color & 0x0000ff00) >> 8;
    unsigned int b = (Color & 0x000000ff);
    unsigned int oldr = (newColor & 0x00ff0000) >> 16;
    unsigned int oldg = (newColor & 0x0000ff00) >> 8;
    unsigned int oldb = (newColor & 0x000000ff);
    screen[x+(y*xSize)] = (((oldr*a/0xff)+(r*(0xff-a)/0xff)) << 16)+(((oldg*a/0xff)+(g*(0xff-a)/0xff)) << 8)+((oldb*a/0xff)+(b*(0xff-a)/0xff));
}
void alphaColorInd(__global unsigned int* screen, int ind, int xSize, unsigned int Color) {
    unsigned int newColor = screen[ind];
    unsigned int a = (Color & 0xff000000) >> 24;
    unsigned int r = (Color & 0x00ff0000) >> 16;
    unsigned int g = (Color & 0x0000ff00) >> 8;
    unsigned int b = (Color & 0x000000ff);
    unsigned int oldr = (newColor & 0x00ff0000) >> 16;
    unsigned int oldg = (newColor & 0x0000ff00) >> 8;
    unsigned int oldb = (newColor & 0x000000ff);
    screen[ind] = (((oldr*a/0xff)+(r*(0xff-a)/0xff)) << 16)+(((oldg*a/0xff)+(g*(0xff-a)/0xff)) << 8)+((oldb*a/0xff)+(b*(0xff-a)/0xff));
}

__kernel void ImageCopy(__global const unsigned int* toCopy, __global unsigned int* toPaste, int xPos, int yPos, int xSize, int ySize, int pasteX, int pasteY, double scale) {
    int currX = get_global_id(0);
    int currY = get_global_id(1);
    if (currX >= xSize*scale || currY >= ySize*scale) retur

    if (currX+xPos < 0 || currX+xPos >= pasteX || currY+yPos < 0 || currY+yPos >= pasteY) {
        return;
    }
    int groupX = (int)(currX/scale
    int groupY = (int)(currY/scale
    unsigned int ColorIn = toCopy[groupX+(groupY*xSize)];
    if ((ColorIn & 0xff000000) == 0x00000000) {
        toPaste[(currX+xPos)+((currY+yPos)*pasteX)] = ColorIn;
        return;
    }
    alphaColor(toPaste,currX+xPos,currY+yPos,xSize,ColorIn);
}

__kernel void ImageCopyHue(__global const unsigned int* toCopy, __global unsigned int* toPaste, int xPos, int yPos, int xSize, int ySize, int pasteX, int pasteY, double scale, unsigned int Color) {
    int currX = get_global_id(0);
    int currY = get_global_id(1);
    if (currX >= xSize*scale || currY >= ySize*scale) retur

    if (currX+xPos < 0 || currX+xPos >= pasteX || currY+yPos < 0 || currY+yPos >= pasteY) {
        return;
    }
    int groupX = (int)(currX/scale
    int groupY = (int)(currY/scale
    unsigned int ColorIn = Color&toCopy[groupX+(groupY*xSize)];
    ColorIn &= 0x00ffffff;
    ColorIn |= (Color|toCopy[groupX+(groupY*xSize)])&0xff000000;
    if ((ColorIn & 0xff000000) == 0x00000000) {
        toPaste[(currX+xPos)+((currY+yPos)*pasteX)] = ColorIn;
        return;
    }
    alphaColor(toPaste,currX+xPos,currY+yPos,xSize,ColorIn);
}

__kernel void ClearScreen(__global unsigned int* screen, int xSize, int ySize, unsigned int Color) {
    int id = get_global_id(0)+(get_global_id(1)*xSize);
    if (id >= xSize*ySize) {
        return;
    }
    if ((Color & 0xff000000) == 0x00000000) {
        screen[id] = Color;
        return;
    }
    alphaColorInd(screen,id,xSize,Color);
}

__kernel void DrawLine(__global unsigned int* screen, int xSize, int ySize, int x1, int y1, int x2, int y2, unsigned int Color) {
    int id = get_global_id(0);
    double slope = (1.0*y2-y1)/(x2-x1);
    if (x2 < x1) {
        id *= -1;
    }

    if (x1+id < 0 || x1+id >= xSize || y1+(id*slope) < 0 || y1+(id*slope) > ySize) {
        return;
    }
    if ((Color & 0xff000000) == 0x00000000) {
        screen[((y1+(int)(id*slope+0.5))*xSize+x1+id)] = Color;
        return;
    }
    alphaColor(screen,x1+id,y1+(int)(id*slope+0.5),xSize,Color);
}

__kernel void DrawLineInv(__global unsigned int* screen, int xSize, int ySize, int x1, int y1, int x2, int y2, unsigned int Color) {
    int id = get_global_id(0);
    double slope = (1.0*x2-x1)/(y2-y1);
    if (y2 < y1) {
        id *= -1;
    }

    if (y1+id < 0 || y1+id >= ySize || x1+(id*slope) < 0 || x1+(id*slope) > xSize) {
        return;
    }
    if ((Color & 0xff000000) == 0x00000000) {
        screen[(((y1+id)*xSize)+x1+(int)(id*slope))] = Color;
        return;
    }
    alphaColor(screen,x1+(int)(id*slope+0.5),y1+id,xSize,Color);
}

__kernel void ScaleUpTo(__global unsigned int* screenSmol, __global unsigned int* screen, int xSize, int ySize, int newXS, int newYS, int scale) {
    int id = get_global_id(0)+(get_global_id(1)*newXS);
    int smolID = id/scale;
    screen[id] = screenSmol[smolID];
}

__kernel void FillTri(__global unsigned int* screen, int xSize, int ySize, int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int safeOutline) {
    int x = get_global_id(0) + (x1<x2?(x1<x3?x1:x3):(x2<x3?x2:x3));
    int y = get_global_id(1) + (y1<y2?(y1<y3?y1:y3):(y2<y3?y2:y3));
    if (x >= xSize || x < 0 || y >= ySize || y < 0) {
        return;
    }
    int v1X = x2-x1;
    int v1Y = y2-y1;
    int v2X = x3-x1;
    int v2Y = y3-y1;
    int v3X = x3-x2;
    int v3Y = y3-y2;
    int vX = x-x1;
    int vY = y-y1;
    int vX2 = x-x2;
    int vY2 = y-y2;
    int aActual = (v1X*v2Y-v1Y*v2X)/2;
    int a = (vX*v1Y-vY*v1X)/2;
    int b = (vX*v2Y-vY*v2X)/2;
    int c = (v3X*vY2-vX2*v3Y)/2;
    aActual *= aActual<0 ? -1 : 1;
    a *= a<0 ? -1 : 1;
    b *= b<0 ? -1 : 1;
    c *= c<0 ? -1 : 1;
    if (a+b+c > aActual || (!safeOutline && (a == 0 || b == 0 || c == 0))) {
        return;
    }
    alphaColor(screen,x,y,xSize,Color);
}

typedef struct vec3CLI vec3CLI {
    int x;
    int y;
    int z;
};
typedef struct vec3CL vec3CL {
    double x;
    double y;
    double z;
};

double dot(vec3CL self, vec3CL other) {
    double sum = self.x*other.x;
    sum += self.y*other.y;
    sum += self.z*other.z;
    return sum;
}
vec3CL cross(vec3CL self, vec3CL other) {
    vec3CL crossed;
    crossed.x = self.y*other.z - self.z*other.y;
    crossed.y = self.z*other.x - self.x*other.z;
    crossed.z = self.x*other.y - self.y*other.x;
    return crossed;
}
vec3CL vadd(vec3CL self, vec3CL other) {
    self.x += other.x;
    self.y += other.y;
    self.z += other.z;
    return self;
}
vec3CL vmult(vec3CL self, double scale) {
    self.x *= scale;
    self.y *= scale;
    self.z *= scale;
    return self;
}
double vMag(vec3CL self) {
    return sqrt(dot(self,self));
}

__kernel void TextureTri(__global unsigned int* screen, __global unsigned int* depth, __global unsigned int* texture, __global unsigned int* light, int xSize, int ySize, vec3CL p1, vec3CL p2, vec3CL p3, vec3CLI SS1, vec3CLI SS2, vec3CLI SS3, vec3CLI TS1, vec3CLI TS2, vec3CLI TS3, vec3CLI textSize) {
    int x = get_global_id(0) + (SS1.x<SS2.x?(SS1.x<SS3.x?SS1.x:SS3.x):(SS2.x<SS3.x?SS2.x:SS3.x));
    int y = get_global_id(1) + (SS1.y<SS2.y?(SS1.y<SS3.y?SS1.y:SS3.y):(SS2.y<SS3.y?SS2.y:SS3.y));
    if (x >= xSize || x < 0 || y >= ySize || y < 0) {
        return;
    }
    int v1X = SS2.x-SS1.x;
    int v1Y = SS2.y-SS1.y;
    int v2X = SS3.x-SS1.x;
    int v2Y = SS3.y-SS1.y;
    int v3X = SS3.x-SS2.x;
    int v3Y = SS3.y-SS2.y;
    int vX = x-SS1.x;
    int vY = y-SS1.y;
    int vX2 = x-SS2.x;
    int vY2 = y-SS2.y;
    int aActual = (v1X*v2Y-v1Y*v2X)/2;
    int a = (vX*v1Y-vY*v1X)/2;
    int b = (vX*v2Y-vY*v2X)/2;
    int c = (v3X*vY2-vX2*v3Y)/2;
    aActual *= aActual<0 ? -1 : 1;
    a *= a<0 ? -1 : 1;
    b *= b<0 ? -1 : 1;
    c *= c<0 ? -1 : 1;
    if (a+b+c > aActual) {
        return;
    }
    vec3CL rayVec;
    rayVec.x = -0.001;
    rayVec.z = 0.001-(0.002*((double)y)/ySize);//FOV = 90 and n = 0.001
    rayVec.y = (0.002*xSize/ySize)*((double)x)/xSize - 0.001;
    vec3CL triNorm = cross(vadd(p2,vmult(p1,-1)),vadd(p3,vmult(p2,-1)));
    double s2 = dot(p1,triNorm);
    double s1 = dot(rayVec,triNorm);
    vec3CL intersect = vmult(rayVec,s2/s1);
    double dist = vMag(intersect);
    if (dist > depth[x+y*xSize] && depth[x+y*xSize] != 0) {
        return;
    }
    depth[x+y*xSize] = dist;
    screen[x+y*xSize] = (p1.x==p3.x) ? 0x00ff00ff : 0x00ffffff;
}
