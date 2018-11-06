/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "ConstantPositionEnforcer.hpp"

using namespace joint_range_safety;

ConstantPositionEnforcer::ConstantPositionEnforcer(std::string const& name)
    : ConstantPositionEnforcerBase(name)
{
}

ConstantPositionEnforcer::ConstantPositionEnforcer(std::string const& name, RTT::ExecutionEngine* engine)
    : ConstantPositionEnforcerBase(name, engine)
{
}

ConstantPositionEnforcer::~ConstantPositionEnforcer()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See ConstantPositionEnforcer.hpp for more detailed
// documentation about them.

bool ConstantPositionEnforcer::configureHook()
{
    if (! ConstantPositionEnforcerBase::configureHook())
        return false;
    return true;
}
bool ConstantPositionEnforcer::startHook()
{
    if (! ConstantPositionEnforcerBase::startHook())
        return false;
    return true;
}
void ConstantPositionEnforcer::updateHook()
{
    ConstantPositionEnforcerBase::updateHook();
}
void ConstantPositionEnforcer::errorHook()
{
    ConstantPositionEnforcerBase::errorHook();
}
void ConstantPositionEnforcer::stopHook()
{
    ConstantPositionEnforcerBase::stopHook();
}
void ConstantPositionEnforcer::cleanupHook()
{
    ConstantPositionEnforcerBase::cleanupHook();
}
