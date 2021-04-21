#include <string>
#include <map>
using namespace std;

namespace ariel
{
    class NumberWithUnits
    {
    string unit;
    double value;

    public:
        NumberWithUnits(double, const string&);
        static void read_units(ifstream&);

        //Basic operators :
        NumberWithUnits operator+() const; //Unary +a
        NumberWithUnits operator+(const NumberWithUnits&) const; //Add a+b
        NumberWithUnits &operator+=(const NumberWithUnits&); //Add to this a+=b
        NumberWithUnits operator-() const; //Unary -a
        NumberWithUnits operator-(const NumberWithUnits&) const; // Subtract a-b
        NumberWithUnits &operator-=(const NumberWithUnits&); //Subtract from this a-=b

        //Boolean opearators:
        bool operator<(const NumberWithUnits&) const;
        bool operator<=(const NumberWithUnits&) const;
        bool operator>(const NumberWithUnits&) const;
        bool operator>=(const NumberWithUnits&) const;
        bool operator==(const NumberWithUnits&) const;
        bool operator!=(const NumberWithUnits&) const;

        NumberWithUnits &operator++();//prefix++
        NumberWithUnits operator++(int); //postfix++
        NumberWithUnits &operator--();//prefix--
        NumberWithUnits operator--(int); //postfix--

        //Multiplication by 'double':
        friend NumberWithUnits operator*(const double&, const NumberWithUnits&);
        NumberWithUnits operator*(const double&) const;
        
        //Input/Output :
        friend ostream &operator<<(ostream&, const NumberWithUnits&);
        friend istream &operator>>(istream&, NumberWithUnits&);
    };
}