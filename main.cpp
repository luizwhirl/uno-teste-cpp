#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

const vector<string> cores = {"vermelho", "verde", "azul", "amarelo"};
const vector<int> numeros = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

struct Carta {
    string cor;
    int numero;
};

vector<Carta> randbaralho() {
    vector<Carta> baralho;
    for (const auto& cor : cores) {
        for (const auto& numero : numeros) {
            baralho.push_back({cor, numero});
            baralho.push_back({cor, numero});
        }
    }
    random_shuffle(baralho.begin(), baralho.end());
    return baralho;
}

bool meuturno(const Carta& carta, const Carta& topo) {
    return carta.cor == topo.cor || carta.numero == topo.numero;
}

void mostrarmao(const vector<Carta>& mao) {
    for (size_t i = 0; i < mao.size(); ++i) {
        cout << i << ": " << mao[i].cor << " " << mao[i].numero << endl;
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
        cout << "\na carta na mesa é " << topo.cor << " " << topo.numero << endl;

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
                    cout << "escolha a carta ou 'comprar' ";
                    string escolha;
                    cin >> escolha;
                    
                    if (escolha == "comprar") {
                        Carta nova_carta = baralho.back();
                        baralho.pop_back();
                        cout << "comprasse " << nova_carta.cor << " " << nova_carta.numero << endl;
                        
                        if (meuturno(nova_carta, topo)) {
                            cout << "jogou essa mermo, seloco" << endl;
                            topo = nova_carta;
                        } else {
                            mao_eu.push_back(nova_carta);
                        }
                        break;
                    } else {
                        try {
                            int escolha_num = stoi(escolha);
                            if (find(jogadas_validas.begin(), jogadas_validas.end(), escolha_num) != jogadas_validas.end()) {
                                topo = mao_eu[escolha_num];
                                cout << "você jogou " << topo.cor << " " << topo.numero << endl;
                                mao_eu.erase(mao_eu.begin() + escolha_num);
                                break;
                            } else {
                                cout << "nada a ver, joga uma carta que exista ou esteja no jogo, bobão" << endl;
                            }
                        } catch (...) {
                            cout << "wtf nada a ver" << endl;
                        }
                    }
                }
            } else {
                cout << "KKKKKKK nem carta tem, vai ter que comprar" << endl;
                Carta nova_carta = baralho.back();
                baralho.pop_back();
                cout << "tu comprasse " << nova_carta.cor << " " << nova_carta.numero << endl;
                
                if (meuturno(nova_carta, topo)) {
                    cout << "Sorte sua, jogou a carta que comprou" << endl;
                    topo = nova_carta;
                } else {
                    mao_eu.push_back(nova_carta);
                }
            }

            if (mao_eu.empty()) {
                cout << "\vvocê ganhou, mas não há mérito nenhum nisso" << endl;
                break;
            }
        } else {
            cout << "turno de PC" << endl;
            bool jogou = false;
            
            for (size_t i = 0; i < mao_pc.size(); ++i) {
                if (meuturno(mao_pc[i], topo)) {
                    topo = mao_pc[i];
                    cout << "PC jogou " << topo.cor << " " << topo.numero << endl;
                    mao_pc.erase(mao_pc.begin() + i);
                    jogou = true;
                    break;
                }
            }
            
            if (!jogou) {
                Carta nova_carta = baralho.back();
                baralho.pop_back();
                cout << "PC comprou uma carta" << endl;
                
                if (meuturno(nova_carta, topo)) {
                    topo = nova_carta;
                    cout << "PC jogou " << topo.cor << " " << topo.numero << endl;
                } else {
                    mao_pc.push_back(nova_carta);
                }
            }

            if (mao_pc.empty()) {
                cout << "\nKKKKKKK PERDEU PRA UM ROBÔ BURRO" << endl;
                break;
            }
        }

        turno = 1 - turno;
    }
}

int main() {
    jogar();
    return 0;
}