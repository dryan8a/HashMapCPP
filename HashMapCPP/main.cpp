#include <iostream>
#include "HashMap.h"

int main()
{
    HashMap<std::string, int> map{5,0.75};
    map.Add("Richard", 69);
    map.Add("Snoop Dogg", 420);
    map.Add("Ivanna Humpalot", 42);
    map.Add("Mike Hunt", 42);
    map.Add("Ho Lee Fuk", 42);
    map.Add("Sum Ting Wong", 42);
    map.Add("Wi Tu Lo", 42);
    map.Remove("Richard");
    std::cout << map.Contains("Richard") << map.Contains("Mike Hunt") << std::endl;
    auto SnoopDogg = map.Find("Snoop Dogg");
    return 0;
}