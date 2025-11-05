#include "functions.h"

#include <stdbool.h>
#include <ctype.h>
#include <math.h>

// Проверка файла на валидность
ErrorCode isThatFileGood(const char *name)
{
    if (name == NULL) {
        return ERROR_INVALID_FILE_POINTER;
    }
    const char *fileTypes[] = {".txt", ".docx", "doc", "rtf", ".c", ".h", ".cpp", ".hpp", ".java", ".py", ".csv", ".json", ".xml", ".html", ".css", ".js", ".md", ".log", ".conf", ".config", NULL};

    char *splitter = strrchr(name, '.');

    if (splitter == NULL) {
        return ERROR_NO_FILE_TYPE;
    }

    for (int i = 0; fileTypes[i] != NULL; i++) {
        if (strcmp(splitter, fileTypes[i]) == 0) {
            return SUCCESS;
        }
    }

    return UNKNOWN_FILE_TYPE;
}

// СТУДЕНТ
typedef struct {
    unsigned int id;
    char* firstName;
    char* lastName;
    char* group;
    unsigned char* grades;
} Student;


// Проверка на условие латинницы
bool latinCheck(const char* str)
{
    for (int i = 0; str[i]!= '\0'; i++) {

        if (!isalpha(str[i])) {
            return 0;
        }
    }

    return 1;
}

// Средний баллъ
float avarage(const Student* stud)
{
    float sum = 0;

    for (int i = 0; i < 5; i++) {
        sum += stud->grades[i];
    }

    return sum / 5.0; 
}

// Очистка
void freeStudent(Student* stud)
{
    free(stud->firstName);
    free(stud->lastName);
    free(stud->group);
    free(stud->grades);
}

// Очистка всех
void freeAll(Student* studs, int count)
{
    for (int i = 0; i < count; i++) {
        freeStudent(&studs[i]);
    }

    free(studs);
}

// Вывод определённого студента
void fprintStudent(const Student* student, FILE* output, short int mode) {
    if (student == NULL || output == NULL) {
        return;
    }

    switch (mode) {
        case(0):
            fprintf(output, "%s %s\n",
                    student->firstName, student->lastName);
            break;

        case(1):
            fprintf(output, "%u: %s \t %s \t %s \t ",
                    student->id, student->firstName, student->lastName, student->group);
            for (int i = 0; i < 5; i++) {
                fprintf(output, "%hhu ", student->grades[i]);
            }
            fprintf(output, "\n");
            break;

        case(2):
            fprintf(output, "ID: %u\nFull name: %s %s\nGroup: %s\n Avarage: %hhu\n",
                    student->id, student->firstName, student->lastName, student->group, avarage(student));
            break;
    }
    
    return;
}

// Вывод всех студентов
void fprintfAll(Student* studs, int count, FILE* output)
{
    for (int i = 0; i < count; i++) {
        fprintStudent(&studs[i], output, 1);
    }
}

