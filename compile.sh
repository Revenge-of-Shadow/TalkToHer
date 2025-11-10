g++ -c TextBox.cpp -o TextBox.o
g++ -c main.cpp -o main.o
g++ main.o TextBox.o -o main -lsfml-graphics -lsfml-window -lsfml-system 
./main
