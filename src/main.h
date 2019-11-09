#ifndef _MAIN_H_
#define _MAIN_H_

#include "../glad/glad.h"
#include "../sub/cbase/src/gc.h"

typedef void (*MainRender)(void*,float,float,double);

GC_BLOCK_START(MainState)
  void* data;
GC_BLOCK_CENTER
  MainRender render;
GC_BLOCK_END

char* main_setup();
MainState* main_init();
GLuint* main_glGenBuffers(GLsizei);
GLuint* main_glCreateShader(GLenum,const GLchar*,const GLint);
GLuint* main_glCreateProgram(unsigned int,...);
GLuint* main_glGenTextures(GLsizei);

#endif
