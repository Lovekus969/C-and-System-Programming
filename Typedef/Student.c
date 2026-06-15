#include <stdio.h>
#include <string.h>

typedef struct {
    int age;
    char name[50];
} Student;

int main() {
    Student s1;

    // Assign values
    s1.age = 21;
    strcpy(s1.name, "Kush");

    // Display values
    printf("Student Name: %s\n", s1.name);
    printf("Student Age: %d\n", s1.age);

    return 0;
}
