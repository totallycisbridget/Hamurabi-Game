#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>


//a thing that stores the important stuff and holds basic checks stuff
struct gameState
{
    int acresCount; //amount of land owned
    int popCount; //the current population number
    int bushelsCount; //amount of bushels owned
    int year; //current year


    gameState() : acresCount(1000), popCount(100), bushelsCount(3000), year(1)
    {
        srand(time(0));
    }

    void saveGame(std::string Game_Save_Data) //saving function
    {
        std::ofstream file(Game_Save_Data);//opens a file
        if (file.is_open())
        {
            file << acresCount << "    "
                << popCount << "    "
                << bushelsCount << "    "
                << year; //if it opens these variables will be added
            file.close(); //close the file
            std::cout << "save successful" << std::endl;
        }
        else
        {
            std::cout << "saving unsuccessful.";
        }
    }

    void loadGame(std::string Game_Save_Data) //like the saveGame but loads it instead
    {
        std::ifstream file(Game_Save_Data); //opens the file to read from
        if (file.is_open())
        {
            file >> acresCount
                >> popCount
                >> bushelsCount
                >> year; //adds each number into the corresponding variable
            file.close();
            std::cout << "load successful" << std::endl;
        }
        else
        {
            std::cout << "loading unsuccessful.";
        }
    }

    //make sure the user hasnt gone past the goal
    bool checkGameStatus()
    {
        //check for alive citizens
        if (popCount <= 0)
        {
            std::cout << "Everyone has starved to death." 
                << std::endl;
            return false;

        }
        if (year >= 10)
        {
            std::cout << "Congratulations Dear Leader. \nYou have successfully completely your 10 year term"
                << std::endl;

            return true;
        }

        return false;


    }
};


//a class to store the actual game stuff
class mainGame
{
private:
    gameState game;


public:
    void start()
    {
        char userLoadInput; //to store user input for if they wanna load a game
        bool playing = true; 

        std::cout << "Would you like to load a save? (y/n"
            << std::endl;
        std::cin >> userLoadInput;

        if (userLoadInput == 'y' || userLoadInput == 'Y')
        {
            game.loadGame("Game_Save_Data.txt");
        }

        while (playing)
        {
            playing = gameYear();
        }
    }


    bool gameYear(){
        int userFoodInp, userPlantingInp, userLandPurchInp; //variables for user inputs

        //general variables ill need
        int populationFed, populationStarved = 0;
        int extraFoodCount, immigrationCount = 0;
        int harvestPerAcre, totalHarvest = 0; //variables for harvesting
        int totalFertileAcres = 0, acresPlanted = 0;

        //user input for saving later on
        char userSaveInput;


        int randLandPrice = 17 + (rand() % 10); //gets a random number between 0 and 9 and adds it onto 17 to get anything from 17 to 26


        std::cout << "\nDear Leader, I wish to present this year's report... \nIn year "
            << game.year << ", the population was "
            << game.popCount << "\nYou own "
            << game.acresCount << " acres of land. \nThere are "
            << game.bushelsCount << " bushels in storage"
            << std::endl;

        
        do //this dowhile loop will loop through if the user tries to spend too much
        {
            std::cout << "How many bushels do you want to feed the people? ";
            std::cin >> userFoodInp;
            //get the users spending 
            std::cout << "How many acres do you want to plant? ";
            std::cin >> userPlantingInp;

            std::cout << "One acre of land costs "
                << randLandPrice 
                << "\nHow many acres of land do you want to purchase ? ";
            std::cin >> userLandPurchInp;



            //some input validation stuff
            if (userFoodInp + userPlantingInp + (userLandPurchInp * randLandPrice) > game.bushelsCount) //adds the three spending together and checks if its bigger than the amount they have.
            {
                std::cout << "You do not have enough bushels." << std::endl;
            }
            else
            {
                break;
            }
        } while (true);

        game.acresCount += userLandPurchInp; //add the amount of acres user bought
        game.bushelsCount -= (userLandPurchInp * randLandPrice); //remove the money they paid for the land

        populationFed = userFoodInp / 20; //get the number of people fed
        if (populationFed < game.popCount) //if there are less people fed than the total population, the amount of deaths will be calculated
        {
            populationStarved = game.popCount - populationFed;
            game.popCount = populationFed;
        }
        else if (populationFed > game.popCount) //if there is more food given than needed, the number of immigrants will be workd out
        {
            extraFoodCount = userFoodInp - (game.popCount * 20);
            immigrationCount = extraFoodCount / 20;
            game.popCount += immigrationCount;
            std::cout << immigrationCount
                << " immigrants have joined our beloved city"
                << std::endl;
        }
        game.bushelsCount -= userFoodInp;

        //farming stuff
        for (int i = 0; i < game.popCount; i++)
        {
            totalFertileAcres += 5 + (rand() % 6); //random number from 5 - 10
        }

        acresPlanted = userPlantingInp / 2; //each acre requires 2 bushels to be planted
        if (acresPlanted > game.acresCount) acresPlanted = game.acresCount; //ensure that user cannot plant more than owned
        if (acresPlanted < totalFertileAcres) acresPlanted = totalFertileAcres; 

        game.bushelsCount -= (acresPlanted * 2);

        harvestPerAcre = 2 + (rand() % 5); //random number from 2 - 6
        totalHarvest = acresPlanted * harvestPerAcre;
        game.bushelsCount += totalHarvest; //figure out how many bushels user gets from acres


        //years summary
        std::cout << "\n"
            << populationStarved << " starved to death.\n"
            << acresPlanted << " acres of land were planted.\n"
            << totalHarvest << " bushels were harvested\n"
            << std::endl;

        game.year++;

        if (game.checkGameStatus()) //stop the game is the game has reached 10 years
        {
            return false;
        }

        //give the user the option to save
        std::cout << "\nWould you like to save? (y/n/q yes, no or quit): ";
        std::cin >> userSaveInput;

        if (userSaveInput == 'y' || userSaveInput == 'Y')
        {
            game.saveGame("Game_Save_Data.txt");
            return true;
        }
        else if (userSaveInput == 'q' || userSaveInput == 'Q')
        {
            game.saveGame("Game_Save_Data.txt");
            return false;
        }
        return true;
    }



};

int main()
{
    mainGame game; //create a game object
    game.start();//start the game

    return 0;
}

