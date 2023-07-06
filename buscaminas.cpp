//made from scratch by Picklin, beta version finished on 06/06/2023
#include <iostream>
#include <vector>

using namespace std;

//se utiliza un doble caracter por posicion
//booleano para el algoritmo de busqueda
struct casilla {
    char debajo;
    char encima;
    bool visitado;
};

typedef vector<vector<casilla> > matrix;

//variables globales
pair<int, int> r = {0,1};
pair<int, int> tr = {-1,1};
pair<int, int> t = {-1,0};
pair<int, int> tl = {-1,-1};
pair<int, int> l = {0,-1};
pair<int, int> bl = {1,-1};
pair<int, int> b = {1,0};
pair<int, int> br = {1,1};
pair<int,int> dir[8] = {r, tr, t, tl, l, bl, b, br};

int num_bombs = 0;
int num_no_bombs;
bool lost;

void welcome() {
    cout << "\t-----BUSCAMINAS----- \n\tmade by Picklin\n" << endl;
    cout << "Seleccione la dificultad:" << endl;
    cout << "\t-Pulse 1 para: 8x8" << endl << "\t-Pulse 2 para: 16x16" << endl << "\t-Pulse 3 para: 20x20" << endl;
    cout << "Pulse 4 para salir" << endl;
}

void help() {
    cout << "Tiene 3 opciones:" << endl;
    cout << "\t-Escriba 's' y las coordenadas para seleccionar las casillas a eliminar (fila columna)" << endl;
    cout << "\t-Escriba 'f' y las coordenadas para marcar las casillas donde puede haber una mina (fila columna)" << endl;
    cout << "\t-Para dejar de escribir coordenadas, escriba 2 caracteres que no sean una coordenada (ej: '. .')" << endl;
    cout << "\t-Escriba 'exit' para salir" << endl;
}

void Usage() {
    cout << "Usage: Option (num 1 to 4)" << endl;
    exit(1);
}

bool initialize(matrix& mapa) {
    int option;
    cin >> option;
    bool initialized;
    if (option > 4 or option < 1) Usage();
    if (option != 4) {
        initialized = true;
        int difficulty[3] = {8, 16, 20};
        int size = difficulty[option-1];
        mapa = matrix(size, vector<casilla>(size));
        //poner bombas
        srand(time(nullptr));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                mapa[i][j].encima = 'X';
                mapa[i][j].visitado = false;
                //chance: 1/8
                int bomb_pos = rand() % 8;
                if (bomb_pos == 0) {
                    mapa[i][j].debajo = 'B';
                    ++num_bombs;
                }
            }
        }
        num_no_bombs = size*size - num_bombs;
        //poner numeros en las casillas
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (mapa[i][j].debajo != 'B') {
                    int n_bomb_ady = 0;
                    for (auto & k : dir) {
                        pair<int, int> pos = {i+k.first, j+k.second};
                        if (pos.first >= 0 and pos.first < size and pos.second >= 0 and pos.second < size) {
                            if (mapa[pos.first][pos.second].debajo == 'B') ++n_bomb_ady;
                        }
                    }
                    mapa[i][j].debajo = '0' + n_bomb_ady;
                    if (mapa[i][j].debajo == '0') mapa[i][j].debajo = ' ';
                }
            }
        }
    }
    else initialized = false;
    return initialized;
}

void print_result(matrix& mapa) {
    int size = int(mapa.size());
    cout << "  ";
    if (size == 8) {
        for (int i = 1; i < size; ++i) cout << i << ' ';
        cout << size << endl;
        for (int i = 0; i < size; ++i) {
            cout << i+1 << ' ';
            for (int j = 0; j < size-1; ++j) {
                cout << mapa[i][j].encima << ' ';
            }
            cout << mapa[i][size-1].encima << endl;
        }
    }
    else {
        for (int i = 1; i < 10; ++i) cout << i << ' ';
        for (int i = 0; i < size-10; ++i) cout << char('a' + i) << ' ';
        cout << char('a' + size-10) << endl;
        for (int i = 0; i < 9; ++i) {
            cout << i+1 << ' ';
            for (int j = 0; j < size-1; ++j) {
                cout << mapa[i][j].encima << ' ';
            }
            cout << mapa[i][size-1].encima << endl;
        }
        for (int i = 9; i < size; ++i) {
            cout << char('a' + i - 9) << ' ';
            for (int j = 0; j < size-1; ++j) {
                cout << mapa[i][j].encima << ' ';
            }
            cout << mapa[i][size-1].encima << endl;
        }
    }
}

void dfs_selection(pair<int, int>& pos, matrix& mapa) {
    if (not mapa[pos.first][pos.second].visitado) {
        mapa[pos.first][pos.second].encima = mapa[pos.first][pos.second].debajo;
        mapa[pos.first][pos.second].visitado = true;
        --num_no_bombs;
        if (mapa[pos.first][pos.second].debajo == 'B') {
            lost = true;
            return;
        }
        if (mapa[pos.first][pos.second].debajo == ' ') {
            int size = int(mapa.size());
            for (auto i: dir) {
                pair<int, int> pos2 = {pos.first + i.first, pos.second + i.second};
                if (pos2.first >= 0 and pos2.first < size and pos2.second >= 0 and pos2.second < size)
                    dfs_selection(pos2, mapa);
            }
        }

    }
}

//result status: 1 win, 2 loose, 3 exit
int rounds(matrix& mapa) {
    cout << "\nPara mas info escriba 'help'" << endl;
    print_result(mapa);
    int result;
    string option;
    while (not lost and num_no_bombs > 0 and cin >> option and option != "exit" ) {
        if (option == "help") {
            help();
        }
        else if (option == "s" or option == "f"){
            pair<char, char> pos;
            while (cin >> pos.first >> pos.second and ((pos.first >= '1' and pos.first <= '9' or (mapa.size() > 8 and pos.first >= 'a' and pos.first <= 'k'))
                   and (pos.second >= '1' and pos.second <= '9' or (mapa.size() > 8 and pos.second >= 'a' and pos.second <= 'k')))) {
                pair<int, int> pos2;
                //pasar el caracter a posicion
                if (pos.first >= 'a' and pos.first <= 'k') pos2.first = pos.first - 'a' + 9;
                else pos2.first = pos.first - 1 - '0';
                if (pos.second >= 'a' and pos.second <= 'k') pos2.second = pos.second - 'a' + 9;
                else pos2.second = pos.second - 1 - '0';

                //algoritmo casillas a eliminar
                if (option == "s") {
                    dfs_selection(pos2, mapa);
                }
                //casillas a marcar
                else {
                    if (mapa[pos2.first][pos2.second].encima == 'X') mapa[pos2.first][pos2.second].encima = 'F';
                    else mapa[pos2.first][pos2.second].encima = 'X';
                }
            }
            print_result(mapa);
        }
        else cout << "Comando incorrecto" << endl;
    }
    if (lost) result = 2;
    else if (num_no_bombs == 0) result = 1;
    else result = 3;
    return result;
}

int main() {
    welcome();
    matrix mapa;
    if (initialize(mapa)) {
        int result = rounds(mapa);
        if (result == 1) cout << "\n¡Enhorabuena, has ganado!" << endl;
        else if (result == 2) cout << "\n¡BOOM! Vaya, has perdido..." << endl;
    }
}
