#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../includes/sput.h"
#include "../includes/levelController.h"
#include "../includes/abilities.h"
#include "../includes/tower.h"
#include "./../includes/actionQueueDataStructure.h"
#include "../includes/parser.h"
#include "../includes/main.h"
#include "../includes/enemy.h"
#include "../includes/Sound.h"
#include "../includes/abilities.h"

int main(int argc, char ** argv)
{
    srand(time(NULL));
	int restart = 0;
    int started = 0;
	Display d = init_SDL();
//  testing();
    while(started == 0){
    	menu_screen(d, &started);
    }

	do	{
		restart = 0;
    	initLevel(1);
		//tutorialLevel(d,&restart);
		startLevel(d,&restart);
		endLevel();
	} while (restart);

    
    shutSDL(d);
    quitGame();
    return 0;
}

void tutorialLevel(Display d,int *restart)	{
	tutPhase tPhase = phaseOne;
    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    int ended = 0;
   	int pause = 0; 
    int steps=0;
	
    //init_sound();
    //playBackgroundSound();
	addClock(tutorialClock);
	int damage;
	int currMemory;
    do{
        startFrame(d);
		while(pause)	{
			pause_screen(d,&pause,restart);
		}
        ++steps;
        drawBackground();
        

		switch(tPhase)	{

			case phaseOne:
					tutorial_one();
					if(checkClock(tutorialClock,TUTORIALCOOLDOWN_SHORT))	{
						tPhase++;
					}
					break;
			case phaseTwo:
					tutorial_two();
					if(getNumOfTowers() > 0)	{
						damage = getTowerDamage(1);
						tPhase++;
					}
					break;
			case phaseThree:
					tutorial_three();
					if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
						tPhase++;
					}
					break;
			case phaseFour:
					startNextWave();
					tutorial_four();
					if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
						tPhase++;
					}
					break;
			case phaseFive:
					tutorial_five();
					if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
						tPhase++;
						currMemory = getAvailableMemory();
					}
					break;
			case phaseSix:	
					if(getTowerDamage(1) > damage)	{
						tutorial_six();
						tPhase++;
					} else if(getAvailableMemory() < currMemory) {
						tutorial_five_error();
					}
					break;
			case phaseSeven:
					startNextWave();
					if(getWave(getGame(NULL)) == 2)  {
						if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
							tPhase++;
						}
					}
					break;
			case phaseEight:
					tutorial_seven();
					if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
						tPhase++;
					}
					break;
			case phaseNine:
					tutorial_eight();
					if(checkCharType())  {
						tPhase++;
					}
					break;
			case phaseTen:
					startNextWave();
					if(getDeathCnt() > 0)	{
						tPhase++;
					}
					break;
			case phaseEleven:
					tutorial_nine();
					if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
						tPhase++;
					}
					break;
			default:


					break;
		}

        levelQueueReader();
        terminal_window(d, pass, clear,&pause, *restart);
    	popToTower();
        if(inputCommand)
        {
            parse(inputCommand);
        }
        present_enemy(d);
        present_tower(d);

    	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            int move = moveEnemy(i);
        }
        presentAnimation();
    	drawAllTowerPositions();
    	tutorialUpdateAllInfoWindow();
        endFrame(d);
        
        //ended = checkIfPlayerDead();
        while (ended) {
            //final screen returns 1 if restart button was pressed...
            if (final_screen()){
                ended = 0;
            }
        }
        
    } while(!terminal_window(d, pass, clear,&pause, *restart));
		printf("finished\n");    
}

void startLevel(Display d, int *restart)	{

    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    int ended = 0;
   	int pause = 0; 
    int steps=0;

    //init_sound();
    //playBackgroundSound();
    do{
        startFrame(d);
		while(pause)	{
			pause_screen(d,&pause,restart);
		}
        ++steps;
        drawBackground();
        
    	startNextWave();
        levelQueueReader();
        terminal_window(d, pass, clear,&pause, *restart);
    	popToTower();
        if(inputCommand)
        {
            parse(inputCommand);
        }
        present_enemy(d);
        present_tower(d);

    	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            int move = moveEnemy(i);
        }
        presentAnimation();
    	drawAllTowerPositions();
        //updateAllInfoWindow();
        endFrame(d);
        
        //ended = checkIfPlayerDead();
        while (ended) {
            //final screen returns 1 if restart button was pressed...
            if (final_screen()){
                ended = 0;
            }
        }
        
    } while(!terminal_window(d, pass, clear,&pause, *restart));
		printf("finished\n");    
}

void quitGame()
{
    //shutSound();
}

