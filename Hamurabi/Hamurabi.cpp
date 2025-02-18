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
    int totalImmigration; //the total amount of immigrants in the city


    float numKilledPercentage = 0;

    gameState() : acresCount(1000), popCount(100), bushelsCount(3000), year(1), totalImmigration(0)
    {
        srand(time(0));
    }

    void saveGame(std::string Game_Save_Data) //saving function
    {
        std::ofstream file(Game_Save_Data);//opens a file
        if (file.is_open())
        {
            file << acresCount << "\t"
                << popCount << "\t"
                << bushelsCount << "\t"
                << year << "\t"
                << totalImmigration; //if it opens these variables will be added
            file.close(); //close the file
            std::cout << "save successful" << std::endl;
        }
        else
        {
            std::cout << "saving unsuccessful.";
        }
    }

    bool loadGame(std::string Game_Save_Data) //like the saveGame but loads it instead
    {
        std::ifstream file(Game_Save_Data); //opens the file to read from
        if (file.is_open())
        {
            file >> acresCount
                >> popCount
                >> bushelsCount
                >> year
                >> totalImmigration; //adds each number into the corresponding variable
            file.close();
            std::cout << "load successful" << std::endl;
            return true;
        }
        else
        {
            std::cout << "loading unsuccessful.";
            return false;
        }
    }

    //make sure the user hasnt gone past the goal
    bool checkGameStatus(int numKilled, int tempPopCount)
    {
        
        numKilledPercentage = (static_cast<float>(numKilled) / tempPopCount) * 100; //converts the variables into floats 
         //the amount of people killed in the year divided by the amount of living people, * 100 to get the percentage of deaths
        std::cout << numKilledPercentage << " " << tempPopCount << " " << numKilled << std::endl;;

        //check for alive citizens
        if (popCount <= 0)
        {
            std::cout << "Everyone has starved to death." 
                << std::endl;
            return true;

        }


        if (year >= 10)
        {
            std::cout << "Congratulations Dear Leader. \nYou have successfully completely your 10 year term"
                << std::endl;

            return true;
        }

        if (numKilledPercentage >= 50)
        {
            std::cout << "At a staggering "
                << numKilledPercentage << "%, you have let a total of "
                << numKilled << " of our citizens die!\nYou shall pay for your poor leadership with your life."
                << "\nYou are beheaded in front of the remains of your people.\nStart again."
                << std::endl;
            return true;
        }
        else if (numKilledPercentage == 0)
        {
            std::cout << "No one died this year.\nHow wonderful."
                << std::endl;
        }
        else
        {
            std::cout << numKilled << " starved to death.\nThat's "
                << numKilledPercentage << "% of our population, now dead."
                << std::endl;
        }
        
        return false;


    }


    bool checkInput(int varCheck)
    {
        if (std::cin.fail() || varCheck < 0) //input validation
        {
            std::cout << "Try Again."
                << std::endl;; //actually tell the user what happened
            std::cin.clear();//clears the inputso it can be redone by the user
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//clears the buffer
            return true;//ensure the users has to redo their input
        }
        
        return false;//returns false if inputs are valid
        
    }
};


//a class to store the actual game stuff
class mainGame
{
private:
    gameState game;


public:

    void start(bool load) {
        if (load) {
            if (!game.loadGame("Hamurabi.txt")) {
                std::cout << "Starting a new game instead.\n"; 
            }
        }

        bool playing = true;
        while (playing) {
            playing = gameYear();
        }
    }
    


