/*
Boundary Value Problem Class
This class will be used to solve boundary value problems using the Modified Damped Newton's Method
*/

using namespace std;
#include <vector>
#include "Jacobian.h"
#include "BVP_Residual.h"

class BoundaryValueProblem
{
public:
    // Functions
    void readUserInput();
    void initialMesh();
    void initialSolution();
    void setFlags(); //Placeholder for future
    void setDependentVariableLimits(); //Placeholder for future
    void setTolerance(); //Placeholder for future
    void residual();
    void checkStateVariableLimits();
    void checkLookAhead(std::vector<double> tempSV);
    void checkSolutionTolerance();
    void checkMeshTolerance();
    void performNewtonInteration();
    void performMeshRefinement();
    


// Variables
    Jacobian MyJacobian;
    BVP_Residual MyRes;

    bool meshRefined;
    bool foundSolution;
    bool SVWithinLimits;
    bool normIsSmaller;
    bool foundNextSV;

    std::vector<double> currentSV;
    std::vector<double> nextSV;
    std::vector<double> currentCorrectionVector;
    std::vector<double> nextCorrectionVector;

    double lambda = 1.0;
};