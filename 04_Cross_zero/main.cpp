#include <iostream>
#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <functional>

enum { Up, Down, Left, Right, DiagonalUL, DiagonalUR, DiagonalDL, DiagonalDR};

class Game
{
public:
	std::function<Game(const int _s, Game _game)> m_f1;
	std::function<Game(const int _s, Game _game)> m_f2;

	static Game botMove(const int _s, Game _game);
	static Game userMove(const int _s, Game _game);

	static Game newBalance(Game, const int _s, int _x, int _y);
	int power(Game _game, int _x, int _y, int _way, int _s, int _stop);
	static void Print(Game game);
	static bool winner(Game _game, int _x, int _y, int _s);
	static void start_Game();

	Game & operator =(Game && _game);

	Game();
	Game(int _column, int _line, char f1_, int _mod);
	Game(const Game & _game);

	int GetMinSize() const;
	bool GetIsPlaying() { return m_isPlaying; }
private:
	Game newBalance_help(Game _game, int _x, int _y, int _way, int _s, int _stop);

	int m_x;
	int m_y;

	bool m_isPlaying;
	bool m_player1;
	bool m_player2;
	int** m_mas;
};

Game::Game() : m_x(0),m_y(0), m_player1(0), m_player2(1), m_isPlaying(true){}

Game::Game(int _x, int _y, char _f1, int _mod)
	:m_isPlaying(true)
{
	if (_x < 5)	m_x = 5;
	else m_x = _x;
	if (_y < 5)	m_y = 5;
	else m_y = _y;

	if (_f1 == 'o')
		m_player1 = 0;
	else
		m_player1 = 1;
	m_player2 = !m_player1;

	if (_mod == 1){
		m_f1 = userMove;
		m_f2 = userMove;
	}

	else if (_mod == 2)	{
		m_f1 = botMove;
		m_f2 = botMove;
	}
	else{
		m_f1 = userMove;
		m_f2 = botMove;
	}



	m_mas = new int *[m_y];
	for (int i = 0; i < m_y; i++)
		m_mas[i] = new int[m_x];

	for (int i = 0; i < m_y; i++)
		for (int j = 0; j < m_x; j++)
			m_mas[i][j] = 2;
}

Game::Game(const Game & _game)
	:m_x(_game.m_x), m_y(_game.m_y), m_player1(_game.m_player1), m_player2(_game.m_player2), m_isPlaying(_game.m_isPlaying)
{
	m_mas = new int *[m_y];
	for (int i = 0; i < m_y; i++)
		m_mas[i] = new int[m_x];

	for (int i = 0; i < m_y; i++)
		memcpy(m_mas[i], _game.m_mas[i], sizeof(int) * m_x);
}

Game & Game::operator=(Game && _game)
{
	if (&_game == this)
		return *this;

	m_x = _game.m_x;
	m_y = _game.m_y;
	m_player1 = _game.m_player1;
	m_player2 = !m_player1;

	m_isPlaying = _game.m_isPlaying;

	delete[] m_mas;

	m_mas = _game.m_mas;
}

int Game::GetMinSize() const
{
	if (m_x < m_y)
		return m_x;
	return m_y;
}

int Game::power(Game _game, int _x, int _y, int _way, int _s, int _stop)
{

	int power = 1;
	int k_y, k_x;
	
	switch (_way)
	{
		case Up: 
			k_y = 1;
			k_x = 0;
			break;

		case Down: 
			k_y = -1;
			k_x = 0;
			break;

		case Left: 
			k_y = 0;
			k_x = 1;
			break;

		case Right: 
			k_y = 0;
			k_x = -1;
			break;

		case DiagonalUL: 
			k_y = 1;
			k_x = 1;
			break;

		case DiagonalUR:
			k_y = 1;
			k_x = -1;
			break;

		case DiagonalDL:
			k_y = -1;
			k_x = 1;
			break;

		case DiagonalDR:
			k_y = -1;
			k_x = -1;
			break;

		default:
			k_y = 0;
			k_x = 0;
	}

	for (int i = 1; i < 4; i++)
	{
		if (_y + k_y * i < 0 || _y + k_y * i >= _game.m_y)
			break;
		if (_x + k_x * i < 0 || _x + k_x * i >= _game.m_x)
			break;

		if (_game.m_mas[_y + k_y * i][_x + k_x * i] == _stop)
			break;

		if (_game.m_mas[_y + k_y * i][_x + k_x * i] == _s)
			power++;

	}
	return power;
}

