g++ -c src/defaults.cpp -o build/defaults.o -I./SFML-2.6.2/include
g++ -c src/Textbox.cpp -o build/Textbox.o -I./SFML-2.6.2/include
g++ -c src/Sound.cpp -o build/Sound.o -I./SFML-2.6.2/include
# g++ -c src/Option.cpp -o build/Option.o
# g++ -c src/DrawableObject.cpp -o build/DrawableObject.o
g++ -c src/Scenario.cpp -o build/Scenario.o -I./SFML-2.6.2/include
g++ -c src/Controller.cpp -o build/Controller.o -I./SFML-2.6.2/include
g++ -c src/Settings.cpp -o build/Settings.o -I./SFML-2.6.2/include
g++ -c src/Extras.cpp -o build/Extras.o -I./SFML-2.6.2/include

g++ -c src/main.cpp -o build/main.o -I./SFML-2.6.2/include
# build/Sound.o build/DrawableObject.o build/Option.o
g++ build/defaults.o build/main.o build/Textbox.o build/Sound.o build/Scenario.o build/Controller.o build/Settings.o build/Extras.o -o TalkToHer  -L./SFML-2.6.2/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system 

