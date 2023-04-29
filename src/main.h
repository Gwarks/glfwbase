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

char* main_setup(GC);
MainState* main_init(GC);
GLuint* main_glGenBuffers(GC,GLsizei);
GLuint* main_glCreateShader(GC,GLenum,const GLchar*,const GLint);
GLuint* main_glCreateProgram(GC,unsigned int,...);
GLuint* main_glGenTextures(GC,GLsizei);

#endif
