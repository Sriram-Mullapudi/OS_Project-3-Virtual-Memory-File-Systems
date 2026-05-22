#include "types.h"
#include "stat.h"
#include "user.h"

#define PAGES 6
#define PAGE_SIZE 4096

int main() {
    char *memory = sbrk(PAGES * PAGE_SIZE); // Allocate memory
    for (int i = 0; i < PAGES; i++) {
        memory[i * PAGE_SIZE] = 'A';
    }
    exit();
}
