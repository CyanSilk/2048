#include <iostream> // To input_&print.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>  // To creat random nums.
#include <string.h>  // String.
#include <windows.h> // To clear the screen.
#include <conio.h>
#include <cctype> // Switch upper letters to lower.
using namespace std;

void move_num(int bd[4][4], string move_, int (&arr)[4][4], bool real_move = true);
void pre_move(int bd[4][4], string move_, int (&arr)[4][4], bool real_move = false);
void print_board();
// void print_board2();
void add_num();
void is_over();
void get_direction(string tips);
void game();
void gotoxy(short x, short y);
// void lt_lab();
// void command();
void setColor(bool flag = false);

string input_(); // input_ without pressing enter.

int where_empty();
int new_num();

// Define global variables.
bool over, w_ok, s_ok, a_ok, d_ok;
string mv;
// char what_color[8];
int Grade;
// Get handle.
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO pos[3];

int board[4][4]; // Define the game board.
int my_bd[4][4]; // The pretend board enables judge the following running steps.
int or_bd[4][4]; // Original board helps judge which pos changes.

// Record which pos changes.
int change[4][4];

// To clear the pos which's yellow in last turn.
int clear_color[2];

int main()
{
    // Set title.
    SetConsoleTitleA("2048");

    // Hide the cursor.
    cout << "\033[?25l";

    // Creat a seed for random num.
    srand(time(0));

    while (1)
    {
        // Initialize the following variables.
        over = false;
        Grade = 0;
        // w_ok = true;
        // s_ok = true;
        // a_ok = true;
        // d_ok = true;

        // Go to game().
        game();

        // // Pause and wait for user closing the window.
        // cout << "Press any key to close the window.";
        // _getch();

        if (mv == "quit")
            break;
        if (mv == "restart")
        {
            for (int x = 0; x < 4; x++)
                for (int y = 0; y < 4; y++)
                    board[x][y] = 0;
            system("cls");
            continue;
        }

        // Pause and wait for an input.
        gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
        cout << "\033[0K" << "Game Over!" << endl;
        cout << "Press \"q\" to quit, \"r\" to restart.";
        string choice;
        while (choice != "q" && choice != "r")
            choice = input_();
        if (choice == "q")
            break;

        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
            {
                board[x][y] = 0;
                my_bd[x][y] = 0;
                or_bd[x][y] = 0;
            }
        system("cls");
    }
    // Exit.
    return 0;
}

// void lt_lab() // Debug the code.
// {
//     // cout << w_ok << endl;

//     // // Debug new_num().
//     // for (int i = 0; i < 20; i++)
//     // {
//     // cout << new_num() << ' ';
//     // }

//     // // Debug print_board().
//     // print_board();

//     // // Debug get_direction().
//     // while (!over)
//     // {
//     // get_direction("输入wsad移动数字：");
//     // cout << "mv = " << mv << endl;
//     // }

//     // a[0][0] = 1;
// }

void game()
{
    // Generate 2 nums.
    add_num();
    add_num();
    // // Print the board.
    // print_board();

    // Print original board.
    cout << "Grade: ";
    // Get coord where grade start.
    CONSOLE_SCREEN_BUFFER_INFO GradeInfo;
    GetConsoleScreenBufferInfo(hConsole, &GradeInfo);
    pos[0] = GradeInfo;
    printf("%6d\n\n", Grade);

    // Get coord where num start.
    CONSOLE_SCREEN_BUFFER_INFO NumInfo;
    GetConsoleScreenBufferInfo(hConsole, &NumInfo);
    pos[1] = NumInfo;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (board[x][y])
                printf("%-8d", board[x][y]);
            else
                printf("%8s", "");
        }
        cout << endl
             << endl;
    }

    // Get coord where tips start.
    CONSOLE_SCREEN_BUFFER_INFO TipInfo;
    GetConsoleScreenBufferInfo(hConsole, &TipInfo);
    pos[2] = TipInfo;

    // Start the game.
    while (!over)
    {
        // Initialize the following variables.
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
                change[x][y] = 0;
        w_ok = true;
        s_ok = true;
        a_ok = true;
        d_ok = true;

        // Judge if game is over.
        is_over();
        if (over)
            break;

        // Input a direction.
        get_direction("Input \"wsad\" to move nums. ");
        if (over) // Considering the case of inputing "q"/"r".
            break;
        pre_move(board, mv, board, true);

        // After the combination/movement of some nums, there must have been at least one gap to add num.
        add_num();

        // // Clear the screen and print the board.
        // system("cls");
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (change[x][y] != 2)
                    change[x][y] = (board[x][y] != or_bd[x][y]);
            }
        }

        print_board();

        // // Change color.
        // command();
        // system(what_color);
    }
}

