#include "main.h"
#include "text.h"
#include "linmath.h"

static const TextVertex vertices[] =
{
    { -0.7f,  0.7f, 0.f, 0.f },
    {  0.7f,  0.7f, 1.f, 0.f },
    { -0.7f, -0.7f, 0.f, 1.f },
    {  0.7f, -0.7f, 1.f, 1.f }
};

char* main_setup()
{
  return gc_new_string("Test GL");
}

GC_BLOCK_START(TestState)
  GLuint* buffers;
  void* text;
GC_BLOCK_CENTER
GC_BLOCK_END

void render(TestState* ts,float w,float h,double t)
{
  mat4x4 m,p,mvp;
  mat4x4_identity(m);
  mat4x4_rotate_Z(m,m,(float) t);
  mat4x4_ortho(p,-w,w,-h,h,1.f,-1.f);
  mat4x4_mul(mvp,p,m);
  text_enable(ts->text,(const GLfloat*)mvp);
  glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

MainState* main_init()
{
  TestState* ts=GC_NEW_BLOCK(TestState);
  MainState* ms=GC_NEW_BLOCK(MainState);
  ms->render=(MainRender)&render; 
  ms->data=ts;
  ts->buffers=main_glGenBuffers(1);
  glBindBuffer(GL_ARRAY_BUFFER,ts->buffers[0]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
  ts->text=text_createPannel(32,32);
  return ms;
}
