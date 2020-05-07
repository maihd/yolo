#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/Array.h>
#include <Yolo/Window.h>
#include <Yolo/Texture.h>
#include <Yolo/Graphics.h>

#include "World.h"

namespace Color
{
    Vector4 HSV(float h, float s, float v)
    {
        if (h == 0 && s == 0)
            return Vector4{ v, v, v, 1.0f };

        float c = s * v;
        float x = c * (1 - fabsf(fmodf(h, 2) - 1));
        float m = v - c;

        if (h < 1)      return Vector4{ c + m, x + m, m, 1.0f };
        else if (h < 2) return Vector4{ x + m, c + m, m, 1.0f };
        else if (h < 3) return Vector4{ m, c + m, x + m, 1.0f };
        else if (h < 4) return Vector4{ m, x + m, c + m, 1.0f };
        else if (h < 5) return Vector4{ x + m, m, c + m, 1.0f };
        else            return Vector4{ c + m, m, x + m, 1.0f };
    }
}

namespace EntityOps
{
    Entity Update(Entity entity, float dt)
    {
        return Entity{
            entity.active,

            entity.scale,
            entity.position + entity.velocity * dt,
            entity.velocity.x != 0.0f || entity.velocity.y != 0.0f ? (entity.velocity.y, entity.velocity.x) : entity.rotation,

            entity.velocity,
            entity.movespeed,

            entity.color,
            entity.radius,
            
            entity.texture,
        };
    }

    void Update(Array<Entity>* entities, float dt)
    {
        for (int i = 0, n = entities->length; i < n; i++)
        {
            entities->elements[i] = Update(entities->elements[i], dt);
        }
    }

    Entity Update(Entity entity, Vector2 bound, float dt)
    {
        Vector2  pos = entity.position + entity.velocity * dt;
        float rad = entity.radius;

        if (pos.x + rad > bound.x)
        {
            pos.x = bound.x - rad;
        }
        else if (pos.x - rad < -bound.x)
        {
            pos.x = rad - bound.x;
        }

        if (pos.y + rad > bound.y)
        {
            pos.y = bound.y - rad;
        }
        else if (pos.y - rad < -bound.y)
        {
            pos.y = rad - bound.y;
        }

        return Entity{
            entity.active,

            entity.scale,
            pos,
            entity.velocity.x != 0.0f || entity.velocity.y != 0.0f ? (entity.velocity.y, entity.velocity.x) : entity.rotation,

            entity.velocity,
            entity.movespeed,

            entity.color,
            entity.radius,
            
            entity.texture,
        };
    }

    void Update(Array<Entity>* entities, Vector2 bound, float dt)
    {
        for (int i = 0, n = entities->length; i < n; i++)
        {
            entities->elements[i] = Update(entities->elements[i], bound, dt);
        }
    }

    void Render(Entity entity)
    {
        if (entity.active)
        { 
            Graphics::DrawTexture(entity.texture, entity.position, entity.rotation, entity.scale, entity.color);
        }
    }

    void Render(Array<Entity> entities)
    {
        for (int i = 0, n = entities.length; i < n; i++)
        {
            Entity entity = entities.elements[i];
            if (entity.active)
            {
                Graphics::DrawTexture(entity.texture, entity.position, entity.rotation, entity.scale, entity.color);
            }
        }
    }
}



namespace WorldOps
{
    World New(void)
    {
        World world = {};

        world.player.active         = true;
        world.player.color          = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
        world.player.position       = Vector2{ 0.0f, 0.0f };
        world.player.rotation       = 0.0f;
        world.player.scale          = Vector2{ 1.0f, 1.0f };
        world.player.movespeed      = 720.0f;
        world.player.texture        = TextureOps::Load("Art/Player.png");
        world.player.radius         = world.player.texture.width * 0.5f;

        world.seekerSpawnRate       = 80;
        world.wandererSpawnRate     = 60;
        world.blackHoleSpawnRate    = 20;

        world.oldFire               = false;
        world.fireTimer             = 0.0f;
        world.fireInterval          = 0.1f;

        world.spawnTimer            = 0.0f;
        world.spawnInterval         = 1.0f;

        world.lock                  = false;
        world.gameOverTimer         = 0.0f;

        ArrayOps::Ensure(&world.bullets, 256);
        ArrayOps::Ensure(&world.seekers, 256);
        ArrayOps::Ensure(&world.wanderers, 256);
        ArrayOps::Ensure(&world.blackHoles, 256);

        return world;
    }
    
