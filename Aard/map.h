#ifndef _MAP_H_
#define _MAP_H_


#include <tmx.h>
unsigned int gid_clear_flags(unsigned int gid);
void set_color(int color);
void draw_polyline(double **points, double x, double y, int pointsc) ;
void draw_polygon(double **points, double x, double y, int pointsc) ;
void draw_layer(tmx_map *map, tmx_layer *layer);
void render_map(tmx_map *map);
void draw_image_layer(tmx_image *img);
void draw_objects(tmx_object_group *objgr) ;
int CheckTileSolid(int x,int y);

void LoadMap(const char* mapPath, const char* imagePath);

#endif