Game Game::newBalance_help(Game _game, int _x, int _y, int _way, int _s, int _stop)
{
	int power = _game.power(_game, _x, _y, _way, _s, _stop);
	int powerMax = 4;
	int k_y, k_x;
	switch (_way)
	{
		case Up:
			k_y = -1;
			k_x = 0;
		break;

		case Down:
			k_y = 1;
			k_x = 0;
		break;

		case Left:
			k_y = 0;
			k_x = -1;
		break;

		case Right:
			k_y = 0;
			k_x = 1;
		break;

		case DiagonalUL:
			k_y = -1;
			k_x = -1;
		break;

		case DiagonalUR:
			k_y = -1;
			k_x = 1;
		break;

		case DiagonalDL:
			k_y = 1;
			k_x = -1;
		break;

		case DiagonalDR:
			k_y = 1;
			k_x = 1;
		break;

		default:
			k_y = 0;
			k_x = 0;
	}

	for (int i = 1; i < 5; i++)
	{
		if (_y + k_y * i < 0 || _y + k_y * i >= _game.m_y)
			break;
		if (_x + k_x * i < 0 || _x + k_x * i >= _game.m_x)
			break;
		if (_game.m_mas[_y + k_y * i][_x + k_x * i] == _stop)
			break;


		if ((_y + !k_y * 4 + k_y * i < 0) || (_y + !k_y * 4 + k_y * i >= _game.m_y))
			if ((_x + !k_x * 4 + k_x * i < 0) || (_x + !k_x * 4 + k_x * i >= _game.m_x))
				if (_game.m_mas[_y + !k_y * 4 + k_y * i][_x + !k_x * 4 + k_x * i] != _s)
					powerMax--;

		if ((_y + 4 - i) < _game.m_y && _game.m_mas[_y + 4 - i][_x] != _s)
			powerMax--;
		if (power > powerMax)
			power = powerMax;

		if (_game.m_mas[_y + k_y * i][_x + k_x * i] != _s)
			_game.m_mas[_y + k_y * i][_x + k_x * i] += (5 - i) * power;
	}

	return _game;
}

Game Game::newBalance(Game _game, int _s, int _x, int _y)
{
	for (int i = Up; i <= DiagonalDR; i++)
	{
		_game = _game.newBalance_help(_game, _x, _y, i, _s, !_s);
	}
	return _game;
}

bool Game::winner(Game _game, int _x, int _y, int _s)
{
	int line = 0;
	//check vertical lines
	for (int i = 0; i < 5; i++)
	{
		for (int j = _y - 4 + i; j <= _y + i; j++)
		{
			if (j < 0 || j >= _game.m_y)
				break;
			if (_game.m_mas[j][_x] == _s)
				line++;
		}

		if (line == 5)
			return true;
		line = 0;
	}

	//check horizontal lines
	for (int i = 0; i < 5; i++)
	{
		for (int j = _x - 4 + i; j <= _x + i; j++)
		{
			if (j < 0 || j >= _game.m_x)
				break;
			if (_game.m_mas[_y][j] == _s)
				line++;
		}

		if (line == 5)
			return true;
		line = 0;
	}
	//check diagonal \ lines 
	for (int i = 0; i < 5; i++)
	{
		for (int j_x = _x - 4 + i, j_y = _y - 4 + i; j_x <= _x + i; j_x++, j_y++)
		{
			if (j_x < 0 || j_x >= _game.m_x)
				break;
			if (j_y < 0 || j_y >= _game.m_y)
				break;

			if (_game.m_mas[j_y][j_x] == _s)
				line++;
		}
		if (line == 5)
			return true;
		line = 0;
	}

	//check diagonal / lines 

	for (int i = 0; i < 5; i++)
	{
		for (int j_x = _x - 4 + i, j_y = _y + 4 - i; j_x <= _x + i; j_x++, j_y--)
		{
			if (j_x < 0 || j_x >= _game.m_x)
				break;
			if (j_y < 0 || j_y >= _game.m_y)
				break;

			if (_game.m_mas[j_y][j_x] == _s)
				line++;
		}
		if (line == 5)
			return true;
		line = 0;
	}

	return false;
}

