
#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <string>
#include <cstring>
#include <string.h>
#include <fstream>

using namespace std;
using namespace sf;
// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameColumns = resolutionY / boxPixelsY;
const int gameRows = resolutionX / boxPixelsX; // Total columns on grid
// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};
void drawPlayer(RenderWindow &window, float player_x, float player_y, Sprite &playerSprite);
void drawPlayerRect(RenderWindow &window, float &player_x, float &player_y, Sprite &);
void moveBullet(float &bullet_y, bool &bullet_exists, Clock &bulletClock, bool);
void drawBullet(RenderWindow &window, float &bullet_x, float &bullet_y, Sprite &bulletSprite);
void drawHoneyComb(RenderWindow &window, float &honeycomb_x, float &honeycomb_y, float &, Sprite &honeyCombSprite, Sprite &);
void drawBee(RenderWindow &window, float &bee_x, float &bee_y, float &beeType, Sprite &beeSprite, Sprite &, Sprite &);
void drawObstacle(RenderWindow &window, int idx, Sprite &obstacleSprite);
void drawBird(RenderWindow &window, float bird_x, float bird_y, Sprite &birdSprite);
void drawHive(RenderWindow &window, float hive_x, float hive_y, Sprite &hiveSprite);
void drawPowerUp(RenderWindow &, float &, float &, Sprite &);
void movePowerUp(float &);
void collisionCheckPowerUp(float &, float &, float &, float &, float &);
bool moveBee(float &bee_x, float &bee_y, float &, float &, float &, float &, int, bool);
bool moveBird(float &, float &, int, int, int, int, int &, int &, bool &, float &, float &, bool);
void combCollisionForHive(float &, float &, float &, bool &, float, float);
void collisionCheckcombAndBullet(float bullet_x, float bullet_y, float &comb_x, float &comb_y, float &, bool &, bool &);
bool collisionCheck(float bullet_x, float bullet_y, float &bee_x, float &bee_y, float &bee_Dir, int beeIdx, bool &, int &beeCount, bool &, bool &, int &, int &, int &, float &, float &, bool beeAlive[]);
void collisionBeeAndComb(float &, float &, float &, float &, float, float, float &, float &);
void collisionHummingBirdAndBullet(float &, float &, float &, float &, int &, bool &, bool &);
void collisionCheckHiveAndBullet(float &, float &, float &, float &, bool &, float &);
void collisionBirdAndComb(float &, float &, float &, float &, float &, float &, int &);
void collisionCheckRedcombAndbulletForLevel4(float &, float &, float &, float &, bool &, float &, bool &);
void MoveToNearestEmpty(float &, float arr[], bool);
void generateRandRow(int &);
void eatingPowerUp(float &player_x, float &player_y, float &, float &, float &, float &, int &, Clock arr[], int &, int &);
void level2(float beeArr[][6], bool &entered, float honeyCombArr[][4], int &, float obstacleArr[], float &, float &);
void level3(float beeArr[][6], bool &entered, float honeyCombArr[][4], int &, float obstacleArr[], float &, float &);
void level4(int &beeCount, int &beeDeadCount, int &combCount, int &hiveCount, int &bulletCount, float beeArr[][6], bool beeAlive[], float honeyCombArr[][4], float hiveArr[][3], float obstacleArr[], bool entered4, float &player_x, float &player_y, bool beePlanted[], float &bird_x, float &bird_y, bool &hummingBirdColor, int &birdHitCount, int &widthDrop, int &widthDropMultiplier);
void gameover();
bool BeeCollideswithCombHorizontally(float HiveArr[][3], float honeyCombArr[][4], int combCount, int hiveCount, float beeArr[], bool direction);
bool BeeColldideswithCombVertically(float HiveArr[][3], float honeyCombArr[][4], int combCount, int hiveCount, float beeArr[]);
void spawnBee(int level, float beeArr[][6], bool beePlanted[], int &beeCount, bool beeAlive[], int &beeDeadCount, int i, float **ptr, int sz);
void drawInfantBee(RenderWindow &window, float &, float &, Sprite &);
void moveInfantBee(float &);
bool collisionCheckInfantBeeWithRight(float x, float y, float honeyCombArr[][4], int combCount, float hiveArr[][3], int hiveCount);
bool collisionCheckInfantBeeWithLeft(float x, float y, float honeyCombArr[][4], int combCount, float hiveArr[][3], int hiveCount);
bool collisionCheckAboveBee(float x, float y, float honeyCombArr[][4], int combCount, float hiveArr[][3], int hiveCount);
bool collisionCheckInfantBullet(float &, float &, float &, float &, float &);
void moveInfantLeft(float &X);
void moveInfantRight(float &X);
void startGame(sf::RenderWindow &window);
void bossLevel(sf::RenderWindow &window);
void exitGame();
bool infantBeeAlive(float **arr, int beeCount)
{
	for (int i = 0; i < beeCount; i++)
	{
		if (arr[i][2] == 1)
			return false;
	}
	return true;
}
// getting coordinates accpording to grids
float GetCoord(float coord, int div, int start)
{
	coord -= start;
	coord /= div;
	coord = int(coord) * div;
	coord += start;
	return coord;
}
// Functions for dynamivally creating infant beees

void CreateNewArray(float **&arr, int &infantBeeSize)
{
	float **temp = new float *[infantBeeSize * 2];
	for (int i = 0; i < infantBeeSize; i++)
		temp[i] = arr[i];
	delete[] arr;
	infantBeeSize *= 2;
	arr = temp;
}
void CreateInfantBee(float x, float y, float **&infantBeeArr, int &beeCount, int &beearraySize)
{
	if (beeCount == beearraySize)
		CreateNewArray(infantBeeArr, beearraySize);
	// 1st row holds x coord
	// 2d rpw holds y coord346
	// 3rd row holds whther bee exists
	// 4th row holds beeDir
	// 5th row holds nummber of collisions of the infant bee
	infantBeeArr[beeCount] = new float[5];
	infantBeeArr[beeCount][0] = GetCoord(x, 64, 16);
	infantBeeArr[beeCount][1] = GetCoord(y, 64, 16);
	infantBeeArr[beeCount][2] = 1;
	infantBeeArr[beeCount][3] = 0;
	beeCount++;
}
int sendRandomHive(int hivecount)
{
	return rand() % hivecount;
}

void ReadScore(int scoresList[], string namesList[])
{
	cout << "Here\n";
	ifstream file("HighScore.txt");
	string line;
	for (int i = 0; i < 10; i++)
		scoresList[i] = 0;
	if (file.is_open())
	{
		for (int i = 0; i < 10 && file.eof() == false; i++)
		{
			getline(file, namesList[i]);
			string sc;
			getline(file, sc);
			scoresList[i] = stoi(sc);
		}

		file.close(); // Close the file after reading
	}
}
void WriteScore(int scoresList[], string namesList[])
{
	ofstream file("HighScore.txt");

	if (file.is_open())
	{
		for (int i = 0; i < 10; i++)
		{
			if (namesList[i].length() == 0)
			{
				file << "------\n-0" << endl;
			}
			else
			{
				file << namesList[i] << "\n"
					 << scoresList[i] << endl;
			}
		}

		file.close(); // Close the file after writing
	}
	else
	{
		cout << "Unable to open the file for writing." << endl;
	}
}
void SortScore(int scoresList[], string namesList[])
{

	int n = 11;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (scoresList[j] < scoresList[j + 1])
			{
				int tempScore = scoresList[j];
				scoresList[j] = scoresList[j + 1];
				scoresList[j + 1] = tempScore;

				string tempName = namesList[j];
				namesList[j] = namesList[j + 1];
				namesList[j + 1] = tempName;
			}
		}
	}
}

