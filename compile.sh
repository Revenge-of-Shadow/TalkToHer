g++ -c src/TextBox.cpp -o build/TextBox.o
# g++ -c src/Sound.cpp -o build/Sound.o
# g++ -c src/DrawableObject.cpp -o build/DrawableObject.o
g++ -c src/Scenario.cpp -o build/Scenario.o
g++ -c src/main.cpp -o build/main.o
# build/Sound.o build/DrawableObject.o 
g++ build/main.o build/TextBox.o build/Scenario.o -o main -lsfml-graphics -lsfml-window -lsfml-system 
./main
