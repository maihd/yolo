#include "ECSComponentManager.h"

void ECSComponentManager_Init(ECSComponentManager* componentManager)
{
    componentManager->ComponentTypes = MakeHashTable<ECSComponentType>();
    componentManager->ComponentArrays = MakeHashTable<ECSComponentArrayRef>();
    componentManager->NextComponentType = 0;
}

void ECSComponentManager_Free(ECSComponentManager* componentManager)
{
    for (I32 i = 0, n = componentManager->ComponentArrays.Count; i < n; i++)
    {
        ECSComponentArrayRef* ref = &componentManager->ComponentArrays.Values[i];
        ref->Free(ref->Array);
        MemoryFree(ref->Array);
    }

    FreeHashTable(&componentManager->ComponentArrays);
    FreeHashTable(&componentManager->ComponentTypes);
    componentManager->NextComponentType = 0;
}

void ECSComponentManager_EntityDestroyed(ECSComponentManager* componentManager, ECSEntity entity)
{
    for (I32 i = 0, n = componentManager->ComponentArrays.Count; i < n; i++)
    {
        ECSComponentArrayRef* ref = &componentManager->ComponentArrays.Values[i];
        ref->EntityDestroyed(ref->Array, entity);
    }
}

ECSComponentType ECSComponentManager_GetComponentType(ECSComponentManager* componentManager, const Symbol name)
{
    I32 index = HashTableIndexOf(componentManager->ComponentTypes, name.Hash);
    DebugAssert(index != -1, "Component not registered before use.");

    return componentManager->ComponentTypes.Values[index];
}

ECSComponentArrayRef ECSComponentManager_GetComponentArray(ECSComponentManager* componentManager, const Symbol name)
{
    I32 index = HashTableIndexOf(componentManager->ComponentArrays, name.Hash);
    DebugAssert(index != -1, "Component not registered before use.");

    return componentManager->ComponentArrays.Values[index];
}
