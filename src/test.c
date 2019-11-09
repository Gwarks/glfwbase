#include "main.h"
#include "linmath.h"

extern GLubyte _binary_bin_VGA_ROM_F08_start;
extern char _binary_bin_VGA_vertex_shader_start;
extern char _binary_bin_VGA_vertex_shader_end;
extern char _binary_bin_VGA_fragment_shader_start;
extern char _binary_bin_VGA_fragment_shader_end;

static const struct
{
    float x, y;
    float u, v;
} vertices[] =
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
  GLuint* vertex_shader;
  GLuint* fragment_shader;
  GLuint* program;
  GLuint* textures;
GC_BLOCK_CENTER
  GLint mvp_location;
GC_BLOCK_END

void render(TestState* ts,float w,float h,double t)
{
  mat4x4 m,p,mvp;
  mat4x4_identity(m);
  mat4x4_rotate_Z(m,m,(float) t);
  mat4x4_ortho(p,-w,w,-h,h,1.f,-1.f);
  mat4x4_mul(mvp,p,m);
  glUseProgram(ts->program[0]);
  glUniformMatrix4fv(ts->mvp_location,1,GL_FALSE,(const GLfloat*)mvp);
  glBindTexture(GL_TEXTURE_2D,ts->textures[0]);
  glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

MainState* main_init()
{
  GLint vpos_location,vuv_location;
  TestState* ts=GC_NEW_BLOCK(TestState);
  MainState* ms=GC_NEW_BLOCK(MainState);
  ms->render=(MainRender)&render; 
  ms->data=ts;
  ts->buffers=main_glGenBuffers(1);
  glBindBuffer(GL_ARRAY_BUFFER,ts->buffers[0]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
  ts->textures=main_glGenTextures(1);
  glBindTexture(GL_TEXTURE_2D,ts->textures[0]);
  glTexImage2D(GL_TEXTURE_2D,0,GL_R8UI,8,256,0,GL_RED_INTEGER,GL_UNSIGNED_BYTE,&_binary_bin_VGA_ROM_F08_start);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  ts->vertex_shader=main_glCreateShader(GL_VERTEX_SHADER,&_binary_bin_VGA_vertex_shader_start,
       &_binary_bin_VGA_vertex_shader_end-&_binary_bin_VGA_vertex_shader_start);
  ts->fragment_shader=main_glCreateShader(GL_FRAGMENT_SHADER,&_binary_bin_VGA_fragment_shader_start,
       &_binary_bin_VGA_fragment_shader_end-&_binary_bin_VGA_fragment_shader_start);
  ts->program=main_glCreateProgram(2,ts->vertex_shader,ts->fragment_shader);
  ts->mvp_location=glGetUniformLocation(ts->program[0],"MVP");
  vpos_location=glGetAttribLocation(ts->program[0],"vPos");
  vuv_location=glGetAttribLocation(ts->program[0],"vUV");
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,sizeof(vertices[0]), (void*) 0);
  glEnableVertexAttribArray(vuv_location);
  glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE,sizeof(vertices[0]), (void*) (sizeof(float) * 2));
  return ms;
}
