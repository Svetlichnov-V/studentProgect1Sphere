#include "TXLib.h"
#include <iostream>
#include <math.h>
#include "sphere.h"

int main()
{
    Sphere sphere1 { Vector2f {200, 100}, Vector2f {200, 100}, Vector2f {20, 10}, Vector2f {2, 1}, 50, 1, RGB(0,   0,   255), RGB(0,   0,   100) };
    Sphere sphere2 { Vector2f {600, 100}, Vector2f {600, 100}, Vector2f {20, 10}, Vector2f {0, 0}, 50, 1, RGB(0,   255, 255), RGB(0,   0,   0)   };
    Sphere sphere3 { Vector2f {200, 400}, Vector2f {200, 400}, Vector2f {20, 10}, Vector2f {0, 0}, 50, 1, RGB(255, 0,   255), RGB(0,   0,   0)   };

    const int X_MAX = 1960;
    const int Y_MAX = 1080;
    
    //здесь написана какая-то ерунда

    //float vx1 = 100;
    //float vy1 = -70.0;

    //float vx2 = 20.5;
    //float vy2 = -45.0;

    //float vx3 = 20.5;
    //float vy3 = 45.0;

    const float DT = 0.1;

    int redFill   = 0;
    int greenFill = 0;
    int blueFill  = 0;
    COLORREF fillColor = RGB (redFill, greenFill, blueFill);
    COLORREF lineColor = RGB (redFill, greenFill, blueFill);

    //int sphereRadius1 = 50;
    //int sphereRadius2 = 50;
    //int sphereRadius3 = 50;
    int numberOfCicles = 50;
    int numberOfCiclesInDrawTrack = 10;

    //int red1 = 0;
    //int green1 = 0;
    //int blue1 = 255;

    //int red2 = 255;
    //int green2 = 0;
    //int blue2 = 0;

    //int red3 = 255;
    //int green3 = 255;
    //int blue3 = 255;

    //int redTrack = 0;
    //int greenTrack = 0;
    //int blueTrack = 75;

    //float x1 = 600;
    //float y1 = 200;

    //float x2 = 1200;
    //float y2 = 800;

    //float x3 = 200;
    //float y3 = 800;

    //float xOld1 = 600;
    //float yOld1 = 200;

    //float xOld2 = 1200;
    //float yOld2 = 800;

    //float xOld3 = 200;
    //float yOld3 = 800;

    //const int MASS_1 = 1;
    //const int MASS_2 = 50;
    //const int MASS_3 = 1;

    //float ax1 = 0;
    //float ay1 = 0;

    //float ax2 = 0;
    //float ay2 = 0;

    //float ax3 = 0.001;
    //float ay3 = 0.001;

    float controllability = 20;

    float coefficientSlowdown = 0.0;

    txCreateWindow(X_MAX, Y_MAX);
    txSetFillColour( fillColor);
    txSetColour( lineColor);
    txClear();

    while (true)
    {
        sphere1.acceleration.x = 0;
        sphere1.acceleration.y = 0;

        controlSphere( &sphere1, controllability, coefficientSlowdown);

        collisionSphere(&sphere1, X_MAX, Y_MAX );
        collisionSphere(&sphere2, X_MAX, Y_MAX );
        collisionSphere(&sphere3, X_MAX, Y_MAX );

        if ( isCollidedTwoSpheres (&sphere1, &sphere2))
            changeSpeedSphereOnCollision( &sphere1, &sphere2);

        if ( isCollidedTwoSpheres (&sphere1, &sphere3))
            changeSpeedSphereOnCollision( &sphere1, &sphere3);

        if ( isCollidedTwoSpheres (&sphere3, &sphere2))
            changeSpeedSphereOnCollision( &sphere3, &sphere2);

        sphere1.oldPosition.x = sphere1.position.x;
        sphere1.oldPosition.y = sphere1.position.y;

        sphere2.oldPosition.x = sphere2.position.x;
        sphere2.oldPosition.y = sphere2.position.y;

        sphere3.oldPosition.x = sphere3.position.x;
        sphere3.oldPosition.y = sphere3.position.y;

        moveSphere( &sphere1, DT);
        moveSphere( &sphere2, DT);
        moveSphere( &sphere3, DT);

        txBegin();

        drawTrack( &sphere1, numberOfCiclesInDrawTrack);
        drawTrack( &sphere2, numberOfCiclesInDrawTrack);
        drawTrack( &sphere3, numberOfCiclesInDrawTrack);

        drawSphere( &sphere1, numberOfCicles);
        drawSphere( &sphere2, numberOfCicles);
        drawSphere( &sphere3, numberOfCicles);

        txEnd();
    }
    return 0;
}

