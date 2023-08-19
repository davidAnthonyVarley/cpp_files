#include <iostream>
#include <cmath>

using namespace std;

char encrypt_Letter(char letter) {
    //add 5 to ascii value in char
    short mask = 0 | letter;
    short encrypted= mask-15;
    char scd = encrypted | 0;


    return scd;
}

string encrypt_String(string word) {

    string newstring="";
    for (int i=0; i<word.length(); i++) {
        char ch = encrypt_Letter(word[i]);
        newstring+=ch;
    }
    return newstring;
}

char decrypt_Letter(char letter) {

    short mask = 0 | letter;
    short encrypted= mask+15;
    char scd = encrypted | 0;

    return scd;
}

string decrypt_String(string w) {
    string newstring="";
    for (int i=0; i<w.length(); i++) {
        char ch = decrypt_Letter(w[i]);
        newstring+=ch;
    }
    return newstring;
}

int main()
{
    string encr = encrypt_String("david");
    cout << encr << endl;
    cout << decrypt_String(encr) << endl;


    return 0;
}