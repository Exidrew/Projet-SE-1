#include "headers/error.h"

char* errormsg[] = {
    "No error", // 0
    ROUGE("Imposible to fork process"), // 1
    ROUGE("Exec failed"), // 2
    ROUGE("Shared Memory key creation failed"), //3
    ROUGE("Shared Memory creation failed"), // 4
    ROUGE("Failed to attach pointer to shared memory"), //5
    ROUGE("Shared Memory detachment failed"), // 6
    ROUGE("Shared Memory deletion failed"), // 7
    ROUGE("Open directory failed"), // 8
    ROUGE("Close directory failed"), // 9
    ROUGE("Failed dynamic memory allocation"), //10
    ROUGE("Failed open file"), // 11
    ROUGE("Failed close file"), // 12
    ROUGE("Getting user's name failed"), // 13
    ROUGE("Pipe creation failed"), // 14
    ROUGE("Redirection failed"), // 15
    ROUGE("Sigempty set error"), // 16
    ROUGE("Sigaction error"), // 17
    ROUGE("Socket error"), // 18
    ROUGE("Binding error"), // 19
    ROUGE("Send error"), // 20
    ROUGE("Accept error"), // 21
    ROUGE("Listen error"), // 22
    ROUGE("Read error"), // 23
    ROUGE("Thread create error"), // 24
    ROUGE("Connection failed")
};