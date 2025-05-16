#!/bin/bash

# Проверяем, передан ли аргумент (директория)
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

# Проверяем, существует ли директория
if [ ! -d "$1" ]; then
    echo "Error: Directory '$1' does not exist"
    exit 1
fi

# Меняем права для директорий на 755
find "$1" -type d -exec chmod 755 {} +

# Меняем права для файлов на 644
find "$1" -type f -exec chmod 644 {} +

echo "Permissions have been fixed recursively in '$1'"
