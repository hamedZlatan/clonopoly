#include "ui.h"
#include "id.h"
#include "options.h"
#include "SDL/SDL_gfxPrimitives.h"


struct StatusArea
{
   Ifel* ifel;
   SDL_Surface* surface;
};

// our 'singleton'
static struct StatusArea* status;

static void DrawStatusArea(Ifel* i)
{
   struct StatusArea* s = (struct StatusArea*)i->data;

   //fprintf(stderr, "DrawOptionsMenu\n");

   // draw it
   roundedBoxRGBA(s->surface, 0, 0, s->surface->w, s->surface->h, 10, 0, 255, 0, 100);

   SDL_BlitSurface(s->surface, NULL, i->surface, NULL);
}

int CreateStatusArea(const SDL_Rect* loc)
{
   // if it doesn't exist yet, create it
   if (!status)
   {
      status = malloc(sizeof(struct StatusArea));
      if (!status)
         return 1;

      // create the surface
      status->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, loc->w, loc->h, 32, 0, 0, 0, 0);
      if (!status->surface)
      {
         fprintf(stderr, "Error creating status area surface\n");
         free(status);
         status = NULL;
         return 2;
      }

      // create the ifel
      status->ifel = malloc(sizeof(struct Ifel));
      if (!status->ifel)
      {
         free(status);
         SDL_FreeSurface(status->surface);
         return 3;
      }
      status->ifel->loc.x = loc->x;
      status->ifel->loc.y = loc->y;
      status->ifel->loc.w = loc->w;
      status->ifel->loc.h = loc->h;
      CreateIfel(ID_MENU_OPTIONS, NULL, status->ifel, IFEL_CUSTOM, DrawStatusArea);
      status->ifel->data = status;
   }

   // set it active
   status->ifel->active = 1;

   return 0;
}

void DeleteStatusArea(void)
{
   if (status)
   {
      DeleteIfel(NULL, status->ifel);
      free(status->ifel);
      free(status);
      status = NULL;
   }
}
