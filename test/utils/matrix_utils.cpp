#include <catch/catch.hpp>

#include "utils.h"

using namespace faasm;

namespace tests {

    void checkSparseMatrixEquality(const SparseMatrix<double> &a, const SparseMatrix<double> &b) {
        // As we can't use a simple equality operator, we need to iterate through both
        // matrices and check the contents in detail

        REQUIRE(a.nonZeros() == b.nonZeros());
        REQUIRE(a.cols() == b.cols());
        REQUIRE(a.rows() == b.rows());

        std::vector<int> rowsA;
        std::vector<int> colsA;
        std::vector<double> valuesA;

        for (int k = 0; k < a.outerSize(); ++k) {
            for (SparseMatrix<double>::InnerIterator it(a, k); it; ++it) {
                valuesA.push_back(it.value());
                rowsA.push_back(it.row());
                colsA.push_back(it.col());
            }
        }

        std::vector<int> rowsB;
        std::vector<int> colsB;
        std::vector<double> valuesB;

        for (int k = 0; k < b.outerSize(); ++k) {
            for (SparseMatrix<double>::InnerIterator it(b, k); it; ++it) {
                valuesB.push_back(it.value());
                rowsB.push_back(it.row());
                colsB.push_back(it.col());
            }
        }

        REQUIRE(rowsA == rowsB);
        REQUIRE(colsA == colsB);
        REQUIRE(valuesA == valuesB);
    }

    double doSgdStep(FaasmMemory *mem, SgdParams &params, int epoch, SparseMatrix<double> &inputs, MatrixXd &outputs) {
        // Shuffle indices
        int *batchStartIndices = randomIntRange(params.nBatches);

        // Prepare update loop
        int batchSize = params.nTrain / params.nBatches;
        MatrixXd weights = readMatrixFromState(mem, WEIGHTS_KEY, 1, params.nWeights);

        // Perform batch updates to weights
        for (int b = 0; b < params.nBatches; b++) {
            int startCol = batchStartIndices[b];

            SparseMatrix<double> inputBatch = inputs.block(0, startCol, params.nWeights, batchSize);
            MatrixXd outputBatch = outputs.block(0, startCol, 1, batchSize);

            // Perform the update
            if(params.lossType == RMSE) {
                leastSquaresWeightUpdate(mem, params, weights, inputBatch, outputBatch);
            }
            else if(params.lossType == HINGE) {
                hingeLossWeightUpdate(mem, params, epoch, weights, inputBatch, outputBatch);
            }

            // Update parameters
            weights = readMatrixFromState(mem, WEIGHTS_KEY, 1, params.nWeights);
        }

        // Calculate the actual values
        MatrixXd actual = weights * inputs;

        double loss = 0;
        if(params.lossType == RMSE) {
            loss = calculateRootMeanSquaredError(actual, outputs);
        }
        else if(params.lossType == HINGE) {
            loss = calculateHingeError(actual, outputs);
        }

        return loss;
    }

}