#include <stdio.h>
#include <stdlib.h>

int my_strlen(char str[]) {
    int len = 0;

    while (str[len] != '\0')
        len++;
    
    return len;
}

char* my_strcat(char str1[], char str2[]) {
    int total_size = my_strlen(str1) + my_strlen(str2) + 1;
    char* fstr = (char*)malloc(total_size*sizeof(char));

    for (int i = 0; i < my_strlen(str1); i++) 
        *(fstr + i) = str1[i];
    
    for (int i = 0; i < my_strlen(str2); i++) 
        *(fstr + i + my_strlen(str1)) = str2[i];
    
    *(fstr + total_size - 1) = '\0';

    return fstr;
}

int main() {
    char* new_string = my_strcat("Cesar", "Luis");
    printf("%s", new_string);
    free(new_string);
    return 0;
}