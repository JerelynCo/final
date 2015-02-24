/*
Compiled via command line using:
	g++ ballHell.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o final
*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <cstdio>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define PI 3.14159265

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_LENGTH = 600;

enum Terrain{
	GRASS,
	BRICK,
	WATER,
	EMPTY
};

//Texture wrapper class
class LTexture{
	public:
		//Initialize variables
		LTexture();
		
		//Deallocates memory
		~LTexture();
		
		//Loads image at specified path
		bool loadFromFile(std::string);
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText(std::string, SDL_Color);
		#endif
		
		//Deallocates texture
		void free();
		
		//Set color modulation
		void setColor(Uint8, Uint8, Uint8);
		
		//Set blending
		void setBlendMode(SDL_BlendMode);
		
		//Set alpha modulation
		void setAlpha(Uint8 alpha);
		
		//Renders texture at given point
		void render(int, int, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
		
		//Gets image dimensions
		int getWidth();
		int getHeight();
		
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		
		//Image dimensions
		int mWidth;
		int mHeight;
};

//Timer wrapper class
class LTimer{
	public:
		//Initializes variables
		LTimer();
		
		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();
		
		//Gets the timer's time
		Uint32 getTicks();
		
		//Checks the status of the timer
		bool isStarted();
		bool isPaused();
		
	private:
		//The clock time when the timer started
		Uint32 mStartTicks;
		
		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;
		
		//The timer status
		bool mPaused;
		bool mStarted;
};

struct Tile{
		static const int LENGTH = 30, WIDTH = 30;
		static const int ROWS = 20, COLS = 30;

		SDL_Rect t;
		const Uint8 m;
		
		Tile(SDL_Rect tile, Uint8 mobility):
			t(tile), m(mobility) {};
};

class Map{
		int tileMap[Tile::COLS][Tile::ROWS];
	public:
		Map();
		Tile* getTile(int, int);
		void render();
};

//Circle struct
struct Circle{
    int x, y;
    int r;
};

//Player class
class Player{
	public:
		//The dimensions of the player
		static const int PLAYER_WIDTH = 20, PLAYER_LENGTH = 20;
		
		//Maximum axis velocity of the player
		static const int PLAYER_VEL = 2;

		//Initializes the variables
		Player(int ID);

		//Takes key presses and adjusts the player's velocity
		void handleEvent(SDL_Event& e);

		//Moves the player
		void move(Map*);

		//Shows the player on the screen
		void render();

		Circle& getCollider();

        void shiftColliders();
		
    private:
		//The X and Y offsets of the player
		int mPosX, mPosY;

		//The velocity of the player
		int mVelX, mVelY;

		//Player ID
		int mPlayerID;

		//the collider for the circle
        Circle mCollider;

};

/*class Powers{
	public:

	private:

};*/

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Globally used font
TTF_Font* gFont = NULL;

//Textures
LTexture gPlayerOneTexture;
LTexture gPlayerTwoTexture;

LTexture gBombTexture;
LTexture gShieldTexture;
LTexture gLifeTexture;

LTexture gTimeTextTexture;
LTexture gPauseTextTexture;

///Vectors for Players
std::vector<Player> gPlayers;

//Global timer
LTimer gTimer;

LTexture gSpriteSheet;
Tile* tile[4];

int main(int argc, char *args[]){	
	gTimer.start();
	if(gTimer.isStarted()){
		printf("Start");
	}
	//Start up SDL and create window
	if(!init()){
		printf("Failed to initialize!\n");
	}else{
		//Load media
		if(!loadMedia()){
			printf("Failed to load media!\n");
		}else{
			//Main loop flag
			bool quit = false;
			
			//Show players
			Player playerOne(0);
			gPlayers.push_back(playerOne);
			Player playerTwo(1);
			gPlayers.push_back(playerTwo);
			
			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//In memory text stream
			std::stringstream timeText;

			//start global game timer
			gTimer.start();
			
			Map map;
			
			//While application is running
			while(!quit){									
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0){
					//User requests quit
					if(e.type == SDL_QUIT){
						quit = true;
					}
					//Reset start time on return keypress
					else if(e.type == SDL_KEYDOWN){
						//Pause/unpause
						if(e.key.keysym.sym == SDLK_p){
							if(gTimer.isPaused()){
								gTimer.unpause();
							}
							else{
								gTimer.pause();
							}
						}
					}
					for(int i = 0; i < gPlayers.size(); i++){
						gPlayers[i].handleEvent(e);
					}
				}
				
				if(gTimer.isStarted() && !gTimer.isPaused()){
					//Set text to be rendered
					timeText.str( "" );
					timeText << "Time: " << (gTimer.getTicks() / 1000);

					//Render text
					if(!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)){
						printf("Unable to render time texture!\n");
					}

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xB4, 0xB4, 0xB4, 0xFF);
					SDL_RenderClear(gRenderer);
					
					map.render();
					
					gTimeTextTexture.render((SCREEN_WIDTH-gTimeTextTexture.getWidth()) - 50, 0);
					gBombTexture.render(50,200);
					gShieldTexture.render(10,200);
					gLifeTexture.render(100,200);

					for(int i = 0; i < gPlayers.size(); i++){
						gPlayers[i].move(&map);
						gPlayers[i].render();
					}
				}

				else{
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderClear(gRenderer);
					gPauseTextTexture.render((SCREEN_WIDTH-gPauseTextTexture.getWidth())/2, (SCREEN_LENGTH-gPauseTextTexture.getHeight())/2);

				}
				
				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	
	//Free resources and close SDL
	close();
	
	return 0;
}

