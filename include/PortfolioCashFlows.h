#ifndef PORTFOLIO_CASH_FLOWS
#define PORTFOLIO_CASH_FLOWS

#include <mutex>
#include <condition_variable>

#include "Policy.h"
#include "TimeStepProjection.h"
#include "DecrementsProjection.h"
#include "CashFlowsProjection.h"
#include "CashFlowsProjectionByValuationYear.h"

class PortfolioCashFlows
{
public:
    // constructor
    PortfolioCashFlows() {};

    // getters
    std::vector<Policy> &getPolicyVector() { return _policyVector; };
    std::vector<TimeStepProjection> &getTimeStepProjectionVector() { return _timeStepProjectionVector; };
    std::vector<DecrementsProjection> &getDecrementsProjectionVector() { return _decrementsProjectionVector; };
    std::vector<CashFlowsProjection> &getCashFlowsProjectionVector() { return _cashFlowsProjectionVector; };
    std::vector<CashFlowsProjectionByValuationYear> &getCashFlowsProjectionByValuationYearVector() { return _cashFlowsProjectionByValuationYearVector; };
    std::map<int,std::vector<double>> &getPortfolioCashFlowsByValuationYear() {return _portfolioCashFlowsByValuationYear; };

    // print methods
    void printPortfolioCashFlows();

    // methods
    void run();


    void pushBackPolicy(Policy &&policy);
    void pushBackTimeStepProjection(TimeStepProjection &&timeStepProjection);
    void pushBackDecrementsProjection(DecrementsProjection &&decrementsProjection);
    void pushBackCashFlowsProjection (CashFlowsProjection &&cashFlowsProjection);
    void pushBackCashFlowsProjectionByValuationYear (CashFlowsProjectionByValuationYear &&cashFlowsProjectionByValuationYear);

    std::map<int,std::vector<double>> createPortfolioCashFlowsByValuationYear();

    //Policy popBack();
    

private:
    std::mutex _mutex;
    
    std::vector<Policy> _policyVector;
    std::vector<TimeStepProjection> _timeStepProjectionVector;
    std::vector<DecrementsProjection> _decrementsProjectionVector;
    std::vector<CashFlowsProjection> _cashFlowsProjectionVector;
    std::vector<CashFlowsProjectionByValuationYear> _cashFlowsProjectionByValuationYearVector;

    std::map<int,std::vector<double>> _portfolioCashFlowsByValuationYear;
};

#endif