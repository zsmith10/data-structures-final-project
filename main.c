/*
 * CS 261 Data Structures
 * Name: Zachary Smith, Neal Gardella, Kalen Whited, Haichao Yang
 * Date: 8/10/2020
 */
#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key);


void popFlight(HashMap* flight){
    char* plane[20] = {"1A","1B","1C","2D","2A","2B","2C","2D","3A","3B",
                       "3C","3D","4A","4B","4C","4D","5A","5B","5C","5D"};
    for(int i = 0; i < 20; i++){
        int occur = hashMapContainsKey(flight, plane[i]);
        if(occur == 0){

        hashMapPut(flight,plane[i], 1);
    }

    else{

        hashMapPut(flight,plane[i],(occur + 1));
    }
  }
}
    
void fillSeat(HashMap* flight, const char* key, char* first, char* last, int age){
    int pos = HASH_FUNCTION(key) % flight->capacity;

    flight->table[pos]->passenger = malloc(sizeof(Passenger));
    flight->table[pos]->passenger->firstName = first;
    flight->table[pos]->passenger->lastName = last;
    flight->table[pos]->passenger->age = age;
}

void freeseat(HashMap* flight, const char* key)
{
    int pos = HASH_FUNCTION(key) % flight->capacity;
    free(flight->table[pos]->passenger);
}

void printSeat(HashMap* flight, const char* key){
    int pos = HASH_FUNCTION(key) % flight->capacity;

    printf("\nSeat: %s\n", key);
    printf("First Name: %s\n", flight->table[pos]->passenger->firstName);
    printf("Last Name: %s\n", flight->table[pos]->passenger->lastName);
    printf("Age: %d\n", flight->table[pos]->passenger->age);
}


int main(int argc, const char** argv)
{
   
    HashMap* pdxLAX = newHashMap(20);
    popFlight(pdxLAX);

    fillSeat(pdxLAX,"1A","John","Smith", 23);
    fillSeat(pdxLAX,"2A","Amanda","Smith", 26);
    fillSeat(pdxLAX,"2B","Jermaine","Johnson", 39);
    fillSeat(pdxLAX,"5C","Rex","Moody", 87);
    
    int age;
    char first[20];
    char last[20];
    
    printf("Please enter your first name: \n");
    fgets(first,20,stdin);
    printf("Please enter your last name: \n");
    fgets(last,20,stdin);
    printf("Please enter your age: \n");
    scanf("%d", &age);
    printf("\n");
   
    fillSeat(pdxLAX,"5A",first,last,age);

    printf("\n\nPortland -> Los Angeles");
    printSeat(pdxLAX, "1A");
    printSeat(pdxLAX, "2A");
    printSeat(pdxLAX, "2B");
    printSeat(pdxLAX, "5C");
    printSeat(pdxLAX, "5A"); 
 
    HashMap* pdxATL = newHashMap(20);
    popFlight(pdxATL);
    fillSeat(pdxATL,"1B","Ron","Briney", 29);
    fillSeat(pdxATL,"3A","Alex","Gonzalez", 32);
    fillSeat(pdxATL,"4D","Eshan","Chopra", 62);
    fillSeat(pdxATL,"5D","Mandy","Jones", 96);

    printf("\n\nPortland -> Atlanta");
    printSeat(pdxATL, "1B");
    printSeat(pdxATL, "3A");
    printSeat(pdxATL, "4D");
    printSeat(pdxATL, "5D");
    
    freeseat(pdxLAX, "1A");
    freeseat(pdxLAX, "2A");
    freeseat(pdxLAX, "2B");
    freeseat(pdxLAX, "5C");
    freeseat(pdxLAX, "5A");
    freeseat(pdxATL, "1B");
    freeseat(pdxATL, "3A");
    freeseat(pdxATL, "4D");
    freeseat(pdxATL, "5D");
   

    deleteHashMap(pdxLAX);
    deleteHashMap(pdxATL);
 
    return 0;
}
