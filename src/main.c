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

GLuint* main_glGenBuffers(GC gc,GLsizei n)
{
  GLuint* buffers=gc_new(gc,0,sizeof(GLuint*)*(n+1),(void (*)(void*))&main_glGenBuffers_gc);
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

GLuint* main_glCreateShader(GC gc,GLenum shaderType,const GLchar* code,const GLint size)
{
  GLuint* shader=gc_new(gc,0,sizeof(GLuint*),(void (*)(void*))&main_glCreateShader_gc);
  *shader=glCreateShader(shaderType);
  glShaderSource(*shader,1,&code,&size);
  glCompileShader(*shader);
  #ifdef DEBUG
  printf("created shader at %p\n",shader);
  #endif
  return shader;
}

void main_glCreateProgram_gc(GLuint* program)
{
  glDeleteProgram(*program);
  #ifdef DEBUG
  printf("deleted program at %p\n",program);
  #endif
}

GLuint* main_glCreateProgram(GC gc,unsigned int n,...)
{
  GLuint* program=gc_new(gc,0,sizeof(GLuint*),(void (*)(void*))&main_glCreateProgram_gc);
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
  printf("deleted %x textures at %p\n",n,textures);
  #endif
}

GLuint* main_glGenTextures(GC gc,GLsizei n)
{
  GLuint* textures=gc_new(gc,0,sizeof(GLuint*)*(n+1),(void (*)(void*))&main_glGenTextures_gc);
  textures[n]=0;
  glGenTextures(n,textures);
  #ifdef DEBUG
  printf("created %x textures at %p\n",n,textures);
  #endif
  return textures;
}

static void error_callback(int error, const char* description)
{
    printf("ERROR(%d) %s\n",error, description);
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  printf(  "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}
int main(void)
{
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if(!glfwInit())return -1;
  GC gc=gc_create();
  glfwWindowHint(GLFW_MAXIMIZED,GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
  window=glfwCreateWindow(1024,512,main_setup(gc),NULL,NULL);
  if(!window)
  {
    gc_destroy(gc);
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback( MessageCallback,0);
  glfwSwapInterval(1);
  MainState* ms=main_init(gc);
  while(!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    if(width>height)ms->render(ms->data,width/(float)height,1.0,glfwGetTime());
    else ms->render(ms->data,1.0,height/(float)width,glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  gc_destroy(gc);
  glfwTerminate();
  return 0;
}
