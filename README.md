# 🏙️ QtMonopoly: Классическая Монополия на Qt5

[![Qt](https://img.shields.io/badge/Qt-5.15%2B-brightgreen)](https://www.qt.io/)

Почти полноценная реализация настольной игры "Монополия" с использованием Qt Widgets. Поддерживает 2-4 игроков, систему строительства домов/отелей, карты Шанс/Казна и полный экономический цикл.

![Скриншот интерфейса](resources/uncle.jpg)

## 🚀 Особенности
- **Динамическое игровое поле** с клетками 6 типов (собственность, ж/д, казна и др.)
- **Визуализация игроков** (4 цвета) и их перемещение по полю
- **Система строительства**:
  - Дома (до 4 на клетку)
  - Отели (замена 5 домов)
  - Иконки зданий из `resources/colour*.png`
- **Аудиосопровождение** (фоновая музыка `music.mp3`)
- **Диалоги выбора**:
  - Количество игроков через `playerdialog.h`
  - Управление аудио (громкость слайдером)
- **Полная экономическая модель**:
  - Покупка имущества
  - Расчет аренды
  - Налоги и штрафы
- **Анимация кубиков** (ресурсы `cube_*.png`)

## 📦 Структура проекта

- `main.cpp` - точка входа в программу.
- `mainwindow.cpp`, `mainwindow.h`, `mainwindow.ui` - главное меню и ее реализация.
- `gamewindow.cpp`, `gamewindow.h`, `gamewindow.ui` - реализация и логика игры.
- `cell.cpp`, `cell.h` - реализация клеток для игрового поля.
- `player.cpp`, `player.h` - реализация игроков.

## Установка и запуск

### Скачиваем репозиторий

```bash
git clone https://github.com/SahibAlMowt/Monopoly.git
```

### Создаем папку build и заходим в нее

```bash
mkdir build
```

```bash
cd build
```
### Создаем папку debug_crumble и заходим в нее

```bash
mkdir debug_crumble
```

```bash
cd debug_crumble
```

### Сборка файла при помощи qmake

```bash 
qmake -makefile
```
```bash 
qmake ../../monopoly.pro
```

```bash 
make
```
```bash
./monopoly
```

### Сборка для гениев 

#### Зайдите в Qt Creator, запускаете проект при помощи `.pro` файла и прожимайте комбинацию клавиш CTRL+R.

---

### И наслаждаетесь прекрасным с геймплеем с прекрасными товарищами.

## Авторы

AslanKarimov46 и я.
