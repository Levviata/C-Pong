#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#define vertLen 4
static SDL_Vertex vert[vertLen];

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS) // Milliseconds per frame
#define UPDATE_INTERVAL (1000 / 20) // Every 20 ticks (approx. every 50 ms)
#define INPUT_INTERVAL (1000 / 30) // Every 30 ticks (approx. every 33.33 ms)

static int windowWidth, windowHeight;

static const char appName[] = "Levviata's C Based Pong";
static const char version[] = "v0.0.0";
static const char appIdentifier[] = "me.levviata.cpong";

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window *window;
    SDL_FRect left_player;
    SDL_FRect right_player;
} AppState;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState* state = malloc(sizeof(AppState));
    if (!state) {
        return SDL_APP_FAILURE; // Handle memory allocation failure
    }

    // nonsensical to set it here instead directly at creation but might change it later or do sum else so it stays like that
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
    if (!SDL_CreateWindowAndRenderer(appName, windowWidth, windowHeight, false, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = state; // Set the pointer to the allocated state
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

void render(void *appstate)
{
    AppState* state = (AppState*)appstate; // Cast the void pointer back to AppState

    SDL_GetWindowSize(state->window, &windowWidth, &windowHeight);

    state->left_player.h = 100;
    state->left_player.w = state->left_player.h / 2;
    state->left_player.x = ((windowWidth - state->left_player.w) / 2.0f); // Centering the rectangle
    state->left_player.y = ((windowHeight - state->left_player.h) / 2.0f); // Centering the rectangle

    // Clear the screen to black
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255); // Set black color
    SDL_RenderClear(state->renderer);

    // Set the draw color for the rectangle outline
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255); // Set red color

    SDL_RenderFillRect(state->renderer, &state->left_player);

    // Draw the rectangle outline
    SDL_RenderRect(state->renderer, &state->left_player);
    

    // Present the rendered frame
    SDL_RenderPresent(state->renderer);
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
    render(appstate);

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
    AppState* state = (AppState*)appstate;
    /* Seems like SDL destroys our render and window on app quit automatically?
    SDL_DestroyRenderer(renderer); // Destroy render 
    SDL_DestroyWindow(window); // Destroy window  */

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_free(appstate);
}

