#include "date.h"
#include <iostream>

int main() {
    // Current date
    Date today;
    std::cout << "Today: " << today << std::endl;
    std::cout << "Day of week: " << today.getWeekday() 
              << " (0-Sunday, 6-Saturday)" << std::endl;

    // Date operations demonstration
    std::cout << "\nDate operations:\n";
    std::cout << "Today + 10 days: " << (today + 10) << std::endl;
    std::cout << "Today - 30 days: " << (today - 35) << std::endl;

    // Adding months and years
    std::cout << "\nAdding months/years:\n";
    std::cout << "Next month: " << (today + 30) << std::endl; // Approximate
    std::cout << "Next year: " << Date(today.getDay(), today.getMonth(), today.getYear() + 1) << std::endl;

    // Alumni meeting day
    std::cout << "\nSpecial dates:\n";
    std::cout << "Next alumni meeting day: " << today.nextAlumniDay() << std::endl; 

    Date date1;
    std::cout << "Enter the date in the format DD.MM.YYYY: ";
    std::cin >> date1;
    std::cout << "You have entered: " << date1 << std::endl;

    Date date2;
    std::cout << "Enter the date in the format DD.MM.YYYY:: ";
    std::cin >> date2;
    std::cout << "You have entered: " << date2 << std::endl;

    if (date1 < date2) {
        std::cout << date1 << " earlier than " << date2 << std::endl;
    } else {
        std::cout << date2 << " earlier than" << date1 << std::endl;
    }

    return 0;
}