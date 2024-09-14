#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h> // Para mkdir en Windows
#else
#include <unistd.h> // Para Unix
#endif

char home_screen_header[] = "C Console Editor:\n\n    Create a file: 1\n    Open a file: 2\n    Exit: 3\n\n";

void clear_buffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void render(char header[], char content[])
{
    system("cls||clear"); // cls para Windows, clear para Unix
    printf("%s\n\n", header);
    printf("%s", content);
}

void print_message(char message[])
{
    printf("%s\n", message);
    printf("Press any letter to continue...");
    clear_buffer();
    getchar(); // Esperar cualquier tecla
}

char get_option()
{
    char option = '\0';
    printf("Enter an option: ");
    scanf("%c", &option);
    clear_buffer(); // Limpiar el buffer después de usar scanf

    if (option != '1' && option != '2' && option != '3')
    {
        print_message("This is not a correct option.");
        return '\0';
    }

    return option;
}

struct Filename
{
    int exists;
    char get[256];
};

struct Filename get_filename()
{
    struct Filename filename;
    char name[256];

    printf("Enter file name: ");
    scanf("%255s", name); // Limitar a 255 caracteres
    clear_buffer();

    snprintf(filename.get, sizeof(filename.get), "./files/%s", name);

    FILE *found_file = fopen(filename.get, "r");
    if (found_file != NULL)
    {
        filename.exists = 1;
        fclose(found_file);
    }
    else
    {
        filename.exists = 0;
    }

    return filename;
}

void editing_file(FILE *file)
{
    char file_content[1000] = "";

    while (1)
    {
        char line[1000] = "";
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0; // Remover salto de línea

        if (strcmp(line, ":w") == 0)
        {
            fprintf(file, "%s", file_content); // Guardar contenido en el archivo
            fflush(file);                      // Asegurarse de que se guarde
            break;
        }

        if (strcmp(line, ":q") == 0)
        {
            break;
        }

        strcat(file_content, line);
        strcat(file_content, "\n");
    }
}

char *get_file_content(FILE *file)
{
    char *file_content = (char *)malloc(1000 * sizeof(char));
    if (file_content == NULL)
    {
        perror("Error allocating memory");
        exit(1);
    }
    file_content[0] = '\0'; // Asegurarse de que esté vacío

    char temp[1000];
    while (fgets(temp, sizeof(temp), file) != NULL)
    {
        strcat(file_content, temp);
    }

    return file_content;
}

void open_file(struct Filename filename)
{
    if (filename.exists == 0)
        filename = get_filename();

    if (filename.exists == 0)
    {
        print_message("This filename doesn't exist.");
        return;
    }

    FILE *file = fopen(filename.get, "r+");
    if (file == NULL)
    {
        print_message("Error opening the file.");
        return;
    }

    char *file_content = get_file_content(file);
    render(filename.get, file_content);
    editing_file(file);

    fclose(file);
    free(file_content);
}

void create_file()
{
#ifdef _WIN32
    _mkdir("./files"); // Para Windows
#else
    mkdir("./files", 0755); // Para Unix
#endif

    struct Filename filename = get_filename();

    if (filename.exists == 1)
    {
        print_message("The file already exists.");
        return;
    }

    FILE *file = fopen(filename.get, "w");
    if (file == NULL)
    {
        print_message("Error creating the file.");
        return;
    }
    fclose(file);

    print_message("The file has been created.");

    filename.exists = 1;
    open_file(filename);
}

void program_iterator()
{
    char option = get_option();

    switch (option)
    {
    case '1':
    {
        create_file();
        break;
    }
    case '2':
    {
        struct Filename filename;
        filename.exists = 0;
        open_file(filename);
        break;
    }
    case '3':
    {
        exit(0); // Salir correctamente
        break;
    }
    default:
        break;
    }
}

int main()
{
    while (1)
    {
        render(home_screen_header, "");
        program_iterator();
    }

    return 0;
}
