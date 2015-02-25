//Using SDL, SDL_image, SDL_ttf, standard IO, vectors, and strings

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
const int SCREEN_WIDTH = 1225;
const int SCREEN_HEIGHT = 625;
const int SCOREBOARD_HEIGHT = 50;
const int PLAYFIELD_HEIGHT = SCREEN_WIDTH-SCOREBOARD_HEIGHT;

//diresctions

const int NORTH = 1;
const int SOUTH = 2;
const int RIGHT = 3;
const int LEFT = 4;


using namespace std;

//circle struct
struct Circle{
    int x, y;
    int r;
};


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
		static const int HEIGHT = 25, WIDTH = 25;
		static const int ROWS = SCREEN_HEIGHT/HEIGHT, COLS = SCREEN_WIDTH/WIDTH;

		SDL_Rect t;
		const Uint8 m, v;

		Tile(SDL_Rect tile, Uint8 mobility, Uint8 vulnerability):
			t(tile), m(mobility), v(vulnerability) {};
};

class Map{
		int tileMap[Tile::COLS][Tile::ROWS];
	public:
		Map();
		Tile* getTile(int, int);
		void render();
};
class Bullet;

//Player class
class Player{
	public:
		//The dimensions of the player
		static const int PLAYER_WIDTH = 20, PLAYER_HEIGHT = 20;

		//Maximum axis velocity of the player
		static const int PLAYER_VEL = 2;

		//Initializes the variables
		Player(int);

		//Takes key presses and adjusts the player's velocity
		void handleEvent(SDL_Event& e);

		//Moves the player
	//	void move(Map*);
		void move();

		//Shows the player on the screen
		void render();

		void activatePowerUp(int id);

		Circle& getCollider();

        void shiftColliders();

        int getNLife();

        //bullets
		std::vector<Bullet> vecBul;

    private:
		//The X and Y offsets of the player
		int mPosX, mPosY;

		//The velocity of the player
		int mVelX, mVelY;

		//Player ID
		int mPlayerID;

		//Player life
		int mLife;

		//the collider for the circle
        Circle mCollider;

};

class PowerUp{
	public:
		//The dimensions of the power ups
		static const int POWERUP_WIDTH = 20;
		static const int POWERUP_HEIGHT = 20;

		//Initializes the variables
		PowerUp(int id);

		//Shows the powerups on the screen
		void render();

		Circle& getCollider();
		int getPowerUpID();

		void clear();

	private:
		int mPosX, mPosY;
		int mPowerUpID;
		Circle mCollider;
};

//2=shoot in front 1,4=shoot left 3,6=shoot right
int shootDir = SOUTH;
int shootDir2 = SOUTH;


class Bullet
{
	 public:
		//The dimensions of the dot
		static const int BULLET_WIDTH = 20;
		static const int BULLET_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int BULLET_VEL = 10;

		Bullet(int x, int y);
		//Bullet(SDL_Point,Xvel,Yvel);
        ~Bullet(){};

		void update(int dir);

        void render();

        int bulPosX,bulPosY,bulVelX,bulVelY;
		bool createBul;


};

Bullet::Bullet(int x, int y)
{
    bulPosX = x;
    bulPosY = y;

    bulVelX = 0;
    bulVelY = 0;

    createBul = false;
}

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Checks collision between c1 and c2
bool checkCollision(Circle& c1, Circle& c2);

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

LTexture gTerrainSheet;
LTexture gBulletTexture;

///Vectors for the objects
vector<Player> gPlayers;
vector<PowerUp> gPowerUps;


//Global timer
LTimer gTimer;
LTimer gPowerUpsTimer;

//Tile array
Tile* tile[4];

//Power up variables
const int LIFEID = 0, BOMBID = 1, SHIELDID = 2, BULLETUPGRADEID = 3;
int nLife = 3, nBomb = 0, nShield = 0, nBulletUpgrade = 0;
int powerUpsDuration = 3;

