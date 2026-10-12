#ifndef GL_FONT_H
#define GL_FONT_H

#define FONT_SIZE 14

void font_init();
void font_clean();
void font_render(float x, float y, const char* text);
int  font_get_height();
int  font_get_width(const char* text);

#endif /* GL_FONT_H */
