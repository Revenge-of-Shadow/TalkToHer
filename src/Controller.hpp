#include "Extras.hpp"
#include "Settings.hpp"
#include "Scenario.hpp"
#include "Textbox.hpp"
#include "defaults.hpp"

enum class State {Menu, Save, Load, Script, Options, Scenarios, Settings, Extras};

class Controller{
    void initMenu();
    void initSave();
    void initLoad();
    void initScenarios();
    void initSettings();

public:
    sf::RenderWindow& window;
    sf::Sprite background;
    Textbox textbox;
    Shortlist<std::string> commandQueue; // FIFO filled during scenario.
    Scenario scenario;
    Shortlist<Option> options; // Filled with scenarios before showcase.
    int optionIndex;
    Shortlist<Textbox> optionboxes; // Filled just before showcase; drawn
    State state;

    Settings settings;
    Extras extras;

    sf::Vector2f actualCenter();// I hate it.
    void performResize();//  It gets WORSE.

    bool saveSettings();
    bool loadSettings();
    bool saveState(std::string filename);
    bool loadState(std::string filename);

    void loadScenario(std::string scenario_path);
    void loadScenarioPaths();
    void listOptions();

    Controller(sf::RenderWindow &w);

    void tryPrevLine();
    void tryNextLine();
    void tryCurrLine();

    bool processKey(sf::Event);
    void draw();
    void mainloop();
};


