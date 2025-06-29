#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <locale>
#include <windows.h>
#include <random>

using namespace std;

const vector<string> cores = {"🟥 vermelho", "🟩 verde", "🟦 azul", "🟨 amarelo"};
const vector<int> numeros = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

struct Carta {
    string cor;
    int numero; // -1 = +2, -2 = reverse, -3 = +4, -4 = bloquear, -5 = mudar cor
};

bool cinOK() {
    if (cin.eof() || cin.fail()) {
        cout << "\ntchaudisgrasa" << endl;
        return false;
    }
    return true;
}


bool lerEntrada(string& entrada) {
    if (!getline(cin, entrada)) {
        if (cin.eof()) {
            cout << "\nPrograma interrompido. Tchau!" << endl;
            return false;
        }
    }
    return true;
}

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
        // cartas reverse
        baralho.push_back({cor, -2});
        baralho.push_back({cor, -2});
        // cartas bloquear
        baralho.push_back({cor, -4});
        baralho.push_back({cor, -4});
    }
    for (int i = 0; i < 4; ++i) {
        baralho.push_back({"⚫ preta", -3});
    }
    for (int i = 0; i < 4; ++i) {
        baralho.push_back({"⚫ preta", -5});
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(baralho.begin(), baralho.end(), g);

    return baralho;
}

bool meuturno(const Carta& carta, const Carta& topo) {
    if (carta.numero == -3 || carta.numero == -5) return true; // +4 e mudar cor sempre podem ser jogados
    return carta.cor == topo.cor || carta.numero == topo.numero;
}

string escolherCor() {
    cout << "escolha uma cor:" << endl;
    cout << "1: 🟥 vermelho" << endl;
    cout << "2: 🟩 verde" << endl;
    cout << "3: 🟦 azul" << endl;
    cout << "4: 🟨 amarelo" << endl;
    
    while (true) {
        string escolha;
        if (!lerEntrada(escolha)) {
            exit(0);
        }
        
        try {
            int cor_num = stoi(escolha);
            if (cor_num >= 1 && cor_num <= 4) {
                return cores[cor_num - 1];
            } else {
                cout << "escolha de 1 a 4" << endl;
            }
        } catch (...) {
            cout << "bote um número, deixe de ser lerdo" << endl;
        }
    }
}

string escolherCorPC() {
    return cores[rand() % 4];
}

