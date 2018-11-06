using_task_library 'joint_range_safety'

describe OroGen.joint_range_safety.ConstantPositionEnforcer do
    before do
        limit_range = Types.base.JointLimitRange.new(
            max: { position: 10 }, min: { position: 0 })
        syskit_stub_conf(OroGen.joint_range_safety.ConstantPositionEnforcer, 'default',
            data: {
                'error_command' => Types.base.samples.Joints.new(elements: [
                    { position: 1, speed: 2, effort: 3, raw: 4, acceleration: 5 },
                    { position: 10, speed: 20, effort: 30, raw: 40, acceleration: 50 }]),
                'joints_limits' => Types.base.JointLimits.new(
                    elements: [limit_range, limit_range])
            }
        )
    end

    describe "joint size validations" do
        it "fails if the error command and joint limits have different sizes" do
            syskit_stub_conf(OroGen.joint_range_safety.ConstantPositionEnforcer, 'default',
                data: {
                    'error_command' => Types.base.commands.Joints.from_efforts(10)
                }
            )
            deployed = syskit_deploy(subject_syskit_model)
            expect_execution.scheduler(true).to do
                fail_to_start deployed, reason: Orocos::StateTransitionFailed
            end
        end
        it "fails if it receives a joint state with an unexpected size" do
            deployed = syskit_deploy_configure_and_start(subject_syskit_model)
            expect_execution do
                syskit_write deployed.joints_samples_port,
                    Types.base.commands.Joints.from_efforts(10)
            end.to do
                emit deployed.invalid_joints_sample_size_event
            end
        end
        it "fails if it receives a nominal command with an unexpected size" do
            deployed = syskit_deploy_configure_and_start(subject_syskit_model)
            expect_execution do
                syskit_write deployed.joints_samples_port,
                    Types.base.commands.Joints.from_efforts(10, 20)
                syskit_write deployed.nominal_commands_port,
                    Types.base.commands.Joints.from_efforts(10)
            end.to do
                emit deployed.invalid_nominal_command_size_event
            end
        end
    end
    it "passes the nominal command through if it is within bounds" do
        deployed = syskit_deploy_configure_and_start(subject_syskit_model)
        cmd = Types.base.commands.Joints.from_efforts(42, 24)
        sample = expect_execution do
            syskit_write deployed.joints_samples_port,
                Types.base.commands.Joints.from_positions(5, 5)
            syskit_write deployed.nominal_commands_port, cmd
        end.to do
            have_one_new_sample deployed.joints_commands_port
        end
        assert_equal cmd, sample
    end

    it "replaces out-of-bounds entries below minimum with the error command" do
        deployed = syskit_deploy_configure_and_start(subject_syskit_model)
        cmd = Types.base.commands.Joints.from_efforts(42, 24)
        expected_cmd = Types.base.commands.Joints.new(elements: [
            { position: 1, speed: 2, effort: 3, raw: 4, acceleration: 5 },
            cmd.elements[1]
        ])
        sample = expect_execution do
            syskit_write deployed.joints_samples_port,
                Types.base.commands.Joints.from_positions(-5, 5)
            syskit_write deployed.nominal_commands_port, cmd
        end.to do
            have_one_new_sample deployed.joints_commands_port
        end
        assert_equal expected_cmd.elements, sample.elements
    end

    it "replaces out-of-bounds entries above maximum with minus the error command" do
        deployed = syskit_deploy_configure_and_start(subject_syskit_model)
        cmd = Types.base.commands.Joints.from_efforts(42, 24)
        expected_cmd = Types.base.commands.Joints.new(elements: [
            { position: -1, speed: -2, effort: -3, raw: -4, acceleration: -5 },
            cmd.elements[1]
        ])
        sample = expect_execution do
            syskit_write deployed.joints_samples_port,
                Types.base.commands.Joints.from_positions(15, 5)
            syskit_write deployed.nominal_commands_port, cmd
        end.to do
            have_one_new_sample deployed.joints_commands_port
        end
        assert_equal expected_cmd.elements, sample.elements
    end
end