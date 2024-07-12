#include <iostream>
#include <memory>
#include <string>

class peer
{
private:
    static std::unique_ptr<peer> instance;
    std::string color;

public:
    peer(const std::string &customColor) : color(customColor)
    {
        if (!instance)
        {
            instance = std::unique_ptr<peer>(new peer(customColor, true));
        }
    }

    static void logColor()
    {
        if (!instance)
        {
            std::cout << "ERROR: Please create an instance first" << std::endl;
            return;
        }

        logColor(*instance);
    }
    static void logColor(const peer &node)
    {
        std::cout << "Provided peer instance color: " << node.color << std::endl;
    }

    std::string getColor() const { return color; }

private:
    peer(const std::string &customColor, bool) : color(customColor) {}
};

std::unique_ptr<peer> peer::instance = nullptr;

int main()
{
    peer::logColor();

    peer("red");
    peer blue("blue");

    peer::logColor();
    peer::logColor(blue);

    return 0;
}
