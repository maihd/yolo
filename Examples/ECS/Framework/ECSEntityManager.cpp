#include "ECSEntityManager.h"

#include <Container/Array.h>

void ECSEntityManager_Init(ECSEntityManager* entityManager)
{
    entityManager->LivingEntityCount = 0;
    entityManager->AvailableEntitiesCount = MAX_ENTITIES;

    for (ECSEntity i = 0, n = MAX_ENTITIES; i < n; i++)
    {
        entityManager->AvailableEntities[i] = MAX_ENTITIES - i - 1;
    }
}

void ECSEntityManager_Free(ECSEntityManager* entityManager)
{
    ECSEntityManager_Init(entityManager);
}

ECSEntity ECSEntityManager_CreateEntity(ECSEntityManager* entityManager)
{
    DebugAssert(entityManager->AvailableEntitiesCount > 0, "Too many entities existent.");

    ECSEntity entity = entityManager->AvailableEntities[--entityManager->AvailableEntitiesCount];
    entityManager->Signatures[entity] = 0;
    entityManager->LivingEntityCount++;
    return entity;
}

void ECSEntityManager_DestroyEntity(ECSEntityManager* entityManager, ECSEntity entity)
{
    DebugAssert(entityManager->AvailableEntitiesCount < MAX_ENTITIES, "Attempt to destroy unexist entity");

    entityManager->LivingEntityCount--;
    entityManager->AvailableEntities[entityManager->AvailableEntitiesCount++] = entity;
}

void ECSEntityManager_SetSignature(ECSEntityManager* entityManager, ECSEntity entity, ECSSignature signature)
{
    entityManager->Signatures[entity] = signature;
}

ECSSignature ECSEntityManager_GetSignature(ECSEntityManager* entityManager, ECSEntity entity)
{
    return entityManager->Signatures[entity];
}
