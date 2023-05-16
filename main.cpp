#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include<iostream>
#include<string>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdint.h>
#include<Windows.h>
#include<CL/cl.hpp>
#include<vector>
#include"src/time.h"
#include"src/window.h"
#include"src/ui/input.h"
#include"src/ui/cursor.h"
#include"src/vec.h"
#include"src/tempCube.h"
#include"src/rendering/rend.h"
#include"src/rendering/rendSettings.h"
#include"src/rendering/lighting.h"
#include"src/rendering/fontLoading/tempDrawFont.h"
using namespace std;
using namespace cl;

void Init();
void Tick();
void Render();
void Kill();

double speed = 10;

vec3 p(0,0,-5);
vec3 r(0,0,0);
Cube c(p, r, 2, "res/testTexture.tga");
double deltaTime = 0;

Light defLight;

int main() {
    double cur = (double) clock();
    Init();

    vector<Platform> all_platforms;
    Platform::get(&all_platforms);
    Platform default_platform = all_platforms[0];
    cout << "Using platform: " <<default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
    vector<Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() == 0) {
        cout << " No devices found.\n";
        exit(1);
    }
    Device default_device = all_devices[0];
    cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
    Context context({default_device});
    CommandQueue queue(context, default_device);

    cout << "Creating Window!\n";
    Window* pWindow = new Window();
    SetupDraw(pWindow->getInst(),pWindow->getWin(),context,default_device,queue);
    SetUpCursor(pWindow->getWin());

    bool running = true;
    while(running) {
        setDeltaTime(deltaTime = (((double) clock() - cur)/CLOCKS_PER_SEC));
        cur = (double) clock();
        if (!pWindow->ProcessMessages()) {
            cout << "Closing Window!\n";
            running = false;
        }
        Tick();
        Render();
    }
    Kill();
    delete pWindow;
    return 0;
}

void Init() {
    for (int i = 0; i < 254; i++) {
        GetStates()[i] = false;
    }
    SetCursorState(CursorLockedHidden);
}
void Tick() {
    updateInput();
    double clockTime = (double) clock()/CLOCKS_PER_SEC;
    vec3 moveVec(0,0,0);
    if (GetKey(KeyCode::W)) {
        moveVec.z -= deltaTime*10;
    }
    if (GetKey(KeyCode::S)) {
        moveVec.z += deltaTime*10;
    }
    if (GetKey(KeyCode::D)) {
        moveVec.x += deltaTime*10;
    }
    if (GetKey(KeyCode::A)) {
        moveVec.x -= deltaTime*10;
    }
    if (GetKey(KeyCode::E)) {
        moveVec.y += deltaTime*10;
    }
    if (GetKey(KeyCode::Q)) {
        moveVec.y -= deltaTime*10;
    }
    vec3 cRot = GetCamRot();

    // moveVec.eulerRot(cRot.x,cRot.y,cRot.z);

    GetCamPos() += moveVec;

    // vec3 lookVec(0,GetMouseIn().y,GetMouseIn().x);
    // lookVec *= deltaTime*-150;
    // GetCamRot() += lookVec;

    if (GetKeyDown(KeyCode::Escape)) {
        SetCursorState(CursorFree);
    }
    if (GetKeyDown(KeyCode::LMB)) {
        SetCursorState(CursorLockedHidden);
    }
    vec3 mouseIn = GetMouseIn();
    vec3 testProj(100,0,-100);
    // c.setRot(sin(clockTime)*180,cos(clockTime)*180,sin(clockTime)*180);
    // c.setRot(0,45,0);
    double xRot = 0;
    double yRot = 45;
    double zRot = 0;
    c.rot(deltaTime*xRot,deltaTime*yRot,deltaTime*zRot);
}
void Render() {
    ClearScreen();
    int fps = (int)getFPS();
    drawNumber(std::isinf(fps)?9999:fps,0,0,20/RES_SCALE);
    // maybe you can put all of the position vec 3 into a kernel that translates and rotates them based on the camera position
    c.render();
    DrawBuffer();
}
void Kill() {
    KillDraw();
}
