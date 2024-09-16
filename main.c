#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

char home_screen_header[] = "C Console Editor:\n\n    Create a file: 1\n    Open a file: 2\n    Exit: 3\n\n";

void clear_buffer() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void render(char header[], char content[]) {
	system("cls");
	printf("%s\n\n", header);
	printf("%s", content);
}

void print_message(char message[]) {
	printf("%s\n", message);
	printf(" Press any letter to continue...");
	clear_buffer();
	getchar();
}

char get_option() {
	char option = '\0';
	printf("%s", "Enter an option: ");
	scanf("%c", &option);
	clear_buffer();

	if (option != '1' && option != '2' && option != '3') {
		print_message("This is not a correct option.");
		return '\0';
	}

	return option;
}

struct Filename {
	int exists;
	char get[256];
};

struct Filename get_filename() {
	struct Filename filename;
	char name[256];

	printf("%s", "Enter file name: ");
	scanf("%255s", name);
	clear_buffer();

	snprintf(filename.get, sizeof(filename.get), "./files/%s", name);

	FILE *found_file = fopen(filename.get, "r");
	filename.exists = (found_file != NULL) ? 1 : 0;

	fclose(found_file);
	return filename;
}

void editing_file(FILE *file) {
	char file_content[1000] = "";

	while (1) {
		char line[1000] = "";
		fgets(line, sizeof(line), stdin);
		line[strcspn(line, "\n")] = 0;

		if (strcmp(line, ":w") == 0) {
			fprintf(file, "%s", file_content);
			fflush(file);
			break;
		}

		if (strcmp(line, ":q") == 0)
			break;

		strcat(file_content, line);
		strcat(file_content, "\n");
	}
}

char *get_file_content(FILE *file) {
	char *file_content = (char *)malloc(1000 * sizeof(char));
	file_content[0] = '\0';
	char temp[1000] = "";
	while (fgets(temp, sizeof(temp), file) != NULL) {
		strcat(file_content, temp);
	}

	return file_content;
}

void open_file(struct Filename filename) {
	// There is a file passed???
	if (filename.exists == 0)
		filename = get_filename();

	// Does the file exits??
	if (filename.exists == 0) {
		print_message("This filename doesnt exists.");
		return;
	}

	// Opening the file to edit
	FILE *file = fopen(filename.get, "r+");
	if (file == NULL) {
		print_message("Error opening the file.");
		return;
	}

	// Getting the file content
	char *file_content = get_file_content(file);

	char message[100] = "Editing ";
	snprintf(message, sizeof(message), "Editing: %s     :w => save and quit     :q => quit", filename.get);

	render(message, file_content);
	editing_file(file);
	fclose(file);
	free(file_content);
}

void create_file() {
	// Ver si la carpeta esta creada, sino, crearla
	mkdir("./files");

	// Ver si el arhivo existe
	struct Filename filename = get_filename();

	if (filename.exists == 1) {
		print_message("The file already exits.");
		return;
	}

	// Crear el archivo
	FILE *file = fopen(filename.get, "w");

	if (file == NULL) {
		print_message("Error creating the file.");
		return;
	}
	fclose(file);

	print_message("The file has been created.");

	// Abrir el archivo
	filename.exists = 1;
	open_file(filename);
}

void program_iterator() {
	char option = get_option();

	switch (option) {
	case '1': {
		render("Creating file...", "");
		create_file();
		break;
	}

	case '2': {
		render("Opening file...", "");
		struct Filename filename;
		filename.exists = 0;
		open_file(filename);
		break;
	}
		
	case '3': {
		exit(0);
		break;
	}

	default:
		break;
	}
}

int main() {
	while (1) {
		render(home_screen_header, "");
		program_iterator();
	}

	return 0;
}