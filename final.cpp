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
<<<<<<< HEAD
	GRASS, BRICK, WATER, EMPTY 
=======
	GRASS, BRICK, WATER, EMPTY
>>>>>>> master
};

enum Direction{
	SOUTH, WEST, NORTH, EAST
};

enum Controls{
<<<<<<< HEAD
	UP, LEFT, DOWN, RIGHT, SHOOT
=======
	UP, LEFT, DOWN, RIGHT, SHOOT, PLACEBOMB
>>>>>>> master
};

enum PowerUps{
	LIFE, BOMB, SHIELD, BULLETUPGRADE
};

struct Circle{
    int x, y;
    int r;
};

class LTexture{
<<<<<<< HEAD
	public:
		//Initialize variables
		LTexture();
		
		//Deallocates memory
		~LTexture();
		
		//Loads image at specified path
		bool loadFromFile(std::string);
		
=======
   		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mLength;

	public:
		//Initialize variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile(std::string);

>>>>>>> master
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText(std::string, SDL_Color);
		#endif
<<<<<<< HEAD
		
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
		
=======

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


};

class LTimer{
	    //The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;

	public:
		//Initializes variables
		LTimer();

>>>>>>> master
		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();
<<<<<<< HEAD
		
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
=======

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

};

class Tile{
 public:
>>>>>>> master
	static const int LENGTH = 30, WIDTH = 30;

	SDL_Rect t;
	const Uint8 m;
<<<<<<< HEAD
	
=======

>>>>>>> master
	Tile(SDL_Rect tile, Uint8 mobility):
		t(tile), m(mobility) {};
};

<<<<<<< HEAD
struct Map{
	static const int ROWS = PLAYFIELD_LENGTH/Tile::LENGTH, COLS = SCREEN_WIDTH/Tile::WIDTH;
	
	Map();
	
	Tile* tile(int, int);
	void hit(int, int);
	void render();
	
	Tile* map[COLS][ROWS];

	private:
		SDL_Rect t;
};

struct Player{
	static const int WIDTH = 20, LENGTH = 20;
	static const int VEL = 2;
	int life = 3;

	Player(LTexture* texture, int lifeAvailableYPos, int x, int y, SDL_Scancode up, SDL_Scancode left, SDL_Scancode down, SDL_Scancode right, SDL_Scancode shoot):
		playerRect{x, y, texture->getWidth(), texture->getLength()},
		dir(SOUTH), playerTex(texture), collider{x,y,WIDTH/2}, lifeYPos(lifeAvailableYPos), con{up, left, down, right, shoot} {};
=======
class Map{

		SDL_Rect t;
 public:
	static const int ROWS = PLAYFIELD_LENGTH/Tile::LENGTH, COLS = SCREEN_WIDTH/Tile::WIDTH;

	Map();

	Tile* tile(int, int);
	void hit(int, int);
	void render();

	Tile* map[COLS][ROWS];

};

class Bullet{
 public:
	static const int LENGTH = 5, WIDTH = 5, VEL = 5;
	double x, y, w, h;
	int dir;

	Bullet(double xStart, double yStart, int direction):
		x(xStart), y(yStart), w(WIDTH), h(LENGTH), dir(direction){};
		//bullet{(int) x, (int) y, WIDTH, LENGTH}{}

	//bool move(SDL_Rect& bul1, SDL_Rect& bul2);
	//bool move(SDL_Rect& bul1,int j);
	bool move(SDL_Rect& rect);
	void render();
	//private:
		SDL_Rect bullet;
};

class Player{

		int dir;
		int lifeYPos;
		SDL_Rect playerRect;
		LTexture* playerTex;
		LTexture* playerLifeTex;
		Circle collider;
		SDL_Scancode con[6];
        bool bombEnable;

		void shoot();
  public:

	static const int WIDTH = 20, LENGTH = 20;
	static const int VEL = 2;
	int life = 3;
    bool bulletUpEnable;

