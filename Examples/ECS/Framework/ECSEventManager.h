#pragma once

#include "ECSTypes.h"

void    ECSEventManager_Init(ECSEventManager* eventManager);
void    ECSEventManager_Free(ECSEventManager* eventManager);

void    ECSEventManager_AddListener(ECSEventManager* eventManager, ECSEventId eventId, ECSEventHandler handler, void* userData);
void    ECSEventManager_RemoveListener(ECSEventManager* eventManager, ECSEventId eventId, ECSEventHandler handler, void* userData);

void    ECSEventManager_SendEvent(ECSEventManager* eventManager, ECSEventId eventId, const void* eventData);
