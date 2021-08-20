#pragma once

#include "ECSTypes.h"

#include "ECSEventManager.h"
#include "ECSEntityManager.h"
#include "ECSSystemManager.h"
#include "ECSComponentManager.h"

inline void ECSCoordinator_Init(ECSCoordinator* coordinator)
{
    ECSEventManager_Init(&coordinator->EventManager);
    ECSEntityManager_Init(&coordinator->EntityManager);
    ECSSystemManager_Init(&coordinator->SystemManager);
    ECSComponentManager_Init(&coordinator->ComponentManager);
}

inline void ECSCoordinator_Free(ECSCoordinator* coordinator)
{
    ECSEventManager_Free(&coordinator->EventManager);
    ECSEntityManager_Free(&coordinator->EntityManager);
    ECSSystemManager_Free(&coordinator->SystemManager);
    ECSComponentManager_Free(&coordinator->ComponentManager);
}

inline ECSEntity ECSCoordinator_CreateEntity(ECSCoordinator* coordinator)
{
    return ECSEntityManager_CreateEntity(&coordinator->EntityManager);
}

inline void ECSCoordinator_DestroyEntity(ECSCoordinator* coordinator, ECSEntity entity)
{
    ECSEntityManager_DestroyEntity(&coordinator->EntityManager, entity);
    ECSSystemManager_EntityDestroyed(&coordinator->SystemManager, entity);
    ECSComponentManager_EntityDestroyed(&coordinator->ComponentManager, entity);
}

template <typename T>
inline void ECSCoordinator_RegisterComponent(ECSCoordinator* coordinator)
{
    ECSComponentManager_RegisterComponent<T>(&coordinator->ComponentManager);
}

template <typename T>
inline void ECSCoordinator_AddComponent(ECSCoordinator* coordinator, ECSEntity entity, T component)
{
    ECSComponentManager_AddComponent<T>(&coordinator->ComponentManager, entity, component);

    ECSSignature signature = ECSEntityManager_GetSignature(&coordinator->EntityManager, entity);
    signature = signature | (1 << ECSComponentManager_GetComponentType<T>(&coordinator->ComponentManager));

    ECSEntityManager_SetSignature(&coordinator->EntityManager, entity, signature);
    ECSSystemManager_EntitySignChanged(&coordinator->SystemManager, entity, signature);
}

template <typename T>
inline void ECSCoordinator_RemoveComponent(ECSCoordinator* coordinator, ECSEntity entity)
{
    ECSComponentManager_RemoveComponent<T>(&coordinator->ComponentManager, entity, component);

    ECSSignature signature = ECSEntityManager_GetSignature(&coordinator->EntityManager, entity);
    signature = signature & ~(1 << ECSComponentManager_GetComponentType<T>(&coordinator->ComponentManager));

    ECSEntityManager_SetSignature(&coordinator->EntityManager, entity, signature);
    ECSSystemManager_EntitySignChanged(&coordinator->SystemManager, entity, signature);
}

template <typename T>
inline T* ECSCoordinator_GetComponent(ECSCoordinator* coordinator, ECSEntity entity)
{
    return ECSComponentManager_GetComponent<T>(&coordinator->ComponentManager, entity);
}

template <typename T>
inline ECSComponentType ECSCoordinator_GetComponentType(ECSCoordinator* coordinator)
{
    return ECSComponentManager_GetComponentType<T>(&coordinator->ComponentManager);
}

template <typename T>
inline T* ECSCoordinator_RegisterSystem(ECSCoordinator* coordinator)
{
    return ECSSystemManager_RegisterSystem<T>(&coordinator->SystemManager);
}

template <typename T>
inline void ECSCoordinator_SetSystemSignature(ECSCoordinator* coordinator, ECSSignature signature)
{
    ECSSystemManager_SetSignature<T>(&coordinator->SystemManager, signature);
}

inline void ECSCoordinator_AddEventListener(ECSCoordinator* coordinator, ECSEventId eventId, ECSEventHandler handler, void* userData)
{
    ECSEventManager_AddListener(&coordinator->EventManager, eventId, handler, userData);
}

inline void ECSCoordinator_RemoveEventListener(ECSCoordinator* coordinator, ECSEventId eventId, ECSEventHandler handler, void* userData)
{
    ECSEventManager_RemoveListener(&coordinator->EventManager, eventId, handler, userData);
}

inline void ECSCoordinator_SendEvent(ECSCoordinator* coordinator, ECSEventId eventId, const void* eventData)
{
    ECSEventManager_SendEvent(&coordinator->EventManager, eventId, eventData);
}
