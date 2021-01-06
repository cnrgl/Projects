#include <iostream>
#include <string>
using namespace std;
//caeser chip

int main() {

string input;
int key=1;//default key

cin>>input;
cin>>key;
string::iterator it;
for(it=input.begin();it!=input.end();it++)
{
    cout<<static_cast<char>(*it+key);
}
    return 0;
}
