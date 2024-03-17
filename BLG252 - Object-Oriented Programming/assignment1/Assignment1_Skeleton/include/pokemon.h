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
        pokemon();
        pokemon(const string&, int);
        pokemon(const pokemon&);
        string get_name() const;
        int get_hpValue() const;
        int get_atkValue() const;
        void set_name(string);
        void set_atkValue(int);
        void set_hpValue(int);
};
class pokedex {
private:
    pokemon pokedexArray[100];
    int value;
public:
    pokedex();
    void updatePokedex(const pokemon&);
    void printMyPokedex();
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
        
        player();
        player(string, pokemon); // duzelt
    
        int showPokemonNumber() const;
        int showPokeballNumber();
        int showBadgeNumber();
        pokemon& getPokemon();
        void battleWon();
        void catchThePokemon(const pokemon);
        void printPokedex();
        void main_pokemon(pokemon);
};
class enemy{
    //-------------Do Not Change Given Variables-------------//
    //-------------You Can Add New Variables If Neccessary-------------//
    private:
        string name;
        pokemon enemyPokemon;
    public:
        enemy(string, pokemon&);

        pokemon& getPokemon();
        string getName();
};

#endif 