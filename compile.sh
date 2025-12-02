g++ -c src/defaults.cpp -o build/defaults.o
g++ -c src/Textbox.cpp -o build/Textbox.o
# g++ -c src/Sound.cpp -o build/Sound.o
# g++ -c src/Option.cpp -o build/Option.o
# g++ -c src/DrawableObject.cpp -o build/DrawableObject.o
g++ -c src/Scenario.cpp -o build/Scenario.o

g++ -c src/main.cpp -o build/main.o
# build/Sound.o build/DrawableObject.o build/Option.o
g++ build/defaults.o build/main.o build/Textbox.o build/Scenario.o -o TalkToHer -lsfml-graphics -lsfml-window -lsfml-system 