int new_num() // To generate new num.
{
    // int flag = rand() % 2; // Flag = 0 or 1.
    // return ++flag * 2;     // Return 2 or 4;
    if (rand() % 10)
        return 2; // Generate 2 at 90%.
    return 4;     // While 4 at 10%.
}

// void print_board() // Print the game board.
// {
//     cout << "Grade: " << Grade << endl;
//     cout << endl;
//     for (int x = 0; x < 4; x++)
//     {
//         for (int y = 0; y < 4; y++)
//         {
//             if (board[x][y])
//                 cout << board[x][y] << '\t';
//             else
//                 cout << " \t";
//         }
//         cout << endl
//              << endl;
//     }
// }

void gotoxy(short x, short y) // Set the cursor's position at coord (x, y).
{
    COORD position = {x, y};
    SetConsoleCursorPosition(hConsole, position);
}

void setColor(bool flag) // Set color.
{
    if (!flag)
        SetConsoleTextAttribute(hConsole, 7); // White
    else
        SetConsoleTextAttribute(hConsole, 14); // Yellow
}

void print_board() // Print the game board, and avoid flashing.
{
    // Go to where grade should be print.
    gotoxy(pos[0].dwCursorPosition.X, pos[0].dwCursorPosition.Y);
    printf("%6d", Grade);
    if (clear_color)
    {
        gotoxy(pos[1].dwCursorPosition.X + 8 * clear_color[1], pos[1].dwCursorPosition.Y + 2 * clear_color[0]);
        if (board[clear_color[0]][clear_color[1]])
            printf("%-8d", board[clear_color[0]][clear_color[1]]);
        else
            printf("%8s", "");
    }

    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (change[x][y])
            {
                setColor(change[x][y] - 1);
                if (change[x][y] == 2)
                {
                    clear_color[0] = x;
                    clear_color[1] = y;
                }

                gotoxy(pos[1].dwCursorPosition.X + 8 * y, pos[1].dwCursorPosition.Y + 2 * x);
                if (board[x][y])
                    printf("%-8d", board[x][y]);
                else
                    printf("%8s", "");
            }
        }
    }
    setColor();
}

// void print_board2() // Print the game board.
// {
//     gotoxy(0, 10);
//     cout << "----------------" << endl
//          << endl;
//     cout << "Grade: " << Grade << endl;
//     cout << endl;
//     for (int x = 0; x < 4; x++)
//     {
//         for (int y = 0; y < 4; y++)
//         {
//             if (board[x][y])
//                 cout << board[x][y] << '\t';
//             else
//                 cout << " \t";
//         }
//         cout << endl
//              << endl;
//     }
//     cout << "----------------" << endl
//          << endl;
//     for (int x = 0; x < 4; x++)
//     {
//         for (int y = 0; y < 4; y++)
//         {
//             cout << change[x][y] << '\t';
//         }
//         cout << endl
//              << endl;
//     }
// }

void add_num() // Generate new num on the board.
{
    int posi = where_empty();
    board[posi / 10 - 1][posi % 10 - 1] = new_num();
    change[posi / 10 - 1][posi % 10 - 1] = 2;
}

int where_empty() // Find an empty place.
{
    // Use x&y to symbolize every position.
    int empty[16], arrow = 0;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            // Find which position is empty on the board.
            if (!board[x][y])
                // Let the x+1 in the tens place while the y+1 in the ones place.
                empty[arrow++] = 10 * (x + 1) + (y + 1);
        }
    }
    return empty[rand() % arrow];
}

