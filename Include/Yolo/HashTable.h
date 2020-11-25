#pragma once

#include <stdlib.h>
#include <assert.h>

#include <Yolo/Types.h>
#include <Yolo/Memory.h>

namespace HashTableOps
{
    template <typename T>
    inline HashTable<T> New(I32 hashCount = 64)
    {
        return HashTable<T>{
            0,
            0,

            0,
            hashCount,
            
            0,
            0,
            0,
        };
    }

    template <typename T>
    inline void Free(HashTable<T>* hashTable)
    {
        assert(hashTable);

        MemoryFree(hashTable->Nexts);
        MemoryFree(hashTable->Hashs);

        *hashTable = HashTableOps::New<T>(hashTable->HashCount);
    }

    // Clean memory usage
    template <typename T>
    inline void Clear(HashTable<T>* hashTable)
    {
        assert(hashTable);

        hashTable->count = 0;
    }

    // Find index of entry with key
    template <typename T>
    inline I32 IndexOf(HashTable<T> hashTable, U64 key, I32* outHash = 0, I32* outPrev = 0)
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
    inline bool ContainsKey(HashTable<T> hashTable, U64 key)
    {
        return IndexOf(hashTable, key) > -1;
    }

    // Get value of entry with key
    template <typename T>
    inline T GetValue(HashTable<T> hashTable, U64 key)
    {
        I32 index = IndexOf(hashTable, key);
        return hashTable.Values[index];
    }

    // Get value of entry with key
    template <typename T>
    inline T GetValue(HashTable<T> hashTable, U64 key, T defaultValue)
    {
        I32 index = IndexOf(hashTable, key);
        return (index > -1) ? hashTable.Values[index] : defaultValue;
    }

    // Get value of entry with key. If entry exists return true, false otherwise.
    template <typename T>
    inline bool TryGetValue(HashTable<T> hashTable, U64 key, T* outValue)
    {
        assert(outValue);

        I32 index = IndexOf(hashTable, key);
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
    inline bool GetValueOrNewSlot(HashTable<T>* hashTable, U64 key, T** outValueSlot)
    {
        assert(hashTable);
        assert(outValueSlot);

        I32 hash, prev;
        I32 curr = IndexOf(*hashTable, key, &hash, &prev);

        if (curr < 0)
        {
            if (hashTable->Hashs == nullptr)
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
                I32 newSize = oldSize > 32 ? oldSize : 32;

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

                hashTable->Nexts    = (I32*)buffer;
                hashTable->Keys     = (U64*)(hashTable->Nexts + newSize);
                hashTable->Values   = (T*)(hashTable->Keys + newSize);

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
    inline T* GetValueOrNewSlot(HashTable<T>* hashTable, U64 key)
    {
        assert(hashTable);

        T* innerValue;
        if (!GetValueOrNewSlot(hashTable, key, &innerValue))
        {
            //ALWAYS_FALSE_ASSERT("Out of memory.");
        }

        return innerValue;
    }

    // Set entry's value, if not exists create new
    template <typename T>
    inline bool SetValue(HashTable<T>* hashTable, U64 key, T value)
    {
        assert(hashTable);

        T* valueSlot;
        if (GetValueOrNewSlot(hashTable, key, &valueSlot))
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
    bool Remove(HashTable<T>* hashTable, U64 key)
    {
        assert(hashTable);

        I32 prev;
        I32 hash;
        I32 curr = IndexOf(*hashTable, key, &hash, &prev);
        return Erase(hashTable, curr, hash, prev);
    }

    // Remove the entry at given index
    template <typename T>
    inline bool Erase(HashTable<T>* hashTable, I32 index)
    {
        assert(hashTable);

        if (index > -1 && index < hashTable->Count)
        {
            return Remove(hashTable, hashTable->Keys[index]);
        }
        else
        {
            return false;
        }
    }

    // Remove the entry at given index, hash entry, and previous entry
    template <typename T>
    bool Erase(HashTable<T>* hashTable, I32 curr, I32 hash, I32 prev)
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

            if (curr < hashTable->count - 1)
            {
                I32 last = hashTable->count - 1;
                hashTable->Nexts[curr]  = hashTable->Nexts[last];
                hashTable->Keys[curr]   = hashTable->Keys[last];
                hashTable->Values[curr] = hashTable->Values[last];

                IndexOf(*hashTable, hashTable->Keys[curr], &hash, &prev);
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
};