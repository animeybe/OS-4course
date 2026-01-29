#ifndef PROCESS_H
#define PROCESS_H

#include <cstdint>

/**
 * Запускает программу в фоновом режиме
 * @param cmd команда для выполнения
 * @param pid дескриптор процесса (выходной параметр)
 * @return 0 при успехе, -1 при ошибке
 */
int launch_background(const char* cmd, int* pid);

/**
 * Ждет завершения процесса и возвращает код выхода
 * @param pid дескриптор процесса
 * @param exit_code код выхода (выходной параметр)
 * @return 0 при успехе, -1 при ошибке
 */
int wait_process(int pid, int* exit_code);

#endif // PROCESS_H
