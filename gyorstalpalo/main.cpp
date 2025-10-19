#include <iostream>
#include <string>
#include <cstdlib> // v�letlenszer�s�ghez
#include <ctime>   // seedhez
#include <locale.h>

using namespace std;

struct Hajo {
    int x;
    int y;
    bool eltalalt; // eltal�lva-e m�r
};

void moveShipTo(Hajo &ship, int x, int y) {
    ship.x = x;
    ship.y = y;
    cout << "A haj� �t lett helyezve ide: (" << x << ", " << y << ")\n";
}

int main() {

    setlocale(LC_ALL, "hun");

    srand(time(0));
    Hajo player1 = {rand() % 10, rand() % 10, false};
    Hajo player2 = {rand() % 10, rand() % 10, false};
    bool player1Robot = false;
    bool gameOver = false;
    int turn = 1;
    int player1Points = 0;
    int player2Points = 0;
    while (!gameOver) {
        cout << "K�vetkez� k�r: ";
        if (turn == 1) cout << "Player 1\n";
        else cout << "Player 2\n";
        int targetX, targetY;
        if ((turn == 1 && player1Robot) || (turn == 2)) {
            targetX = rand() % 10;
            targetY = rand() % 10;
            cout << "A robot l� erre: (" << targetX << ", " << targetY << ")\n";
        } else {
            cout << "Add meg a c�l X koordin�t�t (0-9): ";
            cin >> targetX;
            cout << "Add meg a c�l Y koordin�t�t (0-9): ";
            cin >> targetY;
        }
        Hajo &ellenfelHajo = (turn == 1) ? player2 : player1;
        int &ellenfelPont = (turn == 1) ? player2Points : player1Points;
        if (ellenfelHajo.x == targetX && ellenfelHajo.y == targetY && !ellenfelHajo.eltalalt) {
            cout << "Tal�lt!\n";
            ellenfelHajo.eltalalt = true;
            ellenfelPont += 100;
            cout << "Szeretn�d �thelyezni a haj�t 50 pont levon�s�val? (igen/nem): ";
            string valasz;
            cin >> valasz;
            if (valasz == "igen") {
                int newX, newY;
                cout << "Add meg az �j X koordin�t�t: ";
                cin >> newX;
                cout << "Add meg az �j Y koordin�t�t: ";
                cin >> newY;
                moveShipTo(ellenfelHajo, newX, newY);
                ellenfelPont -= 50;
                if (ellenfelPont < 0) ellenfelPont = 0;
            }
        } else {
            cout << "Nem tal�lt!\n";
        }
        if (player1Points >= 500) {
            cout << "Player 1 nyert!\n";
            gameOver = true;
        } else if (player2Points >= 500) {
            cout << "Player 2 nyert!\n";
            gameOver = true;
        }
        turn = (turn == 1) ? 2 : 1;
    }
    return 0;
}
