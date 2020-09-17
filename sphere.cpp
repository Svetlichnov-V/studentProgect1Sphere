#include "TXLib.h"
#include <iostream>
#include <math.h>
#include "sphere.h"

int main()
{

    const int X_MAX = 1960;
    const int Y_MAX = 1080;

    float vx1 = 40.3;
    float vy1 = -70.0;

    float vx2 = 20.5;
    float vy2 = -45.0;

    float vx3 = 20.5;
    float vy3 = 45.0;

    const float DT = 0.2;

    int redFill = 20;
    int greenFill = 0;
    int blueFill = 0;
    COLORREF fillColor = RGB (redFill, greenFill, blueFill);
    COLORREF lineColor = RGB (redFill, greenFill, blueFill);

    int sphereRadius1 = 50;
    int sphereRadius2 = 50;
    int sphereRadius3 = 50;
    int numberOfCicles = 50;
    int numberOfCiclesInDrawTrack = 10;

    int red1 = 0;
    int green1 = 0;
    int blue1 = 255;

    int red2 = 255;
    int green2 = 0;
    int blue2 = 0;

    int red3 = 0;
    int green3 = 255;
    int blue3 = 0;

    int redTrack = 0;
    int greenTrack = 0;
    int blueTrack = 100;

    float x1 = 600;
    float y1 = 200;

    float x2 = 1200;
    float y2 = 800;

    float x3 = 200;
    float y3 = 800;

    float xOld1 = 600;
    float yOld1 = 200;

    float xOld2 = 1200;
    float yOld2 = 800;

    float xOld3 = 200;
    float yOld3 = 800;

    const int MASS_1 = 1;
    const int MASS_2 = 2;
    const int MASS_3 = 6;

    float ax1 = 0;
    float ay1 = 0;

    float ax2 = 0;
    float ay2 = 0;

    float ax3 = 0.001;
    float ay3 = 0.001;

    float controllability = 20;

    float coefficientSlowdown = 0.0;

    txCreateWindow(X_MAX, Y_MAX);
    txSetFillColour( fillColor);
    txSetColour( lineColor);
    txClear();

    while (true)
    {
        ax1 = 0;
        ay1 = 0;
        
        controlSphere(&ax1, &ay1, vx1, vy1, x1, y1, controllability, coefficientSlowdown);

        collisionSphere(x1, y1, &vx1, &vy1, X_MAX, Y_MAX, sphereRadius1 );
        collisionSphere(x2, y2, &vx2, &vy2, X_MAX, Y_MAX, sphereRadius2 );
        collisionSphere(x3, y3, &vx3, &vy3, X_MAX, Y_MAX, sphereRadius3 );

        if ( isCollidedTwoSpheres (x1, y1, sphereRadius1, x2, y2, sphereRadius2))
            changeSpeedSphereOnCollision(x1, y1, &vx1, &vy1, MASS_1, x2, y2, &vx2, &vy2, MASS_2);

        if ( isCollidedTwoSpheres (x1, y1, sphereRadius1, x3, y3, sphereRadius3))
            changeSpeedSphereOnCollision(x1, y1, &vx1, &vy1, MASS_1, x3, y3, &vx3, &vy3, MASS_3);

        if ( isCollidedTwoSpheres (x3, y3, sphereRadius3, x2, y2, sphereRadius2))
            changeSpeedSphereOnCollision(x3, y3, &vx3, &vy3, MASS_3, x2, y2, &vx2, &vy2, MASS_2);

        xOld1 = x1;
        yOld1 = y1;

        xOld2 = x2;
        yOld2 = y2;

        xOld3 = x3;
        yOld3 = y3;

        moveSphere(&x1, &y1, &vx1, &vy1, DT, ax1, ay1);
        moveSphere(&x2, &y2, &vx2, &vy2, DT, ax2, ay2);
        moveSphere(&x3, &y3, &vx3, &vy3, DT, ax3, ay3);

        txBegin();

        drawTrack(x1, y1, xOld1, yOld1, sphereRadius1, numberOfCiclesInDrawTrack, redTrack, greenTrack, blueTrack);
        drawTrack(x2, y2, xOld2, yOld2, sphereRadius2, numberOfCiclesInDrawTrack, redFill,  greenFill,  blueFill);
        drawTrack(x3, y3, xOld3, yOld3, sphereRadius3, numberOfCiclesInDrawTrack, redFill,  greenFill,  blueFill);

        drawSphere(x1, y1, sphereRadius1, numberOfCicles, red1, green1, blue1);
        drawSphere(x2, y2, sphereRadius2, numberOfCicles, red2, green2, blue2);
        drawSphere(x3, y3, sphereRadius3, numberOfCicles, red3, green3, blue3);

        txEnd();
    }
    return 0;
}