int main(int argc, char *args[]){
	gTimer.start();
	int set = 1;

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
			//Create and push powerups in vector gPowerUps
			int powerUpsArr[5][4] = {{LIFEID, BOMBID, SHIELDID, BULLETUPGRADEID}, {3, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 0, 2}, {0, 0, 2, 0}};


			for(int i = 0; i < 4; i++){
				for(int j = 0; j < powerUpsArr[set][i]; j++){
					PowerUp powerup(powerUpsArr[0][i]);
					gPowerUps.push_back(powerup);
				}
			}

			//Create and push players to vector
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


			//Map map;
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
					//Setting up Viewports' dimensions
					SDL_Rect scoreboard = {0, 0, SCREEN_WIDTH, SCOREBOARD_HEIGHT};
					SDL_Rect playfield = {0, SCOREBOARD_HEIGHT, SCREEN_WIDTH, PLAYFIELD_HEIGHT};

					//Clear screen
					SDL_RenderClear(gRenderer);

					//Set scoreboard and timer viewport
					SDL_RenderSetViewport(gRenderer, &scoreboard);

					SDL_SetRenderDrawColor(gRenderer, 0x3F, 0xB4, 0xB4, 0xFF);
					SDL_RenderFillRect(gRenderer, &scoreboard);

					//Set text to be rendered
					timeText.str( "" );
					timeText << "Time: " << (gTimer.getTicks() / 1000);

					//Render text
					if(!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)){
						printf("Unable to render time texture!\n");
					}
					gTimeTextTexture.render((SCREEN_WIDTH-gTimeTextTexture.getWidth())/2, (SCOREBOARD_HEIGHT-gTimeTextTexture.getHeight())/2);

					for(int i = 0; i < gPlayers.size(); i++){
						for(int j = 0; j < gPlayers[i].getNLife(); j++){
							if(i==0)
								gLifeAvailableTexture.render(gLifeAvailableTexture.getWidth() * j,(SCOREBOARD_HEIGHT-gLifeAvailableTexture.getHeight())/2);
							else if(i==1)
								gLifeAvailableTexture.render(SCREEN_WIDTH-gLifeAvailableTexture.getWidth() * (j+1),(SCOREBOARD_HEIGHT-gLifeAvailableTexture.getHeight())/2);
						}
					}
					//Set playfield viewport
					SDL_RenderSetViewport(gRenderer, &playfield);
					SDL_SetRenderDrawColor(gRenderer, 0xB4, 0xB4, 0xB4, 0xFF);

					//map.render();


					for(int i = 0; i < gPowerUps.size(); i++){
						gPowerUps[i].render();
						if(!gPowerUpsTimer.isStarted()){
							gPowerUpsTimer.start();
						}
					}
					if(gPowerUpsTimer.getTicks()/1000 > powerUpsDuration){
						gPowerUps.clear();
						gPowerUpsTimer.stop();
						set++;
					}

                    for(auto &Bullet : gPlayers[0].vecBul){
                            Bullet.update(shootDir2);
                    }
                    for(auto &Bullet : gPlayers[1].vecBul){
                            Bullet.update(shootDir);
                        }

					for(int i = 0; i < gPlayers.size(); i++){
						/*gPlayers[i].move(&map);*/
						gPlayers[i].move();
						gPlayers[i].render();
						for(auto &Bullet : gPlayers[i].vecBul){
                            Bullet.render();
                        }
                        for(auto &Bullet : gPlayers[i].vecBul){
                            if((Bullet.bulPosY<0)|| (Bullet.bulPosY + Bullet.BULLET_HEIGHT > SCREEN_HEIGHT )||( Bullet.bulPosX < 0 ) || ( Bullet.bulPosX + Bullet.BULLET_WIDTH > SCREEN_WIDTH ))
                            {
                                //deconstruct and erase
                                Bullet.~Bullet();
                                gPlayers[i].vecBul.erase(gPlayers[i].vecBul.begin());

                            }
                        }
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
					gPauseTextTexture.render((SCREEN_WIDTH-gPauseTextTexture.getWidth())/2, (SCREEN_HEIGHT-gPauseTextTexture.getHeight())/2);
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
	return tile[tileMap[x/Tile::WIDTH][y/Tile::HEIGHT]];
}

void Map::render(){
	for(int i = 0; i < Tile::ROWS; ++i){
		for(int j = 0; j < Tile::COLS; ++j){
			gTerrainSheet.render(j*Tile::WIDTH, i*Tile::HEIGHT, &(tile[tileMap[j][i]]->t));
		}
	}
}

