#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 100
#define BUFFER_SIZE 1024

typedef struct {
    char dos_command[50];
    char linux_command[50];
} CommandMapping;

CommandMapping mappings[MAX_COMMANDS];
int mapping_count = 0;

// Load DOS-to-Linux command mappings from a file
void load_mappings(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening mapping file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%49[^=]=%49s\n", mappings[mapping_count].dos_command, mappings[mapping_count].linux_command) == 2) {
        mapping_count++;
    }

    fclose(file);
}

// Display all available DOS commands
void show_mappings() {
    printf("\nAvailable DOS commands:\n");
    for (int i = 0; i < mapping_count; i++) {
        printf("- %s\n", mappings[i].dos_command);
    }
    printf("\n");
}

// Translate DOS command to Linux equivalent
char* translate_command(const char *input) {
    for (int i = 0; i < mapping_count; i++) {
        if (strcmp(input, mappings[i].dos_command) == 0) {
            return mappings[i].linux_command;
        }
    }
    return NULL;
}

// Run user-entered command
void run_command() {
    char input_line[BUFFER_SIZE];
    char command[50], args[BUFFER_SIZE];
    args[0] = '\0';  // Clear argument buffer

    printf("Enter DOS command (with arguments if any): ");
    getchar();  // Clear leftover newline
    fgets(input_line, sizeof(input_line), stdin);

    // Remove newline
    input_line[strcspn(input_line, "\n")] = '\0';

    // Split command and arguments
    sscanf(input_line, "%s %[^\n]", command, args);

    char *linux_cmd = translate_command(command);
    if (linux_cmd) {
        char final_command[BUFFER_SIZE];
        if (strlen(args) > 0)
            snprintf(final_command, sizeof(final_command), "%s %s", linux_cmd, args);
        else
            snprintf(final_command, sizeof(final_command), "%s", linux_cmd);

        printf("Executing: %s\n\n", final_command);
        system(final_command);
    } else {
        printf("Unknown DOS command: '%s'\n", command);
    }
}

// Menu display
void display_menu() {
    printf("\n====== Shell Simulation Menu ======\n");
    printf("1. Run a DOS command\n");
    printf("2. Show available DOS commands\n");
    printf("3. Exit\n");
    printf("===================================\n");
}

// Manual page
void manual_page() {
    printf("\nManual: Shell Simulation (task4)\n");
    printf("----------------------------------\n");
    printf("This program simulates a shell that understands DOS commands.\n");
    printf("It reads mappings from a file and executes the Linux equivalent.\n");
    printf("\nHow to use:\n");
    printf(" - Run with: ./shell_sim\n");
    printf(" - View manual: ./shell_sim man\n");
    printf("\nMapping File Format (command_mapping.txt):\n");
    printf("  dir=ls\n  copy=cp\n  del=rm\n  etc...\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "man") == 0) {
        manual_page();
        return 0;
    }

    load_mappings("commandsList.txt");

    int choice;
    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                run_command();
                break;
            case 2:
                show_mappings();
                break;
            case 3:
                printf("Exiting Shell Simulation...\n");
                break;
            default:
                printf("Invalid choice. Please select 1-3.\n");
        }
    } while (choice != 3);

    return 0;
}
