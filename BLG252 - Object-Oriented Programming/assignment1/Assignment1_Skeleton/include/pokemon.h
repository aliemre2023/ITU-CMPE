//--- 2023-2024 Spring BLG 252E Object Oriented Programing Assignment 1 ---//
//--------------------------//
///---Name & Surname: Ali Emre Kaya
//---Student Number: 150210097
//--------------------------//

#ifndef _H
#define _H

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

//-------------Do Not Add Any New Class(es)-------------//

class pokemon{
    //-------------Do Not Change Given Variables-------------//
    //-------------You Can Add New Variables If Neccessary-------------//
    private:
        string name;
        int hpValue;
        int atkValue;
    public:
        //pokemon(string name, int hpValue, int atkValue) : name(name), hpValue(hpValue), atkValue(atkValue) {}
        pokemon() : name(""), hpValue(100), atkValue(10) {} // default constructor
        pokemon(const std::string& name, int atk) : name(name), hpValue(100), atkValue(atk) {}
        pokemon(const pokemon& other) : name(other.name), hpValue(other.hpValue), atkValue(other.atkValue) {}

        string get_name() const {  // Add const here
            return name;
        }
        int get_hpValue(){
            return hpValue;
        }
        int get_atkValue(){
            return atkValue;
        }
        void set_name(string name){
            this->name = name;
        }
        void set_atkValue(int atk){
            this->atkValue = atk;
        }
        void set_hpValue(int hp){
            this->hpValue = hp;
        }
};


class pokedex {
private:
    pokemon pokedexArray[100];
    int value;

public:
    pokedex() : value(0) {}

    void updatePokedex(const pokemon& new_pokemon) {  // Add const here
        bool duplicate = false;

        for (int i = 0; i < value; i++) {
            if (pokedexArray[i].get_name() == new_pokemon.get_name()) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate && value < 100) {
            pokedexArray[value] = new_pokemon;
            //cout << new_pokemon.get_name() << " is added to the pokedex.\n";
            cout << "You catch " << new_pokemon.get_name() << "\n";
            value++;
        } else if (duplicate) {
            cout << new_pokemon.get_name() << " is already exist.\n";
        } else {
            cout << "Pokedex is full. Cannot add more Pokemon.\n";
        }
    }
    void printMyPokedex() {
        //cout << value << "\n";
        cout << "Pokedex:\n" ;
        for (int i = 0; i < value; i++) {
            cout << pokedexArray[i].get_name() << "\n";
        }
    }

};

class player{
    private:
    //-------------Do Not Change Given Variables-------------//
    //-------------You Can Add New Variables If Neccessary-------------//
        string name;
        int pokemonNumber;
        int pokeballNumber;
        int badgeNumber;
        pokemon playerPokemon;
    public:
        pokedex playerPokedex;
        
        player() : name(""), pokemonNumber(0), pokeballNumber(13), badgeNumber(0), playerPokedex() {}
        player(string name, int pokemonNumber, int pokeballNumber, int badgeNumber, pokedex playerPokedex) : name(name), pokemonNumber(pokemonNumber), pokeballNumber(pokeballNumber), badgeNumber(badgeNumber), playerPokedex(playerPokedex) {}
        // 2. constructor
        //player() : name(""), pokemonNumber(0), pokeballNumber(0), badgeNumber(0), playerPokedex(0) {}

        int showPokemonNumber() const { 
            return pokemonNumber;
        }

        int showPokeballNumber(){
            return pokeballNumber;
        }
        int showBadgeNumber(){
            return badgeNumber;
        }
        pokemon getPokemon(){
            return playerPokemon;
        }
        void battleWon(){
            //this->badgeNumber += 1; maybe
            badgeNumber += 1;
            pokeballNumber += 3;
        }
        void catchThePokemon(){
            pokemonNumber += 1;
            pokeballNumber -= 1;
        }
        
        void set_name(string player_name){
            this->name = player_name;
        }

        void printPokedex(){
            this->playerPokedex.printMyPokedex();
        }
        int plyr_atk(){
            return this->playerPokemon.get_atkValue();
        }
        int plyr_hp(){
            return this->playerPokemon.get_hpValue();
        }  
        void set_plyr_hp(int new_hp){
            this->playerPokemon.set_hpValue(new_hp);
        }
        void main_pokemon(pokemon poke){
            this->playerPokemon = poke;
        }
        void set_badgeNumber(){
            this->badgeNumber++;
        }

};

class enemy{
    //-------------Do Not Change Given Variables-------------//
    //-------------You Can Add New Variables If Neccessary-------------//
    private:
        string name;
        pokemon enemyPokemon;
    public:
        enemy(string name, pokemon& enemyPokemon) : name(name), enemyPokemon(enemyPokemon) {}
        // 2. constructor

        pokemon getPokemon(){
            return enemyPokemon;
        }
        string getName(){
            return name;
        }
        int enmy_atk(){
            return this->enemyPokemon.get_atkValue();
        }
        int enmy_hp(){
            return this->enemyPokemon.get_hpValue();
        } 
        void set_enmy_hp(int new_hp){
            this->enemyPokemon.set_hpValue(new_hp);
        }
        
        
};

#endif