LTexture::LTexture(){
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture(){
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path){
	//Get rid of preexisting texture
	free();
	
	//The final texture
	SDL_Texture* newTexture = NULL;
	
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == NULL){
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}else{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
		
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		
		if(newTexture == NULL){
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}else{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		
		//Get rid of loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
	//Get rid of preexisting texture
	free();
	
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface == NULL){
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}else{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(mTexture == NULL){
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}else{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free(){
	//Free texture if it exists
	if(mTexture != NULL){
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	
	//Set clip rendering dimensions
	if(clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}	

int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}

LTimer::LTimer(){
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;
	
	mPaused = false;
	mStarted = false;
}

void LTimer::start(){
	//Start the timer
	mStarted = true;
	
	//Unpause the timer
	mPaused = false;
	
	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop(){
	//Stop the timer
	mStarted = false;
	
	//Unpause the timer
	mPaused = false;
	
	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause(){
	//If the timer is running and isn't already paused
	if(mStarted && !mPaused){
		//Pause the timer
		mPaused = true;
		
		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks()-mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause(){
	//If the timer is running and paused
	if(mStarted && mPaused){
		//Unpause the timer
		mPaused = false;
		
		//Reset the startng ticks
		mStartTicks = SDL_GetTicks()-mPausedTicks;
		
		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks(){
	//The actual timer time
	Uint32 time = 0;
	
	//If the timer is running
	if(mStarted){
		//If the timer is paused
		if(mPaused){
			//Return the number of ticks when the timer is paused
			time = mPausedTicks;
		}else{
			//Return the current time minus the start time
			time = SDL_GetTicks()-mStartTicks;
		}
	}
	
	return time;
}

bool LTimer::isStarted(){
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused(){
	//Timer is running and paused
	return mPaused && mStarted;
}

Map::Map(){
	static std::random_device tileType;
	
	for(int i = 0; i < Tile::ROWS; ++i){
		for(int j = 0; j < Tile::COLS; ++j){
			if(i%2 == 0 || j%2 == 0){
				if((i == 0 && j == 0)
					|| (i == 0 && j == Tile::COLS-1)
					|| (i == Tile::ROWS-1 && j == 0)
					|| (i == Tile::ROWS-1 && j == Tile::COLS-1)){
					tileMap[j][i] = 0;
				}else{
					tileMap[j][i] = tileType()%2;
				}
			}else{
				tileMap[j][i] = tileType()%2+2;
			}
		}
	}
}

Tile* Map::getTile(int x, int y){
	return tile[tileMap[x/Tile::WIDTH][y/Tile::LENGTH]];
}

void Map::render(){
	for(int i = 0; i < Tile::ROWS; ++i){
		for(int j = 0; j < Tile::COLS; ++j){
			gSpriteSheet.render(j*Tile::WIDTH, i*Tile::LENGTH, &(tile[tileMap[j][i]]->t));
		}
	}
}

Player::Player(int ID){
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //for the colliders
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.r = PLAYER_WIDTH/2;

    mPlayerID = ID;
}

void Player::handleEvent(SDL_Event& e){
	//Controls: WASD and Arrow keys
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
    	if(mPlayerID == 0){
	        switch( e.key.keysym.sym ){
	            case SDLK_UP: mVelY -= PLAYER_VEL; break;
	            case SDLK_DOWN: mVelY += PLAYER_VEL; break;
	            case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
	            case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
	        }
    	}
    	if(mPlayerID == 1){
	        switch( e.key.keysym.sym ){
	            case SDLK_w: mVelY -= PLAYER_VEL; break;
	            case SDLK_s: mVelY += PLAYER_VEL; break;
	            case SDLK_a: mVelX -= PLAYER_VEL; break;
	            case SDLK_d: mVelX += PLAYER_VEL; break;
	        }
    	}
    }
    //If a key was released
    else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        if(mPlayerID == 0){
        	//Adjust the velocity
	        switch( e.key.keysym.sym ){
	            case SDLK_UP: mVelY += PLAYER_VEL; break;
	            case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
	            case SDLK_LEFT: mVelX += PLAYER_VEL; break;
	            case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
	        }
    	}
    	if(mPlayerID == 1){
        	//Adjust the velocity
	        switch( e.key.keysym.sym ){
	            case SDLK_w: mVelY += PLAYER_VEL; break;
	            case SDLK_s: mVelY -= PLAYER_VEL; break;
	            case SDLK_a: mVelX += PLAYER_VEL; break;
	            case SDLK_d: mVelX -= PLAYER_VEL; break;
	        }
    	}
    }
}

void Player::move(Map* map){
    //Move the Player left or right
    mPosX += mVelX;
    shiftColliders();
	
    //If the Player went too far to the left or right
    if((mPosX < 0) || (mPosX + PLAYER_WIDTH > SCREEN_WIDTH)
		|| ((map->getTile(mPosX, mPosY))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY))->m > 0)
		|| ((map->getTile(mPosX, mPosY+PLAYER_LENGTH))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY+PLAYER_LENGTH))->m > 0)){
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Move the Player up or down
    mPosY += mVelY;
    shiftColliders();
    //If the Player went too far up or down
    if((mPosY < 0)||(mPosY + PLAYER_LENGTH > SCREEN_LENGTH)
		|| ((map->getTile(mPosX, mPosY))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY))->m > 0)
		|| ((map->getTile(mPosX, mPosY+PLAYER_LENGTH))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY+PLAYER_LENGTH))->m > 0)){
        //Move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

Circle& Player::getCollider(){
    return mCollider;
}

void Player::shiftColliders(){
	//Align collider to center of ball
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}

void Player::render(){
    //Show the Player
    if(mPlayerID == 0){
    	gPlayerOneTexture.render(mPosX, mPosY);
    }
	if(mPlayerID == 1){
		gPlayerTwoTexture.render(mPosX, mPosY);
	}

}

bool init(){
	//Initialization flag
	bool success = true;
	
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}else{
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Warning: Linear texture filtering not enabled!");
		}
		
		//Create window
		gWindow = SDL_CreateWindow("Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_LENGTH, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}else{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			
			if(gRenderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}else{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)){
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				 //Initialize SDL_ttf
				if(TTF_Init() == -1){
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	
	return success;
}

bool loadMedia(){
	//Loading success flag
	bool success = true;
	//load font
	gFont = TTF_OpenFont("Assets/lazy.ttf", 28);
	SDL_Color textColor = {0xD0, 0xD0, 0xD0, 0xFF};
	if(gFont == NULL){
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else{
		if(!gPauseTextTexture.loadFromRenderedText("Paused. Press P to Resume.", textColor) )
		{
			printf( "Unable to render pause text texture!\n" );
			success = false;
		}
	}
	
	if(!gSpriteSheet.loadFromFile("Assets/terrain.png")){
		printf("Failed to load terrain sprite sheet!\n");
		success = false;
	}else{
		tile[GRASS] = new Tile({0, 0, 32, 32}, 0);
		tile[BRICK] = new Tile({32, 0, 32, 32}, 2);
		tile[WATER] = new Tile({64, 0, 32, 32}, 1);
		tile[EMPTY] = new Tile({96, 0, 32, 32}, 3);
	}

	//Load player textures
	if(!gPlayerOneTexture.loadFromFile("Assets/p1.png")){
		printf("Failed to load player 1 texture!\n");
		success = false;
	}
	if(!gPlayerTwoTexture.loadFromFile("Assets/p2.png")){
		printf("Failed to load player 2 texture!\n");
		success = false;
	}

	//Load power up textures
	if(!gBombTexture.loadFromFile("Assets/bomb.png")){
		printf("Failed to load bomb texture!\n");
		success = false;
	}
	if(!gShieldTexture.loadFromFile("Assets/shield.png")){
		printf("Failed to load shield texture!\n");
		success = false;
	}
	if(!gLifeTexture.loadFromFile("Assets/life.png")){
		printf("Failed to load life texture!\n");
		success = false;
	}
	
	return success;
}

bool checkCollision(Circle& c1, Circle& c2){
	if(sqrt(pow(c1.x-c2.x, 2)+pow(c1.y-c2.y, 2)) < c1.r+c2.r){
		return true;
	}
	return false;
}

void close(){
	//Free loaded images
	gPauseTextTexture.free();
	gTimeTextTexture.free();
	gBombTexture.free();
	gPlayerOneTexture.free();
	gPlayerTwoTexture.free();
	gSpriteSheet.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}