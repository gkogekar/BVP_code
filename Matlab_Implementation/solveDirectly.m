function SV = solveDirectly(MESH, SIM, BC, TOL, FLAG)
% SV = A^-1 * b = A\b
A = zeros(MESH.N_SV, MESH.N_SV);
b = zeros(MESH.N_SV, 1);

k = SIM.k;
q_gen = SIM.q_gen;

%Left (x=0) Control Volume (BC: Constant Temp)
A(1, 1) = 1;
b(1, 1) = BC.T_0;

% Internal Control Volumes
for i = 2:MESH.N_CV-1
    A(i, i-1) = (-1/(MESH.x_vec_p(i) - MESH.x_vec_m(i)))...
               *(-k/(MESH.x_vec(i)   - MESH.x_vec(i-1)));
             
    A(i, i)   = (-1/(MESH.x_vec_p(i) - MESH.x_vec_m(i)))...
               *(-k/(MESH.x_vec(i+1) - MESH.x_vec(i)  ))...
               *(-1)...
               -(-1/(MESH.x_vec_p(i) - MESH.x_vec_m(i)))...
               *(-k/(MESH.x_vec(i)   - MESH.x_vec(i-1)))...
               *(1);
             
    A(i, i+1) = (-1/(MESH.x_vec_p(i) - MESH.x_vec_m(i)))...
               *(-k/(MESH.x_vec(i+1) - MESH.x_vec(i)  ));
    b(i, 1)   = -q_gen;
end

% Right (x=L) Control Volume (BC: Constant Temp)
A(MESH.N_SV, MESH.N_SV) = 1;
b(MESH.N_SV, 1)         = BC.T_L;

%% Solve 
% SV = A^-1 * b = A\b

SV = A\b;

%% Test LU Decomposition here
[L,U] = lu(A);
z     = L\b;
SV_LU = U\z;

error_LU = SV_LU - SV
end