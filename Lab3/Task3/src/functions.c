#include "functions.h"
#include "linked_list.h"
#include <string.h>
#include <math.h>
#include <ctype.h>

// HELP-ФУНКЦИИ

ErrorCode isThatFileGood(const char *name)
{
    if (name == NULL) {
        return ERROR_INVALID_POINTER;
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

bool latinCheck(const char* str)
{
    if (str == NULL || *str == '\0') {
        return 0;
    }

    for (int i = 0; str[i]!= '\0'; i++) {

        if (!isalpha(str[i])) {
            return 0;
        }
    }

    return 1;
}

bool dateCheck(Date d) {
    if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > 31) {
        return 0;
    }

    if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) {
        
        if (d.day > 30) {
            return 0;
        }

    } else if (d.month == 2) {

        if ((d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0)) {
            if (d.day > 29) {
                return 0;
            }

        } else {
            if (d.day > 28) {
                return 0;
            }
        }
    }

    return 1;
}

int compareDates(Date d1, Date d2)
{
    if (d1.year != d2.year) {
        return d1.year - d2.year;
    }

    if (d1.month != d2.month) {
        return d1.month - d2.month;
    }

    return d1.day - d2.day;
}

int removeLiver(LinkedList *list, unsigned int id)
{
    Node *curr = list->head;
    size_t index = 0;

    while (curr) {

        if (curr->data.id == id) {
            delete_at_list(list, index);
            return 1;
        }

        curr = curr->next;
        index++;
    }

    return 0;
}

size_t simpleLiverFinder(LinkedList *list, unsigned int id)
{
    Node *curr = list->head;
    size_t index = 0;

    while (curr) {

        if (curr->data.id == id) {
            return index;
        }

        curr = curr->next;
        index++;
    }

    return list->size;
}

void searchMenu(const LinkedList *list)
{
    int answer;
    printf("\nv ПОИСК v \n");
    printf("1. По ID \n");
    printf("2. По имени \n");
    printf("3. По фамилии \n");
    printf("4. По отчеству \n");
    printf("5. По дате рождения \n");
    printf("6. По полу \n");
    printf("7. По доходу \n");
    printf("Ваш выбор: ");

    if (scanf("%d", &answer) != 1) {
        return;
    }

    while (getchar() != '\n');

    LinkedList results = create_list();
    char name[50];

    switch (answer) {
        case 1:
            unsigned int id;
            printf("Введите ID: ");
            if (scanf("%u", &id) == 1) {
                results = idSearch(list, id);
            }
            break;

        case 2:
            printf("Введите имя: ");
            if (scanf("%49s", name) == 1) {
                results = firstNameSearch(list, name);
            }
            break;

        case 3:
            printf("Введите фамилию: ");
            if (scanf("%49s", name) == 1) {
                results = secondNameSearch(list, name);
            }
            break;

        case 4:
            printf("Введите Отчество: ");
            if (scanf("%49s", name) == 1) {
                results = thirdNameSearch(list, name);
            }
            break;
        
        case 5:
            Date d;
            printf("Введите дату (D M Y): ");
            if (scanf("%d %d %d", &d.day, &d.month, &d.year) == 3) {
                results = dateSearch(list, d);
            }
            break;
        
        case 6:
            char gender_char;
            printf("Введите Пол (M/W): ");
            if (scanf(" %c", &gender_char) == 1) results = genderSearch(list, gender_char);
            break;
        
        case 7:
            double income;
            printf("Введите Доход: ");
            if (scanf("%lf", &income) == 1) results = incomeSearch(list, income);
            break;
        
        default:
            printf("Неверный ввод. \n");
            return;        
    }

    if (results.size > 0) {

        printf("Результаты поиска (найдено %zu человек): \n", results.size);

        Node *curr = results.head;

        while (curr) {
            printLiver(&(curr->data));
            curr = curr->next;
        }

    } else {
        printf("Жителей по заданному параметру не найдено. \n");
    }

    delete_list(&results);
}

// Основные функции