	Player(LTexture* texture, int lifeAvailableYPos, int x, int y, bool enableBombUp, bool enableBulletUp, SDL_Scancode up, SDL_Scancode left, SDL_Scancode down, SDL_Scancode right, SDL_Scancode shoot, SDL_Scancode placebomb):
		playerRect{x, y, texture->getWidth(), texture->getLength()},
		dir(SOUTH), playerTex(texture), collider{x,y,WIDTH/2},bombEnable(enableBombUp), bulletUpEnable(enableBulletUp) ,lifeYPos(lifeAvailableYPos), con{up, left, down, right, shoot, placebomb} {};
>>>>>>> master

	void act(const Uint8*);
	void act(SDL_Scancode);
	void move(int, int);
	Circle& getCollider();
	void shiftColliders();
	void render();
	void renderLifeTexture();
<<<<<<< HEAD
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
=======
	void activatePowerUp(int id, SDL_Rect& Rect);
	void placeBomb();


};

class PowerUp{
 		int id;
		Circle collider;

		LTexture* powerUpTex;
		//Rect is changed to public
 public:
    SDL_Rect powerUpRect;
>>>>>>> master
	static const int WIDTH = 20, LENGTH = 20;

	PowerUp(LTexture* texture, int pwrUp_id);

	void render();
	Circle& getCollider();
	int getPowerUpID();

<<<<<<< HEAD
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
=======
};

class Bomb{

    public:
        static const int WIDTH = 20, LENGTH = 20;
        static const int TIMER = 5;

        int bombPosX, bombPosY;
        LTimer timer;


        //Tile& tile(int x, int y);
        Bomb(int x, int y):
            bombPosX(x), bombPosY(y) {timer.start();};
        void render();
        void blowUp(int x, int y);
>>>>>>> master
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Checks collision between c1 and c2
bool checkCollision(Circle& c1, Circle& c2);
bool checkCollision(Circle& c1, SDL_Rect r);
<<<<<<< HEAD
=======
//bool checkCollision(Circle& c1, SDL_Point p);
>>>>>>> master

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

<<<<<<< HEAD
LTexture gBombTexture;
=======
LTexture gBombPowerUPTexture;
>>>>>>> master
LTexture gShieldTexture;
LTexture gLifeTexture;
LTexture gBulletUpgradeTexture;

LTexture gTimeTextTexture;
LTexture gPauseTextTexture;

LTexture gLifeAvailableTexture;

<<<<<<< HEAD
=======
LTexture gBombTexture;
>>>>>>> master
std::vector<Map> gLevels;
int gLevel = 0;

std::vector<Player> gPlayers;
std::vector<Bullet> gBullets;
std::vector<PowerUp> gPowerUps;

<<<<<<< HEAD
=======
std::vector<Bomb> gBomb;

>>>>>>> master
std::random_device type;

std::vector<int> x;
std::vector<int> y;

LTimer gTimer;
LTimer gDsplyPwrUpsTimer;

LTexture gSpriteSheet;
Tile* gTiles[4];

<<<<<<< HEAD
int main(int argc, char *args[]){	
=======
int main(int argc, char *args[]){
>>>>>>> master
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
<<<<<<< HEAD
			
			//Create players
			gPlayers.emplace_back(&gPlayerOneTexture, 15, 5, 5, SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_C);
			gPlayers.emplace_back(&gPlayerTwoTexture, 37, SCREEN_WIDTH-Player::WIDTH-5, PLAYFIELD_LENGTH-Player::LENGTH-5, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_N);
			
=======

			//Create players
			gPlayers.emplace_back(&gPlayerOneTexture, 15, 5, 5,true,false, SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_C, SDL_SCANCODE_Z);
			gPlayers.emplace_back(&gPlayerTwoTexture, 37, SCREEN_WIDTH-Player::WIDTH-5, PLAYFIELD_LENGTH-Player::LENGTH-5,false,false, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_N, SDL_SCANCODE_M);

>>>>>>> master
			//Power ups variables
			static const int NSETS = 4;
			static const int NPOWERUPS = 4;
			static const int DSPLYTIMEPWRUP = 8;
<<<<<<< HEAD

			static const int powerUpsSet[NSETS][NPOWERUPS] = {{0, 0, 3, 2}, {0, 2, 0, 0}, {3, 0, 1, 0}, {0, 0, 2, 0}};	
			LTexture powerUpsTex[NPOWERUPS] = {gLifeTexture, gBombTexture, gShieldTexture, gBulletUpgradeTexture};

			int set = 0;	
			bool nextSet = true;
			int powerUpsTime[NSETS] = {2, 20, 40, 70};
					
=======
            //LIFE, BOMB, SHIELD, BULLETUPGRADE
			static const int powerUpsSet[NSETS][NPOWERUPS] = {{0, 0, 3, 2}, {0, 2, 0, 0}, {3, 0, 1, 0}, {0, 0, 2, 0}};
			LTexture powerUpsTex[NPOWERUPS] = {gLifeTexture, gBombPowerUPTexture, gShieldTexture, gBulletUpgradeTexture};

			int set = 0;
			int bombtime = 5;
			bool nextSet = true;
			int powerUpsTime[NSETS] = {2, 20, 40, 70};

			int counter = 0;

>>>>>>> master
			//Event handler
			SDL_Event event;
			const Uint8* state = SDL_GetKeyboardState(NULL);

			//Set text color as black
			SDL_Color textColor = {255, 255, 255, 255};

			//In memory text stream
			std::stringstream timeText;
<<<<<<< HEAD
			
=======

>>>>>>> master
			const int LEVELS = 1;
			for(int i = 0; i < LEVELS; ++i){
				gLevels.emplace_back();
			}
<<<<<<< HEAD
			
			//start global game timer
			gTimer.start();
			
			//While application is running
			while(!quit){									
=======

			//start global game timer
			gTimer.start();

			//While application is running
			while(!quit){
>>>>>>> master
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
<<<<<<< HEAD
							for(int i = 0; i < gPlayers.size(); ++i){
								gPlayers[i].act(event.key.keysym.scancode);
							}
						}
					}
				}
				
=======
							//for(int i = 0; i < gPlayers.size(); ++i){
								gPlayers[0].act(event.key.keysym.scancode);
								gPlayers[1].act(event.key.keysym.scancode);
							//}
						}
					}
				}

