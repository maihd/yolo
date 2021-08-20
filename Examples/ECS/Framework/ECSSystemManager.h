#pragma once 

#include "ECSTypes.h"

#include <System/Memory.h>
#include <Container/Array.h>
#include <Container/HashTable.h>

void            ECSSystemManager_Init(ECSSystemManager* manager);
void            ECSSystemManager_Free(ECSSystemManager* manager);

void            ECSSystemManager_SetSignature(ECSSystemManager* manager, Symbol systemName, ECSSignature signature);

void            ECSSystemManager_EntityDestroyed(ECSSystemManager* manager, ECSEntity entity);
void            ECSSystemManager_EntitySignChanged(ECSSystemManager* manager, ECSEntity entity, ECSSignature sign);

template <typename T>
T* ECSSystemManager_RegisterSystem(ECSSystemManager* manager)
{
    Symbol systemName = GetTypeSymbol<T>();

    U64 hash = systemName.Hash;
    DebugAssert(!HashTableContainsKey(manager->Systems, hash), "Registering system more than once.");

    ECSSystem* system = (ECSSystem*)MemoryAlloc(sizeof(T));
    system->Entities = MakeArray<ECSEntity>();

    HashTableSetValue(&manager->Systems, hash, system);
    return (T*)system;
}

template <typename T>
void ECSSystemManager_SetSignature(ECSSystemManager* manager, ECSSignature signature)
{
    ECSSystemManager_SetSignature(manager, ConstSymbol(GetTypeName<T>()), signature);
}
