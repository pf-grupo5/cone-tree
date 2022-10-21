# Ray tracing with a cone tree
# Copyright © 2022 otreblan
#
# cone-tree is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# cone-tree is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with cone-tree.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.13.0)

project(cone-tree
	VERSION 0.0.0
	DESCRIPTION "Ray tracing with a cone tree"
	HOMEPAGE_URL "https://github.com/pf-grupo5/cone-tree"
)

# Without this, paths are not relative in the sources list
cmake_policy(SET CMP0076 NEW)

include(GNUInstallDirs)
include(FindPkgConfig)
include(CPack)

# The program itself
add_executable(${PROJECT_NAME})

# C++ version
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 20
)

# Packages
find_package(bash-completion QUIET)

add_subdirectory(src) # Sources list
add_subdirectory(pkg) # Packaging

# libraries
pkg_check_modules(libraries
	REQUIRED IMPORTED_TARGET
		fmt
		glm
)

# Linking
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PkgConfig::libraries
)

# Default flags
if(UNIX)
	if(NOT (DEFINED ENV{CFLAGS} OR CMAKE_C_FLAGS))
		set(CMAKE_C_FLAGS "-Wall -Wextra -g -Og")
	endif()
	if(NOT (DEFINED ENV{CXXFLAGS} OR CMAKE_CXX_FLAGS))
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -Og")
	endif()
endif()

# Bash completion
file(WRITE ${CMAKE_BINARY_DIR}/bash-completion/${PROJECT_NAME}
	"complete -F _longopt ${PROJECT_NAME}"
)


include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT error)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION ${LTO_SUPPORTED})

install(FILES
	${CMAKE_BINARY_DIR}/bash-completion/${PROJECT_NAME}
	DESTINATION ${BASH_COMPLETION_COMPLETIONSDIR}/
)

# Install target
install(TARGETS ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)
