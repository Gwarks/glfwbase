#include "text.h"
#include "main.h"
#ifdef DEBUG
#include <stdio.h>
#endif

extern GLubyte _binary_bin_VGA_ROM_F08_start;
extern char _binary_bin_VGA_vertex_shader_start;
extern char _binary_bin_VGA_vertex_shader_end;
extern char _binary_bin_VGA_fragment_shader_start;
extern char _binary_bin_VGA_fragment_shader_end;

GC_BLOCK_START(Font)
  GLuint* vertex_shader;
  GLuint* fragment_shader;
  GLuint* program;
  GLuint* textures;
GC_BLOCK_CENTER
  GLint mvp_location;
GC_BLOCK_END

GC_BLOCK_START(Screen)
  Font* font;
GC_BLOCK_CENTER
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
GC_BLOCK_END

Font* font=0;

void getFont_gc(void* ptr)
{
  font=0;
  #ifdef DEBUG
  printf("Last font removed\n");
  #endif
}

Font* getFont(GC gc)
{
  if(font)return font;
  GLint vpos_location,vuv_location;
  Font* f=GC_NEW_BLOCK_GC(gc,Font,&getFont_gc);
  font=f;
  f->textures=main_glGenTextures(gc,2);
  glBindTexture(GL_TEXTURE_2D,f->textures[0]);
  glTexImage2D(GL_TEXTURE_2D,0,GL_R8UI,8,256,0,GL_RED_INTEGER,GL_UNSIGNED_BYTE,&_binary_bin_VGA_ROM_F08_start);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  f->vertex_shader=main_glCreateShader(gc,GL_VERTEX_SHADER,&_binary_bin_VGA_vertex_shader_start,
       &_binary_bin_VGA_vertex_shader_end-&_binary_bin_VGA_vertex_shader_start);
  f->fragment_shader=main_glCreateShader(gc,GL_FRAGMENT_SHADER,&_binary_bin_VGA_fragment_shader_start,
       &_binary_bin_VGA_fragment_shader_end-&_binary_bin_VGA_fragment_shader_start);
  f->program=main_glCreateProgram(gc,2,f->vertex_shader,f->fragment_shader);
  f->mvp_location=glGetUniformLocation(f->program[0],"MVP");
  vpos_location=glGetAttribLocation(f->program[0],"vPos");
  vuv_location=glGetAttribLocation(f->program[0],"vUV");
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,sizeof(TextVertex), (void*) 0);
  glEnableVertexAttribArray(vuv_location);
  glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE,sizeof(TextVertex), (void*) (sizeof(float) * 2));
  #ifdef DEBUG
  printf("Created Font\n");
  #endif
  return f;
}

void* text_createPannel(GC gc,unsigned int w,unsigned int h)
{
  Screen* s=GC_NEW_BLOCK(gc,Screen);
  s->font=getFont(gc);
  s->w=w;
  s->h=h;
  return s;
}
void text_locate(unsigned int,unsigned int);
void text_putc(void*,unsigned char,char);
void text_puts(void*,unsigned char,char*);
void text_enable(void* screen,const GLfloat* mvp)
{
  Screen* s=(Screen*)screen;
  glUseProgram(s->font->program[0]);
  glUniformMatrix4fv(s->font->mvp_location,1,GL_FALSE,mvp);
  glBindTexture(GL_TEXTURE_2D,s->font->textures[0]);
}
