# define the project
PROJECT(openGL_examples)

find_package(GLUT)
find_package(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# define the executable
ADD_EXECUTABLE(line line.cpp)

TARGET_LINK_LIBRARIES(line ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