void mostrarmao(const vector<Carta>& mao) {
    cout << "0: Comprar carta" << endl;
    for (size_t i = 0; i < mao.size(); ++i) {
        string desc;
        if (mao[i].numero == -1) desc = "+2";
        else if (mao[i].numero == -2) desc = "reverse";
        else if (mao[i].numero == -3) desc = "+4";
        else if (mao[i].numero == -4) desc = "bloquear";
        else if (mao[i].numero == -5) desc = "mudar cor";
        else desc = to_string(mao[i].numero);
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
    
    // se caso a primeira carta for uma especial, vai pegar ota
    while (topo.numero < 0) {
        baralho.insert(baralho.begin(), topo);
        random_shuffle(baralho.begin(), baralho.end());
        topo = baralho.back();
        baralho.pop_back();
    }

    int turno = 0; // 0 = eu, 1 = pc
    int direcao = 1; // 1 = normal, -1 = reverso (em jogo de 2 não muda nada, mas fica aí)

    while (true) {
        cout << "A primeira carta na mesa é: " << topo.cor << " ";
        if (topo.numero == -1) cout << "+2" << endl;
        else if (topo.numero == -2) cout << "reverse" << endl;
        else if (topo.numero == -3) cout << "+4" << endl;
        else if (topo.numero == -4) cout << "bloquear" << endl;
        else if (topo.numero == -5) cout << "mudar cor" << endl;
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
                    if (!lerEntrada(escolha)) {
                        return;
                    }

                    try {
                        int escolha_num = stoi(escolha);
                        if (escolha_num == 0) {
                            if (!baralho.empty()) {
                                Carta nova_carta = baralho.back();
                                baralho.pop_back();
                                cout << "comprasse " << nova_carta.cor << " ";
                                if (nova_carta.numero == -1) cout << "+2" << endl;
                                else if (nova_carta.numero == -2) cout << "reverse" << endl;
                                else if (nova_carta.numero == -3) cout << "+4" << endl;
                                else if (nova_carta.numero == -4) cout << "bloquear" << endl;
                                else if (nova_carta.numero == -5) cout << "mudar cor" << endl;
                                else cout << nova_carta.numero << endl;

                                if (meuturno(nova_carta, topo)) {
                                    cout << "jogou essa mermo, seloco" << endl;
                                    topo = nova_carta;
                                    
                                    if (topo.numero == -1) {
                                        cout << "PC compra 2 e se fodeeee" << endl;
                                        for (int i = 0; i < 2 && !baralho.empty(); ++i) {
                                            mao_pc.push_back(baralho.back());
                                            baralho.pop_back();
                                        }
                                        turno = 0;
                                    } else if (topo.numero == -2) {
                                        cout << "reverse! mas como é só 2 jogadores, continua sendo seu turno kkk" << endl;
                                        turno = 0;
                                    } else if (topo.numero == -3) {
                                        topo.cor = escolherCor();
                                        cout << "mudou pra " << topo.cor << endl;
                                        cout << "PC compra 4 cartas e se fode" << endl;
                                        for (int i = 0; i < 4 && !baralho.empty(); ++i) {
                                            mao_pc.push_back(baralho.back());
                                            baralho.pop_back();
                                        }
                                        turno = 0;
                                    } else if (topo.numero == -4) {
                                        cout << "PC foi bloqueado! joga de novo" << endl;
                                        turno = 0;
                                    } else if (topo.numero == -5) {
                                        topo.cor = escolherCor();
                                        cout << "mudou pra " << topo.cor << endl;
                                        turno = 1;
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
                                else if (topo.numero == -2) cout << "reverse" << endl;
                                else if (topo.numero == -3) cout << "+4" << endl;
                                else if (topo.numero == -4) cout << "bloquear" << endl;
                                else if (topo.numero == -5) cout << "mudar cor" << endl;
                                else cout << topo.numero << endl;
                                mao_eu.erase(mao_eu.begin() + index);

                                if (topo.numero == -1) {
                                    cout << "PC vai comprar 2 cartas e perder a vez kkkkk" << endl;
                                    for (int i = 0; i < 2 && !baralho.empty(); ++i) {
                                        mao_pc.push_back(baralho.back());
                                        baralho.pop_back();
                                    }
                                    turno = 0;
                                } else if (topo.numero == -2) {
                                    cout << "reverse! mas como é só vocês dois, continua sendo seu turno" << endl;
                                    turno = 0;
                                } else if (topo.numero == -3) {
                                    topo.cor = escolherCor();
                                    cout << "mudou pra " << topo.cor << endl;
                                    cout << "PC compra 4 cartas e perde a vez" << endl;
                                    for (int i = 0; i < 4 && !baralho.empty(); ++i) {
                                        mao_pc.push_back(baralho.back());
                                        baralho.pop_back();
                                    }
                                    turno = 0;
                                } else if (topo.numero == -4) {
                                    cout << "PC bloqueado! você joga de novo kkkkk" << endl;
                                    turno = 0;
                                } else if (topo.numero == -5) {
                                    topo.cor = escolherCor();
                                    cout << "mudou pra " << topo.cor << endl;
                                    turno = 1;
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
                    else if (nova_carta.numero == -2) cout << "reverse" << endl;
                    else if (nova_carta.numero == -3) cout << "+4" << endl;
                    else if (nova_carta.numero == -4) cout << "bloquear" << endl;
                    else if (nova_carta.numero == -5) cout << "mudar cor" << endl;
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
                        } else if (topo.numero == -2) {
                            cout << "reverse! joga de novo besta" << endl;
                            turno = 0;
                        } else if (topo.numero == -3) {
                            topo.cor = escolherCor();
                            cout << "mudou pra " << topo.cor << endl;
                            cout << "PC compra 4 e se fode" << endl;
                            for (int i = 0; i < 4 && !baralho.empty(); ++i) {
                                mao_pc.push_back(baralho.back());
                                baralho.pop_back();
                            }
                            turno = 0;
                        } else if (topo.numero == -4) {
                            cout << "PC bloqueado, sua vez de novo" << endl;
                            turno = 0;
                        } else if (topo.numero == -5) {
                            topo.cor = escolherCor();
                            cout << "mudou pra " << topo.cor << endl;
                            turno = 1;
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
                    else if (topo.numero == -2) cout << "reverse" << endl;
                    else if (topo.numero == -3) cout << "+4" << endl;
                    else if (topo.numero == -4) cout << "bloquear" << endl;
                    else if (topo.numero == -5) cout << "mudar cor" << endl;
                    else cout << topo.numero << endl;
                    mao_pc.erase(mao_pc.begin() + i);
                    jogou = true;

                    if (topo.numero == -1) {
                        cout << "você compra 2 e perde a vez" << endl;
                        for (int j = 0; j < 2 && !baralho.empty(); ++j) {
                            mao_eu.push_back(baralho.back());
                            baralho.pop_back();
                        }
                        turno = 1;
                    } else if (topo.numero == -2) {
                        cout << "PC jogou reverse! continua sendo a vez dele" << endl;
                        turno = 1;
                    } else if (topo.numero == -3) {
                        topo.cor = escolherCorPC();
                        cout << "PC mudou a cor pra " << topo.cor << endl;
                        cout << "você compra 4 cartas e perde a vez" << endl;
                        for (int j = 0; j < 4 && !baralho.empty(); ++j) {
                            mao_eu.push_back(baralho.back());
                            baralho.pop_back();
                        }
                        turno = 1;
                    } else if (topo.numero == -4) {
                        cout << "você foi bloqueado! PC joga de novo" << endl;
                        turno = 1;
                    } else if (topo.numero == -5) {
                        topo.cor = escolherCorPC();
                        cout << "PC mudou a cor pra " << topo.cor << endl;
                        turno = 0;
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
                    else if (topo.numero == -2) cout << "reverse" << endl;
                    else if (topo.numero == -3) cout << "+4" << endl;
                    else if (topo.numero == -4) cout << "bloquear" << endl;
                    else if (topo.numero == -5) cout << "mudar cor" << endl;
                    else cout << topo.numero << endl;
                    
                    if (topo.numero == -1) {
                        cout << "você compra 2 e perde a vez" << endl;
                        for (int j = 0; j < 2 && !baralho.empty(); ++j) {
                            mao_eu.push_back(baralho.back());
                            baralho.pop_back();
                        }
                        turno = 1;
                    } else if (topo.numero == -2) {
                        cout << "PC jogou reverse, vez dele dinovo" << endl;
                        turno = 1;
                    } else if (topo.numero == -3) {
                        topo.cor = escolherCorPC();
                        cout << "PC mudou a cor pra " << topo.cor << endl;
                        cout << "você compra 4 e perde a vez" << endl;
                        for (int j = 0; j < 4 && !baralho.empty(); ++j) {
                            mao_eu.push_back(baralho.back());
                            baralho.pop_back();
                        }
                        turno = 1;
                    } else if (topo.numero == -4) {
                        cout << "você foi bloqueado, PC joga de novo" << endl;
                        turno = 1;
                    } else if (topo.numero == -5) {
                        topo.cor = escolherCorPC();
                        cout << "PC mudou a cor pra " << topo.cor << endl;
                        turno = 0;
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
    SetConsoleOutputCP(65001); 
    setlocale(LC_ALL, "");

    jogar();
    return 0;
}
