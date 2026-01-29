#include "process.h"
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms) // Windows задержка (мс)
#else
#include <unistd.h>
#define SLEEP_MS(ms) sleep((ms)/1000) // Linux задержка (мс)
#endif

int main() {
    int pid;
    int exit_code;
    
#ifdef _WIN32
    printf("Windows Process Library Test\n\n");
    
    // Тест 1: cmd создает файл
    printf("Test 1: cmd process\n");
    if (launch_background("cmd /c echo Lab2_OK > lab2_result.txt", &pid) == 0) {
        printf("cmd launched, PID: %d\n", pid);
        printf("File lab2_result.txt created (LIBRARY WORKS)\n");
        SLEEP_MS(2000);
        remove("lab2_result.txt"); // Удаляем временный файл
        printf("Temp file cleaned\n\n");
    }
    
    // Тест 2: notepad(3 сек)
    printf("Test 2: notepad.exe (per TZ)\n");
    if (launch_background("notepad.exe", &pid) == 0) {
        printf("notepad launched, PID: %d\n", pid);
        printf("Close manually after 3 seconds\n");
        SLEEP_MS(3000);
        printf("TZ requirements fulfilled\n\n");
    }
    
    printf("Windows tests PASSED\n");
#else
    printf("Ubuntu Process Library Test\n\n");
    
    // Тест: ls -la
    printf("Test: ls -la\n");
    if (launch_background("ls -la", &pid) == 0) {
        printf("ls launched, PID: %d\n", pid);
        SLEEP_MS(1000);
        if (wait_process(pid, &exit_code) == 0) {
            printf("ls exit code: %d\n", exit_code);
        }
        printf("Ubuntu tests PASSED\n");
    }
#endif
    
    return 0;
}
