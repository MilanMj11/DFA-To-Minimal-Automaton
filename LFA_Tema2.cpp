#include <bits/stdc++.h>

using namespace std;

ifstream fin("automat.txt");
ifstream fin_aux("automat.txt");
ifstream f_input("input.txt");
ofstream fout("output.txt");

const int NMAX = 10005;
vector< pair<int,char> > graf[NMAX]; /// v[i] = { vecin , muchie }
vector< pair<int,char> > graf_minimal[NMAX];
map<string,int> conversie;  /// conversie[node] = indicele
map<string,int> conversie_minimal; /// conversie_minimal[node_nou] = indice_nou;
map<string,string> atribuire; /// atribuire[node_vechi] = node_nou  -> la graful minimal;
bool ver_minimal[NMAX];
string conv[NMAX];          /// conv[indice] = node;
string conv_minimal[NMAX];
vector<string> stari_finale;
vector<string> stari_finale_minimal;
string stare_initiala = "";
string stare_initiala_minimal = "";
vector<char> List_litere;   /// lista cu literele
vector<string> List_stari;  /// lista cu starile
vector<string> List_stari_minimale;
string tabel[100][100];     /// tabelul pentru rezolvarea automatului minimal
int cnt_stari_finale = 0;
int cnt_nodes = 0;
int cnt_litere = 0;
int cnt_stari_minimal = 0;

void afisare_graf_minimal();
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
    if(conversie.find(node1) == conversie.end()){
        conversie.insert(make_pair(node1,++cnt_nodes));
        conv[cnt_nodes] = node1;
        List_stari.push_back(node1);
    }
    if(conversie.find(node2) == conversie.end()){
        conversie.insert(make_pair(node2,++cnt_nodes));
        conv[cnt_nodes] = node2;
        List_stari.push_back(node2);
    }
    graf[conversie[node1]].push_back(make_pair(conversie[node2],muchie));
}

