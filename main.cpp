//#include <bits/stdc++.h>
#include <random>
#include <chrono>
#include <map>
#include <iostream>
#include <queue>
#include <cstring>
#include <vector>
#include <algorithm>

#define LEFT_ARROW 68
#define UP_ARROW 65
#define RIGHT_ARROW 67
#define DOWN_ARROW 66

#define fundo0 "  "
#define fundo1 "\033[0;40m  \033[0m"
#define parede "\033[0;47m  \033[0m"
#define player0 "\033[48;2;80;200;200m  \033[0m"
#define player1 "\033[0;42m  \033[0m"
#define coletavel "\033[48;2;242;238;2m  \033[0m"
#define fim "\033[48;2;180;20;255m  \033[0m"

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

std::map<char, int> dictx = {{UP_ARROW, -1}, {DOWN_ARROW, +1}, {'w', -1}, {'s', +1}};
std::map<char, int> dicty = {{LEFT_ARROW, -1}, {RIGHT_ARROW, +1}, {'a', -1}, {'d', +1}};

int dx[4]={0, 1, 0, -1}, dy[4]={1, 0, -1, 0};

int mapa[33][153];

bool garantia[33][153];

bool coletou, acabou;

int columns;

std::pair<int,int> pos;

int16_t getTerminalSize() {
    std::string command = "stty size | cut -d' ' -f2";
    FILE* fpipe = popen(command.c_str(), "r");

	char buffer[128];

    int a = atoi(fgets(buffer, 128, fpipe));

    pclose(fpipe);
    return a;
}

void move(int ox, int oy){
	if(mapa[ox][oy] == 1 || (mapa[ox][oy] == 4 && !coletou))return;
	else if(mapa[ox][oy] == 4)acabou=true;
	else if(mapa[ox][oy] == 3)coletou=true;

	mapa[pos.first][pos.second] = 0;
	mapa[ox][oy] = 2;
	pos=std::make_pair(ox, oy);
}

void imprime(){
	system("clear");

	std::cout << (coletou ? "va para o bloco roxo para ganhar" : "colete o bloco amarelo") << "\n\n";

	for(int i = 0; i <= 31; i++){
		for(int j = 0; j <= columns+1; j++){
			if(mapa[i][j] == 0) {
				std::cout << (((i+j)%2) ? fundo1 : fundo0);
			}
			else if(mapa[i][j] == 1) {
				std::cout << parede;
			}
			else if(mapa[i][j] == 2) {
				std::cout << (coletou ? player1 : player0);
			}
			else if(mapa[i][j] == 3) {
				std::cout << coletavel;
			}
			else {
				std::cout << fim;
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void caminho(std::pair<int,int> ini, std::pair<int,int> obj){
	std::pair<int,int> pai[33][153];

	for(int i = 1; i <= 30; i++)
		for(int j = 1; j <= columns; j++)
			pai[i][j] = std::make_pair(-1, -1);

	pai[ini.first][ini.second] = std::make_pair(0, 0);

	std::queue<std::pair<int,int>> q;
	q.push(ini);

	while(q.size()){
		int cx = q.front().first, cy = q.front().second;
		q.pop();

		if(cx == obj.first && cy == obj.second)break;

		for(int p = 0; p < 4; p++){
			int nx = cx+dx[p], ny = cy+dy[p];

			if(pai[nx][ny].first != -1 || (mapa[nx][ny] != 0 && (nx != obj.first || ny != obj.second)))continue;

			pai[nx][ny] = std::make_pair(cx, cy);
			q.push({nx, ny});
		}
	}

	garantia[obj.first][obj.second] = true;


	while(obj != ini){
		obj = pai[obj.first][obj.second];
		garantia[obj.first][obj.second] = true;
	}
}

char keyboard_input(){
	system("stty raw -echo");

	char inp = (char) std::cin.get();

	system("stty cooked echo");

	return inp;
}

void gerar_mapa(){
	memset(mapa, 0, sizeof mapa);
	memset(garantia, 0, sizeof garantia);
	coletou = false, acabou = false;


	for(int i = 0; i <= 31; i++){
		mapa[i][0] = 1;
		mapa[i][columns+1] = 1;
	}
	for(int j = 0; j <= columns+1; j++){
		mapa[0][j] = 1;
		mapa[31][j] = 1;
	}


	std::vector<std::pair<int,int>> pontos;

	for(int i = 1; i < 31; i++)
		for(int j = 1; j <= columns; j++)
			pontos.push_back({i, j});

	std::shuffle(pontos.begin(), pontos.end(), rng);

	mapa[pontos[0].first][pontos[0].second] = 2;
	mapa[pontos[1].first][pontos[1].second] = 3;
	mapa[pontos[2].first][pontos[2].second] = 4;

	pos = pontos[0];

	caminho(pontos[0], pontos[1]);
	caminho(pontos[0], pontos[2]);

	int qtd = (int)(12*columns), ptr = 3;

	while(qtd--){
		while(garantia[pontos[ptr].first][pontos[ptr].second])ptr++;
		
		mapa[pontos[ptr].first][pontos[ptr].second] = 1;
		ptr++;
	}
}

int main(){
	for (uint8_t i = 1; true; i++){
		columns = std::min(getTerminalSize()/2-2, 150);
		columns = std::max(columns, 4);

		gerar_mapa();
		
		imprime();
		std::cout << "\033[31mRound:\t" << (int)i << "\033[0m" << '\n';

		while(!acabou){
			char inp = keyboard_input();

			if (inp == 3) {
				return 0;
			}

			int nx = pos.first + dictx[inp], ny = pos.second + dicty[inp];

			move(nx, ny);
		
			imprime();
			std::cout << "\033[31mRound:\t" << (int)i << "\033[0m" << '\n';

		}

	}
	
	return 0;
}
