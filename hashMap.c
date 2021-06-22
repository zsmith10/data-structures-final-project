/*
 * CS 261 Data Structures
 * Name: Zachary Smith, Neal Gardella, Kalen Whited, Haichao Yang
 * Date: 8/10/2020
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int j = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        j += key[i];
    }
    return j;
}

int hashFunction2(const char* key)
{
    int j = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        j += (i + 1) * key[i];
    }
    return j;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */

HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);

    link->value = value;
    link->next = next;

    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);

    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
 	assert(map != NULL);

   	HashLink *current;
   	HashLink *next;

   	for (int i = 0; i < map->capacity; i++) {
    //iterate through the bucket
   	    current = map->table[i];
   	    while (current != NULL) {
            //iterate through links in the bucket and remove them
   	        next = current->next;
   	        hashLinkDelete(current);
   	        current = next;
   	    }
   	}

   	free(map->table);
   	map->size = 0;
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* newHashMap(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);

    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void deleteHashMap(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
	assert(map != NULL);
	assert(key != NULL);

	int pos = HASH_FUNCTION(key) % hashMapCapacity(map);
	HashLink* current = map->table[pos];

	while(current != NULL) {
		if(strcmp(current->key, key) == 0) {
   //update the return value
			return &(current->value);
		}
		current = current->next;
	}

	return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, 
 * all of the links need to rehashed into it because the capacity has changed.
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
//create a new table with the new number of buckets
 	HashMap* temp = newHashMap(capacity);
 	int oldSize = hashMapCapacity(map);
//iterate through bucket
 	for(int i = 0; i < oldSize; i++) {
 		HashLink *current = map->table[i];

 		while(current != NULL) {
    //add items to the table
 			hashMapPut(temp, current->key, current->value);
 			current = current->next;
 		}
 	}
//delete the old table
 	hashMapCleanUp(map);
//set the old map size to the new map table and update capacity
 	map->size = temp->size;
 	map->table = temp->table;
 	map->capacity = temp->capacity;
 	temp->table = NULL;
//delete the temporary map
 	free(temp);

}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
	int pos = HASH_FUNCTION(key) % hashMapCapacity(map);
	if(pos < 0) {
		pos += hashMapCapacity(map);
	}

	if(hashMapContainsKey(map, key)){
		int *val = hashMapGet(map, key);
		(*val) += value;
	} 
	else{
		HashLink* newLink = hashLinkNew(key, value, NULL);
		assert(newLink != NULL);

		if(map->table[pos] == NULL) {
			map->table[pos] = newLink;
		} 

		else {
			HashLink *current = map->table[pos];

			while(current->next != NULL) {
				current = current->next;
			}
			current->next = newLink;
		}
		map->size++;
	}

	if(hashMapTableLoad(map) >= MAX_TABLE_LOAD) {
		resizeTable(map, 2 * map->capacity);
	}
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
	assert(map != NULL);
	assert(key != NULL);

	int pos = HASH_FUNCTION(key) % map->capacity;
	HashLink *current = map->table[pos];
	HashLink *previous = NULL;
	if(current != NULL) {

		if(strcmp(current->key, key) == 0) {
   //if the key is at the first entry set the beginning to the next entry
			map->table[pos] = current->next;
		} 

		else {
			while(strcmp(current->key, key) != 0) {
				previous = current;
				current = current->next;
			}

			if(previous != NULL) {
				previous->next = current->next;
			}
		}
//remove link
		hashLinkDelete(current);
		map->size--;
	}
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * @param map
 * @param key
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
	assert(map != NULL);
	assert(key != NULL);

   	int pos = HASH_FUNCTION(key) % map->capacity;
   	if (pos < 0)
   	    pos += map->capacity;
//check if the key exists
   	HashLink *current = map->table[pos];

   	while (current != NULL) {
   	    if (strcmp(current->key, key) == 0)
            //update the return value
   	        return 1;
   	    current = current->next;
   	}
    return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
	assert(map != NULL);
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
	assert(map != NULL);

    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    int count = 0;
    for (int i = 0; i < map->capacity; i++) {
    	if(map->table[i] == NULL) count++;
    }

    return count;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
	assert(map != NULL);

	float size = (float)hashMapSize(map);
	float capacity = (float)hashMapCapacity(map);

	return size / capacity;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void printMap(HashMap* map)
{
	printf("\n");
	for(int i = 0; i < map->capacity; i++) {
 //iterate through buckets
		HashLink* current = map->table[i];

		if(current != NULL) {
			printf("\n");
			printf("Bucket: %i", i);
			
			while(current != NULL) {
      //loop through the links in the bucket
				printf("\nkey: %s, value: %d", current->key, current->value);
				current = current->next;
			}
		}
	}
	printf("\n");
}
