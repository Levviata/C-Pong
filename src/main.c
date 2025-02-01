#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#define vertLen 4
static SDL_Vertex vert[vertLen];

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS) // Milliseconds per frame
#define UPDATE_INTERVAL (1000 / 20) // Every 20 ticks (approx. every 50 ms)
#define INPUT_INTERVAL (1000 / 30) // Every 30 ticks (approx. every 33.33 ms)

// Metadata variables
static const char appName[] = "Levviata's C Based Pong";
static const char version[] = "v0.0.0";
static const char appIdentifier[] = "me.levviata.cpong";

static int windowWidth, windowHeight;

static const int padding = 10;
static const int speed = 1;

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window *window;
    SDL_FRect left_player;
    double yPlayer;
    double yLastPlayer;
    SDL_FRect right_ai_player;
    double yAI;
    double yLastAI;
    float smoothing;
} AppState; // this struct passes around ONLY variables that change during runtime (to not bloat the hardware's memory).

double lerp(double varStart, double varEnd, float t) {
    double result;
    result = varStart + (varEnd - varStart) * t;
    return result;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState* state = malloc(sizeof(AppState));
    if (!state) {
        return SDL_APP_FAILURE; // Handle memory allocation failure
    }

    // nonsensical to set it here instead directly at creation but might change it later or do sum else so it stays like that
    windowWidth = 800, windowHeight = 600; 
    
    state->yPlayer = 0, state->yAI = 0;

    state->yPlayer = 0, state->yLastAI = 0;

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
    AppState* state = (AppState*)appstate; // Cast the void pointer back to AppState

    Uint32 frameStart = SDL_GetTicks();
    int frameTime;

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; 
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_UP: // Arrow Up Key
            case SDL_SCANCODE_W:  // 'W' key
                state->yPlayer -= 5;
                // e.g. state->player.y -= 5; // Move up
                break;

            case SDL_SCANCODE_DOWN: // Arrow Down Key
            case SDL_SCANCODE_S:    // 'S' key
                state->yPlayer += 5;
                // e.g. state->player.y += 5; // Move down
                break;

            default:
                break;
        }
    }

    // Calculate frame time
    frameTime = SDL_GetTicks() - frameStart;

    // Delay to maintain 60 FPS
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }


    return SDL_APP_CONTINUE;
}

void update(AppState *state) 
{
    SDL_GetWindowSize(state->window, &windowWidth, &windowHeight); // todo: if window is the same as last tick continue (so dont get the thing since it's size didnt change)

    state->left_player.h = 100;
    state->left_player.w = state->left_player.h / 4; // make us a quarter in width compared to our height (so we are a slim rectangle)
    state->left_player.x = padding;
    state->left_player.y = ((windowHeight - state->left_player.h) / 2.0f) + state->yPlayer;

    state->right_ai_player.h = 100;
    state->right_ai_player.w = state->right_ai_player.h / 4; // make us a quarter in width compared to our height (so we are a slim rectangle)
    state->right_ai_player.x = (windowWidth - state->right_ai_player.w) - padding;
    state->right_ai_player.y = ((windowHeight - state->right_ai_player.h) / 2.0f) + state->yAI;

    state->smoothing += speed;
    if (state->smoothing > 1.0f) state->smoothing = 1.0f; // Clamp smoothing to avoid overflow

    state->yLastPlayer = state->yPlayer;
    state->yLastAI = state->yAI;
}

void render(AppState *state)
{
    // Clear the screen to black
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255); // Set black color
    SDL_RenderClear(state->renderer);

    // Set the draw color for the rectangle outline
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255); // Set red color

    // Fill our left player
    SDL_RenderFillRect(state->renderer, &state->left_player);
    // Render our left player
    SDL_RenderRect(state->renderer, &state->left_player);

    // Fill our right player
    SDL_RenderFillRect(state->renderer, &state->right_ai_player);
    // Render our right player
    SDL_RenderRect(state->renderer, &state->right_ai_player);

    // Present the rendered frame
    SDL_RenderPresent(state->renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState* state = (AppState*)appstate; // Cast the void pointer back to AppState

    Uint32 frameStart = SDL_GetTicks();
    int frameTime;

    // Render your graphics
    update(state);
    render(state);

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

    AppState* state = (AppState*)appstate; // get the "appstate" pointer so that we can destroy it
    SDL_free(appstate); // destroy appstate pointer
}

