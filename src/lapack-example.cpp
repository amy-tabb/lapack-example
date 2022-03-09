#include <iostream>

#include <chrono>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <Eigen/Geometry>
#include <Eigen/QR>

using namespace Eigen;
using namespace std;

extern "C" void dggev_(const char* JOBVL, const char* JOBVR, const int* N,
                       const double* A, const int* LDA, const double* B, const int* LDB,
                       double* ALPHAR, double* ALPHAI, double* BETA,
                       double* VL, const int* LDVL, double* VR, const int* LDVR,
                       double* WORK, const int* LWORK, int* INFO);

// Generalised Eigen-Problem
// Solve:
// A * v(j) = lambda(j) * B * v(j).
//
// v are the eigenvectors and are stored in v.
// lambda are the eigenvalues and are stored in lambda.
// The eigenvalues are stored as: (lambda(:, 1) + lambda(:, 2)*i)./lambda(:, 3)
//
// returns true on success.
bool GEP(const MatrixXd& A, const MatrixXd& B, MatrixXd& v, MatrixXd& lambda)
{
  int N = A.cols(); // Number of columns of A and B. Number of rows of v.
  if (B.cols() != N  || A.rows()!=N || B.rows()!=N)
    return false;

  v.resize(N,N);
  lambda.resize(N, 3);

  int LDA = A.outerStride();
  int LDB = B.outerStride();
  int LDV = v.outerStride();

  double WORKDUMMY;
  int LWORK = -1; // Request optimum work size.
  int INFO = 0;
  
  double * alphar = const_cast<double*>(lambda.col(0).data());
  double * alphai = const_cast<double*>(lambda.col(1).data());
  double * beta   = const_cast<double*>(lambda.col(2).data());

  // Get the optimum work size.
  dggev_("N", "V", &N, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, &WORKDUMMY, &LWORK, &INFO);

  LWORK = int(WORKDUMMY) + 32;
  VectorXd WORK(LWORK);

  dggev_("N", "V", &N, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, WORK.data(), &LWORK, &INFO);

  return INFO==0;
}

int main(){


    return 0;
}
