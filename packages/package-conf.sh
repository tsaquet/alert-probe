#!/bin/bash -x

#######################
# Configuration - Probe
#######################

# Executable path generate by compilation
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

PKG_DESCRIPTION="The ECHOES Alert Probe"

# Bin name
BIN_NAME=ea-probe

# Package name
PKG_NAME=ea-probe

# Package type
package="probe"

# Version and package version
version="0.1.0"
pkg_version="1"

# Minimum common requirement version
minimun_version="0.1.0"