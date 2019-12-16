#include <Yolo/File.h>
#include <Yolo/Math.h>
#include <Yolo/Graphics.h>

struct Entity
{
    vec2    position;
    float   rotation;
    vec2    velocity;

    vec2    scale;
    vec2    pivot;

    Texture texture;
};

struct PhysicsBody
{
    float radius;
    vec2  position;
};

struct Player
{
    Entity      entity;
    PhysicsBody body;
};

namespace EntityOps
{
    Entity Update(Entity entity, float dt)
    {
        return Entity{
            entity.position + entity.velocity * dt,
            atan2f(entity.velocity.y, entity.velocity.x),
            entity.velocity,

            entity.scale,
            entity.pivot,
            
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

    void Render(Array<Entity> entities)
    {
        for (int i = 0, n = entities.length; i < n; i++)
        {
            Entity entity = entities.elements[i];
            Graphics::DrawTexture(entity.texture, entity.position, entity.rotation, entity.scale, vec4{ 1, 1, 1, 1 }, entity.pivot);
        }
    }
}

namespace Game
{
    void Init(void)
    {
#ifndef NDEBUG
        FileOps::AddSearchPath("../Games/Spaneon/Assets");
#endif
    }

    void Update(float dt)
    {

    }

    void Render(void)
    {
        
    }
}