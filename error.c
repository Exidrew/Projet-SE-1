#include "headers/error.h"

char* errormsg[] = {
    "No error", // 0
    ROUGE("Imposible to fork process"), // 1
    ROUGE("Exec failed"), // 2
    ROUGE("Shared Memory key creation failed"), //3
    ROUGE("Shared Memory creation failed"), // 4
    ROUGE("Failed to attach pointer to shared memory"), //5
    ROUGE("Shared Memory detachment failed"), // 6
    ROUGE("Shared Memory deletion failed") // 7
};