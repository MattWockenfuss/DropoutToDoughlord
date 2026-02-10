#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    int getPlayerChoice(std::string indent, std::string prompt);
    double sanitizeDoubleInput(std::string indent, std::string prompt);
    std::string getCompoundString(std::string string1, std::string string2);
    std::string getLocationString(std::string city, std::string state);
    std::string getMoneyString(double money, bool inchart = false);
    std::string getALessMoneyString(double money);
    std::string get2DigitDecimal(double decimal);
    std::string getDigitDecimal(double decimal);
    std::string getYearString(int week);
    
    std::string getRandomName();
    std::string wrapText(const std::string& blurb, int width = 80);
    int getRandomInt(int min, int max);
    double getRandomProbability();
}


#endif