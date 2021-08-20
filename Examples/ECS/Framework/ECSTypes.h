#pragma once

#include <System/Core.h>

constexpr I32 MAX_ENTITIES      = 5000;
constexpr I32 MAX_COMPONENTS    = 32;

using ECSEntity         = Handle;
using ECSComponentType  = I32;
using ECSSignature      = I32;

using ECSEventId        = Symbol;
using ECSParamId        = I32;

using ECSEventHandler   = void (*)(void* userData, const void* eventData);

constexpr ECSEventId ECS_EVENT_QUIT             = ConstSymbol("ECS::Events::QUIT");
constexpr ECSEventId ECS_EVENT_RESIZED          = ConstSymbol("ECS::Events::RESIZED");
constexpr ECSEventId ECS_EVENT_INPUT            = ConstSymbol("ECS::Events::INPUT");

constexpr ECSEventId ECS_EVENT_INPUT_INPUT      = ConstSymbol("ECS::Events::Input::INPUT");

constexpr ECSEventId ECS_EVENT_RESIZED_WIDTH    = ConstSymbol("ECS::Events::Resized::WIDTH");
constexpr ECSEventId ECS_EVENT_RESIZED_HEIGHT   = ConstSymbol("ECS::Events::Resized::HEIGHT");

template <typename T>
struct ECSComponentArray
{
    Symbol                              Name;
    I32                                 Count;
    T                                   Items[MAX_ENTITIES];
    HashTable<I32>                      EntityToIndex;
    HashTable<ECSEntity>                IndexToEntity;
};

struct ECSComponentArrayRef
{
    void*                               Array;
    void                                (*Free)(void* array);
    void                                (*EntityDestroyed)(void* array, const ECSEntity entity);
};

struct ECSComponentManager
{
    HashTable<ECSComponentType>         ComponentTypes;
    HashTable<ECSComponentArrayRef>     ComponentArrays;
    ECSComponentType                    NextComponentType;
};

struct ECSSystem
{
    Array<ECSEntity>                    Entities;
};

struct ECSEntityManager
{
    I32                                 LivingEntityCount;
    I32                                 AvailableEntitiesCount;
    ECSEntity                           AvailableEntities[MAX_ENTITIES];
    ECSSignature                        Signatures[MAX_ENTITIES];
};

struct ECSEventListener
{
    ECSEventHandler                     Handler;
    void*                               UserData;
};

struct ECSEventManager
{
    HashTable<Array<ECSEventListener>>  ListenerRegistry;
};

struct ECSSystemManager
{
    HashTable<ECSSignature>             Signatures;
    HashTable<ECSSystem*>               Systems;
};

struct ECSCoordinator
{
    ECSComponentManager                 ComponentManager;
    ECSEntityManager                    EntityManager;
    ECSSystemManager                    SystemManager;
    ECSEventManager                     EventManager;
};
