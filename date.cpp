#include "date.h"
#include <iostream>
#include <ctime>

Date::Date(int d, int m, int y) {
    if (d == -1 || m == -1 || y == -1) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        day = ltm->tm_mday;
        month = ltm->tm_mon + 1;
        year = 1900 + ltm->tm_year;
    } else {
        day = d;
        month = m;
        year = y;
    }
    normalize();
    calculateWeekday();
}

void Date::normalize() {
    // Нормализация месяцев
    while (month > 12) {
        month -= 12;
        year++;
    }
    while (month < 1) {
        month += 12;
        year--;
    }

    // Нормализация дней
    while (day > daysInMonth(month, year)) {
        day -= daysInMonth(month, year);
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    while (day < 1) {
        month--;
        if (month < 1) {
            month = 12;
            year--;
        }
        day += daysInMonth(month, year);
    }
}

void Date::calculateWeekday() {
    int m = month;
    int y = year;
    if (m < 3) {
        m += 12;
        y--;
    }
    int k = y % 100;
    int j = y / 100;
    weekday = (day + 13*(m+1)/5 + k + k/4 + j/4 + 5*j) % 7;
    weekday = (weekday + 6) % 7; // Преобразуем к формату 0-воскресенье
}

int Date::daysInMonth(int m, int y) const {
    if (m == 2) return isLeapYear(y) ? 29 : 28;
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
}

bool Date::isLeapYear(int y) const {
    return (y % 400 == 0) || (y % 100 != 0 && y % 4 == 0);
}

int Date::toDays() const {
    int m = month;
    int y = year;
    if (m < 3) {
        m += 12;
        y--;
    }
    return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + day - 306;
}

void Date::addDays(int days) {
    day += days;
    normalize();
    calculateWeekday();
}

void Date::addWeeks(int weeks) {
    addDays(7 * weeks);
}

void Date::addMonths(int months) {
    month += months;
    normalize();
    // Корректировка дня, если он выходит за пределы нового месяца
    if (day > daysInMonth(month, year)) {
        day = daysInMonth(month, year);
    }
    calculateWeekday();
}

void Date::addYears(int years) {
    year += years;
    // Корректировка для 29 февраля
    if (month == 2 && day == 29 && !isLeapYear(year)) {
        day = 28;
    }
    calculateWeekday();
}

void Date::subtractDays(int days) { addDays(-days); }
void Date::subtractWeeks(int weeks) { addWeeks(-weeks); }
void Date::subtractMonths(int months) { addMonths(-months); }
void Date::subtractYears(int years) { addYears(-years); }

Date Date::nextAlumniDay() const {
    Date feb1(1, 2, year);
    if (feb1 < *this) {
        feb1 = Date(1, 2, year + 1);
    }
    
    while (feb1.weekday != 6) { // 6 - суббота
        feb1.addDays(1);
    }
    return feb1;
}

// Арифметические операторы
Date Date::operator+(int days) const {
    Date result = *this;
    result.addDays(days);
    return result;
}

Date Date::operator-(int days) const {
    return *this + (-days);
}

int Date::operator-(const Date& other) const {
    return this->toDays() - other.toDays();
}

// Операторы сравнения
bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date& other) const { return !(*this > other); }
bool Date::operator>(const Date& other) const { return other < *this; }
bool Date::operator>=(const Date& other) const { return !(*this < other); }
bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}
bool Date::operator!=(const Date& other) const { return !(*this == other); }

// Ввод/вывод
std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << (date.day < 10 ? "0" : "") << date.day << "."
       << (date.month < 10 ? "0" : "") << date.month << "."
       << date.year;
    return os;
}

std::istream& operator>>(std::istream& is, Date& date) {
    char dot;
    is >> date.day >> dot >> date.month >> dot >> date.year;
    date.normalize();
    date.calculateWeekday();
    return is;
}

Date operator+(int days, const Date& date) {
    return date + days;
}