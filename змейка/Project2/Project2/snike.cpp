/*-----підключення бібліотек------*/
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

/*-----розміри поля------*/
const int x_max = 100;
const int y_max = 25;

/*----функція котра відповідає за курсор консолі-----*/
void GoToXY(int x, int y);
void CursorVisible(bool visible);

/*-----розрахунок поворотів-----*/
enum Rotation
{
    left,
    right,
    up,
    down
};

/*----структура позиції по х/у-----*/
struct Position
{
    int x;
    int y;
};

/*-----масів символів------*/
class Field
{
public:
    char field[y_max][x_max + 1];
    void CreateBoundaries(char ch);
    void Show();
    void SpownPrice();
};

/*----функція змії -----*/
class Snake
{
private:
    int length; /*----змінна дліни змії -----*/
    Position snakePos[1000]; /*----змінна позиції змії -----*/
    Rotation snakeRot; /*----змінна поворотів змії -----*/

public:
    Snake(int _length, Rotation _snakeRot) :
        length(_length), snakeRot(_snakeRot) {}; 

    void ToStartValues();
    void Move(char field[y_max][x_max + 1]); \
        void AddTail();
    bool IsGameOver();
};

/*----функція видалення курсора в консолі-----*/
int main()
{
    srand(time(NULL));

    CursorVisible(false);

    Field gameField;
    Snake snake(1, up);

    gameField.CreateBoundaries(219);
    snake.ToStartValues();

    for (int i = 0; i < 20; i++) /*----к-ть яблук які будуть на полі (в цьому випадку 20)-----*/
        gameField.SpownPrice();

    /*----фунція закінчення гри-----*/
    while (true)
    {
        if (snake.IsGameOver()) break; /*-----якщо відбувається одне із моментів цикл закінчується------*/

        snake.Move(gameField.field);
        gameField.Show();

        Sleep(85); /*-----скорость змії------*/
    }

    getchar();
}
/*--------------------------------------*/



/*------------------функції поля----------------*/

/*----створення меж----*/
void Field::CreateBoundaries(char ch)
{
    for (int y = 0; y < y_max; y++)
    {
        for (int x = 0; x < x_max; x++)
        {
            if (y == 0 || y == y_max - 1 ||
                x == 0 || x == x_max - 1)
            {
                field[y][x] = ch;
            }
            else
            {
                field[y][x] = ' ';
            }
        }
        field[y][x_max] = '\0';
    }
}

void Field::Show()
{
    GoToXY(0, 0);

    for (int y = 0; y < y_max; y++)
    {
        printf("%s\n", field[y]);
    }
}

/*----------утворення яблук або букв 'a' які стають хвостом смії-------------*/
void Field::SpownPrice()
{
    int x = rand() % (x_max - 2) + 1;
    int y = rand() % (y_max - 2) + 1;

    bool isSpown = false;
    while (true)
    {
        if (x % 2 == 0) isSpown = true;

        if (field[y][x] == 'O' || field[y][x] == '*') isSpown = false;

        if (isSpown) break;

        x = rand() % (x_max - 2) + 1; /*-------функція котра розміщає яблука в рандомних місцях в межах поля------*/
    }

    field[y][x] = 'a';
}
/*-----------------------*/



/*----------функції змії---------*/
void Snake::ToStartValues()
{
    for (int i = 0; i < length; i++)
    {
        snakePos[i].x = 0;
        snakePos[i].y = 0;
    }

    length = 1;

    snakePos[0] = { x_max / 2, y_max / 2 };

    GoToXY(snakePos[0].x, snakePos[0].y);
    putchar('O');
}


/*----функція бінда кнопок для руху змії----*/
void Snake::Move(char field[y_max][x_max + 1])
{
    if (GetAsyncKeyState('W'))
    {
        snakeRot = up;
    }
    else if (GetAsyncKeyState('S'))
    {
        snakeRot = down;
    }
    else if (GetAsyncKeyState('A'))
    {
        snakeRot = left;
    }
    else if (GetAsyncKeyState('D'))
    {
        snakeRot = right;
    }

    field[snakePos[length - 1].y][snakePos[length - 1].x] = ' '; /*----фунція яка ставе пробіл і видаляє останій елемент-----*/

    for (int i = length; i > 0; i--)
    {
        snakePos[i] = snakePos[i - 1]; /*----функція яка присвоєю значення попереднього елемента голови змії-----*/
    }

    switch (snakeRot)
    {
    case up:
        snakePos[0].y = snakePos[0].y - 1;
        break;

    case down:
        snakePos[0].y = snakePos[0].y + 1;
        break;

    case left:
        snakePos[0].x = snakePos[0].x - 2;
        break;

    case right:
        snakePos[0].x = snakePos[0].x + 2;
        break;
    }


    /*-------функція при доторкання голови змії до яблука вона збільшується-------*/

    if (field[snakePos[0].y][snakePos[0].x] == 'a') AddTail(); /*----можна змінити яблуко----*/

    field[snakePos[0].y][snakePos[0].x] = '0'; /*----можна змінити голову змії----*/

    if (length > 1)
    {
        field[snakePos[1].y][snakePos[1].x] = '*'; /*----можна змінити тіло змії----*/
    }
}

void Snake::AddTail()
{
    snakePos[length].x = snakePos[length - 1].x;
    snakePos[length].y = snakePos[length - 1].y;

    length++;
}

/*-----код про закінчення гри------*/
bool Snake::IsGameOver()
{
    bool isGameOver = false;

    /*-----якщо кордината голови змії=кординаті поля, гра закінчується------*/
    if (snakePos[0].x <= 0 || snakePos[0].x >= x_max - 1 ||
        snakePos[0].y <= 0 || snakePos[0].y >= y_max - 1)
    {
        isGameOver = true;
    }

    /*-----якщо кордината голови змії=кординатам хвоста змії, гра закінчується------*/
    for (int i = 2; i < length; i++)
    {
        if (snakePos[0].x == snakePos[i].x &&
            snakePos[0].y == snakePos[i].y)
        {
            isGameOver = true;
        }
    }

    return isGameOver;
}
/*-------------------------------------*/


/*----------функія курсора яка ставить наш курсор в певне місце х/у----------*/
void GoToXY(int x, int y)
{
    COORD pos = { x, y };
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, pos);
}

void CursorVisible(bool visible)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = visible;
    SetConsoleCursorInfo(handle, &structCursorInfo);
}