#ifndef CASH_FLOWS_PROJECTION_BY_VALUATION_YEAR
#define CASH_FLOWS_PROJECTION_BY_VALUATION_YEAR

#include "CashFlowsProjection.h"

/*
The purpose of this class is to group the monthly cashflows from the CashFlowsProjection Class
by valuationYear.
*/

class CashFlowsProjectionByValuationYear
{
public:
    // constructor
    CashFlowsProjectionByValuationYear(CashFlowsProjection &cashFlowsProjection);

    // destructor
    ~CashFlowsProjectionByValuationYear() { std::cout << "CashFlowsProjectionByValuationYear object with id = " << this->getCashFlowsProjection().getDecrementsProjection().getTimeStepProjection().getPolicy().getID() << " destructed" << std::endl; };

    // methods
    void run();

    // print methods
    //void printCashFlowsMultiMap();
    void printCashFlowsByValuationYear();

    // getters
    CashFlowsProjection &getCashFlowsProjection() { return _cashFlowsProjection; };
    //const std::multimap<int,std::vector<double>> &getCashFlowsMonthly() { return _cashFlowsMonthly; };
    const std::map<int,std::vector<double>> &getCashFlowsByValuationYear() { return _cashFlowsByValuationYear; };

    
private:
    CashFlowsProjection &_cashFlowsProjection;

    //const std::multimap<int,std::vector<double>> _cashFlowsMonthly;
    std::map<int,std::vector<double>> _cashFlowsByValuationYear;

protected:
    // methods
    std::multimap<int,std::vector<double>> createMultiMapFromCashFlowsProjectionObject();
    std::map<int,std::vector<double>> createCashFlowsByValuationYear();
};








#endif