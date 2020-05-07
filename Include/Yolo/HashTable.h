#pragma once

#include <stdlib.h>
#include <assert.h>

#include <Yolo/Types.h>

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

        free(hashTable->nexts);
        free(hashTable->hashs);

        *hashTable = HashTableOps::New<T>(hashTable->hashCount);
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
        if (!hashTable.hashs || !hashTable.count)
        {
            return -1;
        }

        I32 hash = (I32)(key % (U64)hashTable.hashCount);
        I32 curr = hashTable.hashs[hash];
        I32 prev = -1;

        while (curr > -1)
        {
            if (hashTable.keys[curr] == key)
            {
                break;
            }

            prev = curr;
            curr = hashTable.nexts[curr];
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
        return hashTable.values[index];
    }

    // Get value of entry with key
    template <typename T>
    inline T GetValue(HashTable<T> hashTable, U64 key, T defaultValue)
    {
        I32 index = IndexOf(hashTable, key);
        return (index > -1) ? hashTable.values[index] : defaultValue;
    }

    // Get value of entry with key. If entry exists return true, false otherwise.
    template <typename T>
    inline bool TryGetValue(HashTable<T> hashTable, U64 key, T* outValue)
    {
        assert(outValue);

        I32 index = IndexOf(hashTable, key);
        if (index > -1)
        {
            *outValue = hashTable.values[index];
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
            if (!hashTable->hashs)
            {
                hashTable->hashs = (I32*)malloc(sizeof(I32) * hashTable->hashCount);
                //assert(hashTable->hashs, "Out of memory");

                for (I32 i = 0; i < hashTable->hashCount; i++)
                {
                    hashTable->hashs[i] = -1;
                }

                // Recalculate hash
                prev = -1;
                hash = (I32)(key % (U64)hashTable->hashCount);
            }

            if (hashTable->count + 1 > hashTable->capacity)
            {
                I32 oldSize = hashTable->capacity;
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
                U8* buffer = (U8*)realloc(hashTable->nexts, newBufferSize);

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

                hashTable->nexts    = (I32*)buffer;
                hashTable->keys     = (U64*)(hashTable->nexts + newSize);
                hashTable->values   = (T*)(hashTable->keys + newSize);

                hashTable->capacity = newSize;
            }

            curr = hashTable->count;
            if (prev > -1)
            {
                hashTable->nexts[prev] = curr;
            }
            else
            {
                hashTable->hashs[hash] = curr;
            }
            hashTable->nexts[curr] = -1;
            hashTable->keys[curr] = key;

            hashTable->count = hashTable->count + 1;
        }

        *outValueSlot = &hashTable->values[curr];
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

        if (index > -1 && index < hashTable->count)
        {
            return Remove(hashTable, hashTable->keys[index]);
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
                hashTable->nexts[prev] = -1;
            }
            else
            {
                hashTable->hashs[hash] = -1;
            }

            if (curr < hashTable->count - 1)
            {
                I32 last = hashTable->count - 1;
                hashTable->nexts[curr]  = hashTable->nexts[last];
                hashTable->keys[curr]   = hashTable->keys[last];
                hashTable->values[curr] = hashTable->values[last];

                IndexOf(*hashTable, hashTable->keys[curr], &hash, &prev);
                if (prev > -1)
                {
                    hashTable->nexts[prev] = curr;
                }
                else
                {
                    hashTable->hashs[hash] = curr;
                }
            }

            hashTable->count = hashTable->count - 1;
            return true;
        }
        else
        {
            return false;
        }
    }
};