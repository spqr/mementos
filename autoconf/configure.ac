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

AC_ARG_WITH(mspgcc,
	    AS_HELP_STRING([--with-mspgcc=PATH],
	    		   [specify path to mspgcc]),
	    MSPGCCPREFIX="$withval")
if [[ -z "$MSPGCCPREFIX" ]]; then
	AC_CHECK_PROGS([MSPGCC], [msp430-gcc])
	if [[ -z "$MSPGCC" ]]; then
  		AC_MSG_ERROR([msp430-gcc not found in your PATH and --with-mspgcc not specified; quitting.])
	fi
	MSPGCCPREFIX=`dirname $(which $MSPGCC)`/..
	CFLAGS="-I${MSPGCCPREFIX}/include"
fi

AC_ARG_WITH(mspgcc-include,
	AS_HELP_STRING([--with-mspgcc-include=PATH],
		       [specify path to mspgcc includes]),
	MSPGCCINCLUDEPREFIX="$withval")
if [[ -z "$MSPGCCINCLUDEPREFIX" ]]; then
	AC_CHECK_HEADERS([in430.h msp430x21x1.h], [],
		[AC_MSG_ERROR([mspgcc headers not found; use --with-mspgcc-include])])
	MSPGCCINCLUDEPREFIX="$MSPGCCPREFIX"
	CFLAGS="-I${MSPGCCINCLUDEPREFIX}/include"
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
AC_SUBST(MSPGCCPREFIX)
AC_SUBST(MSPGCCINCLUDEPREFIX)
AC_SUBST(MSPSIMPREFIX)

dnl This must be last
AC_OUTPUT
