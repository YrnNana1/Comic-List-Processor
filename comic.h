#ifndef COMIC_H
#define COMIC_H

#include <stdio.h>

// Defining a struct to hold information about a single comic.
typedef struct {
    char* date;        // Release date of the comic
    char* code;        // Unique identifier code for the comic
    char* publisher;   // Name of the publisher
    char* title;       // Title of the comic
    char* cost;        // Price of the comic as a string
} Comic;

// Defining a struct for a list of comics, including dynamic array and counters.
typedef struct {
    Comic* list;       // Pointer to an array of Comic structures
    int size;          // Current allocated size of the array
    int count;         // Number of comics currently in the list
} Comic_List;

// Functions that are going to be implemented in comic.c
void init_list(Comic_List* list);                      // Initializes a Comic_List with default values
void free_list(Comic_List* list);                      // Frees all resources associated with a Comic_List
int load_comics(Comic_List* list, const char* filename, FILE* out); // Loads comics from a CSV file
void add_comic(Comic_List* list, Comic comic);         // Adds a comic to the list, resizing if necessary
void display_list(const Comic_List *list, FILE *out);  // Displays all comics in the list
void find_comic(const Comic_List* list, int index, FILE* out); // Finds and displays a comic by its index
void save_list(const Comic_List* list, const char* filename); // Saves the list of comics to a CSV file
void buy_comic(Comic_List* purchase_list, Comic_List* store_list, int index); // Adds a comic to the purchase list
void checkout(Comic_List* purchase_list, FILE* out);   // Processes the checkout of all comics in the purchase list
void clear_list(Comic_List* list);                     // Clears the list of all comics, resetting it
void remove_comic(Comic_List* list, int index, FILE* out); // Removes a comic from the list by its index

#endif