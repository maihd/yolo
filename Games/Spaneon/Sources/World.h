#pragma once

#include <Yolo/Types.h>

struct Entity
{
    bool    active;

    Vector2    scale;
    Vector2    position;
    float   rotation;

    Vector2    velocity;
    float   movespeed;

    Vector4    color;
    float   radius;

    Texture texture;
};

struct World
{
    Entity          player;

    Array<Entity>   bullets;
    Array<Entity>   seekers;
    Array<Entity>   wanderers;
    Array<Entity>   blackHoles;

    Array<int>      freeBullets;
    Array<int>      freeSeekers;
    Array<int>      freeWanderers;
    Array<int>      freeBlackHoles;

    int             seekerSpawnRate;
    int             wandererSpawnRate;
    int             blackHoleSpawnRate;

    bool            oldFire;
    float           fireTimer;
    float           fireInterval;

    float           spawnTimer;
    float           spawnInterval;

    bool            lock;
    float           gameOverTimer;
};

namespace WorldOps
{
    World   New(void);
    void    Free(World* world);

    void    Update(World* world, float horizontalInput, float verticalInput, Vector2 aimDir, bool fire, float deltaTime);
    void    Render(World world);    
}