>>>>>>> master
				for(int i = 0; i < gPlayers.size(); ++i){
					gPlayers[i].act(state);
				}

<<<<<<< HEAD
				if(nextSet && set < NSETS){
=======
				if(nextSet && set < NSETS){//what does this mean?
>>>>>>> master
					for(int i = 0; i < NPOWERUPS; i++){
						for(int j = 0; j < powerUpsSet[set][i]; j++){
							gPowerUps.emplace_back(&powerUpsTex[i], i);
						}
					}
					nextSet = false;
				}
<<<<<<< HEAD
				
=======

>>>>>>> master
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

<<<<<<< HEAD
					SDL_RenderSetViewport(gRenderer, &playfield);				
					
					gLevels[gLevel].render();
					
=======
					SDL_RenderSetViewport(gRenderer, &playfield);

					gLevels[gLevel].render();

>>>>>>> master
					if(gTimer.getTicks()/1000 > powerUpsTime[set] && set < NSETS){
						for(int i = 0; i < gPowerUps.size(); i++){
							gPowerUps[i].render();
							if(!gDsplyPwrUpsTimer.isStarted()){
								gDsplyPwrUpsTimer.start();
							}
							if(gDsplyPwrUpsTimer.getTicks()/1000 > DSPLYTIMEPWRUP){
								gPowerUps.clear();
								gDsplyPwrUpsTimer.stop();
								nextSet = true;
								set++;
								printf("success\n");
							}
						}
					}

					for(int i = 0; i < gBullets.size(); ++i){
<<<<<<< HEAD
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
						    	gPlayers[i].activatePowerUp(gPowerUps[j].getPowerUpID());
						    	gPowerUps.erase(gPowerUps.begin()+j);	
=======
                        if(gBullets[i].move(gBullets[i].bullet)){
                            gBullets[i].render();
                        }else{
                            gBullets.erase(gBullets.begin()+i);
                            printf("bullet erased");
                        }
                    }

                    for(int i = 0; i<gBomb.size(); i++){
                        if(gBomb[i].timer.getTicks()/1000>Bomb::TIMER){
                            gBomb[i].blowUp(gBomb[i].bombPosX, gBomb[i].bombPosY);
                            gBomb.erase(gBomb.begin()+i);
                        }

                        gBomb[i].render();

                    }

                    for(int i = 0; i < gPlayers.size(); i++){
						gPlayers[i].render();
						for(int j = 0; j < gPowerUps.size(); j++){
							if(checkCollision(gPlayers[i].getCollider(), gPowerUps[j].getCollider())){
                                //for(int k = 0; k < gBullets.size(); ++k){
                                    gPlayers[i].activatePowerUp(gPowerUps[j].getPowerUpID(),gPowerUps[j].powerUpRect);
                                    gPowerUps.erase(gPowerUps.begin()+j);
                                //}
>>>>>>> master
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
<<<<<<< HEAD
	
	//The final texture
	SDL_Texture* newTexture = NULL;
	
=======

	//The final texture
	SDL_Texture* newTexture = NULL;

>>>>>>> master
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == NULL){
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}else{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
<<<<<<< HEAD
		
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		
=======

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

>>>>>>> master
		if(newTexture == NULL){
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}else{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mLength = loadedSurface->h;
		}
<<<<<<< HEAD
		
		//Get rid of loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
=======

		//Get rid of loaded surface
		SDL_FreeSurface(loadedSurface);
	}

>>>>>>> master
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
	//Get rid of preexisting texture
	free();
<<<<<<< HEAD
	
=======

>>>>>>> master
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
<<<<<<< HEAD
	
=======

>>>>>>> master
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, src, dst, angle, center, flip);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mLength};
<<<<<<< HEAD
	
=======

>>>>>>> master
	//Set clip rendering dimensions
	if(clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
<<<<<<< HEAD
}		
=======
}
>>>>>>> master

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
<<<<<<< HEAD
	