Player::Player(int id)
{
	//assigning player ID
	mPlayerID = id;

    //Initialize the positions
    if(mPlayerID == 0){
    	mPosX = 0;
    	mPosY = 0;
    }
    else if(mPlayerID == 1){
    	mPosX = SCREEN_WIDTH-PLAYER_WIDTH;
    	mPosY = SCREEN_HEIGHT/2;
    }

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //for the colliders
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.r = PLAYER_WIDTH/2;

    //default number of lives
    mLife = 3;
}

void Player::handleEvent(SDL_Event& e){
	//Controls: WASD and Arrow keys
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
    	if(mPlayerID == 0){
            if((e.type == SDL_KEYDOWN && e.key.repeat == 0) && e.key.keysym.sym == SDLK_q)
            {
                vecBul.emplace_back(mPosX,mPosY);
            }
	        switch( e.key.keysym.sym ){
	        	case SDLK_w:
                mVelY -= PLAYER_VEL;
                shootDir2 = NORTH;
                break;
            case SDLK_s:
                mVelY += PLAYER_VEL;
                shootDir2 = SOUTH;
                break;
            case SDLK_a:
                mVelX -= PLAYER_VEL;
                shootDir2 = LEFT;
                break;
            case SDLK_d:
                mVelX += PLAYER_VEL;
                shootDir2 = RIGHT;
                break;
	        }
    	}
    	if(mPlayerID == 1){
    	    if((e.type == SDL_KEYDOWN && e.key.repeat == 0) && e.key.keysym.sym == SDLK_RSHIFT)
            {
                vecBul.emplace_back(mPosX,mPosY);
            }
	        switch( e.key.keysym.sym ){
	            case SDLK_UP:
                mVelY -= PLAYER_VEL;
                shootDir = NORTH;
                break;
            case SDLK_DOWN:
                mVelY += PLAYER_VEL;
                shootDir = SOUTH;
                break;
            case SDLK_LEFT:
                mVelX -= PLAYER_VEL;
                shootDir = LEFT;
                break;
            case SDLK_RIGHT:
                mVelX += PLAYER_VEL;
                shootDir = RIGHT;
                break;
	        }
    	}
    }
    //If a key was released
    else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        if(mPlayerID == 0){
        	//Adjust the velocity
	        switch( e.key.keysym.sym ){
	        	case SDLK_w: mVelY += PLAYER_VEL; break;
	            case SDLK_s: mVelY -= PLAYER_VEL; break;
	            case SDLK_a: mVelX += PLAYER_VEL; break;
	            case SDLK_d: mVelX -= PLAYER_VEL; break;
	        }
    	}
    	if(mPlayerID == 1){
        	//Adjust the velocity
	        switch( e.key.keysym.sym ){
	            case SDLK_UP: mVelY += PLAYER_VEL; break;
	            case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
	            case SDLK_LEFT: mVelX += PLAYER_VEL; break;
	            case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
	        }
    	}
    }
}

void Bullet::update(int dir)
{
   if(dir == NORTH)
   {
        bulPosY = bulPosY - 10;
   }
   else if(dir == SOUTH)
   {
        bulPosY = bulPosY + 10;
   }
   else if( dir == RIGHT)
   {
        bulPosX = bulPosX + 10;
   }
   else if(dir == LEFT)
   {
        bulPosX = bulPosX - 10;
   }
}

void Bullet::render()
{
    //Show the Bullet
	gBulletTexture.render( bulPosX, bulPosY );
}
/*
void Player::move(Map* map){
    //Move the Player left or right
    mPosX += mVelX;
    shiftColliders();

    //If the Player went too far to the left or right
    if((mPosX < 0) || (mPosX + PLAYER_WIDTH > SCREEN_WIDTH) || (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider()))
		|| ((map->getTile(mPosX, mPosY))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY))->m > 0)
		|| ((map->getTile(mPosX, mPosY+PLAYER_HEIGHT))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY+PLAYER_HEIGHT))->m > 0)){
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Move the Player up or down
    mPosY += mVelY;
    shiftColliders();
    //If the Player went too far up or down

    if((mPosY < 0)||(mPosY + PLAYER_HEIGHT > SCREEN_HEIGHT-SCOREBOARD_HEIGHT)|| (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider()))
		|| ((map->getTile(mPosX, mPosY))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY))->m > 0)
		|| ((map->getTile(mPosX, mPosY+PLAYER_HEIGHT))->m > 0 || (map->getTile(mPosX+PLAYER_WIDTH, mPosY+PLAYER_HEIGHT))->m > 0)){
        //Move back
        mPosY -= mVelY;
        shiftColliders();
    }
}*/

