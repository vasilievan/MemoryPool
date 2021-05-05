#ifndef LABA5_CELL_H
#define LABA5_CELL_H

// структура ячейка содержит следующие поля:
// startDataPointer - указатель на начало памяти, отведенного под эту ячейку;
// inUse - состояние занятности.
typedef struct {
    void * startDataPointer;
    int inUse;
} Cell;

#endif //LABA5_CELL_H