=======

>>>>>>> master
	mPaused = false;
	mStarted = false;
}

void LTimer::start(){
	//Start the timer
	mStarted = true;
<<<<<<< HEAD
	
	//Unpause the timer
	mPaused = false;
	
=======

	//Unpause the timer
	mPaused = false;

>>>>>>> master
	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop(){
	//Stop the timer
	mStarted = false;
<<<<<<< HEAD
	
	//Unpause the timer
	mPaused = false;
	
=======

	//Unpause the timer
	mPaused = false;

>>>>>>> master
	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause(){
	//If the timer is running and isn't already paused
	if(mStarted && !mPaused){
		//Pause the timer
		mPaused = true;
<<<<<<< HEAD
		
=======

>>>>>>> master
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
<<<<<<< HEAD
		
		//Reset the startng ticks
		mStartTicks = SDL_GetTicks()-mPausedTicks;
		
=======

		//Reset the startng ticks
		mStartTicks = SDL_GetTicks()-mPausedTicks;

>>>>>>> master
		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks(){
	//The actual timer time
	Uint32 time = 0;
<<<<<<< HEAD
	
=======

>>>>>>> master
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
<<<<<<< HEAD
	
=======

>>>>>>> master
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
<<<<<<< HEAD
	if(key == con[SHOOT]){shoot();}
}

void Player::move(int vx, int vy){
    playerRect.x += vx;
    shiftColliders();
	
=======
	if(key == con[SHOOT]){
        shoot();
	}
	if(key == con[PLACEBOMB]&&bombEnable == true){
        placeBomb();

	}
}

void Player::move(int vx, int vy){
    playerRect.x += vx;//whatever the position of the rectangle is will also be the position where the rectangel will be rendered
    shiftColliders();

>>>>>>> master
    if((gLevels[gLevel].tile(playerRect.x, playerRect.y))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x, playerRect.y+LENGTH))->m > 0
		|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y+LENGTH))->m > 0
		|| (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) ){
        playerRect.x -= vx;
        shiftColliders();
    }

<<<<<<< HEAD
    playerRect.y += vy;
=======
    playerRect.y += vy;//whatever the position of the rectangle is will also be the position where the rectangel will be rendered
>>>>>>> master
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
<<<<<<< HEAD
	gBullets.emplace_back(playerRect.x+WIDTH/2, playerRect.y+LENGTH/2, dir);
=======
   //gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y+LENGTH/2), dir);
    if(dir == EAST){
        gBullets.emplace_back((playerRect.x+WIDTH), (playerRect.y+LENGTH/2), dir);
        printf("BULLET");
	}
	else if(dir == WEST){
        gBullets.emplace_back((playerRect.x-WIDTH), (playerRect.y+LENGTH/2), dir);
        printf("BULLET");
	}
	else if(dir == SOUTH){
        gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y+LENGTH), dir);
        printf("BULLET");
	}
	else if(dir == NORTH){
        gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y-LENGTH), dir);
        printf("BULLET");
	}
}

