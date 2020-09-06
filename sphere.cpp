#include "TXLib.h"
#include <iostream>

void drawSphere( int x0, int y0, int sphereRadius, int numberOfCicles, int red, int green, int blue )
{
    COLORREF fillColor = txGetFillColor();
    COLORREF lineColor = txGetColor();

    for (int i = 0; i < numberOfCicles; i++ )
    {
        int currentCicleRed = red * i / numberOfCicles;
        int currentCicleGreen = green * i / numberOfCicles;
        int currentCicleBlue = blue * i / numberOfCicles;
        int currentCicleRadius = sphereRadius - sphereRadius * i / numberOfCicles;
        int x = x0 + (sphereRadius * i / 2) / numberOfCicles;
        int y = y0 - (sphereRadius * i / 2) / numberOfCicles;

        txSetFillColour( RGB ( currentCicleRed, currentCicleGreen, currentCicleBlue));
        txSetColour( RGB (currentCicleRed, currentCicleGreen, currentCicleBlue));
        txCircle(x, y, currentCicleRadius);
    }

    txSetFillColour( fillColor);
    txSetColour( lineColor);
}

int main()
{

    const int X_MAX = 1960;
    const int Y_MAX = 1080;

    float vx = 15.0;
    float vy = 10.0;

    const float DT = 0.2;

    COLORREF fillColor = RGB (0, 0, 20);
    COLORREF lineColor = RGB (0, 0, 0);

    int sphereRadius = 100;
    int numberOfCicles = 100;

    int red = 0;
    int green = 0;
    int blue = 255;

    int x = X_MAX / 2;
    int y = Y_MAX / 2;

    txCreateWindow(X_MAX, Y_MAX);
    txSetFillColour( fillColor);
    txSetColour( lineColor);

    while (true)
    {
        x += vx * DT;
        y += vy * DT;

        if ( (x > X_MAX - sphereRadius && vx > 0) || (x < sphereRadius && vx < 0))
            vx = -vx;
        if ( (y > Y_MAX - sphereRadius && vy > 0) || (y < sphereRadius && vy < 0))
            vy = -vy;

        txBegin();
        txClear();
        drawSphere(x, y, sphereRadius, numberOfCicles, red, green, blue);
        txEnd();
    }
    return 0;
}