ErrorCode loadData(LinkedList *list, const char *fileName, History *history)
{
    if (list == NULL || fileName == NULL || history == NULL) {
        return ERROR_INVALID_POINTER;
    }

    FILE *input = fopen(fileName, "r");
    if (!input) {
        return ERROR_CANT_OPEN_FILE;
    }

    erase_list(list);
    makeHistory(history);

    Liver t;
    int readAmount = 0;
    ErrorCode result = SUCCESS;
    bool running = 1;

    while(running) {

        long currPos = ftell(input);
        if (currPos == (long)-1) {
            result = ERROR_INVALID_INPUT;
            break;
        }

        int argCounter = fscanf(input, "%u %49s %49s %49s %d %d %d %c %lf",
                                &t.id, t.secondName, t.name, t.thirdName, &t.birthDate.day, &t.birthDate.month, &t.birthDate.year, &t.gender, &t.income);
        
        if (argCounter == 9) {

            if (!latinCheck(t.secondName) || !latinCheck(t.name) || !latinCheck(t.thirdName)
                || !dateCheck(t.birthDate) || (t.gender != 'M' && t.gender != 'W')
                || t.income < 0.0) {

                result = ERROR_INVALID_INPUT;
                break;
            }

            insertLiver(list, t);
            readAmount++;
            continue;
        }

        if (feof(input)) {
            break;
        }

        fseek(input, currPos, SEEK_SET);

        argCounter = fscanf(input, "%u %49s %49s %d %d %d %c %lf",
                            &t.id, t.secondName, &t.name, &t.birthDate.day, &t.birthDate.month, &t.birthDate.year, &t.gender, &t.income);

        if (argCounter == 8) {
            t.thirdName[0] = '\0';

            if (!latinCheck(t.secondName) || !latinCheck(t.name)
                || !dateCheck(t.birthDate) || (t.gender != 'M' && t.gender != 'W')
                || t.income < 0.0) {

                result = ERROR_INVALID_INPUT;
                break;
            }

            insertLiver(list, t);
            readAmount++;
            continue;

        } else {

            result = ERROR_INVALID_INPUT;
            break;
        }
    }

    fclose(input);

    return result;
}

ErrorCode saveData(const LinkedList *list, const char *fileName)
{
    if (list == NULL || fileName == NULL) {
        return ERROR_INVALID_POINTER;
    }

    FILE *output = fopen(fileName, "w");
    if (!output) {
        return ERROR_CANT_OPEN_FILE;
    }

    Node *curr = list->head;

    while (curr) {
        Liver l = curr->data;

        fprintf(output, "%u %s %s %s %d %d %d %c %.2f\n",
            l.id, l.secondName, l.name, l.thirdName, l.birthDate.day, l.birthDate.month, l.birthDate.year, l.gender, l.income);
        
        curr = curr->next;
    }

    fclose(output);

    return SUCCESS;
}

void insertLiver(LinkedList *list, LIST_TYPE l)
{
    if (list == NULL) {
        return;
    }

    if (list->head == NULL) {
        push_back_list(list, l);
        return;
    }

    Node *curr = list->head;
    size_t index = 0;

    while (curr != NULL) {

        if (compareDates(l.birthDate, curr->data.birthDate) > 0) {
            insert_at_list(list, index, l);
            return;
        }

        curr = curr->next;
        index++;
    }

    push_back_list(list, l);
}

void printLiver(const Liver *l) {
    printf("ID: %u | Фамилия: %s | Имя: %s | Отчество: %s | ДР: %02d.%02d.%04d | Пол: %c | Доход: %.2f \n",
           l->id, l->secondName, l->name, l->thirdName, l->birthDate.day, l->birthDate.month, l->birthDate.year, l->gender, l->income);
}

LinkedList idSearch(const LinkedList *list, unsigned int id)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (curr->data.id == id) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

LinkedList firstNameSearch(const LinkedList *list, const char *name)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (strcmp(curr->data.name, name) == 0) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

LinkedList secondNameSearch(const LinkedList *list, const char *name)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (strcmp(curr->data.secondName, name) == 0) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

LinkedList thirdNameSearch(const LinkedList *list, const char *name)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (strcmp(curr->data.thirdName, name) == 0) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

LinkedList dateSearch(const LinkedList *list, Date date)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (compareDates(curr->data.birthDate, date) == 0) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

LinkedList genderSearch(const LinkedList *list, char gender)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (curr->data.gender == gender) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

LinkedList incomeSearch(const LinkedList *list, double income)
{
    LinkedList result = create_list();
    Node *curr = list->head;

    while (curr) {

        if (fabs(curr->data.income - income) < 1e-14) {
            push_back_list(&result, curr->data);
        }

        curr = curr->next;
    }

    return result;
}

void makeHistory(History *h)
{
    h->top = 0;
    h->amountOfData = 0;
}

