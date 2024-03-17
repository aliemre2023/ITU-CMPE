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

#include "pokemon.h"

using namespace std;

//-------------You Can Add Your Functions Down Below-------------//
//////////////////////////////////
// class pokemon
pokemon::pokemon() : name(""), hpValue(100), atkValue(10) {}
pokemon::pokemon(const string& name, int atk) : name(name), hpValue(100), atkValue(atk) {}
pokemon::pokemon(const pokemon& other) : name(other.name), hpValue(other.hpValue), atkValue(other.atkValue) {}
string pokemon::get_name() const {  
    return name;
}
int pokemon::get_hpValue() const{
    return hpValue;
}
int pokemon::get_atkValue() const{
    return atkValue;
}
void pokemon::set_name(string name){
    this->name = name;
}
void pokemon::set_atkValue(int atk){
    this->atkValue = atk;
}
void pokemon::set_hpValue(int hp){
    this->hpValue = hp;
}

//////////////////////////////////
// class pokedex
pokedex::pokedex() : value(0) {}
void pokedex::updatePokedex(const pokemon& new_pokemon) {  // Add const here
    bool duplicate = false;
    for (int i = 0; i < value; i++) {
        if (pokedexArray[i].get_name() == new_pokemon.get_name()) {
            duplicate = true;
            break;
        }
    }
    if(!duplicate && value < 100){
        pokedexArray[value] = new_pokemon;
        value++;
    } 
    else if(duplicate){
        cout << new_pokemon.get_name() << " is already exist.\n";
    } 
    else{
        cout << "Pokedex is full. Cannot add more Pokemon.\n";
    }
}
void pokedex::printMyPokedex() {
    cout << "------- Pokedex -------\n" ;
    for (int i = 0; i < value; i++) {
        cout << pokedexArray[i].get_name() << "\n";
    }
}
//////////////////////////////////
//////////////////////////////////
// class player     
player::player() : name(""), pokemonNumber(0), pokeballNumber(10), badgeNumber(0), playerPokedex() {}
player::player(string name, pokemon playerPokemon) : name(name), pokemonNumber(0), pokeballNumber(10), badgeNumber(0), playerPokemon(playerPokemon) {}
int player::showPokemonNumber() const { 
    return pokemonNumber;
}
int player::showPokeballNumber(){
    return pokeballNumber;
}
int player::showBadgeNumber(){
    return badgeNumber;
}
pokemon& player::getPokemon(){
    return this->playerPokemon;
}
void player::battleWon(){
    cout << "You Won!" << "\n";
    badgeNumber += 1;
    pokeballNumber += 3;
}
void player::catchThePokemon(const pokemon new_pokemon){
    cout << "You catch " << new_pokemon.get_name() << "\n";
    pokemonNumber += 1;
    pokeballNumber -= 1;
}
void player::printPokedex(){
    this->playerPokedex.printMyPokedex();
}
void player::main_pokemon(pokemon poke){
    this->playerPokemon = poke;
}
//////////////////////////////////
//////////////////////////////////
// class enemy
enemy::enemy(string name, pokemon& enemyPokemon) : name(name), enemyPokemon(enemyPokemon) {}
pokemon& enemy::getPokemon(){
    return this->enemyPokemon;
}
string enemy::getName(){
    return name;
}
//////////////////////////////////