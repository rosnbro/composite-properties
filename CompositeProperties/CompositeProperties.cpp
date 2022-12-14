// CompositeProperties.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

#define N 7
typedef double array_type[3][3];

void rstiffness(double M[][3], double e1, double e2, double g12, double V12) {
	double Q11, Q12, Q22, Q66;
	double V21 = V12*(e2/e1);

	Q11 = (e1) / (1 - V12*V21);
	Q12 = (V12*e2) / (1 - V12*V21);
	Q22 = (e2) / (1 - V12*V21);
	Q66 = g12;

	M[0][0] = Q11;
	M[0][1] = Q12;
	M[0][2] = 0;
	M[1][0] = Q12;
	M[1][1] = Q22;
	M[1][2] = 0;
	M[2][0] = 0;
	M[2][1] = 0;
	M[2][2] = Q66;
}

void rcompliance(double M[][3], double e1, double e2, double g12, double V12) {
	double S11, S12, S22, S66;

	S11 = 1 / e1;
	S12 = -V12 / e1;
	S22 = 1 / e2;
	S66 = 1 / g12;

	M[0][0] = S11;
	M[0][1] = S12;
	M[0][2] = 0;
	M[1][0] = S12;
	M[1][1] = S22;
	M[1][2] = 0;
	M[2][0] = 0;
	M[2][1] = 0;
	M[2][2] = S66;
} 

void trstiffness(double M[][3], double e1, double e2, double g12, double V12, double m, double n) {
	double Q11t, Q12t, Q16t, Q22t, Q26t, Q66t;
	double Q11, Q12, Q22, Q66;
	double V21 = V12*(e2 / e1);

	Q11 = (e1) / (1 - V12*V21);
	Q12 = (V12*e2) / (1 - V12*V21);
	Q22 = (e2) / (1 - V12*V21);
	Q66 = g12;

	Q11t = Q11*pow(m, 4) + 2 * (Q12 + 2 * Q66)*pow(n, 2)*pow(m, 2) + Q22*pow(n, 4);
	Q12t = (Q11 + Q22 - 4 * Q66)*pow(n, 2)*pow(m, 2) + Q12*(pow(n, 4) + pow(m, 4));
	Q16t = (Q11 - Q12 - 2 * Q66)*n*pow(m, 3) + (Q12 - Q22 + 2 * Q66)*pow(n, 3) * m;
	Q22t = Q11*pow(n, 4) + 2 * (Q12 + 2 * Q66)*pow(n, 2) * pow(m, 2) + Q22*pow(m, 4);
	Q26t = (Q11 - Q12 - 2 * Q66)*pow(n, 3) * m + (Q12 - Q22 + 2 * Q66)*n*pow(m, 3);
	Q66t = (Q11 + Q22 - 2 * Q12 - 2 * Q66)*pow(n, 2)*pow(m, 2) + Q66*(pow(n, 4) + pow(m, 4));

	M[0][0] = Q11t;
	M[0][1] = Q12t;
	M[0][2] = Q16t;
	M[1][0] = Q12t;
	M[1][1] = Q22t;
	M[1][2] = Q26t;
	M[2][0] = Q16t;
	M[2][1] = Q26t;
	M[2][2] = Q66t;
}

