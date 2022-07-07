#include "Valuation.h"

// constructor
Valuation::Valuation(date dateOfValuation, RunType runType) : _dateOfValuation(dateOfValuation), _runType(runType) 
{
    std::cout << "Valuation object constructed" << std::endl;
};