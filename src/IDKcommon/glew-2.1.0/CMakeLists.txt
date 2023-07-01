add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

include_directories(
	GLEW PUBLIC include/
)

set(
    GLEW_SOURCE
    src/glew.c
)

set(GLEW_HEADERS)


add_library(
    GLEW_210 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(
    GLEW_210
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)




