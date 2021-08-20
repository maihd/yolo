#pragma once

#include "ECSTypes.h"

#include <System/Memory.h>
#include <Container/HashTable.h>

template <typename T>
inline void ECSComponentArray_Init(ECSComponentArray<T>* array)
{
    array->Name             = GetTypeSymbol<T>();
    array->Count            = 0;
    array->EntityToIndex    = MakeHashTable<I32>();
    array->IndexToEntity    = MakeHashTable<ECSEntity>();
}

template <typename T>
inline void ECSComponentArray_Free(void* array)
{
    ECSComponentArray<T>* typedArray = (ECSComponentArray<T>*)array;

    typedArray->Count = 0;
    FreeHashTable(&typedArray->EntityToIndex);
    FreeHashTable(&typedArray->IndexToEntity);
}

template <typename T>
inline void ECSComponentArray_AddItem(ECSComponentArray<T>* array, const ECSEntity entity, const T component)
{
    DebugAssert(!HashTableContainsKey(array->EntityToIndex, entity), "Component added to same entity more than once.");
    DebugAssert(array->Count < MAX_ENTITIES, "Too many components");

    I32 index = array->Count;
    array->Items[index] = component;
    HashTableSetValue(&array->IndexToEntity, index, entity);
    HashTableSetValue(&array->EntityToIndex, entity, index);
    array->Count++;
}

template <typename T>
inline void ECSComponentArray_RemoveItem(ECSComponentArray<T>* array, const ECSEntity entity)
{
    I32 index = HashTableGetValue(array->EntityToIndex, entity, -1);
    DebugAssert(index != -1, "Removing non-existent component.");

    // Copy element at end into deleted element's place to maintain density
    I32 indexOfRemovedElement = index;
    I32 indexOfLastElement = array->Count - 1;
    array->Items[indexOfRemovedElement] = array->Items[indexOfLastElement];

    // Update map to point moved spot
    ECSEntity entityOfLastElement = HashTableGetValue(array->IndexToEntity, (U64)indexOfLastElement);
    HashTableSetValue(&array->EntityToIndex, (U64)entityOfLastElement, indexOfRemovedElement);
    HashTableSetValue(&array->IndexToEntity, (U64)indexOfRemovedElement, entityOfLastElement);

    HashTableRemove(&array->EntityToIndex, entity);
    HashTableRemove(&array->IndexToEntity, indexOfLastElement);

    array->Count--;
}

template <typename T>
inline T* ECSComponentArray_GetItem(ECSComponentArray<T>* array, const ECSEntity entity)
{
    I32 index = HashTableGetValue(array->EntityToIndex, entity, -1);
    DebugAssert(index != -1, "Retrieving non-existent component.");
    return &array->Items[index];
}

template <typename T>
inline void ECSComponentArray_EntityDestroyed(void* array, const ECSEntity entity)
{
    ECSComponentArray<T>* typedArray = (ECSComponentArray<T>*)array;
    if (HashTableContainsKey(typedArray->EntityToIndex, entity))
    {
        ECSComponentArray_RemoveItem(typedArray, entity);
    }
}

void                    ECSComponentManager_Init(ECSComponentManager* componentManager);
void                    ECSComponentManager_Free(ECSComponentManager* componentManager);

void                    ECSComponentManager_EntityDestroyed(ECSComponentManager* componentManager, ECSEntity entity);

ECSComponentType        ECSComponentManager_GetComponentType(ECSComponentManager* componentManager, const Symbol name);
ECSComponentArrayRef    ECSComponentManager_GetComponentArray(ECSComponentManager* componentManager, const Symbol symbol);

template <typename T>
inline ECSComponentType ECSComponentManager_RegisterComponent(ECSComponentManager* componentManager)
{
    Symbol name = GetTypeSymbol<T>();

    DebugAssert(!HashTableContainsKey(componentManager->ComponentTypes, name.Hash), "Registering component type more than once.");

    ECSComponentType componentType = componentManager->NextComponentType;

    ECSComponentArray<T>* array = (ECSComponentArray<T>*)MemoryAlloc(sizeof(*array));
    ECSComponentArray_Init(array);

    ECSComponentArrayRef componentArray = { array, ECSComponentArray_Free<T>, ECSComponentArray_EntityDestroyed<T> };

    HashTableSetValue(&componentManager->ComponentTypes, name.Hash, componentType);
    HashTableSetValue(&componentManager->ComponentArrays, name.Hash, componentArray);

    componentManager->NextComponentType++;
    return componentType;
}

template <typename T>
inline ECSComponentType ECSComponentManager_GetComponentType(ECSComponentManager* componentManager)
{
    return ECSComponentManager_GetComponentType(componentManager, GetTypeSymbol<T>());
}

template <typename T>
inline ECSComponentArray<T>* ECSComponentManager_GetComponentArray(ECSComponentManager* componentManager)
{
    return (ECSComponentArray<T>*)ECSComponentManager_GetComponentArray(componentManager, GetTypeSymbol<T>()).Array;
}

template <typename T>
inline void ECSComponentManager_AddComponent(ECSComponentManager* componentManager, ECSEntity entity, T component)
{
    ECSComponentArray<T>* componentArray = ECSComponentManager_GetComponentArray<T>(componentManager);
    if (componentArray)
    {
        ECSComponentArray_AddItem(componentArray, entity, component);
    }
}

template <typename T>
inline void ECSComponentManager_RemoveComponent(ECSComponentManager* componentManager, ECSEntity entity)
{
    ECSComponentArray<T>* componentArray = ECSComponentManager_GetComponentArray<T>(componentManager);
    if (componentArray)
    {
        ECSComponentArray_RemoveItem(componentArray, entity);
    }
}

template <typename T>
inline T* ECSComponentManager_GetComponent(ECSComponentManager* componentManager, ECSEntity entity)
{
    ECSComponentArray<T>* componentArray = ECSComponentManager_GetComponentArray<T>(componentManager);
    if (componentArray)
    {
        return ECSComponentArray_GetItem(componentArray, entity);
    }

    return nullptr;
}
