#include "ECSEventManager.h"

#include <Container/Array.h>
#include <Container/HashTable.h>

inline bool operator==(ECSEventListener a, ECSEventListener b)
{
    return a.Handler == b.Handler && a.UserData == b.UserData;
}

inline bool operator!=(ECSEventListener a, ECSEventListener b)
{
    return a.Handler != b.Handler || a.UserData != b.UserData;
}

void ECSEventManager_Init(ECSEventManager* eventManager)
{
    eventManager->ListenerRegistry = MakeHashTable<Array<ECSEventListener>>();
}

void ECSEventManager_Free(ECSEventManager* eventManager)
{
    FreeHashTable(&eventManager->ListenerRegistry);
}

void ECSEventManager_AddListener(ECSEventManager* eventManager, ECSEventId eventId, ECSEventHandler handler, void* userData)
{
    Array<ECSEventListener>* listeners = HashTableGetValueOrNewSlot(&eventManager->ListenerRegistry, eventId.Hash);
    ArrayPush(listeners, ECSEventListener{ handler, userData });
}

void ECSEventManager_RemoveListener(ECSEventManager* eventManager, ECSEventId eventId, ECSEventHandler handler, void* userData)
{
    Array<ECSEventListener>* listeners;
    if (HashTable_TryRefValue(eventManager->ListenerRegistry, eventId.Hash, &listeners))
    {
        ArrayRemove(listeners, ECSEventListener{ handler, userData });
    }
}

void ECSEventManager_SendEvent(ECSEventManager* eventManager, ECSEventId eventId, const void* eventData)
{
    Array<ECSEventListener> listeners;
    if (HashTableTryGetValue(eventManager->ListenerRegistry, eventId.Hash, &listeners))
    {
        for (ECSEventListener listener : IterateArray(listeners))
        {
            listener.Handler(listener.UserData, eventData);
        }
    }
}
