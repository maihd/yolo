#pragma once

#include <stdlib.h>
#include <assert.h>

#include <System/Core.h>
#include <System/Memory.h>

template <typename T>
inline HashTable<T> MakeHashTable(I32 hashCount = 64)
{
    HashTable<T> result = {
        0,
        0,

        0,
        hashCount,

        nullptr,
        nullptr,
        nullptr,
    };
    return result;
}

template <typename T>
inline void FreeHashTable(HashTable<T>* hashTable)
{
    assert(hashTable);

    MemoryFree(hashTable->Nexts);
    MemoryFree(hashTable->Hashs);

    *hashTable = MakeHashTable<T>(hashTable->HashCount);
}

// Clean memory usage
template <typename T>
inline void HashTableClear(HashTable<T>* hashTable)
{
    assert(hashTable);

    hashTable->Count = 0;
}

// Find index of entry with key
template <typename T>
inline I32 HashTableIndexOf(HashTable<T> hashTable, U64 key, I32* outHash = 0, I32* outPrev = 0)
{
    if (!hashTable.Hashs || !hashTable.Count)
    {
        return -1;
    }

    I32 hash = (I32)(key % (U64)hashTable.HashCount);
    I32 curr = hashTable.Hashs[hash];
    I32 prev = -1;

    while (curr > -1)
    {
        if (hashTable.Keys[curr] == key)
        {
            break;
        }

        prev = curr;
        curr = hashTable.Nexts[curr];
    }

    if (outHash) *outHash = hash;
    if (outPrev) *outPrev = prev;
    return curr;
}

// Determine if hash table contains the entry with key
template <typename T>
inline bool HashTableContainsKey(HashTable<T> hashTable, U64 key)
{
    return HashTableIndexOf(hashTable, key) > -1;
}

// Get value of entry with key
template <typename T>
inline T HashTableGetValue(HashTable<T> hashTable, U64 key)
{
    I32 index = HashTableIndexOf(hashTable, key);
    return hashTable.Values[index];
}

// Get value of entry with key
template <typename T>
inline T HashTableGetValue(HashTable<T> hashTable, U64 key, T defaultValue)
{
    I32 index = HashTableIndexOf(hashTable, key);
    return (index > -1) ? hashTable.Values[index] : defaultValue;
}

// Get value of entry with key. If entry exists return true, false otherwise.
template <typename T>
inline bool HashTableTryGetValue(HashTable<T> hashTable, U64 key, T* outValue)
{
    assert(outValue);

    I32 index = HashTableIndexOf(hashTable, key);
    if (index > -1)
    {
        *outValue = hashTable.Values[index];
        return true;
    }
    else
    {
        return false;
    }
}

