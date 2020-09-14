#include "TXLib.h"
#include <iostream>
#include <math.h>

void drawSphere( int x0, int y0, int sphereRadius, int numberOfCicles, int red, int green, int blue )
{
    COLORREF fillColor = txGetFillColor();
    COLORREF lineColor = txGetColor();

    for (int i = 0; i < numberOfCicles; i++ )
    {
        int currentCicleRed   = red * i / numberOfCicles;
        int currentCicleGreen = green * i / numberOfCicles;
        int currentCicleBlue  = blue * i / numberOfCicles;
        int currentCicleRadius = sphereRadius - sphereRadius * i / numberOfCicles;
        int x = x0 + (sphereRadius * i / 2) / numberOfCicles;
        int y = y0 - (sphereRadius * i / 2) / numberOfCicles;

        txSetFillColour( RGB ( currentCicleRed, currentCicleGreen, currentCicleBlue));
        txSetColour    ( RGB (currentCicleRed, currentCicleGreen, currentCicleBlue));
        txCircle(x, y, currentCicleRadius);
    }

    txSetFillColour( fillColor);
    txSetColour( lineColor);
}

void moveSphere(float* x, float* y, float* vx, float* vy, const float DT, float ax, float ay)
{
    *x += *vx * DT + 0.5 * ax * DT * DT;
    *y += *vy * DT + 0.5 * ay * DT * DT;

    *vx += ax * DT;
    *vy += ay * DT;
}

void collisionSphere(float x, float y, float* vx, float* vy, const int X_MAX, const int Y_MAX, int sphereRadius )
{
    if ((x > X_MAX - sphereRadius && *vx > 0) || (x < sphereRadius && *vx < 0))
            *vx = - *vx;
    if ((y > Y_MAX - sphereRadius && *vy > 0) || (y < sphereRadius && *vy < 0))
            *vy = - *vy;
}

bool isCollidedTwoSpheres(float x1, float y1, int radiusSphere1,float x2, float y2, int radiusSphere2 )
{
    int commonRadius = radiusSphere1 + radiusSphere2;
    int commonRadiusSquared = commonRadius * commonRadius;

    float dx = x1 - x2;
    float dy = y1 - y2;

    int distanceSquared = dx * dx + dy * dy;

    if (distanceSquared < commonRadiusSquared)
        return true;
    return false;
}

float projectionVector( float xVector, float yVector, float xAxis, float yAxis)
{
    float scalarComposition = xVector * xAxis + yVector * yAxis;
    float moduleAxis = pow (xAxis * xAxis + yAxis * yAxis, 0.5 );
    float pV = scalarComposition / moduleAxis;
    return pV;
}

float reducedMass( float m1, float m2)
{
    //std::cout << m1;
    //std::cout << "\n";
    //std::cout << m2;
    //std::cout << "\n";

    float rm = (m1 * m2) / (m1 + m2);
    return rm;
}

float moduleVector(float x, float y)
{
    return pow (x * x + y * y, 0.5 );
}

void changeSpeedSphereOnCollision(float x1, float y1, float* vx1, float* vy1, int mass1,
                                  float x2, float y2, float* vx2, float* vy2, int mass2 )
{
    float xAxis = x1 - x2;
    float yAxis = y1 - y2;
    float moduleAxis = pow (xAxis * xAxis + yAxis * yAxis, 0.5 );

    //std::cout << xAxis;
    //std::cout << "\n";
    //std::cout << yAxis;
    //std::cout << "\n";
    //std::cout << moduleAxis;
    //std::cout << "\n";

    float projectionVectorOnSpeedSphere1 = projectionVector( *vx1, *vy1, xAxis, yAxis);
    float projectionVectorOnSpeedSphere2 = projectionVector( *vx2, *vy2, xAxis, yAxis);

    //std::cout << projectionVectorOnSpeedSphere1;
    //std::cout << "\n";
    //std::cout << projectionVectorOnSpeedSphere2;
    //std::cout << "\n";

    float reducedMassSpheres = reducedMass( mass1, mass2);

    //std::cout << reducedMassSpheres;
    //std::cout << "\n";

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / mass1;
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / mass2;

    //std::cout << dv1;
    //std::cout << "\n";
    //std::cout << dv2;
    //std::cout << "\n";

    if (dv1 < 0)
    {
        *vx1 += -2 * dv1 * xAxis / moduleAxis;
        *vy1 += -2 * dv1 * yAxis / moduleAxis;
    }

    if (dv2 < 0)
    {
        *vx2 += 2 * dv2 * xAxis / moduleAxis;
        *vy2 += 2 * dv2 * yAxis / moduleAxis;
    }
}

