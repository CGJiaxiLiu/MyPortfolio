// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban, Justin Madsen
// =============================================================================
//
// Base class for a wheeled vehicle system.
//
// The reference frame for a vehicle follows the ISO standard: Z-axis up, X-axis
// pointing forward, and Y-axis towards the left of the vehicle.
//
// =============================================================================

#ifndef CH_WHEELED_VEHICLE_H
#define CH_WHEELED_VEHICLE_H

#include <vector>

#include "chrono_vehicle/ChVehicle.h"
#include "chrono_vehicle/wheeled_vehicle/ChAntirollBar.h"
#include "chrono_vehicle/wheeled_vehicle/ChBrake.h"
#include "chrono_vehicle/wheeled_vehicle/ChDriveline.h"
#include "chrono_vehicle/wheeled_vehicle/ChSteering.h"
#include "chrono_vehicle/wheeled_vehicle/ChSuspension.h"
#include "chrono_vehicle/wheeled_vehicle/ChWheel.h"

namespace chrono {
namespace vehicle {

/// @addtogroup vehicle_wheeled
/// @{

/// Base class for chrono wheeled vehicle systems.
/// This class provides the interface between the vehicle system and other
/// systems (tires, driver, etc.).
/// The reference frame for a vehicle follows the ISO standard: Z-axis up, X-axis
/// pointing forward, and Y-axis towards the left of the vehicle.
class CH_VEHICLE_API ChWheeledVehicle : public ChVehicle {
  public:
    /// Construct a vehicle system with a default ChSystem.
    ChWheeledVehicle(const std::string& name,                                                  ///< [in] vehicle name
                     ChMaterialSurface::ContactMethod contact_method = ChMaterialSurface::NSC  ///< [in] contact method
                     );

    /// Construct a vehicle system using the specified ChSystem.
    ChWheeledVehicle(const std::string& name,  ///< [in] vehicle name
                     ChSystem* system          ///< [in] containing mechanical system
                     );

    /// Destructor.
    virtual ~ChWheeledVehicle() {}

    /// Get the name of the vehicle system template.
    virtual std::string GetTemplateName() const override { return "WheeledVehicle"; }

    /// Get the specified suspension subsystem.
    std::shared_ptr<ChSuspension> GetSuspension(int id) const { return m_suspensions[id]; }

    /// Get the specified steering subsystem.
    std::shared_ptr<ChSteering> GetSteering(int id) const { return m_steerings[id]; }

    /// Get a handle to the specified vehicle wheel subsystem.
    std::shared_ptr<ChWheel> GetWheel(const WheelID& wheel_id) const { return m_wheels[wheel_id.id()]; }

    /// Get a handle to the specified vehicle brake subsystem.
    std::shared_ptr<ChBrake> GetBrake(const WheelID& wheel_id) const { return m_brakes[wheel_id.id()]; }

    /// Get a handle to the vehicle's driveline subsystem.
    std::shared_ptr<ChDriveline> GetDriveline() const { return m_driveline; }

    /// Get the vehicle total mass.
    /// This includes the mass of the chassis and all vehicle subsystems, but not the mass of tires.
    virtual double GetVehicleMass() const override;

    /// Get the current global vehicle COM location.
    virtual ChVector<> GetVehicleCOMPos() const override;

    /// Get a handle to the vehicle's driveshaft body.
    virtual std::shared_ptr<ChShaft> GetDriveshaft() const override { return m_driveline->GetDriveshaft(); }

    /// Get the angular speed of the driveshaft.
    /// This function provides the interface between a vehicle system and a
    /// powertrain system.
    virtual double GetDriveshaftSpeed() const override;

    /// Return the number of axles for this vehicle.
    virtual int GetNumberAxles() const = 0;

    /// Get a handle to the specified wheel body.
    std::shared_ptr<ChBody> GetWheelBody(const WheelID& wheelID) const;

    /// Get the global location of the specified wheel.
    const ChVector<>& GetWheelPos(const WheelID& wheel_id) const;

    /// Get the orientation of the specified wheel.
    /// The wheel orientation is returned as a quaternion representing a rotation
    /// with respect to the global reference frame.
    const ChQuaternion<>& GetWheelRot(const WheelID& wheel_id) const;

    /// Get the linear velocity of the specified wheel.
    /// Return the linear velocity of the wheel center, expressed in the global
    /// reference frame.
    const ChVector<>& GetWheelLinVel(const WheelID& wheel_id) const;

    /// Get the angular velocity of the specified wheel.
    /// Return the angular velocity of the wheel frame, expressed in the global
    /// reference frame.
    ChVector<> GetWheelAngVel(const WheelID& wheel_id) const;

    /// Get the angular speed of the specified wheel.
    /// This is the angular speed of the wheel axle.
    double GetWheelOmega(const WheelID& wheel_id) const;