int main()
{

	sf::Font font;
	if (!font.loadFromFile("scoreFont.ttf"))
	{
		std::cerr << "Error loading font!" << std::endl;
		return -1;
	}

	// Create text for menu options
	sf::Text startText("Start Game", font, 60);
	sf::Text bossLevelText("Boss Level", font, 60);
	sf::Text exitText("Exit", font, 60);

	// Positioning the text
	startText.setPosition(350, 150);
	bossLevelText.setPosition(350, 250);
	exitText.setPosition(350, 350);

	// Colors for text
	startText.setFillColor(sf::Color::Red);
	bossLevelText.setFillColor(sf::Color::Red);
	exitText.setFillColor(sf::Color::Red);

	// Add a selection indicator
	int selectedOption = 0;

	Clock infantBeeSpawnClock;
	// Create a window

	//////////////////////
	/// phase 2 sprites //
	//////////////////////
	// 1st row tells u x cooerdinates
	// 2md row tells us y coordinates
	// 3rd row tells swhther it powerup exists or not
	// 4th row holds the type of powerup
	Clock powerUpTimer[150];
	float powerUps[150][4] = {0};
	int powerUpCount = 0;
	int atePowerUp = 0;
	int stateSpeed = 0;
	int stateHeight = 0;
	Clock powerUpEffectClock[4];
	Texture heightDectext;
	Sprite heightDecSprite;
	heightDectext.loadFromFile("Sprites/Height_Dec.png");
	heightDecSprite.setTexture(heightDectext);
	heightDecSprite.setScale(2, 2);
	Texture heightInctext;
	Sprite heightIncSprite;
	heightInctext.loadFromFile("Sprites/Height_Inc.png");
	heightIncSprite.setTexture(heightInctext);
	heightIncSprite.setScale(2, 2);

	Texture speedDecText;
	Sprite speedDecSprite;
	speedDecText.loadFromFile("Sprites/Speed_Dec.png");
	speedDecSprite.setTexture(speedDecText);
	speedDecSprite.setScale(2, 2);

	Texture speedIncText;
	Sprite speedIncSprite;
	speedIncText.loadFromFile("Sprites/Speed_Inc.png");
	speedIncSprite.setTexture(speedIncText);
	speedIncSprite.setScale(2, 2);

	bool isGameOver = 0;
	int preLives = 3;
	int lives = 3;
	// For score
	Font scoreFont;
	scoreFont.loadFromFile("scoreFont.ttf");
	Text scoreScreen;
	scoreScreen.setFont(scoreFont);
	string namesList[11];
	int scoresList[11];
	int score = 0;
	scoreScreen.setString("Score: " + to_string(score));
	scoreScreen.setPosition(690, 570);
	scoreScreen.setFillColor(sf::Color::Black);

	Text SpeedTimer;
	SpeedTimer.setFont(scoreFont);
	SpeedTimer.setPosition(140, 600);
	Text heightTimer;
	heightTimer.setPosition(570, 600);
	heightTimer.setFont(scoreFont);
	SpeedTimer.setFillColor(sf::Color::Red);
	heightTimer.setFillColor(sf::Color::Red);
	scoreScreen.setScale(1.05, 1.05);
	scoreScreen.setFillColor(Color::Black);
	srand(time(0));
	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(500, 200));

	// Initializing Background Music.

	Music bgMusic;
	if (!bgMusic.openFromFile("cave.ogg"))
	{
		cout << "Error: Could not load music file!" << endl;
	}
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);
	bgMusic.play();

	// Initializing Player and Player Sprites.
	float player_x = (gameRows / 2) * boxPixelsX;
	float player_y = (gameColumns - 4) * boxPixelsY;
	Music levelCompleted;
	levelCompleted.openFromFile("Sound Effects/levelcompleted.wav");
	levelCompleted.setVolume(100);
	Texture backgroundlevel1Texture;
	Sprite backgroundLevel1Sprite;
	backgroundlevel1Texture.loadFromFile("Textures/buzzBombbackgroundimg.jpg");
	backgroundLevel1Sprite.setTexture(backgroundlevel1Texture);
	backgroundLevel1Sprite.setPosition(0, 0);
	Vector2u textureSize = backgroundlevel1Texture.getSize();
	Vector2u windowSize = window.getSize();
	backgroundLevel1Sprite.setScale(float(windowSize.x) / textureSize.x, float(windowSize.y) / textureSize.y);

	Texture backgroundlevel2Texture;
	Sprite backgroundLevel2Sprite;
	backgroundlevel2Texture.loadFromFile("Textures/buzzlvl2.jpg");
	backgroundLevel2Sprite.setTexture(backgroundlevel2Texture);
	backgroundLevel2Sprite.setPosition(0, 0);
	Vector2u textureSize2 = backgroundlevel2Texture.getSize();
	Vector2u windowSize2 = window.getSize();
	backgroundLevel2Sprite.setScale(float(windowSize2.x) / textureSize2.x, float(windowSize2.y) / textureSize2.y);

	Texture backgroundlevel3Texture;
	Sprite backgroundLevel3Sprite;
	backgroundlevel3Texture.loadFromFile("Textures/buzzbomblevel3.jpg");
	backgroundLevel3Sprite.setTexture(backgroundlevel3Texture);
	Vector2u textureSize3 = backgroundlevel3Texture.getSize();
	Vector2u windowSize3 = window.getSize();
	backgroundLevel3Sprite.setScale(float(windowSize3.x) / textureSize3.x, float(windowSize3.y) / textureSize3.y);

	Texture backgroundlevel4Texture;
	Sprite backgroundLevel4Sprite;
	backgroundlevel4Texture.loadFromFile("Textures/bosslevelbackground.jpeg");
	backgroundLevel4Sprite.setTexture(backgroundlevel4Texture);
	Vector2u textureSize4 = backgroundlevel4Texture.getSize();
	Vector2u windowSize4 = window.getSize();
	backgroundLevel4Sprite.setScale(float(windowSize4.x) / textureSize4.x, float(windowSize4.y) / textureSize4.y);

	Texture menuTexture;
	Sprite menuSprite;
	Clock menuClock;
	menuTexture.loadFromFile("Textures/buzzMenu.jpeg");
	menuSprite.setTexture(menuTexture);
	Vector2u menuTextureSize = menuTexture.getSize();
	Vector2u windowMenuSize = window.getSize();
	menuSprite.setScale(float(windowMenuSize.x) / menuTextureSize.x, float(windowMenuSize.y) / menuTextureSize.y);

	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/spray.png");
	playerSprite.setTexture(playerTexture);
	Texture playerRect;
	Sprite playerRectSprite;
	playerRect.loadFromFile("Textures/sprayRect.png");
	playerRectSprite.setTexture(playerRect);
	float scale_x = .85;
	float scale_y = .75;
	playerRectSprite.setScale(.85, .75);
	Sprite RectShortSprite;
	RectShortSprite.setTexture(playerRect);
	RectShortSprite.setScale(.85, .2);
	Sprite RectLongSprite;
	RectLongSprite.setTexture(playerRect);
	RectLongSprite.setScale(.85, 1);

	//	playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites
	// Data for bullet / Spray pellet
	// bool that tells us whther we have entered the function of level 2 or not
	bool entered2 = 0;
	bool entered3 = 0;
	bool entered4 = 0;
	float bullet_x = -1;
	float bullet_y = -1;
	bool bullet_exists = false;
	int bulletCount = 0;
	int widthDrop = 1;
	int widthDropMultiplier = 1;
	Music fireEffect;
	fireEffect.setVolume(50);
	// Checks if collision occured between bees and the bullet
	bool isCollision = 0;

	Clock bulletClock;
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setScale(2, 2);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	//////////////////////////////////
	//    Hive Realted Varaibles    //
	//////////////////////////////////
	float hive_x;
	float hive_y;
	int hiveCount = 0;
	const int maxhive = 1000;
	// 1st row stores the x coordinates of the huve
	// 2nd row stores the y coordinates of the hive
	// 3rd row stores whether a hive exists or Not
	float hiveArr[maxhive][3];
	Texture hiveTexture;
	Sprite hiveSprite;
	hiveTexture.loadFromFile("Textures/hive.png");
	hiveSprite.setTexture(hiveTexture);
	//////////////////////////////////
	// Humming bird related variables//
	//////////////////////////////////
	// 0 means its blue and 1 means its green
	bool initHummingBirdColor = 0;
	bool hummingBirdColor = 0;
	Clock hummingCanWait;
	hummingCanWait.restart();
	Texture birdTexture;
	Sprite birdSprite;
	birdTexture.loadFromFile("Textures/bird.png");
	birdSprite.setTexture(birdTexture);
	birdSprite.setScale(3, 3);
	birdSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	bool putDelayOnBird = 0;
	float bird_x;
	float bird_y;
	int randomNumForBirdInitiaposition = (rand() % 4) + 1;
	switch (randomNumForBirdInitiaposition)
	{
	case 1:
		bird_x = -300;
		bird_y = -300;
		break;
	case 2:
		bird_x = 2500;
		bird_y = 1220;
		break;
	case 3:
		bird_x = 2500;
		bird_y = -300;
		break;
	case 4:
		bird_x = -300;
		bird_y = 1220;
	}
	// Storing the initial coordinates of the bird to send it back to agian after it gets sick
	float currRand_x;
	float currRand_y;
	float offScreenBird_x = bird_x;
	float offScreenBird_y = bird_y;
	float birdRow;
	float randomBird_x = 16 + (rand() % (800 - 16 + 1));
	float randomBird_y = 32 + (rand() % (480 - 32 + 1));
	bool birdReached = 0;
	int stepBirdCount_x = 0;
	int stepBirdCount_y = 0;
	int totBirdStep_x = 0;
	int totBirdStep_y = 0;
	float initialBird_x = bird_x;
	float initialBird_y = bird_y;
	int birdHitcount = 0;
	birdSprite.setColor(Color::Blue);
	Clock clockBird;
	if (bird_x > randomBird_x)
	{
		totBirdStep_x = (int)(bird_x / 32) - (int)(randomBird_x / 32);
	}
	else if (bird_x < randomBird_x)
	{
		totBirdStep_x = (int)(randomBird_x / 32) - (int)(bird_x / 32);
	}
	if (bird_y < randomBird_y)
	{
		totBirdStep_y = (int)(randomBird_y / 32) - (int)(bird_y / 32);
	}
	else if (bird_y > randomBird_y)
	{
		totBirdStep_y = (int)(bird_y / 32) - (int)(randomBird_y / 32);
	}
	////////////////////////////////
	// Obstacles reated variables //
	////////////////////////////////
	Texture obstacleTexture;
	Sprite obstacleSprite;
	obstacleTexture.loadFromFile("Textures/obstacles.png");
	obstacleSprite.setTexture(obstacleTexture);
	obstacleSprite.setScale(2, 2);

	bool beePlanted[6] = {0};
	float obstacleArr[20] = {0};
	int obsCount = 0;
	// obstacleSprite.setScale()

	/////////////////////////////////
	// Honey comb related variables//
	/////////////////////////////////
	//{
	Texture honeyCombTexture;
	Sprite honeyCombSprite;
	const int size = 100;
	int combCount = 3;
	// 1st row stores the x coordinates of the comb
	// 2nd row holds the y coordinate of the comb
	// 3rd row holds whehther comb exists still or not
	// 4th Row stores the type 0f the bee
	float honeyCombArr[size][4];
	honeyCombTexture.loadFromFile("Textures/honeycomb.png");
	honeyCombSprite.setTexture(honeyCombTexture);
	honeyCombSprite.setScale(1, 1);

	Texture redHoneyCombTexture;
	Sprite redHoneyCombSprite;
	redHoneyCombTexture.loadFromFile("Textures/honeycomb_red.png");
	redHoneyCombSprite.setTexture(redHoneyCombTexture);

	for (int i = 0; i < 3; i++)
	{
		honeyCombArr[i][0] = 64 + (rand() % (700 - 64 + 1));
		// honeyCombArr[i][0] = GetCoord(honeyCombArr[i][0] , 64 , 16);
		honeyCombArr[i][1] = 64 + (rand() % (420 - 64 + 1));
		// honeyCombArr[i][1] = GetCoord(honeyCombArr[i][1] , 64 , 16);
		honeyCombArr[i][2] = 1;
		honeyCombArr[i][3] = 0;
	}

	// combCount--;
	// honeyCombArr[0][0] = 32;
	// honeyCombArr[0][1] = 224;

	// honeyCombArr[1][0] = 64;
	// honeyCombArr[1][1] = 160;

	/////////////////////////
	// Level Related variables
	//////////////////////////
	///////////////////////
	int level = 1;
	Font levelFont;
	levelFont.loadFromFile("scoreFont.ttf");
	Text levelScreen;
	levelScreen.setFont(levelFont);

	levelScreen.setString("level : " + to_string(level));
	levelScreen.setScale(2, 2);
	levelScreen.setPosition(480, 560);
	levelScreen.setFillColor(sf::Color::Black);
	Text gameover;
	gameover.setFont(levelFont);
	gameover.setString("GAME OVER");
	gameover.setScale(3, 3);
	gameover.setPosition(270, 260);
	gameover.setFillColor(Color::Red);
	Music gameoverMusic;
	gameoverMusic.openFromFile("Sound Effects/gameOver.wav");
	gameoverMusic.setVolume(100);
	/////////////////////////
	// bee realted variables//
	/////////////////////////

	// float bee_x;
	// float bee_y;
	// bool beeDir=0;
	Clock level4BeeClock;
	int beeCount = 6;
	Clock beeClock;
	int x = 898;
	int x1 = 16;
	float randomBees_x[6] = {-1000};
	// 1st row stores the x coordinates of all the bees
	// 2nd row stores alll the y coordinates of the bees
	// 3rd row stores all the directions of the bees
	// 4th row stores whether bee is to move vertically or not
	// 5th row stores beetype
	// 6th row stores whther beetype has changed or not
	float beeArr[6][6] = {};
	Clock BeeCanWait[6];
	bool beeAlive[6] = {1, 1, 1, 1, 1, 1};

	int infantBeeSize = 2;
	int infantBeeCount = 0;
	float **infantBeeArr = new float *[infantBeeSize];
	Music hitEffect;
	int beeDeadCount = 0;
	int redbeeCount = 0;
	hitEffect.setVolume(50);
	for (int i = 0; i < 6; i++)
	{
		// if(i==3)
		// {
		// 	beeArr[i][0]=600;
		// 	beeArr[i][1]= 32;
		// 	continue;
		// }
		beeArr[i][4] = 0;
		beeArr[i][0] = -1;
		if (i % 2)
		{
			beeArr[i][2] = 1;
		}
		else
		{
			beeArr[i][2] = 0;
		}
		beeArr[i][1] = 32;
	}
	beeArr[0][0] = x;
	Texture beeTexture;
	Sprite beeSprite;
	beeTexture.loadFromFile("Textures/Regular_bee.png");
	beeSprite.setTexture(beeTexture);
	Sprite ReverseBee;
	ReverseBee.setTexture(beeTexture);
	ReverseBee.setScale(-1, 1);
	ReverseBee.setOrigin(ReverseBee.getLocalBounds().width, 0);

	Texture fastBeeTexture;
	Sprite fastBeeSprite;
	fastBeeTexture.loadFromFile("Textures/Fast_bee.png");
	fastBeeSprite.setTexture(fastBeeTexture);
	Sprite ReverseFastBeeSprite;
	ReverseFastBeeSprite.setTexture(fastBeeTexture);
	ReverseFastBeeSprite.setScale(-1, 1);
	Clock infantBeeClock;
	infantBeeClock.restart();
	Texture infantBeeTexture;
	Sprite infantBeeSprite;
	infantBeeTexture.loadFromFile("Sprites/infantBee.png");
	infantBeeSprite.setTexture(infantBeeTexture);
	Sprite reverseInfantBeeSprite;
	reverseInfantBeeSprite.setTexture(infantBeeTexture);
	infantBeeSprite.setScale(1.75, 1.75);
	reverseInfantBeeSprite.setScale(-1.5, 1.5);
	reverseInfantBeeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	reverseInfantBeeSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	Sprite playerShortSprite;
	playerShortSprite.setTexture(playerTexture);
	playerShortSprite.setScale(1.25, .625);

	Sprite playerLargeSprite;
	playerLargeSprite.setTexture(playerTexture);
	playerLargeSprite.setScale(1.25, 2);
	// The ground on which player moves
	RectangleShape groundRectangle(Vector2f(960, 64));
	groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
	groundRectangle.setFillColor(Color::Green);
	// bee_x = 900;
	// bee_y = 32;
	beeClock.restart();

	Clock FPSCLOCK;
	FPSCLOCK.restart();
	int FPS = 40;

	Texture mainMenuTexture;
	Sprite mainMenuSprite;
	mainMenuTexture.loadFromFile("Textures/buzzBombersMainMenu.jpg");
	mainMenuSprite.setTexture(mainMenuTexture);
	Vector2u mainMenuTextureSize = mainMenuTexture.getSize();
	Vector2u mainMenuWindowSize = window.getSize();
	mainMenuSprite.setScale(float(mainMenuWindowSize.x) / mainMenuTextureSize.x, float(mainMenuWindowSize.y) / mainMenuTextureSize.y);
	Text eventText;
	eventText.setFont(scoreFont);
	string state = "name";

	string name = "";

	Clock GameOVer;
	bool GameOVerRestared = false;

	Clock LevelOver;
	bool LevelOverFlag = false;
	while (window.isOpen())
	{

		if (LevelOverFlag == false && level == 4 && beeDeadCount >= 35)
		{
			bool breakFlag = 0;
			for (int i = 0; i < 6; i++)
			{
				if (beeAlive[i] == true)
				{
					breakFlag = 1;
					break;
				}
			}
			if (!breakFlag)
			{
				LevelOverFlag = true;
				LevelOver.restart();
			}
		}

		if (isGameOver == true)
		{

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			if (GameOVerRestared == false)
			{
				ReadScore(scoresList, namesList);

				GameOVer.restart();
				scoresList[10] = score;

				if (name.length() > 15)
					name = name.substr(0, 14);
				namesList[10] = name;
				GameOVerRestared = true;

				SortScore(scoresList, namesList);
				WriteScore(scoresList, namesList);
			}

			window.clear();

			// Display "High Scores" title
			Text title;
			title.setFont(levelFont);
			title.setScale(3, 3); // Bigger scale for the title
			title.setFillColor(Color::Yellow);
			title.setString("High Scores");
			title.setPosition(250, 10); // Position title at the top-center
			window.draw(title);

			for (int i = 0; i < 10; i++)
			{
				// Display name
				Text name;
				name.setFont(levelFont);
				name.setFillColor(Color::White);

				name.setString(namesList[i].size() ? namesList[i] : "------");
				name.setFillColor(Color::Red);
				name.setPosition(200, 150 + i * 40); // Adjust x and y positions based on index
				window.draw(name);
				Text score;
				score.setFont(levelFont);
				score.setFillColor(Color::White);
				score.setString(to_string(scoresList[i]));
				score.setPosition(600, 150 + i * 40); // Place score to the right of the name
				window.draw(score);
			}

			window.display();

			if (GameOVer.getElapsedTime().asMilliseconds() > 10000)
			{
				window.close();
			}
			continue;
		}

		if (state == "name")
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}

				if (state == "name")
				{
					if (event.type == sf::Event::TextEntered)
					{
						if (event.text.unicode < 128)
						{
							char enteredChar = static_cast<char>(event.text.unicode);

							if (enteredChar == '\b' && !name.empty())
							{
								name.pop_back();
							}
							else if (enteredChar == '\r' || enteredChar == '\n')
							{
								state = "menu";
							}
							else if (enteredChar >= ' ' && enteredChar <= '~')
							{
								name += enteredChar;
							}
						}
					}
				}
			}
			window.clear();
			window.draw(mainMenuSprite);

			sf::Text temptext("Enter name:", font, 60); // Text to display the entered name
			temptext.setPosition(350, 150);
			temptext.setFillColor(sf::Color::Yellow);
			window.draw(temptext);

			sf::Text nameText(name, font, 40); // Text to display the entered name
			nameText.setPosition(350, 250);
			nameText.setFillColor(sf::Color::White);
			window.draw(nameText);
			window.display();

			continue;
		}
		if (state == "menu")
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}

				if (event.type == sf::Event::MouseButtonPressed)
				{
					// Check if the mouse was clicked
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						// Get mouse position
						sf::Vector2i mousePos = sf::Mouse::getPosition(window);

						// Check if the mouse is over a menu option
						if (startText.getGlobalBounds().contains(mousePos.x, mousePos.y))
						{
							state = "play"; // Start the game
						}
						else if (bossLevelText.getGlobalBounds().contains(mousePos.x, mousePos.y))
						{
							state = "play"; // Start the game and set level to 4
							level = 4;
							level4(beeCount, beeDeadCount, combCount, hiveCount, bulletCount, beeArr, beeAlive, honeyCombArr, hiveArr, obstacleArr, entered4, player_x, player_y, beePlanted, bird_x, bird_y, hummingBirdColor, birdHitcount, widthDrop, widthDropMultiplier);
							entered4 = 1;
						}
						else if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y))
						{
							window.close(); // Exit the game
						}
					}
				}
			}

			window.clear();
			window.draw(mainMenuSprite);
			window.draw(startText);
			window.draw(bossLevelText);
			window.draw(exitText);
			window.display();

			continue;
		}

		if (stateHeight == 0)
			playerSprite.setScale(1.25, 1.25);
		if (FPSCLOCK.getElapsedTime().asMilliseconds() < 1000 / FPS)
		{
			continue;
		}
		FPSCLOCK.restart();
		// cout << "BeeDEadCount " << beeDeadCount << "---- beecount = " << beeCount << endl;

		if (level == 2 && !entered2)
		{
			for (int i = 0; i < hiveCount; i++)
			{
				if (hiveArr[i][2] == 1)
				{
					if (hiveArr[i][1] <= 96)
					{
						score += 2000;
					}
					else if (hiveArr[i][1] <= 288)
					{
						score += 1600;
					}
					else
						score += 1000;
				}
			}
			widthDrop = 1;
			widthDropMultiplier = 1;
			bgMusic.setVolume(0);
			levelCompleted.play();
			// Assuming these variables are defined somewhere in your code
			menuClock.restart();
			window.clear();

			// Loop until 5 seconds have passed
			while (menuClock.getElapsedTime().asMilliseconds() < 4000)
			{
				// Handle events (e.g., window close event)
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
				}

				// Clear the window before drawing
				window.clear();

				// Draw the menu sprite (transition image)
				window.draw(menuSprite);

				// Display the window content
				window.display();
			}
			hummingBirdColor = 0;
			birdHitcount = 0;
			switch (randomNumForBirdInitiaposition)
			{
			case 1:
				bird_x = -100;
				bird_y = -100;
				break;
			case 2:
				bird_x = 1000;
				bird_y = 820;
				break;
			case 3:
				bird_x = 1000;
				bird_y = -100;
				break;
			case 4:
				bird_x = -100;
				bird_y = 820;
			}
			combCount = 0;
			beeCount = 6;
			beeDeadCount = 0;

			level2(beeArr, entered2, honeyCombArr, combCount, obstacleArr, player_x, player_y);
			for (int i = 0; i < 6; i++)
			{
				beeAlive[i] = true;
				beePlanted[i] = false;
			}
			bulletCount = 0;
			combCount = 9;
			for (int i = 0; i < 9; i++)
			{
				honeyCombArr[i][0] = 64 + (rand() % (700 - 64 + 1));
				honeyCombArr[i][1] = 32 + (rand() % (300 - 32 + 1));
				honeyCombArr[i][2] = 1;
				if (i % 4 == 0)
					honeyCombArr[i][3] = 1;
				else
					honeyCombArr[i][3] = 0;
			}
			for (int i = 0; i < 6; i++)
			{
				beeAlive[i] = 1;
			}
			// if (LevelOverFlag == false && level == 4 && beeCount >= 35)
			// {
			// 	LevelOverFlag = true;
			// 	LevelOver.restart();
			// }
			beeClock.restart();
		}
		if (level == 3 && !entered3)
		{
			for (int i = 0; i < hiveCount; i++)
			{
				if (hiveArr[i][2] == 1)
				{
					if (hiveArr[i][1] <= 96)
					{
						score += 2000;
					}
					else if (hiveArr[i][1] <= 288)
					{
						score += 1600;
					}
					else
						score += 1000;
				}
			}
			bgMusic.setVolume(0);
			levelCompleted.play();
			menuClock.restart();
			window.clear();
			widthDrop = 1;
			widthDropMultiplier = 1;
			// Loop until 4 seconds have passed
			while (menuClock.getElapsedTime().asMilliseconds() < 4000)
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
				}
				// Clear the window before drawing
				window.clear();

				// Draw the menu sprite (transition image)
				window.draw(menuSprite);

				// Display the window content
				window.display();
			}

			hummingBirdColor = 0;
			birdHitcount = 0;
			switch (randomNumForBirdInitiaposition)
			{
			case 1:
				bird_x = -100;
				bird_y = -100;
				break;
			case 2:
				bird_x = 1000;
				bird_y = 820;
				break;
			case 3:
				bird_x = 1000;
				bird_y = -100;
				break;
			case 4:
				bird_x = -100;
				bird_y = 820;
			}

			level3(beeArr, entered3, honeyCombArr, combCount, obstacleArr, player_x, player_y);
			for (int i = 0; i < 6; i++)
			{
				beeAlive[i] = true;
				beePlanted[i] = false;
			}
			beeCount = 6;
			beeDeadCount = 0;
			bulletCount = 0;
			combCount = 7;
			for (int i = 0; i < 15; i++)
			{
				honeyCombArr[i][0] = 64 + (rand() % (700 - 64 + 1));
				honeyCombArr[i][1] = 32 + (rand() % (300 - 32 + 1));
				honeyCombArr[i][2] = 1;
				if (i % 3 == 0)
					honeyCombArr[i][3] = 1;
				else
					honeyCombArr[i][3] = 0;
			}
			beeClock.restart();
			bulletCount = 0;
		}
		if (level == 4 && entered4 == 0)
		{
			bgMusic.setVolume(0);
			levelCompleted.play();
			menuClock.restart();
			while (menuClock.getElapsedTime().asMilliseconds() < 4000)
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
				}
				// Clear the window before drawing
				window.clear();

				// Draw the menu sprite (transition image)
				window.draw(menuSprite);

				// Display the window content
				window.display();
			}
			level4(beeCount, beeDeadCount, combCount, hiveCount, bulletCount, beeArr, beeAlive, honeyCombArr, hiveArr, obstacleArr, entered4, player_x, player_y, beePlanted, bird_x, bird_y, hummingBirdColor, birdHitcount, widthDrop, widthDropMultiplier);
			entered4 = 1;
			beeDeadCount = 0;
			beeCount = 6;
			bulletCount = 0;
		}
		for (int i = 0; i < 6; i++)
		{
			if (beeAlive[i] == false && level == 4 && beeDeadCount <= 29)
			{
				spawnBee(level, beeArr, beePlanted, beeCount, beeAlive, beeDeadCount, i, infantBeeArr, infantBeeCount);
			}
		}
		for (int i = 0; i < combCount; i++)
		{
			honeyCombArr[i][0] = GetCoord(honeyCombArr[i][0], 64, 16);
			honeyCombArr[i][1] = GetCoord(honeyCombArr[i][1], 64, 32);
		}
		for (int i = 0; i < hiveCount; i++)
		{
			if (hiveArr[i][1] == 32)
				hiveArr[i][2] = false;
			hiveArr[i][0] = GetCoord(hiveArr[i][0], 64, 16);
			hiveArr[i][1] = GetCoord(hiveArr[i][1], 64, 32);
		}
		// if (level == 4)
		// {
		// 	if (level4BeeClock.getElapsedTime().asMilliseconds() > 10000)
		// 	{
		// 		beeCount++;
		// 		// beeArr[]
		// 	}
		// }
		switch (level)
		{
		case 1:
			window.draw(backgroundLevel1Sprite);
			break;
		case 2:
			window.draw(backgroundLevel2Sprite);
			break;
		case 3:
			window.draw(backgroundLevel3Sprite);
			break;
		case 4:
			window.draw(backgroundLevel4Sprite);
		}
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				return 0;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (player_x - 10 >= 0)
			{
				int tempidx = (player_x - 10) / 64;
				if (obstacleArr[tempidx] == false)
				{
					if (level == 4)
					{
						if (powerUpEffectClock[1].getElapsedTime().asMilliseconds() < 10000 && stateSpeed == -1)
							player_x -= 4; // Move left (decrease x-coordinate)
						else if (powerUpEffectClock[0].getElapsedTime().asMilliseconds() < 10000 && stateSpeed == 1)
							player_x -= 32;
						else
							player_x -= 16;
					}
					else
						player_x -= 16;
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (player_x + boxPixelsX + 10 < resolutionX)
			{
				int tempidx = (player_x + boxPixelsX) / 64;
				if (obstacleArr[tempidx] == false)
				{
					if (level == 4)
					{
						if (powerUpEffectClock[0].getElapsedTime().asMilliseconds() < 10000 && stateSpeed == 1)
							player_x += 32; // Move right (increase x-coordinate)
						else if (powerUpEffectClock[1].getElapsedTime().asMilliseconds() < 10000 && stateSpeed == -1)
							player_x += 4;
						else
							player_x += 16;
					}
					else
						player_x += 16;
				}
			}
		}
		// Changing states after the time is up
		if (stateSpeed == 1 && powerUpEffectClock[0].getElapsedTime().asMilliseconds() >= 10000)
		{
			stateSpeed = 0;
		}
		if (stateSpeed == -1 && powerUpEffectClock[1].getElapsedTime().asMilliseconds() >= 10000)
		{
			stateSpeed = 0;
		}
		if (stateHeight == 1 && powerUpEffectClock[2].getElapsedTime().asMilliseconds() >= 10000)
		{
			stateHeight = 0;
		}
		if (stateHeight == -1 && powerUpEffectClock[3].getElapsedTime().asMilliseconds() >= 10000)
		{
			stateHeight = 0;
		}
		if (!isGameOver)
			bgMusic.setVolume(70);
		// puttig clock on bees inititally to put delayy in thier spawn
		int Dura = 3000;
		if (beeClock.getElapsedTime().asMilliseconds() > Dura && beeArr[1][0] == -1)
		{
			beeArr[1][0] = x1;
		}
		else if (beeClock.getElapsedTime().asMilliseconds() > Dura * 2 && beeArr[2][0] == -1)
		{
			beeArr[2][0] = x;
		}
		else if (beeClock.getElapsedTime().asMilliseconds() > Dura * 3 && beeArr[3][0] == -1)
		{
			beeArr[3][0] = x1;
		}
		else if (beeClock.getElapsedTime().asMilliseconds() > Dura * 4 && beeArr[4][0] == -1)
		{
			beeArr[4][0] = x;
		}
		else if (beeClock.getElapsedTime().asMilliseconds() > Dura * 5 && beeArr[5][0] == -1)
		{
			beeArr[5][0] = x1;
		}
		for (int i = 0; i < 6; i++)
		{
			if (beeAlive[i] && beeArr[i][0] != -1)
				drawBee(window, beeArr[i][0], beeArr[i][1], beeArr[i][4], !beeArr[i][2] ? beeSprite : ReverseBee, !beeArr[i][2] ? fastBeeSprite : ReverseFastBeeSprite, !beeArr[i][2] ? infantBeeSprite : reverseInfantBeeSprite);
		}
		for (int i = 0; i < 6; i++)
		{
			if (beeAlive[i] && beeArr[i][0] != -1)
			{ // bee_x = 900;
				// bee_y = 32;
				if (beeArr[i][4] == 0 && beeArr[i][0] == randomBees_x[i] && BeeCanWait[i].getElapsedTime().asMilliseconds() > 3000)
				{
					BeeCanWait[i].restart();
				}
				if (BeeCanWait[i].getElapsedTime().asMilliseconds() < 1500)
				{
					continue;
				}
				if (moveBee(beeArr[i][0], beeArr[i][1], beeArr[i][2], beeArr[i][3], randomBees_x[i], beeArr[i][4], i, isGameOver) == false)
				{
					spawnBee(level, beeArr, beePlanted, beeCount, beeAlive, beeDeadCount, i, infantBeeArr, infantBeeCount);
				}
			}
		}

		for (int i = 0; i < 6; i++)
		{
			if (beeArr[i][1] > 480)
			{
				if (i % 2)
				{
					beeArr[i][0] = 898;
					beeArr[i][2] = 0;
				}
				else
				{
					beeArr[i][0] = 16;
					beeArr[i][2] = 1;
				}
				beeArr[i][1] = 32;
				spawnBee(level, beeArr, beePlanted, beeCount, beeAlive, beeDeadCount, i, infantBeeArr, infantBeeCount);
			}
		}
		// collisioncheck for hives
		// for (int i = 0; i < 6; i++)
		// {
		// 	if (beeArr[i][4] == 1)
		// 		continue;
		// 	if ((beeArr[i][0] <= 100 && beeArr[i][2] == 1) || (beeArr[i][0] > 800 && beeArr[i][2] == 0))
		// 	{
		// 		if (beeArr[i][2] == 0)
		// 		{
		// 			if (BeeCollideswithCombHorizontally(hiveArr, honeyCombArr, combCount, hiveCount, beeArr[i], true))
		// 			{
		// 				if (BeeColldideswithCombVertically(hiveArr, honeyCombArr, combCount, hiveCount, beeArr[i]))
		// 				{
		// 					hiveArr[hiveCount][0] = 898;
		// 					hiveArr[hiveCount][1] = GetCoord(beeArr[i][1], 64, 32);
		// 					hiveArr[hiveCount][2] = true;
		// 					hiveCount++;
		// 					spawnBee(level, beeArr, beePlanted, beeCount, beeAlive, beeDeadCount, i, infantBeeArr, infantBeeCount);
		// 				}
		// 			}
		// 		}
		// 		else if (beeArr[i][2] == 1)
		// 		{
		// 			if (BeeCollideswithCombHorizontally(hiveArr, honeyCombArr, combCount, hiveCount, beeArr[i], false))

		// 			{
		// 				if (BeeColldideswithCombVertically(hiveArr, honeyCombArr, combCount, hiveCount, beeArr[i]))
		// 				{
		// 					hiveArr[hiveCount][0] = 16;
		// 					hiveArr[hiveCount][1] = GetCoord(beeArr[i][1], 64, 32);
		// 					hiveArr[hiveCount][2] = true;
		// 					hiveCount++;
		// 					if (i % 2)
		// 					{
		// 						beeArr[i][0] = 898;
		// 						beeArr[i][2] = 0;
		// 					}
		// 					else
		// 					{
		// 						beeArr[i][0] = 16;
		// 						beeArr[i][2] = 1;
		// 					}
		// 					beeArr[i][1] = 32;
		// 					spawnBee(level, beeArr, beePlanted, beeCount, beeAlive, beeDeadCount, i, infantBeeArr, infantBeeCount);
		// 				}
		// 			}
		// 		}
		// 	}
		// }
		// checking collision of bee and bullet
		for (int i = 0; i < 6; i++)
		{
			isCollision = 0;
			float currBee_x = beeArr[i][0];
			float currBee_y = beeArr[i][1];
			float currBeeDir = beeArr[i][4];
			if (bullet_exists && collisionCheck(bullet_x, bullet_y, beeArr[i][0], beeArr[i][1], beeArr[i][2], i, bullet_exists, beeCount, beeAlive[i], isCollision, level, beeDeadCount, redbeeCount, beeArr[i][4], honeyCombArr[combCount][3], beeAlive))
			{
				if (currBeeDir)
					score += 1000;
				else
					score += 100;
				spawnBee(level, beeArr, beePlanted, beeCount, beeAlive, beeDeadCount, i, infantBeeArr, infantBeeCount);
			}
			if (isCollision)
			{
				beeArr[i][3] = 0;
				hitEffect.openFromFile("Sound Effects/hit.wav");
				hitEffect.play();
				combCount++;
				honeyCombArr[combCount - 1][0] = GetCoord(currBee_x, 32, 16);
				honeyCombArr[combCount - 1][1] = GetCoord(currBee_y, 64, 32);
				honeyCombArr[combCount - 1][2] = 1;
			}
			if (bullet_exists == false)
				break;
		}
		if (beeDeadCount >= 20 && level == 1)
		{
			level++;
		}
		else if (beeDeadCount >= 20 && level == 2)
		{
			level++;
		}
		else if (beeDeadCount >= 30 && level == 3)
		{
			level++;
		}
		else if ((beeDeadCount >= 35 && (LevelOver.getElapsedTime().asMilliseconds() > 3000 && LevelOverFlag == true)) && level == 4)
		{
			for (int i = 0; i < hiveCount; i++)
			{
				if (hiveArr[i][2] == 1)
				{
					if (hiveArr[i][1] <= 96)
					{
						score += 2000;
					}
					else if (hiveArr[i][1] <= 288)
					{
						score += 1600;
					}
					else
						score += 1000;
				}
			}
			isGameOver = true;
		}
		if (level == 4)
			cout << "bee dead Count -----" << beeDeadCount << endl;
		if (!isCollision)
		{
			for (int i = 0; i < combCount; i++)
			{
				if (bullet_exists)
				{
					collisionCheckcombAndBullet(bullet_x, bullet_y, honeyCombArr[i][0], honeyCombArr[i][1], honeyCombArr[i][2], bullet_exists, isCollision);
				}
				if (isCollision)
				{
					// making poweryps for red honey combs in level 4
					if (level == 4 && honeyCombArr[i][3] == 1)
					{
						powerUpCount++;
						powerUps[powerUpCount - 1][2] = 1;
						powerUps[powerUpCount - 1][0] = honeyCombArr[i][0];
						powerUps[powerUpCount - 1][1] = honeyCombArr[i][1];
						powerUps[powerUpCount - 1][3] = (rand() % 4) + 1;
						powerUpTimer[powerUpCount - 1].restart();
					}
					hitEffect.openFromFile("Sound Effects/hit.wav");
					hitEffect.play();
					break;
				}
			}
		}
		for (int i = 0; i < powerUpCount; i++)
		{
			if (powerUps[i][2] == 1 && powerUpTimer[i].getElapsedTime().asMilliseconds() > 5000)
			{
				powerUps[i][2] = 0;
			}
		}
		if (powerUps)
			// logic for eating up the powerup sprite only for level 4
			if (level == 4)
			{
				for (int i = 0; i < powerUpCount; i++)
					eatingPowerUp(player_x, player_y, powerUps[i][0], powerUps[i][1], powerUps[i][2], powerUps[i][3], atePowerUp, powerUpEffectClock, stateSpeed, stateHeight);
			}
		if (!isCollision)
		{
			for (int i = 0; i < hiveCount; i++)
			{
				if (bullet_exists)
					collisionCheckHiveAndBullet(bullet_x, bullet_y, hiveArr[i][0], hiveArr[i][1], isCollision, hiveArr[i][2]);
				if (isCollision)
				{
					if (hive_y < 400)
						bullet_exists = 0;
					hiveArr[i][2] = 0;
					hitEffect.openFromFile("Sound Effects/hit.wav");
					hitEffect.play();
					break;
				}
			}
		}
		if (!isCollision)
		{
			if (bullet_exists)
				collisionHummingBirdAndBullet(bullet_x, bullet_y, bird_x, bird_y, birdHitcount, bullet_exists, isCollision);
			if (isCollision)
			{
				if (birdHitcount % 3 == 0)
				{
					switch (randomNumForBirdInitiaposition)
					{
					case 1:
						randomBird_x = -400;
						randomBird_y = -400;
						break;
					case 2:
						randomBird_x = 2200;
						randomBird_y = 1220;
						break;
					case 3:
						randomBird_x = 2200;
						randomBird_y = -400;
						break;
					case 4:
						randomBird_x = -400;
						randomBird_y = 1220;
					}
					currRand_x = randomBird_x;
					currRand_y = randomBird_y;
					hummingBirdColor = 1;
					if (bird_x > randomBird_x)
					{
						totBirdStep_x = (int)(bird_x / 32) - (int)(randomBird_x / 32);
					}
					else if (bird_x < randomBird_x)
					{
						totBirdStep_x = (int)(randomBird_x / 32) - (int)(bird_x / 32);
					}
					if (bird_y < randomBird_y)
					{
						totBirdStep_y = (int)(randomBird_y / 32) - (int)(bird_y / 32);
					}
					else if (bird_y > randomBird_y)
					{
						totBirdStep_y = (int)(bird_y / 32) - (int)(randomBird_y / 32);
					}
				}
				hitEffect.openFromFile("Sound Effects/hit.wav");
				hitEffect.play();
			}
			// cout<<"count is : "<<birdHitcount<<endl;
		}
		if (!isCollision && level == 4)
		{
			for (int i = 0; i < infantBeeCount; i++)
				if (collisionCheckInfantBullet(bullet_x, bullet_y, infantBeeArr[i][0], infantBeeArr[i][1], infantBeeArr[i][2]) == true)
				{
					infantBeeArr[i][2] = 0;
					isCollision = 1;
					if (infantBeeArr[i][1] < 400)
					{
						bullet_exists = 1;
					}
					score -= 500;
				}
		}
		if (hummingBirdColor == 1)
		{
			birdSprite.setColor(Color::Green);
		}
		else
			birdSprite.setColor(Color::Blue);

		int breakFlag = 0;

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < combCount; j++)
			{
				if (honeyCombArr[j][2])
				{
					collisionBeeAndComb(beeArr[i][0], beeArr[i][1], beeArr[i][2], beeArr[i][3], honeyCombArr[j][0], honeyCombArr[j][1], randomBees_x[i], beeArr[i][4]);
				}
			}
		}
		for (int i = 0; i < 6; i++)
		{
			if (beeArr[i][1] == 480 && beePlanted[i] == 0)
			{
				int idx = beeArr[i][0] / 64;
				if (obstacleArr[idx] == 0)
				{
					obsCount++;
					obstacleArr[idx] = 1;
					beePlanted[i] = 1;
					if (idx == 14 || idx == 13)
					{
						obstacleArr[13] = 1;
						obstacleArr[14] = 1;
						obsCount++;
					}
					else if (idx == 0 || idx == 1)
					{
						obstacleArr[1] = 1;
						obstacleArr[0] = 1;
						obsCount++;
					}
					if ((obstacleArr[int((player_x - boxPixelsX) / 64) - 1] && int(player_x / 64 != 0)) && (obstacleArr[int((player_x + boxPixelsX) / 64) + 1] && int(player_x / 64) != 14))
					{
						lives--;
						player_x = (gameRows / 2) * boxPixelsX;
						player_y = (gameColumns - 4) * boxPixelsY;
					}
					else if (obstacleArr[int(player_x / 64)] || obstacleArr[int((player_x + boxPixelsX) / 64)])
					{
						MoveToNearestEmpty(player_x, obstacleArr, isGameOver);
					}

					beeArr[i][2] = 1 - beeArr[i][2];
				}
			}
		}
		for (int i = 0; i < 15; i++)
		{
			if (obstacleArr[i])
				drawObstacle(window, i, obstacleSprite);
		}
		// for(int i = 0; i < )
		if (bullet_exists == true)
		{
			moveBullet(bullet_y, bullet_exists, bulletClock, isGameOver);
			drawBullet(window, bullet_x, bullet_y, bulletSprite);
		}
		else
		{
			if (Keyboard::isKeyPressed(Keyboard::Space) && bulletCount <= 120 && !isGameOver)
			{
				bullet_x = player_x;
				bullet_y = player_y;
				bullet_exists = true;
				bulletCount++;
				if (bulletCount > 30 && bulletCount <= 60)
				{
					widthDrop = 4;
					widthDropMultiplier = 1;
				}
				else if (bulletCount > 60 && bulletCount <= 90)
				{
					widthDrop = 2;
					widthDropMultiplier = 1;
				}
				else if (bulletCount > 90 && bulletCount <= 120)
				{
					widthDrop = 4;
					widthDropMultiplier = 3;
				}
				else if (bulletCount > 120)
				{
					bulletCount = 0;
					lives--;
					widthDrop = 1;
					widthDropMultiplier = 0;
				}
				fireEffect.openFromFile("Sound Effects/fire.wav");
				fireEffect.play();
			}
		}
		for (int i = 0; i < combCount; i++)
		{
			if (honeyCombArr[i][2])
				drawHoneyComb(window, honeyCombArr[i][0], honeyCombArr[i][1], honeyCombArr[i][3], honeyCombSprite, redHoneyCombSprite);
		}
		if (totBirdStep_x * 4 == stepBirdCount_x && totBirdStep_y * 4 == stepBirdCount_y)
		{
			for (int i = 0; i < combCount; i++)
			{
				collisionBirdAndComb(bird_x, bird_y, honeyCombArr[i][0], honeyCombArr[i][1], honeyCombArr[i][2], honeyCombArr[i][3], score);
			}
			// if(clockBird.getElapsedTime().asMilliseconds() >  1000)

			clockBird.restart();
			if (hummingBirdColor)
			{
				hummingBirdColor = 0;
				// putDelayOnBird = 1;
			}
			hummingCanWait.restart();
			stepBirdCount_x = 0;
			stepBirdCount_y = 0;
			initialBird_x = bird_x;
			initialBird_y = bird_y;
			if (currRand_x == randomBird_x && currRand_y == randomBird_y)
			{
				randomBird_x = 16 + (rand() % (800 - 16 + 1));
				randomBird_y = 32 + (rand() % (480 - 32 + 1));
			}
			currRand_x = randomBird_x;
			currRand_y = randomBird_y;
			if (bird_x > randomBird_x)
			{
				totBirdStep_x = (int)(bird_x / 32) - (int)(randomBird_x / 32);
			}
			else if (bird_x < randomBird_x)
			{
				totBirdStep_x = (int)(randomBird_x / 32) - (int)(bird_x / 32);
			}
			if (bird_y < randomBird_y)
			{
				totBirdStep_y = (int)(randomBird_y / 32) - (int)(bird_y / 32);
			}
			else if (bird_y > randomBird_y)
			{
				totBirdStep_y = (int)(bird_y / 32) - (int)(randomBird_y / 32);
			}
			// }
		}
		// dispkaying score
		levelScreen.setString("level : " + to_string(level));
		levelScreen.setScale(1.65, 1.65);
		levelScreen.setPosition(390, 580);
		window.draw(levelScreen);
		// cout << endl
		// 	 << score;
		scoreScreen.setString("Score: " + to_string(score));
		window.draw(scoreScreen);
		int Time = 1500;
		if (putDelayOnBird)
			Time = 4000;
		if (hummingCanWait.getElapsedTime().asMilliseconds() > Time)
		{
			if (beeDeadCount >= 3)
				moveBird(bird_x, bird_y, randomBird_x, randomBird_y, totBirdStep_x, totBirdStep_y, stepBirdCount_x, stepBirdCount_y, birdReached, initialBird_x, initialBird_y, isGameOver);
		}
		if (putDelayOnBird)
		{
			putDelayOnBird = 0;
		}
		float X = 16;
		float Y = 575;

		drawBird(window, bird_x, bird_y, birdSprite);
		if (!isGameOver)
		{
			switch (stateHeight)
			{
			case 0:
				drawPlayer(window, player_x, player_y, playerSprite);
				break;
			case 1:
				drawPlayer(window, player_x, player_y - 44, playerLargeSprite);
				break;
			case -1:
				drawPlayer(window, player_x, player_y + 40, playerShortSprite);
			}
		}
		playerRectSprite.setScale(scale_x, (widthDrop == 1 && widthDropMultiplier == 1) ? -scale_y : (bulletCount > 76) ? 0
																														: -(scale_y - (scale_y * widthDropMultiplier / widthDrop)));
		if (!isGameOver)
		{
			switch (stateHeight)
			{
			case 0:
				drawPlayerRect(window, player_x, player_y, playerRectSprite);
				break;
			case 1:
				drawPlayer(window, player_x + 10, player_y + 16, RectLongSprite);
				break;
			case -1:
				drawPlayer(window, player_x + 10, player_y + 67, RectShortSprite);
			}
		}
		for (int i = 1; i < lives; i++)
		{
			playerSprite.setScale(.75, .75);
			drawPlayer(window, X, Y, playerSprite);
			X = X + 50;
		}
		// Resetting in case lives become Zero
		if (lives == 0)
		{
			bgMusic.setVolume(0);
			gameoverMusic.play();
			isGameOver = 1;
			window.draw(gameover);
		}
		else if (preLives - lives == 1)
		{
			if (level == 4)
			{
				stateHeight = 0;
				stateSpeed = 0;
				level4(beeCount, beeDeadCount, combCount, hiveCount, bulletCount, beeArr, beeAlive, honeyCombArr, hiveArr, obstacleArr, entered4, player_x, player_y, beePlanted, bird_x, bird_y, hummingBirdColor, birdHitcount, widthDrop, widthDropMultiplier);
			}
			for (int i = 0; i < 6; i++)
			{
				beeAlive[i] = 1;
			}
			if (level == 3)
			{
				combCount = 10;
				for (int i = 0; i < 10; i++)
				{
					honeyCombArr[i][0] = 64 + (rand() % (700 - 64 + 1));
					honeyCombArr[i][1] = 32 + (rand() % (300 - 32 + 1));
					honeyCombArr[i][2] = 1;
					if (i % 3 == 0)
						honeyCombArr[i][3] = 1;
					else
						honeyCombArr[i][3] = 0;
				}
			}
			else if (level == 2)
			{
				combCount = 6;
				for (int i = 0; i < 9; i++)
				{
					honeyCombArr[i][0] = 64 + (rand() % (700 - 64 + 1));
					honeyCombArr[i][1] = 32 + (rand() % (300 - 32 + 1));
					honeyCombArr[i][2] = 1;
					if (i % 3 == 0)
						honeyCombArr[i][3] = 1;
					else
						honeyCombArr[i][3] = 0;
				}
			}
			hummingBirdColor = 0;
			birdHitcount = 0;
			switch (randomNumForBirdInitiaposition)
			{
			case 1:
				bird_x = -100;
				bird_y = -100;
				break;
			case 2:
				bird_x = 1000;
				bird_y = 820;
				break;
			case 3:
				bird_x = 1000;
				bird_y = -100;
				break;
			case 4:
				bird_x = -100;
				bird_y = 820;
			}
			widthDrop = 1;
			widthDropMultiplier = 1;
			preLives--;
			birdHitcount = 0;
			beeDeadCount = 0;
			beeCount = 6;
			int x = 898;
			for (int i = 0; i < 6; i++)
			{
				beeArr[i][4] = 0;
				beeArr[i][0] = -1;
				if (i % 2)
				{
					beeArr[i][2] = 1;
				}
				else
				{
					beeArr[i][2] = 0;
				}
				beeArr[i][1] = 32;
				beeArr[i][3] = 0;
			}
			beeArr[0][0] = x;
			beeArr[3][4] = 1;
			for (int i = 0; i < 15; i++)
			{
				obstacleArr[i] = 0;
			}
			for (int i = 0; i < 6; i++)
			{
				beePlanted[i] = 0;
			}
			player_x = (gameRows / 2) * boxPixelsX;
			player_y = (gameColumns - 4) * boxPixelsY;
			beeClock.restart();
		}
		if (level == 4)
		{
			for (int i = 0; i < powerUpCount; i++)
			{
				if (powerUps[i][2])
				{
					movePowerUp(powerUps[i][1]);
				}
			}
			for (int i = 0; i < powerUpCount; i++)
			{
				if (powerUps[i][2])
				{
					switch (int(powerUps[i][3]))
					{
					case 1:
						drawPowerUp(window, powerUps[i][0], powerUps[i][1], speedIncSprite);
						break;
					case 2:
						drawPowerUp(window, powerUps[i][0], powerUps[i][1], speedDecSprite);
						break;
					case 3:
						drawPowerUp(window, powerUps[i][0], powerUps[i][1], heightIncSprite);
						break;
					case 4:
						drawPowerUp(window, powerUps[i][0], powerUps[i][1], heightDecSprite);
					}
				}
			}
		}
		bool hivesExist = 0;
		int randomHiveInfant;
		if (level == 4)
		{
			for (int i = 0; i < hiveCount; i++)
			{
				if (hiveArr[i][2] == 1)
				{
					hivesExist = 1;
				}
			}
			if (hivesExist)
			{
				if (infantBeeClock.getElapsedTime().asMilliseconds() > 3000)
				{
					randomHiveInfant = sendRandomHive(hiveCount);
					if (hiveArr[randomHiveInfant][2] == 1 && beeDeadCount < 35)
					{
						CreateInfantBee(hiveArr[randomHiveInfant][0], max(hiveArr[randomHiveInfant][1] - 64, float(32)), infantBeeArr, infantBeeCount, infantBeeSize);
						infantBeeClock.restart();
						// break;
					}
				}
			}
		}

		for (int i = 0; i < infantBeeCount; i++)
		{
			if (infantBeeArr[i][2] == 0 || infantBeeArr[i][1] <= 16)
				continue;
			if (false == collisionCheckAboveBee(infantBeeArr[i][0], infantBeeArr[i][1], honeyCombArr, combCount, hiveArr, hiveCount))
			{
				moveInfantBee(infantBeeArr[i][1]);
			}
			else if (false == collisionCheckInfantBeeWithLeft(infantBeeArr[i][0], infantBeeArr[i][1], honeyCombArr, combCount, hiveArr, hiveCount))
			{
				moveInfantLeft(infantBeeArr[i][0]);
			}
			else if (false == collisionCheckInfantBeeWithRight(infantBeeArr[i][0], infantBeeArr[i][1], honeyCombArr, combCount, hiveArr, hiveCount))
			{
				moveInfantRight(infantBeeArr[i][0]);
			}
			else
			{
				infantBeeArr[i][2] = false;
				hiveArr[hiveCount][0] = GetCoord(infantBeeArr[i][0], 64, 16);
				hiveArr[hiveCount][1] = GetCoord(infantBeeArr[i][1], 64, 32);
				hiveArr[hiveCount][2] = true;
				hiveCount++;
			}
		}

		for (int i = 0; i < hiveCount; i++)
		{
			if (hiveArr[i][2] == 1)
				drawHive(window, hiveArr[i][0], hiveArr[i][1], hiveSprite);
		}
		for (int i = 0; i < infantBeeCount; i++)
		{

			if (infantBeeArr[i][2])
			{
				drawInfantBee(window, infantBeeArr[i][0], infantBeeArr[i][1], infantBeeSprite);
			}
		}
		if (stateHeight == 1 && powerUpEffectClock[2].getElapsedTime().asMilliseconds() < 10000)
		{
			playerSprite.setScale(1.25, 2);
			playerRectSprite.setScale(.85, (widthDrop == 1 && widthDropMultiplier == 1) ? -.5 * scale_y : (bulletCount > 76) ? 0
																															 : -.5 * (scale_y - (scale_y * widthDropMultiplier / widthDrop)));
		}
		else if (stateHeight == -1 && powerUpEffectClock[3].getElapsedTime().asMilliseconds() < 10000)
		{
			playerSprite.setScale(1.25, .625);
			playerRectSprite.setScale(.85, (widthDrop == 1 && widthDropMultiplier == 1) ? -2 * scale_y : (bulletCount > 76) ? 0
																															: -2 * (scale_y - (scale_y * widthDropMultiplier / widthDrop)));
		}
		if (stateSpeed == -1)
		{
			SpeedTimer.setString("SpeedDrop:" + to_string(10 - powerUpEffectClock[1].getElapsedTime().asSeconds()));
			window.draw(SpeedTimer);
		}
		else if (stateSpeed == 1)
		{
			SpeedTimer.setString("SpeedInc:" + to_string(10 - powerUpEffectClock[0].getElapsedTime().asSeconds()));
			window.draw(SpeedTimer);
		}
		if (stateHeight == -1)
		{
			heightTimer.setString("heightDrop:" + to_string(10 - powerUpEffectClock[3].getElapsedTime().asSeconds()));
			window.draw(heightTimer);
		}
		if (stateHeight == 1)
		{

			heightTimer.setString("heightInc:" + to_string(10 - powerUpEffectClock[2].getElapsedTime().asSeconds()));
			window.draw(heightTimer);
		}
		// window.draw(groundRectangle);
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
void drawInfantBee(RenderWindow &window, float &infant_x, float &infant_y, Sprite &infantBeeSprite)
{
	infantBeeSprite.setPosition(infant_x, infant_y);
	window.draw(infantBeeSprite);
}
void moveInfantLeft(float &X)
{
	X -= 64;
}
void moveInfantRight(float &X)
{
	X += 64;
}
void moveInfantBee(float &Y)
{
	Y -= 4;
}

