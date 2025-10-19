#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <locale.h>
#include <vector>
#include <cmath>

using namespace std;

struct Hajo {
    int x;
    int y;
    bool eltalalt; // már eltalálták-e
};

struct Coordinate {
    int x, y;
};

void moveShipTo(Hajo &ship, int x, int y) {
    ship.x = x;
    ship.y = y;
    cout << "A hajó át lett helyezve ide: (" << x << ", " << y << ")\n";
}

// Számolja az euclidean távolságot két pont között
double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

// Robot lövés: okos célzás az előző találatok közelébe
Coordinate robotTarget(const vector<vector<bool>> &shotGrid) {
    int bestX = rand()%10;
    int bestY = rand()%10;
    // Próbáljuk elkerülni a már meglőtt mezőket
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(!shotGrid[i][j]){
                bestX = i;
                bestY = j;
                return {bestX,bestY};
            }
        }
    }
    return {bestX,bestY};
}

// Robot hajó áthelyezése: a találati ponttól legtávolabb eső üres mező
Coordinate robotMove(const Hajo &ship, int hitX, int hitY) {
    double maxDist = -1;
    Coordinate best = {ship.x, ship.y};
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            double d = distance(hitX, hitY, i, j);
            if(d > maxDist){
                maxDist = d;
                best = {i,j};
            }
        }
    }
    return best;
}

int main() {
    setlocale(LC_ALL,"");

    srand(time(0));
    Hajo player = {rand()%10, rand()%10, false};
    Hajo robot = {rand()%10, rand()%10, false};
    int playerPoints = 0;
    int robotPoints = 0;
    bool gameOver = false;
    int turn = 1; // 1=player, 2=robot

    vector<vector<bool>> playerShots(10, vector<bool>(10,false));
    vector<vector<bool>> robotShots(10, vector<bool>(10,false));

    while(!gameOver){
        cout << "-----------------------------------\n";
        if(turn==1) cout << "Te következel!\n";
        else cout << "Robot következik.\n";

        int targetX, targetY;

        if(turn==1){
            cout << "Add meg a cél X koordinátát (0-9): ";
            cin >> targetX;
            cout << "Add meg a cél Y koordinátát (0-9): ";
            cin >> targetY;
        } else {
            Coordinate t = robotTarget(robotShots);
            targetX = t.x;
            targetY = t.y;
            cout << "A robot erre lő: (" << targetX << ", " << targetY << ")\n";
        }

        if(turn==1){
            playerShots[targetX][targetY] = true;
            if(robot.x==targetX && robot.y==targetY && !robot.eltalalt){
                cout << "Találtál a robot hajójára!\n";
                robot.eltalalt = true;
                playerPoints += 100;
                if(robotPoints >=50){
                    Coordinate newPos = robotMove(robot, targetX, targetY);
                    moveShipTo(robot,newPos.x,newPos.y);
                    robotPoints -= 50;
                    if(robotPoints<0) robotPoints=0;
                }
            } else {
                cout << "Nem találtál.\n";
            }
        } else {
            robotShots[targetX][targetY] = true;
            if(player.x==targetX && player.y==targetY && !player.eltalalt){
                cout << "A robot talált a hajódra!\n";
                player.eltalalt = true;
                robotPoints += 100;
                if(playerPoints>=50){
                    cout << "Szeretnéd áthelyezni a hajódat 50 pont levonással? (igen/nem): ";
                    string valasz;
                    cin >> valasz;
                    if(valasz=="igen"){
                        int newX,newY;
                        cout << "Add meg az új X koordinátát: ";
                        cin >> newX;
                        cout << "Add meg az új Y koordinátát: ";
                        cin >> newY;
                        moveShipTo(player,newX,newY);
                        playerPoints -=50;
                        if(playerPoints<0) playerPoints=0;
                    }
                }
            } else {
                cout << "A robot nem talált.\n";
            }
        }

        cout << "Pontjaid: " << playerPoints << ", Robot pontjai: " << robotPoints << "\n";

        if(playerPoints>=500){
            cout << "Nyertél! Gratulálok!\n";
            gameOver=true;
        } else if(robotPoints>=500){
            cout << "A robot nyert!\n";
            gameOver=true;
        }

        turn = (turn==1)?2:1;
    }

    return 0;
}
