#include "main.h"
#include "linmath.h"

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

char* main_setup()
{
  return gc_new_string("Test GL");
}

GC_BLOCK_START(TestState)
  GLuint* buffers;
  GLuint* vertex_shader;
  GLuint* fragment_shader;
  GLuint* program;
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
  glDrawArrays(GL_TRIANGLES,0,3);
}

MainState* main_init()
{
  GLint vpos_location,vcol_location;
  TestState* ts=GC_NEW_BLOCK(TestState);
  MainState* ms=GC_NEW_BLOCK(MainState);
  ms->render=(MainRender)&render; 
  ms->data=ts;
  ts->buffers=main_glGenBuffers(1);
  glBindBuffer(GL_ARRAY_BUFFER,ts->buffers[0]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
  ts->vertex_shader=main_glCreateShader(GL_VERTEX_SHADER,&vertex_shader_text);
  ts->fragment_shader=main_glCreateShader(GL_FRAGMENT_SHADER,&fragment_shader_text);
  ts->program=main_glCreateProgram(2,ts->vertex_shader,ts->fragment_shader);
  ts->mvp_location=glGetUniformLocation(ts->program[0],"MVP");
  vpos_location=glGetAttribLocation(ts->program[0],"vPos");
  vcol_location=glGetAttribLocation(ts->program[0],"vCol");
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,sizeof(vertices[0]), (void*) 0);
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,sizeof(vertices[0]), (void*) (sizeof(float) * 2));
  return ms;
}
