# This sets the following variables:
# mpfr target

if(NOT TARGET mpfr)
	if(NOT MPFR_INSTALL_ROOT)
		set(MPFR_INSTALL_ROOT $ENV{USR_INSTALL_ROOT})
	endif()
	
	set(mpfr_INCLUDE_DIRS ${MPFR_INSTALL_ROOT}/include/)
		set(mpfr_LIBRARIES_DEBUG "${MPFR_INSTALL_ROOT}/lib/libmpfr-4.lib")
	    set(mpfr_LIBRARIES_RELEASE "${MPFR_INSTALL_ROOT}/lib/libmpfr-4.lib")
	    set(mpfr_LOC_DEBUG "${MPFR_INSTALL_ROOT}/bin/libmpfr-4.dll")
	    set(mpfr_LOC_RELEASE "${MPFR_INSTALL_ROOT}/bin/libmpfr-4.dll")	



	__test_import(mpfr dll)
endif()
