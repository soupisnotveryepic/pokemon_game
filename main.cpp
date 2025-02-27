//main file
#include "Pokemon.h"
#include "Player.h"
#include "Enemy.h"
#include "Attack.h"
#include "QuickAttack.h"
#include "SigAttack.h"
#include "TypeAttack.h"
#include "medicine.h"
#include "Multiplier.h"
#include "Random.h"

//#include <opencv2/opencv.hpp>  // need this: sudo apt-get update, sudo apt-get install libopencv-dev

#include <limits>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>

// Platform specific includes
#ifdef _WIN32
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

// Function to simulate getch()
int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Menu options
std::string pokemonMenu[] = {"Squirtle", "Bulbasaur", "Charmander", "Quit Game"};
std::string mainMenu[] = {"Fight", "Heal", "Quit Game"};
std::string mainMenu2[] = {"Fight", "Quit Game"};
std::string battleMenu[] = {"Signature", "Type Attack", "Quick Attack"};
std::string healMenu[] = {"Morphine", "Codeine", "Panadol"};
std::string confirmMenu[]={"Confirm this Pokemon", "Choose different Pokemon"};

int confirmSize = 2;
int pokemonMenuSize = 4;
int mainMenuSize = 3;
int mainMenuSize2 = 2;
int battleMenuSize = 3;
int healMenuSize = 3;

// Function to display a menu and get the selected option
int displayMenu(std::string menu[], int menuSize) {
    int highlight = 0;
    int choice = 0;
    char c;

    // Print menu
    while (1) {
        system("clear || cls"); // Clear console screen (cross-platform)
        
        for (int i = 0; i < menuSize; i++) {
            if (i == highlight)
                cout << "> "; // Highlight current selection
            else
                cout << "  ";
            
            cout << menu[i] << endl;
        }

        c = _getch();  // Get user input
        
        if (c == 27) {  // Check for arrow keys (esc sequence starts with 27)
            _getch();    
            switch (_getch()) {
                case 'A':
                    highlight = (highlight == 0) ? menuSize - 1 : highlight - 1;
                    break;
                case 'B':
                    highlight = (highlight == menuSize - 1) ? 0 : highlight + 1;
                    break;
            }
        } else if (c == 10 || c == 13) { // Enter key
            choice = highlight;
            return choice;
        }
    }
}

using namespace std;

