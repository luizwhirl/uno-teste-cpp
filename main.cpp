#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <locale>
#include <windows.h>

using namespace std;

const vector<string> cores = {"🟥 vermelho", "🟩 verde", "🟦 azul", "🟨 amarelo"};
const vector<int> numeros = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

struct Carta {
    string cor;
    int numero; // -1 == carta +2
};

vector<Carta> randbaralho() {
    vector<Carta> baralho;
    for (const auto& cor : cores) {
        for (const auto& numero : numeros) {
            baralho.push_back({cor, numero});
            baralho.push_back({cor, numero});
        }
        // cartas +2
        baralho.push_back({cor, -1});
        baralho.push_back({cor, -1});
    }
    random_shuffle(baralho.begin(), baralho.end());
    return baralho;
}

bool meuturno(const Carta& carta, const Carta& topo) {
    return carta.cor == topo.cor || carta.numero == topo.numero || carta.numero == -1;
}

void mostrarmao(const vector<Carta>& mao) {
    cout << "0: Comprar carta" << endl;
    for (size_t i = 0; i < mao.size(); ++i) {
        string desc = (mao[i].numero == -1) ? "+2" : to_string(mao[i].numero);
        cout << (i + 1) << ": " << mao[i].cor << " " << desc << endl;
    }
}