void Player::placeBomb(){
    gBomb.emplace_back(playerRect.x, playerRect.y);
    //start timer here
    printf("bomb placed");
>>>>>>> master
}

void Player::render(){
    playerTex->render(&playerRect, NULL, 90*dir);
}

PowerUp::PowerUp(LTexture* texture, int pwrUp_id){
	//Gets grass tiles positions for power up positioning
	for(int i = 0; i < SCREEN_WIDTH; i+=Tile::WIDTH){
		for(int j = 0; j < PLAYFIELD_LENGTH; j+=Tile::LENGTH){
			if(gLevels[gLevel].tile(i,j) == gTiles[GRASS]){
				x.push_back(i);
				y.push_back(j);
			}
		}
	}
	//gets a random position to place the powerup
	int randInd = type()%x.size();
	powerUpRect = {x[randInd]+Tile::WIDTH/5, y[randInd]+Tile::LENGTH/5, texture->getWidth(), texture->getLength()};
	powerUpTex = texture;
	id = pwrUp_id;
	collider = {x[randInd]+Tile::WIDTH/5, y[randInd]+Tile::LENGTH/5, WIDTH/2};
<<<<<<< HEAD
	

=======
>>>>>>> master
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
<<<<<<< HEAD
	for(int i = 0; i < life; i++){	
		gLifeAvailableTexture.render(6*SCREEN_WIDTH/7+WIDTH*i, lifeYPos);
	}
}
void Player::activatePowerUp(int id){
	switch(id){
		case LIFE:		
=======
	for(int i = 0; i < life; i++){
		gLifeAvailableTexture.render(6*SCREEN_WIDTH/7+WIDTH*i, lifeYPos);
	}
}

void Player::activatePowerUp(int id, SDL_Rect& Rect){
	switch(id){
		case LIFE:
>>>>>>> master
			printf("life\n");
			life++;
			break;
		case BOMB:
<<<<<<< HEAD
			printf("bomb\n");
			break;
		case SHIELD:
			printf("shield\n");
			break;
		case BULLETUPGRADE:
			printf("bullet upgraded\n");
=======
            printf("bomb\n");
            //turn the area around the bomb to grass
            bombEnable = true;
            break;
		case SHIELD:
			printf("shield\n");
			//add to life if hit
            /*if(checkCollision(gPlayers[0].getCollider())){
                life++;
            }
            if(checkCollision(gPlayers[1].getCollider())){
                life++;
            }*/
			break;
		case BULLETUPGRADE:
			printf("bullet upgraded\n");
            bulletUpEnable = true;
>>>>>>> master
			break;
	}
}

<<<<<<< HEAD
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
=======
bool Bullet::move(SDL_Rect& rect){
	x += VEL*cos(PI*(dir+1)/2);
	y += VEL*sin(PI*(dir+1)/2);

   if(gLevels[gLevel].tile(x, y) == gTiles[BRICK]){
        gLevels[gLevel].hit(x, y);
        return false;
    }
    else if(gLevels[gLevel].tile(x+WIDTH, y+LENGTH) == gTiles[BRICK]){
        gLevels[gLevel].hit(x+WIDTH, y+LENGTH);
        return false;
    }
    else if(gLevels[gLevel].tile(x, y) == gTiles[EMPTY]){
        return false;
    }

    if(checkCollision(gPlayers[0].getCollider(),rect)){
        printf("BULLET collided");
        gPlayers[0].life--;
        return false;
    }
    if(checkCollision(gPlayers[1].getCollider(),rect)){
        printf("BULLET collided");
        gPlayers[1].life--;
        return false;
    }
	//checks if the bullet of other player has collided with player
	//fix bullet on player collision
	/*if(j == 0){          //player1     //bullet of player 2
        if(checkCollision(gPlayers[1].getCollider(),bullet)){
            return false;
        }
	}
	else if(j == 1){
        if(checkCollision(gPlayers[0].getCollider(),bul1)){
            return false;
        }
	}*/
	/*else if(checkCollision(gPlayers[1].getCollider(),bul1)||checkCollision(gPlayers[0].getCollider(),bul1)){
>>>>>>> master
		return false;
	}*/
	return true;
}

void Bullet::render(){
	SDL_Rect bullet{(int) x, (int) y, WIDTH, LENGTH};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &bullet);
}