    void Free(World* world)
    {
        ArrayOps::Free(&world->bullets);
        ArrayOps::Free(&world->seekers);
        ArrayOps::Free(&world->wanderers);
        ArrayOps::Free(&world->blackHoles);

        ArrayOps::Free(&world->freeBullets);
        ArrayOps::Free(&world->freeSeekers);
        ArrayOps::Free(&world->freeWanderers);
        ArrayOps::Free(&world->freeBlackHoles);

        world = {};
    }

    void SpawnBullet(World* world, Vector2 pos, Vector2 vel)
    {
        Texture texture = TextureOps::Load("Art/Bullet.png");
        Entity entity = {
            true,
            Vector2{ 1.0f, 1.0f },
            pos,
            atan2f(vel.y, vel.x),

            vel,
            1280.0f,

            Vector4{1.0f, 1.0f, 1.0f, 1.0f},
            texture.height * 0.5f,
            texture,
        };

        if (world->freeBullets.length > 0)
        {
            int index = ArrayOps::Pop(&world->freeBullets);
            world->bullets.elements[index] = entity;
        }
        else
        {
            ArrayOps::Push(&world->bullets, entity);
        }
    }

    void FireBullets(World* world, Vector2 aim_dir)
    {
        float angle = atan2f(aim_dir.y, aim_dir.x) + (rand() % 101) / 100.0f * (PI * 0.025f);
        float offset = PI * 0.1f;

        aim_dir = Vector2{ cosf(angle), sinf(angle) };

        // First bullet
        {
            Vector2 vel = normalize(aim_dir);
            Vector2 pos = world->player.position + Vector2{ cosf(angle + offset), sinf(angle + offset) } * (float)world->player.texture.width * 1.25f;
            SpawnBullet(world, pos, vel);
        }

        // Second bullet
        {
            Vector2 vel = normalize(aim_dir);
            Vector2 pos = world->player.position + Vector2{ cosf(angle - offset), sinf(angle - offset) } * (float)world->player.texture.width * 1.25f;
            SpawnBullet(world, pos, vel);
        }
    }

    Vector2 GetSpawnPosition(World world)
    {
        const float min_distance_sqr = (Window::GetHeight() * 0.3f) * (Window::GetHeight() * 0.3f);

        Vector2 pos;
        do
        {
            float x = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * Window::GetWidth();
            float y = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * Window::GetHeight();
            pos = Vector2{ x, y };
        } while (distsqr(pos, world.player.position) < min_distance_sqr);

        return pos;
    }

    void SpawnSeeker(World* world)
    {
        //GameAudio::PlaySpawn();

        Vector2 pos = GetSpawnPosition(*world);
        Vector2 vel = normalize(world->player.position - pos);

        Texture texture = TextureOps::Load("Art/Seeker.png");

        Entity entity = {
            true,

            Vector2{ 1.0f, 1.0f },
            pos,
            atan2f(vel.y, vel.x),

            vel,
            360.0f,

            Vector4{1.0f, 1.0f, 1.0f, 0.0f},
            texture.width * 0.5f,
            texture
        };

        if (world->freeSeekers.length > 0)
        {
            int index = ArrayOps::Pop(&world->freeSeekers);
            world->seekers.elements[index] = entity;
        }
        else
        {
            ArrayOps::Push(&world->seekers, entity);
        }
    }

    void SpawnWanderer(World* world)
    {
        //GameAudio::PlaySpawn();

        Vector2 pos = GetSpawnPosition(*world);
        Vector2 vel = normalize(world->player.position - pos);

        Texture texture = TextureOps::Load("Art/Wanderer.png");

        Entity entity = {
            true,

            Vector2{ 1.0f, 1.0f },
            pos,
            atan2f(vel.y, vel.x),

            vel,
            240.0f,

            Vector4{1.0f, 1.0f, 1.0f, 0.0f},
            texture.width * 0.5f,
            texture
        };

        if (world->freeWanderers.length > 0)
        {
            int index = ArrayOps::Pop(&world->freeWanderers);
            world->wanderers.elements[index] = entity;
        }
        else
        {
            ArrayOps::Push(&world->wanderers, entity);
        }
    }

