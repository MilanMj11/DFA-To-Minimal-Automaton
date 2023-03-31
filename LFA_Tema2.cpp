#include <bits/stdc++.h>

using namespace std;

ifstream fin("automat.txt");
ifstream fin_aux("automat.txt");
ifstream f_input("input.txt");
ofstream fout("output.txt");

const int NMAX = 10005;
vector< pair<int,char> > graf[NMAX]; /// v[i] = { vecin , muchie }
map<string,int> conversie; /// conversie[node] = indicele
string conv[NMAX]; /// conv[indice] = node;
string stari_finale[NMAX];
string stare_initiala = "";
int cnt_stari_finale = 0;
int cnt_nodes = 0;
int dim_sol = 0, dim_drum = 0;
string sol[NMAX],drum[NMAX];

void procesare_linie(string s){
    string node1 = "",node2 = "";
    char muchie;
    int ind = 0;
    while(s[ind]!=' '){
        node1 = node1 + s[ind];
        ind++;
    }
    ind++;
    while(s[ind]!=' '){
        muchie = s[ind];
        ind++;
    }
    ind++;
    int dim = s.size();
    while(ind!=dim){
        node2 = node2 + s[ind];
        ind++;
    }
//    fout << node1 << '\n' << muchie << '\n' << node2 << '\n';
    if(conversie.find(node1) == conversie.end()){
        conversie.insert(make_pair(node1,++cnt_nodes));
        conv[cnt_nodes] = node1;
    }
    if(conversie.find(node2) == conversie.end()){
        conversie.insert(make_pair(node2,++cnt_nodes));
        conv[cnt_nodes] = node2;
    }
    graf[conversie[node1]].push_back(make_pair(conversie[node2],muchie));
}

void procesare_stari_finale(string s){
    string nod = "";
    int ind = 0;
    int dim = s.size();
    while(ind!=dim){
        if(s[ind]==' '){
            stari_finale[++cnt_stari_finale] = nod;
            nod = "";
            ind++;
        }
        nod = nod + s[ind];
        ind++;
    }
    stari_finale[++cnt_stari_finale] = nod;
//    fout << cnt_stari_finale << '\n';
//    for(int i=1;i<=cnt_stari_finale;i++){
//        fout << stari_finale[i] << '\n';
//    }
}

void citire(){
    /// atentie sa nu pun randuri goale in plus in input
    getline(fin_aux,stare_initiala);
    getline(fin,stare_initiala);
    //fout << stare_initiala << '\n';
    string s;
    int nr_linii = 0;
    while(!fin_aux.eof()){
        getline(fin_aux,s);
        if(s.size()==0) continue;
        nr_linii++;
        //fout << s << '\n';
    }
    int aux = 0;
    while(!fin.eof()){
        getline(fin,s);
        aux++;
        if(aux>nr_linii) break;
        if(aux == nr_linii){
            procesare_stari_finale(s);
        } else
            procesare_linie(s);
    }
}

bool acceptat = false;

void bfs(string cuvant){
    vector <int> vdrum;
    vdrum.clear();
    queue < pair< vector<int> , pair<string,int> > > q;
    q.push({vdrum,make_pair(cuvant,conversie[stare_initiala])});
    bool acceptat = false;
    while(!q.empty()){
        pair< vector<int> , pair<string,int> > stare_curenta = q.front();
        string cuv = stare_curenta.second.first;
        int node = stare_curenta.second.second;
        vector <int> drum_aux = stare_curenta.first;
        drum_aux.push_back(node);
        /// conditie oprire
        if(cuv.size()==0){
            bool ok = false;
            for(int i=1;i<=cnt_stari_finale;i++)
                if(conv[node] == stari_finale[i]){
                    acceptat = true;
                    ok = true;
                }
            if(ok == true) {
                for(int i=0;i<drum_aux.size();i++){
                    fout << "->" << conv[drum_aux[i]];
                }
                fout << '\n';
            }

            /// aici as vrea sa afisez si drumurile
            /// deci ar trebui sa retin drumul pana in aceasta stare
        }
        ///

        for(int i=0;i<graf[node].size();i++){
            pair<int,char> aux = graf[node][i];
            int vecin = aux.first;
            char litera = aux.second;
            if(litera == cuv[0]){
                string cuv_aux = cuv;
                string cuvant_nou = cuv.erase(0,1);
                cuv = cuv_aux;
                q.push({drum_aux,make_pair(cuvant_nou,vecin)});
            }
        }
        q.pop();
    }
    if(acceptat == true){
        fout << "acceptat" << '\n';
    } else fout << "neacceptat" << '\n';
}

void solve(){
    string cuvant;
    f_input >> cuvant;
    bfs(cuvant);
}

int main() {

    citire();
    /// acum am datele procesate si salvate in structuri
    solve();
    return 0;
}