void trcompliance(double M[][3], double e1, double e2, double g12, double V12, double m, double n) {
	double S11, S12, S22, S66;
	double S11t, S12t, S16t, S22t, S26t, S66t;

	S11 = 1 / e1;
	S12 = -V12 / e1;
	S22 = 1 / e2;
	S66 = 1 / g12;

	S11t = S11*pow(m, 4) + 2 * (S12 + 2 * S66)*pow(n, 2)*pow(m, 2) + S22*pow(n, 4);
	S12t = (S11 + S22 - 4 * S66)*pow(n, 2)*pow(m, 2) + S12*(pow(n, 4) + pow(m, 4));
	S16t = (S11 - S12 - 2 * S66)*n*pow(m, 3) + (S12 - S22 + 2 * S66)*pow(n, 3) * m;
	S22t = S11*pow(n, 4) + 2 * (S12 + 2 * S66)*pow(n, 2) * pow(m, 2) + S22*pow(m, 4);
	S26t = (S11 - S12 - 2 * S66)*pow(n, 3) * m + (S12 - S22 + 2 * S66)*n*pow(m, 3);
	S66t = (S11 + S22 - 2 * S12 - 2 * S22)*pow(n, 2)*pow(m, 2) + S66*(pow(n, 4) + pow(m, 4));

	M[0][0] = S11t;
	M[0][1] = S12t;
	M[0][2] = S16t;
	M[1][0] = S12t;
	M[1][1] = S22t;
	M[1][2] = S26t;
	M[2][0] = S16t;
	M[2][1] = S26t;
	M[2][2] = S66t;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	//Define variables

	double E1[10], E2[10], E3[10];
	double Eex, Eey, Gexy, vexy, veyx;
	double v23[10], v13[10], v12[10];
	double G23[10], G13[10], G12[10];
	double a1[10], a2[10], a3[10];
	double ax[10], ay[10], axy[10];
	double B1[10], B2[10], B3[10];
	double Bx[10], By[10], Bxy[10];
	double dT, dM;
	double T[3][3], RCM[3][3], RSM[3][3], TRCM[3][3], TRSM[3][3];
	double A1[10][5], A2[10][5], C1[10][5], C2[10][5], D1[10][5], D2[10][5], F1[10][5], F2[10][5];
	double ABD[6][6], abd[6][6], abdinplace[6][12];
	double Ep_x_o, Ep_y_o, Gam_xy_o, K_x_o, K_y_o, K_xy_o;
	double Nx, Ny, Nxy, Mx, My, Mxy;
	double Nxt, Nyt, Nxyt, Mxt, Myt, Mxyt;
	double Nxth, Nyth, Nxyth, Mxth, Myth, Mxyth;
	double Nxm, Nym, Nxym, Mxm, Mym, Mxym;
	double Nxmh, Nymh, Nxymh, Mxmh, Mymh, Mxymh;
	double NM[6], MSC[6];
	double Ep_x1, Ep_y1, Gam_xy1, Sig_x1, Sig_y1, t_xy1;
	double Ep_x2, Ep_y2, Gam_xy2, Sig_x2, Sig_y2, t_xy2;
	double Ep1_1, Ep2_1, Gam12_1, Sig1_1, Sig2_1, t12_1;
	double Ep1_2, Ep2_2, Gam12_2, Sig1_2, Sig2_2, t12_2;
	double angle;
	double H;
	double h[10];
	double z1[10], z2[10];
	int material, path1, layers, b;
	int x = 1;
	double theta[10];
	double m[10], n[10];
	double pi = acos(-1.0);
	double ratio, a;
	int num = 6;
	int  p, q;

	//Initialize unit thermal and moisture forces and moments

	Nxth = 0;
	Nyth = 0;
	Nxyth = 0;
	Mxth = 0;
	Myth = 0;
	Mxyth = 0;
	Nxmh = 0;
	Nymh = 0;
	Nxymh = 0;
	Mxmh = 0;
	Mymh = 0;
	Mxymh = 0;

	//Define standard material constants

	//Graphite
	double E1gr = 155.0e9;
	double E2gr = 12.10e9;
	double E3gr = E2gr;
	double v23gr = 0.458;
	double v13gr = 0.248;
	double v12gr = v13gr;
	double G23gr = 3.20e9;
	double G13gr = 4.40e9;
	double G12gr = G13gr;
	double a1gr = -18e-9;
	double a2gr = 243e-7;
	double a3gr = a2gr;
	double B1gr = 146e-6;
	double B2gr = 4770e-6;
	double B3gr = B2gr;
	
	//Glass
	double E1gl = 50.0;
	double E2gl = 15.20;
	double E3gl = E2gl;
	double v23gl = 0.428;
	double v13gl = 0.254;
	double v12gl = v13gl;
	double G23gl = 3.28;
	double G13gl = 4.70;
	double G12gl = G13gl;
	double a1gl = 634e-8;
	double a2gl = 233e-7;
	double a3gl = a2gl;
	double B1gl = 434e-6;
	double B2gl = 6320e-6;
	double B3gl = B2gl;
	
	//Aluminum
	double E1a = 72.4;
	double E2a = E1a;
	double E3a = E1a;
	double v23a = 0.300;
	double v13a = v23a;
	double v12a = v23a;
	double G23a = E2a / (2 * (1 + v23a));
	double G13a = E1a / (2 * (1 + v13a));
	double G12a = E1a / (2 * (1 + v12a));
	double a1a = 225e-7;
	double a2a = a1a;
	double a3a = a1a;
	double B1a = 0;
	double B2a = 0;
	double B3a = 0;

	//Set standard precision

	std::cout << std::scientific;
	std::cout << std::setprecision(3);

	//Prompt user for number of layers

	cout << "Input number of layers in the composite (10 max): ";
	cin >> layers;

	b = layers + 1;
	H = 0;

	cout << "Consider environmental effects" << endl;
	cout << "Enter the change in temperature (C): ";
	cin >> dT;
	cout << "Enter the change in moisture (%): ";
	cin >> dM;

	//////////////////////////////////////////////////////////////Define Layers//////////////////////////////////////////////////////////////

	for (int a = 1; a < b; a++) {
		
		//Prompt user for layer thickness

		cout << endl;
		cout << "Input the thickness (h) of layer " << a << ":	";
		cin >> h[a];

		//layer thickness storage

		H = H + h[a];

		//Prompt user for material properties

		cout << "Specify mechanical constants for layer " << a << ":" << endl;
		cout << "  1)Graphite-Polymer Composite" << endl << "  2)Glass-Polymer Composite" << endl << "  3)Aluminum" << endl << "  4)Custom" << endl;
		cout << "Input choice:	";
		cin >> material;
		
		//Set material properties

		do {
			if (material == 1) {
				//Mechanical Constants for Graphite-Polymer Composites
				E1[a] = E1gr;
				E2[a] = E2gr;
				E3[a] = E3gr;
				v23[a] = v23gr;
				v13[a] = v13gr;
				v12[a] = v12gr;
				G23[a] = G23gr;
				G13[a] = G13gr;
				G12[a] = G12gr;
				a1[a] = a1gr;
				a2[a] = a2gr;
				a3[a] = a3gr;
				B1[a] = B1gr;
				B2[a] = B2gr;
				B3[a] = B3gr;
				x = 0;
			}
			else if (material == 2) {
				//Mechanical Constants for Glass-Polymer Composites
				E1[a] = E1gl;
				E2[a] = E2gl;
				E3[a] = E3gl;
				v23[a] = v23gl;
				v13[a] = v13gl;
				v12[a] = v12gl;
				G23[a] = G23gl;
				G13[a] = G13gl;
				G12[a] = G12gl;
				a1[a] = a1gl;
				a2[a] = a2gl;
				a3[a] = a3gl;
				B1[a] = B1gl;
				B2[a] = B2gl;
				B3[a] = B3gl;
				x = 0;
			}
			else if (material == 3) {
				//Mechanical Constants for Aluminum
				E1[a] = E1a;
				E2[a] = E2a;
				E3[a] = E3a;
				v23[a] = v23a;
				v13[a] = v13a;
				v12[a] = v12a;
				G23[a] = G23a;
				G13[a] = G13a;
				G12[a] = G12a;
				a1[a] = a1a;
				a2[a] = a2a;
				a3[a] = a3a;
				B1[a] = B1a;
				B2[a] = B2a;
				B3[a] = B3a;
				x = 0;
			}
			else if (material == 4) {
				//User prompted for custom mechanical constants
				cout << "  E1: ";
				cin >> E1[a];
				cout << "  E2: ";
				cin >> E2[a];
				cout << "  E3: ";
				cin >> E3[a];
				cout << "  v23: ";
				cin >> v23[a];
				cout << "  v13: ";
				cin >> v13[a];
				cout << "  v12: ";
				cin >> v12[a];
				cout << "  G23: ";
				cin >> G23[a];
				cout << "  G13: ";
				cin >> G13[a];
				cout << "  G12: ";
				cin >> G12[a];
				cout << "  a1: ";
				cin >> a1[a];
				cout << "  a2: ";
				cin >> a2[a];
				cout << "  a3: ";
				cin >> a3[a];
				cout << "  B1: ";
				cin >> B1[a];
				cout << "  B2: ";
				cin >> B2[a];
				cout << "  B3: ";
				cin >> B3[a];
				x = 0;
			}
			else {
				cout << "Please input a valid option: ";
				cin >> material;
			}
		} while (x == 1);

		//Prompt user for material direction

		cout << "Input material direction (in degrees):	";
		cin >> angle;

		//Convert degrees to radians and store m & n values

		theta[a] = (angle*pi) / 180;
		m[a] = cos(theta[a]);
		n[a] = sin(theta[a]);
		
		//Transform Thermal constants

		ax[a] = a1[a] * (cos(theta[a])*cos(theta[a])) + a2[a] * (sin(theta[a])*sin(theta[a]));
		ay[a] = a1[a] * (sin(theta[a])*sin(theta[a])) + a2[a] * (cos(theta[a])*cos(theta[a]));
		axy[a] = 2.0 * (a1[a] - a2[a])*cos(theta[a])*sin(theta[a]);

		//Print transformed thermal constants

		cout << endl << "Global thermal expansion coefficients for layer " << a << ":" << endl;
		cout << "ax = " << ax[a] << endl;
		cout << "ay = " << ay[a] << endl;
		cout << "axy = " << axy[a] << endl;

		//Transform Moisture constants

		Bx[a] = B1[a] * (cos(theta[a])*cos(theta[a])) + B2[a] * (sin(theta[a])*sin(theta[a]));
		By[a] = B1[a] * (sin(theta[a])*sin(theta[a])) + B2[a] * (cos(theta[a])*cos(theta[a]));
		Bxy[a] = 2.0 * (B1[a] - B2[a])*cos(theta[a])*sin(theta[a]);

		cout << endl;

		//Print Transform Reduced Stiffness Matrix

		trstiffness(TRSM, E1[a], E2[a], G12[a], v12[a], m[a], n[a]);

		cout << "Transform Reduced Stiffness Matrix: " << endl;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cout << "	" << TRSM[i][j];
			}
			cout << endl;
		}
	}
	cout << endl;

	//Determine layer interface locations

	z1[1] = -H / 2;
	z2[1] = z1[1] + h[1];

	for (int a = 2; a < b; a++) {
		z1[a] = z2[a - 1];
		z2[a] = z1[a] + h[a];
	}
	
	//////////////////////////////////////////////////////////Define Materials////////////////////////////////////////////////////////////////////////////

	//Zero out the ABD and abd matrices

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			ABD[i][j] = 0.0;
			abd[i][j] = 0.0;
		}
	}

	//Calculate ABD matrix

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (i < 3 && j < 3) {
				for (int a = 1; a < b; a++) {
					trstiffness(TRSM, E1[a], E2[a], G12[a], v12[a], m[a], n[a]);
					//A portion of ABD
					ABD[i][j] = ABD[i][j] + TRSM[i][j] * (z2[a] - z1[a]);

				}
			}
			else if (i < 3 && j >= 3) {
				for (int a = 1; a < b; a++) {
					int q = j - 3;
					trstiffness(TRSM, E1[a], E2[a], G12[a], v12[a], m[a], n[a]);
					//Top B portion of ABD
					ABD[i][j] = ABD[i][j] + 0.5 * TRSM[i][q] * (pow(z2[a], 2) - pow(z1[a], 2));
				}
			}
			else if (i >= 3 && j < 3) {
				for (int a = 1; a < b; a++) {
					int p = i - 3;
					trstiffness(TRSM, E1[a], E2[a], G12[a], v12[a], m[a], n[a]);
					//Bottom B portion of ABD
					ABD[i][j] = ABD[i][j] + 0.5 * TRSM[p][j] * (pow(z2[a], 2) - pow(z1[a], 2));
				}
			}
			else if (i >= 3 && j >= 3) {
				for (int a = 1; a < b; a++) {
					p = i - 3;
					q = j - 3;
					trstiffness(TRSM, E1[a], E2[a], G12[a], v12[a], m[a], n[a]);
					//D portion of ABD
					ABD[i][j] = ABD[i][j] + (TRSM[p][q] * (pow(z2[a], 3) - pow(z1[a], 3))) / 3.0;

				}
			}
		}
	}

	//Calculate abd matrix (inverse of ABD)		

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			//Create matrix to be destroyed by pivoting
			abdinplace[i][j] = ABD[i][j];
		}
	}

	for (int i = 0; i < num; i++) {
		for (int j = num; j < 2 * num; j++) {
			//Append identity matrix to dummy inverse matrix
			if (i == (j - num))
				abdinplace[i][j] = 1.0;
			else
				abdinplace[i][j] = 0.0;
		}
	}
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			//Row reduction
			if (i != j) {
				ratio = abdinplace[j][i] / abdinplace[i][i];
				for (int k = 0; k < 2 * num; k++) {
					abdinplace[j][k] -= ratio * abdinplace[i][k];
				}
			}
		}
	}
	for (int i = 0; i < num; i++) {
		a = abdinplace[i][i];
		for (int j = 0; j < 2 * num; j++) {
			abdinplace[i][j] /= a;
		}
	}

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			//assign true inverse matrix
			abd[i][j] = abdinplace[i][j + 6];
		}
	}

	//Print ABD and abd matrices

	cout << endl << "The ABD Matrix is: " << endl;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			cout << ABD[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "The abd Matrix is: " << endl;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			cout << abd[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;

	//Calculate effective composite properties

	Eex = (ABD[0][0] * ABD[1][1] - ABD[0][1] * ABD[0][1]) / (ABD[1][1] * H);
	Eey = (ABD[0][0] * ABD[1][1] - ABD[0][1] * ABD[0][1]) / (ABD[0][0] * H);
	Gexy = ABD[2][2] / H;
	vexy = ABD[0][1] / ABD[1][1];
	veyx = ABD[0][1] / ABD[0][0];

	//Print effective composite properties

	cout << "Effective composite properties: " << endl;
	cout << "  Ex - " << Eex << endl;
	cout << "  Ey - " << Eey << endl;
	cout << "  Gxy - " << Gexy << endl;
	cout << "  vxy - " << vexy << endl;
	cout << "  vyx - " << veyx << endl;
	cout << endl;

	//Prompt user for given values

	cout << "Which values are given?" << endl;
	cout << "  1) Midsurface strains and curvatures" << endl;
	cout << "  2) Force and Moment Resultants" << endl;
	cout << "Input choice: ";
	cin >> path1;
	cout << endl;

	////////////////////////////////////////////////////////Target Value Computation///////////////////////////////////////////////////////////

	if (path1 == 1) {
		//Prompt user for midsurface strains and curvatures 
		cout << "Input the extensional strains of the reference surface:" << endl;
		cout << "  Epsilon_x_o: ";
		cin >> Ep_x_o;
		cout << "  Epsilon_y_o: ";
		cin >> Ep_y_o;
		cout << "  Gamma_xy_o: ";
		cin >> Gam_xy_o;

		cout << "Input the curvatures of the reference surface:" << endl;
		cout << "  Kappa_x_o: ";
		cin >> K_x_o;
		cout << "  Kappa_y_o: ";
		cin >> K_y_o;
		cout << "  Kappa_xy_o: ";
		cin >> K_xy_o;
		cout << endl;

		//Kirchhoff Hypothesis

		for (int c = 1; c < b; c++) {

			//Define transform matrix

			T[0][0] = pow(m[c], 2);
			T[0][1] = pow(n[c], 2);
			T[0][2] = 2 * m[c] * n[c];
			T[1][0] = pow(n[c], 2);
			T[1][1] = pow(m[c], 2);
			T[1][2] = -2 * m[c] * n[c];
			T[2][0] = -m[c] * n[c];
			T[2][1] = m[c] * n[c];
			T[2][2] = pow(m[c], 2) - pow(n[c], 2);

			//Call stress/strain relations matrix functions

			rstiffness(RSM, E1[c], E2[c], G12[c], v12[c]);
			rcompliance(RCM, E1[c], E2[c], G12[c], v12[c]);
			trstiffness(TRSM, E1[c], E2[c], G12[c], v12[c], m[c], n[c]);
			trcompliance(TRCM, E1[c], E2[c], G12[c], v12[c], m[c], n[c]);

			//Calculate unit thermal stress resultants 
			
			Nxth = Nxth + (TRSM[0][0] * ax[c] + TRSM[0][1] * ay[c] + TRSM[0][2] * axy[c]) * (z2[c]-z1[c]);
			Mxth = Mxth + 0.5 * (TRSM[0][0] * ax[c] + TRSM[0][1] * ay[c] + TRSM[0][2] * axy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nyth = Nyth + (TRSM[1][0] * ax[c] + TRSM[1][1] * ay[c] + TRSM[1][2] * axy[c]) * (z2[c] - z1[c]);
			Myth = Myth + 0.5 * (TRSM[1][0] * ax[c] + TRSM[1][1] * ay[c] + TRSM[1][2] * axy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nxyth = Nxyth + (TRSM[2][0] * ax[c] + TRSM[2][1] * ay[c] + TRSM[2][2] * axy[c]) * (z2[c] - z1[c]);
			Mxyth = Mxyth + 0.5 * (TRSM[2][0] * ax[c] + TRSM[2][1] * ay[c] + TRSM[2][2] * axy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);

			//Calculate unit moisture stress resultants

			Nxmh = Nxmh + (TRSM[0][0] * Bx[c] + TRSM[0][1] * By[c] + TRSM[0][2] * Bxy[c]) * (z2[c] - z1[c]);
			Mxmh = Mxmh + 0.5 * (TRSM[0][0] * Bx[c] + TRSM[0][1] * By[c] + TRSM[0][2] * Bxy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nymh = Nymh + (TRSM[1][0] * Bx[c] + TRSM[1][1] * By[c] + TRSM[1][2] * Bxy[c]) * (z2[c] - z1[c]);
			Mymh = Mymh + 0.5 * (TRSM[1][0] * Bx[c] + TRSM[1][1] * By[c] + TRSM[1][2] * Bxy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nxymh = Nxymh + (TRSM[2][0] * Bx[c] + TRSM[2][1] * By[c] + TRSM[2][2] * Bxy[c]) * (z2[c] - z1[c]);
			Mxymh = Mxymh + 0.5 * (TRSM[2][0] * Bx[c] + TRSM[2][1] * By[c] + TRSM[2][2] * Bxy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			
			//Calculate global strains 

			Ep_x1 = Ep_x_o + (z1[c] * K_x_o);
			Ep_x2 = Ep_x_o + (z2[c] * K_x_o);
			Ep_y1 = Ep_y_o + (z1[c] * K_y_o);
			Ep_y2 = Ep_y_o + (z2[c] * K_y_o);
			Gam_xy1 = Gam_xy_o + (z1[c] * K_xy_o);
			Gam_xy2 = Gam_xy_o + (z2[c] * K_xy_o);

			//Calculate global stresses

			Sig_x1 = TRSM[0][0] * (Ep_x1 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[0][1] * (Ep_y1 - (ay[c] * dT) - (By[c] * dM)) + TRSM[0][2] * (Gam_xy1 - (axy[c] * dT) - (Bxy[c] * dM));
			Sig_x2 = TRSM[0][0] * (Ep_x2 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[0][1] * (Ep_y2 - (ay[c] * dT) - (By[c] * dM)) + TRSM[0][2] * (Gam_xy2 - (axy[c] * dT) - (Bxy[c] * dM));
			Sig_y1 = TRSM[1][0] * (Ep_x1 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[1][1] * (Ep_y1 - (ay[c] * dT) - (By[c] * dM)) + TRSM[1][2] * (Gam_xy1 - (axy[c] * dT) - (Bxy[c] * dM));
			Sig_y2 = TRSM[1][0] * (Ep_x2 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[1][1] * (Ep_y2 - (ay[c] * dT) - (By[c] * dM)) + TRSM[1][2] * (Gam_xy2 - (axy[c] * dT) - (Bxy[c] * dM));
			t_xy1 = TRSM[2][0] * (Ep_x1 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[2][1] * (Ep_y1 - (ay[c] * dT) - (By[c] * dM)) + TRSM[2][2] * (Gam_xy1 - (axy[c] * dT) - (Bxy[c] * dM));
			t_xy2 = TRSM[2][0] * (Ep_x2 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[2][1] * (Ep_y2 - (ay[c] * dT) - (By[c] * dM)) + TRSM[2][2] * (Gam_xy2 - (axy[c] * dT) - (Bxy[c] * dM));

			//Calculate principal strains

			Ep1_1 = (T[0][0] * Ep_x1 + T[0][1] * Ep_y1 + T[0][2] * 0.5 * Gam_xy1);
			Ep1_2 = (T[0][0] * Ep_x2 + T[0][1] * Ep_y2 + T[0][2] * 0.5 * Gam_xy2);
			Ep2_1 = (T[1][0] * Ep_x1 + T[1][1] * Ep_y1 + T[1][2] * 0.5 * Gam_xy1);
			Ep2_2 = (T[1][0] * Ep_x2 + T[1][1] * Ep_y2 + T[1][2] * 0.5 * Gam_xy2);
			Gam12_1 = 2.0 * (T[2][0] * Ep_x1 + T[2][1] * Ep_y1 + T[2][2] * 0.5 * Gam_xy1);
			Gam12_2 = 2.0 * (T[2][0] * Ep_x2 + T[2][1] * Ep_y2 + T[2][2] * 0.5 * Gam_xy2);

			//Calculate principal stresses

			Sig1_1 = T[0][0] * Sig_x1 + T[0][1] * Sig_y1 + T[0][2] * t_xy1;
			Sig1_2 = T[0][0] * Sig_x2 + T[0][1] * Sig_y2 + T[0][2] * t_xy2;
			Sig2_1 = T[1][0] * Sig_x1 + T[1][1] * Sig_y1 + T[1][2] * t_xy1;
			Sig2_2 = T[1][0] * Sig_x2 + T[1][1] * Sig_y2 + T[1][2] * t_xy2;
			t12_1 = T[2][0] * Sig_x1 + T[2][1] * Sig_y1 + T[2][2] * t_xy1;
			t12_2 = T[2][0] * Sig_x2 + T[2][1] * Sig_y2 + T[2][2] * t_xy2;

			//Store data for table of values

			//Global Strains

			A1[c][0] = c;
			A2[c][0] = c;
			A1[c][1] = z1[c];
			A2[c][1] = z2[c];
			A1[c][2] = Ep_x1;
			A2[c][2] = Ep_x2;
			A1[c][3] = Ep_y1;
			A2[c][3] = Ep_y2;
			A1[c][4] = Gam_xy1;
			A2[c][4] = Gam_xy2;

			//Global Stresses

			C1[c][0] = c;
			C2[c][0] = c;
			C1[c][1] = z1[c];
			C2[c][1] = z2[c];
			C1[c][2] = Sig_x1;
			C2[c][2] = Sig_x2;
			C1[c][3] = Sig_y1;
			C2[c][3] = Sig_y2;
			C1[c][4] = t_xy1;
			C2[c][4] = t_xy2;

			//Principal Strains

			D1[c][0] = c;
			D2[c][0] = c;
			D1[c][1] = z1[c];
			D2[c][1] = z2[c];
			D1[c][2] = Ep1_1;
			D2[c][2] = Ep1_2;
			D1[c][3] = Ep2_1;
			D2[c][3] = Ep2_2;
			D1[c][4] = Gam12_1;
			D2[c][4] = Gam12_2;

			//Principal Stresses

			F1[c][0] = c;
			F2[c][0] = c;
			F1[c][1] = z1[c];
			F2[c][1] = z2[c];
			F1[c][2] = Sig1_1;
			F2[c][2] = Sig1_2;
			F1[c][3] = Sig2_1;
			F2[c][3] = Sig2_2;
			F1[c][4] = t12_1;
			F2[c][4] = t12_2;
		}

		//Print unit force and moment resultants

		cout << "The Unit Thermal Force and Moment Resultants are:" << endl;
		cout << "  Nxth = " << Nxth << endl;
		cout << "  Nyth = " << Nyth << endl;
		cout << "  Nxyth = " << Nxyth << endl;
		cout << "  Mxth = " << Mxth << endl;
		cout << "  Myth = " << Myth << endl;
		cout << "  Mxyth = " << Mxyth << endl;
		cout << endl;

		cout << "The Unit Moisture Force and Moment Resultants are:" << endl;
		cout << "  Nxmh = " << Nxmh << endl;
		cout << "  Nymh = " << Nymh << endl;
		cout << "  Nxymh = " << Nxymh << endl;
		cout << "  Mxmh = " << Mxmh << endl;
		cout << "  Mymh = " << Mymh << endl;
		cout << "  Mxymh = " << Mxymh << endl;
		cout << endl;

		//Calcluate thermal force and moment resultants

		Nxt = Nxth * dT;
		Nyt = Nyth * dT;
		Nxyt = Nxyth * dT;
		Mxt = Mxth * dT;
		Myt = Myth * dT;
		Mxyt = Mxyth * dT;

		//Calcluate moisture force and moment resultants

		Nxm = Nxmh * dM;
		Nym = Nymh * dM;
		Nxym = Nxymh * dM;
		Mxm = Mxmh * dM;
		Mym = Mymh * dM;
		Mxym = Mxymh * dM;

		//ABD matrix relations

		for (int i = 0; i < 7; i++) {
			//Calculate force and moment resultants
			NM[i] = ABD[i][0] * Ep_x_o + ABD[i][1] * Ep_y_o + ABD[i][2] * Gam_xy_o + ABD[i][3] * K_x_o + ABD[i][4] * K_y_o + ABD[i][5] * K_xy_o;
		}

		//Print results

		cout << "The corresponding Force and Moment Resultants are:" << endl;
		cout << "  Nx = " << NM[0] - Nxt - Nxm << endl;
		cout << "  Ny = " << NM[1] - Nyt - Nym << endl;
		cout << "  Nxy = " << NM[2] - Nxyt - Nxym << endl;
		cout << "  Mx = " << NM[3] - Mxt - Mxm << endl;
		cout << "  My = " << NM[4] - Myt - Mym << endl;
		cout << "  Mxy = " << NM[5] - Mxyt - Mxym << endl;
		cout << endl;

		//Print Thermal and Moisture force and moment resultants

		cout << "The corresponding Thermal Force and Moment Resultants are:" << endl;
		cout << "  Nxt = " << Nxt << endl;
		cout << "  Nyt = " << Nyt << endl;
		cout << "  Nxyt = " << Nxyt << endl;
		cout << "  Mxt = " << Mxt << endl;
		cout << "  Myt = " << Myt << endl;
		cout << "  Mxyt = " << Mxyt << endl;
		cout << endl;

		cout << "The corresponding Moisture Force and Moment Resultants are:" << endl;
		cout << "  Nxm = " << Nxm << endl;
		cout << "  Nym = " << Nym << endl;
		cout << "  Nxym = " << Nxym << endl;
		cout << "  Mxm = " << Mxm << endl;
		cout << "  Mym = " << Mym << endl;
		cout << "  Mxym = " << Mxym << endl;
		cout << endl;

		//Print Global and Principal Stresses and Strains

		cout << "The strains in the x-y coordinate system:" << endl;
		cout << "	Ply		z		epsilon_x	epsilon_y	gamma_xy" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << A1[a][0];
			std::cout << "	" << A1[a][1] << "	" << A1[a][2] << "	" << A1[a][3] << "	" << A1[a][4] << endl;
			cout << "	" << A2[a][0];
			std::cout << "	" << A2[a][1] << "	" << A2[a][2] << "	" << A2[a][3] << "	" << A2[a][4] << endl;
		}
		cout << endl;

		cout << "The stresses in the x-y coordinate system:" << endl;
		cout << "	Ply		z		sigma_x		sigma_y		tau_xy" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << C1[a][0];
			std::cout << "	" << C1[a][1] << "	" << C1[a][2] << "	" << C1[a][3] << "	" << C1[a][4] << endl;
			cout << "	" << C2[a][0];
			std::cout << "	" << C2[a][1] << "	" << C2[a][2] << "	" << C2[a][3] << "	" << C2[a][4] << endl;
		}
		cout << endl;

		cout << "The strains in the 1-2 coordinate system:" << endl;
		cout << "	Ply		z		epsilon_1	epsilon_2	gamma_12" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << D1[a][0];
			std::cout << "	" << D1[a][1] << "	" << D1[a][2] << "	" << D1[a][3] << "	" << D1[a][4] << endl;
			cout << "	" << D2[a][0];
			std::cout << "	" << D2[a][1] << "	" << D2[a][2] << "	" << D2[a][3] << "	" << D2[a][4] << endl;
		}
		cout << endl;

		cout << "The stresses in the 1-2 coordinate system:" << endl;
		cout << "	Ply		z		sigma_1		sigma_2		tau_12" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << F1[a][0];
			std::cout << "	" << F1[a][1] << "	" << F1[a][2] << "	" << F1[a][3] << "	" << F1[a][4] << endl;
			cout << "	" << F2[a][0];
			std::cout << "	" << F2[a][1] << "	" << F2[a][2] << "	" << F2[a][3] << "	" << F2[a][4] << endl;
		}
		cout << endl;
		
	}
	else if (path1 == 2) {
		//Prompt user for force and moment resultants
		cout << "Input the Force Resultants:" << endl;
		cout << "  Nx: ";
		cin >> Nx;
		cout << "  Ny: ";
		cin >> Ny;
		cout << "  Nxy: ";
		cin >> Nxy;

		cout << "Input the Moment Resultants:" << endl;
		cout << "  Mx: ";
		cin >> Mx;
		cout << "  My: ";
		cin >> My;
		cout << "  Mxy: ";
		cin >> Mxy;
		cout << endl;
		
		for (int c = 1; c < b; c++) {

			//Define transform matrix

			T[0][0] = pow(m[c], 2);
			T[0][1] = pow(n[c], 2);
			T[0][2] = 2 * m[c] * n[c];
			T[1][0] = pow(n[c], 2);
			T[1][1] = pow(m[c], 2);
			T[1][2] = -2 * m[c] * n[c];
			T[2][0] = -m[c] * n[c];
			T[2][1] = m[c] * n[c];
			T[2][2] = pow(m[c], 2) - pow(n[c], 2);

			//Call stress/strain relations matrix functions

			rstiffness(RSM, E1[c], E2[c], G12[c], v12[c]);
			rcompliance(RCM, E1[c], E2[c], G12[c], v12[c]);
			trstiffness(TRSM, E1[c], E2[c], G12[c], v12[c], m[c], n[c]);
			trcompliance(TRCM, E1[c], E2[c], G12[c], v12[c], m[c], n[c]);

			//Calculate unit thermal stress resultants 

			Nxth = Nxth + (TRSM[0][0] * ax[c] + TRSM[0][1] * ay[c] + TRSM[0][2] * axy[c]) * (z2[c] - z1[c]);
			Mxth = Mxth + 0.5 * (TRSM[0][0] * ax[c] + TRSM[0][1] * ay[c] + TRSM[0][2] * axy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nyth = Nyth + (TRSM[1][0] * ax[c] + TRSM[1][1] * ay[c] + TRSM[1][2] * axy[c]) * (z2[c] - z1[c]);
			Myth = Myth + 0.5 * (TRSM[1][0] * ax[c] + TRSM[1][1] * ay[c] + TRSM[1][2] * axy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nxyth = Nxyth + (TRSM[2][0] * ax[c] + TRSM[2][1] * ay[c] + TRSM[2][2] * axy[c]) * (z2[c] - z1[c]);
			Mxyth = Mxyth + 0.5 * (TRSM[2][0] * ax[c] + TRSM[2][1] * ay[c] + TRSM[2][2] * axy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);

			//Calculate unit moisture stress resultants

			Nxmh = Nxmh + (TRSM[0][0] * Bx[c] + TRSM[0][1] * By[c] + TRSM[0][2] * Bxy[c]) * (z2[c] - z1[c]);
			Mxmh = Mxmh + 0.5 * (TRSM[0][0] * Bx[c] + TRSM[0][1] * By[c] + TRSM[0][2] * Bxy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nymh = Nymh + (TRSM[1][0] * Bx[c] + TRSM[1][1] * By[c] + TRSM[1][2] * Bxy[c]) * (z2[c] - z1[c]);
			Mymh = Mymh + 0.5 * (TRSM[1][0] * Bx[c] + TRSM[1][1] * By[c] + TRSM[1][2] * Bxy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);
			Nxymh = Nxymh + (TRSM[2][0] * Bx[c] + TRSM[2][1] * By[c] + TRSM[2][2] * Bxy[c]) * (z2[c] - z1[c]);
			Mxymh = Mxymh + 0.5 * (TRSM[2][0] * Bx[c] + TRSM[2][1] * By[c] + TRSM[2][2] * Bxy[c]) * (z2[c] * z2[c] - z1[c] * z1[c]);

		}
		
		//Print unit force and moment resultants

		cout << "The Unit Thermal Force and Moment Resultants are:" << endl;
		cout << "  Nxth = " << Nxth << endl;
		cout << "  Nyth = " << Nyth << endl;
		cout << "  Nxyth = " << Nxyth << endl;
		cout << "  Mxth = " << Mxth << endl;
		cout << "  Myth = " << Myth << endl;
		cout << "  Mxyth = " << Mxyth << endl;
		cout << endl;

		cout << "The Unit Moisture Force and Moment Resultants are:" << endl;
		cout << "  Nxmh = " << Nxmh << endl;
		cout << "  Nymh = " << Nymh << endl;
		cout << "  Nxymh = " << Nxymh << endl;
		cout << "  Mxmh = " << Mxmh << endl;
		cout << "  Mymh = " << Mymh << endl;
		cout << "  Mxymh = " << Mxymh << endl;
		cout << endl;
		
		//Calcluate thermal force and moment resultants

		Nxt = Nxth * dT;
		Nyt = Nyth * dT;
		Nxyt = Nxyth * dT;
		Mxt = Mxth * dT;
		Myt = Myth * dT;
		Mxyt = Mxyth * dT;

		//Calcluate moisture force and moment resultants

		Nxm = Nxmh * dM;
		Nym = Nymh * dM;
		Nxym = Nxymh * dM;
		Mxm = Mxmh * dM;
		Mym = Mymh * dM;
		Mxym = Mxymh * dM;
		
		//Inverse ABD (abd) relations

		for (int i = 0; i < 7; i++) {
			//Calculate force and moment resultants
			MSC[i] = abd[i][0] * (Nx + Nxt + Nxm) + abd[i][1] * (Ny + Nyt + Nym) + abd[i][2] * (Nxy + Nxyt + Nxym) + abd[i][3] * (Mx + Mxt + Mxm) + abd[i][4] * (My + Myt + Mym) + abd[i][5] * (Mxy + Mxyt + Mxym);
		}

		Ep_x_o = MSC[0];
		Ep_y_o = MSC[1];
		Gam_xy_o = MSC[2];
		K_x_o = MSC[3];
		K_y_o = MSC[4];
		K_xy_o = MSC[5];

		//Print Thermal and Moisture force and moment resultants

		cout << "The Thermal Force and Moment Resultants are:" << endl;
		cout << "  Nxt = " << Nxt << endl;
		cout << "  Nyt = " << Nyt << endl;
		cout << "  Nxyt = " << Nxyt << endl;
		cout << "  Mxt = " << Mxt << endl;
		cout << "  Myt = " << Myt << endl;
		cout << "  Mxyt = " << Mxyt << endl;
		cout << endl;

		cout << "The Moisture Force and Moment Resultants are:" << endl;
		cout << "  Nxm = " << Nxm << endl;
		cout << "  Nym = " << Nym << endl;
		cout << "  Nxym = " << Nxym << endl;
		cout << "  Mxm = " << Mxm << endl;
		cout << "  Mym = " << Mym << endl;
		cout << "  Mxym = " << Mxym << endl;
		cout << endl;

		//Print midsurface strains and curvatures

		cout << "The corresponding Midsurface Strains and Curvatures are:" << endl;
		cout << "  Epsilon_x_o: " << MSC[0] << endl;
		cout << "  Epsilon_y_o: " << MSC[1] << endl;
		cout << "  Gamma_xy_o: " << MSC[2] << endl;
		cout << "  Kappa_x_o: " << MSC[3] << endl;
		cout << "  Kappa_y_o: " << MSC[4] << endl;
		cout << "  Kappa_xy_o: " << MSC[5] << endl;
		cout << endl;

		//Kirchhoff Hypothesis
		
		for (int c = 1; c < b; c++) {
			
			//Define transform matrix

			T[0][0] = pow(m[c], 2);
			T[0][1] = pow(n[c], 2);
			T[0][2] = 2 * m[c] * n[c];
			T[1][0] = pow(n[c], 2);
			T[1][1] = pow(m[c], 2);
			T[1][2] = -2 * m[c] * n[c];
			T[2][0] = -m[c] * n[c];
			T[2][1] = m[c] * n[c];
			T[2][2] = pow(m[c], 2) - pow(n[c], 2);

			//Call stress/strain relations matrix functions

			rstiffness(RSM, E1[c], E2[c], G12[c], v12[c]);
			rcompliance(RCM, E1[c], E2[c], G12[c], v12[c]);
			trstiffness(TRSM, E1[c], E2[c], G12[c], v12[c], m[c], n[c]);
			trcompliance(TRCM, E1[c], E2[c], G12[c], v12[c], m[c], n[c]);

			//Calculate global strains 

			Ep_x1 = Ep_x_o + (z1[c] * K_x_o);
			Ep_x2 = Ep_x_o + (z2[c] * K_x_o);
			Ep_y1 = Ep_y_o + (z1[c] * K_y_o);
			Ep_y2 = Ep_y_o + (z2[c] * K_y_o);
			Gam_xy1 = Gam_xy_o + (z1[c] * K_xy_o);
			Gam_xy2 = Gam_xy_o + (z2[c] * K_xy_o);

			//Calculate global stresses

			Sig_x1 = TRSM[0][0] * (Ep_x1 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[0][1] * (Ep_y1 - (ay[c] * dT) - (By[c] * dM)) + TRSM[0][2] * (Gam_xy1 - (axy[c] * dT) - (Bxy[c] * dM));
			Sig_x2 = TRSM[0][0] * (Ep_x2 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[0][1] * (Ep_y2 - (ay[c] * dT) - (By[c] * dM)) + TRSM[0][2] * (Gam_xy2 - (axy[c] * dT) - (Bxy[c] * dM));
			Sig_y1 = TRSM[1][0] * (Ep_x1 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[1][1] * (Ep_y1 - (ay[c] * dT) - (By[c] * dM)) + TRSM[1][2] * (Gam_xy1 - (axy[c] * dT) - (Bxy[c] * dM));
			Sig_y2 = TRSM[1][0] * (Ep_x2 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[1][1] * (Ep_y2 - (ay[c] * dT) - (By[c] * dM)) + TRSM[1][2] * (Gam_xy2 - (axy[c] * dT) - (Bxy[c] * dM));
			t_xy1 = TRSM[2][0] * (Ep_x1 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[2][1] * (Ep_y1 - (ay[c] * dT) - (By[c] * dM)) + TRSM[2][2] * (Gam_xy1 - (axy[c] * dT) - (Bxy[c] * dM));
			t_xy2 = TRSM[2][0] * (Ep_x2 - (ax[c] * dT) - (Bx[c] * dM)) + TRSM[2][1] * (Ep_y2 - (ay[c] * dT) - (By[c] * dM)) + TRSM[2][2] * (Gam_xy2 - (axy[c] * dT) - (Bxy[c] * dM));

			//Calculate principal strains

			Ep1_1 =(T[0][0] * Ep_x1 + T[0][1] * Ep_y1 + T[0][2] * 0.5 * Gam_xy1);
			Ep1_2 = (T[0][0] * Ep_x2 + T[0][1] * Ep_y2 + T[0][2] * 0.5 * Gam_xy2);
			Ep2_1 = (T[1][0] * Ep_x1 + T[1][1] * Ep_y1 + T[1][2] * 0.5 * Gam_xy1);
			Ep2_2 = (T[1][0] * Ep_x2 + T[1][1] * Ep_y2 + T[1][2] * 0.5 * Gam_xy2);
			Gam12_1 = 2.0 * (T[2][0] * Ep_x1 + T[2][1] * Ep_y1 + T[2][2] * 0.5 * Gam_xy1);
			Gam12_2 = 2.0 * (T[2][0] * Ep_x2 + T[2][1] * Ep_y2 + T[2][2] * 0.5 * Gam_xy2);

			//Calculate principal stresses

			Sig1_1 = T[0][0] * Sig_x1 + T[0][1] * Sig_y1 + T[0][2] * t_xy1;
			Sig1_2 = T[0][0] * Sig_x2 + T[0][1] * Sig_y2 + T[0][2] * t_xy2;
			Sig2_1 = T[1][0] * Sig_x1 + T[1][1] * Sig_y1 + T[1][2] * t_xy1;
			Sig2_2 = T[1][0] * Sig_x2 + T[1][1] * Sig_y2 + T[1][2] * t_xy2;
			t12_1 = T[2][0] * Sig_x1 + T[2][1] * Sig_y1 + T[2][2] * t_xy1;
			t12_2 = T[2][0] * Sig_x2 + T[2][1] * Sig_y2 + T[2][2] * t_xy2;

			//Store data for table of values

			//Global Strains

			A1[c][0] = c;
			A2[c][0] = c;
			A1[c][1] = z1[c];
			A2[c][1] = z2[c];
			A1[c][2] = Ep_x1;
			A2[c][2] = Ep_x2;
			A1[c][3] = Ep_y1;
			A2[c][3] = Ep_y2;
			A1[c][4] = Gam_xy1;
			A2[c][4] = Gam_xy2;

			//Global Stresses

			C1[c][0] = c;
			C2[c][0] = c;
			C1[c][1] = z1[c];
			C2[c][1] = z2[c];
			C1[c][2] = Sig_x1;
			C2[c][2] = Sig_x2;
			C1[c][3] = Sig_y1;
			C2[c][3] = Sig_y2;
			C1[c][4] = t_xy1;
			C2[c][4] = t_xy2;

			//Principal Strains

			D1[c][0] = c;
			D2[c][0] = c;
			D1[c][1] = z1[c];
			D2[c][1] = z2[c];
			D1[c][2] = Ep1_1;
			D2[c][2] = Ep1_2;
			D1[c][3] = Ep2_1;
			D2[c][3] = Ep2_2;
			D1[c][4] = Gam12_1;
			D2[c][4] = Gam12_2;

			//Principal Stresses

			F1[c][0] = c;
			F2[c][0] = c;
			F1[c][1] = z1[c];
			F2[c][1] = z2[c];
			F1[c][2] = Sig1_1;
			F2[c][2] = Sig1_2;
			F1[c][3] = Sig2_1;
			F2[c][3] = Sig2_2;
			F1[c][4] = t12_1;
			F2[c][4] = t12_2;
		}
		
		//Print Global and Principal Stresses and Strains

		cout << "The strains in the x-y coordinate system:" << endl;
		cout << "	Ply		z		epsilon_x	epsilon_y	gamma_xy" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << A1[a][0];
			std::cout << "	" << A1[a][1] << "	" << A1[a][2] << "	" << A1[a][3] << "	" << A1[a][4] << endl;
			cout << "	" << A2[a][0];
			std::cout << "	" << A2[a][1] << "	" << A2[a][2] << "	" << A2[a][3] << "	" << A2[a][4] << endl;
		}
		cout << endl;

		cout << "The stresses in the x-y coordinate system:" << endl;
		cout << "	Ply		z		sigma_x		sigma_y		tau_xy" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << C1[a][0];
			std::cout << "	" << C1[a][1] << "	" << C1[a][2] << "	" << C1[a][3] << "	" << C1[a][4] << endl;
			cout << "	" << C2[a][0];
			std::cout << "	" << C2[a][1] << "	" << C2[a][2] << "	" << C2[a][3] << "	" << C2[a][4] << endl;
		}
		cout << endl;

		cout << "The strains in the 1-2 coordinate system:" << endl;
		cout << "	Ply		z		epsilon_1	epsilon_2	gamma_12" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << D1[a][0];
			std::cout << "	" << D1[a][1] << "	" << D1[a][2] << "	" << D1[a][3] << "	" << D1[a][4] << endl;
			cout << "	" << D2[a][0];
			std::cout << "	" << D2[a][1] << "	" << D2[a][2] << "	" << D2[a][3] << "	" << D2[a][4] << endl;
		}
		cout << endl;

		cout << "The stresses in the 1-2 coordinate system:" << endl;
		cout << "	Ply		z		sigma_1		sigma_2		tau_12" << endl;
		for (int a = 1; a < b; a++) {
			cout << "	" << F1[a][0];
			std::cout << "	" << F1[a][1] << "	" << F1[a][2] << "	" << F1[a][3] << "	" << F1[a][4] << endl;
			cout << "	" << F2[a][0];
			std::cout << "	" << F2[a][1] << "	" << F2[a][2] << "	" << F2[a][3] << "	" << F2[a][4] << endl;
		}
		cout << endl;
			
	}	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

