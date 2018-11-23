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

    mLimits = _joints_limits.get();
    mErrorCommand = _error_command.get();
    mJointsSize = mLimits.elements.size();
    if (mJointsSize != mErrorCommand.elements.size())
        throw std::runtime_error("limits and error command sizes differ");

    mCommand.elements.resize(mErrorCommand.size());
    return true;
}
bool ConstantPositionEnforcer::startHook()
{
    if (! ConstantPositionEnforcerBase::startHook())
        return false;
    return true;
}

bool ConstantPositionEnforcer::processCommand(
        base::commands::Joints& jointsCommand,
        base::samples::Joints const& jointsState) {

    bool valid = true;
    for (size_t i = 0; i < jointsState.elements.size(); ++i) {
        auto const& state = jointsState.elements[i];
        auto range = mLimits.elements[i];
        if (state.position > range.max.position) {
            auto field = mErrorCommand.elements[i];
            field.position     = -field.position;
            field.speed        = -field.speed;
            field.effort       = -field.effort;
            field.raw          = -field.raw;
            field.acceleration = -field.acceleration;
            jointsCommand.elements[i] = field;
            valid = false;
        }
        else if (state.position < range.min.position) {
            jointsCommand.elements[i] = mErrorCommand.elements[i];
            valid = false;
        }
    }
    return valid;
}

void ConstantPositionEnforcer::updateHook()
{
    if (_joints_samples.read(mJointsState, false) == RTT::NoData) {
        return;
    }

    if (mJointsState.elements.size() != mJointsSize) {
        return exception(INVALID_JOINTS_SAMPLE_SIZE);
    }

    if (_nominal_commands.read(mCommand, false) == RTT::NewData) {
        if (mCommand.elements.size() != mJointsSize) {
            return exception(INVALID_NOMINAL_COMMAND_SIZE);
        }

        if (processCommand(mCommand, mJointsState)) {
            if (state() != RUNNING) {
                state(RUNNING);
            }
        }
        else {
            if (state() != OUT_OF_BOUNDS) {
                state(OUT_OF_BOUNDS);
            }
        }
        _joints_commands.write(mCommand);
    }

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