bool collisionCheckAboveBee(float bee_x, float bee_y, float honeyCombArr[][4], int combCount, float hiveArr[][3], int hiveCount)
{
	bee_x = GetCoord(bee_x, 64, 16);
	bee_y = GetCoord(bee_y - 4, 64, 32);

	for (int i = 0; i < hiveCount; i++)
	{
		if (hiveArr[i][2] == 0)
			continue;
		float comb_x = GetCoord(hiveArr[i][0], 64, 16);
		float comb_y = GetCoord(hiveArr[i][1], 64, 32);

		if (bee_x == comb_x && bee_y == comb_y)
		{
			// cout << " up -- hive -- " << comb_x << " " << comb_y << endl;
			return true;
		}
	}
	for (int i = 0; i < combCount; i++)
	{
		if (honeyCombArr[i][2] == 0)
			continue;
		float comb_x = GetCoord(honeyCombArr[i][0], 64, 16);
		float comb_y = GetCoord(honeyCombArr[i][1], 64, 32);
		if (bee_x == comb_x && bee_y == comb_y)
		{
			// cout << " up -- comb -- " << comb_x << " " << comb_y << endl;

			return true;
		}
	}
	return false;
}
bool collisionCheckInfantBeeWithRight(float bee_x, float bee_y, float honeyCombArr[][4], int combCount, float hiveArr[][3], int hiveCount)
{
	bee_x = GetCoord(bee_x + 64, 64, 16);
	bee_y = GetCoord(bee_y, 64, 32);
	for (int i = 0; i < hiveCount; i++)
	{
		if (hiveArr[i][2] == 0)
			continue;
		float comb_x = GetCoord(hiveArr[i][0], 64, 16);
		float comb_y = GetCoord(hiveArr[i][1], 64, 32);
		if (bee_x == comb_x && bee_y == comb_y)
		{
			// cout << " right -- hive -- " << comb_x << " " << comb_y << endl;

			return true;
		}
	}
	for (int i = 0; i < combCount; i++)
	{
		if (honeyCombArr[i][2] == 0)
			continue;
		float comb_x = GetCoord(honeyCombArr[i][0], 64, 16);
		float comb_y = GetCoord(honeyCombArr[i][1], 64, 32);
		if (bee_x == comb_x && bee_y == comb_y)
		{
			// cout << "right  -- comb -- " << comb_x << " " << comb_y << endl;

			return true;
		}
	}
	return false;
}
bool collisionCheckInfantBeeWithLeft(float bee_x, float bee_y, float honeyCombArr[][4], int combCount, float hiveArr[][3], int hiveCount)
{
	bee_x = GetCoord(bee_x - 64, 64, 16);
	bee_y = GetCoord(bee_y, 64, 32);

	for (int i = 0; i < hiveCount; i++)
	{
		if (hiveArr[i][2] == 0)
			continue;
		float comb_x = GetCoord(hiveArr[i][0], 64, 16);
		float comb_y = GetCoord(hiveArr[i][1], 64, 32);
		if (bee_x == comb_x && bee_y == comb_y)
		{
			// cout << " left -- hive -- " << comb_x << " " << comb_y << endl;

			return true;
		}
	}
	for (int i = 0; i < combCount; i++)
	{
		if (honeyCombArr[i][2] == 0)
			continue;
		float comb_x = GetCoord(honeyCombArr[i][0], 64, 16);
		float comb_y = GetCoord(honeyCombArr[i][1], 64, 32);
		if (bee_x == comb_x && bee_y == comb_y)
		{
			// cout << " left -- comb -- " << comb_x << " " << comb_y << endl;

			return true;
		}
	}
	return false;
}
void drawPowerUp(RenderWindow &window, float &powerUp_x, float &powerUp_y, Sprite &powerUpSprite)
{
	powerUpSprite.setPosition(powerUp_x, powerUp_y);
	window.draw(powerUpSprite);
}
void movePowerUp(float &powerUp_y)
{
	if (powerUp_y + 8 < 544)
		powerUp_y += 8;
	if (powerUp_y + 8 == 544)
		powerUp_y += 8;
	if (powerUp_y + 8 > 544)
		powerUp_y == 544;
}
void eatingPowerUp(float &player_x, float &player_y, float &powerUp_x, float &powerUp_y, float &powerUpExist, float &powerUpType, int &atePowerUp, Clock powerUpClock[], int &stateSpeed, int &stateHeight)
{
	if (player_x <= powerUp_x + 32 && player_x + 32 > powerUp_x && player_y <= powerUp_y + 64 && player_y + 64 > powerUp_y && powerUpExist)
	{
		powerUpExist = 0;
		atePowerUp = powerUpType;
		powerUpClock[atePowerUp - 1].restart();
		switch (atePowerUp)
		{
		case 1:
			stateSpeed += 1;
			break;
		case 2:
			stateSpeed -= 1;
			break;
		case 3:
			stateHeight += 1;
			break;
		case 4:
			stateHeight -= 1;
		}
		switch (stateSpeed)
		{
		case 2:
			stateSpeed = 1;
			powerUpClock[0].restart();
			break;
		case -2:
			stateSpeed = -1;
			powerUpClock[1].restart();
		}
		switch (stateHeight)
		{
		case 2:
			stateHeight = 1;
			powerUpClock[2].restart();
			break;
		case -2:
			stateHeight = -1;
			powerUpClock[3].restart();
		}
	}
}
void drawPlayer(RenderWindow &window, float player_x, float player_y, Sprite &playerSprite)
{
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}
void drawPlayerRect(RenderWindow &window, float &player_x, float &player_y, Sprite &playerRectSprite)
{
	playerRectSprite.setPosition(player_x + 10, player_y + 82);
	window.draw(playerRectSprite);
}
void moveBullet(float &bullet_y, bool &bullet_exists, Clock &bulletClock, bool isGameOver)
{
	if (!isGameOver)
	{
		if (bulletClock.getElapsedTime().asMilliseconds() < 20)
			return;
		bulletClock.restart();
		bullet_y -= 38;
		if (bullet_y < -38)
			bullet_exists = false;
	}
}
void drawBullet(sf::RenderWindow &window, float &bullet_x, float &bullet_y, Sprite &bulletSprite)
{
	// if(bullet_exists==true)
	{
		bulletSprite.setPosition(bullet_x, bullet_y);
		window.draw(bulletSprite);
	}
}
void drawBee(RenderWindow &window, float &bee_x, float &bee_y, float &beeType, Sprite &beeSprite, Sprite &fastBeeSprite, Sprite &infantBeeSprite)
{
	if (!beeType)
	{
		beeSprite.setPosition(bee_x, bee_y);
		window.draw(beeSprite);
	}
	else if (beeType == 1)
	{
		fastBeeSprite.setPosition(bee_x, bee_y);
		window.draw(fastBeeSprite);
	}
	else if (beeType == 2)
	{
		infantBeeSprite.setPosition(bee_x, bee_y);
		window.draw(infantBeeSprite);
	}
}
void drawHoneyComb(RenderWindow &window, float &honeycomb_x, float &honeycomb_y, float &beeType, Sprite &honeyCombSprite, Sprite &redHoneyCombSprite)
{
	if (!beeType)
	{
		honeyCombSprite.setPosition(honeycomb_x, honeycomb_y);
		window.draw(honeyCombSprite);
	}
	else
	{
		redHoneyCombSprite.setPosition(honeycomb_x, honeycomb_y);
		window.draw(redHoneyCombSprite);
	}
}
void drawObstacle(RenderWindow &window, int idx, Sprite &obstacleSprite)
{
	obstacleSprite.setPosition(idx * 64, 520);
	window.draw(obstacleSprite);
}
void drawBird(RenderWindow &window, float bird_x, float bird_y, Sprite &birdSprite)
{
	birdSprite.setPosition(bird_x, bird_y);
	window.draw(birdSprite);
}
void drawHive(RenderWindow &window, float hive_x, float hive_y, Sprite &hiveSprite)
{
	hiveSprite.setPosition(hive_x, hive_y);
	window.draw(hiveSprite);
}
bool moveBee(float &bee_x, float &bee_y, float &beeDir, float &beeVertical, float &randomBees_x, float &beeType, int beeIdx, bool isGameOver)
{
	if (!isGameOver)
	{
		if (beeVertical)
		{
			bee_y += 8;
			if ((int)bee_y % 64 == 32)
				beeVertical = 0;
			return true;
		}
		if (!beeDir && bee_x >= 64)
		{
			if (!beeType || beeType == 2)
				bee_x -= 8;
			else
				bee_x -= 16;
		}
		if (beeDir && bee_x < 898)
		{
			if (!beeType || beeType == 2)
				bee_x += 8;
			else
				bee_x += 16;
		}
		if (beeDir && bee_x >= 898)
		{
			if (bee_y == 480)
			{
				if (beeIdx % 2)
				{
					bee_x = 898;
					beeDir = 0;
				}
				else
				{
					bee_x = 16;
					beeDir = 1;
				}
				bee_y = 32;
				return false;
			}
			beeVertical = 1;
			randomBees_x = ((rand() % 116) + 1) * 8;
			beeDir = 1 - beeDir;
		}
		if (!beeDir && bee_x <= 64)
		{
			if (bee_y == 480)
			{
				if (beeIdx % 2)
				{
					bee_x = 898;
					beeDir = 0;
				}
				else
				{
					bee_x = 16;
					beeDir = 1;
				}
				bee_y = 32;
				return false;
			}

			beeVertical = 1;
			randomBees_x = ((rand() % 116) + 1) * 8;
			beeDir = 1 - beeDir;
		}
	}
	return true;
}
bool collisionCheck(float bullet_x, float bullet_y, float &bee_x, float &bee_y, float &bee_Dir, int beeIdx, bool &bullet_exists, int &beeCount, bool &beeAlive, bool &isCollision, int &level, int &beeDeadCount, int &redbeeCount, float &beeType, float &beeTypeInComb, bool beealive[])
{
	if (bullet_x <= bee_x + 32 && bullet_x + 32 > bee_x && bullet_y <= bee_y + 32 && bullet_y + 32 > bee_y && beeAlive)
	{
		beeTypeInComb = beeType; // honeycombArr[combCount][3] stores beeTypeIComb
		isCollision = 1;
		if (beeType)
		{
			beeType = 0; // beeArr[i][5]
		}
		else
		{
			if (level == 2 && beeDeadCount % 4 == 0)
			{
				redbeeCount++;
				beeType = 1;
			}
			else if ((level == 3 || level == 4) && beeDeadCount % 2 == 0)
			{
				redbeeCount++;
				beeType = 1;
			}
		}
		if (beeDeadCount <= 29)
		{
			if (beeIdx % 2)
			{
				bee_x = 898;
				bee_Dir = 0;
			}
			else
			{
				bee_x = 16;
				bee_Dir = 1;
			}
			bee_y = 32;
		}
		bullet_exists = false;
		return true;
	}
	return false;
}
void collisionCheckcombAndBullet(float bullet_x, float bullet_y, float &comb_x, float &comb_y, float &combExist, bool &bullet_exists, bool &isCollision)
{
	if (bullet_x <= comb_x + 32 && bullet_x + 32 > comb_x && bullet_y <= comb_y + 32 && bullet_y + 32 > comb_y && combExist)
	{
		combExist = 0;
		if (bullet_y < 416)
			bullet_exists = false;
		isCollision = 1;
	}
}
void collisionBeeAndComb(float &bee_x, float &bee_y, float &beeDir, float &beeVertical, float comb_x, float comb_y, float &randomBees_x, float &beeType)
{
	if (bee_x <= comb_x + 64 && bee_x + 64 > comb_x && bee_y <= comb_y + 64 && bee_y + 64 > comb_y && beeType != 1)
	{
		beeVertical = 1;
		beeDir = 1 - beeDir;
	}
}
void collisionCheckHiveAndBullet(float &bullet_x, float &bullet_y, float &hive_x, float &hive_y, bool &isCollision, float &hiveExists)
{
	if (bullet_x <= hive_x + 32 && bullet_x + 32 > hive_x && bullet_y <= hive_y + 32 && bullet_y + 32 > hive_y && hiveExists)
	{
		isCollision = 1;
	}
}
void MoveToNearestEmpty(float &player_x, float obs[], bool isGameOver)
{
	if (!isGameOver)
	{
		int idx = player_x / 64;
		if (obs[idx] == false)
			idx++;
		int leftNearest = -1000;
		int RightNearest = 1000;
		for (int i = idx; i >= 0; i--)
		{
			if (obs[i] == false)
			{
				leftNearest = i;
				break;
			}
		}
		for (int i = idx; i < 15; i++)
		{
			if (obs[i] == false)
			{
				RightNearest = i;
				break;
			}
		}
		if (RightNearest - idx > idx - leftNearest)
		{
			player_x = leftNearest * 64;
		}
		else
			player_x = RightNearest * 64;
	}
}
bool moveBird(float &bird_x, float &bird_y, int randomBird_x, int randomBird_y, int totBirdStep_x, int totBirdStep_y, int &stepBirdCount_x, int &stepBirdCount_y, bool &destinationReached, float &initialBird_x, float &initialBird_y, bool isGameOver)
{
	if (!isGameOver)
	{
		if (stepBirdCount_x < totBirdStep_x * 4)
		{
			if (initialBird_x < randomBird_x)
				bird_x += 8;
			else if (initialBird_x > randomBird_x)
				bird_x -= 8;
			stepBirdCount_x++;
		}
		if (stepBirdCount_y < (totBirdStep_y) * 4)
		{
			if (initialBird_y > randomBird_y)
				bird_y -= 8;
			else if (initialBird_y < randomBird_y)
				bird_y += 8;
			stepBirdCount_y++;
		}
	}
	if (stepBirdCount_x == totBirdStep_x && stepBirdCount_y == totBirdStep_y)
		return true;
	return false;
}
void combCollisionForHive(float &bee_x, float &bee_y, float &beeDir, bool &collision, float comb_x, float comb_y)
{
	// cout<<"Entered combCollisionForHive\n";
	if (bee_x <= comb_x + 32 && bee_x + 32 > comb_x && bee_y <= comb_y + 32 && bee_y + 64 > comb_y)
	{
		collision = 1;
	}
	// cout<<"Left combCollisionForHive";
}
void collisionHummingBirdAndBullet(float &bullet_x, float &bullet_y, float &bird_x, float &bird_y, int &birdHitcount, bool &bullet_exists, bool &isCollision)
{
	if (bullet_x <= bird_x + 32 && bullet_x + 32 > bird_x && bullet_y <= bird_y + 32 && bullet_y + 32 > bird_y)
	{
		birdHitcount++;
		bullet_exists = false;
		isCollision = 1;
	}
}
void collisionBirdAndComb(float &bird_x, float &bird_y, float &comb_x, float &comb_y, float &combExist, float &combType, int &Score)
{
	if (bird_x <= comb_x + 32 && bird_x + 32 > comb_x && bird_y <= comb_y + 32 && bird_y + 64 > comb_y)
	{
		combExist = 0;
		if (combType == 0)
			if (bird_y <= 96)
			{
				Score += 1000;
			}
			else if (bird_y <= 288)
			{
				Score += 800;
			}
			else
				Score += 500;
		else
		{
			if (bird_y <= 96)
			{
				Score += 2000;
			}
			else if (bird_y <= 288)
			{
				Score += 1800;
			}
			else
				Score += 1500;
		}
	}
}
void level2(float beeArr[][6], bool &entered, float honeyCombArr[][4], int &combCount, float obstacleArr[], float &player_x, float &player_y)
{
	// cout << ":: LEVEL 2 REACHED ::";
	entered = 1;
	int x = 898;
	for (int i = 0; i < 6; i++)
	{

		beeArr[i][4] = 0;
		beeArr[i][0] = -1;
		if (i % 2)
		{
			beeArr[i][2] = 1;
		}
		else
		{
			beeArr[i][2] = 0;
		}
		beeArr[i][1] = 32;
		beeArr[i][3] = 0;
	}
	beeArr[0][0] = x;
	beeArr[3][4] = 1;
	for (int i = 0; i < combCount; i++)
	{
		honeyCombArr[i][2] = 0;
	}
	for (int i = 0; i < 15; i++)
	{
		obstacleArr[i] = 0;
	}
	player_x = (gameRows / 2) * boxPixelsX;
	player_y = (gameColumns - 4) * boxPixelsY;
}
void level3(float beeArr[][6], bool &entered, float honeyCombArr[][4], int &combCount, float obstacleArr[], float &player_x, float &player_y)
{
	// cout << ":: LEVEL 3 REACHED ::";
	entered = 1;
	int x = 898;
	for (int i = 0; i < 6; i++)
	{

		beeArr[i][4] = 0;
		beeArr[i][0] = -1;
		if (i % 2)
		{
			beeArr[i][2] = 1;
		}
		else
		{
			beeArr[i][2] = 0;
		}
		beeArr[i][1] = 32;
		beeArr[i][3] = 0;
	}
	beeArr[0][0] = x;
	beeArr[2][4] = 1;
	beeArr[5][4] = 1;
	for (int i = 0; i < combCount; i++)
	{
		honeyCombArr[i][2] = 0;
	}
	for (int i = 0; i < 15; i++)
	{
		obstacleArr[i] = 0;
	}
	player_x = (gameRows / 2) * boxPixelsX;
	player_y = (gameColumns - 4) * boxPixelsY;
}
void level4(int &beeCount, int &beeDeadCount, int &combCount, int &hiveCount, int &bulletCount, float beeArr[][6], bool beeAlive[], float honeyCombArr[][4], float hiveArr[][3], float obstacleArr[], bool entered4, float &player_x, float &player_y, bool beePlanted[], float &bird_x, float &bird_y, bool &birdColor, int &birdHitCount, int &widthDrop, int &widthDropMultiplier)
{
	beeDeadCount = 0;
	beeCount = 6;
	combCount = 6;
	hiveCount = 4;
	for (int i = 0; i < 6; i++)
	{
		beeAlive[i] = 1;
	}
	entered4 = 1;
	int x = 898;
	for (int i = 0; i < 6; i++)
	{

		beeArr[i][4] = 0;
		beeArr[i][0] = -1;
		if (i % 2)
		{
			beeArr[i][2] = 1;
		}
		else
		{
			beeArr[i][2] = 0;
		}
		beeArr[i][1] = 32;
		beeArr[i][3] = 0;
	}
	beeArr[0][0] = x;
	beeArr[3][4] = 1;
	for (int i = 0; i < combCount; i++)
	{
		honeyCombArr[i][2] = 0;
	}
	for (int i = 0; i < 15; i++)
	{
		obstacleArr[i] = 0;
	}
	for (int i = 0; i < 6; i++)
	{
		beePlanted[i] = 0;
	}
	for (int i = 0; i < 10; i++)
	{
		honeyCombArr[i][0] = 64 + (rand() % (750 - 64 + 1));
		honeyCombArr[i][1] = 64 + (rand() % (300 - 64 + 1));
		honeyCombArr[i][2] = 1;
		if (i % 4 == 0)
			honeyCombArr[i][3] = 1;
		else
			honeyCombArr[i][3] = 0;
	}
	for (int i = 0; i < 5; i++)
	{
		hiveArr[i][0] = 32 + (rand() % (750 - 32 + 1));
		hiveArr[i][1] = 64 + (rand() % (300 - 64 + 1));
		hiveArr[i][2] = 1;
	}
	player_x = (gameRows / 2) * boxPixelsX;
	player_y = (gameColumns - 4) * boxPixelsY;
	int randomNumForBirdInitiaposition4 = (rand() % 4) + 1;
	switch (randomNumForBirdInitiaposition4)
	{
	case 1:
		bird_x = -100;
		bird_y = -100;
		break;
	case 2:
		bird_x = 1000;
		bird_y = 820;
		break;
	case 3:
		bird_x = 1000;
		bird_y = -100;
		break;
	case 4:
		bird_x = -100;
		bird_y = 820;
	}
	birdHitCount = 0;
	birdColor = 0;
	widthDrop = 1;
	widthDropMultiplier = 1;
	bulletCount = 0;
}
void collisionCheckRedcombAndbulletForLevel4(float &bullet_x, float &bullet_y, float &redComb_x, float &redComb_y, bool &isCollision, float &combExist, bool &bulletExist)
{
	if (bullet_x <= redComb_x + 32 && bullet_x + 32 > redComb_x && bullet_y <= redComb_y + 32 && bullet_y + 32 > redComb_y && combExist)
	{
		if (bullet_y < 416)
			bulletExist = 0;
		isCollision = 1;
		combExist = 0;
	}
}

