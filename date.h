#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
private:
    int day;
    int month;
    int year;
    int weekday; // 0 - воскресенье, 1 - понедельник, ..., 6 - суббота

    void updateWeekday(); // Обновляет день недели
    bool isLeapYear(int y) const; // Проверка на високосный год
    int daysInMonth(int m, int y) const; // Количество дней в месяце
    void normalize(); // Нормализует дату (исправляет переполнения)

public:
    Date(int d = 1, int m = 1, int y = 2000);
    
    // Методы получения данных
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    int getWeekday() const { return weekday; }
    
    // Методы изменения даты
    void addDays(int days);
    void addWeeks(int weeks);
    void addMonths(int months);
    void addYears(int years);
    
    void subtractDays(int days);
    void subtractWeeks(int weeks);
    void subtractMonths(int months);
    void subtractYears(int years);
    
    // Находит ближайшую субботу февраля (день встречи выпускников)
    Date findNextAlumniDay() const;
    
    // Перегрузка операторов
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend std::istream& operator>>(std::istream& is, Date& date);
};

#endif // DATE_H
