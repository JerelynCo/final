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
const int SCREEN_WIDTH = 1170;
const int SCREEN_LENGTH = 600;
const int SCOREBOARD_LENGTH = 60;
const int PLAYFIELD_LENGTH = SCREEN_LENGTH-SCOREBOARD_LENGTH;

enum Terrain{
	GRASS, BRICK, WATER, EMPTY 
};

enum Direction{
	SOUTH, WEST, NORTH, EAST
};

enum Controls{
	UP, LEFT, DOWN, RIGHT, SHOOT
};

enum PowerUps{
	LIFE, BOMB, SHIELD, BULLETUPGRADE
};

struct Circle{
    int x, y;
    int r;
};

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
		void render(SDL_Rect*, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
		void render(int, int, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
		
		//Gets image dimensions
		int getWidth();
		int getLength();
		
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		
		//Image dimensions
		int mWidth;
		int mLength;
};

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

	SDL_Rect t;
	const Uint8 m;
	
	Tile(SDL_Rect tile, Uint8 mobility):
		t(tile), m(mobility) {};
};

struct Map{
	static const int ROWS = PLAYFIELD_LENGTH/Tile::LENGTH, COLS = SCREEN_WIDTH/Tile::WIDTH;
	
	Map();
	
	Tile* tile(int, int);
	void hit(int, int);
	void render();
	
	private:
		Tile* map[COLS][ROWS];
		SDL_Rect t;
};

struct Player{
	static const int WIDTH = 20, LENGTH = 20;
	static const int VEL = 2;
	int life = 3;

	Player(LTexture* texture, int x, int y, SDL_Scancode up, SDL_Scancode left, SDL_Scancode down, SDL_Scancode right, SDL_Scancode shoot, int lifeAvailableYPos):
		playerRect{x, y, texture->getWidth(), texture->getLength()},
		dir(SOUTH), playerTex(texture), collider{x,y,WIDTH/2}, lifeYPos(lifeAvailableYPos), con{up, left, down, right, shoot} {};

	void act(const Uint8*);
	void act(SDL_Scancode);
	void move(int, int);
	Circle& getCollider();
	void shiftColliders();
	void render();
	void renderLifeTexture();
	void activatePowerUp(int id);
	
    private:
		int dir;
		int lifeYPos;
		SDL_Rect playerRect;
		LTexture* playerTex;
		LTexture* playerLifeTex;
		Circle collider;
		SDL_Scancode con[5];
		
		void shoot();
};

struct PowerUp{
	static const int WIDTH = 20, LENGTH = 20;

	PowerUp(LTexture* texture, int id):
		powerUpRect{rand()%(SCREEN_WIDTH - WIDTH), rand()%(SCREEN_LENGTH-SCOREBOARD_LENGTH) - LENGTH, texture->getWidth(), texture->getLength()},
		powerUpTex(texture), id(id){};

		void render();
		Circle& getCollider();
		int getPowerUpID();

	private:
		int id;
		Circle collider;
		SDL_Rect powerUpRect;
		LTexture* powerUpTex;

};

struct Bullet{
	static const int LENGTH = 5, WIDTH = 5, VEL = 5;
	double x, y;
	int dir;
	
	Bullet(double xStart, double yStart, int direction):
		x(xStart), y(yStart), dir(direction){};
		//bullet{(int) x, (int) y, WIDTH, LENGTH}{}
		
	bool move();
	void render();
	/*private:
		SDL_Rect bullet;*/
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Checks collision between c1 and c2
bool checkCollision(Circle& c1, Circle& c2);
bool checkCollision(Circle& c1, SDL_Rect r);

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
LTexture gBulletUpgradeTexture;

LTexture gTimeTextTexture;
LTexture gPauseTextTexture;

LTexture gLifeAvailableTexture;

std::vector<Map> gLevels;
int gLevel = 0;

std::vector<Player> gPlayers;
std::vector<Bullet> gBullets;
std::vector<PowerUp> gPowerUps;

LTimer gTimer;
LTimer gDisplayPowerUpsTimer;

LTexture gSpriteSheet;
Tile* gTiles[4];

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
			
			//Create players
			gPlayers.emplace_back(&gPlayerOneTexture, 5, 5, SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_C, 15);
			gPlayers.emplace_back(&gPlayerTwoTexture, SCREEN_WIDTH-Player::WIDTH-5, PLAYFIELD_LENGTH-Player::LENGTH-5, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_N, 37);
			
