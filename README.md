# Лабораторная работа 4: Логгер температуры

Кроссплатформенная программа: чтение температуры > 3 лога с автоочисткой.

## Как работает

Устройство каждые 10 секунд отправляет температуру по USB/серийному порту.

Логгер автоматически:

- Raw данные (`raw_temps.log`) > 24 часа хранения
- Средняя/час (`hourly_avg.log`) > 1 месяц хранения
- Средняя/день (`daily_avg.log`) > 1 год хранения

Автоочистка по расписанию:

- `raw_temps.log`: > 10MB > удалить
- `hourly_avg.log`: > 5MB > удалить
- `daily_avg.log`: > 1MB > удалить

## Как пользоваться

### Запуск программы

```bash
Temperature Logger started
Raw log:     logs/raw_temps.log (24h)
Hourly log:  logs/hourly_avg.log (1 month)
Daily log:   logs/daily_avg.log (1 year)
Simulating device... Press Ctrl+C to stop

[2026-02-01 00:00:10.123] 23.45 degrees (C)
[2026-02-01 00:00:20.456] 22.89 degrees (C)
```

### Команды

```bash
Ctrl+C              # Завершение программы
# Симуляция: 23C (+-2C) каждые 10с
```

### Где смотреть логи

```bash
logs/raw_temps.log:
# [2026-02-01 00:00:00.123] 23.45 C
# [2026-02-01 00:00:10.456] 22.89 C
logs/hourly_avg.log:
# 2026-02-01 00:00:00 23.12 C (360 readings)
logs/daily_avg.log:
# 2026-02-01 23.85 C (24 hours)
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