void testing()	{

	setUpTesting();
	//!Unit Tests	
	testLevelController(); //! Working
	testingTowerPositions(); //!Working
    testingGameStructure(); //!Memory Tests Failing
    testingActionQueue(); //! Working
    //testEnemy(); // ! No longer works.
    testingTowerModule(); //! working

   	//! System Tests 
   	parseToQueueTesting(); //!Segfaults
	//parseToTowerTesting(); //!Segfaults
    //towerToEnemyTesting(); //! Doesnt work.  Firing and range dont seem to be working
	enemyToGamePropertiesTesting();
    testingInformationWindowModule();

}

void enemyToGamePropertiesTesting()	{

	sput_start_testing();
    sput_set_output_stream(NULL);

	sput_enter_suite("testEnemyDeath(): Game Properties capturing enemy deaths corectly");
	sput_run_test(testEnemyDeath);
	sput_leave_suite();
}

void testEnemyDeath()	{
	int enemyID = createSpecificEnemy(1,1,1), 
		currDeathCnt = getDeathCnt(),
		currMemory = getAvailableMemory();
	damageEnemy(50,enemyID,1);
	sput_fail_unless(getDeathCnt() > currDeathCnt, "Valid: One Enemy has died");
	sput_fail_unless(getAvailableMemory() > currMemory,"Valid: Enemy has died and added to available memory");
	resetEnemyCounts();
}

void towerToEnemyTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testEnemyInRange(): tower detecting single enemy in range");
	sput_run_test(testEnemyInRange);
	sput_leave_suite();

	sput_finish_testing();
}

void testEnemyInRange()	{

	createEnemy();
	setEnemyHealth(1,100);
	setEnemyArmour(1,0);
	setEnemyX(1,50);
	setEnemyY(1,50);
	setTowerY(1,400);
	setTowerX(1,400);
	setTowerRange(1,15);
	setTowerDamage(1,10);
	sput_fail_unless(inRange(400,400,10,1)== 0, "Enemy 1 is out of range of tower 1");
	fire();
	sput_fail_unless(getEnemyHealth(1) == 100, "Out of range enemy 1 has full health after tower has fired");
	setEnemyX(1,400);
	setEnemyY(1,400);
	sput_fail_unless(inRange(400,400,39,1)== 1, "Enemy 1 is in range of tower 1");
	sput_fail_unless(getEnemyHealth(1) == 100, "Enemy 1 has full health");
	fire();
	//sput_fail_unless(getEnemyHealth(1) == 100 - getTowerDamage(1),"In range enemy has reduced health from tower damage");
	int i;
	for(i = 0; i < 9; i++)	{
	fire();
	}
	sput_fail_unless(isDead(1) == 1, "Enemy dead after being fired on 10 times");	
	
}

void parseToTowerTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testParseToTower():Tower popping parsed commands");
	sput_run_test(testParseToTower);
	sput_leave_suite();
	
	sput_finish_testing();

}

void parseToQueueTesting()	{
	
	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testValidParses():Testing Valid parsed commands being placed on queue");
	sput_run_test(testValidParses);
	sput_leave_suite();
	
	sput_finish_testing();
}

void testParseToTower()	{

	freeAllTowers();
	clearQueue();	
	createTowerFromPositions(1);
	createTowerFromPositions(2);
	int originalValue = 10;
	setTowerRange(1,originalValue); //Setting tower range to 10 for tests.
	setTowerDamage(2,originalValue); //Setting tower damage to 10 for tests.
	addMemory(10000);
	parse("upgrade r t1");
	parse("upgrade p t2");
	sput_fail_unless(getFirstTarget() == 1, "First target is 1");
	sput_fail_unless(getLastTarget() == 2, "Last target is 2");
	delayGame(ACTIONCOOLDOWN);
	popToTower();
	sput_fail_unless(getTowerRange(1) > originalValue, "Upgraded range is greater than original value");
	delayGame(ACTIONCOOLDOWN);
	popToTower();
	sput_fail_unless(getTowerDamage(2) > originalValue, "Upgraded Damage is greater than original Value");
	freeAllTowers();
	clearQueue();
}

void testValidParses()	{
	
	createTower();
    sput_fail_unless(parse("upgrade r t1")== 1, "upgrade r t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "First command in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_range, "First option in queue: range");
    sput_fail_unless(parse("upgrade p t1")== 1, "upgrade p t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_power, "Last option in queue: power");

    sput_fail_unless(parse("upgrade s t1")== 1, "upgrade s t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_speed, "Last option in queue: speed");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "First command in queue: upgrade");
    sput_fail_unless(parse("  ??D--") == 0, "  ??D-- is invalid command");
    sput_fail_unless(parse("upgrade r r1") == 0, "upgrade r r1 is invalid command");
    sput_fail_unless(parse("upgrade r t") == 0, "upgrade r t is invalid command");
    sput_fail_unless(parse("upgrade t") == 0, "upgrade t is invalid command");
    sput_fail_unless(parse("cat t") == 0, "cat t is invalid command");
	freeAllTowers();
	clearQueue();
}

