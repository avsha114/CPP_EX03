#include "doctest.h"
#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace ariel;
using namespace std;

TEST_CASE("Reading from file"){

    ofstream file{"test_units.txt"};

    file << "1 km = 1000 m\n";
    file.close();
    
    ifstream units{"test_units.txt"};
    NumberWithUnits::read_units(units);
    
    CHECK_NOTHROW(NumberWithUnits a(3.5, "km"));
    CHECK_NOTHROW(NumberWithUnits b(1500, "m"));
    units.close();

    file.open("test_units.txt");
    file << "1m = 100 cm\n"
            "1 cm = 10mm\n"
            "1 hour = 60min\n";
    file.close();

    units.open("test_units.txt");
    NumberWithUnits::read_units(units);
    CHECK_NOTHROW(NumberWithUnits c(1000, "cm"));
    CHECK_NOTHROW(NumberWithUnits d(100000, "mm"));
    CHECK_NOTHROW(NumberWithUnits e(30, "min"));
    units.close();
}

TEST_CASE("Basic operators +Unary, + , -Unary, -, +=, -="){
    NumberWithUnits a{5, "km"};
    NumberWithUnits b{2000, "m"};
    NumberWithUnits c{5, "hour"};

    CHECK((+a) == NumberWithUnits{5, "km"});
    CHECK((-a) == NumberWithUnits{-5, "km"});
    CHECK((a+a) == NumberWithUnits{10, "km"});
    CHECK((a+b) == NumberWithUnits{7, "km"});
    CHECK((b+a) == NumberWithUnits{7000, "m"});
    CHECK((a+=b) == NumberWithUnits{7, "km"});
    CHECK(a == NumberWithUnits{7, "km"});
    CHECK((b-=a) == NumberWithUnits{-5000, "m"});
    CHECK(b == NumberWithUnits{-5000, "m"});
}

TEST_CASE("Increment and Decrement operators ++, --"){
    NumberWithUnits a{5, "km"};
    NumberWithUnits b{10, "m"};
    CHECK(a++ == NumberWithUnits{5, "km"});
    CHECK(a == NumberWithUnits{6, "km"});
    CHECK(++a == NumberWithUnits{7, "km"});
    CHECK(a == NumberWithUnits{7, "km"});
    CHECK(a-- == NumberWithUnits{7, "km"});
    CHECK(a == NumberWithUnits{6, "km"});
    CHECK(--a == NumberWithUnits{5, "km"});
    CHECK(a == NumberWithUnits{5, "km"});
}

TEST_CASE("Multiply with double opearaotr *"){
    NumberWithUnits a{5, "km"};

    CHECK((a*2) == NumberWithUnits{10, "km"});
    CHECK((0.2*a) == NumberWithUnits{1, "km"});
    CHECK(a == NumberWithUnits{5, "km"});
}

TEST_CASE("Boolean operators > , >= , < , <= , == , !="){

    NumberWithUnits a{5, "km"};
    NumberWithUnits b{2000, "m"};
    NumberWithUnits c{60, "min"};
    bool t = true;
    bool f = false;

    CHECK((a > b) == t);
    CHECK((a > NumberWithUnits{800000, "cm"}) == f);
    CHECK((a < b) == f);
    CHECK((a < NumberWithUnits{800000, "cm"}) == t);
    CHECK((a >= b) == t);
    CHECK((a >= NumberWithUnits{500000, "cm"}) == t);
    CHECK((a >= NumberWithUnits{600000, "cm"}) == f);
    CHECK((a <= b) == f);
    CHECK((a <= NumberWithUnits{500000, "cm"}) == t);
    CHECK((a <= NumberWithUnits{600000, "cm"}) == t);
    CHECK((a == b) == f);
    CHECK((a == NumberWithUnits{5000, "m"}) == t);
    CHECK((a != b) == t);
    CHECK((a != NumberWithUnits{5000, "m"}) == f);
}

TEST_CASE("Input/Output operator << , >>"){
    ostringstream output;
    istringstream input1{"50 [ m ]"};
    istringstream input2{"100 [ kg ]"};
    istringstream input3{"Blah"};
    NumberWithUnits a{5, "km"};

    CHECK_NOTHROW(output << a);
    CHECK_NOTHROW(input1 >> a);
    CHECK(output.str() == "5[km]");
    CHECK(a == NumberWithUnits{50, "m"});
}