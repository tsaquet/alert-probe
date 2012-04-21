#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/addon.o \
	${OBJECTDIR}/src/plugin.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/sender.o \
	${OBJECTDIR}/src/conf.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe \
	${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe \
	${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/addon.o: src/addon.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon.o src/addon.c

${OBJECTDIR}/src/plugin.o: src/plugin.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plugin.o src/plugin.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/src/sender.o: src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender.o src/sender.c

${OBJECTDIR}/src/conf.o: src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf.o src/conf.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${TESTDIR}/tests/confcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c}   -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe $^ ${LDLIBSOPTIONS} 

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${TESTDIR}/tests/plugincunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c}   -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe $^ ${LDLIBSOPTIONS} 

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${TESTDIR}/tests/sendercunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c}   -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/confcunittest.o: tests/confcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${TESTDIR}/tests/confcunittest.o tests/confcunittest.c


${TESTDIR}/tests/plugincunittest.o: tests/plugincunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${TESTDIR}/tests/plugincunittest.o tests/plugincunittest.c


${TESTDIR}/tests/sendercunittest.o: tests/sendercunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O2 -Iinclude -MMD -MP -MF $@.d -o ${TESTDIR}/tests/sendercunittest.o tests/sendercunittest.c


${OBJECTDIR}/src/addon_nomain.o: ${OBJECTDIR}/src/addon.o src/addon.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon_nomain.o src/addon.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon.o ${OBJECTDIR}/src/addon_nomain.o;\
	fi

${OBJECTDIR}/src/plugin_nomain.o: ${OBJECTDIR}/src/plugin.o src/plugin.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/plugin.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plugin_nomain.o src/plugin.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/plugin.o ${OBJECTDIR}/src/plugin_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/src/sender_nomain.o: ${OBJECTDIR}/src/sender.o src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/sender.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender_nomain.o src/sender.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/sender.o ${OBJECTDIR}/src/sender_nomain.o;\
	fi

${OBJECTDIR}/src/conf_nomain.o: ${OBJECTDIR}/src/conf.o src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/conf.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -Iinclude -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf_nomain.o src/conf.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/conf.o ${OBJECTDIR}/src/conf_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe || true; \
	    ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe || true; \
	    ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