    void SpawnBlackhole(World* world)
    {
        //GameAudio::PlaySpawn();

        Vector2 pos = GetSpawnPosition(*world);
        Vector2 vel = Vector2{ 0.0f, 0.0f };

        Texture texture = TextureOps::Load("Art/Black Hole.png");

        Entity entity = {
            true,

            Vector2{ 1.0f, 1.0f },
            pos,
            atan2f(vel.y, vel.x),

            vel,
            240.0f,

            Vector4{1.0f, 1.0f, 1.0f, 0.0f},
            texture.width * 0.5f,
            texture
        };

        if (world->freeBlackHoles.length > 0)
        {
            int index = ArrayOps::Pop(&world->freeBlackHoles);
            world->blackHoles.elements[index] = entity;
        }
        else
        {
            ArrayOps::Push(&world->blackHoles, entity);
        }
    }

    void DestroyBullet(World* world, int index, bool explosion = false)
    {
        world->bullets.elements[index].active = false;
        ArrayOps::Push(&world->freeBullets, index);

        if (explosion)
        {
            const int PARTICLE_COUNT = 30;
            Texture texture = TextureOps::Load("Art/Laser.png");

            for (int i = 0; i < PARTICLE_COUNT; i++)
            {
                float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
                float angle = rand() % 101 / 100.0f * 2 * PI;
                Vector2  vel   = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
                Vector4  color = Vector4{ 0.6f, 1.0f, 1.0f, 1.0f };

                //ParticleSystem::SpawnParticle(texture, bullet->position, color, 1.0f, float2(1.0f), 0.0f, vel);
            }
        }
    }

    void DestroySeeker(World* world, int index)
    {
        //GameAudio::PlayExplosion();

        world->seekers.elements[index].active = false;
        ArrayOps::Push(&world->freeSeekers, index);

        Texture texture = TextureOps::Load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4  color1 = Color::HSV(hue1, 0.5f, 1);
        Vector4  color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2  vel   = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
            Vector4  color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);

