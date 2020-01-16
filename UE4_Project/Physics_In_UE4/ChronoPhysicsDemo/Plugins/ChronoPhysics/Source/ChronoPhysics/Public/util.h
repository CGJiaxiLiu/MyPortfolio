#pragma once

#define CHRONO_SCALE 100.0f

#define CHRONO_VEC_TO_FVECTOR(chvector) FVector(chvector[0] * CHRONO_SCALE, chvector[2] * CHRONO_SCALE, chvector[1] * CHRONO_SCALE)
#define FVECTOR_TO_CHRONO_VEC(fvector) chrono::ChVector<>(fvector.X /CHRONO_SCALE, fvector.Z / CHRONO_SCALE, fvector.Y /CHRONO_SCALE)
#define CHRONO_QUAT_TO_FQUAT(chquat) FQuat(chquat[1], chquat[3], chquat[2], -chquat[0])
#define FQUAT_TO_CHRONO_QUAT(fquat) chrono::ChQuaternion<>(-fquat.W, fquat.X, fquat.Z, fquat.Y)

//#define CHRONO_VEC_TO_FVECTOR(chvector) FVector(chvector[0] * 1000.f, chvector[2] * 1000.f, chvector[1] * 1000.f)
//#define FVECTOR_TO_CHRONO_VEC(fvector) chrono::ChVector<>(fvector.X /1000.f, fvector.Z / 1000.f, fvector.Y /1000.f)
//#define CHRONO_QUAT_TO_FQUAT(chquat) FQuat(chquat[1], chquat[3], chquat[2], -chquat[0])
//#define FQUAT_TO_CHRONO_QUAT(fquat) chrono::ChQuaternion<>(-fquat.W, fquat.X, fquat.Z, fquat.Y)