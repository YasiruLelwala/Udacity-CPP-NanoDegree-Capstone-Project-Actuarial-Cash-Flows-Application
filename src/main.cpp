#include "PortfolioCashFlows.h"

#include <memory>
#include <future>

std::vector<std::vector<std::string>> readPolicyData () {

    std::string line;
    std::string PRODUCT_TYPE, POLICY_NUMBER, GENDER, PAYMENT_MODE, DATE_OF_COMMENCEMENT, DATE_OF_BIRTH, POLICY_TERM, SUM_ASSURED, PREMIUM;
    std::ifstream samplePoliciesCsv ("../data/Policy_Data.csv");

    std::vector<std::string> policy;
    std::vector<std::vector<std::string>> policies;

    if (samplePoliciesCsv.is_open()) {

        getline(samplePoliciesCsv, line); // Skip the first line. 

        while (getline( samplePoliciesCsv, line) ) {

            replace(line.begin(), line.end(), ',', ' ' );

            std::istringstream linestream(line);

            while (linestream >> PRODUCT_TYPE >> POLICY_NUMBER >> GENDER >> PAYMENT_MODE  >> DATE_OF_COMMENCEMENT >> DATE_OF_BIRTH >> POLICY_TERM  >> SUM_ASSURED >> PREMIUM) {

                policy.push_back(PRODUCT_TYPE);
                policy.push_back(POLICY_NUMBER);
                policy.push_back(GENDER);
                policy.push_back(PAYMENT_MODE);
                policy.push_back(DATE_OF_COMMENCEMENT);
                policy.push_back(DATE_OF_BIRTH);
                policy.push_back(POLICY_TERM);
                policy.push_back (SUM_ASSURED);
                policy.push_back(PREMIUM);
                
                policies.emplace_back(policy);
                policy.clear();
            }
        }
    }
    return policies;
};


