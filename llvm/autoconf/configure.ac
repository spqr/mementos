dnl **************************************************************************
dnl * Initialize
dnl **************************************************************************
AC_INIT([[[mementos]]],[[[1.1]]],[ransford@cs.umass.edu])

AC_ARG_WITH(llvm,
	    AS_HELP_STRING([--with-llvm=PATH],
	    		   [specify path to LLVM]),
	    LLVMPREFIX="$withval")
if [[ -z "$LLVMPREFIX" ]]; then
	AC_CHECK_PROGS([LLVMCONFIG], [llvm-config])
	if [[ -z "$LLVMCONFIG" ]]; then
	  AC_MSG_ERROR([llvm-config not found in your PATH; quitting.])
	fi
	LLVMPREFIX=`$LLVMCONFIG --prefix`
fi
LLVMCONFIG="${LLVMPREFIX}/bin/llvm-config"

dnl Identify where LLVM source tree is
LLVM_SRC_ROOT=`$LLVMCONFIG --src-root`
if [[ -z "$LLVM_SRC_ROOT" ]] || [[ ! -d "$LLVM_SRC_ROOT" ]]; then
  AC_MSG_ERROR([LLVM sources not found.])
fi
LLVM_OBJ_ROOT=`$LLVMCONFIG --obj-root`
if [[ -z "$LLVM_OBJ_ROOT" ]] || [[ ! -d "$LLVM_OBJ_ROOT" ]]; then
  AC_MSG_ERROR([LLVM object files not found.])
fi

dnl Tell autoconf that this is an LLVM project being configured
dnl This provides the --with-llvmsrc and --with-llvmobj options
LLVM_CONFIG_PROJECT($LLVM_SRC_ROOT,$LLVM_OBJ_ROOT)

dnl Tell autoconf that the auxilliary files are actually located in
dnl the LLVM autoconf directory, not here.
AC_CONFIG_AUX_DIR($LLVM_SRC/autoconf)

dnl Verify that the source directory is valid
AC_CONFIG_SRCDIR(["Makefile.common.in"])

dnl Configure a common Makefile
AC_CONFIG_FILES(Makefile.common)

dnl Configure project makefiles
dnl List every Makefile that exists within your source tree
AC_CONFIG_MAKEFILE(Makefile)
AC_CONFIG_MAKEFILE(lib/Makefile)
AC_CONFIG_MAKEFILE(lib/Mementos/Makefile)

dnl **************************************************************************
dnl * Determine which system we are building on
dnl **************************************************************************

dnl **************************************************************************
dnl * Check for programs.
dnl **************************************************************************

dnl **************************************************************************
dnl * Check for libraries.
dnl **************************************************************************

dnl **************************************************************************
dnl * Checks for header files.
dnl **************************************************************************

dnl **************************************************************************
dnl * Checks for typedefs, structures, and compiler characteristics.
dnl **************************************************************************

dnl **************************************************************************
dnl * Checks for library functions.
dnl **************************************************************************

dnl **************************************************************************
dnl * Enable various compile-time options
dnl **************************************************************************

dnl **************************************************************************
dnl * Set the location of various third-party software packages
dnl **************************************************************************

dnl **************************************************************************
dnl * Create the output files
dnl **************************************************************************

dnl This must be last
AC_OUTPUT
