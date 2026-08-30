#include "omnikit/filter/kalman.hpp"

namespace omnikit::filter {

template <int N, int M, int L>
void KalmanFilter<N, M, L>::predict() {
    // x⁻ = F x
    x_ = F_ * x_;
    // P⁻ = F P Fᵀ + Q
    P_ = F_ * P_ * F_.transpose() + Q_;
}

template <int N, int M, int L>
void KalmanFilter<N, M, L>::predict(const ControlVec& u) {
    // x⁻ = F x + B u
    x_ = F_ * x_ + B_ * u;
    P_ = F_ * P_ * F_.transpose() + Q_;
}

template <int N, int M, int L>
void KalmanFilter<N, M, L>::update(const MeasVec& z) {
    // Innovation: y = z - H x⁻
    y_ = z - H_ * x_;

    // Innovation covariance: S = H P⁻ Hᵀ + R
    const Eigen::Matrix<double, M, M> S =
        H_ * P_ * H_.transpose() + R_;

    // Kalman gain: K = P⁻ Hᵀ S⁻¹
    const Gain K = P_ * H_.transpose() * S.inverse();

    // Corrected state: x = x⁻ + K y
    x_ = x_ + K * y_;

    // Joseph-form covariance update for numerical stability:
    // P = (I - K H) P⁻ (I - K H)ᵀ + K R Kᵀ
    const StateMat I = StateMat::Identity();
    const StateMat IKH = I - K * H_;
    P_ = IKH * P_ * IKH.transpose() + K * R_ * K.transpose();
}

} // namespace omnikit::filter