#ifndef LABA5_MEMORYPOOL_H
#define LABA5_MEMORYPOOL_H
#include "cell.h"
#include <stdio.h>

// Пул Памяти содержит следующие поля:
// regionLength - длина региона;
// lastRegionIndex - к-во регионов;
// memoryTotal - всего занято памяти в байтах
// markedMemory - указатель на первый адрес выделенной области
// cells - двумерный массив, указатель на указатели на ячейки с границами разметки и отметкой об использовании

typedef struct {
    int regionLength;
    int lastRegionIndex;
    int memoryTotal;
    void * markedMemory;
    Cell** cells;
} MemoryPool;

// возвращает полное к-во байт, необходимое для адресного п-ва Пула Памяти
int getTotalMemory(int lastRegionIndex, int regionLength);

// создает указатель на указатели на ячейки с границами разметки и отметкой об использовании
Cell** getCells(int lastRegionIndex, int regionLength);

// выделяет память из кучи для Пула Памяти, возвращает указатель на начало этого блока
void * getMarkedMemory(int memoryTotal);

// размечает ячейки по адресам памяти,
// для примера:
// если длина региона = 3,
// к-во регионов = 4:
//
// 1 1 1 (соответствующее к-во байт)
// 2 2 2
// 4 4 4
// 8 8 8
void markCells(MemoryPool* memoryPool);

// выделяет требуемое к-во байт из Пула, возвращает указатель на свободную ячейку требуемого размера
void * allocate(int bytesNeeded, MemoryPool* memoryPool);

// освобождает ячейку из Пула Памяти по данному указателю
void deallocate(void * pointer, MemoryPool* memoryPool);

// освобождает все используемые ресурсы памяти кучи программы
void freeMemoryPool(MemoryPool* memoryPool);

// создаёт структуру Пул Памяти
MemoryPool createMemoryPool (int regionLength, int lastRegionIndex);

// выводит содержимое состояния занятости данными ячеек памяти
void showMemoryPool(MemoryPool* memoryPool, FILE* fp);

// очищает ячейки, не освобождая память, для того, чтобы можно было записывать заново
void deallocateAllInMemoryPool(MemoryPool* memoryPool);

#endif //LABA5_MEMORYPOOL_H
