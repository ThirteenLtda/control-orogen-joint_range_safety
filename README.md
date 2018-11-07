# `joint_range_safety`: Joint range safety components for kinematic control chains

The components in this package are meant as a last-line-of-defense in
kinematic control chains. They can react quickly to out-of-bound
situations, reporting it as well for higher-level reaction (e.g. braking)

## Usage

This package is meant to be used with Rock, the Robot Construction Kit.
See https://rock-core.github.com/rock-and-syskit for more information.

## Running the Tests

The tests in this package are meant to be run using Syskit. Go into
a valid Syskit app and run

~~~
syskit test -rlive /path/to/dir
~~~