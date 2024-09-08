add_library(launcher SHARED proxy.cpp proxy.def)
set_target_properties(launcher PROPERTIES
	PREFIX ""
	OUTPUT_NAME "XInput1_3")