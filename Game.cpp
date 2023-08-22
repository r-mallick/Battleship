#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

//What I included
#include <vector>

using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);

private:
    int m_rows;
    int m_cols;
    struct ship
    {
        int m_length;
        char m_symbol;
        string m_name;
        int m_shipID;
    };
    vector<ship> listOfShips;
    int numOfShips;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    numOfShips = 0;
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    ship newShip;
    newShip.m_length = length;
    newShip.m_symbol = symbol;
    newShip.m_name = name;
    newShip.m_shipID = numOfShips;
    listOfShips.push_back(newShip);
    numOfShips++;
    return true;
}

int GameImpl::nShips() const
{
    return numOfShips;
}

int GameImpl::shipLength(int shipId) const
{
    return listOfShips[shipId].m_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return listOfShips[shipId].m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return listOfShips[shipId].m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    //places ships on both boards
    b1.clear();
    b2.clear();
    if (p1->placeShips(b1) == false)
    {
        return nullptr;
    }
    if (p2->placeShips(b2) == false)
    {
        return nullptr;
    }
    //if pauses should be made
    if (shouldPause)
    {
        while ((b1.allShipsDestroyed() == false) && (b2.allShipsDestroyed() == false))
        {
            //player 1's turn
            cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
            if (p1->isHuman())
            {
                b2.display(true);
            }
            else
            {
                b2.display(false);
            }
            Point p1Attack = p1->recommendAttack();
            bool shotHit1 = false;
            bool shipDestroyed1 = false;
            int shipId1 = -1;
            bool validShot1 = b2.attack(p1Attack, shotHit1, shipDestroyed1, shipId1);
            p1->recordAttackResult(p1Attack, validShot1, shotHit1, shipDestroyed1, shipId1);
            if (shotHit1 && shipDestroyed1)
            {
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and destroyed the " << p2->game().shipName(shipId1) << ", resulting in: " << endl;
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            else if (shotHit1)
            {
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and hit something, resulting in:" << endl;
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            else if (p1->isHuman() && !validShot1)
            {
                cout << p1->name() << " wasted a shot at (" << p1Attack.r << "," << p1Attack.c << ")." << endl;
            }
            else
            {
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and missed, resulting in:" << endl;
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            if (b2.allShipsDestroyed())
            {
                cout << p1->name() << " wins!" << endl;
                if (p2->isHuman())
                {
                    cout << "Here is where " << p1->name() << "'s ships were " << endl;
                    b1.display(false);
                }
                return p1;
            }
            cout << "Press enter to continue:";
            cin.ignore(10000, '\n');

            //player 2's turn
            cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
            if (p2->isHuman())
            {
                b1.display(true);
            }
            else
            {
                b1.display(false);
            }
            Point p2Attack = p2->recommendAttack();
            bool shotHit2 = false;
            bool shipDestroyed2 = false;
            int shipId2 = -1;
            bool validShot2 = b1.attack(p2Attack, shotHit2, shipDestroyed2, shipId2);
            p2->recordAttackResult(p2Attack, validShot2, shotHit2, shipDestroyed2, shipId2);
            if (shotHit2 && shipDestroyed2)
            {
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and destroyed the " << p1->game().shipName(shipId2) << ", resulting in: " << endl;
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            else if (shotHit2)
            {
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and hit something, resulting in:" << endl;
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            else if (p2->isHuman() && !validShot1)
            {
                cout << p2->name() << " wasted a shot at (" << p1Attack.r << "," << p1Attack.c << ")." << endl;
            }
            else
            {
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and missed, resulting in:" << endl;
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            if (b1.allShipsDestroyed())
            {
                cout << p2->name() << " wins!" << endl;
                if (p1->isHuman())
                {
                    cout << "Here is where " << p2->name() << "'s ships were " << endl;
                    b2.display(false);
                }
                return p2;
            }
            cout << "Press enter to continue:";
            cin.ignore(10000, '\n');
        }
    }
    //if pauses should not be made
    else
    {
        while ((b1.allShipsDestroyed() == false) || (b2.allShipsDestroyed() == false))
        {
            //player 1's turn
            cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
            if (p1->isHuman())
            {
                b2.display(true);
            }
            else
            {
                b2.display(false);
            }
            Point p1Attack = p1->recommendAttack();
            bool shotHit1 = false;
            bool shipDestroyed1 = false;
            int shipId1 = -1;
            bool validShot1 = b2.attack(p1Attack, shotHit1, shipDestroyed1, shipId1);
            p1->recordAttackResult(p1Attack, validShot1, shotHit1, shipDestroyed1, shipId1);
            if (shotHit1 && shipDestroyed1)
            {
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and destroyed the " << p2->game().shipName(shipId1) << ", resulting in: " << endl;
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            else if (shotHit1)
            {
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and hit something, resulting in:" << endl;
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            else if (p1->isHuman() && !validShot1)
            {
                cout << p1->name() << " wasted a shot at (" << p1Attack.r << "," << p1Attack.c << ")." << endl;
            }
            else
            {
                cout << p1->name() << " attacked (" << p1Attack.r << "," << p1Attack.c << ") and missed, resulting in:" << endl;
                if (p1->isHuman())
                {
                    b2.display(true);
                }
                else
                {
                    b2.display(false);
                }
            }
            if (b2.allShipsDestroyed())
            {
                cout << p1->name() << " wins!" << endl;
                if (p2->isHuman())
                {
                    cout << "Here is where " << p1->name() << "'s ships were " << endl;
                    b1.display(false);
                }
                return p1;
            }
            //cout << "Press enter to continue:" << endl;

            //player 2's turn
            cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
            if (p2->isHuman())
            {
                b1.display(true);
            }
            else
            {
                b1.display(false);
            }
            Point p2Attack = p2->recommendAttack();
            bool shotHit2 = false;
            bool shipDestroyed2 = false;
            int shipId2 = -1;
            bool validShot2 = b1.attack(p2Attack, shotHit2, shipDestroyed2, shipId2);
            p2->recordAttackResult(p2Attack, validShot2, shotHit2, shipDestroyed2, shipId2);
            if (shotHit2 && shipDestroyed2)
            {
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and destroyed the " << p1->game().shipName(shipId2) << ", resulting in: " << endl;
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            else if (shotHit2)
            {
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and hit something, resulting in:" << endl;
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            else if (p2->isHuman() && !validShot1)
            {
                cout << p2->name() << " wasted a shot at (" << p1Attack.r << "," << p1Attack.c << ")." << endl;
            }
            else
            {
                cout << p2->name() << " attacked (" << p2Attack.r << "," << p2Attack.c << ") and missed, resulting in:" << endl;
                if (p2->isHuman())
                {
                    b1.display(true);
                }
                else
                {
                    b1.display(false);
                }
            }
            if (b1.allShipsDestroyed())
            {
                cout << p2->name() << " wins!" << endl;
                if (p1->isHuman())
                {
                    cout << "Here is where " << p2->name() << "'s ships were " << endl;
                    b2.display(false);
                }
                return p2;
            }
            //cout << "Press enter to continue:" << endl;
        }
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1 || nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1 || nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows() && length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
            << endl;
        return false;
    }
    if (!isascii(symbol) || !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
            << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X' || symbol == '.' || symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
            << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr || p2 == nullptr || nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