void addAction(History *h, Action act)
{
    if (h->top < 999) {
        h->actions[h->top] = act;
        h->top++;
        h->amountOfData++;

    } else {
        printf("Предупреждение: стек истории переполнен. Новая операция не записана. \n");
    }

}

ErrorCode undo(LinkedList *list, History *h)
{
    if (h->amountOfData == 0) {
        return ERROR_EMPTY_LIST;
    }

    int numberOfActions = h->amountOfData / 2;
    if (numberOfActions == 0) {
        return ERROR_NOT_FOUND;
    }

    printf("\nОтмена: будет выполнено %d отмен. \n", numberOfActions);

    for (int i = 0; i < numberOfActions; i++) {
        h->top--;
        Action act = h->actions[h->top];
        
        switch (act.type) {
            case HISTORY_ADD:
                printf("Удалён новый житель с id: %u! \n", act.newData.id);
                removeLiver(list, act.newData.id);
                break;
                
            case HISTORY_DELETE:
                printf("Добавлен удалённый житель с id: %u! \n", act.oldData.id);
                insertLiver(list, act.oldData);
                break;

            case HISTORY_MODIFY:
                printf("Житель с id: %u возвращён в исходное состояние. \n", act.newData.id);
                removeLiver(list, act.newData.id);
                insertLiver(list, act.oldData);
                break;
        }
    }

    h->amountOfData = numberOfActions;

    return SUCCESS;
}

ErrorCode addLiver(LinkedList *list, History *history) {
    Liver l;
    char inputBuffer[512];

    char answer = ' ';
    printf("Отсутствует ли у жителя отчество? (y/n): ");

    if (scanf(" %c", &answer) != 1) {
        return ERROR_INVALID_INPUT;
    }

    while (getchar() != '\n');

    printf("Введите данные в ОДНУ строку: \n");
    
    if (answer == 'y') {
        printf("Введите данные нового жителя в формате \"ID Фамилия Имя ДД ММ ГГГГ [Дата рождения через пробелы] Пол(M/W) Доход\". \n");
        l.thirdName[0] = '\0';
    } else {
        printf("Введите данные нового жителя в формате \"ID Фамилия Имя Отчество День/Месяц/Год Рождения (через пробелы) Пол(M/W) Доход\". \n");
    }

    if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
        return ERROR_INVALID_INPUT;
    }

    int argCounter = 0;

    if (answer != 'y') {
        argCounter = sscanf(inputBuffer, "%u %49s %49s %49s %d %d %d %c %lf",
                            &l.id, l.secondName, l.name, l.thirdName, &l.birthDate.day, &l.birthDate.month, &l.birthDate.year, &l.gender, &l.income);
                    
        if (argCounter != 9) {
            return ERROR_INVALID_INPUT;
        }

    } else {
        argCounter = sscanf(inputBuffer, "%u %49s %49s %d %d %d %c %lf",
                            &l.id, l.secondName, l.name, &l.birthDate.day, &l.birthDate.month, &l.birthDate.year, &l.gender, &l.income);
        
        if (argCounter != 8) {
            return ERROR_INVALID_INPUT;
        }

        l.thirdName[0] = '\0';
    }


    if (!latinCheck(l.secondName) || !latinCheck(l.name)
        || !dateCheck(l.birthDate) || (l.gender != 'M' && l.gender != 'W')
        || l.income < 0.0) {

        return ERROR_INVALID_INPUT;
    }

    if (answer != 'y' && !latinCheck(l.thirdName)) {
        return ERROR_INVALID_INPUT;
    }

    Action act;
    act.type = HISTORY_ADD;
    act.newData = l;

    insertLiver(list, l);

    addAction(history, act);

    return SUCCESS;
}

ErrorCode deleteLiver(LinkedList *list, History *history)
{
    unsigned int id;
    printf("Введите ID жителя для удаления (Предупреждение: если в списке несколько жителей с одинаковым ID, то будет удалён самый младший житель): ");

    if (scanf("%u", &id) != 1) {
        return ERROR_INVALID_INPUT;
    }

    while(getchar() != '\n');

    size_t targetIndex = simpleLiverFinder(list, id);
    
    if (targetIndex >= list->size) {
        return ERROR_NOT_FOUND;
    }

    Liver saved = get_at_list(list, targetIndex);

    delete_at_list(list, targetIndex);

    Action act;
    act.type = HISTORY_DELETE;
    act.oldData = saved;

    addAction(history, act);

    return SUCCESS;
}

