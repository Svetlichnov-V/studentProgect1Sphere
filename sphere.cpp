//#include "TXLib.h"
#include "SFML/Graphics.hpp"
//#include <iostream>
//#include <math.h>
//#include "sphere.h"

int main()
{
    const int X_MAX = 1960;
    const int Y_MAX = 1080;

    const float DT = 0.1;

    int redFill   = 10;
    int greenFill = 10;
    int blueFill  = 0;
    COLORREF fillColor = RGB (redFill, greenFill, blueFill);
    COLORREF lineColor = RGB (redFill, greenFill, blueFill);

    int numberOfCicles = 50;
    int numberOfCiclesInDrawTrack = 10;

    Vector2f positionSphere1 {200, 100};
    Vector2f velositySphere1 { 0, 0 };

    int sphereRadius1 = 50;
    const float SPHERE_MASS_1 = 1;

    COLORREF colorSphere1 = RGB(0, 0, 255);
    COLORREF colorTrackSphere1 = RGB(0, 0, 50);

    Sphere sphere1 { positionSphere1,     positionSphere1,     velositySphere1,     Vector2f {0, 0}, sphereRadius1, SPHERE_MASS_1, colorSphere1,       colorTrackSphere1 };
    Sphere sphere2 { Vector2f {600, 100}, Vector2f {600, 100}, Vector2f {20, 10},   Vector2f {0, 0}, 50,            1,             RGB(0,   255, 255), fillColor         };
    Sphere sphere3 { Vector2f {200, 400}, Vector2f {200, 400}, Vector2f {20, 10},   Vector2f {0, 0}, 50,            1,             RGB(255, 0,   255), fillColor         };

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

