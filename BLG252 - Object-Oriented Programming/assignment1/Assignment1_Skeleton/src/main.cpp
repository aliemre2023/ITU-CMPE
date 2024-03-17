//--- 2023-2024 Spring BLG 252E Object Oriented Programing Assignment 1 ---//
//--------------------------//
//---Name & Surname: Ali Emre Kaya
//---Student Number: 150210097
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream> 
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "pokemon.h"

using namespace std;

//-------------Do Not Change These Global Variables-------------//
int NAME_COUNTER = 0;   //Use this to keep track of the enemy names
int POK_COUNTER = 0;    //Use this to keep track of the pokemon names
int PLAYER_POKEMON_ATTACK = 20; //You don't have to use this variable but its here if you need it
int ENEMY_POKEMON_ATTACK = 10;  //You don't have to use this variable but its here if you need it
//--------------------------------------------------------------//

//---If Necessary Add Your Global Variables Here---// 
int catch_or_fight = 0;

//-------------------------------------------------//

//-------------Do Not Change These Function Definitions-------------//
string* readEnemyNames(const char*);
string* readPokemonNames(const char*);
player characterCreate(string, int);
void mainMenu();
void fightEnemy(player*, string*, string*);
void catchPokemon(player*, string*);
//------------------------------------------------------------------//

//---If Necessary Add Your Function Definitions Here---// 
//
//
//
//-----------------------------------------------------//

//-------------Do Not Change This Function-------------//
int main(int argc, char* argv[]){
	system("clear");

    //---Creating Enemy and Pokemon Name Arrays---//
    string* enemyNames = readEnemyNames(argv[1]);         
    string* pokemonNames = readPokemonNames(argv[2]);
    

    string playerName;
    int pokemonChoice;

    cout << "Welcome to the Pokemon Game! \n" << endl;
    cout << "Please enter your name: "; 
    cin >> playerName;
    cout << "Please choose one of these pokemons as your pokemon: \n1- Bulbasaur \n2- Charmender \n3- Squirtle \nChoice: ";
    cin >> pokemonChoice;

    //---Character Creation--//
    player newPlayer = characterCreate(playerName, pokemonChoice);
    
    int menuChoice;

    while(true){
        mainMenu(); 
        cin>>menuChoice;

        switch (menuChoice){
        case 1:
            fightEnemy(&newPlayer, enemyNames, pokemonNames);
            break;
        case 2:
            catchPokemon(&newPlayer, pokemonNames);
            break;
        case 3:
            cout<<newPlayer.showPokemonNumber()<<endl;
            break;
        case 4:
            cout<<newPlayer.showPokeballNumber()<<endl;
            break;
        case 5:
            cout<<newPlayer.showBadgeNumber()<<endl;
            break;
        case 6:
            cout << endl;
            cout << "------- Pokedex -------" <<endl;
            newPlayer.playerPokedex.printMyPokedex();
            break;
        case 7:
            //---Deletes Dynamic Arrays From Memory and Exits---//
            delete[] enemyNames;
            delete[] pokemonNames;
            return EXIT_SUCCESS;
            break;
  
        default:
            cout << "Please enter a valid number!!!" << endl;
            break;
        }
    }
    return EXIT_FAILURE;
};
//-----------------------------------------------------//

//-------------Code This Function-------------//
string* readEnemyNames(const char* argv){
    //---Code Here---//
    ifstream file;
    file.open(argv);

    string count_str;
    string name;
    getline(file, count_str);
    int count_int = stoi(count_str);

    string* temp_enemyNames = new string[count_int];
    
    int i = 0; 
    while(getline(file, name)){
        temp_enemyNames[i] = name;
        i++;
    }

    return temp_enemyNames;
    //---------------//   
};
//-----------------------------------------------------//

//-------------Code This Function-------------//
string* readPokemonNames(const char* argv){
    //---Code Here---//
    ifstream file;
    file.open(argv);

    string count_str;
    string name;
    getline(file, count_str);
    int count_int = stoi(count_str);
     
    string* temp_pokemonNames = new string[count_int];

    int i = 0;
    while(getline(file, name)){
        temp_pokemonNames[i] = name;
        i++;
        POK_COUNTER++;  
    }
    return temp_pokemonNames;
    //---------------//
};
//-----------------------------------------------------//

