#include "date.h"
#include <iostream>

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    normalize();
    updateWeekday();
}

void Date::updateWeekday() {
    int m = month;
    int y = year;
    if (m < 3) {
        m += 12;
        y--;
    }
    int k = y % 100;
    int j = y / 100;
    weekday = (day + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    // Корректировка для соответствия стандарту (0 - воскресенье, 1 - понедельник и т.д.)
    weekday = (weekday + 6) % 7; // Сдвиг, чтобы 0 = понедельник, 6 = воскресенье
}

bool Date::isLeapYear(int y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) const {
    if (m == 2) {
        return isLeapYear(y) ? 29 : 28;
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        return 30;
    } else {
        return 31;
    }
}

void Date::normalize() {
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

void Date::addDays(int days) {
    day += days;
    normalize();
    updateWeekday();
}

void Date::addWeeks(int weeks) {
    addDays(weeks * 7);
}

void Date::addMonths(int months) {
    month += months;
    normalize();
    updateWeekday();
}

void Date::addYears(int years) {
    year += years;
    if (month == 2 && day == 29 && !isLeapYear(year)) {
        day = 28;
    }
    updateWeekday();
}

void Date::subtractDays(int days) {
    addDays(-days);
}

void Date::subtractWeeks(int weeks) {
    addWeeks(-weeks);
}

void Date::subtractMonths(int months) {
    addMonths(-months);
}

void Date::subtractYears(int years) {
    addYears(-years);
}

Date Date::findNextAlumniDay() const {
    Date alumniDay(1, 2, year); // 1 февраля текущего года
    if (year < 2023) { // Просто пример, можно настроить логику
        alumniDay = Date(1, 2, 2023);
    }

    // Ищем первую субботу февраля
    while (alumniDay.month == 2 && alumniDay.weekday != 6) {
        alumniDay.addDays(1);
    }

    // Если текущая дата уже прошла, берем следующий год
    if (*this > alumniDay) {
        alumniDay = Date(1, 2, year + 1);
        while (alumniDay.month == 2 && alumniDay.weekday != 6) {
            alumniDay.addDays(1);
        }
    }

    return alumniDay;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>(const Date& other) const {
    return !(*this <= other);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

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
    date.updateWeekday();
    return is;
}