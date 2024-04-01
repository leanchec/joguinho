#include<bits/stdc++.h>

#define endl '\n'
#define LEFT_ARROW 68
#define UP_ARROW 65
#define RIGHT_ARROW 67
#define DOWN_ARROW 66

using namespace std;

string fundo0="  ", fundo1="\033[0;40m  \033[0m", parede="\033[0;47m  \033[0m", player0="\033[0;46m  \033[0m", player1="\033[0;42m  \033[0m", coletavel="\033[0;43m  \033[0m", fim="\033[0;45m  \033[0m";

int mapa[33][33]={}, dx[4]={0, 1, 0, -1}, dy[4]={1, 0, -1, 0};
bool garantia[33][33]={};
bool coletou=false, acabou=false;
pair<int,int> pos;

void move_up(){
	if(mapa[pos.first-1][pos.second]==1)return;
	if(mapa[pos.first-1][pos.second]==4){
		if(coletou){
			acabou=true;
			mapa[pos.first][pos.second]=0;
			mapa[pos.first-1][pos.second]=2;
			pos.first-=1;
		}
		return;
	}
	if(mapa[pos.first-1][pos.second]==3){
		coletou=true;
		mapa[pos.first][pos.second]=0;
		mapa[pos.first-1][pos.second]=2;
		pos.first-=1;
	}
	else{
		mapa[pos.first][pos.second]=0;
		mapa[pos.first-1][pos.second]=2;
		pos.first-=1;
	}
}

void move_down(){
	if(mapa[pos.first+1][pos.second]==1)return;
	if(mapa[pos.first+1][pos.second]==4){
		if(coletou){
			acabou=true;
			mapa[pos.first][pos.second]=0;
			mapa[pos.first+1][pos.second]=2;
			pos.first+=1;
		}
		return;
	}
	if(mapa[pos.first+1][pos.second]==3){
		coletou=true;
		mapa[pos.first][pos.second]=0;
		mapa[pos.first+1][pos.second]=2;
		pos.first+=1;
	}
	else{
		mapa[pos.first][pos.second]=0;
		mapa[pos.first+1][pos.second]=2;
		pos.first+=1;
	}
}

void move_left(){
	if(mapa[pos.first][pos.second-1]==1)return;
	if(mapa[pos.first][pos.second-1]==4){
		if(coletou){
			acabou=true;
			mapa[pos.first][pos.second]=0;
			mapa[pos.first][pos.second-1]=2;
			pos.second-=1;
		}
		return;
	}
	if(mapa[pos.first][pos.second-1]==3){
		coletou=true;
		mapa[pos.first][pos.second]=0;
		mapa[pos.first][pos.second-1]=2;
		pos.second-=1;
	}
	else{
		mapa[pos.first][pos.second]=0;
		mapa[pos.first][pos.second-1]=2;
		pos.second-=1;
	}
}

void move_right(){
	if(mapa[pos.first][pos.second+1]==1)return;
	if(mapa[pos.first][pos.second+1]==4){
		if(coletou){
			acabou=true;
			mapa[pos.first][pos.second]=0;
			mapa[pos.first][pos.second+1]=2;
			pos.second+=1;
		}
		return;
	}
	if(mapa[pos.first][pos.second+1]==3){
		coletou=true;
		mapa[pos.first][pos.second]=0;
		mapa[pos.first][pos.second+1]=2;
		pos.second+=1;
	}
	else{
		mapa[pos.first][pos.second]=0;
		mapa[pos.first][pos.second+1]=2;
		pos.second+=1;
	}
}

void imprime(){
	system("clear");

	cout << (coletou ? "va para o bloco roxo para ganhar":"colete o bloco amarelo") << endl << endl;
	for(int i=0; i<=31; i++){
		for(int j=0; j<=31; j++){
			if(mapa[i][j]==0){
				if((i+j)%2)cout << fundo1;
				else cout << fundo0;
			}
			else if(mapa[i][j]==1)cout << parede;
			else if(mapa[i][j]==2){
				if(coletou)cout << player1;
				else cout << player0;
			}
			else if(mapa[i][j]==3)cout << coletavel;
			else cout << fim;
		}
		cout << endl;
	}
	cout << endl;
}

void caminho(pair<int,int> ini, pair<int,int> obj){
	pair<int,int> pai[33][33];

	for(int i=1; i<=30; i++)
		for(int j=1; j<=30; j++)
			pai[i][j]=make_pair(-1, -1);

	pai[ini.first][ini.second]=make_pair(0, 0);

	queue<pair<int,int>> q;
	q.push(ini);

	while(q.size()){
		int cx=q.front().first, cy=q.front().second;
		q.pop();

		if(cx==obj.first && cy==obj.second)break;

		for(int p=0; p<4; p++){
			int nx=cx+dx[p], ny=cy+dy[p];

			if(pai[nx][ny].first!=-1 || (mapa[nx][ny]!=0 && (nx!=obj.first || ny!=obj.second)))continue;

			pai[nx][ny]=make_pair(cx, cy);
			q.push({nx, ny});
		}
	}

	garantia[obj.first][obj.second]=true;


	while(obj!=ini){
		obj=pai[obj.first][obj.second];
		garantia[obj.first][obj.second]=true;
	}
}

void sane() {
	system("stty cooked echo");
}

void insane() {
	system("stty raw -echo");
}

int main(){
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	atexit(sane);
	for (uint8_t i = 1; true; i++){
		
		memset(mapa, 0, sizeof mapa);
		memset(garantia, 0, sizeof garantia);
		coletou = false, acabou = false;


		for(int i=0; i<=31; i++){
			mapa[0][i]=1;
			mapa[i][0]=1;
			mapa[31][i]=1;
			mapa[i][31]=1;
		}


		vector<pair<int,int>> pontos;

		for(int i=1; i<31; i++)
			for(int j=1; j<31; j++)
				pontos.push_back({i, j});

		shuffle(pontos.begin(), pontos.end(), rng);

		mapa[pontos[0].first][pontos[0].second]=2;
		pos=pontos[0];
		mapa[pontos[1].first][pontos[1].second]=3;
		mapa[pontos[2].first][pontos[2].second]=4;


		caminho(pontos[0], pontos[1]);
		caminho(pontos[0], pontos[2]);

		int qtd=350, ptr=3;

		while(qtd--){
			while(garantia[pontos[ptr].first][pontos[ptr].second])ptr++;
			mapa[pontos[ptr].first][pontos[ptr].second]=1;
			ptr++;
		}

		cout << "\ncolete o bloco marrom/amarelo e va para o bloco roxo para ganhar" << endl;
		cout << "mova-se com >, <, v, ^" << endl;
		cout << "escrava uma string com uma quantia qualquer desses caracteres para se movimentar" << endl;
		cout << "outros caracteres serao ignorados" << endl;
		cout << "apos uma sequencia de movimentos o mapa sera impresso" << endl << endl;

		imprime();
		cout << "Round:\t" << (int)i << endl;

		while(!acabou){
			string movimentos;

			insane();
			char inp = (char) cin.get();
			sane();

			if (inp == 3) {
				return 1;
			}

			if(inp == UP_ARROW || inp == 'w') move_up();
			else if(inp == LEFT_ARROW || inp == 'a') move_left();
			else if(inp == RIGHT_ARROW || inp == 'd') move_right();
			else if(inp == DOWN_ARROW || inp == 's') move_down();
		
			imprime();
			cout << "Round:\t" << (int)i << endl;

		}

	}
	
	return 0;
}