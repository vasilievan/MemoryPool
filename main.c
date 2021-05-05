#include "memoryPool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ф-ция main с 3-мя аргументами: первый - к-во аргументов при запуске + имя исполняемой программы
 * входные аргументы программы: имя файла с данными для аллокации, а также размером региона и к-вом регионов в странице
 * */

int main(int argc, char *argv[]) {
    // проверка на корректное к-во аргементов
    if( argc == 3 ) {
        printf("Performing the task.\n");
    } else if( argc > 3 ) {
        printf("Too many arguments supplied.\n");
        return -1;
    } else {
        printf("Not enough arguments.\n");
        return -1;
    }
    // файл с данными
    FILE *mf;
    // вывод после работы программы
    FILE *fp;
    // буфер для чтения строк из файла
    char str[60];
    // флаг для контроля ошибок или конца файла
    char *estr;
    // счётчик строк в файле
    int stringCounter = 0;
    // длина региона в Пуле памяти
    int regionLength;
    // к-во регионов в Пуле памяти
    int lastRegionIndex;
    // пул памяти
    MemoryPool memoryPool;
    // открываю файлы в разных режимах, для чтения и записи
    mf = fopen (argv[1], "r");
    fp = fopen(argv[2], "w+");
    // если вышла ошибка чтения, нет прав доступа к файлу или он не найден
    if (mf == NULL) {
        printf("IO error.");
        return -1;
    }
    // иначе построчно читаю файл
    while (1) {
        // проверяю, есть ли следующая строка
        estr = fgets(str, sizeof(str), mf);
        // если нет, то это конец файла или ошибка при чтении
        if (estr == NULL) {
            if ( feof (mf) != 0) {
                break;
            } else {
                printf ("Error.");
                break;
            }
        }
        // инкремент счетчика строк
        stringCounter += 1;
        // если это первая строка, то это размер региона
        if (stringCounter == 1) {
            regionLength = strtol(str, NULL, 0);
            // если это вторая строка, то это к-во регионов
        } else if (stringCounter == 2) {
            // если это вторая строка, то это к-во регионов
            lastRegionIndex = strtol(str, NULL, 0);
            // создаю Пул Памяти
            memoryPool = createMemoryPool(regionLength, lastRegionIndex);
            // размечаю память
            markCells(&memoryPool);
        } else if ((stringCounter > 2) & (stringCounter % 2 == 0)) {
            // если номер строки четный, демонстрирую и аллокацию и деаллокацию
            // создаю указатель на символ начала строки
            char * pointer;
            // выделяю под него память
            pointer = (char *) allocate((int) strlen(str), &memoryPool);
            // переношу указатель строки в новый адрес
            strcpy(pointer, str);
            // вывожу логи в файл
            // содержимое по указателю
            fprintf(fp, "The string is: %s", pointer);
            // адрес указателя
            fprintf(fp,"It has the address: %p\n", pointer);
            // к-во памяти, занятой данными
            fprintf(fp,"It takes %llu bytes in memory\n", (long long unsigned int) (strlen(str)*sizeof(char)));
            // выводит содержимое состояния занятости данными ячеек памяти
            showMemoryPool(&memoryPool, fp);
            fprintf(fp,"Now I\'ll clear the cell of Memory Pool.\n");
            // освобождает ячейку из Пула Памяти по данному указателю
            deallocate(pointer, &memoryPool);
            fprintf(fp, "%s is deallocated.\n", pointer);
            // выводит содержимое состояния занятости данными ячеек памяти
            showMemoryPool(&memoryPool, fp);
        } else if ((stringCounter > 2) & (stringCounter % 2 != 0)) {
            // если номер строки нечетный, только аллокацию
            // все по аналогии с блоком выше
            char * pointer;
            pointer = (char *) allocate((int) strlen(str), &memoryPool);
            strcpy(pointer, str);
            fprintf(fp,"The string is: %s", pointer);
            fprintf(fp,"It has the address: %p\n", pointer);
            fprintf(fp,"It takes %llu bytes in memory\n", (long long unsigned int) strlen(str)*sizeof(char));
            showMemoryPool(&memoryPool, fp);
        }
    }
    // закрываю файл для чтения, освобождаю ресурсы
    fclose(mf);
    // записываю в логи содержимое состояния занятости данными ячеек памяти
    fprintf(fp,"Now I clear all.\n");
    // очищает ячейки, не освобождая память, для того, чтобы можно было записывать заново
    deallocateAllInMemoryPool(&memoryPool);
    // выводит содержимое состояния занятости данными ячеек памяти
    showMemoryPool(&memoryPool, fp);
    fprintf(fp,"Now I free resources.\n");
    // освобождает все используемые ресурсы памяти кучи программы
    freeMemoryPool(&memoryPool);
    // выводит содержимое состояния занятости данными ячеек памяти, демонстрация переиспользования файлов
    showMemoryPool(&memoryPool, fp);
    // закрываю файл для записи, освобождаю ресурсы
    fclose(fp);
    return 0;
}