#include "Policy.h"
int Policy::idCnt = 0;

// constructor
Policy::Policy
(   
    Valuation &valuation,

    std::string productType,
    std::string policyNumber,
    std::string gender,
    std::string paymentMode,
    std::string dateOfCommencement,
    std::string dateOfBirth,
    std::string policyTerm,
    std::string sumAssured,
    std::string premium 
) :
    //Initializer list
    _valuation(valuation),

    _productType(productType),
    _policyNumber(policyNumber),
    _gender(gender),
    _paymentMode(paymentMode),
    _dateOfCommencement(from_simple_string(dateOfCommencement)),
    _dateOfBirth(from_simple_string(dateOfBirth)),
    _policyTerm(std::stoi(policyTerm)),           
    _sumAssured(std::stod(sumAssured)),
    _premium(std::stof(premium)),

    _dateOfMaturity(setDateOfMaturity(from_simple_string(dateOfCommencement), std::stoi(policyTerm))),
    _durationInForce(setDurationInForce(from_simple_string(dateOfCommencement))),
    _durationOutstanding(setDurationOutstanding(from_simple_string(dateOfCommencement), std::stoi(policyTerm))),
    _premiumDueMonths(setPremiumDueMonths(from_simple_string(dateOfCommencement), paymentMode))

{
    _id = idCnt++;
}

// print methods
void Policy::printPolicyAttributes()
{
    std::cout << "POLICY_NUMBER: " << this->getPolicyNumber() << std::endl;
    std::cout << "GENDER: " << this->getGender() << std::endl;
    std::cout << "PAYMENT_MODE: " << this->getPaymentMode() << std::endl;
    std::cout << "DATE_OF_COMMENCEMENT: " << this->getDateOfCommencement() << std::endl;
    std::cout << "DATE_OF_BIRTH: " << this->getDateOfBirth() << std::endl;
    std::cout << "POLICY_TERM: " << this->getPolicyTerm() << std::endl;
    std::cout << "SUM_ASSURED: " << this->getSumAssured() << std::endl;
    std::cout << "PREMIUM: " << this->getPremium() << std::endl;
    std::cout << "DATE_OF_MATURITY: " << this->getDateOfMaturity() << std::endl;
    std::cout << "DURATION_IN_FORCE: " << this->getDurationInForce() << std::endl;
    std::cout << "DURATION_OUTSTANDING: " << this->getDurationOutstanding() << std::endl;
    std::cout << "PREMIUM_DUE_MONTHS: ";
    for (int i = 0; i < this->getPremiumDueMonths().size(); i++)
    {
        std::cout << this->getPremiumDueMonths()[i] << ",";
    }
   
}


// setters
date Policy::setDateOfMaturity(date dateOfCommencement, int policyTerm) 
{  
    date dateOfMaturity = dateOfCommencement + years(policyTerm); 
        
    return dateOfMaturity;
}

int Policy::setDurationInForce(date dateOfCommencement)
{
    int durationInForce = ( this->_valuation.getDateOfValuation() - dateOfCommencement ).days() / (365.2425/12);
    
    return durationInForce;   
}

int Policy::setDurationOutstanding(date dateOfCommencement, int policyTerm) 
{
    date dateOfMaturity = dateOfCommencement + years(policyTerm);
    int durationOutstanding = ceil( ( dateOfMaturity - this->_valuation.getDateOfValuation() ).days() / (365.2425/12) ); 

    return durationOutstanding;
}


std::vector<greg_month> Policy::setPremiumDueMonths(date dateOfCommencement, std::string paymentMode) 
{
    std::vector<greg_month> premiumDueMonths;

    char paymentModeCharacter = *(paymentMode.c_str()); //.c_str() from cstring is a pointer so dereferencing. 


    switch (paymentModeCharacter) 
    {

        case 'M':
        {
            premiumDueMonths.push_back(dateOfCommencement.month());
            premiumDueMonths.push_back((dateOfCommencement + months(1)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(2)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(3)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(4)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(5)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(6)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(7)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(8)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(9)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(10)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(11)).month());
        }
        break;

        case 'Q': 
        {
            premiumDueMonths.push_back(dateOfCommencement.month());
            premiumDueMonths.push_back((dateOfCommencement + months(3)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(6)).month());
            premiumDueMonths.push_back((dateOfCommencement + months(9)).month());
        }
        break;

        case 'H':
        {
            premiumDueMonths.push_back(dateOfCommencement.month());
            premiumDueMonths.push_back((dateOfCommencement + months(6)).month());
        }
        break;

        case 'Y':
        {
            premiumDueMonths.push_back(dateOfCommencement.month());
        }
        break;

        default: std::cout << "Not a valid payment mode." << std::endl;
    }
    return premiumDueMonths;
}