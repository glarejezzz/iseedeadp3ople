#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
private:
    int day;
    int month;
    int year;
    int weekday; // 0 - воскресенье, 1 - понедельник, ..., 6 - суббота

    void normalize();
    void calculateWeekday();
    int daysInMonth(int m, int y) const;
    bool isLeapYear(int y) const;
    int toDays() const;

public:
    Date(int d = -1, int m = -1, int y = -1); // -1 означает текущую дату
    
    // Основные методы
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    int getWeekday() const { return weekday; }
    
    // Операции с датами
    void addDays(int days);
    void addWeeks(int weeks);
    void addMonths(int months);
    void addYears(int years);
    void subtractDays(int days);
    void subtractWeeks(int weeks);
    void subtractMonths(int months);
    void subtractYears(int years);
    
    // Специальные методы
    Date nextAlumniDay() const;
    
    // Арифметические операторы
    Date operator+(int days) const;
    Date operator-(int days) const;
    int operator-(const Date& other) const;
    
    // Операторы сравнения
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    
    // Ввод/вывод
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend std::istream& operator>>(std::istream& is, Date& date);
    friend Date operator+(int days, const Date& date);
};

#endif