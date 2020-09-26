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
    Vector2f velocity;
    Vector2f acceleration;
    int radius;
    const float MASS;
    sf::Color colorSphere;
    sf::Color colorTrack;
};

void drawSphere(sf::RenderWindow* window, Sphere* sphere, int numberOfCicles, bool constColour = false)
{
    int red = sphere->colorSphere.r;
    int green = sphere->colorSphere.g;
    int blue = sphere->colorSphere.b;

    int currentCircleRed = red;
    int currentCircleGreen = green;
    int currentCircleBlue = blue;

    int sphereRadius = sphere->radius;

    int x0 = sphere->position.x;
    int y0 = sphere->position.y;

    for (int i = 0; i < numberOfCicles; i++)
    {
        if (!constColour)
        {
            currentCircleRed = red * i / numberOfCicles;
            currentCircleGreen = green * i / numberOfCicles;
            currentCircleBlue = blue * i / numberOfCicles;
        }

        int currentCircleRadius = sphereRadius - sphereRadius * i / numberOfCicles;
        int x = x0 + (sphereRadius * i / 2) / numberOfCicles;
        int y = y0 - (sphereRadius * i / 2) / numberOfCicles;

        sf::Color currentCircleColor = sf::Color(currentCircleRed, currentCircleGreen, currentCircleBlue);

        sf::CircleShape circle = sf::CircleShape(currentCircleRadius);
        circle.setFillColor(currentCircleColor);
        circle.setOutlineColor(currentCircleColor);
        circle.setPosition(x - currentCircleRadius, y - currentCircleRadius);
        window->draw(circle);
    }
}

void moveSphere(Sphere* sphere, const float DT)
{
    sphere->position.x += sphere->velocity.x * DT + 0.5 * sphere->acceleration.x * DT * DT;
    sphere->position.y += sphere->velocity.y * DT + 0.5 * sphere->acceleration.y * DT * DT;

    sphere->velocity.x += sphere->acceleration.x * DT;
    sphere->velocity.y += sphere->acceleration.y * DT;
}

void collisionSphere(Sphere* sphere, const int X_MAX, const int Y_MAX)
{
    if ((sphere->position.x > X_MAX - (sphere->radius) && (sphere->velocity.x) > 0)
        || (sphere->position.x < (sphere->radius) && (sphere->velocity.x) < 0))
        sphere->velocity.x = -sphere->velocity.x;
    if ((sphere->position.y > Y_MAX - (sphere->radius) && (sphere->velocity.y) > 0)
        || (sphere->position.y < (sphere->radius) && (sphere->velocity.y) < 0))
        sphere->velocity.y = -sphere->velocity.y;
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

    float projectionVectorOnSpeedSphere1 = projectionVector(sphere1->velocity.x, sphere1->velocity.y, xAxis, yAxis);
    float projectionVectorOnSpeedSphere2 = projectionVector(sphere2->velocity.x, sphere2->velocity.y, xAxis, yAxis);

    float reducedMassSpheres = reducedMass(sphere1->MASS, sphere2->MASS);

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere1->MASS);
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere2->MASS);

    if (dv1 < 0)
    {
        sphere1->velocity.x += -2 * dv1 * xAxis / moduleAxis;
        sphere1->velocity.y += -2 * dv1 * yAxis / moduleAxis;
    }

    if (dv2 < 0)
    {
        sphere2->velocity.x += 2 * dv2 * xAxis / moduleAxis;
        sphere2->velocity.y += 2 * dv2 * yAxis / moduleAxis;
    }
}

void drawTrack(sf::RenderWindow* window, Sphere* sphere, int numberOfCiclesInDrawTrack)
{
    sf::Color trackColor = sphere->colorTrack;

    float xNew = sphere->position.x;
    float yNew = sphere->position.y;
    float xOld = sphere->oldPosition.x;
    float yOld = sphere->oldPosition.y;
    float radius = sphere->radius;

    for (int i = 0; i <= numberOfCiclesInDrawTrack; ++i)
    {
        sf::CircleShape circle = sf::CircleShape(radius);
        circle.setFillColor(trackColor);
        circle.setOutlineColor(trackColor);
        circle.setPosition(xNew + (xOld - xNew) * i / numberOfCiclesInDrawTrack - radius, yNew + (yOld - yNew) * i / numberOfCiclesInDrawTrack - radius);
        window -> draw(circle);
    }

    Sphere sphereOld{ Vector2f { xOld, yOld}, Vector2f { xOld, yOld}, Vector2f { 0, 0 }, Vector2f { 0, 0 },
           radius, 0, sphere->colorTrack, sphere->colorTrack };

    drawSphere(window, &sphereOld, numberOfCiclesInDrawTrack, true);
}

void controlSphere(Sphere* sphere, const float controllability, const float coefficientSlowdown)
{
    sphere->acceleration.x += -sphere->velocity.x * coefficientSlowdown;
    sphere->acceleration.y += -sphere->velocity.y * coefficientSlowdown;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i position = sf::Mouse::getPosition();

        float differnceMouseXAndX1 = position.x - sphere->position.x;
        float differnceMouseYAndY1 = position.y - sphere->position.y;
        float modeleDiffernceMouseAndSphere1 = moduleVector(differnceMouseXAndX1, differnceMouseYAndY1);

        sphere->acceleration.x += controllability * differnceMouseXAndX1 / modeleDiffernceMouseAndSphere1;
        sphere->acceleration.y += controllability * differnceMouseYAndY1 / modeleDiffernceMouseAndSphere1;
    }
}
