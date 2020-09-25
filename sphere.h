//#include "TXLib.h"
#include "SFML/Graphics.hpp"
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
    const float MASS;
    sf::Color colorSphere;
    sf::Color colorTrack;
};

void drawSphere(sf::RenderWindow* window, Sphere* sphere, int numberOfCicles, bool constColour = false)
{
    //sf::Color fillColor = txGetFillColor();
    //sf::Color lineColor = txGetColor();

    //unsigned 

    int red = sphere->colorSphere.r;
    int green = sphere->colorSphere.g;
    int blue = sphere->colorSphere.b;

    int currentCicleRed = red;
    int currentCicleGreen = green;
    int currentCicleBlue = blue;

    int sphereRadius = sphere->radius;

    int x0 = sphere->position.x;
    int y0 = sphere->position.y;

    for (int i = 0; i < numberOfCicles; i++)
    {
        if (!constColour)
        {
            currentCicleRed = red * i / numberOfCicles;
            currentCicleGreen = green * i / numberOfCicles;
            currentCicleBlue = blue * i / numberOfCicles;
        }

        int currentCicleRadius = sphereRadius - sphereRadius * i / numberOfCicles;
        int x = x0 + (sphereRadius * i / 2) / numberOfCicles;
        int y = y0 - (sphereRadius * i / 2) / numberOfCicles;

        sf::Color currentCicleColor = sf::Color(currentCicleRed, currentCicleGreen, currentCicleBlue);

        //txSetColour(RGB(currentCicleRed, currentCicleGreen, currentCicleBlue));
        sf::CircleShape cicle = sf::CircleShape(currentCicleRadius);
        cicle.setFillColor(currentCicleColor);
        cicle.setOutlineColor(currentCicleColor);
        cicle.setPosition(x - currentCicleRadius, y - currentCicleRadius);
        window->draw(cicle);
    }

    //txSetFillColour(fillColor);
    //txSetColour(lineColor);
}

void moveSphere(Sphere* sphere, const float DT)
{
    sphere->position.x += sphere->velosity.x * DT + 0.5 * sphere->acceleration.x * DT * DT;
    sphere->position.y += sphere->velosity.y * DT + 0.5 * sphere->acceleration.y * DT * DT;

    sphere->velosity.x += sphere->acceleration.x * DT;
    sphere->velosity.y += sphere->acceleration.y * DT;
}

void collisionSphere(Sphere* sphere, const int X_MAX, const int Y_MAX)
{
    if ((sphere->position.x > X_MAX - (sphere->radius) && (sphere->velosity.x) > 0)
        || (sphere->position.x < (sphere->radius) && (sphere->velosity.x) < 0))
        sphere->velosity.x = -sphere->velosity.x;
    if ((sphere->position.y > Y_MAX - (sphere->radius) && (sphere->velosity.y) > 0)
        || (sphere->position.y < (sphere->radius) && (sphere->velosity.y) < 0))
        sphere->velosity.y = -sphere->velosity.y;
}

bool isCollidedTwoSpheres(Sphere* sphere1, Sphere* sphere2)
{
    int commonRadius = (sphere1->radius) + (sphere2->radius);
    int commonRadiusSquared = commonRadius * commonRadius;

    float dx = (sphere1->position.x) - (sphere2->position.x);
    float dy = (sphere1->position.y) - (sphere2->position.y);

    float distanceSquared = dx * dx + dy * dy;

    if (distanceSquared < commonRadiusSquared)
        return true;
    return false;
}

float projectionVector(float xVector, float yVector, float xAxis, float yAxis)
{
    float scalarComposition = xVector * xAxis + yVector * yAxis;
    float moduleAxis = pow(xAxis * xAxis + yAxis * yAxis, 0.5);
    float pV = scalarComposition / moduleAxis;
    return pV;
}

float reducedMass(const float m1, const float m2)
{
    if (m1 == 0 || m2 == 0)
        return 0;

    float rm = (m1 * m2) / (m1 + m2);
    return rm;
}