int main()
{
//-------------------------------------------------------------------------------------------------//
    auto portfolioCashFlows = std::make_shared<PortfolioCashFlows>();
    std::vector<std::future<void>> futures;

    // Setting of parameters
    std::string dateOfValuation;
    std::cout << "Enter Date of Valuation (YYYY-MM-DD)" << std::endl;
    std::cin >> dateOfValuation; 

    RunType runType;
    std::string runTypeConsoleInput;
    std::cout << "Enter Run Type: Base or Stress" << std::endl;
    std::cin >> runTypeConsoleInput;
    if (runTypeConsoleInput == "Base") 
    {
        runType = RunType::Base;
    } else
    {
        runType = RunType::Stress;
    };

            std::cout << std::endl;

    std::shared_ptr<Valuation> valuation = std::make_shared<Valuation>(from_simple_string(dateOfValuation), runType);
            std::cout << "Valuation object use count is " << valuation.use_count() << std::endl;
    std::shared_ptr<MortalityRates> mortalityRates = std::make_shared<MortalityRates>(*valuation); // mortality rates object is constructed with a reference to a valuation object. 
            std::cout << "Valuation object use count is " << valuation.use_count() << std::endl;
    std::shared_ptr<LapseRates> lapseRates = std::make_shared<LapseRates>(*valuation);
            std::cout << "Valuation object use count is " << valuation.use_count() << std::endl;

            std::cout << std::endl;

    std::vector<std::vector<std::string>> policyData = readPolicyData();
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct Policy objects and push into vector of Policy objects in the PortfolioCashFlows object using std::async
    std::for_each(policyData.begin(), policyData.end(), [&portfolioCashFlows, &valuation, &futures] (std::vector<std::string> &data) {
        std::shared_ptr<Policy> policy = std::make_shared<Policy>(*valuation, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]);
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackPolicy, portfolioCashFlows, std::move(policy))); // moving the shared pointer to a function. The use count will not increase. No destruction upon leaving current scope. 
    });

            std::cout << std::endl; 

    // wait for threads to finish
    std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct TimeStepProjection objects and push into vector of TimeStepProjection objects in the PortfolioCashFlows object using std::async
   std::for_each(portfolioCashFlows->getPolicyVector().begin(), portfolioCashFlows->getPolicyVector().end(), [&portfolioCashFlows,&futures] (std::shared_ptr<Policy> policy) { // taking a shared pointer by value. This will create another shared pointer on the stack while pointing to the same object on heap memory. 
       std::cout << "Policy object use count is " << policy.use_count() << std::endl;  // use count will now increase to 2.
       std::shared_ptr<TimeStepProjection> timeStepProjection = std::make_shared<TimeStepProjection>(*policy.get()); // passing the object managed by the smart pointer. It is passed as a reference (check class definition). I am not simply derefernicing the pointer because the pointer could go out of scope before thread executes which would invalidate the pointer. 
       futures.emplace_back(std::async(&PortfolioCashFlows::pushBackTimeStepProjection, portfolioCashFlows, std::move(timeStepProjection))); // moving the shared pointer to a function. The use count will not increase. No destruction upon leaving current scope. 
   } );

        std::cout << std::endl;

   // wait for threads to finish
   std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the TimeStepProjection vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getTimeStepProjectionVector().begin(), portfolioCashFlows->getTimeStepProjectionVector().end(), [&futures] (std::shared_ptr<TimeStepProjection> timeStepProjection) {
        std::cout << "TimeStepProjection object use count is " << timeStepProjection.use_count() << std::endl;  // use count will now increase to 2.
        futures.emplace_back(std::async(&TimeStepProjection::run, std::ref(*timeStepProjection.get()))); // i am not passing a reference to timestepprojection pointer because while the thread is exectuing and the for loop finishes, the pointer would go out of scope. I dont want to pass the pointer by value to avoid creating of another pointer and unncessarily increase the use count. 
        std::cout << "TimeStepProjection object use count is " << timeStepProjection.use_count() << std::endl;  // use count will remain at 2.
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct DecrementsProjection objects, and push into vector of DecrementsProjection objects in the PortfolioCashFlows object using std::async
    std::for_each(portfolioCashFlows->getTimeStepProjectionVector().begin(), portfolioCashFlows->getTimeStepProjectionVector().end(), [&portfolioCashFlows, &futures, &mortalityRates, &lapseRates] (std::shared_ptr<TimeStepProjection> timeStepProjection) {
        std::cout << "TimeStepProjection object use count is " << timeStepProjection.use_count() << std::endl;  // will increase to 2.
        std::shared_ptr<DecrementsProjection> decrementsProjection = std::make_shared<DecrementsProjection>(*timeStepProjection.get(), *mortalityRates.get(), *lapseRates.get());
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackDecrementsProjection, portfolioCashFlows, std::move(decrementsProjection))); // moving the shared pointer to a function. The use count will not increase. No destruction upon leaving current scope. 
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the DecrementsProjection vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getDecrementsProjectionVector().begin(), portfolioCashFlows->getDecrementsProjectionVector().end(), [&futures] (std::shared_ptr<DecrementsProjection> decrementsProjection) {
        std::cout << "DecrementsProjection object use count is " << decrementsProjection.use_count() << std::endl;  // will increase to 2.
        futures.emplace_back(std::async(&DecrementsProjection::run, std::ref(*decrementsProjection.get()))); // i am not passing a reference to timestepprojection pointer because while the thread is exectuing and the for loop finishes, the pointer would go out of scope. I dont want to pass the pointer by value to avoid creating of another pointer and unncessarily increase the use count. 
        std::cout << "DecrementsProjection object use count is " << decrementsProjection.use_count() << std::endl;  // will remain at 2.
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct CashFlowsProjection objects, and push into vector of CashFlowsProjection objects in the PortfolioCashFlows object using std::async
    std::for_each(portfolioCashFlows->getDecrementsProjectionVector().begin(), portfolioCashFlows->getDecrementsProjectionVector().end(), [&portfolioCashFlows,&futures] (std::shared_ptr<DecrementsProjection> decrementsProjection) {
        std::cout << "DecrementsProjection object use count is " << decrementsProjection.use_count() << std::endl;  // will increase to 2.
        std::shared_ptr<CashFlowsProjection> cashFlowsProjection = std::make_shared<CashFlowsProjection>(*decrementsProjection.get());
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackCashFlowsProjection, portfolioCashFlows, std::move(cashFlowsProjection)));
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the CashFlowsProjection vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getCashFlowsProjectionVector().begin(), portfolioCashFlows->getCashFlowsProjectionVector().end(), [&futures] (std::shared_ptr<CashFlowsProjection> cashFlowsProjection) {
        std::cout << "CashFlowsProjection object use count is " << cashFlowsProjection.use_count() << std::endl;  // will increase to 2.
        futures.emplace_back(std::async(&CashFlowsProjection::run, std::ref(*cashFlowsProjection.get())));
        std::cout << "CashFlowsProjection object use count is " << cashFlowsProjection.use_count() << std::endl;  // will remain at 2.
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // construct CashFlowsProjectionByValuationYear objects, and push into vector of CashFlowsProjectionByValuationYear objects in the PortfolioCashFlows object using std::async
    std::for_each(portfolioCashFlows->getCashFlowsProjectionVector().begin(), portfolioCashFlows->getCashFlowsProjectionVector().end(), [&portfolioCashFlows,&futures] (std::shared_ptr<CashFlowsProjection> cashFlowsProjection) {
        std::cout << "CashFlowsProjection object use count is " << cashFlowsProjection.use_count() << std::endl;  // will increase to 2.
        std::shared_ptr<CashFlowsProjectionByValuationYear> cashFlowsProjectionByValuationYear = std::make_shared<CashFlowsProjectionByValuationYear>(*cashFlowsProjection.get());
        futures.emplace_back(std::async(&PortfolioCashFlows::pushBackCashFlowsProjectionByValuationYear, portfolioCashFlows, std::move(cashFlowsProjectionByValuationYear)));
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });

    // invoke the run function on each element of the CashFlowsProjectionByValuationYear vector in the PortfolioCashFlows object
    std::for_each(portfolioCashFlows->getCashFlowsProjectionByValuationYearVector().begin(), portfolioCashFlows->getCashFlowsProjectionByValuationYearVector().end(), [&futures] (std::shared_ptr<CashFlowsProjectionByValuationYear> cashFlowsProjectionByValuationYear) {
        std::cout << "CashFlowsProjectionByValuationYear object use count is " << cashFlowsProjectionByValuationYear.use_count() << std::endl;  // will increase to 2.
        futures.emplace_back(std::async(&CashFlowsProjectionByValuationYear::run, std::ref(*cashFlowsProjectionByValuationYear.get())));
    });

            std::cout << std::endl;

    // wait for threads to finish
     std::for_each(futures.begin(), futures.end(), [] (auto &ftr) {
        ftr.wait();
    });
//-------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------------------------//
    // invoke the run function of the PortfolioCashFlows object.
    portfolioCashFlows->run();




    std::cout << "Finished" << std::endl;

   

    return 0;
//-------------------------------------------------------------------------------------------------//
}