// Чтение студентов из файла
Student* readStudents(FILE* input, int* studCount, ErrorCode* error)
{
    if (input == NULL || studCount == NULL) {
        if (error) {*error = ERROR_INVALID_FILE_POINTER; }
        return NULL;
    }

    Student tempStud;

    unsigned int id;
    char firstName[1024], lastName[1024], group[512]; // 256
    unsigned char grades[5];

    *studCount = 0;
    while(fscanf(input,
                "%u %s %s %s %hhu %hhu %hhu %hhu %hhu",
                id, firstName, lastName, group,
                &grades[0], &grades[1], &grades[2], &grades[3], &grades[4]) == 9) {
        (*studCount)++;
    }

    if (*studCount == 0) {
        if (error) {*error = ERROR_INVALID_DATA; }

        return NULL;
    }

    Student* students = malloc(*studCount * sizeof(Student));
    if (!students) {
        if (error) {*error = ERROR_MEMORY_ALLOCATION; }

        return NULL;
    }

    rewind(input);

    for (int i = 0; i < *studCount; i++) {

        if (fscanf(input,
                "%u %s %s %s %hhu %hhu %hhu %hhu %hhu",
                id, firstName, lastName, group,
                &grades[0], &grades[1], &grades[2], &grades[3], &grades[4]) != 9) {

            for (int j = 0; j < i; j++) {
                freeStudent(&students[j]);
            }
            free(students);
            if (error) {*error = ERROR_INVALID_DATA; }

            return NULL;
        }

        if (!latinCheck(firstName) || !latinCheck(lastName)) {

            for (int j = 0; j < i; j++) {
                free(&students[j]);
            }
            free(students);
            if (error) {*error = ERROR_INVALID_DATA; }

            return NULL;
        }

        for (int j = 0; j < 5; j++) {
            if (!(grades[j] >= 0 && grades[j] < 100)) {

                for (int g = 0; g < i; g++) {
                    freeStudent(&students[g]);
                }
                free(students);
                if (error) {*error = ERROR_INVALID_DATA; }

                return NULL;
            }
        }

        students[i].id = id;
        students[i].firstName = malloc(strlen(firstName) + 1);
        students[i].lastName = malloc(strlen(lastName) + 1);
        students[i].group = malloc(strlen(group) + 1);
        students[i].grades = malloc(5 * sizeof(unsigned char));

        if (!students[i].firstName || !students[i].lastName || !students->group[i] || !students->grades[i]) {

            for (int j = 0; j <= i; j++) {
                freeStudent(&students[j]);
            }

            free(students);
            if (error) {*error = ERROR_MEMORY_ALLOCATION; }
            return NULL;
        }

        strcpy(students[i].firstName, firstName);
        strcpy(students[i].lastName, lastName);
        strcpy(students[i].group, group);
        memcpy(students[i].grades, grades, 5 * sizeof(unsigned char));   
    }

    if (error) {*error = SUCCESS; }

    return students;
}

// Поиски

Student* idSearch(Student* students, int count, unsigned int id, int* found)
{
    if (students == NULL || found == NULL) {
        return NULL;
    }

    *found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            (*found)++;
        }
    }

    if (*found == 0) {
        return NULL;
    }

    Student* hits = malloc(*found * sizeof(Student));
    if (!hits) {
        return NULL;
    }

    int outPos = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            hits[outPos++] = students[i];
        }
    }

    return hits;
}

Student* firstNameSearch(Student* students, int count, const char* firstName, int* found)
{
    if (students == NULL || found == NULL) {
        return NULL;
    }

    *found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].firstName, firstName) == 0) {
            (*found)++;
        }
    }

    if (*found == 0) {
        return NULL;
    }

    Student* hits = malloc(*found * sizeof(Student));
    if (!hits) {
        return NULL;
    }

    int outPos = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].firstName, firstName) == 0) {
            hits[outPos++] = students[i];
        }
    }

    return hits;
}

Student* lastNameSearch(Student* students, int count, const char* lastName, int* found)
{
    if (students == NULL || found == NULL) {
        return NULL;
    }

    *found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].lastName, lastName) == 0) {
            (*found)++;
        }
    }

    if (*found == 0) {
        return NULL;
    }

    Student* hits = malloc(*found * sizeof(Student));
    if (!hits) {
        return NULL;
    }

    int outPos = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].lastName, lastName) == 0) {
            hits[outPos++] = students[i];
        }
    }

    return hits;
}

Student* groupSearch(Student* students, int count, const char* group, int* found)
{
    if (students == NULL || found == NULL) {
        return NULL;
    }

    *found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            (*found)++;
        }
    }

    if (*found == 0) {
        return NULL;
    }

    Student* hits = malloc(*found * sizeof(Student));
    if (!hits) {
        return NULL;
    }

    int outPos = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            hits[outPos++] = students[i];
        }
    }

    return hits;
}

// Компараторы

bool idComparator(const Student* a, const Student* b) {
    return (a->id < b->id);
}

bool firstNameComparator(const Student* a, const Student* b) {
    return (a->firstName < b->firstName);
}

bool lastNameComparator(const Student* a, const Student* b) {
    return (a->lastName < b->lastName);
}

bool groupComparator(const Student* a, const Student* b) {
    return (a->group < b->group);
}

// Студенты, набравшие больше среднего

void goodSearch(Student* students, int count, FILE* output)
{
    if (students == NULL || count <= 0 || output == NULL) {
        return;
    }

    float allAvg = 0;
    for (int i = 0; i < count; i++) {
        allAvg += avarage(&students[i]);
    }
    allAvg /= count;

    for (int i = 0; i < count; i++) {
        
        if (avarage(&students[i]) > allAvg) {
            fprintStudent(&students[i], output, 0);
        }
    }
}

