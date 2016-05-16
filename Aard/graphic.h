#ifndef _GRAPHIC_H_
#define _GRAPHIC_H
void Init_Graphics(	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight);
void graphics_close();

SDL_Renderer* GetRenderer();
SDL_Window* GetWindow();
SDL_Texture* GetTextureBuffer();
void NextFrame();
void FrameDelay();
void graphics_close();
#endif