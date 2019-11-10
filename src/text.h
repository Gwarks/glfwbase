#ifndef _TEXT_H_
#define _TEXT_H_

#include "../glad/glad.h"

typedef struct
{
    float x, y;
    float u, v;
} TextVertex;

void* text_createPannel(unsigned int,unsigned int);
void text_locate(unsigned int,unsigned int);
void text_putc(void*,unsigned char,char);
void text_puts(void*,unsigned char,char*);
void text_enable(void*,const GLfloat*);

#endif