ErrorCode modifyLiver(LinkedList *list, History *history) {
    unsigned int id;
    printf("Введите ID жителя для изменения: ");

    if (scanf("%u", &id) != 1) {
        return ERROR_INVALID_INPUT;
    }

    while (getchar() != '\n');

    size_t targetIndex = simpleLiverFinder(list, id);

    if (targetIndex >= list->size) {
        return ERROR_NOT_FOUND;
    }

    Liver oldL = get_at_list(list, targetIndex);
    Liver newL = oldL;

    printf("Текущий доход жителя: %.2f. Введите новый доход: ", oldL.income);

    if (scanf("%lf", &newL.income) != 1) {
        while (getchar() != '\n');
        return ERROR_INVALID_INPUT;
    }

    while (getchar() != '\n');

    Action act;
    act.type = HISTORY_MODIFY;
    act.oldData = oldL;
    
    delete_at_list(list, targetIndex);
    insertLiver(list, newL);

    act.newData = newL;    
    addAction(history, act);

    return SUCCESS;
}

void menu(LinkedList *list, History *history)
{
    int answer;
    ErrorCode error;
    bool running = 1;

    while (running) {
        printf("\nv v v МЕНЮ v v v\n");
        printf("[Модификаций: %d] \n", history->amountOfData);
        printf("1. Список жителей \n");
        printf("2. Добавить жителя \n");
        printf("3. Удалить жителя \n");
        printf("4. Изменить жителя (Доход) \n");
        printf("5. Поиск жителя \n");
        printf("6. Сохранить в файл \n");
        printf("7. Отменить N/2 модификаций \n");
        printf("0. Выход \n");
        printf("Ваш выбор: ");

        if (scanf("%d", &answer) != 1) {
            printf("Неверный ввод. Попробуйте ввести ещё раз. \n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        switch (answer) {
            case 1:
                Node *curr = list->head;

                if (list->size == 0) {
                    printf("\nСписок пуст. \n");
                } else {
                    printf("\nv v Список жителей v v \n");
                    while (curr) {
                        printLiver(&(curr->data));
                        curr = curr->next;
                    }
                }
                break;

            case 2:
                error = addLiver(list, history);
                if (error == SUCCESS) {
                    printf("Житель добавлен успешно. \n");
                }
                else if (error == ERROR_INVALID_INPUT) {
                    printf("Ошибка: неверный ввод. \n");
                }
                break;

            case 3:
                error = deleteLiver(list, history);
                if (error == SUCCESS) {
                    printf("Житель удалён успешно. \n");
                }
                else if (error == ERROR_INVALID_INPUT) {
                    printf("Ошибка: неверный ввод. \n");
                }
                else if (error == ERROR_NOT_FOUND) {
                    printf("Ошибка: житель с указанным id не найден. \n");
                }
                break;
            
            case 4:
                error = modifyLiver(list, history);
                if (error == SUCCESS) {
                    printf("Доход жителя изменён успешно. \n");
                }
                else if (error == ERROR_INVALID_INPUT) {
                    printf("Ошибка: неверный ввод. \n");
                }
                else if (error == ERROR_NOT_FOUND) {
                    printf("Ошибка: житель с указанным id не найден. \n");
                }
                break;

            case 5:
                searchMenu(list);
                break;
            
            case 6:
                char outFileName[256];
                printf("Введите имя файла для сохранения списка: ");
                if (scanf("%s", outFileName) != 1) {
                    break;
                }

                error = saveData(list, outFileName);
                if (error == SUCCESS) {
                    printf("Жители сохранены в файл успешно. \n");
                }
                else if (error == ERROR_INVALID_INPUT) {
                    printf("Ошибка: не удалось записать жителей в файл. \n");
                }

                break;

            case 7:
                error = undo(list, history);
                if (error == SUCCESS) {
                    printf("Изменения отменены успешно. \n");
                }
                else if (error == ERROR_EMPTY_LIST) {
                    printf("Ошибка: нет модификаций для отмены. \n");
                }
                else if (error == ERROR_NOT_FOUND) {
                    printf("Ошибка: недостаточно модификаций для отмены. \n");
                }
                break;

            case 0:
                running = 0;
                printf("Работа с программой завершена успешно. \n");
                break;
            
            default:
                printf("Ошибка: неверный ввод команды. Попробуйте ещё раз. \n");
        }
    }
}