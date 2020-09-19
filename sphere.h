#include "TXLib.h"
#include <iostream>
#include <math.h>

struct Vector2f
{
    float x;
    float y;
};

struct Sphere
{
    Vector2f position;
    Vector2f oldPosition;
    Vector2f velosity;
    Vector2f acceleration;
    int radius;
    int mass;
    COLORREF colorSphere;
    COLORREF colorTrack;
};

void drawSphere( Sphere* sphere, int numberOfCicles, bool constColour = false )
{
    COLORREF fillColor = txGetFillColor();
    COLORREF lineColor = txGetColor();

    unsigned int red   = txExtractColor(sphere -> colorSphere, TX_RED) ;
    unsigned int green = txExtractColor(sphere -> colorSphere, TX_GREEN);
    unsigned int blue  = txExtractColor(sphere -> colorSphere, TX_BLUE);

    unsigned int currentCicleRed   = red;
    unsigned int currentCicleGreen = green;
    unsigned int currentCicleBlue  = blue;

    int sphereRadius = sphere -> radius;

    int x0 = sphere -> position.x;
    int y0 = sphere -> position.y;

    for (int i = 0; i < numberOfCicles; i++ )
    {
        if ( !constColour)
        {
            currentCicleRed   = red   * i / numberOfCicles;
            currentCicleGreen = green * i / numberOfCicles;
            currentCicleBlue  = blue  * i / numberOfCicles;
        }

        int currentCicleRadius = sphereRadius - sphereRadius * i / numberOfCicles;
        int x = x0 + (sphereRadius * i / 2) / numberOfCicles;
        int y = y0 - (sphereRadius * i / 2) / numberOfCicles;

        txSetFillColour( RGB ( currentCicleRed, currentCicleGreen, currentCicleBlue));
        txSetColour    ( RGB ( currentCicleRed, currentCicleGreen, currentCicleBlue));
        txCircle(x, y, currentCicleRadius);
    }

    txSetFillColour( fillColor);
    txSetColour    ( lineColor);
}

void moveSphere( Sphere* sphere, const float DT)
{
    sphere -> position.x += sphere -> velosity.x * DT + 0.5 * sphere -> acceleration.x * DT * DT;
    sphere -> position.y += sphere -> velosity.y * DT + 0.5 * sphere -> acceleration.y * DT * DT;

    sphere -> velosity.x += sphere -> acceleration.x * DT;
    sphere -> velosity.y += sphere -> acceleration.y * DT;
}

void collisionSphere( Sphere* sphere, const int X_MAX, const int Y_MAX )
{
    if ((sphere -> position.x > X_MAX - (sphere -> radius) && (sphere -> velosity.x) > 0)
             || (sphere -> position.x < (sphere -> radius) && (sphere -> velosity.x) < 0))
            sphere -> velosity.x = - sphere -> velosity.x;
    if ((sphere -> position.y > Y_MAX - (sphere -> radius) && (sphere -> velosity.y) > 0)
             || (sphere -> position.y < (sphere -> radius) && (sphere -> velosity.y) < 0))
            sphere -> velosity.y = - sphere -> velosity.y;
}

bool isCollidedTwoSpheres( Sphere* sphere1, Sphere* sphere2 )
{
    int commonRadius = (sphere1 -> radius) + (sphere2 -> radius);
    int commonRadiusSquared = commonRadius * commonRadius;

    float dx = (sphere1 -> position.x) - (sphere2 -> position.x);
    float dy = (sphere1 -> position.y) - (sphere2 -> position.y);

    float distanceSquared = dx * dx + dy * dy;

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
    if (m1 == 0 || m2 == 0)
        return 0;

    float rm = (m1 * m2) / (m1 + m2);
    return rm;
}

float moduleVector(float x, float y)
{
    return pow (x * x + y * y, 0.5 );
}

void changeSpeedSphereOnCollision( Sphere* sphere1,
                                   Sphere* sphere2 )
{
    if ( sphere1 -> mass == 0 || sphere2 -> mass == 0)
            return;

    float xAxis = sphere1 -> position.x - sphere2 -> position.x;
    float yAxis = sphere1 -> position.y - sphere2 -> position.y;
    float moduleAxis = pow (xAxis * xAxis + yAxis * yAxis, 0.5 );

    float projectionVectorOnSpeedSphere1 = projectionVector( sphere1 -> velosity.x, sphere1 -> velosity.y, xAxis, yAxis);
    float projectionVectorOnSpeedSphere2 = projectionVector( sphere2 -> velosity.x, sphere2 -> velosity.y, xAxis, yAxis);

    float reducedMassSpheres = reducedMass( sphere1 -> mass, sphere2 -> mass);

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere1 -> mass);
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere2 -> mass);

    if (dv1 < 0)
    {
        sphere1 -> velosity.x += -2 * dv1 * xAxis / moduleAxis;
        sphere1 -> velosity.y += -2 * dv1 * yAxis / moduleAxis;
    }

    if (dv2 < 0)
    {
        sphere2 -> velosity.x += 2 * dv2 * xAxis / moduleAxis;
        sphere2 -> velosity.y += 2 * dv2 * yAxis / moduleAxis;
    }
}

void drawTrack( Sphere* sphere, int numberOfCiclesInDrawTrack)
{
    COLORREF fillColor = txGetFillColor();
    COLORREF lineColor = txGetColor();

    txSetFillColour( sphere -> colorTrack);
    txSetColour    ( sphere -> colorTrack);

    int xNew = sphere -> position.x;
    int yNew = sphere -> position.y;
    int xOld = sphere -> oldPosition.x;
    int yOld = sphere -> oldPosition.y;
    int radius = sphere -> radius;

    for ( int i = 0; i <= numberOfCiclesInDrawTrack; ++i)
            txCircle(xNew + (xOld - xNew) * i / numberOfCiclesInDrawTrack, yNew + (yOld - yNew) * i / numberOfCiclesInDrawTrack, radius);

     Sphere sphereOld { Vector2f { xOld, yOld}, Vector2f { xOld, yOld}, Vector2f { 0, 0 }, Vector2f { 0, 0 },
            radius, 0, sphere -> colorTrack, sphere -> colorTrack};

     drawSphere( &sphereOld, numberOfCiclesInDrawTrack, true );

     txSetFillColour( fillColor);
     txSetColour    ( lineColor);
}

void controlSphere( Sphere* sphere, const float controllability, const float coefficientSlowdown)
{
    sphere -> acceleration.x += - sphere -> velosity.x * coefficientSlowdown;
    sphere -> acceleration.y += - sphere -> velosity.y * coefficientSlowdown;

    if (txMouseButtons() > 0 )
    {
        double mouseX = txMouseX();
        double mouseY = txMouseY();

        float differnceMouseXAndX1 = mouseX - sphere -> position.x;
        float differnceMouseYAndY1 = mouseY - sphere -> position.y;
        float modeleDiffernceMouseAndSphere1 = moduleVector( differnceMouseXAndX1, differnceMouseYAndY1);

        sphere -> acceleration.x += controllability * differnceMouseXAndX1 / modeleDiffernceMouseAndSphere1;
        sphere -> acceleration.y += controllability * differnceMouseYAndY1 / modeleDiffernceMouseAndSphere1;
    }
}
