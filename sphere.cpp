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

void moveSphere(float* x, float* y, float vx, float vy, const float DT)
{
    *x += vx * DT;
    *y += vy * DT;
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
    std::cout << m1;
    std::cout << "\n";
    std::cout << m2;
    std::cout << "\n";

    float rm = (m1 * m2) / (m1 + m2);
    return rm;
}

void changeSpeedSphereOnCollision(float x1, float y1, float* vx1, float* vy1, int mass1,
            float x2, float y2, float* vx2, float* vy2, int mass2 )
{
    float xAxis = x1 - x2;
    float yAxis = y1 - y2;
    float moduleAxis = pow (xAxis * xAxis + yAxis * yAxis, 0.5 );

    std::cout << xAxis;
    std::cout << "\n";
    std::cout << yAxis;
    std::cout << "\n";
    std::cout << moduleAxis;
    std::cout << "\n";

    float projectionVectorOnSpeedSphere1 = projectionVector( *vx1, *vy1, xAxis, yAxis);
    float projectionVectorOnSpeedSphere2 = projectionVector( *vx2, *vy2, xAxis, yAxis);

    std::cout << projectionVectorOnSpeedSphere1;
    std::cout << "\n";
    std::cout << projectionVectorOnSpeedSphere2;
    std::cout << "\n";

    float reducedMassSpheres = reducedMass( mass1, mass2);

    std::cout << reducedMassSpheres;
    std::cout << "\n";

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / mass1;
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / mass2;

    *vx1 += -2 * dv1 * xAxis / moduleAxis;
    *vy1 += -2 * dv1 * yAxis / moduleAxis;

    *vx2 += -2 * dv2 * xAxis / moduleAxis;
    *vy2 += -2 * dv2 * yAxis / moduleAxis;
}

int main()
{

    const int X_MAX = 1960;
    const int Y_MAX = 1080;

    float vx1 = -30.0;
    float vy1 = 0;

    float vx2 = 30.0;
    float vy2 = 0;

    const float DT = 0.2;

    COLORREF fillColor = RGB (0, 0, 20);
    COLORREF lineColor = RGB (0, 0, 0);

    int sphereRadius1 = 100;
    int sphereRadius2 = 100;
    int numberOfCicles = 100;

    int red1 = 0;
    int green1 = 0;
    int blue1 = 255;

    int red2 = 255;
    int green2 = 0;
    int blue2 = 0;

    float x1 = X_MAX / 2;
    float y1 = Y_MAX / 2;

    float x2 = 600;
    float y2 = Y_MAX / 2;

    txCreateWindow(X_MAX, Y_MAX);
    txSetFillColour( fillColor);
    txSetColour( lineColor);

    while (true)
    {
        moveSphere(&x1, &y1, vx1, vy1, DT);
        moveSphere(&x2, &y2, vx2, vy2, DT);

        collisionSphere(x1, y1, &vx1, &vy1, X_MAX, Y_MAX, sphereRadius1 );
        collisionSphere(x2, y2, &vx2, &vy2, X_MAX, Y_MAX, sphereRadius2 );

        if ( isCollidedTwoSpheres (x1, y1, sphereRadius1, x2, y2, sphereRadius2))
            changeSpeedSphereOnCollision(x1, y1, &vx1, &vy1, 1, x2, y2, &vx2, &vy2, 1);

        txBegin();
        txClear();
        drawSphere(x1, y1, sphereRadius1, numberOfCicles, red1, green1, blue1);
        drawSphere(x2, y2, sphereRadius2, numberOfCicles, red2, green2, blue2);
        txEnd();
    }
    return 0;
}
