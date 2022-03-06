#include <iostream>
#include "Eigen/Eigen"
#include "Eigen/IterativeLinearSolvers"

#include <ZuoGuangCFD/finite_volume_method/implicit_calculus.h>
#include <ZuoGuangCFD/solver/solver.h>
#include <ZuoGuangCFD/struct/zg_field.h>
#include <ZuoGuangCFD/mesh/of_mesh.h>
#include "ZuoGuangCFD/struct/fvm_matrix.h"

using namespace zgCFD;
void a(int& b) {
    int* p;
    p = &b;
    *p = 3;
}

void printVector(std::vector<int>& v) {

    for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main() {
    
    OfMesh mesh("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\explicit_calculus_test\\constant\\polyMesh");

    ScalarVolumeField s(mesh, "tmp", "tmp");

    FvmScalarMatrix seqn;

    seqn.SetVarible(&s);

    seqn.matrix_.SetRow(9);
    seqn.matrix_.SetColumn(9);

    seqn.matrix_.InsertValue(1-1, 1-1, 0.005);
    seqn.matrix_.InsertValue(1-1, 2-1, -0.002);
    seqn.matrix_.InsertValue(1-1, 4-1, -0.001);
    seqn.matrix_.InsertValue(2-1, 2-1, 240.002);
    seqn.matrix_.InsertValue(2-1, 3-1, -40);
    seqn.matrix_.InsertValue(2-1, 1-1, -0.002);
    seqn.matrix_.InsertValue(2-1, 5-1, -200);
    seqn.matrix_.InsertValue(3-1, 3-1, 340);
    seqn.matrix_.InsertValue(3-1, 2-1, -40);
    seqn.matrix_.InsertValue(3-1, 6-1, -300);
    seqn.matrix_.InsertValue(4-1, 4-1, 0.006);
    seqn.matrix_.InsertValue(4-1, 5-1, -0.002);
    seqn.matrix_.InsertValue(4-1, 7-1, -0.001);
    seqn.matrix_.InsertValue(4-1, 1-1, -0.001);
    seqn.matrix_.InsertValue(5-1, 5-1, 440.002);
    seqn.matrix_.InsertValue(5-1, 6-1, -40);
    seqn.matrix_.InsertValue(5-1, 4-1, -0.002);
    seqn.matrix_.InsertValue(5-1, 8-1, -200);
    seqn.matrix_.InsertValue(5-1, 2-1, -200);
    seqn.matrix_.InsertValue(6-1, 6-1, 640);
    seqn.matrix_.InsertValue(6-1, 5-1, -40);
    seqn.matrix_.InsertValue(6-1, 9-1, -300);
    seqn.matrix_.InsertValue(6-1, 3-1, -300);
    seqn.matrix_.InsertValue(7-1, 7-1, 0.006998);
    seqn.matrix_.InsertValue(7-1, 8-1, -0.002);
    seqn.matrix_.InsertValue(7-1, 4-1, -0.001);
    seqn.matrix_.InsertValue(8-1, 8-1, 243.9624);
    seqn.matrix_.InsertValue(8-1, 9-1, -40);
    seqn.matrix_.InsertValue(8-1, 7-1, -0.002);
    seqn.matrix_.InsertValue(8-1, 5-1, -200);
    seqn.matrix_.InsertValue(9-1, 9-1, 345.9406);
    seqn.matrix_.InsertValue(9-1, 8-1, -40);
    seqn.matrix_.InsertValue(9-1, 6-1, -300);

    seqn.sorce_.push_back(0.64);
    seqn.sorce_.push_back(20);
    seqn.sorce_.push_back(30);
    seqn.sorce_.push_back(0.64);
    seqn.sorce_.push_back(0);
    seqn.sorce_.push_back(0);
    seqn.sorce_.push_back(1.2394);
    seqn.sorce_.push_back(1188.12);
    seqn.sorce_.push_back(1782.18);

    Solver a;
    a.Solve(seqn);

    return 0;

}

