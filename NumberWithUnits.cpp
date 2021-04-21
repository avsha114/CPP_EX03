#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

namespace ariel
{
    static map<string, map<string, double>> _units; //All units and conversion values

    NumberWithUnits::NumberWithUnits(double value, const string& unit)
    {
        if (_units.count(unit) == 0) //Check if unit exist
        {
            throw invalid_argument("There's no unit called " + unit + " !");
        }

        this->value = value;
        this->unit = unit;
    }

    void matchUnits(const string &firstUnit, const string &secondUnit)
    {
        //Go through every pair<string,double> in the second unit's list,
        // and then add the first unit to each of that unit's lists.
        for (const auto &pair1 : _units[firstUnit])
        {
            for (const auto &pair2 : _units[secondUnit])
            {
                //The formula for example for km->mm:
                //km->m * m->cm * cm->mm
                double convertRatio = _units[pair1.first][firstUnit] * _units[firstUnit][secondUnit] 
                                        * _units[secondUnit][pair2.first];
                _units[pair1.first][pair2.first] = convertRatio;
                _units[pair2.first][pair1.first] = 1 / convertRatio;
            }
        }
    }

    void NumberWithUnits::read_units(ifstream &units_file)
    {
        string first_unit;
        string second_unit;
        string temp; //temp is used to discard '='
        double first_value = 0;
        double second_value = 0;

        while (units_file >> first_value >> first_unit >> temp >> second_value >> second_unit)
        {
            //Insert new units to units list :
            _units[first_unit][second_unit] = second_value;
            _units[first_unit][first_unit] = 1;
            _units[second_unit][first_unit] = 1 / second_value;
            _units[second_unit][second_unit] = 1;

            //Now we will merge the conversion lists between both units.
            matchUnits(first_unit, second_unit);
        }
    }

    double convertNumber(const string &src, const string &dst, const double &value)
    {
        if (_units.at(src).count(dst) == 0) //If no conversion ratio found between the units
        {
            throw invalid_argument("Units do not match - [" + src + "] cannot be converted to [" + dst + "]");
        }

        if (src == dst)
        {
            return value;
        }

        return value * _units[src][dst];
    }

    //Unary +
    NumberWithUnits NumberWithUnits::operator+() const
    {
        return NumberWithUnits{this->value, this->unit};
    }

    //Calculate num1+num2
    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);

        //return a new number with mofiications
        return NumberWithUnits(this->value + converted, this->unit);
    }

    //Add num2 to num1
    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &num2)
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        this->value += converted;

        //return num1 after the modification
        return *this;
    }

    //Unary -
    NumberWithUnits NumberWithUnits::operator-() const
    {
        return NumberWithUnits(-this->value, this->unit);
    }

    //Calculate num1-num2
    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return NumberWithUnits(this->value - converted, this->unit);
    }

    //Subtract num2 from num1
    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &num2)
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        this->value -= converted;

        //return num1 after the modification
        return *this;
    }

    /* ####### Boolean operators: ####### */

    bool NumberWithUnits::operator<(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return this->value < converted;
    }

    bool NumberWithUnits::operator<=(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return this->value <= converted;
    }

    bool NumberWithUnits::operator>(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return this->value > converted;
    }

    bool NumberWithUnits::operator>=(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return this->value >= converted;
    }

    bool NumberWithUnits::operator==(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return this->value == converted;
    }

    bool NumberWithUnits::operator!=(const NumberWithUnits &num2) const
    {
        //Convert the value to the desire value in num1.unit
        double converted = convertNumber(num2.unit, this->unit, num2.value);
        return this->value != converted;
    }

    //Prefix ++
    NumberWithUnits &NumberWithUnits::operator++()
    {
        //Incerement num value and then return num with it's new value
        this->value++;
        return *this;
    }

    //Postfix ++
    NumberWithUnits NumberWithUnits::operator++(int)
    {
        //The old num value is copied to a new variable, then we increment num'
        // and return the temp value from before.
        NumberWithUnits temp = *this;
        this->value++;
        return temp;
    }

    //Prefix --
    NumberWithUnits &NumberWithUnits::operator--()
    {
        //Incerement num value and then return num with it's new value
        this->value--;
        return *this;
    }

    //Postfix --
    NumberWithUnits NumberWithUnits::operator--(int)
    {
        //The old num value is copied to a new variable, then we increment num'
        // and return the temp value from before.
        NumberWithUnits temp = *this;
        this->value--;
        return temp;
    }

    //Multiply double * NumberWithUnits
    NumberWithUnits operator*(const double &numDouble, const NumberWithUnits &num)
    {
        return NumberWithUnits{num.value * numDouble, num.unit};
    }

    //Multiply NumberWithUnits * double
    NumberWithUnits NumberWithUnits::operator*(const double &num2) const
    {
        return NumberWithUnits{this->value * num2, this->unit};
    }

    //Output "value[unit]"
    ostream &operator<<(ostream &out, const NumberWithUnits &num)
    {
        return out << num.value << "[" << num.unit << "]";
    }

    //input line to vaariables.
    istream &operator>>(istream &in, NumberWithUnits &num)
    {
        string temp; //to discard '[' sign
        double value = 0;
        string unit;
        in >> value >> temp >> unit;
        num = NumberWithUnits{value, unit};
        return in;
    }
}