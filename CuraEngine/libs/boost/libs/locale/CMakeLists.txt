add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_LOCALE_NO_POSIX_BACKEND)

set(SRC encoding/codepage.cpp
		shared/date_time.cpp
		shared/format.cpp
		shared/formatting.cpp
		shared/generator.cpp
		shared/ids.cpp
		shared/localization_backend.cpp
		shared/message.cpp
		shared/mo_lambda.cpp
		std/codecvt.cpp
		std/collate.cpp
		std/converter.cpp
		std/numeric.cpp
		std/std_backend.cpp
		util/codecvt_converter.cpp
		util/default_locale.cpp
		util/gregorian.cpp
		util/info.cpp
		util/locale_data.cpp
		)

set(LIBS)		
if(WIN32)
	list(APPEND SRC win32/collate.cpp
					win32/converter.cpp
					win32/lcid.cpp
					win32/numeric.cpp
					win32/win_backend.cpp
					)
else()
	list(APPEND SRC icu/boundary.cpp
			icu/codecvt.cpp
			icu/conversion.cpp
			icu/date_time.cpp
			icu/formatter.cpp
			icu/collator.cpp
			icu/icu_backend.cpp
			icu/time_zone.cpp
			icu/numeric.cpp
			)
	list(APPEND LIBS boost_thread icutu icuuc icudata icui18n)
	add_definitions(-DBOOST_LOCALE_WITH_ICU)
	add_definitions(-DBOOST_LOCALE_NO_WINAPI_BACKEND)
endif()

__add_real_target(boost_locale dll SOURCE ${SRC}
									  INC ${CMAKE_CURRENT_SOURCE_DIR}/../../
									  LIB ${LIBS}
									  DEF BOOST_ALL_DYN_LINK
									  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../)
									  
if(INSTALL_BOOST)
	INSTALL(TARGETS boost_locale
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION .
				RUNTIME DESTINATION bin)
endif()
