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
"#version 130\n"
"varying vec3 color;\n"
"uniform isampler2D myTextureSampler;\n"
"void main()\n"
"{\n"
"int i=3;i=(i<<2)&8;\n"
"    gl_FragColor = vec4(vec3(float(texture(myTextureSampler,color.gb).r/255.)),0.0);\n"
"    gl_FragColor += vec4(color, 1.0);\n"
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
  glDrawArrays(GL_TRIANGLES,0,3);
}

GLubyte* genXORbitmap()
{
  GLubyte* data=gc_new(0,256*256*sizeof(GLubyte),0);
  GLubyte* p=data;
  for(int y=0;y<256;y++)
  {
    for(int x=0;x<256;x++)
    {
      *(p++)=x^y;
    }
  }
  return data;
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
  ts->textures=main_glGenTextures(1);
  glBindTexture(GL_TEXTURE_2D,ts->textures[0]);
  glTexImage2D(GL_TEXTURE_2D,0,GL_R8UI,256,256,0,GL_RED_INTEGER,GL_UNSIGNED_BYTE,genXORbitmap());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
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
