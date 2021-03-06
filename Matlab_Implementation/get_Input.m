%% get_Input
% This is a file that contains the users input

function [MESH, SIM, TOL, BC, FLAG] = get_Input()
%% Domain Parameters
MESH.jPoints = 10; % Number of control volumes
MESH.L       = 1; % Length of domain in x-dir

%% BC
BC.T_0 = 100;  %[K], temperature at x = 0
BC.T_L = 1000; %[K], temperature at x = L

%% Properties
SIM.k = 20;
SIM.q_gen = 100000;
% SIM.q_gen = 0;

%% Tolerance Parameters
TOL.A = 1e-6; % Absolute tolerance for MDNM
TOL.R = 1e-3; % Relative tolerance for MDNM
TOL.a = 1e-6; % Absolute tolerance for pertebation
TOL.r = 1e-3; % Relative tolerance for pertebation
TOL.delta = 0.2; 
TOL.gamma = 0.5; %%%%Not implemented yet

%% Residual Function Name
SIM.Res_name = 'BVP_residual'; % File name associated with the residual function %%%%%%Not really used in Matlab so maybe won't be needed

%% Variables
SIM.nDepVariables = 1; %%%%%%%I don't know how this would look for our implementation

%% Initial Guess
SIM.SV_0 = 200 * ones(MESH.jPoints,1); %%%%%%%I don't know how this would look for our implementation


%% Limits on Variables


%% Save Results File Location


%% Flags
FLAG.Do_Mesh_Refinement = 0; % 1 if the algorithm should perform mesh refinement

end