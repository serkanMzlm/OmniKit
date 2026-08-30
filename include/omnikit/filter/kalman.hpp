#pragma once

#include <Eigen/Dense>

namespace omnikit::filter {

/// @brief A linear discrete-time Kalman filter.
///        Estimates the state of a linear system from noisy measurements,
///        assuming zero-mean Gaussian process and measurement noise.
///
/// State model:      x_k = F x_{k-1} + B u_k + w,   w ~ N(0, Q)
/// Measurement model: z_k = H x_k + v,              v ~ N(0, R)
///
/// @tparam N  state dimension
/// @tparam M  measurement dimension
/// @tparam L  control-input dimension (0 if no control input)
template <int N, int M, int L = 0> class KalmanFilter {
public:
    using StateVec = Eigen::Matrix<double, N, 1>;
    using MeasVec = Eigen::Matrix<double, M, 1>;
    using ControlVec = Eigen::Matrix<double, L, 1>;

    using StateMat = Eigen::Matrix<double, N, N>;
    using MeasMat = Eigen::Matrix<double, M, N>;
    using ControlMat = Eigen::Matrix<double, N, L>;
    using ProcNoise = Eigen::Matrix<double, N, N>;
    using MeasNoise = Eigen::Matrix<double, M, M>;
    using Gain = Eigen::Matrix<double, N, M>;

    KalmanFilter() = default;

    /// @brief Sets the state-transition matrix F (n x n).
    void setStateTransition(const StateMat& F) { F_ = F; }

    /// @brief Sets the control-input matrix B (n x l).
    void setControlMatrix(const ControlMat& B) { B_ = B; }

    /// @brief Sets the measurement matrix H (m x n).
    void setMeasurementMatrix(const MeasMat& H) { H_ = H; }

    /// @brief Sets the process-noise covariance Q (n x n).
    void setProcessNoise(const ProcNoise& Q) { Q_ = Q; }

    /// @brief Sets the measurement-noise covariance R (m x m).
    void setMeasurementNoise(const MeasNoise& R) { R_ = R; }

    /// @brief Initializes the state estimate and its covariance.
    /// @param x0  initial state
    /// @param P0  initial state covariance
    void init(const StateVec& x0, const StateMat& P0) {
        x_ = x0;
        P_ = P0;
    }

    /// @brief Prediction step with no control input.
    void predict();

    /// @brief Prediction step with a control input u (l x 1).
    void predict(const ControlVec& u);

    /// @brief Correction step given a measurement z (m x 1).
    void update(const MeasVec& z);

    /// @brief Convenience: predict then update in one call (no control).
    const StateVec& step(const MeasVec& z) {
        predict();
        update(z);
        return x_;
    }

    const StateVec& state() const { return x_; }
    const StateMat& covariance() const { return P_; }

    /// @brief Last innovation (z - H x). Useful for outlier gating.
    const MeasVec& innovation() const { return y_; }

private:
    // Model matrices.
    StateMat F_ = StateMat::Identity();
    ControlMat B_ = ControlMat::Zero();
    MeasMat H_ = MeasMat::Zero();
    ProcNoise Q_ = ProcNoise::Identity();
    MeasNoise R_ = MeasNoise::Identity();

    // Estimate.
    StateVec x_ = StateVec::Zero();
    StateMat P_ = StateMat::Identity();

    // Cached innovation.
    MeasVec y_ = MeasVec::Zero();
};

} // namespace omnikit::filter