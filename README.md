# Лабораторная работа 3: Многопроцессный таймер

Кроссплатформенная программа: лог + счетчик + самовоспроизводство.

## Как работает

Счетчик растет (+1 каждые 300мс) во всех процессах одновременно.

Мастер (Master=YES):

- Каждую секунду пишет в лог: время+PID+счетчик
- Каждые 3 секунды запускает 2 копии:
  - Copy1: `counter += 10` → сразу завершается
  - Copy2: `counter *= 2` → ждет 2с → `counter /= 2` → завершается

Управление:
PID=XXXX, Counter=NNN  
Master=YES/NO  
Value (Enter=OK): → Вводите число или Enter

## Запуск

### Windows:

```bash
build.bat
```

### Linux:

```bash
chmod +x build.sh
./build.sh
```

## 📁 Repository Structure

OS_lab/  
├── main/ # Lab 1: Hello World + Git + CMake setup  
├── lab2/ # Lab 2: Background Process Library  
├── lab3/ # Lab 3: Multi-process timer (current)  
├── lab4/ # Lab 4: Temperature logger (planned)  
├── lab5/ # Lab 5: HTTP server + DB (planned)  
├── lab6/ # Lab 6: GUI application (planned)  
└── lab7/ # Lab 7: Kiosk mode (planned)
