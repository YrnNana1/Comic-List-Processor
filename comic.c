#include "comic.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // this is for the isdigit function

/**
 * This funciton nitializes a comic list with a predefined initial capacity.
 * it sets up the list structure with allocated memory and initializes the size and count.
 *
 * @param list Pointer to the Comic_List to initialize.
 */
void init_list(Comic_List* list) {
    list->list = (Comic*) malloc(10 * sizeof(Comic));  // Allocating memory for 10 comics initially.
    list->size = 10;  // Sets the initial capacity of the list.
    list->count = 0;  // Starting with an empty list.
}

/**
 * This function frees all memory associated with 
 * the comics in the list and then the list itself.
 *
 * @param list Pointer to the Comic_List to be freed.
 */
void free_list(Comic_List* list) {
    for (int i = 0; i < list->count; i++) {  // Iterates over each comic in the list.
        // Freeing all heap-allocated fields in the Comic struct.
        free(list->list[i].date);
        free(list->list[i].code);
        free(list->list[i].publisher);
        free(list->list[i].title);
        free(list->list[i].cost);
    }
    free(list->list);  // Frees the list array itself.
    list->list = NULL;  // Setting the list pointer to NULL to prevent use after free.
    list->size = 0;  // Resets the size of the list.
    list->count = 0;  // Resets the count of comics in the list.
}

/**
 * This funtion loads comics from a specified CSV file into a comic list
 * it also reports the number of comics loaded.
 *
 * @param list Pointer to the Comic_List to populate.
 * @param filename Path to the CSV file containing comic data.
 * @param out FILE pointer to output any results or errors.
 * @return The number of comics successfully loaded into the list.
 */
int load_comics(Comic_List* list, const char* filename, FILE* out) {
    FILE* file = fopen(filename, "r");  // Opening the file for reading.
    if (!file) {
        fprintf(out, "Error opening file: %s\n", filename);  // handeling erros if file isn't opened correctly
        return 0;
    }

    int comicsLoaded = 0;  // Counter for the number of comics loaded.
    char buffer1[100], buffer2[100], buffer3[100], buffer4[1000], buffer5[10];
    fscanf(file, "%*[^\n]\n");  // Skips the header line of the CSV.

    // Reading each comic's data from the file and storing it in the list.
    while (fscanf(file, "%99[^,],%99[^,],%99[^,],%999[^,],%9s\n", buffer1, buffer2, buffer3, buffer4, buffer5) == 5) {
        Comic comic = {strdup(buffer1), strdup(buffer2), strdup(buffer3), strdup(buffer4), strdup(buffer5)};
        add_comic(list, comic);
        comicsLoaded++;
    }
    fclose(file);  // Closes the file after reading.

    fprintf(out, "\tNumber of comics: %d\n", comicsLoaded);  // Reporting the number of loaded comics.
    return comicsLoaded;
}

/**
 * This function dds a comic to the comic list. If the list is full, it doubles the list's capacity.
 * it handles reallocation of the list
 *
 * @param list Pointer to the Comic_List where the comic will be added.
 * @param comic The Comic to add to the list.
 */
void add_comic(Comic_List* list, Comic comic) {
    if (list->count >= list->size) {  // Checking if the list is full.
        list->size *= 2;  // Doubling the list size.
        list->list = realloc(list->list, list->size * sizeof(Comic));  // Reallocating memory.
    }
    list->list[list->count++] = comic;  // Adds the comic and increment the count.
}

/**
 * This function displays all comics in the list 
 * the details of the comics are also formatted
 *
 * @param list Pointer to the Comic_List to display.
 * @param out FILE pointer to the output file for display output.
 */
void display_list(const Comic_List *list, FILE *out) {
    if (list->count == 0) {
        fprintf(out, "List is currently empty.\n");  // Displays this message if the list is empty.
    } else {
        for (int i = 0; i < list->count; i++) {  // Looping through each comic in the list.
            fprintf(out, "Comic Number: %d\n\tDate: %s\n\tCode: %s\n\tPublisher: %s\n\tTitle: %s\n\tCost: %s\n",
                    i + 1, list->list[i].date, list->list[i].code, list->list[i].publisher, list->list[i].title, list->list[i].cost);
        }
    }
}

/**
 * This function saves the comic list to a specified file in CSV format
 * just like the provided csv files 
 *
 * @param list Pointer to the Comic_List to save.
 * @param filename The filename where the list will be saved.
 */
void save_list(const Comic_List* list, const char* filename) {
    FILE* file = fopen(filename, "w");  // Opening the file for writing.
    fprintf(file, "DATE,CODE,PUBLISHER,TITLE,PRICE\n");  // Writes the CSV header.
    for (int i = 0; i < list->count; i++) {  // Writes each comic's data to the file.
        fprintf(file, "%s,%s,%s,%s,%s\n", list->list[i].date, list->list[i].code, list->list[i].publisher, list->list[i].title, list->list[i].cost);
    }
    fclose(file);  // Closing the file after writing.
}

/**
 * This function removes a comic from the list at the specified index
 * Then shifts the rest of the comics to fill the gaps
 *
 * @param list Pointer to the Comic_List from which to remove a comic.
 * @param index The index of the comic to remove.
 * @param out FILE pointer to output the result or error message.
 */
