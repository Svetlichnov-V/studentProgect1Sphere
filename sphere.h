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
    txSetColour    ( RGB ( red, green, blue));

    int numberOfCiclesInDrawTrack = 10;
    for ( int i = 0; i <= numberOfCiclesInDrawTrack + 2; ++i)
            txCircle(x - i * vx * DT / numberOfCiclesInDrawTrack, y - i * vy * DT / numberOfCiclesInDrawTrack, radius);

    txSetFillColour( fillColor);
    txSetColour    ( lineColor);
}

void controlSphere1(float* ax, float* ay, float vx, float vy, float x, float y, const float controllability, const float coefficientSlowdown)
{
    *ax = 0;
    *ay = 0;

    if (txMouseButtons() > 0 )
    {
        double mouseX = txMouseX();
        double mouseY = txMouseY();

        float differnceMouseXAndX1 = mouseX - x;
        float differnceMouseYAndY1 = mouseY - y;
        float modeleDiffernceMouseAndSphere1 = moduleVector( differnceMouseXAndX1, differnceMouseYAndY1);

        *ax += controllability * differnceMouseXAndX1 / modeleDiffernceMouseAndSphere1;
        *ay += controllability * differnceMouseYAndY1 / modeleDiffernceMouseAndSphere1;
    }

    *ax += -vx * coefficientSlowdown;
    *ay += -vy * coefficientSlowdown;
}
