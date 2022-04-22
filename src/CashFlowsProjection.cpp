#include "CashFlowsProjection.h"

// constructor
CashFlowsProjection::CashFlowsProjection(DecrementsProjection &decrementsProjection) : _decrementsProjection(decrementsProjection) {}

// methods
void CashFlowsProjection::run()
{
    this->_premium = projectPremium();
    this->_expectedPremium = projectExpectedPremium();
    this->_deathOutgo = projectDeathOutgo();
    this->_expectedDeathOutgo = projectExpectedDeathOutgo();
    this->_expectedNetCashFlowsBOM = projectExpectedNetCashFlowsBOM();
    this->_expectedNetCashFlowsEOM = projectExpectedNetCashFlowsEOM();
}

// print methods
void CashFlowsProjection::printCashFlows()
{
    std::ofstream myFile("../results/cashFlowsProjection.csv");

    myFile 

    << "TIME_STEP" << "," 
    << "DATE_BOM" << ","
    << "PREMIUM_INDICATOR" << ","
    << "AGE_NEAREST" << "," 
    << "VALUATION_YEAR" << ","
    << "POLICY_YEAR" << "," 
    << "INDEPENDENT_DEATH_RATES" << "," 
    << "INDEPENDENT_LAPSE_RATES" << ","
    << "SURVIVING_POPULATION_BOM" << ","  
    << "DEPENDENT_DEATH_OUTGO" << "," 
    << "DEPENDENT_LAPSE_OUTGO" << "," 
    << "SURVIVING_POPULATION_EOM" << ","

    << "PREMIUM_BOM" << ","
    << "EXPECTED_PREMIUM_BOM" << ","
    << "DEATH_OUTGO_EOM" << ","
    << "EXPECTED_DEATH_OUTGO_EOM" << ","
    << "NET_CASH_FLOW_BOM" << ","
    << "NET_CASH_FLOW_EOM" << std::endl;

    for (int i = 0; i < this->getDecrementsProjection().getTimeStepProjection().getTimeStep().size(); i++)
    {
        myFile 
        << this->getDecrementsProjection().getTimeStepProjection().getTimeStep()[i] << ","
        << this->getDecrementsProjection().getTimeStepProjection().getDate()[i] << ","
        << this->getDecrementsProjection().getTimeStepProjection().getPremiumIndicator()[i] << ","
        << this->getDecrementsProjection().getTimeStepProjection().getAgeNearest()[i] << ","
        << this->getDecrementsProjection().getTimeStepProjection().getValuationYear()[i] << ","
        << this->getDecrementsProjection().getTimeStepProjection().getPolicyYear()[i] << ","
        << this->getDecrementsProjection().getIndependentLoadedMortalityRatesMonthly()[i] << ","
        << this->getDecrementsProjection().getIndependentLoadedLapseRatesMonthly()[i] << ","
        << this->getDecrementsProjection().getSurvivingPopulationBOM()[i] << ","
        << this->getDecrementsProjection().getDependentDeathOutgo()[i] << ","
        << this->getDecrementsProjection().getDependentLapseOutgo()[i] << ","
        << this->getDecrementsProjection().getSurvivingPopulationEOM()[i] << ","

        
        << this->getPremium()[i] << ","
        << this->getExpectedPremium()[i] << ","
        << this->getDeathOutgo()[i] << ","
        << this->getExpectedDeathOutgo()[i] << std::endl;
    }

    myFile.close();
}


// methods
// inflows BOM
std::vector<double> CashFlowsProjection::projectPremium()
{
    const std::vector<bool> &premiumIndicator = this->getDecrementsProjection().getTimeStepProjection().getPremiumIndicator();
    
    const double &premium = this->getDecrementsProjection().getTimeStepProjection().getPolicy().getPremium();

    std::vector<double> premiumInflow(premiumIndicator.size());

    std::transform
    (
        premiumIndicator.begin(),
        premiumIndicator.end(),
        premiumInflow.begin(),
        [&premium] (const bool &i) 
        {
            return i * premium;
        }
    );

    return premiumInflow;   
}

std::vector<double> CashFlowsProjection::projectExpectedPremium()
{
    // applicable decrement column
    const std::vector<double> &decrement = this->getDecrementsProjection().getSurvivingPopulationBOM();
    
    const std::vector<double> &premium = this->getPremium();

    std::vector<double> expectedPremium(decrement.size());

    std::transform
    (
        premium.begin(),
        premium.end(),
        decrement.begin(),
        expectedPremium.begin(),
        [] (const double &i, const double &j)
        {
            return i*j;
        }
    );

    return expectedPremium;
}


// outflows EOM

std::vector<double> CashFlowsProjection::projectDeathOutgo()
{
    const double &sumAssured = this->getDecrementsProjection().getTimeStepProjection().getPolicy().getSumAssured();

    std::vector<double> deathOutgo(this->getDecrementsProjection().getDependentDeathOutgo().size(), sumAssured);

    return deathOutgo;
}

std::vector<double> CashFlowsProjection::projectExpectedDeathOutgo()
{
    // applicable decrement column
    const std::vector<double> &decrement = this->getDecrementsProjection().getDependentDeathOutgo();

    const std::vector<double> &deathOutgo = this->getDeathOutgo();

    std::vector<double> expectedDeathOutgo(decrement.size());

    std::transform
    (
        deathOutgo.begin(),
        deathOutgo.end(),
        decrement.begin(),
        expectedDeathOutgo.begin(),
        [] (const double &i, const double &j)
        {
            return i*j;
        }
    );

    return expectedDeathOutgo;
}

// net cash-flows

std::vector<double> CashFlowsProjection::projectExpectedNetCashFlowsBOM()
{
    // addition of all inflows bom, less the addition of all outflows bom

    std::vector<double> netCashFlowBOM = this->getExpectedPremium();

    return netCashFlowBOM;
}

std::vector<double> CashFlowsProjection::projectExpectedNetCashFlowsEOM()
{
    // addition of all inflows eom, less the addition of all outflows eom

    std::vector<double> netCashFlowEOM = this->getExpectedDeathOutgo();

    return netCashFlowEOM;
}

