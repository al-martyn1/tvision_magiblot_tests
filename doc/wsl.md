# Использование WSL для разработки

  - [Настройка Debian в WSL](#user-content-настройка-debian-в-wsl)
  - [VSCode расширения](#user-content-vscode-расширения)
    - [Для WSL кросс-платформенной разработки](#user-content-для-wsl-кросс-платформенной-разработки)
    - [Прочие полезности (записал тут по случаю, потом уберу куда релевантнее)](#user-content-прочие-полезности-записал-тут-по-случаю-потом-уберу-куда-релевантнее)
  - [Запуск сборки/отладки под WSL](#user-content-запуск-сборкиотладки-под-wsl)
  - [Установка необходимых пакетов под WSL системой (Ubuntu/Debian like)](#user-content-установка-необходимых-пакетов-под-wsl-системой-ubuntudebian-like)


Информация отсюда - https://learn.microsoft.com/ru-ru/windows/wsl/setup/environment

Конкретно по WSL - https://learn.microsoft.com/ru-ru/windows/wsl/tutorials/wsl-vscode


# Настройка Debian в WSL

Если прямой доступ к сети заблокирован корпоративными правилами, надо настроить прокси

```
sudo nano /etc/apt/apt.conf
```

Добавить
```
Acquire::http::Proxy "http://User:Password@Proxy.holding.com:3128";
```

# VSCode расширения

## Для WSL кросс-платформенной разработки

  - Remote Development
  - Remote - SSH
  - Remote - Tunnels
  - Dev Containers
  - WSL

## Прочие полезности (записал тут по случаю, потом уберу куда релевантнее)

  - Doxygen
  - Doxygen Documentation Generator
  - Doxygen Previewer
  - Markdown All in One
  - Markdown Preview Github Styling
  - Markdown Theme Kit
  - markdownlint
  - Peripheral Viewer (это для STM32ки)
  - Squirrel Language Supports
  - Squirrel Language Linter
  - SVG
  - SVG Viewer
  - Image Viewer

# Запуск сборки/отладки под WSL

Когда всё правильно установлено, на панели слева появляется значек экрана с каким-то кружком справа внизу, выдаёт хинт: "Remote Explorer".
Там можно подключится к удалённому хосту - `WSL Targets`, на нужной целевой машине


Затем надо пройтись по установленным расширениям, и везде нажать `Install in WSL`. Да, ещё в какой-то момент попросит установить VSCode сервисный стаф.


# Установка необходимых пакетов под WSL системой (Ubuntu/Debian like)

```
sudo apt update && sudo apt upgrade
sudo apt-get install wget ca-certificates
sudo apt-get install gcc g++ gdb cmake git man
```

Для TurboVision
```
sudo apt-get install libncurses-dev
sudo apt-get install libgmp-dev
```

Сборка TurboVision с `gpm` не работает, хз почему, пока делаем в CMakeLists.txt так:
```
set(TV_BUILD_USING_GPM OFF)
```

Надо посмотреть, что такое TVISION_NO_STL





