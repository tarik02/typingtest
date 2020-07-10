cmake_minimum_required(VERSION 3.16.2)

project(typingtest)

set(LOCALE "en" CACHE STRING "Locale")

find_package(Curses REQUIRED)

add_executable(typingtest
	src/locale/conclusions.c
	src/locale/messages_${LOCALE}.c
	src/list.c
	src/platform.c
	src/str.c
	src/typingtest.c
	src/util.c
	src/words.c
)

IF(NOT WIN32)
	set(MATH_LIBRARIES m)
ELSE()
	set(MATH_LIBRARIES)
ENDIF()

target_link_libraries(typingtest
	PRIVATE
		${CURSES_LIBRARIES}
		${MATH_LIBRARIES}
)

target_include_directories(typingtest
	PRIVATE
		${CURSES_INCLUDE_DIR}
)

install(TARGETS typingtest RUNTIME DESTINATION bin)
