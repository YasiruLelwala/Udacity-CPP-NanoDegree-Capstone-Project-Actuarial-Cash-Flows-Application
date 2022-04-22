#include "TimeStepProjection.h"

// constructor
TimeStepProjection::TimeStepProjection(Policy &policy) : _policy(policy) {};

// methods
void TimeStepProjection::run()
{
    this->_timeStep = projectTimeStep();
    this->_dateBOM = projectDate();
    this->_premiumIndicator = projectPremiumIndicator();
    this->_policyYear = projectPolicyYear();
    this->_valuationYear = projectValuationYear();
    this->_ageNearest = projectAgeNearest();

}

// print methods
void TimeStepProjection::printTimeStep()
{
    std::ofstream myFile("../results/timeStepProjection.csv");

    myFile 
    << "TIME_STEP" << "," 
    << "DATE_BOM" << ","
    << "PREMIUM_INDICATOR" << ","
    << "AGE_NEAREST" << "," 
    << "VALUATION_YEAR" << ","
    << "POLICY_YEAR" << "," << std::endl;

    for (int i = 0; i < this->getTimeStep().size(); i++)
    {
        myFile 
        << this->getTimeStep()[i] << ","
        << this->getDate()[i] << ","
        << this->getPremiumIndicator()[i] << ","
        << this->getAgeNearest()[i] << ","
        << this->getValuationYear()[i] << ","
        << this->getPolicyYear()[i] << std::endl;
    }
}

// methods
std::vector<int> TimeStepProjection::projectTimeStep()
{
    std::vector<int> timeStep(this->_policy.getDurationOutstanding());

    for (int i = 0; i < timeStep.size(); i++)
    {
        timeStep[i] = i+1;
    }

    return timeStep; 
}


std::vector<date> TimeStepProjection::projectDate()
{
    std::vector<date> dateBOM(this->_policy.getDurationOutstanding());

    const date &dateOfValuation = this->_policy.getValuation().getDateOfValuation();

    for (int i = 0; i < dateBOM.size(); i++)
    {
        dateBOM[i] = dateOfValuation + months(i);
    }

    return dateBOM;
}


std::vector<bool> TimeStepProjection::projectPremiumIndicator()
{
    const std::vector<greg_month> &premiumDueMonths = this->_policy.getPremiumDueMonths();
    
    const std::vector<date> &dateBOM = this->_dateBOM;

    //Initialising a vector of size, durationOutstanding, with all elements equal to false.
    std::vector<bool> premiumIndicator (this->_policy.getDurationOutstanding(), false);

    for (int i = 0; i < dateBOM.size(); i++) 
    {
        if ( find(premiumDueMonths.begin(), premiumDueMonths.end(), dateBOM[i].month()) 
            != premiumDueMonths.end() ) 
            {
                premiumIndicator[i] = true;   
            }
    }

    return premiumIndicator;
}

std::vector<int> TimeStepProjection::projectPolicyYear()
{
    const std::vector<date> &dateBOM = this->_dateBOM;

    const date &dateOfCommencement = this->_policy.getDateOfCommencement();

    std::vector<int> policyYear(this->_policy.getDurationOutstanding());

    std::transform
    (
        dateBOM.begin(),
        dateBOM.end(),
        policyYear.begin(),
        [&dateOfCommencement] (date i) 
        {
            return ceil ( (i - dateOfCommencement).days() / 365.2425 );
        }
    );

    return policyYear;
}


std::vector<int> TimeStepProjection::projectValuationYear()
{
    const std::vector<int> &timeStep = this->_timeStep;
    
    //Initialising an empty vector of size, durationLimitingAge.
    std::vector<int> valuationYear(this->_policy.getDurationOutstanding());

    std::transform
    ( 
        timeStep.begin(), 
        timeStep.end(),
        valuationYear.begin(),
        [] (float value) 
        {
            return ceil(value/12); 
        }
    );

    return valuationYear;
}


std::vector<int> TimeStepProjection::projectAgeNearest()
{
    const std::vector<date> &dateBOM = this->_dateBOM;
    
    const date &dateOfBirth = this->_policy.getDateOfBirth();

    std::vector<int> ageNearest(this->_policy.getDurationOutstanding());

    std::transform
    (
        dateBOM.begin(),
        dateBOM.end(),
        ageNearest.begin(),
        [&dateOfBirth] (date i) 
        {    
            return  std::roundf(  (i - dateOfBirth).days() / 365.2425 );
        }
    );

    return ageNearest;
}