bool BeeCollideswithCombHorizontally(float HiveArr[][3], float honeyCombArr[][4], int combCount, int Hivecount, float beeArr[], bool direction)
{
	float beeX = beeArr[0];
	float beeY = beeArr[1];

	if (direction)
	{
		beeX += 24;
	}
	else
	{
		beeX -= 24;
	}
	for (int i = 0; i < Hivecount; i++)
	{
		if (HiveArr[i][2] && beeX <= HiveArr[i][0] + 32 && beeX + 32 > HiveArr[i][0] && beeY <= HiveArr[i][1] + 32 && beeY + 32 > HiveArr[i][1])
			return true;
	}
	for (int i = 0; i < combCount; i++)
	{
		if (honeyCombArr[i][2] && beeX <= honeyCombArr[i][0] + 32 && beeX + 32 > honeyCombArr[i][0] && beeY <= honeyCombArr[i][1] + 32 && beeY + 32 > honeyCombArr[i][1])
		{
			// cout << "honeycomn hor collide\n";
			return true;
		}
	}
	return false;
}
bool BeeColldideswithCombVertically(float HiveArr[][3], float honeyCombArr[][4], int combCount, int Hivecount, float beeArr[])
{
	float beeX = beeArr[0];
	float beeY = beeArr[1] + 32;

	for (int i = 0; i < Hivecount; i++)
	{
		if (HiveArr[i][2] && beeX <= HiveArr[i][0] + 32 && beeX + 32 > HiveArr[i][0] && beeY <= HiveArr[i][1] + 32 && beeY + 32 > HiveArr[i][1])
		{
			// cout << "HiveArr ver collide\n";
			return true;
		}
	}
	for (int i = 0; i < combCount; i++)
	{
		if (honeyCombArr[i][2] && beeX <= honeyCombArr[i][0] + 32 && beeX + 32 > honeyCombArr[i][0] && beeY <= honeyCombArr[i][1] + 32 && beeY + 32 > honeyCombArr[i][1])
		{
			// cout << "honeycomn ver collide\n";
			return true;
		}
	}
	return false;
}
bool collisionCheckInfantBullet(float &bullet_x, float &bullet_y, float &infant_x, float &infant_y, float &infantExist)
{
	if (bullet_x <= infant_x + 32 && bullet_x + 32 > infant_x && bullet_y <= infant_y + 32 && bullet_y + 32 > infant_y && infantExist == 1)
	{
		return true;
	}
	return false;
}
void spawnBee(int level, float beeArr[][6], bool beePlanted[], int &beeCount, bool beeAlive[], int &beeDeadCount, int i, float **infantBee, int infantBeecount)
{
	if (level == 1 || level == 2)
	{
		beeArr[i][4] = 0;
		beePlanted[i] = 0;
		if (beeCount < 20)
		{
			beeAlive[i] = true;
			beeCount++;
		}
		else
			beeAlive[i] = false;
		beeDeadCount++;
		beeArr[i][4] = 0;
		if (beeDeadCount % 4 == 0 && level == 2)
			beeArr[i][4] = 1;
	}
	else if (level == 3)
	{
		beeArr[i][4] = 0;
		beePlanted[i] = 0;
		if (beeCount < 30)
		{
			beeAlive[i] = true;
			beeCount++;
		}
		else
			beeAlive[i] = false;
		beeDeadCount++;
		beeArr[i][4] = 0;
		if (beeDeadCount % 3 == 0)
			beeArr[i][4] = 1;
	}
	else if (level == 4)
	{
		beeArr[i][4] = 0;
		beePlanted[i] = 0;
		if (beeDeadCount <= 29)
		{
			beeAlive[i] = true;
			beeCount++;
		}
		else
			beeAlive[i] = false;

		beeDeadCount++;
		beeArr[i][4] = 0;
		if (beeDeadCount % 3 == 0)
			beeArr[i][4] = 1;

		for (int j = 0; j < infantBeecount; j++)
		{
			if (infantBee[j][2] && infantBee[j][1] <= 32)
			{
				infantBee[j][2] = 0;
				beeArr[i][0] = infantBee[j][0];
				beeArr[i][1] = infantBee[j][1];
				beeAlive[i] = true;
				beeArr[i][4] = 1;
				break;
			}
		}
	}
}
