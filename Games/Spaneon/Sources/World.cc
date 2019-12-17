
#include <Yolo/Math.h>
#include <Yolo/Graphics.h>

struct Entity
{
    vec2    position;
    float   rotation;
    vec2    velocity;

    vec2    scale;
    vec2    pivot;
    vec4    color;

    float   radius;

    Texture texture;
};

namespace Color
{
    vec4 HSV(float h, float s, float v)
    {
        if (h == 0 && s == 0)
            return vec4{ v, v, v, 1.0f };

        float c = s * v;
        float x = c * (1 - fabsf(fmodf(h, 2) - 1));
        float m = v - c;

        if (h < 1)      return vec4{ c + m, x + m, m, 1.0f };
        else if (h < 2) return vec4{ x + m, c + m, m, 1.0f };
        else if (h < 3) return vec4{ m, c + m, x + m, 1.0f };
        else if (h < 4) return vec4{ m, x + m, c + m, 1.0f };
        else if (h < 5) return vec4{ x + m, m, c + m, 1.0f };
        else            return vec4{ c + m, m, x + m, 1.0f };
    }
}

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

    void Render(Array<Entity> entities)
    {
        for (int i = 0, n = entities.length; i < n; i++)
        {
            Entity entity = entities.elements[i];
            Graphics::DrawTexture(entity.texture, entity.position, entity.rotation, entity.scale, entity.color, entity.pivot);
        }
    }
}

namespace World
{

}
