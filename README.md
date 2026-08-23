# OmniKit

```
OmniKit/
├── CMakeLists.txt
├── README.md
|
├── include/
│   └── omnikit/
│       │
│       ├── core/
│       │   ├── types.h         # omk_real_t, ortak struct'lar
│       │   ├── error.h         # hata kodları
│       │   └── version.h
│       │
│       ├── math/
│       │   ├── scalar.h        # clamp, lerp, wrap_angle, sign, vs.
│       │   ├── vector.h
│       │   ├── matrix.h
│       │   └── quaternion.h
│       │
│       ├── time/
│       │   ├── clock.h         # platforma özgü zaman okuma (port edilecek tek yer)
│       │   ├── timer.h         # periyodik görev / kronometre
│       │   └── delay.h
│       │
│       ├── geometry/
│       │   ├── rotation.h      # euler <-> quaternion <-> matrix
│       │   ├── transform.h     # homojen dönüşüm / pose
│       │   └── frame.h         # referans çerçeveler arası dönüşüm
│       │
│       ├── control/
│       │   ├── controller_base.h  # ortak arayüz: init/update/reset
│       │   ├── pid.h
│       │   ├── lqr.h
│       │   └── state_space.h      # A,B,C,D modeli, gözlemci
│       │
│       └── filter/
│           ├── low_pass.h
│           ├── moving_average.h
│           ├── median.h
│           └── kalman.h
│
├── src/                        # include/ ile birebir aynı klasör ağacı, .c/.cpp dosyaları
│   ├── core/...
│   ├── math/...
│   ├── time/...
│   ├── geometry/...
│   ├── control/...
│   └── filter/...
│
├── tests/
│   ├── test_pid.c
│   ├── test_kalman.c
│   └── ...
│
├── examples/
│   ├── pid_motor_control/
│   └── imu_orientation_filter/
│
└── docs/
    └── api/
```

```
make lint-module MODULE=progress
```