void get_direction(string tips) // input_ how to move the num.
{
    bool conti = true;
    if (tips != "")
    {
        gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
        cout << "\033[0K" << tips;
    }
    while (conti)
    {
        // if (mv = input_())
        // {
        mv = input_();
        if (mv == "w" || mv == "s" || mv == "a" || mv == "d")
            conti = false;
        // else if (mv == "W")
        // {
        //     mv = "w";
        //     conti = false;
        // }
        // else if (mv == "S")
        // {
        //     mv = "s";
        //     conti = false;
        // }
        // else if (mv == "A")
        // {
        //     mv = "a";
        //     conti = false;
        // }
        // else if (mv == "D")
        // {
        //     mv = "d";
        //     conti = false;
        // }
        else if (/*mv == "Q" || */ mv == "q") // Allow quit the game.
        {
            string quit;
            gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
            cout << "\033[0K" << "Want to quit the game? (\'y\')";
            // if (quit = input_())
            // {
            quit = input_();
            if (quit == "y")
            {
                over = true;
                gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
                cout << "\033[0K";
                mv = "quit";
                break;
            }
            // // system("cls");
            // print_board();
            // cout << tips;
            gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
            cout << "\033[0K" << "Input \"wsad\" to move nums. ";
            // }
            // else
            // {
            //     cin.clear(); // Avoid the bug occurs in case of the input_ of Ctrl+Z.
            //     string quit;
            //     cout << "Wrong input_ detected, exit?(\'y\')";
            //     if (quit = input_())
            //     {
            //         if (quit == "y")
            //         {
            //             over = true;
            //             break;
            //         }
            //         // system("cls");
            //         print_board();
            //         cout << tips;
            //     }
            //     else
            //     {
            //         cin.clear();
            //         over = true;
            //         break;
            //     }
            // }
        }
        else if (mv == "r")
        {
            string restart;
            gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
            cout << "\033[0K" << "Want to restart the game? (\'y\')";
            restart = input_();
            if (restart == "y")
            {
                over = true;
                gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
                cout << "\033[0K";
                mv = "restart";
                break;
            }
            gotoxy(pos[2].dwCursorPosition.X, pos[2].dwCursorPosition.Y);
            cout << "\033[0K" << "Input \"wsad\" to move nums. ";
        }

        else
        {
            // // input_ not in law.
            // // system("cls");
            // print_board();
            // cout << "Not a legal value, please enter another: ";
            continue;
        }
        // }
        // else
        // {
        //     cin.clear();
        //     string quit;
        //     cout << "Wrong input_ detected, exit?(\'y\')";
        //     quit = input_();
        //     if (quit == "y")
        //     {
        //         over = true;
        //         break;
        //     }
        //     // system("cls");
        //     print_board();
        //     cout << tips;
        //     // if (quit = input_())
        //     // {
        //     //     if (quit == "y")
        //     //     {
        //     //         over = true;
        //     //         break;
        //     //     }
        //     //     // system("cls");
        //     //     print_board();
        //     //     cout << tips;
        //     // }
        //     // else
        //     // {
        //     //     cin.clear();
        //     //     over = true;
        //     //     break;
        //     // }
        // }
    }
}

void pre_move(int bd[4][4], string move_, int (&arr)[4][4], bool real_move) // Avoid orgin board being changed.
{
    int temp[4][4];
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            // Copy the bd.
            temp[x][y] = bd[x][y];
        }
    }

    // Record orinal board.
    if (real_move)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
                or_bd[x][y] = bd[x][y];
        }
    }

    move_num(temp, move_, arr, real_move);
}

void is_over() // Judge if game is over.
{
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            my_bd[x][y] = board[x][y];

    /* Predict every direction, in a bid to judge
    what direction the user cannot move to.*/
    int w[4][4], s[4][4], a[4][4], d[4][4];
    pre_move(my_bd, "w", w);
    pre_move(my_bd, "a", a);
    pre_move(my_bd, "s", s);
    pre_move(my_bd, "d", d);

    // Four situations:
    bool flag = true;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (w[x][y] != my_bd[x][y])
            {
                flag = false;
                break;
            }
        }
    }
    if (flag)
        w_ok = false;

    flag = true;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (s[x][y] != my_bd[x][y])
            {
                flag = false;
                break;
            }
        }
    }
    if (flag)
        s_ok = false;

    flag = true;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (a[x][y] != my_bd[x][y])
            {
                flag = false;
                break;
            }
        }
    }
    if (flag)
        a_ok = false;

    flag = true;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            if (d[x][y] != my_bd[x][y])
            {
                flag = false;
                break;
            }
        }
    }
    if (flag)
        d_ok = false;

    if (!(w_ok || s_ok || a_ok || d_ok))
        over = true;
}

