#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#define vertLen 4
static SDL_Vertex vert[vertLen];

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS) // Milliseconds per frame
#define UPDATE_INTERVAL (1000 / 20) // Every 20 ticks (approx. every 50 ms)
#define INPUT_INTERVAL (1000 / 30) // Every 30 ticks (approx. every 33.33 ms)

static SDL_Window *window = NULL; // the window where our rendering takes place in (and game also, its a window duh)
static SDL_Renderer *renderer = NULL; // the variable where we will mash shit-in to print a beautiful or disfigured picture (WHEN we actually tell the GPU to render it)

static SDL_FRect left_player = {0,0,0,0};
static SDL_FRect right_player = {0,0,0,0};

static int windowWidth, windowHeight;

static const char appName[] = "Levviata's C Based Pong";
static const char version[] = "v0.0.0";
static const char appIdentifier[] = "me.levviata.cpong";

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    windowWidth = 800, windowHeight = 600;
    // set our basic metadata, todo: expand later
    SDL_SetAppMetadata(appName, version, appIdentifier); 
        
    // Initalize subsystems, these can be manually closed with SDL_QuitSubSystem().
    // For when we close our app we use SDL_QuitSubSystem() for each subsystem and then SDL_Quit() for recleanup (as far as I know)
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    /* Create the window and the renderer! */
    if (!SDL_CreateWindowAndRenderer(appName, windowWidth, windowHeight, false, &window, &renderer)) {
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
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    left_player.h = 100;
    left_player.w = left_player.h / 2;
    left_player.x = ((windowWidth - left_player.w) / 2.0f); // Centering the rectangle
    left_player.y = ((windowHeight - left_player.h) / 2.0f); // Centering the rectangle

    // Clear the screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set black color
    SDL_RenderClear(renderer);

    // Set the draw color for the rectangle outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set red color

    SDL_RenderFillRect(renderer, &left_player);

    // Draw the rectangle outline
    SDL_RenderRect(renderer, &left_player);
    

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    static Uint32 lastFrameTime = 0;
    static Uint32 lastInputTime = 0;  // Time tracker for input
    static Uint32 lastUpdateTime = 0; // Time tracker for updates

    Uint32 frameStart = SDL_GetTicks();
    int frameTime;
/*
    // Process input every 30 ticks
    if (frameStart - lastInputTime >= INPUT_INTERVAL) {
        processInput();
        lastInputTime = frameStart; // Update last input time
    }
/*
    // Update every 20 ticks
    if (frameStart - lastUpdateTime >= UPDATE_INTERVAL) {
        update();
        lastUpdateTime = frameStart; // Update last update time
    }
*/
    // Render your graphics
    render();

    // Calculate frame time
    frameTime = SDL_GetTicks() - frameStart;

    // Delay to maintain 60 FPS
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }

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