            //ParticleSystem::SpawnParticle(texture, world->seekers.elements[index].position, color, 1.0f, Vector2{ 1.0f, 1.0f }, 0.0f, vel);
        }
    }

    void DestroyWanderer(World* world, int index)
    {
        //GameAudio::PlayExplosion();

        world->wanderers.elements[index].active = false;
        ArrayOps::Push(&world->freeWanderers, index);

        Texture texture = TextureOps::Load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4  color1 = Color::HSV(hue1, 0.5f, 1);
        Vector4  color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2  vel   = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
            Vector4  color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);

            //ParticleSystem::SpawnParticle(texture, world->seekers.elements[index].position, color, 1.0f, Vector2{ 1.0f, 1.0f }, 0.0f, vel);
        }
    }

    void DestroyBlackhole(World* world, int index)
    {
        //GameAudio::PlayExplosion();

        world->blackHoles.elements[index].active = false;
        ArrayOps::Push(&world->freeBlackHoles, index);

        Texture texture = TextureOps::Load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4  color1 = Color::HSV(hue1, 0.5f, 1);
        Vector4  color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2  vel   = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
            Vector4  color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);

            //ParticleSystem::SpawnParticle(texture, world->seekers.elements[index].position, color, 1.0f, Vector2{ 1.0f, 1.0f }, 0.0f, vel);
        }
    }

    void OnGameOver(World* world)
    {
        //GameAudio::PlayExplosion();

        ArrayOps::Clear(&world->bullets);
        ArrayOps::Clear(&world->seekers);
        ArrayOps::Clear(&world->wanderers);
        ArrayOps::Clear(&world->blackHoles);
        ArrayOps::Clear(&world->freeBullets);
        ArrayOps::Clear(&world->freeSeekers);
        ArrayOps::Clear(&world->freeWanderers);
        ArrayOps::Clear(&world->freeBlackHoles);

        world->gameOverTimer = 3.0f;
        Texture texture = TextureOps::Load("Art/Laser.png");

        float hue1      = rand() % 101 / 100.0f * 6.0f;
        float hue2      = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4  color1    = Color::HSV(hue1, 0.5f, 1);
        Vector4  color2    = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 1200; i++)
        {
            float speed = 10.0f * maxf((float)Window::GetWidth(), (float)Window::GetHeight()) * (0.6f + (rand() % 101 / 100.0f) * 0.4f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2  vel   = Vector2{cosf(angle) * speed, sinf(angle) * speed };

            Vector4 color  = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            //ParticleSystem::SpawnParticle(texture, player.position, color, gameOverTimer, float2(1.0f), 0.0f, vel);
        }

        world->player.position = {};
        world->player.velocity = {};
        world->player.rotation = 0.0f;
    }

    bool UpdateBlackhole(Entity* blackhole, Entity* other)
    {
        if (distance(other->position, blackhole->position) <= other->radius + blackhole->radius)
        {
            return true;
        }
        else if (distance(other->position, blackhole->position) <= other->radius + blackhole->radius * 10.0f)
        {
            Vector2 diff = blackhole->position - other->position;
            other->velocity += normalize(diff) * lerpf(1, 0, length(diff) / (Window::GetWidth() * 0.2f));
            other->velocity  = normalize(other->velocity);
        }

        return false;
    }

    void Update(World* world, float vertical, float horizontal, Vector2 aim_dir, bool fire, float dt)
    {
        if (world->gameOverTimer > 0.0f)
        {
            world->gameOverTimer -= dt;
            return;
        }

        // Update is in progress, locking the list
        world->lock = true;

        world->player.velocity = lerp(world->player.velocity, normalize(Vector2{ horizontal, vertical }), 5.0f * dt);
        world->player = EntityOps::Update(world->player, Window::GetSize(), dt);
        //if (lensqr(world->player.velocity) > 0.1f && fmodf(Time::GetTotalTime(), 0.025f) <= 0.01f)
        //{
        //    float speed;
        //    float angle = atan2f(player.velocity.y, player.velocity.x);
        //
        //    Texture glow_tex = LoadTexture("Art/Laser.png");
        //    Texture line_tex = LoadTexture("Art/Laser.png");
        //
        //    float2 vel = -0.25f * player.movespeed * player.velocity;
        //    float2 pos = player.position + 45.0f * (-player.velocity);
        //    float2 nvel = float2(vel.y, -vel.x) * 0.9f * sinf(Game::totalTime * 10.0f);
        //    float alpha = 0.7f;
        //
        //    float2 mid_vel = vel;
        //    ParticleSystem::SpawnParticle(glow_tex, pos, float4(1.0f, 0.7f, 0.1f, 1.0f) * alpha, 0.4f, float2(3.0f, 2.0f), angle, mid_vel);
        //    ParticleSystem::SpawnParticle(line_tex, pos, float4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, float2(3.0f, 1.0f), angle, mid_vel);
        //
        //    speed = rand() % 101 / 100.0f * 40.0f;
        //    angle = rand() % 101 / 100.0f * 2.0f * PI;
        //    float2 side_vel1 = vel + nvel + float2(cosf(angle), sinf(angle)) * speed;
        //    ParticleSystem::SpawnParticle(glow_tex, pos, float4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, float2(3.0f, 2.0f), angle, side_vel1);
        //    ParticleSystem::SpawnParticle(line_tex, pos, float4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, float2(3.0f, 1.0f), angle, side_vel1);
        //
        //    speed = rand() % 101 / 100.0f * 40.0f;
        //    angle = rand() % 101 / 100.0f * 2.0f * PI;
        //    float2 side_vel2 = vel - nvel + float2(cosf(angle), sinf(angle)) * speed;
        //    ParticleSystem::SpawnParticle(glow_tex, pos, float4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, float2(3.0f, 2.0f), angle, side_vel2);
        //    ParticleSystem::SpawnParticle(line_tex, pos, float4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, float2(3.0f, 1.0f), angle, side_vel2);
        //}

        for (int i = 0, n = world->bullets.length; i < n; i++)
        {
            if (world->bullets.elements[i].active)
            {
                Entity bullet = world->bullets.elements[i] = EntityOps::Update(world->bullets.elements[i], dt);
                if (bullet.position.x < -Window::GetWidth() 
                    || bullet.position.x > Window::GetWidth()
                    || bullet.position.y < -Window::GetHeight() 
                    || bullet.position.y > Window::GetHeight())
                {
                    DestroyBullet(world, i, true);
                }
            }
        }

        for (int i = 0, n = world->seekers.length; i < n; i++)
        {
            Entity* s = &world->seekers.elements[i];
            if (s->active)
            {
                if (s->color.w < 1.0f)
                {
                    s->color.w += dt;
                    if (s->color.w > 1.0f)
                    {
                        s->color.w = 1.0f;
                    }
                }
                else
                {
                    s->velocity = normalize(s->velocity + normalize(world->player.position - s->position) * 10.0f * dt);
                    *s = EntityOps::Update(*s, dt);
                }
            }
        }

        for (int i = 0, n = world->wanderers.length; i < n; i++)
        {
            Entity* s = &world->wanderers.elements[i];
            if (s->active)
            {
                if (s->color.w < 1.0f)
                {
                    s->color.w += dt;
                    if (s->color.w > 1.0f)
                    {
                        s->color.w = 1.0f;
                    }
                }
                else
                {
                    const int INTERPOLATIONS = 6;
                    const float real_speed = s->movespeed / INTERPOLATIONS;

                    float direction = atan2f(s->velocity.y, s->velocity.x);
                    for (int j = 0; j < INTERPOLATIONS; j++)
                    {
                        direction += (0.12f * (rand() % 101 / 100.0f) - 0.06f) * PI;

                        if (s->position.x < -Window::GetWidth() || s->position.x > Window::GetWidth()
                            || s->position.y < -Window::GetHeight() || s->position.y > Window::GetHeight())
                        {
                            direction = atan2f(-s->position.y, -s->position.x) + (1.0f * (rand() % 101 / 100.0f) - 0.5f) * PI;
                        }

                        s->rotation = direction;
                        s->velocity = Vector2{ cosf(direction), sinf(direction) };
                        s->position = s->position + s->velocity * real_speed * dt;
                    }
                }
            }
        }

        for (int i = 0, n = world->bullets.length; i < n; i++)
        {
            Entity* b = &world->bullets.elements[i];

            if (!b->active) continue;
            for (int j = 0, m = world->seekers.length; j < m; j++)
            {
                Entity* s = &world->seekers.elements[j];
                if (!s->active || s->color.w < 1.0f) continue;

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    DestroyBullet(world, i);
                    DestroySeeker(world, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = world->wanderers.length; j < m; j++)
            {
                Entity* s = &world->wanderers.elements[j];
                if (!s->active || s->color.w < 1.0f) continue;

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    DestroyBullet(world, i);
                    DestroyWanderer(world, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = world->blackHoles.length; j < m; j++)
            {
                Entity* s = &world->blackHoles.elements[j];
                if (!s->active || s->color.w < 1.0f) continue;

                float d = distance(b->position, s->position);
                if (d <= b->radius + s->radius)
                {
                    DestroyBullet(world, i);
                    DestroyBlackhole(world, j);
                    break;
                }
                else if (d <= b->radius + s->radius * 5.0f)
                {
                    float r = b->radius + s->radius * 5.0f;
                    float t = (d - r) / r;
                    b->velocity = normalize(b->velocity + normalize(b->position - s->position) * 0.3f);
                }
            }
        }

        for (int j = 0, m = world->seekers.length; j < m; j++)
        {
            Entity* s = &world->seekers.elements[j];
            if (!s->active || s->color.w < 1.0f) continue;

            if (distance(world->player.position, s->position) <= world->player.radius + s->radius)
            {
                OnGameOver(world);
                break;
            }
        }

        for (int j = 0, m = world->wanderers.length; j < m; j++)
        {
            Entity* s = &world->wanderers.elements[j];
            if (!s->active || s->color.w < 1.0f) continue;

            if (distance(world->player.position, s->position) <= world->player.radius + s->radius)
            {
                OnGameOver(world);
                break;
            }
        }

        for (int i = 0, n = world->blackHoles.length; i < n; i++)
        {
            Entity* s = &world->blackHoles.elements[i];
            if (s->active)
            {
                Texture glow_tex = TextureOps::Load("Art/Glow.png");
                Texture line_tex = TextureOps::Load("Art/Laser.png");

                Vector4 color1 = Vector4{ 0.3f, 0.8f, 0.4f, 1.0f };
                Vector4 color2 = Vector4{ 0.5f, 1.0f, 0.7f, 1.0f };

                if (Time::GetTotalFrames() % 3 == 0)
                {
                    float speed = 16.0f * s->radius * (0.8f + (rand() % 101 / 100.0f) * 0.2f);
                    float angle = rand() % 101 / 100.0f * Time::GetTotalTime();
                    Vector2  vel   = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
                    Vector2  pos   = s->position + 0.4f * Vector2{vel.y, -vel.x } + (4.0f + rand() % 101 / 100.0f * 4.0f);

                    Vector4  color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
                    //ParticleSystem::SpawnParticle(glow_tex, pos, color, 4.0f, float2(0.3f, 0.2f), 0.0f, vel);
                    //ParticleSystem::SpawnParticle(line_tex, pos, color, 4.0f, float2(1.0f, 1.0f), 0.0f, vel);
                }

                if (Time::GetTotalFrames() % 60 == 0)
                {
                    Texture texture = TextureOps::Load("Art/Laser.png");

                    float hue1 = rand() % 101 / 100.0f * 6.0f;
                    float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
                    Vector4  color1 = Color::HSV(hue1, 0.5f, 1);
                    Vector4  color2 = Color::HSV(hue2, 0.5f, 1);

                    for (int i = 0; i < 120.0f; i++)
                    {
                        float speed = 180.0f;
                        float angle = rand() % 101 / 100.0f * 2 * PI;
                        Vector2  vel   = Vector2{ cosf(angle) * speed, sinf(angle) * speed };
                        Vector2  pos   = s->position + vel;
                        Vector4  color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
                        //ParticleSystem::SpawnParticle(texture, pos, color, 2.0f, float2(1.0f), 0.0f, float2(0.0f));
                    }
                }

                if (s->color.w < 1.0f)
                {
                    s->color.w += dt;
                    if (s->color.w > 1.0f)
                    {
                        s->color.w = 1.0f;
                    }
                }
                else
                {
                    if (UpdateBlackhole(s, &world->player))
                    {
                        OnGameOver(world);
                        break;
                    }

                    for (int j = 0, m = world->seekers.length; j < m; j++)
                    {
                        Entity* other = &world->seekers.elements[j];
                        if (!other->active || other->color.w < 1.0f) continue;

                        if (UpdateBlackhole(s, other))
                        {
                            DestroySeeker(world, j);
                            break;
                        }
                    }

                    for (int j = 0, m = world->wanderers.length; j < m; j++)
                    {
                        Entity* other = &world->wanderers.elements[j];
                        if (!other->active || other->color.w < 1.0f) continue;

                        if (UpdateBlackhole(s, other))
                        {
                            DestroyWanderer(world, j);
                            break;
                        }
                    }
                }
            }
        }

        // Update is done, unlock the list
        world->lock = false;

        // Fire bullet if requested
        if (!fire)
        {
            world->oldFire = false;
            world->fireTimer = 0.0f;

            //GameAudio::StopShoot();
        }
        else
        {
            if (world->oldFire != fire)
            {
                world->oldFire = fire;
                world->fireTimer = world->fireInterval;
            }

            world->fireTimer += dt;
            if (world->fireTimer >= world->fireInterval)
            {
                world->fireTimer = 0;
                FireBullets(world, aim_dir);
                
                //GameAudio::PlayShoot();
            }
        }

        // Spawn enemies
        world->spawnTimer += dt;
        if (world->spawnTimer >= world->spawnInterval)
        {
            world->spawnTimer -= world->spawnInterval;

            if (rand() % 101 < world->seekerSpawnRate) SpawnSeeker(world);
            if (rand() % 101 < world->wandererSpawnRate) SpawnWanderer(world);
            if (rand() % 101 < world->blackHoleSpawnRate) SpawnBlackhole(world);
        }
    }

    void Render(World world)
    {
        if (world.gameOverTimer > 0)
        {
            return;
        }

        EntityOps::Render(world.player);
        EntityOps::Render(world.bullets);
        EntityOps::Render(world.seekers);
        EntityOps::Render(world.wanderers);
        EntityOps::Render(world.blackHoles);
    }
}
