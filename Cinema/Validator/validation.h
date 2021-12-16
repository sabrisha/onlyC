#ifndef VALIDATION_H
#define VALIDATION_H
#include <iomanip>
#include <string>
#include <regex>

using namespace std;

/*
    This class is an utility for better and easier validation
*/
class Validator
{
public:
    /*
        Checks if string is valid to "dd.mm.yyyy" and values
        are valid dd=[0-31] mm=[0-12] yyyy=[1970-(current year + 1)] 
    */
    static bool isDataValid(string str)
    {
        regex pattern("^[0-9]{2}\.[0-9]{2}\.[0-9]{4}$");
        if (regex_match(str, pattern))
        {
            char unused_c;
            int day, month, year, currentYear, unused_i;
            sscanf(str.c_str(), "%d\.%d\.%d", &day, &month, &year);
            sscanf(__DATE__, "%c%c%c %d %d", &unused_c, &unused_c, &unused_c, &unused_i, &currentYear);
            return month > 0 && month < 13
                && day > 0 && day < 32
                && year > 2019 && year < currentYear + 2;
        }
        return false;
    }
    /*
        Checks if string is valid to regex pattern
    */
    static inline bool useRegex(string str, string regex_pattern)
    {
        return regex_match(str, regex(regex_pattern));
    }
    /*
        Checks if str containes pattern as a substring
    */
    static bool isSubstring(string str, string pattern)
    {
        return str.find(pattern) == -1 ? false : true;
    }
    /*
        Checks if string is valid either to
        "[A-Z]([a-z]*) [A-Z]([a-z]*)", where [A-Z]([a-z]*) is english word starts with upper register
        or
        "[А-Я]([а-я]*) [А-Я]([а-я]*)", where [А-Я]([а-я]*) is russian word starts with upper register
    */
    static bool isNameValid(string str)
    {
        return regex_match(str, fioPattern_eng) || regex_match(str, fioPattern_rus);
    }
    /*
        Checks if string is valid to "hh:mm" format
        hh = [0-12]
        mm = [0-59]
    */
    static bool isTimeValid(string str)
    {
        regex timePattern("^[0-9]{2}\:[0-9]{2}$");
        if (regex_match(str, timePattern))
        {
            int min, hours;
            sscanf(str.c_str(), "%d\:%d", &hours, &min);
            if (hours >= 0 && hours <= 23
                && min >= 0 && min < 60)
                return true;
        }
        return false;
    }
};

//Шаблонный класс
template <class Template>
class Output {
    Template* Temp;
    int flag;
public:
    Output(Template* Temps, int flags) :Temp(Temps), flag(flags) {};
    Output get() { return Temp; };
    template <class Templates>
    friend ostream& operator <<(ostream& out, const Output<Templates>& temp);
};
//Шаблонная функция перегрузки оператора вывода в поток
template <class Template>
ostream& operator <<(ostream& out, const Output<Template>& temp) {
    int flag = temp.flag;
    out << "|" << setw(14) << temp.Temp[flag].flight_number << "|" << setw(16) << temp.Temp[flag].town_departure << "|" << setw(16) << temp.Temp[flag].town_arrival
        << "|" << setw(15) << temp.Temp[flag].date << "|" << setw(19) << temp.Temp[flag].cost_econom << "$|" << setw(19) << temp.Temp[flag].cost_business << "$|" << setw(15) << temp.Temp[flag].seats_amount << "|" << endl;
    out << "|______________________________________________________________";
    out << "________________________________________________________________|" << endl;
    return out;
}


#endif /*VALIDATION_H*/