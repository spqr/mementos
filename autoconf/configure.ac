dnl **************************************************************************
dnl * Initialize
dnl **************************************************************************
AC_INIT([[[mementos]]],[[[1.1]]],[ransford@cs.umass.edu])

AC_ARG_WITH(llvm,
	    AS_HELP_STRING([--with-llvm=PATH],
	    		   [specify path to LLVM]),
	    LLVMPREFIX="$withval")
AC_PATH_PROGS([LLVMCONFIG], [llvm-config llvm-config-2.9], [], [${LLVMPREFIX}/bin])
if [[ -z "$LLVMCONFIG" ]]; then
  AC_MSG_ERROR([llvm-config not found in your PATH; quitting.])
fi

LLVMPREFIX=`$LLVMCONFIG --prefix`
LLVM_BIN=`$LLVMCONFIG --bindir`
LLVM_LIB=`$LLVMCONFIG --libdir`

AC_PATH_PROGS([CLANG], [clang], [], [${LLVM_BIN}])
AC_PATH_PROGS([LLVM_OPT], [opt], [], [${LLVM_BIN}])
if [[ -z "$LLVM_OPT" ]]; then
	AC_MSG_ERROR("LLVM misconfigured (opt tool is missing)")
fi
AC_PATH_PROGS([LLVM_LLC], [llc], [], [${LLVM_BIN}])
if [[ -z "$LLVM_LLC" ]]; then
	AC_MSG_ERROR("LLVM misconfigured (llc tool is missing)")
fi
AC_PATH_PROGS([LLVM_LINK], [llvm-link], [], [${LLVM_BIN}])
if [[ -z "$LLVM_LINK" ]]; then
	AC_MSG_ERROR("LLVM misconfigured (llvm-link tool is missing)")
fi

AC_ARG_WITH(mspgcc,
	    AS_HELP_STRING([--with-mspgcc=PATH],
	    		   [specify path to mspgcc]),
	    [MSPGCCPREFIX="$withval"])
AC_PATH_PROGS([MSPGCC], [msp430-gcc], [], [${MSPGCCPREFIX}/bin])
if [[ -z "$MSPGCC" ]]; then
	AC_MSG_ERROR([msp430-gcc not found; use --with-mspgcc=PATH.])
fi
MSPGCCPREFIX=`dirname $(which $MSPGCC)`/..
MSPGCCHEADERS="${MSPGCCPREFIX}/include"

AC_ARG_WITH(mspgcc-include,
	AS_HELP_STRING([--with-mspgcc-include=PATH],
		       [specify path to mspgcc headers]),
	[MSPGCCHEADERS="$withval"])
if [[ -d "$MSPGCCHEADERS" ]]; then
	AC_CHECK_FILE([${MSPGCCHEADERS}/in430.h], [],
		AC_MSG_ERROR([mspgcc headers not found; use --with-mspgcc-include=PATH]))
	CPPFLAGS="${CPPFLAGS} -I${MSPGCCPREFIX}/include"
else
	AC_MSG_ERROR([mspgcc headers not found; use --with-mspgcc-include=PATH.])
fi

AC_ARG_WITH(mspsim,
	    AS_HELP_STRING([--with-mspsim=PATH],
	    		   [specify path to mspsim]),
	    MSPSIMPREFIX="$withval")

dnl Verify that the source directory is valid
AC_CONFIG_SRCDIR(["Makefile.common.in"])

dnl Configure a common Makefile
AC_CONFIG_FILES(Makefile.common)

AC_SUBST(LLVMPREFIX)
AC_SUBST(LLVM_BIN)
AC_SUBST(LLVM_LIB)
AC_SUBST(MSPGCCPREFIX)
AC_SUBST(MSPGCCHEADERS)
AC_SUBST(MSPSIMPREFIX)

dnl This must be last
AC_OUTPUT
