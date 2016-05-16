#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <chipmunk.h>
#include <jansson.h>
#include "globals.h"
#include "graphic.h"
#include "map.h"

unsigned int gid_clear_flags(unsigned int gid) {
	return gid & TMX_FLIP_BITS_REMOVAL;
}
void set_color(int color) {
	unsigned char r, g, b;

	r = (color >> 16) & 0xFF;
	g = (color >>  8) & 0xFF;
	b = (color)       & 0xFF;

	SDL_SetRenderDrawColor(GetRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
}
void draw_polyline(double **points, double x, double y, int pointsc) {
	int i;
	for (i=1; i<pointsc; i++) {
		SDL_RenderDrawLine(GetRenderer(), x+points[i-1][0], y+points[i-1][1], x+points[i][0], y+points[i][1]);
	}
}

void draw_polygon(double **points, double x, double y, int pointsc) {
	draw_polyline(points, x, y, pointsc);
	if (pointsc > 2) {
		SDL_RenderDrawLine(GetRenderer(), x+points[0][0], y+points[0][1], x+points[pointsc-1][0], y+points[pointsc-1][1]);
	}
}
void draw_layer(tmx_map *map, tmx_layer *layer)
{
	unsigned long i, j;
	unsigned int gid;
	float op;
	tmx_tileset *ts;
	tmx_image *im;
	SDL_Rect srcrect, dstrect;
	SDL_Texture* tileset;
	op = layer->opacity;
	for (i=0; i<map->height; i++) {
		for (j=0; j<map->width; j++) {
			gid = gid_clear_flags(layer->content.gids[(i*map->width)+j]);
			if (map->tiles[gid] != NULL) {
				ts = map->tiles[gid]->tileset;
				im = map->tiles[gid]->image;
				srcrect.x = map->tiles[gid]->ul_x;
				srcrect.y = map->tiles[gid]->ul_y;
				srcrect.w = dstrect.w = ts->tile_width;
				srcrect.h = dstrect.h = ts->tile_height;
				dstrect.x = j*ts->tile_width - globals.camera.x;  
				dstrect.y = i*ts->tile_height - globals.camera.y;
				if (im) {
					tileset = (SDL_Texture*)im->resource_image;
				}
				else {
					tileset = (SDL_Texture*)ts->image->resource_image;
				}
				SDL_RenderCopy(GetRenderer(), tileset, &srcrect, &dstrect);
			}
		}
	}
}
void draw_image_layer(tmx_image *img)
{
		SDL_Rect dim;

	dim.x = dim.y = 0;
	SDL_QueryTexture((SDL_Texture*)img->resource_image, NULL, NULL, &(dim.w), &(dim.h));

	SDL_RenderCopy(GetRenderer(), (SDL_Texture*)img->resource_image, NULL, &dim);
}

void draw_objects(tmx_object_group *objgr) {
	SDL_Rect rect;
	set_color(objgr->color);
	tmx_object *head = objgr->head;
	/* FIXME line thickness */
	while (head) {
		if (head->visible) {
			if (head->shape == S_SQUARE) {
				rect.x = head->x- globals.camera.x;  
				rect.y = head->y- globals.camera.y;
				rect.w = head->width;  
				rect.h = head->height;
				SDL_RenderDrawRect(GetRenderer(), &rect);
			} else if (head->shape  == S_POLYGON) {
				draw_polygon(head->points, head->x, head->y, head->points_len);
			} else if (head->shape == S_POLYLINE) {
				draw_polyline(head->points, head->x, head->y, head->points_len);
			} else if (head->shape == S_ELLIPSE) {
				/* FIXME: no function in SDL2 */
			}
		}
		head = head->next;
	}
}


void render_map(tmx_map *map)
{
	tmx_layer *layers = map->ly_head;
	SDL_SetRenderTarget(GetRenderer(), globals.mapTexture);

	SDL_RenderClear(GetRenderer());

	while (layers) {
		if (layers->visible) {
			if (layers->type == L_OBJGR) {
				draw_objects(layers->content.objgr);
			} else if (layers->type == L_IMAGE) {
				draw_image_layer(layers->content.image);
			} else if (layers->type == L_LAYER) {
				draw_layer(map, layers);
			}
		}
		layers = layers->next;
	}

	SDL_SetRenderTarget(GetRenderer(), NULL);
	SDL_RenderCopy(GetRenderer(),globals.mapTexture,NULL,&globals.mapRect);
}

void LoadMap(const char* mapPath, const char* imagePath)
{
	
	if(!(globals.map = tmx_load(mapPath)))
	{
		printf("Error with loading map");
		exit(1);
	}
	globals.map->ts_head->image->resource_image = IMG_LoadTexture(GetRenderer(), imagePath);
	globals.mapRect.w = globals.map->width  * globals.map->tile_width;
	globals.mapRect.h = globals.map->height * globals.map->tile_height;
	globals.mapRect.x = 0;  globals.mapRect.y = 0;

	tmx_layer *layers = globals.map->ly_head;
	while (layers)
	{
		if(strcmp(layers->name,"SolidLayer") == 0)
		{
			globals.solidLayer= layers;
			break;
		}
		layers = layers->next;
	}
	globals.tileHeight = globals.map->tile_height;
	globals.tileWidth = globals.map->tile_width;
	if(globals.mapTexture != NULL)
		SDL_DestroyTexture(globals.mapTexture);
	globals.mapTexture = SDL_CreateTexture(GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,globals.map->width  * globals.map->tile_width,
		globals.map->height * globals.map->tile_height);

}
int CheckTileSolid(int x,int y)
{
	if(globals.solidLayer->content.gids[y*globals.map->width+x] != 0){
		return true;	
	}
    return false;
}