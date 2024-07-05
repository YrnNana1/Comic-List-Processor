#include "comic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // Checking for correct command line arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char* command_file = argv[1];
    const char* output_file = argv[2];

    // Opening the output file for writing
    FILE* out = fopen(output_file, "w");
    if (!out) {
        perror("Failed to open output file");
        return 1;
    }

    // Opening the command file for reading
    FILE* cmd_file = fopen(command_file, "r");
    if (!cmd_file) {
        perror("Failed to open command file");
        fclose(out);  // closing the file
        return 1;
    }

    // Initializes comic lists for store and purchases
    Comic_List store_list, purchase_list;
    init_list(&store_list);
    init_list(&purchase_list);

    char command[256];
    while (fgets(command, sizeof(command), cmd_file) != NULL) {
        // Removing any newline character at the end of the command
        command[strcspn(command, "\n")] = '\0';

        // Skipping empty lines
        if (strlen(command) == 0) continue;

        fprintf(out, "Command: %s\n", command); // Log each command

        // Processing each command by checking the command type
        if (strncmp(command, "load ", 5) == 0) {
            load_comics(&store_list, command + 5, out);
        } else if (strncmp(command, "find ", 5) == 0) {
            int index = atoi(command + 5);
            find_comic(&store_list, index, out);
        } else if (strncmp(command, "buy ", 4) == 0) {
            int index = atoi(command + 4) - 1;
            if (index >= 0 && index < store_list.count) {
                buy_comic(&purchase_list, &store_list, index);
                fprintf(out, "Comic #%d added to purchase list\n", index + 1);
            } else {
                fprintf(out, "Invalid comic index: %d\n", index + 1);
            }
        } else if (strcmp(command, "checkout") == 0) {
            checkout(&purchase_list, out);
        } else if (strcmp(command, "display") == 0) {
            display_list(&store_list, out);
        } else if (strncmp(command, "save ", 5) == 0) {
            save_list(&store_list, command + 5);
        } else if (strcmp(command, "clear") == 0) {
            clear_list(&store_list);
        } else if (strncmp(command, "remove ", 7) == 0) {
            int index = atoi(command + 7);
            remove_comic(&store_list, index, out);
        }
    }

    // Cleaning up resources and closing the files
    free_list(&store_list);
    free_list(&purchase_list);
    fclose(out);
    fclose(cmd_file);
    return 0;
}