
#include "BoundaryValueProblem.h"
#include <iostream>


BoundaryValueProblem::BoundaryValueProblem()
{
    // Default values for tolerance
    absTol = 1e-6;
    relTol = 1e-3;
    
    // Set default values for logic variables
    foundSolution  = true;
    SVWithinTrustRegion = true;
    normIsSmaller  = true;
    foundNextSV    = true;
}

void BoundaryValueProblem::readUserInput()   // PLACEHOLDER
{
    std::cout<<"Reading user input \n";
}

void BoundaryValueProblem::initialSolution(Mesh* ptrMesh, Residual* ptrRes)
{
    std::cout<<"Setting the initial solution\n";

    // Initialize the size of SV
    currentSV.resize((ptrRes->nVariables), (ptrMesh->jPoints));

    // Set the value of all dependent variables linearly from the given seed values
    for (int i = 0; i < (ptrRes->nVariables); i++)
    {
        for(int j = 0; j < (ptrMesh->jPoints); j++)
        {
            currentSV(i,j) = ( (ptrRes->IC(i,1) - ptrRes->IC(i,0))/(ptrMesh->x(ptrMesh->jPoints-1) - ptrMesh->x(0)) ) 
                                * ptrMesh->x(j) + ptrRes->IC(i,0); // Slope eqn for a line
        }
    }
}

void BoundaryValueProblem::setFlags()   // PLACEHOLDER
{
    std::cout<<"Setting Algorithm Flags\n";
}

void BoundaryValueProblem::setDependentVariableLimits() // PLACEHOLDER
{
    std::cout<<"Setting Limits on dependent variables\n";
}

void BoundaryValueProblem::setTolerance() // PLACEHOLDER
{
    std::cout<<"Setting Tolerances\n";
}

void BoundaryValueProblem::performNewtonIteration(Mesh* ptrMesh, Residual* ptrRes, Jacobian* ptrJac)
{
    std::cout<<"\nPerforming Modified Damped Newtons Method to find the solution\n";
    
    // Start the algorithm
    while (!foundSolution)
    {
        foundNextSV = false;
        lambda = 1.0; 
        calcCorrectionVector(ptrMesh, ptrRes, ptrJac);
        foundSolution = true;///////////////////-----------Remove this to test the next section
        /* 
        calcNextSV(ptrMesh, ptrRes, ptrJac);
        checkSolutionTolerance(); //This will determine if another MDNM needs to be performed 
        */
    }    
}


void BoundaryValueProblem::calcCorrectionVector(Mesh* ptrMesh, Residual* ptrRes, Jacobian* ptrJac)
{

    //std::cout<<"\tCalculating correction vector\n";
 
    // Calculate Residual
    MatrixXd residual; ////------------------ Everytime this algorithm runs, will it create a new residual vector or will it replace the memory location of the first one?
    residual = (ptrRes->calculateResidual(currentSV, ptrMesh));
 
    // Calculate Jacobian
    ptrJac->calculateJacobian(currentSV, ptrMesh, ptrRes); ////------------------ This also calls residual many times...only one of them is used more than once. Could make a temporary residual variable in Jacobian?
    
    // Solve for correction vector ---  CV = inv(Jac(SV))*(-Res(SV))  ---
    currentCorrectionVector = (ptrJac->jac.inverse()) * (-1 * residual); 
        //std::cout << "Calculation of the correction vector is: \n" << currentCorrectionVector << std::endl;
    
    // Calculate the norm of the correction vector (This is used later)
    currentNorm = currentCorrectionVector.norm();
}

void BoundaryValueProblem::calcNextSV(Mesh* ptrMesh, Residual* ptrRes, Jacobian* ptrJac)
{
    //std::cout<<"\nCalculating the next solution vector\n";
    
    // while a nextSV has not been accepted
    while (!foundNextSV)
    {
        // Calculate a temporary SV
        tempSV = currentSV + lambda * currentCorrectionVector;

        // Check if this SV is within the trust region
        checkSVTrustRegion(tempSV); //// --------- This currently does nothing

        // Check if the damping is satisfied (----- Is this the right way of saying this?)
        checkLookAhead(tempSV, ptrMesh, ptrRes, ptrJac);

        // Determine if the temp solution vector is accepted (---------or if a new Jacobian should be calculated or a time step should be taken ----- not implemented yet)
        if (normIsSmaller && SVWithinTrustRegion) // <-- requirement for the tempSV to be accepted
        {
            foundNextSV = true; // Stops the while loop in this function
            nextSV = tempSV;
            ////--------- Set the next correction vector from what was calculated inside check Look Ahead so maybe just do this there?
        }
        else
        {
            lambda = lambda * 0.5;
        }
        std::cout<< "lambda is: " << lambda << std::endl;
    }
}

void BoundaryValueProblem::checkSVTrustRegion(MatrixXd tempSV) ////-------------- This function needs to be built
{
    //std::cout<<"\t\tChecking if all state variables are within the defined limits\n";
    SVWithinTrustRegion = true; 
}

void BoundaryValueProblem::checkLookAhead(MatrixXd tempSV, Mesh* ptrMesh, Residual* ptrRes, Jacobian* ptrJac)
{
    std::cout<<"\t\tChecking if the new state vector meets look ahead tolerance\n";
    
    // Calculate the next correction vector from the next SV
    MatrixXd tempRes;  ////---------- Another temporary residual... consumes more memory?
    tempRes = (ptrRes->calculateResidual(tempSV, ptrMesh));
    nextCorrectionVector = (ptrJac->jac.inverse()) * tempRes;
    
    //Calculate the norm of the current and next correction vectors
    tempNorm = nextCorrectionVector.norm();
    
    std::cout << "The norm of the current correction vector is: " << currentNorm << std::endl;
    std::cout << "The norm of the next correction vector is: "    << tempNorm    << std::endl;

    if (tempNorm < currentNorm)
    {
        normIsSmaller = true;
        nextNorm = tempNorm; //---------Maybe?
    }
    else
    {
        normIsSmaller = false;
    }
}

void BoundaryValueProblem::checkSolutionTolerance()
{
    std::cout<<"\tChecking if the latest solution is within tolerance\n";
    ////-------- norm(next(or current) correction vector) < max of (  Abs_tol or Rel_tol*SV_next (or curent?)  )
    double maxTol;
    maxTol = std::max(absTol,relTol*nextSV.norm());

    if (nextNorm < maxTol)
    {
        foundSolution = true;
    }
    else
    {
        // Increase the number on Jacobian count
    }
    currentSV = nextSV;
}

void BoundaryValueProblem::saveResults()   // PLACEHOLDER
{
    std::cout<<"Saving the results to a file\n";
}
