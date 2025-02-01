#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define vertLen 4
static SDL_Vertex vert[vertLen];

static SDL_Window *window = NULL; // the window where our rendering takes place in (and game also, its a window duh)
static SDL_Renderer *renderer = NULL; // the variable where we will mash shit-in to print a beautiful or disfigured picture (WHEN we actually tell the GPU to render it)

static const char appName[] = "Levviata's C Based Pong";
static const char version[] = "v0.0.0";
static const char appIdentifier[] = "me.levviata.cpong";

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    // set our basic metadata, todo: expand later
    SDL_SetAppMetadata(appName, version, appIdentifier); 
        
    // Initalize subsystems, these can be manually closed with SDL_QuitSubSystem().
    // For when we close our app we use SDL_QuitSubSystem() for each subsystem and then SDL_Quit() for recleanup (as far as I know)
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    /* Create the window and the renderer! */
    if (!SDL_CreateWindowAndRenderer(appName, 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

void render()
{
    // Setup the vertices for the rectangle (top-left, top-right, bottom-left, bottom-right)
    vert[0] = (SDL_Vertex){.position = {200, 150}, .color = {255, 0, 0, 255}}; // Top-left
    vert[1] = (SDL_Vertex){.position = {600, 150}, .color = {0, 255, 0, 255}}; // Top-right
    vert[2] = (SDL_Vertex){.position = {200, 450}, .color = {0, 0, 255, 255}}; // Bottom-left
    vert[3] = (SDL_Vertex){.position = {600, 450}, .color = {255, 255, 0, 255}}; // Bottom-right

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer); // Clear the canvas/render screen

    SDL_RenderGeometry(renderer, NULL, vert, vertLen, NULL, 0);

    SDL_RenderPresent(renderer);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    //processInput();
    //update();
    render();

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* Seems like SDL destroys our render and window on app quit automatically?
    SDL_DestroyRenderer(renderer); // Destroy render 
    SDL_DestroyWindow(window); // Destroy window  */

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    //SDL_free(appstate);
}