    bool gameYear(){
        int userFoodInp, userPlantingInp, userLandPurchInp; //variables for user inputs

        //some constants
        const int foodPerPerson = 20; //each person will always require 20 bushels
        const int plantingCost = 2; //each acre can only be planted with 2 bushels

        //general variables ill need
        int populationFed, populationStarved = 0; //how many people are fed and how many are starved
        int extraFoodCount, immigrationCount = 0; //how much extra food was given, how many immigrants there in the current year

        

        int tempPopCount = 0;
        tempPopCount += game.popCount;

        int harvestPerAcre, totalHarvest = 0; //how many bushels each acre can produce, the total harvest for the year
        int totalFertileAcres = 0, acresPlanted = 0; //the amount of acres able to be planted, amount of acres planted

        //user input for saving later on
        int userSaveInput;


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
            if (game.checkInput(userFoodInp)) continue;


            //get the users spending 
            std::cout << "How many acres do you want to plant? ";
            std::cin >> userPlantingInp;
            //game.checkInput(userPlantingInp);
            if (game.checkInput(userPlantingInp)) continue;
            

            std::cout << "One acre of land costs "
                << randLandPrice 
                << "\nHow many acres of land do you want to purchase? ";
            std::cin >> userLandPurchInp;
            if (game.checkInput(userLandPurchInp)) continue;



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

        //updating the player's various counts
        game.acresCount += userLandPurchInp; //add the amount of acres user bought
        game.bushelsCount -= (userLandPurchInp * randLandPrice); //remove the money they paid for the land
        game.bushelsCount -= userFoodInp;


        //feeding the population
        populationFed = userFoodInp / foodPerPerson; //get the number of people fed
        if (populationFed < game.popCount) //if there are less people fed than the total population, the amount of deaths will be calculated
        {
            populationStarved = game.popCount - populationFed;
            game.popCount = populationFed;

        }
        else if (populationFed > game.popCount) //if there is more food given than needed, the number of immigrants will be workd out
        {
            extraFoodCount = userFoodInp - (game.popCount * foodPerPerson);
            immigrationCount = extraFoodCount / foodPerPerson;
            game.totalImmigration += immigrationCount; //keep track of total immigration numbers
            game.popCount += immigrationCount;
            std::cout << immigrationCount
                << " immigrants have joined our beloved city"
                << std::endl;
        }
        


        //farming stuff
        for (int i = 0; i < game.popCount; i++) //basically goes tyhrough each of the population and works out how much they harvest
        {
            totalFertileAcres += 5 + (rand() % 6); //random number from 5 - 10
        }

        //acresPlanted = userPlantingInp * plantingCost; //each acre requires 2 bushels to be planted
        if ((acresPlanted = userPlantingInp * plantingCost) > game.acresCount) acresPlanted = game.acresCount; //ensure that user cannot plant more than owned
         

        //std::cout << "Bushels before land purchase: " << game.bushelsCount << "\n";
        //game.bushelsCount -= (userLandPurchInp * randLandPrice);
        //std::cout << "land: " << userLandPurchInp * randLandPrice << " After purchase: " << game.bushelsCount << "\n";

        

        harvestPerAcre = 2 + (rand() % 5); //random number from 2 - 6
        totalHarvest = acresPlanted * harvestPerAcre;
        //std::cout << "Bushels before harvest: " << game.bushelsCount << "\n";
        game.bushelsCount += totalHarvest;
        //std::cout << "Harvested: " << totalHarvest << " After harvest: " << game.bushelsCount << "\n";
        //figure out how many bushels user gets from acres


        //years summary
        std::cout << "\n"
            << acresPlanted << " acres of land were planted.\n"
            << totalHarvest << " bushels were harvested.\n"
            << immigrationCount << " immigrants moved into the city.\n"
            << game.totalImmigration << " total immigrants have joined the city so far\n"
            << std::endl;



        if (game.checkGameStatus(populationStarved, tempPopCount)) return false;//stop the game is the game has reached 10 years
        

        game.year++; //actualy increase the round by 1

        do
        {
            //give the user the option to save
            std::cout << "\nWould you like to save?"
                << "\n1. Yes \n2. No \n3. Save and quit \n4. Quit without saving"
                << std::endl;
            std::cin >> userSaveInput;
            game.checkInput(userSaveInput);

            if (userSaveInput == 1)
            {
                game.saveGame("Hamurabi.txt");
                return true;
            }
            else if (userSaveInput == 2)
            {
                return true;
            }
            else if (userSaveInput == 3)
            {
                game.saveGame("Hamurabi.txt");
                return false;
            }
            else if (userSaveInput == 4)
            {
                return false;
            }
            /*else
            {
                std::cout << "Continuing game" << std::endl;
            }*/
        } while (true);
        return true;
    }



};

//a class that does menu stuff
class gameMainMenu
{

public:
    void mainMenu()
    {
        int menuUserChoice;
        mainGame game;
        gameState checks;

        do//a dowhile loop so it repeat if needed
        {
            std::cout << "\n______------Hamurabi------______\n";
            std::cout << "1.Start New Game\n";
            std::cout << "2.Load Game\n";
            std::cout << "3.How To Play\n";
            std::cout << "4.Exit\n";
            std::cin >> menuUserChoice;
            if (checks.checkInput(menuUserChoice)) continue;


            
            switch (menuUserChoice)
            {
            case 1:
                game.start(false);
                return;

            case 2:
                game.start(true);
                return;

            case 3:
                htpInstructions();
                break;
            case 4:
                std::cout << "Bye."
                    << std::endl;
                exit(0);
            default:
                std::cout << "Invalid. \nPick again."
                    << std::endl;

            }
        } while (true);
    }


        void htpInstructions() //instructions on how the game works
        {
            std::cout << "As our beloved Dear Leader, you are tasked with allocating bushels of grain to our people! \nYou have been given a 10 year term to prove yourself."
                << "\nHowever, you must be be careful as you only have so many bushels of grain to feed our people with."
                << "\n\n\nEach person requires 20 bushels to survive throughout the year."
                << "\nOver-allocating food will bring in immigrants who will be accounted for within the next fiscal year."
                << "\nEach person that has been fed will be able to harvest up to 10 acres of land for the year. \nEach acre of land require 2 bushels to plant seeds for the year"
                << "\nFinally, any remaining bushels can be used to purchase more land, if you so wish."
                << std::endl;
        }
};

int main()
{
    gameMainMenu menu;
    while (true)
    {
        menu.mainMenu(); //player goes back to menu when they press quit midgame
    }
    

    return 0;
}

