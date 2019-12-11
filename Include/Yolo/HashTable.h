#pragma once

#include <stdlib.h>
#include <assert.h>

#include <Yolo/Types.h>

namespace HashTableOps
{
    template <typename T>
    inline HashTable<T> New(int hashCount = 64)
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

        free(hashTable->values);
        free(hashTable->keys);
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
    inline int IndexOf(HashTable<T> hashTable, uint64 key, int* outHash = 0, int* outPrev = 0)
    {
        if (!hashTable.hashs || !hashTable.count)
        {
            return -1;
        }

        int hash = (int)(key % (uint64)hashTable.hashCount);
        int curr = hashTable.hashs[hash];
        int prev = -1;

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
    inline bool ContainsKey(HashTable<T> hashTable, uint64 key)
    {
        return IndexOf(hashTable, key) > -1;
    }

    // Get value of entry with key
    template <typename T>
    inline T GetValue(HashTable<T> hashTable, uint64 key)
    {
        int index = IndexOf(hashTable, key);
        return hashTable.values[index];
    }

    // Get value of entry with key
    template <typename T>
    inline T GetValue(HashTable<T> hashTable, uint64 key, T defaultValue)
    {
        int index = IndexOf(hashTable, key);
        return (index > -1) ? hashTable.values[index] : defaultValue;
    }

    // Get value of entry with key. If entry exists return true, false otherwise.
    template <typename T>
    inline bool TryGetValue(HashTable<T> hashTable, uint64 key, T* outValue)
    {
        assert(outValue);

        int index = IndexOf(hashTable, key);
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
    inline bool GetValueOrNewSlot(HashTable<T>* hashTable, uint64 key, T** outValueSlot)
    {
        assert(hashTable);
        assert(outValueSlot);

        int hash, prev;
        int curr = IndexOf(*hashTable, key, &hash, &prev);

        if (curr < 0)
        {
            if (!hashTable->hashs)
            {
                hashTable->hashs = (int*)malloc(sizeof(int) * hashTable->hashCount);
                //assert(hashTable->hashs, "Out of memory");

                for (int i = 0; i < hashTable->hashCount; i++)
                {
                    hashTable->hashs[i] = -1;
                }

                // Recalculate hash
                prev = -1;
                hash = (int)(key % (uint64)hashTable->hashCount);
            }

            if (hashTable->count + 1 > hashTable->capacity)
            {
                int   oldSize = hashTable->capacity;
                int   newSize = oldSize | 32;

                newSize -= 1;
                newSize |= newSize >> 1;
                newSize |= newSize >> 2;
                newSize |= newSize >> 4;
                newSize |= newSize >> 8;
                newSize |= newSize >> 16;
                newSize += 1;
                
                hashTable->nexts    = (int*)    realloc(hashTable->nexts,   newSize * sizeof(int));
                hashTable->keys     = (uint64*) realloc(hashTable->keys,    newSize * sizeof(uint64));
                hashTable->values   = (T*)      realloc(hashTable->values,  newSize * sizeof(T));

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
    inline T* GetValueOrNewSlot(HashTable<T>* hashTable, uint64 key)
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
    inline bool SetValue(HashTable<T>* hashTable, uint64 key, T value)
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
    bool Remove(HashTable<T>* hashTable, uint64 key)
    {
        assert(hashTable);

        int prev;
        int hash;
        int curr = IndexOf(*hashTable, key, &hash, &prev);
        return Erase(hashTable, curr, hash, prev);
    }

    // Remove the entry at given index
    template <typename T>
    inline bool Erase(HashTable<T>* hashTable, int index)
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
    bool Erase(HashTable<T>* hashTable, int curr, int hash, int prev)
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
                int last = hashTable->count - 1;
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