void drawTrack(int x, int y, int radius, int red, int green, int blue, float vx, float vy, const float DT)
{
    COLORREF fillColor = txGetFillColor();
    COLORREF lineColor = txGetColor();

    txSetFillColour( RGB ( red, green, blue));
    txSetColour    ( RGB (red, green, blue));

    int numberOfCiclesInDrawTrack = 10;
    for ( int i = 0; i < numberOfCiclesInDrawTrack; ++i)
            txCircle(x - i * vx * DT / numberOfCiclesInDrawTrack, y - i * vy * DT / numberOfCiclesInDrawTrack, radius);

    txSetFillColour( fillColor);
    txSetColour( lineColor);
}

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
    COLORREF lineColor = RGB (0, 0, 0);

    int sphereRadius1 = 25;
    int sphereRadius2 = 25;
    int sphereRadius3 = 25;
    int numberOfCicles = 25;

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

    const int MASS_1 = 1;
    const int MASS_2 = 2;
    const int MASS_3 = 6;

    double mouseX = 0;
    double mouseY = 0;

    float ax1 = 0;
    float ay1 = 0;

    float ax2 = 0;
    float ay2 = 0;

    float ax3 = 0.001;
    float ay3 = 0.001;

    float controllability = 20;

    float coefficientSlowdown = 0.025;

    txCreateWindow(X_MAX, Y_MAX);
    txSetFillColour( fillColor);
    txSetColour( lineColor);
    txClear();

    while (true)
    {
        txBegin();

        drawTrack(x1, y1, sphereRadius1, redTrack, greenTrack, blueTrack, vx1, vy1, DT);
        drawTrack(x2, y2, sphereRadius2, redFill,  greenFill,  blueFill,  vx2, vy2, DT);
        drawTrack(x3, y3, sphereRadius3, redFill,  greenFill,  blueFill,  vx3, vy3, DT);

        ax1 = 0;
        ay1 = 0;

        if (txMouseButtons() > 0 )
        {
            mouseX = txMouseX();
            mouseY = txMouseY();

            float differnceMouseXAndX1 = mouseX - x1;
            float differnceMouseYAndY1 = mouseY - y1;
            float modeleDiffernceMouseAndSphere1 = moduleVector( differnceMouseXAndX1, differnceMouseYAndY1);

            ax1 += controllability * differnceMouseXAndX1 / modeleDiffernceMouseAndSphere1;
            ay1 += controllability * differnceMouseYAndY1 / modeleDiffernceMouseAndSphere1;
        }

        ax1 += -vx1 * coefficientSlowdown;
        ay1 += -vy1 * coefficientSlowdown;

        moveSphere(&x1, &y1, &vx1, &vy1, DT, ax1, ay1);
        moveSphere(&x2, &y2, &vx2, &vy2, DT, ax2, ay2);
        moveSphere(&x3, &y3, &vx3, &vy3, DT, ax3, ay3);

        collisionSphere(x1, y1, &vx1, &vy1, X_MAX, Y_MAX, sphereRadius1 );
        collisionSphere(x2, y2, &vx2, &vy2, X_MAX, Y_MAX, sphereRadius2 );
        collisionSphere(x3, y3, &vx3, &vy3, X_MAX, Y_MAX, sphereRadius3 );

        if ( isCollidedTwoSpheres (x1, y1, sphereRadius1, x2, y2, sphereRadius2))
            changeSpeedSphereOnCollision(x1, y1, &vx1, &vy1, MASS_1, x2, y2, &vx2, &vy2, MASS_2);

        if ( isCollidedTwoSpheres (x1, y1, sphereRadius1, x3, y3, sphereRadius3))
            changeSpeedSphereOnCollision(x1, y1, &vx1, &vy1, MASS_1, x3, y3, &vx3, &vy3, MASS_3);

        if ( isCollidedTwoSpheres (x3, y3, sphereRadius3, x2, y2, sphereRadius2))
            changeSpeedSphereOnCollision(x3, y3, &vx3, &vy3, MASS_3, x2, y2, &vx2, &vy2, MASS_2);

        drawSphere(x1, y1, sphereRadius1, numberOfCicles, red1, green1, blue1);
        drawSphere(x2, y2, sphereRadius2, numberOfCicles, red2, green2, blue2);
        drawSphere(x3, y3, sphereRadius3, numberOfCicles, red3, green3, blue3);

        txEnd();
    }
    return 0;
}