Game Game::botMove(const int _s, Game _game)
{
	int maxY = 0, maxX = 0;
	for (int i = 0; i < _game.m_y; i++)
		for (int j = 0; j < _game.m_x; j++)
			if (_game.m_mas[i][j] > _game.m_mas[maxY][maxX])
			{
				maxY = i;
				maxX = j;
			}
	if (_game.m_mas[maxY][maxX] == 2)
	{
		srand(time(NULL));
		maxX = rand() % _game.m_x;
		maxY = rand() % _game.m_y;
	}

	if(_game.m_mas[maxY][maxX] > 1)
		_game.m_mas[maxY][maxX] = _s;

	if (winner(_game, maxX, maxY, _s))
	{
		_game.m_isPlaying = false;
		return _game;
	}
	return newBalance(_game, _s, maxX,maxY);
}

Game Game::userMove(const int _s, Game _game)
{
	int x = _game.m_x / 2;
	int y = _game.m_y / 2;

	char ch;
	while (true)
	{
		ch = 0;
		Game g_print = _game;
		g_print.m_mas[y][x] = -1;

		Game::Print(g_print);

		while(ch <= 0)
			ch = _getch();
		
			switch (ch)
			{
			case 72:
				// Up arrow
				if(y - 1 >= 0)
					y--;
				break;

			case 80:
				// Down arrow
				if (y + 1 < _game.m_y)
					y++;
				break;

			case 75:
				// Left arrow

				if (x - 1 >= 0)
					x--;
				break;

			case 77:
				// Right arrow
				if (x + 1 < _game.m_x)
					x++;
				break;

			case 32:
				// Space
				if (_game.m_mas[y][x] > 1)
				{
					_game.m_mas[y][x] = _s;
					if (winner(_game, x, y, _s))
					{
						_game.m_isPlaying = false;
						return _game;
					}
					return _game.newBalance(_game, _s, x, y);
				}
				break;
			}
		
	}

	return _game;
}

void Game::Print(Game _game)
{
	system("cls");
	for (int i = 0; i < _game.m_x * 4; i++)
		std::cout << '_';
	std::cout << '\n';
	
	for (int i = 0; i < _game.m_y; i++)
	{
		for (int j = 0; j < _game.m_x; j++)
		{
			if (_game.m_mas[i][j] == 0)
				std::cout << "|/\\|";
			else if (_game.m_mas[i][j] == 1)
				std::cout << "|\\/|";
			else if (_game.m_mas[i][j] == -1)
				std::cout << "|**|";
			else
			{
				std::cout << '|';
				std::cout.width(2);
				std::cout << _game.m_mas[i][j];
				std::cout.width(1);
				std::cout << '|';
			}
		}

		std::cout << '\n';
		for (int j = 0; j < _game.m_x; j++)
		{
			if (_game.m_mas[i][j] == 0)
				std::cout << "|\\/|";
			else if (_game.m_mas[i][j] == 1)
				std::cout << "|/\\|";
			else if (_game.m_mas[i][j] == -1)
				std::cout << "|**|";
			else
			{
				std::cout << "|  |";
			}
		}

		std::cout << '\n';
		for (int i = 0; i < _game.m_x * 4; i++)
			std::cout << '_';
		std::cout << '\n';
	}
}

Game giveMeGame()
{
	int x, y, mod;
	char f1;
	std::cout << "move by arrows, space -- enter zero or cross \n";
	std::cout << "enter x(min 5) ";
	std::cin >> x;
	std::cout <<'\n'<< "enter y(min 5) ";
	std::cin >> y;
	std::cout << '\n';
	std::cout << '\n' << "enter figure1(o/x) ";
	std::cin >> f1;
	std::cout << '\n';
	std::cout << '\n' << "enter mod(0 - user vs pc/ 1 - user vs user/ 2 - pc vs pc) ";
	std::cin >> mod;
	std::cout << '\n';
	return Game(x, y, f1, mod);
}

void Game::start_Game()
{
	Game game = giveMeGame();

	while (true)
	{
		Game::Print(game);
		if (game.GetIsPlaying() == false) {
			std::cout << "end Game ";
			system("pause");
			break;
		}

		system("pause");
		game = game.m_f1(game.m_player1,game);

		Game::Print(game);
		if (game.GetIsPlaying() == false){
			std::cout << "end Game ";
			system("pause");
			break;
		}
		system("pause");
		game = game.m_f2(game.m_player2, game);
	}
}

int main()
{
	Game::start_Game();
	return 0;
}