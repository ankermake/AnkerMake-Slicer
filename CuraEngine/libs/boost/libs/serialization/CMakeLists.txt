set(SRC 
	archive_exception.cpp
	basic_archive.cpp
	basic_iarchive.cpp
	basic_iserializer.cpp
	basic_oarchive.cpp
	basic_oserializer.cpp
	basic_pointer_iserializer.cpp
	basic_pointer_oserializer.cpp
	basic_serializer_map.cpp
	basic_text_iprimitive.cpp
	basic_text_oprimitive.cpp
	basic_xml_archive.cpp
	binary_iarchive.cpp
	binary_oarchive.cpp
	extended_type_info_no_rtti.cpp
	extended_type_info_typeid.cpp
	extended_type_info.cpp
	polymorphic_iarchive.cpp
	polymorphic_oarchive.cpp
	polymorphic_text_iarchive.cpp
	polymorphic_text_oarchive.cpp
	stl_port.cpp
	text_iarchive.cpp
	text_oarchive.cpp
	void_cast.cpp
	xml_archive_exception.cpp
	xml_iarchive.cpp
	xml_oarchive.cpp
	xml_grammar.cpp
	utf8_codecvt_facet.cpp
	basic_xml_grammar.ipp 

	#codecvt_null.cpp
	#basic_text_wiprimitive.cpp
	#basic_text_woprimitive.cpp
	#text_wiarchive.cpp
	#text_woarchive.cpp
	#xml_wiarchive.cpp
	#xml_woarchive.cpp
	#xml_wgrammar.cpp
	#basic_xml_grammar.ipp
)

set(LIBS)
set(DEFS)

__add_real_target(boost_serialization dll SOURCE ${SRC}
									  INC ${CMAKE_CURRENT_SOURCE_DIR}/../../
									  DEF ${DEFS} BOOST_ALL_DYN_LINK
									  LIB ${LIBS}
									  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../)
									  
if(INSTALL_BOOST)
	INSTALL(TARGETS boost_log
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION .
				RUNTIME DESTINATION bin)
endif()