ErrorCode menu(FILE *output, Student* students, const short int studentCount, const int *choice) {
    if (choice == NULL) {
        return ERROR_INVALID_DATA;
    }

    switch (*choice) {
        case(1):
            unsigned int id;
            printf("Student ID: ");
            scanf("%u", &id);

            int found;
            Student* hits = idSearch(students, studentCount, id, &found);

            if (found) {
                printf("Found %d students:\n", found);

                for (int i = 0; i < found; i++) {
                    fprintStudent(&hits[i], stdout, 1);
                }

                free(hits);

            } else {
                printf("Students not found\n");
            }
            
            break;

        case(2):
            char* firstName;
            printf("Student first name: ");
            scanf("%s", &firstName);

            int found;
            Student* hits = firstNameSearch(students, studentCount, firstName, &found);

            if (found) {
                printf("Found %d students:\n", found);

                for (int i = 0; i < found; i++) {
                    fprintStudent(&hits[i], stdout, 1);
                }

                free(hits);
                
            } else {
                printf("Students not found\n");
            }
            
            break;

        case(3):
            char* lastName;
            printf("Student last name: ");
            scanf("%s", &lastName);

            int found;
            Student* hits = lastNameSearch(students, studentCount, lastName, &found);

            if (found) {
                printf("Found %d students:\n", found);

                for (int i = 0; i < found; i++) {
                    fprintStudent(&hits[i], stdout, 1);
                }

                free(hits);
                
            } else {
                printf("Students not found\n");
            }
            
            break;

        case(4):
            char* group;
            printf("Student group: ");
            scanf("%s", &group);

            int found;
            Student* hits = groupSearch(students, studentCount, group, &found);

            if (found) {
                printf("Found %d students:\n", found);

                for (int i = 0; i < found; i++) {
                    fprintStudent(&hits[i], stdout, 1);
                }

                free(hits);
                
            } else {
                printf("Students not found\n");
            }
            
            break;
        
        case(5):
            qsort(students, studentCount, sizeof(Student), idComparator);
            printf("Students sorted succesfully.\n");
            break;
           
        case(6):
            qsort(students, studentCount, sizeof(Student), firstNameComparator);
            printf("Students sorted succesfully.\n");
            break;
           
        case(7):
            qsort(students, studentCount, sizeof(Student), lastNameComparator);
            printf("Students sorted succesfully.\n");
            break;
           
        case(8):
            qsort(students, studentCount, sizeof(Student), groupComparator);
            printf("Students sorted succesfully.\n");
            break;
           
        
        case(9):
            unsigned int id;
            printf("Student ID: ");
            scanf("%u", &id);

            int found;
            Student* hits = idSearch(students, studentCount, id, &found);

            if (found >= 0) {
                fprintf(output, "Detailed student info:\n");
                fprintStudent(&hits[0], output, 2);
                printf("Student information written into the file.\n");

            } else {
                printf("Student not found.\n");
            }
            
            break;
        
        case(10):
            fprintf(output, "Good students:\n");
            goodSearch(students, studentCount, output);
            printf("Good students information written into the file.\n");

            break;
        
        case(11):
            fprintfAll(students, studentCount, stdout);
            break;
        
        case(12):
            fprintfAll(students, studentCount, output);
            break;
        
        case(0):
            freeAll(students, studentCount);
            return SUCCESS;

        default:
            freeAll(students, studentCount);
            return ERROR_INVALID_INPUT;
    }

    return ERROR_UNKNOWN;
}

void printMenu() {
    printf("\n <Student Manager> \n\n");

    printf("1. Find student by ID\n");
    printf("2. Find student by first name\n");
    printf("3. Find student by last name\n");
    printf("4. Find student by group\n\n");

    printf("5. Sort students by ID\n");
    printf("6. Sort students by first name\n");
    printf("7. Sort students by last name\n");
    printf("8. Sort students by group\n\n");

    printf("9. Student full information by ID (output file)\n");
    printf("10. Good students (output file)\n\n");

    printf("11. Display all students (console)\n");
    printf("12. Display all students (output file)\n\n");

    printf("0. Exit\n");
    printf("Your choice: ");
}