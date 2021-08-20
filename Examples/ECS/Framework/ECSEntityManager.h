#pragma once

#include "ECSTypes.h"

void            ECSEntityManager_Init(ECSEntityManager* entityManager);
void            ECSEntityManager_Free(ECSEntityManager* entityManager);

ECSEntity       ECSEntityManager_CreateEntity(ECSEntityManager* entityManager);
void            ECSEntityManager_DestroyEntity(ECSEntityManager* entityManager, ECSEntity entity);

void            ECSEntityManager_SetSignature(ECSEntityManager* entityManager, ECSEntity entity, ECSSignature signature);
ECSSignature    ECSEntityManager_GetSignature(ECSEntityManager* entityManager, ECSEntity entity);
