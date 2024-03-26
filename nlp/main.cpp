#include "header.h"

int main() {

    //TopDownParser tdp("cgram");
    //tdp.parse("the man hit the dog");

    BottomUpParser bup("cgram");
    bup.parse_input("the man hit the dog");
   
}