void procesare_stari_finale(string s){
    string nod = "";
    int ind = 0;
    int dim = s.size();
    while(ind!=dim){
        if(s[ind]==' '){
            stari_finale.push_back(nod);
            nod = "";
            ind++;
        }
        nod = nod + s[ind];
        ind++;
    }
    stari_finale.push_back(nod);

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

bool bfs(string start_node,string cuvant){
    vector <int> vdrum;
    vdrum.clear();
    queue < pair< vector<int> , pair<string,int> > > q;
    q.push({vdrum,make_pair(cuvant,conversie[start_node])});
    bool acceptat = false;
    while(!q.empty()){
        pair< vector<int> , pair<string,int> > stare_curenta = q.front();
        string cuv = stare_curenta.second.first;
        int node = stare_curenta.second.second;
        vector <int> drum_aux = stare_curenta.first;
        drum_aux.push_back(node);
        /// conditie oprire
        if(cuv.size()==0){
            for(int i=0;i<stari_finale.size();i++)
                if(conv[node] == stari_finale[i]){
                    acceptat = true;
                }
            return acceptat;
            /// aici vreau sa verific daca drumul duce spre un nod final sau nu
        }

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
}

void construct_lista_litere(){
    for(int i=0;i<graf[conversie[stare_initiala]].size();i++){
        List_litere.push_back(graf[conversie[stare_initiala]][i].second);
    }
}

void completam_tabel_cu_lambda(){
    for(int i=0;i<100;i++)
        for(int j=0;j<100;j++)
            tabel[i][j] = "??";
    for(int i=0;i<stari_finale.size();i++){
        int linie = conversie[stari_finale[i]]-1;
        for(int j=0;j<List_stari.size();j++){
            int coloana = conversie[List_stari[j]]-1;
            bool finala = false;
            /// daca nodul j e final nu facem nimic
            for(int t=0;t<stari_finale.size();t++){
                if(List_stari[j] == stari_finale[t]){
                    finala = true;
                    break;
                }
            }
            if(finala==false){
                tabel[linie][coloana]="YY";
            }
        }
    }

}

int a[NMAX];
bool ver[NMAX];
vector<string> Lista_gen;

void genereaza_cuvinte(int lungime,int ind = 1){
    if(ind==lungime+1){
        string aux = "";
        for(int i=1;i<=lungime;i++)
            aux+=List_litere[a[i]-1];
        Lista_gen.push_back(aux);
        return;
        /// gata permutarea
    }
    for(int i=1;i<=cnt_litere;i++) {
        a[ind] = i;
        genereaza_cuvinte(lungime,ind+1);
    }
}

bool tabel_complet(){
    int cnt = 0;
    for(int i=0;i<List_litere.size();i++){
        for(int j=0;j<List_litere.size();j++){
            if(tabel[i][j] == "??") cnt++;
        }
    }
}

void completam_tabel(){

    int lungime = 1;
    //a[0]=1;
    while(true){
       // if(tabel_complet() == true) break;
        if(lungime>List_stari.size()) break; /// NU ASTA E CONDITIA DE OPRIRE
        Lista_gen.clear();
        genereaza_cuvinte(lungime);
        vector<string> Finale,NeFinale;
        for(int i=0;i<Lista_gen.size();i++){

            string cuvant = Lista_gen[i];
            for(int j=0;j<List_stari.size();j++){
                string stare = List_stari[j];
                bool ACC = bfs(stare,cuvant);
                if(ACC == true){
                    Finale.push_back(stare);
                } else  NeFinale.push_back(stare);
            }

            /// acum avem cele care apartin de F si cele care apartin de NF
            for(int i1=0;i1<Finale.size();i1++){
                int linie = conversie[Finale[i1]]-1;
                for(int j1=0;j1<NeFinale.size();j1++){
                    int coloana = conversie[NeFinale[j1]]-1;
                    if(tabel[linie][coloana]=="??")
                        tabel[linie][coloana] = cuvant;
                    if(tabel[coloana][linie]=="??")
                        tabel[coloana][linie] = cuvant;
                }
            }
            Finale.clear();
            NeFinale.clear();

        }

        lungime++;
    }

}

void afis_tabel(){
    for(int i=0;i<List_stari.size();i++){
        for(int j=0;j<=List_stari.size();j++){
            fout << tabel[i][j] << ' ';
        }
        fout << '\n';
    }
}

void reconstructiv_graph(){
    /// gasesc liniile din tabel care sunt identice
    /// ultima coloana a tabelului reprezeinta si noile denumiri ale nodurilor
    /// practic atribui nodurilor care urmeaza a fi "combinate" aceeasi denumire
    for(int i=0;i<List_stari.size();i++){
        if(tabel[i][List_stari.size()] != "??") continue;

        tabel[i][List_stari.size()] = 'A'-1+ ++cnt_stari_minimal;
        List_stari_minimale.push_back(tabel[i][List_stari.size()]);
        conversie_minimal.insert(make_pair(tabel[i][List_stari.size()],cnt_stari_minimal));
        conv_minimal[cnt_stari_minimal] = tabel[i][List_stari.size()];
        for(int j=i+1;j<List_stari.size();j++){

            /// daca liniile i si j sunt egale atunci nodurile alea can "act like one"
            bool egale = true;
            for(int t=0;t<List_stari.size();t++){
                if(tabel[i][t] != tabel[j][t]){
                    egale = false;
                    break;
                }
            }
            if(egale == true){
                tabel[j][List_stari.size()] = 'A'-1 + cnt_stari_minimal;
            }
        }
    }
    for(int i=0;i<List_stari.size();i++){
        atribuire.insert(make_pair(List_stari[i],tabel[i][List_stari.size()]));
        /// pentru fiecare stare din graful vechi atribuim nodurile din graful minimal;
    }
//    for(auto it : atribuire){
//        fout << it.first << ' ' << it.second << '\n';
//    }
    for(int i=0;i<List_stari.size();i++){
        string node = List_stari[i];
        int node_int = conversie[node];

        if(ver_minimal[conversie_minimal[tabel[i][List_stari.size()]]] == true) continue;

        ver_minimal[conversie_minimal[tabel[i][List_stari.size()]]] = true;

        for(int j=0;j<graf[node_int].size();j++){
            pair<int,char> aux = graf[node_int][j];
            int ind_vecin = aux.first;
            char litera = aux.second;
            string vecin = conv[ind_vecin];
            //fout << ind_vecin << ' ' << litera << '\n';
            graf_minimal[conversie_minimal[atribuire[node]]].push_back(make_pair(conversie_minimal[atribuire[vecin]],litera));
            //fout << conversie_minimal[atribuire[node]] << ' ' << conversie_minimal[atribuire[vecin]] << ' ' << litera << '\n';
        }
    }
    /// am creat acum si graful minimal
    /// am sa il afisam acum

    afisare_graf_minimal();
}

void afisare_graf_minimal() {
    stare_initiala_minimal = atribuire[stare_initiala];
    fout << stare_initiala_minimal << '\n';
    for(int i=0;i<List_stari_minimale.size();i++){
        string node = List_stari_minimale[i];
        int node_int = conversie_minimal[node];
        for(int j=0;j<graf_minimal[node_int].size();j++){
            pair<int,char> aux =  graf_minimal[node_int][j];
            int vecin_ind = aux.first;
            char litera = aux.second;
            string vecin = conv_minimal[vecin_ind];
            fout << node << ' ' << litera << ' ' << vecin << '\n';
        }
    }
    for(int i=0;i<stari_finale.size();i++){
        stari_finale_minimal.push_back(atribuire[stari_finale[i]]);
        fout << stari_finale_minimal[i] << ' ';
    }
    fout << '\n';
}

void solve(){
    construct_lista_litere();
    cnt_litere = List_litere.size();

    completam_tabel_cu_lambda();
    completam_tabel();
    /// tabelul e complet
    reconstructiv_graph();
    //afis_tabel();
}

int main() {

    citire();
    solve();
    return 0;
}
