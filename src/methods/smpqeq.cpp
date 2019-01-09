//
// Created by krab1k on 31/10/18.
//

#include <vector>
#include <cmath>
#include <mkl_lapacke.h>
#include <mkl.h>

#include "smpqeq.h"
#include "../parameters.h"
#include "../geometry.h"


#define IDX(i, j) ((i) * m + (j))


std::vector<double> SMP_QEq::calculate_charges(const Molecule &molecule) {

    size_t n = molecule.atoms().size();
    size_t m = n + 1;

    auto *A = (double *) mkl_calloc(m * m, sizeof(double), 64);
    auto *b = (double *) mkl_calloc(m, sizeof(double), 64);
    auto *ipiv = (MKL_INT *) mkl_calloc(m, sizeof(MKL_INT), 64);

    for (int iter = 0; iter < 5; iter++)
    {
        for (size_t i = 0; i < n; i++) {
            const auto &atom_i = molecule.atoms()[i];
            A[IDX(i, i)] = 2 * (parameters_->atom()->parameter(atom::second)(atom_i) +
                                parameters_->atom()->parameter(atom::third)(atom_i) * b[i] +
                                parameters_->atom()->parameter(atom::fourth)(atom_i) * b[i] * b[i]);
            b[i] = -parameters_->atom()->parameter(atom::first)(atom_i);
            for (size_t j = i + 1; j < n; j++) {
                const auto &atom_j = molecule.atoms()[j];
                auto gamma = 2 * std::sqrt(parameters_->atom()->parameter(atom::second)(atom_i) *
                                           parameters_->atom()->parameter(atom::second)(atom_j));
                A[IDX(i, j)] = 1 / std::cbrt(1 / std::pow(gamma, 3) + std::pow(distance(atom_i, atom_j), 3));
            }
        }

        for (size_t i = 0; i < n; i++) {
            A[IDX(i, n)] = 1;
        }

        A[IDX(n, n)] = 0;
        b[n] = molecule.total_charge();

        MKL_INT info = LAPACKE_dsysv(LAPACK_ROW_MAJOR, 'U', m, 1, A, m, ipiv, b, 1);
        if (info) {
            throw std::runtime_error("Cannot solve linear system");
        }
    }
    std::vector<double> results;
    results.assign(b, b + n);

    mkl_free(A);
    mkl_free(b);
    mkl_free(ipiv);

    return results;
}