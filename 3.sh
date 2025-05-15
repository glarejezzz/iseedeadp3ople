#!/bin/bash

if [ -z "$1" ]; then
    echo "Использование: $0 <директория>"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Указанная директория не существует."
    exit 1
fi

find "$1" -type d -exec chmod 755 {} \;   # Права 755 для директорий
find "$1" -type f -exec chmod 644 {} \;   # Права 644 для файлов

echo "Права доступа успешно изменены."
