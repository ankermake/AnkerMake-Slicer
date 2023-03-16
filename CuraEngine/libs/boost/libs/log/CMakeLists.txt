add_definitions(-DBOOST_LOG_SOURCE)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_LOG_BUILDING_THE_LIB)
add_definitions(-DBOOST_LOG_DLL)
add_definitions(-DBOOST_LOG_SETUP_BUILDING_THE_LIB)
add_definitions(-DBOOST_LOG_SETUP_DLL)

set(SRC attribute_name.cpp
		attribute_set.cpp
		attribute_value_set.cpp
		code_conversion.cpp
		core.cpp
		date_time_format_parser.cpp
		default_attribute_names.cpp
		default_sink.cpp
		dump.cpp
		dump_avx2.cpp
		dump_ssse3.cpp
		event.cpp
		exceptions.cpp
		format_parser.cpp
		global_logger_storage.cpp
		named_scope.cpp
		named_scope_format_parser.cpp
		once_block.cpp
		permissions.cpp
		process_id.cpp
		process_name.cpp
		record_ostream.cpp
		severity_level.cpp
		spirit_encoding.cpp
		syslog_backend.cpp
		text_file_backend.cpp
		text_multifile_backend.cpp
		text_ostream_backend.cpp
		thread_id.cpp
		thread_specific.cpp
		threadsafe_queue.cpp
		timer.cpp
		timestamp.cpp
		trivial.cpp
		setup/default_filter_factory.cpp
		setup/default_formatter_factory.cpp
		setup/filter_parser.cpp
		setup/formatter_parser.cpp
		setup/init_from_settings.cpp
		setup/init_from_stream.cpp
		setup/matches_relation_factory.cpp
		setup/parser_utils.cpp
		setup/settings_parser.cpp
		)

set(LIBS boost_file_system boost_regex boost_thread)
set(DEFS)
		
if(WIN32)
	list(APPEND SRC windows/debug_output_backend.cpp
					windows/event_log_backend.cpp
					windows/ipc_reliable_message_queue.cpp
					windows/ipc_sync_wrappers.cpp
					windows/light_rw_mutex.cpp
					windows/mapped_shared_memory.cpp
					windows/object_name.cpp
					)
	list(APPEND LIBS Secur32.lib)
	list(APPEND DEFS SECURITY_WIN32)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mssse3")
endif()

__add_real_target(boost_log dll SOURCE ${SRC}
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