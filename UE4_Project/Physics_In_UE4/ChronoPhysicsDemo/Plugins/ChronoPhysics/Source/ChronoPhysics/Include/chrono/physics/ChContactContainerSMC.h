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
// Authors: Alessandro Tasora, Radu Serban
// =============================================================================

#ifndef CH_CONTACTCONTAINER_SMC_H
#define CH_CONTACTCONTAINER_SMC_H

#include <algorithm>
#include <cmath>
#include <list>

#include "chrono/physics/ChContactContainer.h"
#include "chrono/physics/ChContactSMC.h"
#include "chrono/physics/ChContactable.h"

namespace chrono {

/// Class representing a container of many smooth (penalty) contacts.
/// This is implemented as a typical linked list of ChContactSMC objects
/// (that is, contacts between two ChContactable objects).
class ChApi ChContactContainerSMC : public ChContactContainer {

  public:
    typedef ChContactSMC<ChContactable_1vars<3>, ChContactable_1vars<3> > ChContactSMC_3_3;
    typedef ChContactSMC<ChContactable_1vars<6>, ChContactable_1vars<3> > ChContactSMC_6_3;
    typedef ChContactSMC<ChContactable_1vars<6>, ChContactable_1vars<6> > ChContactSMC_6_6;
    typedef ChContactSMC<ChContactable_3vars<3, 3, 3>, ChContactable_1vars<3> > ChContactSMC_333_3;
    typedef ChContactSMC<ChContactable_3vars<3, 3, 3>, ChContactable_1vars<6> > ChContactSMC_333_6;
    typedef ChContactSMC<ChContactable_3vars<3, 3, 3>, ChContactable_3vars<3, 3, 3> > ChContactSMC_333_333;
    typedef ChContactSMC<ChContactable_3vars<6, 6, 6>, ChContactable_1vars<3> > ChContactSMC_666_3;
    typedef ChContactSMC<ChContactable_3vars<6, 6, 6>, ChContactable_1vars<6> > ChContactSMC_666_6;
    typedef ChContactSMC<ChContactable_3vars<6, 6, 6>, ChContactable_3vars<3, 3, 3> > ChContactSMC_666_333;
    typedef ChContactSMC<ChContactable_3vars<6, 6, 6>, ChContactable_3vars<6, 6, 6> > ChContactSMC_666_666;

  protected:
    std::list<ChContactSMC_3_3*> contactlist_3_3;
    std::list<ChContactSMC_6_3*> contactlist_6_3;
    std::list<ChContactSMC_6_6*> contactlist_6_6;
    std::list<ChContactSMC_333_3*> contactlist_333_3;
    std::list<ChContactSMC_333_6*> contactlist_333_6;
    std::list<ChContactSMC_333_333*> contactlist_333_333;
    std::list<ChContactSMC_666_3*> contactlist_666_3;
    std::list<ChContactSMC_666_6*> contactlist_666_6;
    std::list<ChContactSMC_666_333*> contactlist_666_333;
    std::list<ChContactSMC_666_666*> contactlist_666_666;

    int n_added_3_3;
    int n_added_6_3;
    int n_added_6_6;
    int n_added_333_3;
    int n_added_333_6;
    int n_added_333_333;
    int n_added_666_3;
    int n_added_666_6;
    int n_added_666_333;
    int n_added_666_666;

    std::list<ChContactSMC_3_3*>::iterator lastcontact_3_3;
    std::list<ChContactSMC_6_3*>::iterator lastcontact_6_3;
    std::list<ChContactSMC_6_6*>::iterator lastcontact_6_6;
    std::list<ChContactSMC_333_3*>::iterator lastcontact_333_3;
    std::list<ChContactSMC_333_6*>::iterator lastcontact_333_6;
    std::list<ChContactSMC_333_333*>::iterator lastcontact_333_333;
    std::list<ChContactSMC_666_3*>::iterator lastcontact_666_3;
    std::list<ChContactSMC_666_6*>::iterator lastcontact_666_6;
    std::list<ChContactSMC_666_333*>::iterator lastcontact_666_333;
    std::list<ChContactSMC_666_666*>::iterator lastcontact_666_666;

  public:
    ChContactContainerSMC();
    ChContactContainerSMC(const ChContactContainerSMC& other);
    virtual ~ChContactContainerSMC();

    /// "Virtual" copy constructor (covariant return type).
    virtual ChContactContainerSMC* Clone() const override { return new ChContactContainerSMC(*this); }

    /// Tell the number of added contacts
    virtual int GetNcontacts() const override {
        return n_added_3_3 + n_added_6_3 + n_added_6_6 + n_added_333_3 + n_added_333_6 + n_added_333_333 +
               n_added_666_3 + n_added_666_6 + n_added_666_333 + n_added_666_666;
    }

    /// Remove (delete) all contained contact data.
    virtual void RemoveAllContacts() override;

    /// The collision system will call BeginAddContact() before adding
    /// all contacts (for example with AddContact() or similar). Instead of
    /// simply deleting all list of the previous contacts, this optimized implementation
    /// rewinds the link iterator to begin and tries to reuse previous contact objects
    /// until possible, to avoid too much allocation/deallocation.
    virtual void BeginAddContact() override;

    /// Add a contact between two frames.
    virtual void AddContact(const collision::ChCollisionInfo& mcontact) override;

    /// The collision system will call BeginAddContact() after adding
    /// all contacts (for example with AddContact() or similar). This optimized version
    /// purges the end of the list of contacts that were not reused (if any).
    virtual void EndAddContact() override;

    /// Scans all the contacts and for each contact executes the OnReportContact()
    /// function of the provided callback object.
    virtual void ReportAllContacts(ReportContactCallback* mcallback) override;

    /// In detail, it computes jacobians, violations, etc. and stores
    /// results in inner structures of contacts.
    virtual void Update(double mtime, bool update_assets = true) override;

    /// Compute contact forces on all contactable objects in this container.
    virtual void ComputeContactForces() override;

    // STATE FUNCTIONS

    virtual void IntLoadResidual_F(const unsigned int off, ChVectorDynamic<>& R, const double c) override;
    virtual void KRMmatricesLoad(double Kfactor, double Rfactor, double Mfactor) override;
    virtual void InjectKRMmatrices(ChSystemDescriptor& mdescriptor) override;

    // SOLVER INTERFACE

    virtual void ConstraintsFbLoadForces(double factor) override;

    // SERIALIZATION

    /// Method to allow serialization of transient data to archives.
    virtual void ArchiveOUT(ChArchiveOut& marchive) override;

    /// Method to allow de-serialization of transient data from archives.
    virtual void ArchiveIN(ChArchiveIn& marchive) override;
};

CH_CLASS_VERSION(ChContactContainerSMC, 0)

}  // end namespace chrono

#endif
