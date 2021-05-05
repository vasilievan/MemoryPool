#include "memoryPool.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// возвращает полное к-во байт, необходимое для адресного п-ва Пула Памяти
int getTotalMemory(int lastRegionIndex, int regionLength) {
    int memoryTotal = 0;
    for (int i = 0; i <= lastRegionIndex; i++) {
        int bitsMove = 1 << i;
        memoryTotal += regionLength * bitsMove;
    }
    return memoryTotal;
}

// создаёт массив ячеек, под каждую динамически выделяется память
Cell** getCells(int lastRegionIndex, int regionLength) {
    Cell** cells = (Cell**) malloc(lastRegionIndex * sizeof(Cell*));

    if (cells ==  NULL) {
        printf("Memory wasn\'t allocated.");
        exit(-1);
    }

    for (int i = 0; i < lastRegionIndex; i++) {
        cells[i] = (Cell*) malloc(regionLength * sizeof(Cell));
        if (cells[i] ==  NULL) {
            printf("Memory wasn\'t allocated.");
            exit(-1);
        }
    }

    return cells;
}

// выделяет указатель область памяти для bytesNeeded из Пула памяти
void * allocate(int bytesNeeded, MemoryPool* memoryPool) {
    int regionNumber = ceil(log2((double) bytesNeeded));
    if (bytesNeeded <= (1 << memoryPool->lastRegionIndex)) {
        for (int i=regionNumber; i < memoryPool->lastRegionIndex; i++) {
            for (int j=0; j < memoryPool->regionLength; j++) {
                if (memoryPool->cells[i][j].inUse == 0) {
                    memoryPool->cells[i][j].inUse = 1;
                    return memoryPool->cells[i][j].startDataPointer;
                }
            }
        }
    }
    printf("No more memory.");
    exit(-1);
}

// освобождает область памяти для данного указателя
void deallocate(void * pointer, MemoryPool* memoryPool) {
    if ((long long) pointer > (long long) memoryPool->markedMemory + memoryPool->memoryTotal) {
        printf("Incorrect pointer.");
        exit(-1);
    }
    long long difference = (long long) pointer - (long long) memoryPool->markedMemory;
    int regionNumber = 0;
    int memoryTotal = 0;
    for (int i = 0; i <= memoryPool->lastRegionIndex; i++) {
        int bitsMove = 1 << i;
        memoryTotal += memoryPool->regionLength * bitsMove;
        if (memoryTotal >= difference) {
            regionNumber = i;
            break;
        }
    }
    for (int i=regionNumber; i < memoryPool->lastRegionIndex; i++) {
        for (int j=0; j < memoryPool->regionLength; j++) {
            if (memoryPool->cells[i][j].startDataPointer == pointer) {
                memoryPool->cells[i][j].inUse = 0;
            }
        }
    }
}

// размечает ячейки, добавляя в каждую указатель на начальную границу
void markCells(MemoryPool* memoryPool) {
    long long start = (long long) memoryPool->markedMemory;
    for (int i = 0; i < memoryPool->lastRegionIndex; i++) {
        int bitsMove = 1 << i;
        for (int j = 0; j < memoryPool->regionLength; j++) {
            long long beginning = start;
            start += bitsMove;
            Cell cell = { (void *) beginning, 0 };
            memoryPool->cells[i][j] = cell;
        }
    }
}

// запрашивает основной блок памяти для Пула памяти у системы, возвращает указатель на его начало
void * getMarkedMemory(int memoryTotal) {
    void * markedMemory = malloc( memoryTotal );
    if (markedMemory ==  NULL) {
        printf("Memory wasn\'t allocated.");
        exit(-1);
    }
    return markedMemory;
}

// делает ВСЕ ячейки Пула памяти свободными для записи
void deallocateAllInMemoryPool(MemoryPool* memoryPool) {
    for (int i=0; i < memoryPool->lastRegionIndex; i++) {
        for (int j=0; j < memoryPool->regionLength; j++) {
            memoryPool->cells[i][j].inUse = 0;
        }
    }
}

// осовобождает системные ресурсы процесса для указателя Пула памяти
void freeMemoryPool(MemoryPool* memoryPool) {
    free(memoryPool->markedMemory);
    for (int i=0; i < memoryPool->lastRegionIndex; i++) {
        free(memoryPool->cells[i]);
    }
    free(memoryPool->cells);
}

// создает Пул памяти
MemoryPool createMemoryPool (int regionLength, int lastRegionIndex) {
    int memoryTotal = getTotalMemory(lastRegionIndex, regionLength);
    void * markedMemory = getMarkedMemory(memoryTotal);
    Cell** cells = getCells(lastRegionIndex, regionLength);
    MemoryPool memoryPool = { regionLength, lastRegionIndex, memoryTotal, markedMemory, cells };
    return memoryPool;
}

// выводит содержимое состояния занятости данными ячеек памяти
void showMemoryPool(MemoryPool* memoryPool, FILE* fp) {
    for (int i=0; i < memoryPool->lastRegionIndex; i++) {
        for (int j=0; j < memoryPool->regionLength; j++) {
            fprintf(fp,"%d ", memoryPool->cells[i][j].inUse);
        }
        fprintf(fp,"\n");
    }
    fprintf(fp, "\n");
}