#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <direct.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 100

char home_screen_header[] = "C Console Editor:\n\n    Create a file: 1\n    Open a file: 2\n    Exit: 3\n";

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char getUserOption(char message[]) {
    char option;

    printf("%s", message);
    scanf("%c", &option);
    clearInputBuffer();

    if (option != '1' && option != '2' && option != '3')
        return getUserOption("Your option is not valid. Select again: ");

    return option;
}

char *getFileName() {
    char *filename = (char *)malloc(MAX_FILENAME_LENGTH * sizeof(char));
    printf("%s", "Enter file name: ");
    scanf("%s", filename);
    return filename;
}

char *get_file_content(FILE *file) {
    char *buffer = (char *)malloc(1000 * sizeof(char));
    char temp[1000];

    if (buffer == NULL) {
        printf("%s", "Error al asignar memoria");
        return NULL;
    }

    buffer[0] = '\0';

    while (fgets(temp, 1000, file) != NULL) {
        strcat(buffer, temp);
    }

    return buffer;
}

void render(char header[], char content[]) {
    system("cls");
    printf("%s\n\n", header);
    printf("%s\n", content);
}

void editFile(char *filename) {
    FILE *found_file = fopen(filename, "r+");

    if (found_file == NULL) {
        printf("%s\n", "Error opening the file");
        return;
    }

    char *file_content = get_file_content(found_file);
    render(filename, file_content);

    char new_content[1000] = "";

    while (1) {
        char line[1000] = "";
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, ":w") == 0) {
            fprintf(found_file, "%s", new_content);
            break;
        }

        if (strcmp(line, ":q") == 0) {
            break;
        }

        if (strlen(new_content) > 0) 
            strcat(new_content, "\n"); 

        strcat(new_content, line);
    }

    free(file_content);
}

void createFile() {
    char *filename = getFileName();
    struct _stat stats;

    if (_stat("./files", &stats) == -1) {
        if (_mkdir("./files") == -1) {
            printf("%s", "There was an error creating the file");
            free(filename);
            return;
        }
    }

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "./files/%s", filename);

    FILE *found_file = fopen(filepath, "r");

    if (found_file != NULL) {
        printf("%s\n", "This file alredy exits");
        fclose(found_file);
        free(filename);
        return;
    }
    fclose(found_file);

    FILE *file = fopen(filepath, "w+");
    if (file == NULL) {
        printf("%s\n", "Error opening the file");
        free(filename);
        return;
    }

    printf("%s\n", "The file has been created");
    editFile(filepath);
    fclose(file);
    free(filename);
}

void logic() {
    render(home_screen_header, "");
    char option = getUserOption("Select option: ");

    switch (option) {
        case '1':
            createFile();
            break;
        case '2': {
            char* filename = getFileName();
            char filepath[256];
            snprintf(filepath, sizeof(filepath), "./files/%s", filename);
            editFile(filepath);
            free(filename);
            break;
        }
        case '3':
            system("exit");
            break;
    }
}

int main() {
    while(1) 
        logic();
    return 0;
}


// Problems:
// Problem writing the file. We have to close the program to write it.
// Render before messages.
// Option 3 doesnt exits???