#include "ECSSystemManager.h"

#include <Text/String.h>
#include <System/Memory.h>

#include <Container/Array.h>
#include <Container/HashTable.h>

void ECSSystemManager_Init(ECSSystemManager* manager)
{
    manager->Signatures = MakeHashTable<ECSSignature>();
    manager->Systems = MakeHashTable<ECSSystem*>();
}

void ECSSystemManager_Free(ECSSystemManager* manager)
{
    for (int i = 0, n = manager->Systems.Count; i < n; i++)
    {
        FreeArray(&manager->Systems.Values[i]->Entities);
        MemoryFree(manager->Systems.Values[i]);
    }

    FreeHashTable(&manager->Systems);
    FreeHashTable(&manager->Signatures);
}

void ECSSystemManager_SetSignature(ECSSystemManager* manager, Symbol systemName, ECSSignature sign)
{
    U64 hash = systemName.Hash;
    DebugAssert(HashTableIndexOf(manager->Systems, hash) != -1, "System used before registered.");

    HashTableSetValue(&manager->Signatures, hash, sign);
}

void ECSSystemManager_EntityDestroyed(ECSSystemManager* manager, ECSEntity entity)
{
    for (int i = 0, n = manager->Systems.Count; i < n; i++)
    {
        ArrayRemove(&manager->Systems.Values[i]->Entities, entity);
    }
}

void ECSSystemManager_EntitySignChanged(ECSSystemManager* manager, ECSEntity entity, ECSSignature sign)
{
    for (int i = 0, n = manager->Systems.Count; i < n; i++)
    {
        U64             hash        = manager->Systems.Keys[i];
        ECSSystem*      system      = manager->Systems.Values[i];
        ECSSignature    systemSign  = HashTableGetValue(manager->Signatures, hash, 0);
    
        if ((sign & systemSign) == systemSign)
        {
            ArrayPush(&system->Entities, entity);
        }
        else
        {
            ArrayRemove(&system->Entities, entity);
        }
    }
}
