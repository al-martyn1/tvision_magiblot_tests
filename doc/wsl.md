# Использование WSL для разработки

  - [VSCode расширения](#user-content-vscode-расширения)
    - [Для WSL кросс-платформенной разработки](#user-content-для-wsl-кросс-платформенной-разработки)
    - [Прочие полезности (записал тут по случаю, потом уберу куда релевантнее)](#user-content-прочие-полезности-записал-тут-по-случаю-потом-уберу-куда-релевантнее)
  - [Установка необходимых пакетов под WSL системой (Ubuntu/Debian like)](#user-content-установка-необходимых-пакетов-под-wsl-системой-ubuntudebian-like)
  - [Запуск сборки/отладки под WSL](#user-content-запуск-сборкиотладки-под-wsl)


Информация отсюда - https://learn.microsoft.com/ru-ru/windows/wsl/setup/environment

Конкретно по WSL - https://learn.microsoft.com/ru-ru/windows/wsl/tutorials/wsl-vscode


# VSCode расширения

## Для WSL кросс-платформенной разработки

  - Remote Development
  - WSL

## Прочие полезности (записал тут по случаю, потом уберу куда релевантнее)

  - Doxygen Documentation Generator
  - Markdown All in One
  - Markdown Preview Github Styling
  - Markdown Theme Kit
  - markdownlint
  - Peripheral Viewer (это для STM32ки)
  - Squirrel Language Supports
  - SVG
  - SVG Viewer

Затем надо пройтись по установленным расширениям, и везде нажать `Install in WSL`. Да, ещё в какой-то момент попросит установить VSCode сервисный стаф.


# Установка необходимых пакетов под WSL системой (Ubuntu/Debian like)

```
sudo apt update && sudo apt upgrade
sudo apt-get install wget ca-certificates
sudo apt-get install gcc
sudo apt-get install g++
sudo apt-get install cmake
sudo apt-get install git
sudo apt-get install man
```

Для TurboVision
```
sudo apt-get install libncurses-dev
sudo apt-get install libgmp-dev

# Варианты (не помню уже, работает или нет)
sudo apt-get install libncursesw6
sudo apt-get install libncursesw6-dev
```

Сборка TurboVision с `gpm` не работает, хз почему, пока делаем в CMakeLists.txt так:
```
set(TV_BUILD_USING_GPM OFF)
```

Надо посмотреть, что такое TVISION_NO_STL


# Запуск сборки/отладки под WSL

Когда всё правильно установлено, на панели слева появляется значек экрана с каким-то кружком справа внизу, выдаёт хинт: "Remote Explorer".