			//Power ups variables
			static const int NSETS = 4;
			static const int NPOWERUPS = 4;
			static const int POWERUPSDURATION = 3;
			int powerUpsArr[NSETS+1][NPOWERUPS] = {{LIFE, BOMB, SHIELD, BULLETUPGRADE}, {0, 0, 3, 2}, {0, 2, 0, 0}, {3, 0, 1, 0}, {0, 0, 2, 0}};	
			bool switchUpgrades = true;
			int powerUpsTime[NSETS] = {2, 7, 12, 16};
			int set = 1;
			LTexture powerUpsTex[NPOWERUPS] = {gLifeTexture, gBombTexture, gShieldTexture, gBulletUpgradeTexture};

			//Event handler
			SDL_Event event;
			const Uint8* state = SDL_GetKeyboardState(NULL);

			//Set text color as black
			SDL_Color textColor = {255, 255, 255, 255};

			//In memory text stream
			std::stringstream timeText;
			
			const int LEVELS = 1;
			for(int i = 0; i < LEVELS; ++i){
				gLevels.emplace_back();
			}
			
			//start global game timer
			gTimer.start();
			
			//While application is running
			while(!quit){									
				while(SDL_PollEvent(&event)){
					//User requests quit
					if(event.type == SDL_QUIT){
						quit = true;
					}
					//Pause/Unpause
					if(event.type == SDL_KEYDOWN){
						if(event.key.keysym.sym == SDLK_p){
							if(gTimer.isPaused()){
								gTimer.unpause();
							}else{
								gTimer.pause();
							}

						}else if(event.key.repeat == 0){
							for(int i = 0; i < gPlayers.size(); ++i){
								gPlayers[i].act(event.key.keysym.scancode);
							}
						}
					}
				}
				
				for(int i = 0; i < gPlayers.size(); ++i){
					gPlayers[i].act(state);
				}

				if(switchUpgrades && set <= NSETS){
					for(int i = 0; i < NPOWERUPS; i++){
						for(int j = 0; j < powerUpsArr[set][i]; j++){
							gPowerUps.emplace_back(&powerUpsTex[i], i);
						}
					}
					switchUpgrades = false;
				}
				
				if(gTimer.isStarted() && !gTimer.isPaused()){
					//Viewports
					SDL_Rect scoreboard = {0, 0, SCREEN_WIDTH, SCOREBOARD_LENGTH};
					SDL_Rect playfield = {0, SCOREBOARD_LENGTH, SCREEN_WIDTH, PLAYFIELD_LENGTH};

					SDL_RenderClear(gRenderer);
					SDL_RenderSetViewport(gRenderer, &scoreboard);

					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x80, 0x80, 0xFF);
					SDL_RenderFillRect(gRenderer, &scoreboard);

					//Set text to be rendered
					timeText.str("");
					timeText << "Time: " << gTimer.getTicks()/1000;

					//Render text
					if(!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)){
						printf("Unable to render time texture!\n");
					}

					gTimeTextTexture.render((SCREEN_WIDTH-gTimeTextTexture.getWidth())/2, (SCOREBOARD_LENGTH-gTimeTextTexture.getLength())/2);
					gPlayerOneTexture.render(6*SCREEN_WIDTH/7-2*Player::WIDTH, 15);
					gPlayerTwoTexture.render(6*SCREEN_WIDTH/7-2*Player::WIDTH, 37);

					for(int i = 0; i < gPlayers.size(); i++){
						gPlayers[i].renderLifeTexture();
					}

					SDL_RenderSetViewport(gRenderer, &playfield);				
					
					gLevels[gLevel].render();
					
					if(gTimer.getTicks()/1000 > powerUpsTime[set-1] && set <= NSETS){
						for(int i = 0; i < gPowerUps.size(); i++){
							gPowerUps[i].render();
							if(!gDisplayPowerUpsTimer.isStarted()){
								gDisplayPowerUpsTimer.start();
							}
							if(gDisplayPowerUpsTimer.getTicks()/1000 > POWERUPSDURATION){
								gPowerUps.clear();
								gDisplayPowerUpsTimer.stop();
								switchUpgrades = true;
								set++;
								printf("success\n");
							}
						}
					}