void Player::move()
{
    //Move the Player left or right
    mPosX += mVelX;
    shiftColliders();
    //If the Player went too far to the left or right
    if((mPosX < 0) || (mPosX + PLAYER_WIDTH > SCREEN_WIDTH) || (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) ){
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Move the Player up or down
    mPosY += mVelY;
    shiftColliders();
    //If the Player went too far up or down
    if((mPosY < 0)||(mPosY + PLAYER_HEIGHT > SCREEN_HEIGHT-SCOREBOARD_HEIGHT) || (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) ){
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

int Player::getNLife(){
	return mLife;
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

void Player::activatePowerUp(int id){
	switch(id){
		case LIFEID:
			printf("life\n");
			mLife++;
			break;
		case BOMBID:
			printf("bomb\n");
			break;
		case SHIELDID:
			printf("shield\n");
			break;
		case BULLETUPGRADEID:
			printf("bullet upgraded\n");
			break;
	}
}


PowerUp::PowerUp(int id){
	//assigning powerup ID
	mPowerUpID = id;

    //Initialize the positions
    switch(mPowerUpID){
    	case 0:
    		mPosX = rand()%SCREEN_WIDTH - POWERUP_WIDTH;
    		mPosY = rand()%(SCREEN_HEIGHT) - (POWERUP_HEIGHT+SCOREBOARD_HEIGHT);
    		break;
    	case 1:
    		mPosX = rand()%SCREEN_WIDTH - POWERUP_WIDTH;
    		mPosY = rand()%(SCREEN_HEIGHT) - (POWERUP_HEIGHT+SCOREBOARD_HEIGHT);
    		break;
    	case 2:
    		mPosX = rand()%SCREEN_WIDTH - POWERUP_WIDTH;
    		mPosY = rand()%(SCREEN_HEIGHT) - (POWERUP_HEIGHT+SCOREBOARD_HEIGHT);
    		break;
    	case 3:
    		mPosX = rand()%SCREEN_WIDTH - POWERUP_WIDTH;
    		mPosY = rand()%(SCREEN_HEIGHT) - (POWERUP_HEIGHT+SCOREBOARD_HEIGHT);
    		break;
    }
    //for the colliders
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.r = POWERUP_WIDTH/2;
}

Circle& PowerUp::getCollider(){
    return mCollider;
}

int PowerUp::getPowerUpID(){
	return mPowerUpID;
}

void PowerUp::render(){
    //Show the powerups
	switch(mPowerUpID){
    	case 0:
    		gLifeTexture.render(mPosX, mPosY);
    		break;
    	case 1:
    		gBombTexture.render(mPosX, mPosY);
    		break;
    	case 2:
    		gShieldTexture.render(mPosX, mPosY);
    		break;
    	case 3:
    		gBulletUpgradeTexture.render(mPosX, mPosY);
    		break;
    }
}
void PowerUp::clear(){
	printf("CLEAR\n");
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
		gWindow = SDL_CreateWindow("Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	if(!gTerrainSheet.loadFromFile("Assets/terrain.png")){
		printf("Failed to load terrain sprite sheet!\n");
		success = false;
	}else{
		tile[GRASS] = new Tile({0, 0, 32, 32}, 0, 0);
		tile[BRICK] = new Tile({32, 0, 32, 32}, 2, 1);
		tile[WATER] = new Tile({64, 0, 32, 32}, 1, 0);
		tile[EMPTY] = new Tile({96, 0, 32, 32}, 3, 0);
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
		printf("Failed to load life texture!\n");
		success = false;
	}

	//Load players' life available image
	if(!gLifeAvailableTexture.loadFromFile("Assets/lifeAvailable.png")){
		printf("Failed to load life texture!\n");
		success = false;
	}
	if(!gBulletTexture.loadFromFile("Assets/bullet.bmp")){
		printf("Failed to load bullet texture!\n");
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
	gLifeTexture.free();
	gShieldTexture.free();
	gBulletUpgradeTexture.free();

	gLifeAvailableTexture.free();
	gPlayerOneTexture.free();
	gPlayerTwoTexture.free();

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