void jogar() {
    srand(time(0));
    auto baralho = randbaralho();

    vector<Carta> mao_eu;
    vector<Carta> mao_pc;

    for (int i = 0; i < 7; ++i) {
        mao_eu.push_back(baralho.back());
        baralho.pop_back();
        mao_pc.push_back(baralho.back());
        baralho.pop_back();
    }

    Carta topo = baralho.back();
    baralho.pop_back();

    int turno = 0; // 0 = eu, 1 = pc

    while (true) {
        cout << "\na carta na mesa é " << topo.cor << " ";
        if (topo.numero == -1) cout << "+2" << endl;
        else cout << topo.numero << endl;

        if (turno == 0) {
            cout << "\nseu turno" << endl;
            mostrarmao(mao_eu);
            vector<int> jogadas_validas;

            for (size_t i = 0; i < mao_eu.size(); ++i) {
                if (meuturno(mao_eu[i], topo)) {
                    jogadas_validas.push_back(i);
                }
            }

            if (!jogadas_validas.empty()) {
                while (true) {
                    string escolha;
                    cin >> escolha;

                    try {
                        int escolha_num = stoi(escolha);
                        if (escolha_num == 0) {
                            if (!baralho.empty()) {
                                Carta nova_carta = baralho.back();
                                baralho.pop_back();
                                cout << "comprasse " << nova_carta.cor << " ";
                                if (nova_carta.numero == -1) cout << "+2" << endl;
                                else cout << nova_carta.numero << endl;

                                if (meuturno(nova_carta, topo)) {
                                    cout << "jogou essa mermo, seloco" << endl;
                                    topo = nova_carta;
                                    if (topo.numero == -1) {
                                        cout << "Pc compra e se fodeeee" << endl;
                                        for (int i = 0; i < 2 && !baralho.empty(); ++i) {
                                            mao_pc.push_back(baralho.back());
                                            baralho.pop_back();
                                        }
                                        turno = 0; // volta a ser o meu turno
                                    } else {
                                        turno = 1;
                                    }
                                } else {
                                    mao_eu.push_back(nova_carta);
                                    turno = 1;
                                }
                            } else {
                                cout << "nao dá pra comprar: sem baralho" << endl;
                                turno = 1;
                            }
                            break;
                        } else if (escolha_num >= 1 && escolha_num <= (int)mao_eu.size()) {
                            int index = escolha_num - 1;
                            if (meuturno(mao_eu[index], topo)) {
                                topo = mao_eu[index];
                                cout << "você jogou " << topo.cor << " ";
                                if (topo.numero == -1) cout << "+2" << endl;
                                else cout << topo.numero << endl;
                                mao_eu.erase(mao_eu.begin() + index);

                                if (topo.numero == -1) {
                                    cout << "PC vai comprar 2 cartas e perder a vez kkkkk" << endl;
                                    for (int i = 0; i < 2 && !baralho.empty(); ++i) {
                                        mao_pc.push_back(baralho.back());
                                        baralho.pop_back();
                                    }
                                    turno = 0; // volta a ser meu turno
                                } else {
                                    turno = 1;
                                }
                                break;
                            } else {
                                cout << "essa carta nem bate com a mesa, zé" << endl;
                            }
                        } else {
                            cout << "isso não deveria acontecer" << endl;
                        }
                    } catch (...) {
                        cout << "bote um negócio certo, bobão" << endl;
                    }
                }
            } else {
                cout << "KKKKKKK nem carta tem, vai ter que comprar" << endl;
                if (!baralho.empty()) {
                    Carta nova_carta = baralho.back();
                    baralho.pop_back();
                    cout << "tu comprasse " << nova_carta.cor << " ";
                    if (nova_carta.numero == -1) cout << "+2" << endl;
                    else cout << nova_carta.numero << endl;

                    if (meuturno(nova_carta, topo)) {
                        cout << "sorte sua, jogou a carta que comprou" << endl;
                        topo = nova_carta;
                        if (topo.numero == -1) {
                            cout << "PC compra 2 e se fode" << endl;
                            for (int i = 0; i < 2 && !baralho.empty(); ++i) {
                                mao_pc.push_back(baralho.back());
                                baralho.pop_back();
                            }
                            turno = 0;
                        } else {
                            turno = 1;
                        }
                    } else {
                        mao_eu.push_back(nova_carta);
                        turno = 1;
                    }
                } else {
                    cout << "acabou tudo o baralho kkk" << endl;
                    turno = 1;
                }
            }

            if (mao_eu.empty()) {
                cout << "\nvocê ganhou, mas não há mérito nenhum nisso" << endl;
                break;
            }
        } else {
            cout << "turno de PC" << endl;
            bool jogou = false;

            for (size_t i = 0; i < mao_pc.size(); ++i) {
                if (meuturno(mao_pc[i], topo)) {
                    topo = mao_pc[i];
                    cout << "PC jogou " << topo.cor << " ";
                    if (topo.numero == -1) cout << "+2" << endl;
                    else cout << topo.numero << endl;
                    mao_pc.erase(mao_pc.begin() + i);
                    jogou = true;

                    if (topo.numero == -1) {
                        cout << "você compra 2 e perde a vez" << endl;
                        for (int j = 0; j < 2 && !baralho.empty(); ++j) {
                            mao_eu.push_back(baralho.back());
                            baralho.pop_back();
                        }
                        turno = 1; // lá vai esse corno jogar de novo, parece até que o jogo é dele
                    } else {
                        turno = 0;
                    }
                    break;
                }
            }

            if (!jogou && !baralho.empty()) {
                Carta nova_carta = baralho.back();
                baralho.pop_back();
                cout << "PC comprou uma carta" << endl;

                if (meuturno(nova_carta, topo)) {
                    topo = nova_carta;
                    cout << "PC jogou " << topo.cor << " ";
                    if (topo.numero == -1) cout << "+2" << endl;
                    else cout << topo.numero << endl;
                    // sendo +2
                    if (topo.numero == -1) {
                        cout << "você compra 2 e perde a vez" << endl;
                        for (int j = 0; j < 2 && !baralho.empty(); ++j) {
                            mao_eu.push_back(baralho.back());
                            baralho.pop_back();
                        }
                        turno = 1;
                    } else {
                        turno = 0;
                    }
                } else {
                    mao_pc.push_back(nova_carta);
                    turno = 0;
                }
            } else if (!jogou && baralho.empty()) {
                cout << "acabou o baralho, PC não pode comprar" << endl;
                turno = 0;
            }

            if (mao_pc.empty()) {
                cout << "\nKKKKKKK PERDEU PRA UM ROBÔ BURRO" << endl;
                break;
            }
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    setlocale(LC_ALL, "pt_BR.utf8"); 
    jogar();
    return 0;
}