					for(int i = 0; i < gBullets.size(); ++i){
						if(gBullets[i].move()){
							gBullets[i].render();
						}else{
							gBullets.erase(gBullets.begin()+i);
						}
					}
					
					for(int i = 0; i < gPlayers.size(); i++){
						gPlayers[i].render();
						for(int j = 0; j < gPowerUps.size(); j++){
							if(checkCollision(gPlayers[i].getCollider(), gPowerUps[j].getCollider())){
						    	gPowerUps.erase(gPowerUps.begin()+j);
						    	gPlayers[i].activatePowerUp(gPowerUps[j].getPowerUpID());
						    }
						}
					}
				}

				else{
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderClear(gRenderer);
					gPauseTextTexture.render(new SDL_Rect{(SCREEN_WIDTH-gPauseTextTexture.getWidth())/2,
						(SCREEN_LENGTH-gPauseTextTexture.getLength())/2, 0, 0});

				}
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}

LTexture::LTexture(){
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mLength = 0;
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
			mLength = loadedSurface->h;
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
			mLength = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;
}
#endif

void LTexture::free(){
	//Free texture if it exists
	if(mTexture != NULL){
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mLength = 0;
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

void LTexture::render(SDL_Rect* dst, SDL_Rect* src, double angle, SDL_Point* center, SDL_RendererFlip flip){
	//Set clip rendering dimensions
	if(dst->w == 0 || dst->h == 0){
		dst->w = mWidth;
		dst->h = mLength;
	}
	
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, src, dst, angle, center, flip);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mLength};
	
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

int LTexture::getLength(){
	return mLength;
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
	std::random_device type;
	for(int i = 0; i < ROWS; ++i){
		for(int j = 0; j < COLS; ++j){
			if(i%2 == 0 || j%2 == 0){
				if((i == 0 && j == 0)
					|| (i == 0 && j == COLS-1)
					|| (i == ROWS-1 && j == 0)
					|| (i == ROWS-1 && j == COLS-1)){
					map[j][i] = gTiles[GRASS];
				}else{
					map[j][i] = gTiles[type()%2];
				}
			}else{
				map[j][i] = gTiles[type()%2+2];
			}
		}
	}
	
	t = {0, 0, Tile::WIDTH, Tile::LENGTH};
}

Tile* Map::tile(int x, int y){
	if(x >= 0 && x < Tile::WIDTH*Map::COLS
		&& y >= 0 && y < Tile::LENGTH*Map::ROWS){
		return map[x/Tile::WIDTH][y/Tile::LENGTH];
	}else{
		return gTiles[EMPTY];
	}
}

void Map::hit(int x, int y){
	if(tile(x, y) == gTiles[BRICK]){
		map[x/Tile::WIDTH][y/Tile::LENGTH] = gTiles[GRASS];
	}
}

void Map::render(){
	for(int i = 0; i < ROWS; ++i){
		for(int j = 0; j < COLS; ++j){
			t.x = j*Tile::WIDTH;
			t.y = i*Tile::LENGTH;
			gSpriteSheet.render(&t, &(map[j][i]->t));
		}
	}
}

void Player::act(const Uint8* state){
	if(state[con[UP]]){move(0, -VEL); dir = NORTH;}
	if(state[con[LEFT]]){move(-VEL, 0); dir = WEST;}
	if(state[con[DOWN]]){move(0, VEL); dir = SOUTH;}
	if(state[con[RIGHT]]){move(VEL, 0); dir = EAST;}
}

void Player::act(SDL_Scancode key){
	if(key == con[SHOOT]){shoot();}
}

void Player::move(int vx, int vy){
    playerRect.x += vx;
    shiftColliders();
	
    if((gLevels[gLevel].tile(playerRect.x, playerRect.y))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x, playerRect.y+LENGTH))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y+LENGTH))->m > 0
		|| (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) ){
        playerRect.x -= vx;
        shiftColliders();
    }

    playerRect.y += vy;
    shiftColliders();

    if((gLevels[gLevel].tile(playerRect.x, playerRect.y))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x, playerRect.y+LENGTH))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y+LENGTH))->m > 0
		|| (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) ){
        playerRect.y -= vy;
        shiftColliders();
    }
}
Circle& Player::getCollider(){
	return collider;
}

