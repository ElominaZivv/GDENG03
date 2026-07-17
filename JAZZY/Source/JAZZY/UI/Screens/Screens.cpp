#include <JAZZY/UI/Screens/Screens.h>

jazzy::Screens::Screens(const std::string& name)
    : screenName(name)
{
}


const std::string& jazzy::Screens::getName() const
{
    return screenName;
}