// Get value entry, if not exists create new. 
// Return true if success, false otherwise.
template <typename T>
inline bool HashTableGetValueOrNewSlot(HashTable<T>* hashTable, U64 key, T** outValueSlot)
{
    assert(hashTable);
    assert(outValueSlot);

    I32 hash, prev;
    I32 curr = HashTableIndexOf(*hashTable, key, &hash, &prev);
    if (curr < 0)
    {
        if (!hashTable->Hashs)
        {
            hashTable->Hashs = (I32*)MemoryAlloc(sizeof(I32) * hashTable->HashCount);
            //assert(hashTable->hashs, "Out of memory");

            for (I32 i = 0; i < hashTable->HashCount; i++)
            {
                hashTable->Hashs[i] = -1;
            }

            // Recalculate hash
            prev = -1;
            hash = (I32)(key % (U64)hashTable->HashCount);
        }

        if (hashTable->Count + 1 > hashTable->Capacity)
        {
            I32 oldSize = hashTable->Capacity;
            I32 newSize = oldSize | 32;

            newSize -= 1;
            newSize |= newSize >> 1;
            newSize |= newSize >> 2;
            newSize |= newSize >> 4;
            newSize |= newSize >> 8;
            newSize |= newSize >> 16;
            newSize += 1;

            I32 oldBufferSize = oldSize * (sizeof(I32) + sizeof(U64) + sizeof(T));
            I32 newBufferSize = newSize * (sizeof(I32) + sizeof(U64) + sizeof(T));
            U8* buffer = (U8*)MemoryRealloc(hashTable->Nexts, newBufferSize);

            if (oldSize > 0)
            {
                // move hashTable->values memory
                memmove(
                    buffer + newBufferSize - newSize * sizeof(T),
                    buffer + oldBufferSize - oldSize * sizeof(T),
                    oldSize * sizeof(T)
                );

                // move hashTable->keys memory
                memmove(
                    buffer + newBufferSize - newSize * (sizeof(U64) + sizeof(T)),
                    buffer + oldBufferSize - oldSize * (sizeof(U64) + sizeof(T)),
                    oldSize * (sizeof(U64) + sizeof(T))
                );
            }

            hashTable->Nexts = (I32*)buffer;
            hashTable->Keys = (U64*)(hashTable->Nexts + newSize);
            hashTable->Values = (T*)(hashTable->Keys + newSize);

            hashTable->Capacity = newSize;
        }

        curr = hashTable->Count;
        if (prev > -1)
        {
            hashTable->Nexts[prev] = curr;
        }
        else
        {
            hashTable->Hashs[hash] = curr;
        }
        hashTable->Nexts[curr] = -1;
        hashTable->Keys[curr] = key;

        hashTable->Count = hashTable->Count + 1;
    }

    *outValueSlot = &hashTable->Values[curr];
    return true;
}

// Get value entry, if not exists create new.
// Return a reference to value entry if success, otherwise abort the process.
template <typename T>
inline T* HashTableGetValueOrNewSlot(HashTable<T>* hashTable, U64 key)
{
    assert(hashTable);

    T* innerValue;
    if (!HashTableGetValueOrNewSlot(hashTable, key, &innerValue))
    {
        //ALWAYS_FALSE_ASSERT("Out of memory.");
    }

    return innerValue;
}

// Set entry's value, if not exists create new
template <typename T>
inline bool HashTableSetValue(HashTable<T>* hashTable, U64 key, T value)
{
    assert(hashTable);

    T* valueSlot;
    if (HashTableGetValueOrNewSlot(hashTable, key, &valueSlot))
    {
        *valueSlot = value;
        return true;
    }
    else
    {
        return false;
    }
}

// Remove an entry that has given key
template <typename T>
bool HashTableRemove(HashTable<T>* hashTable, U64 key)
{
    assert(hashTable);

    I32 prev;
    I32 hash;
    I32 curr = HashTableIndexOf(*hashTable, key, &hash, &prev);
    return HashTableErase(hashTable, curr, hash, prev);
}

// Remove the entry at given index
template <typename T>
inline bool HashTableErase(HashTable<T>* hashTable, I32 index)
{
    assert(hashTable);

    if (index > -1 && index < hashTable->count)
    {
        return HashTableRemove(hashTable, hashTable->keys[index]);
    }
    else
    {
        return false;
    }
}

// Remove the entry at given index, hash entry, and previous entry
template <typename T>
bool HashTableErase(HashTable<T>* hashTable, I32 curr, I32 hash, I32 prev)
{
    assert(hashTable);

    if (curr > -1)
    {
        if (prev > -1)
        {
            hashTable->Nexts[prev] = -1;
        }
        else
        {
            hashTable->Hashs[hash] = -1;
        }

        if (curr < hashTable->Count - 1)
        {
            I32 last = hashTable->Count - 1;
            hashTable->Nexts[curr] = hashTable->Nexts[last];
            hashTable->Keys[curr] = hashTable->Keys[last];
            hashTable->Values[curr] = hashTable->Values[last];

            IndexOf(*hashTable, hashTable->keys[curr], &hash, &prev);
            if (prev > -1)
            {
                hashTable->Nexts[prev] = curr;
            }
            else
            {
                hashTable->Nashs[hash] = curr;
            }
        }

        hashTable->Count = hashTable->Count - 1;
        return true;
    }
    else
    {
        return false;
    }
}
