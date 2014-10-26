#pragma once

#ifdef _WIN32
#   define NOMINMAX
#   define _CRT_SECURE_NO_DEPRECATE
#   include <Windows.h>
#endif

#if EMSCRIPTEN
    #include <GL/glut.h>
    #include <EGL/egl.h>
#else
    #include <GL/freeglut.h>
    #include <GL/gl.h>
#endif