#ifndef CASH_FLOWS_PROJECTION_H
#define CASH_FLOWS_PROJECTION_H

#include "DecrementsProjection.h"

/*
The purpose of this class is to project and store cashflows and expected cashflows.
*/

class CashFlowsProjection
{
public:
    // constructor
    CashFlowsProjection(DecrementsProjection &decrementsProjection);

    // methods
    void run();

    // print methods
    void printCashFlows();


    // getters
    DecrementsProjection &getDecrementsProjection() { return _decrementsProjection; };

    // inflows BOM
    const std::vector<double> &getPremium() { return _premium; };
    const std::vector<double> &getExpectedPremium() { return _expectedPremium; };

    // outflows EOM
    const std::vector<double> &getDeathOutgo() { return _deathOutgo; };
    const std::vector<double> &getExpectedDeathOutgo() { return _expectedDeathOutgo; };


private:
    // members
    DecrementsProjection &_decrementsProjection;

    // inflows BOM
    std::vector<double> _premium;
    std::vector<double> _expectedPremium;

    // outflows EOM
    std::vector<double> _deathOutgo;
    std::vector<double> _expectedDeathOutgo;

    // expected net cash-flows
    std::vector<double> _expectedNetCashFlowsBOM;
    std::vector<double> _expectedNetCashFlowsEOM;

protected:
    // methods

    // inflows BOM
    std::vector<double> projectPremium();
    std::vector<double> projectExpectedPremium();

    // outflows EOM
    std::vector<double> projectDeathOutgo();
    std::vector<double> projectExpectedDeathOutgo();

    // expected net cash-flows
    std::vector<double> projectExpectedNetCashFlowsBOM();
    std::vector<double> projectExpectedNetCashFlowsEOM();
};



#endif