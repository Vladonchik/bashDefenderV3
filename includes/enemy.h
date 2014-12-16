#ifndef TEST_Enemy_h
#define TEST_Enemy_h

#include "../includes/gameProperties.h"
#include "../includes/Display.h"

#define DELAY_BETWEEN_ENEMIES	1

typedef struct path *Path;
typedef struct levelPaths *LevelPaths;
typedef struct enemy *Enemy;
typedef struct enemyGroup *EnemyGroup;



typedef enum typeOfEnemy	{

	intBasic = 1,
	intHeavy = 2,
	charBasic =3,
	charHeavy =4
} typeOfEnemy;


typedef enum BOOL {TRUE, FALSE} BOOL;
void createEnemyGroup();
void freeEnemyGroup();
void createEnemy();
int getNumberOfEnemies();
void Test_createEnemy();
void initialiseEnemy(Enemy newEnemy);
EnemyGroup getEnemyGroup(EnemyGroup enemyList);
int getEnemyHealth();
void freeEnemy(int enemyID);
int moveEnemy(int enemyID);
int isDead(int enemyID);
int inRange(int tX, int tY, int tRange, int enemyID);
void damageEnemy(int damage, int enemyID);
void towerGetTargetPos(int * towerTargetPosition, int enemyID);
void printEnemy(int enemyID);
void createPath();
void freePath(Path p);
void freeLevelPaths();
int **getPathPointer(int **newPath);
void testEnemy();
int setEnemyX(int enemyID, int newX);
int setEnemyY(int enemyID, int newY);
int setEnemyHealth(int enemyID, int newHealth);
int setEnemyArmour(int enemyID, int newArmour);
void present_enemy(Display d);
void createLevelPaths();
LevelPaths getLevelPaths(LevelPaths pathList);
void assignMemoryForPaths(int numberOfPaths);
void layPaths(int numberOfPaths);
void createHorizontalPath(Path P);
void createDogLegPath(Path P);

void initialiseHeavyEnemy(Enemy newEnemy);
int distanceToEndOfPath(int enemyID);


#endif
