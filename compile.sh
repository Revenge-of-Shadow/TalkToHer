g++ -c src/TextBox.cpp -o build/TextBox.o
g++ -c src/main.cpp -o build/main.o
g++ build/main.o build/TextBox.o -o main -lsfml-graphics -lsfml-window -lsfml-system 
./main
