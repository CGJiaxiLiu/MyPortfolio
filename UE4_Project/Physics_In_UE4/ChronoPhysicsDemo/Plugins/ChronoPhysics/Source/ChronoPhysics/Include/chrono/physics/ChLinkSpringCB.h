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
// Authors: Radu Serban
// =============================================================================

#ifndef CH_LINK_SPRING_CB_H
#define CH_LINK_SPRING_CB_H

#include "chrono/physics/ChLinkMarkers.h"

namespace chrono {

/// Class for spring-damper systems with the force specified through a functor object.
class ChApi ChLinkSpringCB : public ChLinkMarkers {

  public:
    ChLinkSpringCB();
    ChLinkSpringCB(const ChLinkSpringCB& other);
    virtual ~ChLinkSpringCB() {}

    /// "Virtual" copy constructor (covariant return type).
    virtual ChLinkSpringCB* Clone() const override { return new ChLinkSpringCB(*this); }

    // data fetch/store
    double GetSpringRestLength() const { return m_rest_length; }
    double GetSpringDeform() const { return dist - m_rest_length; }
    double GetSpringLength() const { return dist; }
    double GetSpringVelocity() const { return dist_dt; }
    double GetSpringReact() const { return m_force; }

    void SetSpringRestLength(double len) { m_rest_length = len; }

    /// Class to be used as a functor interface for calculating the general spring-damper force.
    /// A derived class must implement the virtual operator().
    class ChApi ForceFunctor {
      public:
        virtual ~ForceFunctor() {}

        /// Calculate and return the general spring-damper force at the specified configuration.
        virtual double operator()(double time,          ///< current time
                                  double rest_length,   ///< undeformed length
                                  double length,        ///< current length
                                  double vel,           ///< current velocity (positive when extending)
                                  ChLinkSpringCB* link  ///< back-pointer to associated link
                                  ) = 0;
    };

    /// Specify the functor object for calculating the force.
    void RegisterForceFunctor(ForceFunctor* functor) { m_force_fun = functor; }

    /// Specialized initialization for springs, given the two bodies to be connected,
    /// the positions of the two anchor endpoints of the spring (each expressed
    /// in body or abs. coordinates) and the imposed rest length of the spring.
    /// NOTE! As in ChLinkMarkers::Initialize(), the two markers are automatically
    /// created and placed inside the two connected bodies.
    void Initialize(
        std::shared_ptr<ChBody> body1,  ///< first body to link
        std::shared_ptr<ChBody> body2,  ///< second body to link
        bool pos_are_relative,          ///< true: following pos. are relative to bodies
        ChVector<> pos1,                ///< pos. of spring endpoint for 1st body (rel. or abs., see flag above)
        ChVector<> pos2,                ///< pos. of spring endpoint for 2nd body (rel. or abs., see flag above)
        bool auto_rest_length = true,   ///< if true, initializes the rest length as the distance between pos1 and pos2
        double rest_length = 0          ///< rest length (no need to define if auto_rest_length=true.)
        );

    /// Get the 1st spring endpoint (expressed in Body1 coordinate system)
    ChVector<> GetEndPoint1Rel() { return marker1->GetPos(); }
    /// Set the 1st spring endpoint (expressed in Body1 coordinate system)
    void SetEndPoint1Rel(const ChVector<>& mset) { marker1->Impose_Rel_Coord(ChCoordsys<>(mset, QUNIT)); }
    /// Get the 1st spring endpoint (expressed in absolute coordinate system)
    ChVector<> GetEndPoint1Abs() { return marker1->GetAbsCoord().pos; }
    /// Set the 1st spring endpoint (expressed in absolute coordinate system)
    void SetEndPoint1Abs(ChVector<>& mset) { marker1->Impose_Abs_Coord(ChCoordsys<>(mset, QUNIT)); }

    /// Get the 2nd spring endpoint (expressed in Body2 coordinate system)
    ChVector<> GetEndPoint2Rel() { return marker2->GetPos(); };
    /// Set the 2nd spring endpoint (expressed in Body2 coordinate system)
    void SetEndPoint2Rel(const ChVector<>& mset) { marker2->Impose_Rel_Coord(ChCoordsys<>(mset, QUNIT)); }
    /// Get the 1st spring endpoint (expressed in absolute coordinate system)
    ChVector<> GetEndPoint2Abs() { return marker2->GetAbsCoord().pos; }
    /// Set the 1st spring endpoint (expressed in absolute coordinate system)
    void SetEndPoint2Abs(ChVector<>& mset) { marker2->Impose_Abs_Coord(ChCoordsys<>(mset, QUNIT)); }

    /// Inherits, then also adds the spring custom forces to the C_force and C_torque.
    virtual void UpdateForces(double time) override;

    /// Method to allow serialization of transient data to archives.
    virtual void ArchiveOUT(ChArchiveOut& marchive) override;

    /// Method to allow deserialization of transient data from archives.
    virtual void ArchiveIN(ChArchiveIn& marchive) override;

  protected:
    ForceFunctor* m_force_fun;  ///< functor for force calculation
    double m_rest_length;       ///< undeform length
    double m_force;             ///< resulting force in dist. coord
};

CH_CLASS_VERSION(ChLinkSpringCB, 0)

}  // end namespace chrono

#endif
