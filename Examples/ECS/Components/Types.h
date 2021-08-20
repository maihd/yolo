#pragma once

#include <System/Core.h>

struct Camera
{
    Matrix4     Projection;
};

struct Player
{

};

struct Transform
{
    Vector3     Position;
    Vector3     Rotation;
    Vector3     Scale;
};

struct Renderable
{
    Vector3     Color;
};

struct Gravity
{
    Vector3     Force;
};

struct RigidBody
{
    Vector3     Velocity;
    Vector3     Acceleration;
};

struct Thrust
{
    Vector3     Force;
};
