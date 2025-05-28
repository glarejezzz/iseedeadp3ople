#include "date.h"
#include <iostream>

int main() {
    Date d1(28, 5, 2025);
    std::cout << "Текущая дата: " << d1 << std::endl;
    
    d1.addDays(5);
    std::cout << "После добавления 5 дней: " << d1 << std::endl;
    
    d1.subtractMonths(2);
    std::cout << "После вычитания 2 месяцев: " << d1 << std::endl;
    
    Date alumniDay = d1.findNextAlumniDay();
    std::cout << "Ближайший день встречи выпускников: " << alumniDay << std::endl;
    
    Date d2;
    std::cout << "Введите дату (ДД.ММ.ГГГГ): ";
    std::cin >> d2;
    
    if (d1 < d2) {
        std::cout << d1 << " раньше, чем " << d2 << std::endl;
    } else {
        std::cout << d2 << " раньше, чем " << d1 << std::endl;
    }
    
    return 0;
}