float moduleVector(float x, float y)
{
    return pow(x * x + y * y, 0.5);
}

void changeSpeedSphereOnCollision(Sphere* sphere1,
    Sphere* sphere2)
{
    if (sphere1->MASS == 0 || sphere2->MASS == 0)
        return;

    float xAxis = sphere1->position.x - sphere2->position.x;
    float yAxis = sphere1->position.y - sphere2->position.y;
    float moduleAxis = pow(xAxis * xAxis + yAxis * yAxis, 0.5);

    float projectionVectorOnSpeedSphere1 = projectionVector(sphere1->velosity.x, sphere1->velosity.y, xAxis, yAxis);
    float projectionVectorOnSpeedSphere2 = projectionVector(sphere2->velosity.x, sphere2->velosity.y, xAxis, yAxis);

    float reducedMassSpheres = reducedMass(sphere1->MASS, sphere2->MASS);

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere1->MASS);
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere2->MASS);

    if (dv1 < 0)
    {
        sphere1->velosity.x += -2 * dv1 * xAxis / moduleAxis;
        sphere1->velosity.y += -2 * dv1 * yAxis / moduleAxis;
    }

    if (dv2 < 0)
    {
        sphere2->velosity.x += 2 * dv2 * xAxis / moduleAxis;
        sphere2->velosity.y += 2 * dv2 * yAxis / moduleAxis;
    }
}

void drawTrack(sf::RenderWindow* window, Sphere* sphere, int numberOfCiclesInDrawTrack)
{
    //COLORREF fillColor = txGetFillColor();
    //COLORREF lineColor = txGetColor();

    sf::Color trackColor = sphere->colorTrack;

    //txSetColour(sphere->colorTrack);

    float xNew = sphere->position.x;
    float yNew = sphere->position.y;
    float xOld = sphere->oldPosition.x;
    float yOld = sphere->oldPosition.y;
    float radius = sphere->radius;

    //std::cout << 1;

    for (int i = 0; i <= numberOfCiclesInDrawTrack; ++i)
    {
        //txCircle(xNew + (xOld - xNew) * i / numberOfCiclesInDrawTrack, yNew + (yOld - yNew) * i / numberOfCiclesInDrawTrack, radius);
        //sf::CircleShape cicle = sf::CircleShape(radius);
        //cicle.setFillColor(trackColor);
        //cicle.setOutlineColor(trackColor);
        //cicle.setPosition(xNew + (xOld - xNew) * i / numberOfCiclesInDrawTrack - radius, yNew + (yOld - yNew) * i / numberOfCiclesInDrawTrack - radius);
        //window -> draw(cicle);
    }

    Sphere sphereOld{ Vector2f { xOld, yOld}, Vector2f { xOld, yOld}, Vector2f { 0, 0 }, Vector2f { 0, 0 },
           radius, 0, sphere->colorTrack, sphere->colorTrack };

    drawSphere(window, &sphereOld, numberOfCiclesInDrawTrack, true);

    //txSetFillColour(fillColor);
    //txSetColour(lineColor);
}

void controlSphere(Sphere* sphere, const float controllability, const float coefficientSlowdown)
{
    sphere->acceleration.x += -sphere->velosity.x * coefficientSlowdown;
    sphere->acceleration.y += -sphere->velosity.y * coefficientSlowdown;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i position = sf::Mouse::getPosition();
        //double mouseX = txMouseX();
        //double mouseY = txMouseY();

        float differnceMouseXAndX1 = position.x - sphere->position.x;
        float differnceMouseYAndY1 = position.y - sphere->position.y;
        float modeleDiffernceMouseAndSphere1 = moduleVector(differnceMouseXAndX1, differnceMouseYAndY1);

        sphere->acceleration.x += controllability * differnceMouseXAndX1 / modeleDiffernceMouseAndSphere1;
        sphere->acceleration.y += controllability * differnceMouseYAndY1 / modeleDiffernceMouseAndSphere1;
    }
}