void Player::shiftColliders(){
	collider.x = playerRect.x;
	collider.y = playerRect.y;
}

void Player::shoot(){
	gBullets.emplace_back(playerRect.x+WIDTH/2, playerRect.y+LENGTH/2, dir);
}

void Player::render(){
    playerTex->render(&playerRect, NULL, 90*dir);
}

void PowerUp::render(){
	powerUpTex->render(&powerUpRect);
}

int PowerUp::getPowerUpID(){
	return id;
}

Circle& PowerUp::getCollider(){
    return collider;
}

void Player::renderLifeTexture(){
	for(int i = 0; i < life; i++){	
		gLifeAvailableTexture.render(6*SCREEN_WIDTH/7+WIDTH*i, lifeYPos);
	}
}
void Player::activatePowerUp(int id){
	switch(id){
		case LIFE:		
			printf("life\n");
			life++;
			break;
		case BOMB:
			printf("bomb\n");
			break;
		case SHIELD:
			printf("shield\n");
			break;
		case BULLETUPGRADE:
			printf("bullet upgraded\n");
			break;
	}
}

bool Bullet::move(){
	x += VEL*cos(PI*(dir+1)/2);
	y += VEL*sin(PI*(dir+1)/2);
	
	if(gLevels[gLevel].tile(x, y) == gTiles[BRICK]){
		gLevels[gLevel].hit(x, y);
		return false;
	}else if(gLevels[gLevel].tile(x+WIDTH, y+LENGTH) == gTiles[BRICK]){
		gLevels[gLevel].hit(x+WIDTH, y+LENGTH);
		return false;
	}else if(gLevels[gLevel].tile(x, y) == gTiles[EMPTY]){
		return false;
	}/*else if(checkCollision(bullet, Player::gPlayers[1].getCollider())){
		return false;
	}*/
	return true;
}

void Bullet::render(){
	SDL_Rect bullet{(int) x, (int) y, WIDTH, LENGTH};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &bullet);
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
	gFont = TTF_OpenFont("Assets/ostrich.ttf", 50);
	SDL_Color textColor = {0xD0, 0xD0, 0xD0, 0xFF};
	if(gFont == NULL){
		printf("Failed to load ostrich font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else{
		if(!gPauseTextTexture.loadFromRenderedText("Paused. Press P to Resume.", textColor) ){
			printf( "Unable to render pause text texture!\n" );
			success = false;
		}
	}
	
	if(!gSpriteSheet.loadFromFile("Assets/terrain.png")){
		printf("Failed to load terrain sprite sheet!\n");
		success = false;
	}else{
		gTiles[GRASS] = new Tile({0, 0, 32, 32}, 0);
		gTiles[BRICK] = new Tile({32, 0, 32, 32}, 2);
		gTiles[WATER] = new Tile({64, 0, 32, 32}, 1);
		gTiles[EMPTY] = new Tile({96, 0, 32, 32}, 3);
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
	if(!gBulletUpgradeTexture.loadFromFile("Assets/bulletUp.png")){
		printf("Failed to load bulletUpgrade texture!\n");
		success = false;
	}

	//Load players' life available image
	if(!gLifeAvailableTexture.loadFromFile("Assets/lifeAvailable.png")){
		printf("Failed to load life available texture!\n");
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
bool checkCollision(Circle& c1, SDL_Rect r){
	//Closest point on collision box
    int cX, cY;
    if(c1.x < r.x){
    	cX = r.x;
    }
    else if(c1.x > r.x + r.w){
        cX = r.x + r.w;
    }
    else{
        cX = c1.x;
    }

    //Find closest y offset
    if(c1.y < r.y){
        cY = r.y;
    }
    else if(c1.y > r.y + r.h) {
        cY = r.y + r.h;
    }
    else{
        cY = c1.y;
    }

    //If the closest point is inside the circle
    if(sqrt(pow(cX-c1.x, 2)+pow(cY-c1.y, 2)) < c1.r){
        //This box and the circle have collided
        return true;
    }
    //If the shapes have not collided
    return false;
}
void close(){
	//Free loaded images
	gPauseTextTexture.free();
	gTimeTextTexture.free();
	
	gBombTexture.free();
	gShieldTexture.free();
	gLifeTexture.free();
	
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