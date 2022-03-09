#include <iostream>
#include <Eigen/Dense>


using namespace Eigen;
using namespace std;

extern "C" void dggev_(const char* JOBVL, const char* JOBVR, const int* N,
        const double* A, const int* LDA, const double* B, const int* LDB,
        double* ALPHAR, double* ALPHAI, double* BETA,
        double* VL, const int* LDVL, double* VR, const int* LDVR,
        double* WORK, const int* LWORK, int* INFO);

// Generalised Eigen-Problem
// source adapted from https://eigen.tuxfamily.org/index.php?title=Lapack
// Solve:
// A * v(j) = lambda(j) * B * v(j).
//
// v are the eigenvectors and are stored in v.
// lambda are the eigenvalues and are stored in lambda.
// The eigenvalues are stored as: (lambda(:, 1) + lambda(:, 2)*i)./lambda(:, 3)
//
// returns true on success.
// A and B will be changed.
bool GEP(MatrixXd& A, MatrixXd& B, MatrixXd& v, MatrixXd& lambda)
{
    int N = A.cols(); // Number of columns of A and B. Number of rows of v.
    if (B.cols() != N  || A.rows()!=N || B.rows()!=N){
        cout << "Matrices A and B are not square and the same size, line " << __LINE__ <<  endl;
        return false;
    }

    v.resize(N,N);
    lambda.resize(N, 3);

    int LDA = A.outerStride();
    int LDB = B.outerStride();
    int LDV = v.outerStride();

    double WORKDUMMY;
    int LWORK = -1; // Request optimum work size.
    int INFO = 0;

//    double * alphar = const_cast<double*>(lambda.col(0).data());
//    double * alphai = const_cast<double*>(lambda.col(1).data());
//    double * beta   = const_cast<double*>(lambda.col(2).data());

    double * alphar = lambda.col(0).data();
    double * alphai = lambda.col(1).data();
    double * beta   = lambda.col(2).data();

    // Get the optimum work size.
    dggev_("N", "V", &N, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, &WORKDUMMY, &LWORK, &INFO);

    LWORK = int(WORKDUMMY) + 32;
    VectorXd WORK(LWORK);

    dggev_("N", "V", &N, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, WORK.data(), &LWORK, &INFO);

    return INFO==0;
}

int main(){

    MatrixXd A;
    MatrixXd B;
    MatrixXd v;
    MatrixXd lambda;

    A.setRandom(4, 4);
    B.setRandom(4, 4);

    //A(1, 1) = 2;

    cout << "Before calling the GEP function. " << endl;
    cout << "Matrix A " << endl << A << endl;
    cout << "Matrix B " << endl << B << endl;

    bool success = GEP(A, B, v, lambda);

    if (success){
        cout << "A " << endl << A << endl;
        cout << "B" << endl << B << endl;

        cout << "lambda " << endl << lambda << endl;
        cout << "v " << endl << v << endl;

    }   else {
        cout << "GEP failed." << endl;
    }


    return 0;
}
