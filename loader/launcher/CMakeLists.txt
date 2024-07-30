add_library(ProxyLauncher SHARED proxy.cpp proxy.def)
set_target_properties(ProxyLauncher PROPERTIES
	PREFIX ""
	OUTPUT_NAME "XInput1_3")