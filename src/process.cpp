#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#endif
#include "process.h"
#include <cstdio>

// Запуск процесса в фоне
int launch_background(const char* cmd, int* pid) {
#ifdef _WIN32
    // Windows: CreateProcessA
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {0};
    si.dwFlags = STARTF_USESHOWWINDOW; si.wShowWindow = SW_SHOW;
    
    if (!CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, FALSE, 
                       CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        return -1;  // Ошибка запуска
    }   
    *pid = (int)pi.dwProcessId;  // Сохраняем PID
    CloseHandle(pi.hThread); 
    CloseHandle(pi.hProcess);
    return 0;
#else
    // Ubuntu: fork + execl
    *pid = fork();
    if (*pid == -1) return -1;  // Ошибка fork
    if (*pid == 0) {  // Дочерний процесс
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        _exit(127);  // Ошибка execl
    }
    return 0;   
#endif
}

// Ожидание завершения процесса
int wait_process(int pid, int* exit_code) {
#ifdef _WIN32
    // Windows: OpenProcess + GetExitCodeProcess
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (h) {
        DWORD code; 
        if (GetExitCodeProcess(h, &code) && code != STILL_ACTIVE) {
            CloseHandle(h); 
            *exit_code = (int)code;
            return 0;
        }
        CloseHandle(h);
    }
    *exit_code = -1; 
    return -1;
#else
    // Ubuntu: waitpid
    int status; 
    if (waitpid(pid, &status, 0) != pid) return -1;
    if (WIFEXITED(status)) {
        *exit_code = WEXITSTATUS(status);
        return 0;
    }
    return -1;
#endif
}