void move_num(int bd[4][4], string move_, int (&arr)[4][4], bool real_move) // Move and combine nums.
{
    // If over stop the game.
    if (over)
        return;

    if (move_ == "w")
    {
        // Judge if the direction's correct.
        if (!w_ok)
        {
            // // system("cls");
            // print_board();
            // get_direction("Not a legal direction, please input_ another: ");
            get_direction("");
            move_num(bd, mv, arr);
            return;
        }

        // Combine the same nums that's near another.
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (!bd[x][y])
                    continue;
                int orgin = bd[x][y], i = 1;
                while (bd[x][y] == orgin)
                {
                    if (x + i > 3)
                        break; // Avoid out of range.
                    if (bd[x][y] == bd[x + i][y])
                    {
                        if (real_move)
                            Grade += bd[x][y];
                        bd[x][y] *= 2;
                        bd[x + i][y] = 0;
                        break;
                    }
                    else if (!bd[x + i][y])
                        i++;
                    else
                        break;
                }
            }
        }

        // Fill the gaps.
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                int i = 1;
                while (!bd[x][y])
                {
                    if (x + i > 3)
                        break;
                    // Move the num.
                    bd[x][y] = bd[x + i][y];
                    bd[x + i][y] = 0;
                    i++;
                }
            }
        }
    }

    else if (move_ == "s")
    {
        // Judge.
        if (!s_ok)
        {
            // // system("cls");
            // print_board();
            // get_direction("Not a legal direction, please input_ another: ");
            get_direction("");
            move_num(bd, mv, arr);
            return;
        }

        // Add.
        for (int x = 3; x >= 0; x--)
        {
            for (int y = 0; y < 4; y++)
            {
                if (!bd[x][y])
                    continue;
                int orgin = bd[x][y], i = 1;
                while (bd[x][y] == orgin)
                {
                    if (x - i < 0)
                        break;
                    if (bd[x][y] == bd[x - i][y])
                    {
                        if (real_move)
                            Grade += bd[x][y];
                        bd[x][y] *= 2;
                        bd[x - i][y] = 0;
                        break;
                    }
                    else if (!bd[x - i][y])
                        i++;
                    else
                        break;
                }
            }
        }

        // Fill.
        for (int x = 3; x >= 0; x--)
        {
            for (int y = 0; y < 4; y++)
            {
                int i = 1;
                while (!bd[x][y])
                {
                    if (x - i < 0)
                        break;
                    bd[x][y] = bd[x - i][y];
                    bd[x - i][y] = 0;
                    i++;
                }
            }
        }
    }

    else if (move_ == "a")
    {
        // Judge.
        if (!a_ok)
        {
            // // system("cls");
            // print_board();
            // get_direction("Not a legal direction, please input_ another: ");
            get_direction("");
            move_num(bd, mv, arr);
            return;
        }

        // Add.
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                if (!bd[x][y])
                    continue;
                int orgin = bd[x][y], i = 1;
                while (bd[x][y] == orgin)
                {
                    if (y + i > 3)
                        break;
                    if (bd[x][y] == bd[x][y + i])
                    {
                        if (real_move)
                            Grade += bd[x][y];
                        bd[x][y] *= 2;
                        bd[x][y + i] = 0;
                        break;
                    }
                    else if (!bd[x][y + i])
                        i++;
                    else
                        break;
                }
            }
        }

        // Fill.
        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                int i = 1;
                while (!bd[x][y])
                {
                    if (y + i > 3)
                        break;
                    bd[x][y] = bd[x][y + i];
                    bd[x][y + i] = 0;
                    i++;
                }
            }
        }
    }

    else
    {
        // Judge.
        if (!d_ok)
        {
            // // system("cls");
            // print_board();
            // get_direction("Not a legal direction, please input_ another: ");
            get_direction("");
            move_num(bd, mv, arr);
            return;
        }

        // Add.
        for (int x = 0; x < 4; x++)
        {
            for (int y = 3; y >= 0; y--)
            {
                if (!bd[x][y])
                    continue;
                int orgin = bd[x][y], i = 1;
                while (bd[x][y] == orgin)
                {
                    if (y - i < 0)
                        break;
                    if (bd[x][y] == bd[x][y - i])
                    {
                        if (real_move)
                            Grade += bd[x][y];
                        bd[x][y] *= 2;
                        bd[x][y - i] = 0;
                        break;
                    }
                    else if (!bd[x][y - i])
                        i++;
                    else
                        break;
                }
            }
        }

        // Fill.
        for (int x = 0; x < 4; x++)
        {
            for (int y = 3; y >= 0; y--)
            {
                int i = 1;
                while (!bd[x][y])
                {
                    if (y - i < 0)
                        break;
                    bd[x][y] = bd[x][y - i];
                    bd[x][y - i] = 0;
                    i++;
                }
            }
        }
    }

    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            // Copy the bd.
            arr[x][y] = bd[x][y];
        }
    }
}

// void command()
// {
//     char a, b;
//     a = rand() % 16;
//     b = rand() % 16;
//     while (a == b)
//     {
//         b = rand() % 16; // Avoid a = b.
//     }
//     if (a == 10)
//         a = 'a';
//     else if (a == 11)
//         a = 'b';
//     else if (a == 12)
//         a = 'c';
//     else if (a == 13)
//         a = 'd';
//     else if (a == 14)
//         a = 'e';
//     else
//         a = 'f';
//     if (b == 10)
//         b = 'a';
//     else if (b == 11)
//         b = 'b';
//     else if (b == 12)
//         b = 'c';
//     else if (b == 13)
//         b = 'd';
//     else if (b == 14)
//         b = 'e';
//     else
//         b = 'f';
//     what_color[0] = 'c';
//     what_color[1] = 'o';
//     what_color[2] = 'l';
//     what_color[3] = 'o';
//     what_color[4] = 'r';
//     what_color[5] = ' ';
//     what_color[6] = a;
//     what_color[7] = b;
// }

string input_()
{
    char key = _getch();
    return string(1, tolower(key));
}
