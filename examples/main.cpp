#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<short int> v1 = {1, 2, 3, 6};
    vector<short int> v2 = {1, 2, 3, 5};
    if(v1 < v2) cout<<"true";
    else cout<<"false";
    return 0;
}