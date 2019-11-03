#include "main.h"
#include <GLFW/glfw3.h>
#include <stdarg.h>
#include <stdio.h>

void main_glGenBuffers_gc(GLuint* buffers)
{
  GLsizei n=0;
  for(GLuint* p=buffers;*p;p++)n++;
  glDeleteBuffers(n,buffers);
  #ifdef DEBUG
  printf("deleted %x buffers at %p\n",n,buffers);
  #endif
}

GLuint* main_glGenBuffers(GLsizei n)
{
  GLuint* buffers=gc_new(0,sizeof(GLuint*)*(n+1),(void (*)(void*))&main_glGenBuffers_gc);
  buffers[n]=0;
  glGenBuffers(n,buffers);
  #ifdef DEBUG
  printf("created %x buffers at %p\n",n,buffers);
  #endif
  return buffers;
}

void main_glCreateShader_gc(GLuint* shader)
{
  glDeleteShader(*shader);
  #ifdef DEBUG
  printf("deleted shader at %p\n",shader);
  #endif
}

GLuint* main_glCreateShader(GLenum shaderType,const GLchar* const* code)
{
  GLuint* shader=gc_new(0,sizeof(GLuint*),(void (*)(void*))&main_glCreateShader_gc);
  *shader=glCreateShader(shaderType);
  glShaderSource(*shader,1,code,NULL);
  glCompileShader(*shader);
  #ifdef DEBUG
  printf("created shader at %p\n",shader);
  #endif
  return shader;
}

void main_glCreateProgram_gc(GLuint* program)
{
  glDeleteShader(*program);
  #ifdef DEBUG
  printf("deleted program at %p\n",program);
  #endif
}

GLuint* main_glCreateProgram(unsigned int n,...)
{
  GLuint* program=gc_new(0,sizeof(GLuint*),(void (*)(void*))&main_glCreateProgram_gc);
  *program=glCreateProgram();
  va_list progs;
  va_start(progs,n); 
  while(n--)glAttachShader(*program,*(va_arg(progs,GLuint*)));
  glLinkProgram(*program);
  #ifdef DEBUG
  printf("created program at %p\n",program);
  #endif
  return program;
}

void main_glGenTextures_gc(GLuint* textures)
{
  GLsizei n=0;
  for(GLuint* p=textures;*p;p++)n++;
  glDeleteBuffers(n,textures);
  #ifdef DEBUG
  printf("deleted %x buffers at %p\n",n,textures);
  #endif
}

GLuint* main_glGenTextures(GLsizei n)
{
  GLuint* textures=gc_new(0,sizeof(GLuint*)*(n+1),(void (*)(void*))&main_glGenTextures_gc);
  textures[n]=0;
  glGenTextures(n,textures);
  #ifdef DEBUG
  printf("created %x tetures at %p\n",n,textures);
  #endif
  return textures;
}

static void error_callback(int error, const char* description)
{
    printf("ERROR(%d) %s\n",error, description);
}

int main(void)
{
  glfwSetErrorCallback(error_callback);
  GLFWwindow* window;
  if(!glfwInit()) return -1;
  glfwWindowHint(GLFW_MAXIMIZED,GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  window=glfwCreateWindow(1024,512,main_setup(),NULL,NULL);
  if(!window)
  {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  MainState* ms=main_init();
  while(!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    if(width>height)ms->render(ms->data,width / (float) height,1.0,glfwGetTime());
    else ms->render(ms->data,1.0,height / (float) width,glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  gc_sweep();
  glfwTerminate();
  return 0;
}