int main(){
    
    system("clear");
    //game intro
    cout << "Welcome to Pokemon Battles!"<<"\n";
    cout << "The aim of the game is to defeat as many pokemons as you can." << "\n";
    cout << "Your chosen Pokemon will have  HP, strengths and weaknesses." << "\n\n" ;
    cout << "HP is how much health your pokemon still has. There are 3 types of attack: quick attack deals 10 damage with 80% success rate, \n";
    cout << "type attack deals 30 damage with 50% success rate, and the signature attack deals 70 damage, but requires \n";
    cout << "you to have had 5 successful attacks. \n\n";
    cout << "As each Pokemon has a type, the damage to the opponent will be doubled if it is a strength, and halved if it is a weakness of the enemy's. \n";
    cout << "Only type attack and signature move will be the same type as the pokemon, hence, quick attack will have no multiplier. \n\n";
    cout << "There also medicines availible to increase your HP, and can be utilised instead of an attack. \n";
    cout << "At any point requiring an input in the game, you can enter 0, to exit the game. \n \n \n";

    cout << "Enter any key to continue - note the information above will be cleared. ";
    _getch();

    system("clear");

    //number of enemies beaten
    int num_wins = 0;

    //display first three available Pokemon stats, and get user to select from them and store in chosen_ID
    //initialise first 3 pokemon to print their info. -this has been tested, and it works.

    Pokemon Pok1 = Pokemon(1);
    Pokemon Pok2 = Pokemon(2);
    Pokemon Pok3 = Pokemon(3);

    cout<< "Time to choose your pokemon!"<<endl;
    _getch();
    int chosen_ID = 0;
    while(chosen_ID == 0){
        int pokChoice = displayMenu(pokemonMenu, pokemonMenuSize);
        if ( pokemonMenu[pokChoice] == "Squirtle") {
            system("clear || cls");
            Pok1.print_Pokemon_info(1);
            cout<<"\n Do you confirm this pokemon choice? \n";
            _getch();
            int confirm = displayMenu(confirmMenu, confirmSize);
            if (confirmMenu[confirm] == "Confirm this Pokemon"){
                chosen_ID = 1;
            }

        } else if (pokemonMenu[pokChoice] == "Bulbasaur") {
            system("clear || cls");
            Pok2.print_Pokemon_info(2);
            cout<<"\n Do you confirm this pokemon choice? \n";
            _getch();
            int confirm = displayMenu(confirmMenu, confirmSize);
            if (confirmMenu[confirm] == "Confirm this Pokemon"){
                chosen_ID = 2;
            }
        } else if (pokemonMenu[pokChoice] == "Charmander") {
            system("clear || cls");
            Pok3.print_Pokemon_info(3);
            cout<<"\n Do you confirm this pokemon choice? \n";
            _getch();
            int confirm = displayMenu(confirmMenu, confirmSize);
            if (confirmMenu[confirm] == "Confirm this Pokemon"){
                chosen_ID = 3;
            }
        } else if (pokemonMenu[pokChoice] == "Quit Game"){
            return 0;
        }
    }
    //initiate chosen pokemon and assign it to player
    Pokemon player_Pok = Pokemon(chosen_ID);
    Player player_user_Pok = Player(player_Pok);

    
    player_user_Pok.set_name(); //base class Pokemon implementation of naming function (virtual)
    while (player_user_Pok.name == "0") {
        system("clear");
        cout << "Game exited. " << endl;
        return 0;
    }

    player_Pok.name = player_user_Pok.name;
    

    
    //Initialise medicines
    player_user_Pok.set_medicine_bag(3);
    player_user_Pok.add_medicine(1, medicine("Panadol", 25));
    player_user_Pok.add_medicine(2, medicine("Codeine", 50));
    player_user_Pok.add_medicine(3, medicine("Morphine", 75));

    //
    
    int num_attacks = 0; //for later so you don't have to display attack information after first attack
    cout << "It's time for your first battle!" << endl;
    _getch();
    while(player_Pok.get_Pokemon_HP() > 0) { 
        //rng to get random pokemon ID Pok_ID
        int Pok_ID = Random::rand(1,8);
        
        
        //initiate pokemon and assign it to enemy and display its information
        Pokemon enemy_Pok = Pokemon(Pok_ID);
        Enemy enemy_user_Pok = Enemy(enemy_Pok);


        enemy_user_Pok.set_name(enemy_Pok);
        if(enemy_user_Pok.name == "0") {
            system("clear");
            cout << "Game exited. " << endl;
            return 0;  
        }
        enemy_Pok.HP = enemy_user_Pok.HP + enemy_Pok.HP; // add 0, 20 or 40 to current enemy HP based on choice
        enemy_Pok.name = enemy_user_Pok.name;
        
        cout << "The new enemy pokemon information is below:" << endl;
        enemy_Pok.print_Pokemon_info(Pok_ID);
        _getch();
        cout << "For this enemy pokemon: " << endl;
        //multiplier function
        Multiplier mult = Multiplier();

        double multiplier;
        multiplier = mult.find_mult(player_Pok, enemy_Pok);
        _getch();
        system("clear");

        while (enemy_Pok.get_Pokemon_HP() > 0){
        cout << "Your HP: " << player_Pok.get_Pokemon_HP() << "\nEnemy HP: " << enemy_Pok.get_Pokemon_HP() << endl;
        _getch();
        system("clear");
        //ask user if they would like to use a medicine or attack
        //either 1 or 2 or do it like highlighted text etc
        int choice = 0;;
        int mainChoice;
        //change menu depending on medicine count
        if (player_user_Pok.get_medicine_count()>0){
            mainChoice = displayMenu(mainMenu, mainMenuSize);
        } else{
            mainChoice = displayMenu(mainMenu2, mainMenuSize2);
        }

        //choose medicine
        if (mainMenu[mainChoice] == "Heal"){
            int medicine_choice = 0;
            system("clear");
            cout << "You have " << player_user_Pok.get_medicine_count() << " medicines. Which medicine you would like to use:" << endl;
            for (int i = 0; i <= (player_user_Pok.get_medicine_count() - 1); i++){
                cout << (i + 1) << " - " << (*(player_user_Pok.get_medicine_bag_pointer() + i)).getMedicineName() << endl;
            }
            while(medicine_choice <=0 || medicine_choice > player_user_Pok.get_medicine_count()){
                cin >> medicine_choice;
                if (medicine_choice == 0) {
                    system("clear");
                    cout << "Game exited. " << endl;
                    return 0;
                }
                if(medicine_choice <=0 || medicine_choice > player_user_Pok.get_medicine_count()){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input.  Try again: "<<endl;
                    _getch();
                }
            }
            medicine selected_medicine = *(player_user_Pok.get_medicine_bag_pointer() + (medicine_choice - 1));
            int current_hp = player_Pok.get_Pokemon_HP();
            int hp_gained = selected_medicine.getMedicineHealing();

            player_Pok.set_Pokemon_HP(current_hp + hp_gained);
            cout << "Your Pokemon gained " << hp_gained << "HP. Your total HP is now " << player_Pok.get_Pokemon_HP() << "." << endl;
            _getch();
            player_user_Pok.remove_medicine(medicine_choice);

            choice =0;
        }    
        //chosen attack
        else if (mainMenu[mainChoice] == "Fight") {
            system("clear");   
            cout << "You have " << player_Pok.get_num_hits() << " successful attacks. " << endl;
            _getch();
            cout << "Pick an attack: " << endl;
            _getch();
            int attack_choice = displayMenu(battleMenu, battleMenuSize);
    
            int chance = Attack::success_rate();

            //quick attack
            if (battleMenu[attack_choice] == "Quick Attack") {
                QuickAttack quickattack1 = QuickAttack(chance, player_Pok);
                _getch();
                if (quickattack1.damage > 0) {
                    player_Pok.inc_num_hits(player_Pok.get_num_hits());
                }

                quickattack1.damage =  quickattack1.damage;
            
                enemy_Pok.HP_drain(quickattack1.damage);
                cout << "You attacked the enemy with " << quickattack1.damage << " damage." << endl;
                _getch();
            }

            //type attack
            if (battleMenu[attack_choice] == "Type Attack") {
                TypeAttack typeattack1 = TypeAttack(chance, player_Pok);
                _getch();
                if (typeattack1.damage > 0) {
                    player_Pok.inc_num_hits(player_Pok.get_num_hits());
                }
                typeattack1.damage = multiplier * typeattack1.damage;
                
                enemy_Pok.HP_drain(typeattack1.damage);
                cout << "You attacked the enemy with " << typeattack1.damage << " damage." << endl;
                _getch();
            }

            //sig attack
            if (battleMenu[attack_choice] == "Signature") {
                SigAttack sigattack1 = SigAttack(player_Pok.get_num_hits(), player_Pok);
                _getch();

                //multiplier function
                sigattack1.damage = multiplier * sigattack1.damage;
                
                enemy_Pok.HP_drain(sigattack1.damage);
                if (sigattack1.damage > 0) {
                    int new_hits;
                    new_hits = player_Pok.get_num_hits() - 5;
                    player_Pok.set_num_hits(new_hits);
                    cout << "You attacked the enemy with " << sigattack1.damage << " damage." << endl;
                    _getch();
                }
            }
        } //end attack section
        else if (mainMenu[mainChoice] == "Quit Game"){
            return 0;
        }


        //enemy attack
        //random number generator between 1 and 2
        int enemy_attack_choice = Random::rand(1,2);
        int chance2 = Attack::success_rate();

        //quick attack
        if (enemy_attack_choice == 1) {
            QuickAttack quickattack2 = QuickAttack(chance2, enemy_Pok);
            player_Pok.HP_drain(quickattack2.damage);
            cout << "The enemy attacked you with " << quickattack2.damage << " damage." << endl;
            _getch();
        }

        //type attack
        if (enemy_attack_choice == 2) {
            TypeAttack typeattack2 = TypeAttack(chance2, enemy_Pok);
            player_Pok.HP_drain(typeattack2.damage);
            cout << "The enemy attacked you with " << typeattack2.damage << " damage." << endl;
            _getch();
        }
        //if player has lost
        if (player_Pok.get_Pokemon_HP() <= 0) {
        break;
        }

        if (enemy_Pok.get_Pokemon_HP() <=0) {
            num_wins++;
            cout << "You beat " << enemy_Pok.name << "! \n";
            _getch();
        }
        }//end enemy while loop
        
    } //end player while loop
    /*
    //load image
    cv::Mat image = cv::imread("temp.jpg");
    //create window
    cv::namedWindow("YOU SUCK!!!");
    // show the image in the window
    cv::imshow("YOU SUCK!!!", image);
    cv::waitKey(0);
    */

    cout << "You lose! You beat " << num_wins << " enemies. " << endl;
    //save number of enemies beaten to text file 
    //read section
    ifstream scoreIn("score.txt");
    if (!scoreIn) {
        cerr << "Error: Unable to open file " << endl;
        return 0;
    }

    string line;
    int previousBest;

    if (scoreIn.is_open()) {
        // skip first line 
        getline(scoreIn, line);
        //skip line 2        
        getline(scoreIn, line);

        // read val in prev best
        getline(scoreIn, line);
        size_t pos = line.find(": ");
        previousBest = stoi(line.substr(pos + 2));

        scoreIn.close();
    }

    //write section
    ofstream scoreOut("score.txt");
    //update
    if (num_wins > previousBest) {
        previousBest = num_wins;
    }    

    //check if the file was opened
    if (!scoreOut) {
        cerr << "Error: Unable to open file " << endl;
        return 0;
    }

    if (scoreOut.is_open()) {
        scoreOut << "You have now lost!!" << endl;
        scoreOut << "You scored: " << num_wins << endl;
        scoreOut << "Your previous best is: " << previousBest << endl;
        scoreOut.close();
    }   

    delete player_user_Pok.medicine_bag;

return 0;
}
        
