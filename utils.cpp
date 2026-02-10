#include "utils.h"

#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>

using namespace std;

namespace Utils {
    static const string first_names[16] = {
        "John",
        "Larry",
        "Melissa",
        "Matt",
        "Mike",
        "Abby",
        "Elizabeth",
        "Percy",
        "Mary",
        "Gary",
        "James",
        "Patricia",
        "Susan",
        "Tom",
        "Ron",
        "Donald",
    };
    static const string last_names[20] = {
        "Smith",
        "Williams",
        "Wockenfuss",
        "Brown",
        "Jones",
        "Anderson",
        "Garcia",
        "Miller",
        "Davis",
        "Jackson",
        "Lee",
        "Allen",
        "Scott",
        "Hill",
        "Adams",
        "Hall",
        "Mitchell",
        "Baker",
        "Edwards",
        "Cook",
    };
    int getPlayerChoice(string indent, string prompt){
        //   https://en.cppreference.com/w/cpp/io/basic_ios/clear
        int x;
        while(true){
            cout << prompt;
            if (cin >> x) return x; //so we can put a cin inside of an if statement, and return if its valid, otherwise continue.
            cout << endl << indent << "Invalid Input: Please enter an Integer\n";
            cin.clear();
            cin.ignore(10000, '\n');//this clears everything the user typed (upto 10k characters apparently) and starts at a new line.
        }
    }
    double sanitizeDoubleInput(string indent, string prompt){
        double x;
        while(true){
            cout << prompt;
            if (cin >> x) return x;
            cout << indent << "Invalid Input: Please enter a Double\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    string getCompoundString(string s1, string s2){
        //i should make this take in an arraylist of arguments, or atleast more than two, but im lazy and dumb
        std::ostringstream ss;
        ss << s1 << s2;
        return ss.str();
    }
    string getLocationString(string city, string state){
        std::ostringstream ss;
        ss << city << ", " << state;
        return ss.str();
    }
    string getMoneyString(double money, bool inchart){
        if(isnan(money)) return "-";

        string moneySign = "$";
        if(inchart) moneySign = "";

        
        std::ostringstream ss;
        ss << fixed << setprecision(2) << money;
        
        string s = ss.str(); //convert to a string so we can .find and insert

        if(money >= -1000 && money <= 1000) return moneySign + s;

        int pos = s.find('.');
        int insertPos = pos - 3;

        while(insertPos > 0){
            s.insert(insertPos, ",");
            insertPos -= 3;
        }


        return moneySign + s;
    }
    string getALessMoneyString(double money){
        //A less money string is used in accounting when displaying a negative number, its surrounded by Parenthesis
        if(isnan(money)) return "-";
        std::ostringstream ss;
        ss << "(" << fixed << setprecision(2) << money <<")";
        return ss.str();
    }
    string get2DigitDecimal(double decimal){
        std::ostringstream ss;
        ss << fixed << setprecision(2) << decimal;
        return ss.str();
    }
    string getDigitDecimal(double decimal){//maybe make this just one function with a default parameter? idk im lazy
        std::ostringstream ss;
        ss << fixed << setprecision(1) << decimal;
        return ss.str();
    }
    string getYearString(int week){
        //we accept an input week and calculate the number of years and weeks. WE START AT 1, NOT 0,
        //Week 0 Returns Jan week 0,
        //53 -> Returns Jan Week 1, Year 2
        int years = (week - 1) / 52 + 1;
        int weeks = (week - 1) % 52 + 1; //always returns a range from 1 -> 52

        struct month {
            string name;
            int firstWeek;
        };

        //hmm this is really cool actually
        const month months[] = {
            {"January", 1},
            {"Febuary", 5},
            {"March", 9},
            {"April", 13},
            {"May", 17},
            {"June", 22},
            {"July", 27},
            {"August", 32},
            {"September", 37},
            {"October", 41},
            {"November", 45},
            {"December", 49}
        };

        int indexOfHighest = 0;
        int count = sizeof(months) / sizeof(months[0]);
        for(int i = 0; i < count; i++){
            if(months[i].firstWeek > weeks) break;
            if(months[i].firstWeek > months[indexOfHighest].firstWeek) indexOfHighest = i;
        }
        string r = months[indexOfHighest].name + " Week " + to_string(weeks - months[indexOfHighest].firstWeek + 1);
        if(years > 1) r += (", Year " + to_string(years));
        return r;
    }

    string wrapText(const string& blurb, int width){
        std::istringstream words(blurb); //converts the string to an input stream just like when we are reading from files
        std::string word;               
        std::string line = "       "; //we want the first line to start with an indent and be accounted for
        std::string result;

        // declare strings for a word, a line, and the result

        while (words >> word) {//loop through the entire string, and for every single word (because the >> operator by default uses a space as a delimiter, just like how we read from files)
            if (line.length() + word.length() + 1 > width) {
                //if this word would put us over the limit, add a new line to the string, and start the next line with this word
                result += line + "\n";
                line = "\t" + word;
            } else {
                //otherwise add this word to the line, if there is something here add a space between
                if (!line.empty()) line += " ";
                line += word;
            }
        }
        //now we add the last line if its not empty
        if (!line.empty())
            result += line;
        //this is a badass function
        return result;
    }

    string getRandomName(){
        int fname = getRandomInt(0, sizeof(first_names) / sizeof(first_names[0]) - 1);
        int lname = getRandomInt(0, sizeof(last_names) / sizeof(last_names[0]) - 1);

        return first_names[fname] + " " + last_names[lname];
    }
    int getRandomInt(int min, int max) {
        //this function returns a random int between min and max, INCLUSIVE
        //https://stackoverflow.com/questions/22923551/generating-number-0-1-using-mersenne-twister-c
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
    double getRandomProbability() {
        // Returns a random double between 0 and 1, inclusive
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(gen);
    }
}