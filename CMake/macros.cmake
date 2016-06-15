# assert test if the dependency was found
function (assert TARGET LIB LIB_NAME)
	if (${LIB})
		message ("Library found: ${LIB_NAME}. LINKING")
		target_link_libraries(${TARGET} ${LIB})
	elseif (NOT ${LIB})
		message (WARNING "Assertion failed, libary not found ${LIB_NAME}")
	endif()
endfunction(assert)

# welcome function
function (welcome)
	message ("The build process is starting")
endfunction()


