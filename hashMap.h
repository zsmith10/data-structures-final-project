#ifndef HASH_MAP_H
#define HASH_MAP_H 1

#define HASH_FUNCTION hashFunction1
#define MAX_TABLE_LOAD 1

typedef struct HashMap HashMap;
typedef struct HashLink HashLink;

typedef struct Passenger Passenger;

struct Passenger
{
    char* firstName;
    char* lastName;
    int age;
};

struct HashLink
{
    char* key;
    int value;
    HashLink* next;
    Passenger* passenger;
};

struct HashMap
{
    HashLink** table;	
    int size;
    int capacity;
};

HashMap* newHashMap(int capacity);
void deleteHashMap(HashMap* map);
int* hashMapGet(HashMap* map, const char* key);
void hashMapPut(HashMap* map, const char* key, int value);
void hashMapRemove(HashMap* map, const char* key);
int hashMapContainsKey(HashMap* map, const char* key);

int hashMapSize(HashMap* map);
int hashMapCapacity(HashMap* map);
int hashMapEmptyBuckets(HashMap* map);
float hashMapTableLoad(HashMap* map);
void printMap(HashMap* map);

#endif