#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND=60;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *bomb=NULL;
SDL_Surface *fant=NULL;
SDL_Surface *meta=NULL;
SDL_Surface *ganar=NULL;
SDL_Surface *lose=NULL;
SDL_Surface *screen = NULL;
SDL_Event *event;



class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status

    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

SDL_Surface *load_image( std::string filename )
{
    return IMG_Load(filename.c_str());
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "PNG test", NULL );

    //If everything initialized fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( image );
    SDL_FreeSurface(bomb);
    SDL_FreeSurface(fant);
    SDL_FreeSurface(meta);
    SDL_FreeSurface(ganar);
    SDL_FreeSurface(lose);

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{

SDL_Rect posicion_image;

bool cap = true;
Timer fps;

posicion_image.x=300;
posicion_image.y=300;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    SDL_Surface* images[4];

    image = load_image("vida.png");
    images[0]=load_image( "run01.png" );
    images[1]=load_image( "run02.png" );
    images[2]=load_image( "run03.png" );
    images[3]=load_image( "run04.png" );
    bomb=load_image("bomba03.png");
    fant=load_image("volador02.png");
    meta=load_image("meta.png");
    ganar=load_image("ganaste.png");
    lose=load_image("lose.png");

    int a=300;
    int b=10;
    int c=-800;
    int f=200;
    int r=-800;
    int w=200;
    int pos_x=500, pos_y=200;
    int pos_x2=-20,  pos_y2=1;

    bool fin=false;
    bool cont = true;
    int x=0;
    while(cont){

        if(x>3){
            x=0;
        }
        //If there was a problem in loading the image
        if( images == NULL )
        {
            return 1;
        }

        SDL_Event event;
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
//                quit = true;

                cont = false;
            }

        }

        Uint8 *keystates = SDL_GetKeyState( NULL );

        if(keystates[SDLK_RIGHT]){
            a += 10;
        }

        if(keystates[SDLK_LEFT]){
            a -= 10;
        }

        if(keystates[SDLK_UP]){
            b -= 10;
        }

        if(keystates[SDLK_DOWN]){
            b += 10;
        }

        //Apply the surface to the screen
        apply_surface(0,0, image,screen);
        apply_surface( a, b, images[x], screen );
        apply_surface(pos_x, pos_y, bomb, screen);
        apply_surface(pos_x2, pos_y2, fant, screen);
        apply_surface(0, 400, meta, screen);
        apply_surface(c, f, ganar, screen);
        apply_surface(r, w, lose, screen);

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Wait 2 seconds
        if( ( cap == true ) && ( fps.get_ticks() < 100 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 100 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

        if(pos_x>-150&&pos_y<210)
        {
            pos_x=pos_x-10;
            pos_y=pos_y-5;
        }
        if(pos_x<=-150)
        {
            pos_y=pos_y+5;
        }
        if(pos_y>210)
        {

            pos_x=pos_x+5;
        }
        if(pos_x>500)
        {
            pos_x=500;
            pos_y=200;
        }


          if(pos_x2<500&&pos_y2<=270)
        {
            pos_x2=pos_x2+5;
        }
        if(pos_x2>=500&&pos_y2<270)
        {
            pos_y2=pos_y2+5;
        }
        if(pos_y2>=270&&pos_x2>-20)
        {
            pos_x2=pos_x2-5;
        }
        if(pos_x2<=-20)
        {
            pos_x2=-20;
            pos_y2=1;
        }

        if(a<=25 && b>=330 && r!=30 && fin==false)
        {
                c=0;
                f=-80;
                fin=true;
        }

        if(a>=pos_x-70 && a<=pos_x+70 && b>=pos_y-70 && b<=pos_y+70 && fin==false)
        {
            r=0;
            w=-115;
            fin=true;
        }
        if(a>=pos_x2-100 && a<=pos_x2+100 && b>=pos_y2-100 && b<=pos_y2+100 && fin==false)
        {
           r=0;
           w=-115;
           fin=true;
        }

        x++;
    }



    //Free the surface and quit SDL
    clean_up();

    return 0;
}
