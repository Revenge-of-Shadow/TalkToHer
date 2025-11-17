g++ -c src/TextBox.cpp -o build/TextBox.o
# g++ -c src/Sound.cpp -o build/Sound.o
# g++ -c src/Character.cpp -o build/Character.o
g++ -c src/Scenario.cpp -o build/Scenario.o
g++ -c src/main.cpp -o build/main.o
# build/Sound.o build/Character.o 
g++ build/main.o build/TextBox.o build/Scenario.o -o main -lsfml-graphics -lsfml-window -lsfml-system 
./main
