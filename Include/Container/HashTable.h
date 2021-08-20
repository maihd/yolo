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

    MemoryFree(hashTable->Nexts); // Nexts, Keys, Values is continous in ram, so we just need call free upon Nexts
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
inline I32 HashTableIndexOf(HashTable<T> hashTable, U64 key, I32* outHashIndex = nullptr, I32* outPrevIndex = nullptr)
{
    if (!hashTable.Hashs || !hashTable.Count)
    {
        return -1;
    }

    I32 hashIndex = (I32)(key % (U64)hashTable.HashCount);
    I32 currIndex = hashTable.Hashs[hashIndex];
    I32 prevIndex = -1;

    while (currIndex > -1)
    {
        if (hashTable.Keys[currIndex] == key)
        {
            break;
        }

        prevIndex = currIndex;
        currIndex = hashTable.Nexts[currIndex];
    }

    if (outHashIndex) *outHashIndex = hashIndex;
    if (outPrevIndex) *outPrevIndex = prevIndex;
    return currIndex;
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

// Get value of entry with key. If entry exists return true, false otherwise.
template <typename T>
inline bool HashTable_TryRefValue(const HashTable<T>& hashTable, U64 key, T** outValue)
{
    assert(outValue);

    I32 index = HashTableIndexOf(hashTable, key);
    if (index > -1)
    {
        *outValue = &hashTable.Values[index];
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
inline I32 HashTableGetValueOrNewSlot(HashTable<T>* hashTable, U64 key, T** outValueSlot)
{
    assert(hashTable);
    assert(outValueSlot);

    I32 hashIndex, prevIndex;
    I32 currIndex = HashTableIndexOf(*hashTable, key, &hashIndex, &prevIndex);
    if (currIndex < 0)
    {
        if (!hashTable->Hashs)
        {
            hashTable->Hashs = (I32*)MemoryAlloc(sizeof(I32) * hashTable->HashCount);
            DebugAssert(hashTable->Hashs, "Out of memory");

            for (I32 i = 0; i < hashTable->HashCount; i++)
            {
                hashTable->Hashs[i] = -1;
            }

            // Recalculate hash
            prevIndex = -1;
            hashIndex = (I32)(key % (U64)hashTable->HashCount);
        }

        if (hashTable->Count + 1 > hashTable->Capacity)
        {
            const I32 oldCapacity = hashTable->Capacity;
            const I32 newCapacity = oldCapacity > 0 ? oldCapacity * 2 : 32;

            const I32 oldBufferSize = oldCapacity * (sizeof(I32) + sizeof(U64) + sizeof(T));
            const I32 newBufferSize = newCapacity * (sizeof(I32) + sizeof(U64) + sizeof(T));
            
            U8* oldBuffer = (U8*)hashTable->Nexts;
            U8* newBuffer = (U8*)MemoryAlloc(newBufferSize);

            if (oldCapacity > 0 && oldBuffer != nullptr)
            {
                // Copy old hashTable->Nexts memory
                MemoryCopy(
                    newBuffer,
                    oldBuffer,
                    oldCapacity * sizeof(I32)
                );

                // Copy old hashTable->Keys memory
                MemoryCopy(
                    newBuffer + newCapacity * sizeof(I32),
                    oldBuffer + oldCapacity * sizeof(I32),
                    oldCapacity * sizeof(U64)
                );
            
                // Copy old hashTable->Values memory
                MemoryCopy(
                    newBuffer + newCapacity * (sizeof(I32) + sizeof(U64)),
                    oldBuffer + oldCapacity * (sizeof(I32) + sizeof(U64)),
                    oldCapacity * sizeof(T)
                );
            }
            
            // Release old buffer
            MemoryFree(oldBuffer);

            hashTable->Nexts    = (I32*) newBuffer;
            hashTable->Keys     = (U64*)(newBuffer + newCapacity *  sizeof(I32));
            hashTable->Values   = (T*)  (newBuffer + newCapacity * (sizeof(I32) + sizeof(U64)));
            hashTable->Capacity = newCapacity;
        }
        
        // Append the value to the tail of the array
        currIndex = hashTable->Count;
        if (prevIndex > -1)
        {
            hashTable->Nexts[prevIndex] = currIndex;
        }
        else
        {
            hashTable->Hashs[hashIndex] = currIndex;
        }

        hashTable->Nexts[currIndex] = -1;
        hashTable->Keys[currIndex] = key;
        hashTable->Count++;
    }

    *outValueSlot = &hashTable->Values[currIndex];
    return currIndex;
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
inline I32 HashTableSetValue(HashTable<T>* hashTable, U64 key, T value)
{
    assert(hashTable);

    T* valueSlot;
    I32 index = HashTableGetValueOrNewSlot(hashTable, key, &valueSlot);
    if (index != -1)
    {
        *valueSlot = value;
    }

    return index;
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

            HashTableIndexOf(*hashTable, hashTable->Keys[curr], &hash, &prev);
            if (prev > -1)
            {
                hashTable->Nexts[prev] = curr;
            }
            else
            {
                hashTable->Hashs[hash] = curr;
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
