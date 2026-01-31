# Лабораторная работа 4: Логгер температуры

Кроссплатформенная программа: чтение температуры → 3 лога с автоочисткой.

## Как работает

Устройство каждые 10 секунд отправляет температуру по USB/серийному порту.

Логгер автоматически:

- Raw данные (raw_temps.log) → 24 часа хранения.
- Средняя/час (hourly_avg.log) → 1 месяц хранения
- Средняя/день (daily_avg.log) → 1 год хранения.

Автоочистка по расписанию:

- raw_temps.log: удаляет >24ч
- hourly_avg.log: удаляет >1мес
- daily_avg.log: удаляет >1год

## Как пользоваться

### Запуск программы

```bash
Temperature Logger started
Raw log:    build/logs/raw_temps.log (24ч)
Hourly log: build/logs/hourly_avg.log (1мес)
Daily log:  build/logs/daily_avg.log (1год)
Simulating device... Press Ctrl+C to stop
```

### Команды

```bash
Ctrl+C              # Завершение программы
# Автосимуляция: 23°C ±2°C каждые 10с
```

### Где смотреть логи

```bash
build/logs/raw_temps.log
# [2026-02-01 10:00:00.123] 23.45°C
# [2026-02-01 10:00:10.456] 22.89°C
build/logs/hourly_avg.log
# [2026-02-01 10:00:00] 23.12°C (60 измерений)
build/logs/daily_avg.log
# [2026-02-01] 22.85°C (24 часа)
```

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
├── lab3/ # Lab 3: Multi-process timer
├── lab4/ # Lab 4: Temperature logger (current)  
├── lab5/ # Lab 5: HTTP server + DB (planned)  
├── lab6/ # Lab 6: GUI application (planned)  
└── lab7/ # Lab 7: Kiosk mode (planned)
