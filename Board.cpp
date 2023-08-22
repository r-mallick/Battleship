#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

//What I included
#include <vector>
#include <algorithm>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

private:
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    const Game& m_game;
    char grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
    vector<int> listOfShipIds;
};

BoardImpl::BoardImpl(const Game& g)
    : m_game(g)
{
    // This compiles, but may not be correct
    m_rows = m_game.rows();
    m_cols = m_game.cols();
}

void BoardImpl::clear()
{
    // This compiles, but may not be correct
    // All dots basically 
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            grid[i][j] = '.';
        }
    }
}

void BoardImpl::block()
{
    // Block cells with 50% probability
    randInt(2);
    int spots = (m_rows * m_cols) / 2;
    do {
        int row = randInt(m_rows);
        int col = randInt(m_cols);
        if (grid[row][col] == '#')
        {
            grid[row][col] = '#';
        }
        else
        {
            grid[row][col] = '#';
            spots--;
        }
    } while (spots != 0);
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (grid[r][c] == '#')
            {
                grid[r][c] = '.';
            }
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    //check if point is out of bounds
    if ((topOrLeft.r >= m_rows) || (topOrLeft.c >= m_cols) || (topOrLeft.r < 0) || (topOrLeft.c < 0))
    {
        return false;
    }
    //if the shipId is invalid
    if (shipId < 0 || shipId >= m_game.nShips())
    {
        return false;
    }
    //check if ship would be partly or fully outside the board (HORIZONTALLY)
    if (dir == HORIZONTAL)
    {
        if ((topOrLeft.c + m_game.shipLength(shipId)) > m_cols)
        {
            return false;
        }
    }
    //check if ship would be partly or fully outside the board (VERTICALLY)
    if (dir == VERTICAL)
    {
        if ((topOrLeft.r + m_game.shipLength(shipId)) > m_rows)
        {
            return false;
        }
    }
    //check if the ship would overlap an already-placed ship or blocked by block function (HORIZONTALLY)
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (grid[topOrLeft.r][topOrLeft.c + i] != '.')
            {
                return false;
            }
        }
    }
    //check if the ship would overlap an already-placed ship or blocked by block function (VERTICALLY)
    if (dir == VERTICAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (grid[topOrLeft.r + i][topOrLeft.c] != '.')
            {
                return false;
            }
        }
    }
    //check if the ship with that shipId has been placed before 
    if (find(listOfShipIds.begin(), listOfShipIds.end(), shipId) != listOfShipIds.end())
    {
        return false;
    }
    //conditions met, now can place the ship (HORIZONTALLY)
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            grid[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
        }
        listOfShipIds.push_back(shipId);
        return true;
    }
    //conditions met, now can place the ship (VERTICALLY)
    if (dir == VERTICAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            grid[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
        listOfShipIds.push_back(shipId);
        return true;
    }
    return false;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    //if the shipId is invalid
    if (shipId < 0 || shipId >= m_game.nShips())
    {
        return false;
    }
    //if the board does not contain the entire ship at the indicated locations (HORIZONTALLY)
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (grid[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
    }
    //if the board does not contain the entire ship at the indicated locations (VERTICALLY)
    if (dir == VERTICAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (grid[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
    }
    //if conditions are met, unplace ship (HORIZONTALLY)
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            grid[topOrLeft.r][topOrLeft.c + i] = '.';
        }
        //note: needed iterator to use remove function
        listOfShipIds.erase(remove(listOfShipIds.begin(), listOfShipIds.end(), shipId), listOfShipIds.end());
        return true;
    }
    //if conditions are met, unplace ship (VERTICALLY)
    if (dir == VERTICAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            grid[topOrLeft.r + i][topOrLeft.c] = '.';
        }
        //note: needed iterator to use remove function
        listOfShipIds.erase(remove(listOfShipIds.begin(), listOfShipIds.end(), shipId), listOfShipIds.end());
        return true;
    }
    return false;
}

void BoardImpl::display(bool shotsOnly) const
{
    // This compiles, but may not be correct
    //BELOW IS INCORRECT
    //Also note: did not utilize the shotsOnly parameter
    cout << "  ";
    for (int i = 0; i < m_cols; i++)
    {
        cout << i;
    }
    cout << endl;
    if (shotsOnly)
    {
        for (int r = 0; r < m_rows; r++)
        {
            cout << r << " ";
            for (int c = 0; c < m_cols; c++)
            {
                if (grid[r][c] == 'X')
                {
                    cout << 'X';
                }
                else if (grid[r][c] == 'o')
                {
                    cout << 'o';
                }
                else
                {
                    cout << '.';
                }
            }
            cout << endl;
        }
    }
    else
    {
        for (int r = 0; r < m_rows; r++)
        {
            cout << r << " ";
            for (int c = 0; c < m_cols; c++)
            {
                cout << grid[r][c];
            }
            cout << endl;
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //if point is out of bounds
    if ((p.r < 0 || p.r >= m_rows) || (p.c < 0 || p.c >= m_cols))
    {
        return false;
    }
    //if point is attacking an area that is already attacked
    if (grid[p.r][p.c] == 'X' || grid[p.r][p.c] == 'o')
    {
        return false;
    }
    //If point does not hit ship (ocean), set shotHit to false. Otherwise, set shotHit to true. Change board accordingly.
    char symbol = '.';
    int myShipId = -1;
    if (grid[p.r][p.c] == '.')
    {
        grid[p.r][p.c] = 'o';
        shotHit = false;
        return true;
    }
    else
    {
        //find the shipId that matches the symbol and store it
        symbol = grid[p.r][p.c];
        for (size_t i = 0; i < listOfShipIds.size(); i++)
        {
            if (m_game.shipSymbol(listOfShipIds[i]) == symbol)
            {
                myShipId = listOfShipIds[i];
            }
        }
        grid[p.r][p.c] = 'X';
        shotHit = true;
    }
    //find any instance of the ship symbol with given shipId
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
        {
            if (grid[r][c] == m_game.shipSymbol(myShipId))
            {
                shipDestroyed = false;
                return true;
            }
        }
    }
    //ship must be destroyed if it failed previous check. Update accordingly
    shipDestroyed = true;
    if (shipDestroyed == true)
    {
        listOfShipIds.erase(remove(listOfShipIds.begin(), listOfShipIds.end(), myShipId), listOfShipIds.end());
        shipId = myShipId;
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    if (listOfShipIds.size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
