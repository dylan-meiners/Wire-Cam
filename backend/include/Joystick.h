#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Windows.h>

typedef struct LogitechExtreme3DPro {

    UINT id;
    double x_deadzone;
    double y_deadzone;
    double z_deadzone;
    double x;
    double y;
    double throttle;
    double z;
    bool trigger;
    bool thumb;
    bool three;
    bool four;
    bool five;
    bool six;
    bool seven;
    bool eight;
    bool nine;
    bool ten;
    bool eleven;
    bool twelve;
} LogitechExtreme3DPro;

LogitechExtreme3DPro* _LogitechExtreme3DPro(const UINT id,
                                            const double x_deadzone = 0.0,
                                            const double y_deadzone = 0.0,
                                            const double z_deadzone = 0.0) {

    LogitechExtreme3DPro* p = (LogitechExtreme3DPro*)malloc(sizeof(LogitechExtreme3DPro));
    p->id = id;
    p->x_deadzone = x_deadzone;
    p->y_deadzone = y_deadzone;
    p->z_deadzone = z_deadzone;
    return p;
}

bool CaptureInput(LogitechExtreme3DPro* joystick, const bool useKeyboard = false) {

    if (!useKeyboard) {

        JOYINFOEX joyinfo;
        joyinfo.dwSize = sizeof(joyinfo);
        joyinfo.dwFlags = JOY_RETURNALL;
        MMRESULT errorCode = joyGetPosEx(joystick->id, &joyinfo);
        switch (errorCode) {

        case JOYERR_NOERROR:

            joystick->x = ((joyinfo.dwXpos + 1) / 32768.0) - 1;
            joystick->y = ((joyinfo.dwYpos + 1) / 32768.0) - 1;
            joystick->throttle = ((joyinfo.dwZpos + 1) / 32768.0) - 1;
            joystick->z = ((joyinfo.dwRpos + 1) / 32768.0) - 1;
            joystick->trigger = joyinfo.dwButtons & JOY_BUTTON1;
            joystick->thumb = joyinfo.dwButtons & JOY_BUTTON2;
            joystick->three = joyinfo.dwButtons & JOY_BUTTON3;
            joystick->four = joyinfo.dwButtons & JOY_BUTTON4;
            joystick->five = joyinfo.dwButtons & JOY_BUTTON5;
            joystick->six = joyinfo.dwButtons & JOY_BUTTON6;
            joystick->seven = joyinfo.dwButtons & JOY_BUTTON7;
            joystick->eight = joyinfo.dwButtons & JOY_BUTTON8;
            joystick->nine = joyinfo.dwButtons & JOY_BUTTON9;
            joystick->ten = joyinfo.dwButtons & JOY_BUTTON10;
            joystick->eleven = joyinfo.dwButtons & JOY_BUTTON11;
            joystick->twelve = joyinfo.dwButtons & JOY_BUTTON12;
            return true;

        case JOYERR_UNPLUGGED:

            std::cout << "Joystick unplugged" << std::endl;
            return false;

        case JOYERR_PARMS:

            std::cout << "Invalid parameters given to joyGetPosEx!" << std::endl;
            return false;

        case JOYERR_NOCANDO:

            std::cout << "Cannot get joystick input" << std::endl;
            return false;

        case MMSYSERR_NODRIVER:

            std::cout << "No active joystick driver available" << std::endl;
            return false;

        default:
            std::cout << "Unknown error getting joystick data" << std::endl;
            return false;
        }
    }
    else {

        joystick->x =           (GetKeyDown('D') ? 1.0 : 0.0) + (GetKeyDown('A') ? -1.0 : 0.0);
        joystick->y =           (GetKeyDown('W') ? 1.0 : 0.0) + (GetKeyDown('S') ? -1.0 : 0.0);
        joystick->throttle =    0.0;
        joystick->z =           0.0;
        joystick->trigger =     GetKeyDown(VK_RETURN);
        joystick->thumb =       GetKeyDown('2');
        joystick->three =       GetKeyDown(VK_SPACE);
        joystick->four =        GetKeyDown('4');
        joystick->five =        GetKeyDown('5');
        joystick->six =         GetKeyDown('6');
        joystick->seven =       GetKeyDown('7');
        joystick->eight =       GetKeyDown('8');
        joystick->nine =        GetKeyDown('9');
        joystick->ten =         GetKeyDown('0');
        joystick->eleven =      GetKeyDown('N');
        joystick->twelve =      GetKeyDown('M');
    }
}

void CorrectAxis(LogitechExtreme3DPro * joystick,
                 const bool flipX,
                 const bool flipY,
                 const bool flipThrottle,
                 const bool flipZ) {

    joystick->x *= (flipX) ? -1 : 1;
    joystick->y *= (flipY) ? -1 : 1;
    joystick->throttle *= (flipThrottle) ? -1 : 1;
    joystick->z *= (flipZ) ? -1 : 1;
}

void CorrectDeadzones(LogitechExtreme3DPro * joystick) {

    if (abs(joystick->x) < joystick->x_deadzone) joystick->x = 0;
    if (abs(joystick->y) < joystick->y_deadzone) joystick->y = 0;
    if (abs(joystick->z) < joystick->z_deadzone) joystick->z = 0;
}

#endif