//-------------Code This Function-------------//
player characterCreate(string playerName, int pokemonChoice){
    //---Code Here---//
    pokedex newPokedex;
    pokemon newPokemon("", 20);
    if(pokemonChoice == 1){
        newPokemon.set_name("Bulbasaur");
        newPokemon.set_atkValue(20);
    }
    if(pokemonChoice == 2){
        newPokemon.set_name("Charmender");
        newPokemon.set_atkValue(20);
    }
    if(pokemonChoice == 3){
        newPokemon.set_name("Squirtle");
        newPokemon.set_atkValue(20);
    }
    player newPlayer(playerName, newPokemon);
    newPlayer.main_pokemon(newPokemon);
    
    return newPlayer;
    //---------------//
};

//--------------------------------------------//

//-------------Do Not Change This Function-------------//
void mainMenu(){
    cout << endl;
    cout << "-------- Menu --------" << endl;
    cout << "1. Fight for a badge" << endl;
    cout << "2. Catch a Pokemon" << endl;
    cout << "3. Number of Pokemons" << endl;
    cout << "4. Number of Pokeballs " << endl;
    cout << "5. Number of Badges" << endl;
    cout << "6. Pokedex" << endl;
    cout << "7. Exit" << endl;
    cout << endl;
    cout << "Choice: ";
};
//-----------------------------------------------------//

//-------------Code This Function-------------//
void fightEnemy(player* newPlayer, string* enemyNames, string* pokemonNames){
    //---Code Here---//
    string enemyName = enemyNames[catch_or_fight];
    string pokemonName = pokemonNames[catch_or_fight];

    pokemon enemy_poke(pokemonName, 10);
    enemy newEnemy(enemyName, enemy_poke);
    newPlayer->playerPokedex.updatePokedex(enemy_poke);

    int my_atk = newPlayer->getPokemon().get_atkValue(); // 20?
    int en_atk = newEnemy.getPokemon().get_atkValue(); // 10

    cout << "You encounter with " << enemyName << " and his/hers pokemon " << pokemonName << "\n";
    cout << pokemonName << " Health: " << newEnemy.getPokemon().get_hpValue() << " Attack:" << en_atk << "\n";
    cout << "1- Fight" << "\n"; 
    cout << "2- Runaway" << "\n";
    cout << "Choice: ";
    int choice;
    cin >> choice;

    while(newPlayer->getPokemon().get_hpValue() > 0 && newEnemy.getPokemon().get_hpValue() > 0){
        if(choice == 1){
            newPlayer->getPokemon().set_hpValue((newPlayer->getPokemon().get_hpValue()) - (en_atk));
            newEnemy.getPokemon().set_hpValue((newEnemy.getPokemon().get_hpValue()) - (my_atk));

            cout << "Your Pokemons Healt: " << newPlayer->getPokemon().get_hpValue() << "\n";
            cout << enemyName << " Pokemons Healt: " << newEnemy.getPokemon().get_hpValue() << "\n";

            if(newEnemy.getPokemon().get_hpValue() <= 0){
                newPlayer->battleWon();
                break;
            }
            else if(newPlayer->getPokemon().get_hpValue() <= 0){
                cout << "You Lose!" << "\n";
                break;
            }
            else{
                cout << "1- Fight" << "\n";
                cout << "2- Runaway" << "\n";
                cout << "Choice: ";
                cin >> choice;
            }
        }
        else if(choice == 2){
            break;
        }

    }
    catch_or_fight++;
    //---------------//
};
//--------------------------------------------//

//-------------Code This Function-------------//
void catchPokemon(player* newPlayer, string* pokemonNames) {
    pokemon newPokemon;
    
    newPokemon.set_name(pokemonNames[catch_or_fight]);
    
    string poke_name = newPokemon.get_name();
    int poke_hp = newPokemon.get_hpValue();
    int poke_atk = newPokemon.get_atkValue();

    newPlayer->playerPokedex.updatePokedex(newPokemon);

    cout << "You encounter with " << poke_name << " Health: " << poke_hp << " Attack: " << poke_atk << "\n";
    cout << "1- Catch" << "\n";
    cout << "2- Runaway" << "\n";
    cout << "Choice: ";
    int choice;
    cin >> choice;

    if(choice == 1){
        newPlayer->catchThePokemon(newPokemon);
    }
    else if(choice == 2){
        cout << "You ranaway" << "\n";
    }
    catch_or_fight++;
    //---------------//
}
//--------------------------------------------//