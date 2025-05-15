#!/bin/bash


for i in {1..100}; do
    echo $((RANDOM % 1000)) >> numbers.txt
done

./sort_numbers numbers.txt > sorted_numbers.txt

sort -n numbers.txt > expected_sorted.txt

if cmp -s sorted_numbers.txt expected_sorted.txt; then
    echo "Числа отсортированы правильно!"
else
    echo "Ошибка: числа отсортированы неправильно!"
fi

rm numbers.txt sorted_numbers.txt expected_sorted.txt
