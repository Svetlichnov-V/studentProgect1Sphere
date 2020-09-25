//#include "TXLib.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <math.h>
#include "sphere.h"

int main()
{
    const int X_MAX = 1960;
    const int Y_MAX = 1080;

    const float DT = 0.1;

    sf::Uint8 redFill = 10;
    sf::Uint8 greenFill = 10;
    sf::Uint8 blueFill = 0;
    sf::Color fillColor = sf::Color(redFill, greenFill, blueFill);
    sf::Color lineColor = sf::Color(redFill, greenFill, blueFill);

    int numberOfCicles = 50;
    int numberOfCiclesInDrawTrack = 10;

    Vector2f positionSphere1{ 200, 100 };
    Vector2f velositySphere1{ 0, 0 };

    int sphereRadius1 = 50;
    const float SPHERE_MASS_1 = 1;

    sf::Color colorSphere1      = sf::Color(0, 0, 255);
    sf::Color colorTrackSphere1 = sf::Color(0, 0, 100);

    Sphere sphere1{ positionSphere1,     positionSphere1,     velositySphere1,     Vector2f {0, 0}, sphereRadius1, SPHERE_MASS_1, colorSphere1,             colorTrackSphere1 };
    Sphere sphere2{ Vector2f {600, 100}, Vector2f {600, 100}, Vector2f {20, 10},   Vector2f {0, 0}, 50,            1,             sf::Color(0,   255, 255), fillColor         };
    Sphere sphere3{ Vector2f {200, 400}, Vector2f {200, 400}, Vector2f {20, 10},   Vector2f {0, 0}, 50,            1,             sf::Color(255, 0,   255), fillColor         };

    float controllability = 20;

    float coefficientSlowdown = 0.0;

    sf::RenderWindow window(sf::VideoMode(X_MAX, Y_MAX), "Main window");
    window.clear(fillColor);
    //txCreateWindow(X_MAX, Y_MAX);
    //txSetFillColour(fillColor);
    //txSetColour(lineColor);
    //txClear();

    while (true)
    {
        sphere1.acceleration.x = 0;
        sphere1.acceleration.y = 0;

        controlSphere(&sphere1, controllability, coefficientSlowdown);

        collisionSphere(&sphere1, X_MAX, Y_MAX);
        collisionSphere(&sphere2, X_MAX, Y_MAX);
        collisionSphere(&sphere3, X_MAX, Y_MAX);

        if (isCollidedTwoSpheres(&sphere1, &sphere2))
            changeSpeedSphereOnCollision(&sphere1, &sphere2);

        if (isCollidedTwoSpheres(&sphere1, &sphere3))
            changeSpeedSphereOnCollision(&sphere1, &sphere3);

        if (isCollidedTwoSpheres(&sphere3, &sphere2))
            changeSpeedSphereOnCollision(&sphere3, &sphere2);

        sphere1.oldPosition.x = sphere1.position.x;
        sphere1.oldPosition.y = sphere1.position.y;

        sphere2.oldPosition.x = sphere2.position.x;
        sphere2.oldPosition.y = sphere2.position.y;

        sphere3.oldPosition.x = sphere3.position.x;
        sphere3.oldPosition.y = sphere3.position.y;

        moveSphere(&sphere1, DT);
        moveSphere(&sphere2, DT);
        moveSphere(&sphere3, DT);

        //txBegin();

        //drawSphere(&window, &sphere1, numberOfCicles);
        //drawSphere(&window, &sphere2, numberOfCicles);
        //drawSphere(&window, &sphere3, numberOfCicles);

        window.pushGLStates();

        drawTrack(&window, &sphere1, numberOfCiclesInDrawTrack);
        drawTrack(&window, &sphere2, numberOfCiclesInDrawTrack);
        drawTrack(&window, &sphere3, numberOfCiclesInDrawTrack);

        //window.pushGLStates();
        //window.clear(fillColor);

        drawSphere(&window, &sphere1, numberOfCicles);
        drawSphere(&window, &sphere2, numberOfCicles);
        drawSphere(&window, &sphere3, numberOfCicles);

        window.popGLStates();

        window.display();

        //txEnd();
    }
    return 0;
}