    /// Get the complete state for the specified wheel.
    /// This includes the location, orientation, linear and angular velocities,
    /// all expressed in the global reference frame, as well as the wheel angular
    /// speed about its rotation axis.
    WheelState GetWheelState(const WheelID& wheel_id) const;

    /// Return the vehicle wheelbase.
    virtual double GetWheelbase() const  = 0;

    /// Return the vehicle wheel track of the specified suspension subsystem.
    double GetWheeltrack(int id) const { return m_suspensions[id]->GetTrack(); }

    /// Return the minimum turning radius.
    /// A concrete wheeled vehicle class should override the default value (20 m).
    virtual double GetMinTurningRadius() const { return 20; }

    /// Return the maximum steering angle.
    /// This default implementation estimates the maximum steering angle based on a bicycle model
    /// and the vehicle minimum turning radius.
    virtual double GetMaxSteeringAngle() const;

    /// Set visualization type for the suspension subsystems.
    /// This function should be called only after vehicle initialization.
    void SetSuspensionVisualizationType(VisualizationType vis);

    /// Set visualization type for the steering subsystems.
    /// This function should be called only after vehicle initialization.
    void SetSteeringVisualizationType(VisualizationType vis);

    /// Set visualization type for the wheel subsystems.
    /// This function should be called only after vehicle initialization.
    void SetWheelVisualizationType(VisualizationType vis);

    /// Enable/disable collision between the chassis and all other vehicle subsystems.
    /// This only controls collisions between the chassis and the tire systems.
    virtual void SetChassisVehicleCollide(bool state) override;

    /// Enable/disable output from the suspension subsystems.
    void SetSuspensionOutput(int id, bool state);

    /// Enable/disable output from the steering subsystems.
    void SetSteeringOutput(int id, bool state);

    /// Enable/disable output from the anti-roll bar subsystems.
    void SetAntirollbarOutput(int id, bool state);

    /// Enable/disable output from the driveline subsystem.
    void SetDrivelineOutput(bool state);

    /// Initialize this vehicle at the specified global location and orientation.
    /// This base class implementation only initializes the chassis subsystem.
    /// Derived classes must extend this function to initialize all other wheeled
    /// vehicle subsystems (steering, suspensions, wheels, brakes, and driveline).
    virtual void Initialize(const ChCoordsys<>& chassisPos,  ///< [in] initial global position and orientation
                            double chassisFwdVel = 0         ///< [in] initial chassis forward velocity
                            ) override;

    /// Update the state of this vehicle at the current time.
    /// The vehicle system is provided the current driver inputs (throttle between
    /// 0 and 1, steering between -1 and +1, braking between 0 and 1), the torque
    /// from the powertrain, and tire forces (expressed in the global reference
    /// frame).
    virtual void Synchronize(double time,                      ///< [in] current time
                             double steering,                  ///< [in] current steering input [-1,+1]
                             double braking,                   ///< [in] current braking input [0,1]
                             double powertrain_torque,         ///< [in] input torque from powertrain
                             const TerrainForces& tire_forces  ///< [in] vector of tire force structures
    );

    /// Lock/unlock the differential on the specified axle.
    /// By convention, axles are counted front to back, starting with index 0.
    void LockAxleDifferential(int axle, bool lock);

    /// Lock/unlock the specified central differential.
    /// By convention, central differentials are counted from front to back, starting with index 0 for
    /// the central differential between the two front-most axles.
    void LockCentralDifferential(int which, bool lock);

    /// Log current constraint violations.
    virtual void LogConstraintViolations() override;

    /// Return a JSON string with information on all modeling components in the vehicle system.
    /// These include bodies, shafts, joints, spring-damper elements, markers, etc.
    virtual std::string ExportComponentList() const override;

    /// Write a JSON-format file with information on all modeling components in the vehicle system.
    /// These include bodies, shafts, joints, spring-damper elements, markers, etc.
    virtual void ExportComponentList(const std::string& filename) const override;

    /// Output data for all modeling components in the vehicle system.
    virtual void Output(int frame, ChVehicleOutput& database) const override;

  protected:
    ChSuspensionList m_suspensions;            ///< list of handles to suspension subsystems
    ChAntirollbarList m_antirollbars;          ///< list of handles to antirollbar subsystems (optional)
    std::shared_ptr<ChDriveline> m_driveline;  ///< handle to the driveline subsystem
    ChSteeringList m_steerings;                ///< list of handles to steering subsystems
    ChWheelList m_wheels;                      ///< list of handles to wheel subsystems
    ChBrakeList m_brakes;                      ///< list of handles to brake subsystems
};

/// @} vehicle_wheeled

}  // end namespace vehicle
}  // end namespace chrono

#endif
