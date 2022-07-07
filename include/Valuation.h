#ifndef VALUATION_H
#define VALUATION_H

#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

/*
The purpose of the Valuation Class is to hold the date of valuation 
(i.e. the time point from which cash-flows will be projected into the future),
and to indicate the run type - i.e. the cashflows are projected under the base scenario 
or the stress scenario (the mortality rates and lapse rates that are used would differ depending on base or stress).
*/


enum RunType
{
    Base,
    Stress,
};

class Valuation
{
public:
    // constructor
    Valuation(date dateOfValuation, RunType runType);

    ~Valuation() { std::cout << "Valuation object destructed" << std::endl; };

    // getters
    const date &getDateOfValuation() { return _dateOfValuation; };
    const RunType &getRunType() { return _runType; };

private:
    const date _dateOfValuation;
    const RunType _runType;    
};

#endif