#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

//What I included
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
    : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
    bool /* shotHit */, bool /* shipDestroyed */,
    int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

HumanPlayer::HumanPlayer(string nm, const Game& g) :Player(nm, g), m_lastCellAttacked(0, 0) {}

bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board& p)
{
    int numOfAvailableSpaces = game().rows() * game().cols();
    cout << name() << " the Human must place " << game().nShips() << " ships." << endl;
    p.display(false);
    for (int i = 0; i < game().nShips(); i++)
    {
        char direction = 'n';
        while ((direction != 'h') && (direction != 'v'))
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            cin >> direction;
            if ((direction != 'h') && (direction != 'v'))
            {
                cout << "Direction must be h or v" << endl;
            }
        }
        Point coordinate(-1, -1);
        if (direction == 'h')
        {
            while ((coordinate.r < 0) || (coordinate.c < 0) || (coordinate.r >= game().rows()) || (coordinate.c >= game().cols()))
            {
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                int row = -1;
                int col = -1;
                bool result = getLineWithTwoIntegers(row, col);
                if (result)
                {
                    coordinate.r = row;
                    coordinate.c = col;
                    if ((row < 0) || (col < 0) || (row >= game().rows()) || (col >= game().cols()))
                    {
                        coordinate.r = -1;
                        coordinate.c = -1;
                        cout << "The ship can not be placed there" << endl;
                    }
                    else if (p.placeShip(coordinate, i, HORIZONTAL) == false)
                    {
                        numOfAvailableSpaces--;
                        coordinate.r = -1;
                        coordinate.c = -1;
                        cout << "The ship can not be placed there" << endl;
                    }
                    else
                    {
                        numOfAvailableSpaces--;
                        p.display(false);
                    }
                    if (numOfAvailableSpaces <= 0)
                    {
                        return false;
                    }
                }
                else
                {
                    cout << "You must enter two integers" << endl;
                }
            }
        }
        else
        {
            while ((coordinate.r < 0) || (coordinate.c < 0) || (coordinate.r >= game().rows()) || (coordinate.c >= game().cols()))
            {
                cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                int row = -1;
                int col = -1;
                bool result = getLineWithTwoIntegers(row, col);
                if (result)
                {
                    coordinate.r = row;
                    coordinate.c = col;
                    if ((row < 0) || (col < 0) || (row >= game().rows()) || (col >= game().cols()))
                    {
                        coordinate.r = -1;
                        coordinate.c = -1;
                        cout << "The ship can not be placed there" << endl;
                    }
                    else if (p.placeShip(coordinate, i, VERTICAL) == false)
                    {
                        numOfAvailableSpaces--;
                        coordinate.r = -1;
                        coordinate.c = -1;
                        cout << "The ship can not be placed there" << endl;
                    }
                    else
                    {
                        numOfAvailableSpaces--;
                        p.display(false);
                    }
                    if (numOfAvailableSpaces <= 0)
                    {
                        return false;
                    }
                }
                else
                {
                    cout << "You must enter two integers" << endl;
                }
            }
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    Point coordinate(-1, -1);
    int row = -1;
    int col = -1;
    while (getLineWithTwoIntegers(row, col) == false)
    {
        cout << "You must enter two integers" << endl;
        row = -1;
        col = -1;
        cout << "Enter the row and column to attack (e.g., 3 5): ";
    }
    coordinate.r = row;
    coordinate.c = col;
    return coordinate;
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
    bool /* shotHit */, bool /* shipDestroyed */,
    int /* shipId */)
{
    // HumanPlayer completely ignores the result of any attack
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
    // HumanPlayer completely ignores what the opponent does
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    //what I added
    bool placeShipsHelper(Board& b, int shipId, int rows, int cols);
    bool findPreviousAttack(Point p);

private:
    int state;
    Point m_lastCellAttacked;
    vector<Point> previousAttacks;
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
    : Player(nm, g), state(1), m_lastCellAttacked(-1, -1)
{}

bool MediocrePlayer::placeShipsHelper(Board& b, int shipId, int rows, int cols)
{
    //all ships have been placed
    if (shipId < 0)
    {
        return true;
    }
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            Point p(r, c);

            if (b.placeShip(p, shipId, HORIZONTAL))
            {
                if (placeShipsHelper(b, shipId - 1, rows, cols))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(p, shipId, HORIZONTAL);
                }
            }
            if (b.placeShip(p, shipId, VERTICAL))
            {
                if (placeShipsHelper(b, shipId - 1, rows, cols))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(p, shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

bool MediocrePlayer::placeShips(Board& b)
{
    int shipId = game().nShips() - 1;
    int rows = game().rows();
    int cols = game().cols();
    for (int i = 0; i < 50; i++)
    {
        b.block();
        if (placeShipsHelper(b, shipId, rows, cols))
        {
            b.unblock();
            return true;
        }
        b.unblock();
    }
    b.unblock();
    return false;
}

bool MediocrePlayer::findPreviousAttack(Point p)
{
    for (size_t i = 0; i < previousAttacks.size(); i++)
    {
        if ((previousAttacks[i].r == p.r && previousAttacks[i].c == p.c) || (game().isValid(p) == false))
        {
            return true;
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    if (state == 1)
    {
        int row = randInt(game().rows());
        int col = randInt(game().cols());
        Point p(row, col);
        while (findPreviousAttack(p))
        {
            row = randInt(game().rows());
            col = randInt(game().cols());
            p.r = row;
            p.c = col;
        }
        previousAttacks.push_back(p);
        return p;
    }
    else
    {
        //randomly selects a point within a 4-col radius from the point
        int row = m_lastCellAttacked.r;
        int col = m_lastCellAttacked.c - 4 + rand() % 9;
        Point p(row, col);
        int counter = 0;
        while (findPreviousAttack(p))
        {
            row = m_lastCellAttacked.r;
            col = m_lastCellAttacked.c - 4 + rand() % 9;
            p.r = row;
            p.c = col;
            counter++;
            if (counter >= 100)
            {
                break;
            }
        }
        if (counter < 100)
        {
            previousAttacks.push_back(p);
            return p;
        }
        //randomly selects a point within a 4-row radius from the point
        int row2 = m_lastCellAttacked.r - 4 + rand() % 9;
        int col2 = m_lastCellAttacked.c;
        Point p2(row2, col2);
        while (findPreviousAttack(p2))
        {
            row2 = m_lastCellAttacked.r - 4 + rand() % 9;
            col2 = m_lastCellAttacked.c;
            p2.r = row2;
            p2.c = col2;
        }
        previousAttacks.push_back(p2);
        return p2;
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{
    if (state == 1)
    {
        m_lastCellAttacked.r = p.r;
        m_lastCellAttacked.c = p.c;
        if (!shotHit && validShot && (shipId < 50))
        {
            state = 1;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            state = 2;
            return;
        }
    }
    else
    {
        if (!shotHit && validShot && (shipId < 50))
        {
            state = 2;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            state = 2;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            m_lastCellAttacked.r = -1;
            m_lastCellAttacked.c = -1;
            return;
        }
    }
    return;
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
    // MediocrePlayer completely ignores what the opponent does
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool findPreviousAttack(Point p);

private:
    int state;
    Point m_lastCellAttacked;
    Point centralPoint;
    vector<Point> previousAttacks;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
    : Player(nm, g), state(1), m_lastCellAttacked(-1, -1)
{}

bool GoodPlayer::placeShips(Board& b)
{
    for (int i = 0; i < game().nShips(); i++)
    {
        int dir = randInt(2);
        int row = randInt(game().rows());
        int col = randInt(game().cols());
        Point p(row, col);
        if (dir == 0)
        {
            int count = 0;
            while (!(b.placeShip(p, i, HORIZONTAL)))
            {
                row = randInt(game().rows());
                col = randInt(game().cols());
                p.r = row;
                p.c = col;
                count++;
                if (count > 100)
                {
                    return false;
                }
            }
        }
        else
        {
            int count = 0;
            while (!(b.placeShip(p, i, VERTICAL)))
            {
                row = randInt(game().rows());
                col = randInt(game().cols());
                p.r = row;
                p.c = col;
                count++;
                if (count > 100)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool GoodPlayer::findPreviousAttack(Point p)
{
    for (size_t i = 0; i < previousAttacks.size(); i++)
    {
        if ((previousAttacks[i].r == p.r && previousAttacks[i].c == p.c) || (game().isValid(p) == false))
        {
            return true;
        }
    }
    return false;
}

Point GoodPlayer::recommendAttack()
{
    if (state == 1)
    {
        //checkerboard pattern
        int row = randInt(game().rows());
        int col;
        if (row % 2 == 0) {
            col = (2 * rand()) % (game().rows());
        }
        else
        {
            col = 1 + (2 * rand()) % 10;
        }
        Point p(row, col);
        while (findPreviousAttack(p))
        {
            row = randInt(game().rows());
            if (row % 2 == 0) {
                col = (2 * rand()) % (game().rows());
            }
            else
            {
                col = 1 + (2 * rand()) % 10;
            }
            p.r = row;
            p.c = col;
        }
        previousAttacks.push_back(p);
        return p;
    }
    /*
        Plan: Have four state representing the four cardinal directions
        - initially, we attack east by one unit
        - if we miss east, we target west,
        - if we hit east, we continue east
        - same for all cardinal directions
    */
    else if (state == 2)
    {
        int row = m_lastCellAttacked.r;
        int col = m_lastCellAttacked.c + 1;
        Point p(row, col);
        if (findPreviousAttack(p)) {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            state = 3;
            return recommendAttack();
        }
        else {
            previousAttacks.push_back(p);
            return p;
        }
    }
    else if (state == 3) {
        int row = m_lastCellAttacked.r;
        int col = m_lastCellAttacked.c - 1;
        Point p(row, col);
        if (findPreviousAttack(p)) {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            state = 4;
            return recommendAttack();
        }
        else {
            previousAttacks.push_back(p);
            return p;
        }
    }
    else if (state == 4) {
        int row = m_lastCellAttacked.r - 1;
        int col = m_lastCellAttacked.c;
        Point p(row, col);
        if (findPreviousAttack(p)) {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            state = 5;
            return recommendAttack();
        }
        else {
            previousAttacks.push_back(p);
            return p;
        }
    }
    else if (state == 5) {
        int row = m_lastCellAttacked.r + 1;
        int col = m_lastCellAttacked.c;
        Point p(row, col);
        if (findPreviousAttack(p)) {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            state = 1;
            return recommendAttack();
        }
        else {
            previousAttacks.push_back(p);
            return p;
        }
    }
        /*
        //randomly selects a point within a 4-col radius from the point
        int row = m_lastCellAttacked.r;
        int col = m_lastCellAttacked.c - 4 + rand() % 9;
        Point p(row, col);
        int counter = 0;
        while (findPreviousAttack(p))
        {
            row = m_lastCellAttacked.r;
            col = m_lastCellAttacked.c - 4 + rand() % 9;
            p.r = row;
            p.c = col;
            counter++;
            if (counter >= 100)
            {
                break;
            }
        }
        if (counter < 100)
        {
            previousAttacks.push_back(p);
            return p;
        }
        //randomly selects a point within a 4-row radius from the point
        int row2 = m_lastCellAttacked.r - 4 + rand() % 9;
        int col2 = m_lastCellAttacked.c;
        Point p2(row2, col2);
        while (findPreviousAttack(p2))
        {
            row2 = m_lastCellAttacked.r - 4 + rand() % 9;
            col2 = m_lastCellAttacked.c;
            p2.r = row2;
            p2.c = col2;
        }
        previousAttacks.push_back(p2);
        return p2;
    }
    */
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{

    if (state == 1)
    {
        m_lastCellAttacked.r = p.r;
        m_lastCellAttacked.c = p.c;
        if (!shotHit && validShot && (shipId < 50))
        {
            state = 1;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            centralPoint.r = p.r;
            centralPoint.c = p.c;
            state = 2;
            return;
        }
    }
    //state 2 = attack EAST
    else if (state == 2)
    {
        if (!shotHit && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            //target not found in east, so attack west
            state = 3;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = p.r;
            m_lastCellAttacked.c = p.c;
            //target found in east, continue attacking east
            state = 2;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            m_lastCellAttacked.r = -1;
            m_lastCellAttacked.c = -1;
            return;
        }
    }
    //state 3 = attack WEST
    else if (state == 3) {
        if (!shotHit && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            //target not found in west, so attack north
            state = 4;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = p.r;
            m_lastCellAttacked.c = p.c;
            //target found in west, continue attacking west
            state = 3;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            m_lastCellAttacked.r = -1;
            m_lastCellAttacked.c = -1;
            return;
        }
    }
    //state 4 = attack NORTH
    else if (state == 4) {
        if (!shotHit && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            //target not found in north, so attack south
            state = 5;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = p.r;
            m_lastCellAttacked.c = p.c;
            //target found in north, continue attacking north
            state = 4;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            m_lastCellAttacked.r = -1;
            m_lastCellAttacked.c = -1;
            return;
        }
    }
    //state 5 = attack SOUTH
    else if (state == 5) {
        if (!shotHit && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = centralPoint.r;
            m_lastCellAttacked.c = centralPoint.c;
            //target not found in south, resume checkerboard approach
            state = 1;
            return;
        }
        if (shotHit && !shipDestroyed && validShot && (shipId < 50))
        {
            m_lastCellAttacked.r = p.r;
            m_lastCellAttacked.c = p.c;
            //target found in south, so stay attacking south
            state = 5;
            return;
        }
        if (shotHit && shipDestroyed && validShot && (shipId < 50))
        {
            state = 1;
            m_lastCellAttacked.r = -1;
            m_lastCellAttacked.c = -1;
            return;
        }
    }
    return;
}

void GoodPlayer::recordAttackByOpponent(Point /* p */)
{
    // GoodPlayer completely ignores what the opponent does
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types) / sizeof(types[0]) &&
        type != types[pos]; pos++)
        ;
    switch (pos)
    {
    case 0:  return new HumanPlayer(nm, g);
    case 1:  return new AwfulPlayer(nm, g);
    case 2:  return new MediocrePlayer(nm, g);
    case 3:  return new GoodPlayer(nm, g);
    default: return nullptr;
    }
}