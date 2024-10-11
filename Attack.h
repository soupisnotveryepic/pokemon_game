#ifndef ATTACK_H
#define ATTACK_H

#include <iostream>
#include <string>

#include "Pokemon.h"

//Attack Pokemon class child class of Pokemon
class Attack : public Pokemon{
public:
    //int attack_used;
    int damage;
    int attack_type;
    //bool hit_success;
    
    int chance;

    //returns value (chance) between 1 and 100 to use for quick and type attacks
    int success_rate();

    //constructor
    Attack(); 
    //Attack(int attack_used);

    //bool hit_success(int attack_used, bool hit_success);

    //int set_numHits(int num_hits);
    //int get_numHits();

};

#endif