void remove_comic(Comic_List* list, int index, FILE* out) {
    if (index < 0 || index >= list->count) {
        fprintf(out, "Comic at index %d was not removed\n", index);  // handling if the index is incorrect
        return;
    }

    // Freeing memory for the comic to be removed.
    free(list->list[index].date);
    free(list->list[index].code);
    free(list->list[index].publisher);
    free(list->list[index].title);
    free(list->list[index].cost);

    // Shifts the remaining comics down in the list to fill the gap.
    for (int i = index; i < list->count - 1; i++) {
        list->list[i] = list->list[i+1];
    }
    list->count--;  // Decrementing the count after removing the comic.

    fprintf(out, "Comic at index %d successfully removed\n", index);  // printing out that the removal was successful
}

/**
 * This function resets the comic list by clearing all comics and reinitializing it to the default state.
 *
 * @param list Pointer to the Comic_List to clear and reinitialize.
 */
void clear_list(Comic_List* list) {
    free_list(list);  // Frees all allocated memory within the list.
    init_list(list);  // Reinitializes the list to its default state.
}


/**
 * This function deep copies the attributes of one comic to another.
 * it allocates memory for each field of the comic
 *
 * @param dest Pointer to the destination comic where the data will be copied.
 * @param src Pointer to the source comic from which data is copied.
 */
static void copy_comic(Comic* dest, Comic* src) {
    dest->date = strdup(src->date);
    dest->code = strdup(src->code);
    dest->publisher = strdup(src->publisher);
    dest->title = strdup(src->title);
    dest->cost = strdup(src->cost);
}

/**
 * This function adds a comic to a purchase list from the store list if the index is valid.
 * it checks for the validity of the index, resizes the purchase list if necessary, 
 * and uses a deep copy to prevent the same memory addresses from being used in both lists.
 *
 * @param purchase_list Pointer to the list of comics intended for purchase.
 * @param store_list Pointer to the store list of comics.
 * @param index The index of the comic in the store list to add to the purchase list.
 */
void buy_comic(Comic_List* purchase_list, Comic_List* store_list, int index) {
    if (index < 0 || index >= store_list->count) {
        printf("Invalid index: %d\n", index);
        return;
    }
    if (purchase_list->count >= purchase_list->size) {
        purchase_list->size *= 2;
        purchase_list->list = realloc(purchase_list->list, purchase_list->size * sizeof(Comic));
    }
    Comic comic;
    copy_comic(&comic, &store_list->list[index]);
    add_comic(purchase_list, comic);
}

/**
 * This function converts a string representation of cost into a double
 * it is a helper function 
 *
 * @param cost Pointer to the string representing the comic cost.
 * @return The numeric value of the cost as a double.
 */
double parse_cost(const char* cost) {
    while (*cost && !isdigit(*cost) && *cost != '+' && *cost != '-') {
        cost++;
    }
    return atof(cost);  // Converts and returns the numeric part of the cost string.
}

/**
 * This function processes the checkout for a list of comics
 * it calculates the subtotal of all comics and also clears 
 * the purchase list after processing.
 *
 * @param purchase_list Pointer to the list of comics to be checked out.
 * @param out FILE pointer for writing the checkout summary to an output stream.
 */
void checkout(Comic_List* purchase_list, FILE* out) {
    fprintf(out, "Comics in Purchase List\n");
    double subtotal = 0.0;
    for (int i = 0; i < purchase_list->count; i++) {
        Comic* c = &purchase_list->list[i];
        fprintf(out, "Comic Number: %d\n\tDate: %s\n\tCode: %s\n\tPublisher: %s\n\tTitle: %s\n\tCost: %s\n",
                i + 1, c->date, c->code, c->publisher, c->title, c->cost);
        
        if (strncmp(c->cost, "AR", 2) != 0) {  // Skipping the "AR" costs
            double costValue = parse_cost(c->cost);
            subtotal += costValue;
        }
    }
    double tax = subtotal * 0.05;  // Calculating the tax at a rate of 5%
    double total = subtotal + tax;  // Total cost including tax
    fprintf(out, "Subtotal: $%.2f\nTax: $%.2f\nTotal: $%.2f\n", subtotal, tax, total);

    // Clearing the purchase list to reset for future transactions
    free_list(purchase_list);
    init_list(purchase_list);
}

/**
 * This function finds and displays a comic at a specified 
 * index from a list of comics.
 *
 * @param list Pointer to the Comic_List containing the comics.
 * @param index The zero-based index of the comic to find in the list.
 * @param out FILE pointer to the output file where results should be written.
 */
void find_comic(const Comic_List* list, int index, FILE* out) {
    // Making sure it is within the bounds of the list
    if (index < 0 || index >= list->count) {
        fprintf(out, "There is no comic at index #%d in the list.\n", index);
    } else {
        // Retrieving the comic at the given index
        Comic comic = list->list[index];
        fprintf(out, "\tDate: %s\n\tCode: %s\n\tPublisher: %s\n\tTitle: %s\n\tCost: %s\n",
                comic.date, comic.code, comic.publisher, comic.title, comic.cost);
    }
}