<<<<<<< HEAD
bool init(){
	//Initialization flag
	bool success = true;
	
=======
void Bomb::render(){
    gBombTexture.render(bombPosX+Player::WIDTH-(bombPosX+Player::WIDTH)%Tile::WIDTH+Tile::WIDTH/2-gBombTexture.getWidth()/2,
        bombPosY+Player::LENGTH-(bombPosY+Player::LENGTH)%Tile::LENGTH+Tile::LENGTH/2-gBombTexture.getLength()/2);
}

void Bomb::blowUp(int x, int y){
    for(int i = 1; i<=1; i++){
        if(gLevels[gLevel].tile((x+Tile::WIDTH*i), y) == gTiles[BRICK]){
            gLevels[gLevel].map[(x+Tile::WIDTH*i)/Tile::WIDTH][y/Tile::LENGTH]=gTiles[GRASS];//right
        }
        if(gLevels[gLevel].tile((x-Tile::WIDTH*i), y) == gTiles[BRICK]){
            gLevels[gLevel].map[(x-Tile::WIDTH*i)/Tile::WIDTH][y/Tile::LENGTH]=gTiles[GRASS];//left
        }
        if(gLevels[gLevel].tile((x), (y-Tile::LENGTH*i)) == gTiles[BRICK]){
            gLevels[gLevel].map[(x)/Tile::WIDTH][(y-Tile::LENGTH*i)/Tile::LENGTH]=gTiles[GRASS];//up
        }
        if(gLevels[gLevel].tile((x), (y+Tile::LENGTH*i)) == gTiles[BRICK]){
            gLevels[gLevel].map[(x)/Tile::WIDTH][(y+Tile::LENGTH*i)/Tile::LENGTH]=gTiles[GRASS];//down
        }
    }
}

bool init(){
	//Initialization flag
	bool success = true;

>>>>>>> master
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}else{
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Warning: Linear texture filtering not enabled!");
		}
<<<<<<< HEAD
		
=======

>>>>>>> master
		//Create window
		gWindow = SDL_CreateWindow("Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_LENGTH, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}else{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
<<<<<<< HEAD
			
=======

>>>>>>> master
			if(gRenderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}else{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
<<<<<<< HEAD
				
=======

>>>>>>> master
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
<<<<<<< HEAD
	
=======

>>>>>>> master
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
<<<<<<< HEAD
	
=======

>>>>>>> master
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
<<<<<<< HEAD
=======
	if(!gBombPowerUPTexture.loadFromFile("Assets/bomb.png")){
		printf("Failed to load bomb texture!\n");
		success = false;
	}
>>>>>>> master
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
<<<<<<< HEAD
=======
//for player and bullet collision
/*bool checkCollision(Circle& c1, SDL_Point p){
    if(sqrt(pow(c1.x-p.x, 2)+pow(c1.y-p.y, 2)) < c1.r){
		return true;
	}
	return false;
}*/
>>>>>>> master
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
<<<<<<< HEAD
	
	gBombTexture.free();
	gShieldTexture.free();
	gLifeTexture.free();
	
	gPlayerOneTexture.free();
	gPlayerTwoTexture.free();
	
=======

	gBombPowerUPTexture.free();
	gShieldTexture.free();
	gLifeTexture.free();

	gPlayerOneTexture.free();
	gPlayerTwoTexture.free();

>>>>>>> master
	gSpriteSheet.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
<<<<<<< HEAD
	
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
=======

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
>>>>>>> master
