#include "readData.h"
#include "CustoIn.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include "infoSeq.h"
#include <sys/timeb.h>
#include <sys/resource.h>


using namespace std;

double ** mJobs; // matriz de adjacencia
double ** mSetupTimes; // matriz reorganizada;
int n; // quantidade total de vertices
vector<double> compTimes;

int melhoras = 0, melhorasSwap = 0, melhoras2opt = 0;
int melhorasptb = 0;
int melhorasReinsert[13];
vector<int> listaSub = {0,1,2,3};

//mJobs[1][] = release date
//mJobs[2][] = processing time

void printData(int n, double ** mJobs, double ** mSetupTimes);
vector<int> construction(int n, double ** mJobs, double ** mSetupTimes, double alfa, double &cost);
double sequenceTime(vector<int> s, double ** mJobs, double **mSetupTimes);
vector<CustoIn> calculaCusto(vector<int> listaCandidatos, vector<int> &s, double custoAtual);
bool comp(const CustoIn& a, const CustoIn& b);
double compCostSwap(vector<int> &s, int i, int j, double &compTime);
//void swap(vector<int> &solution, double &cost);
double compCostBlock(vector<int> &s, int lb, int a, int b, double compTime);
void compCompletionTime(vector<int> &s, vector<double> &compTimes);
vector<int> perturb(vector<int> solucao);
vector<int> ils(int i_max, int i_ils);
void reInsertion(int l, vector<int> &solucao, double &custo);
double compCostReinsertion(int l, int n, vector<int> &s, int i, int j, vector<double> compTimes);
void rvnd(vector<int> &solucao, double &custo);
double revCompCostBlock(vector<int> &s, int lb, int a, int b, double compTime);
void twoOptN(vector<int> &solucao, double &custo);
double compCost2opt(vector<int> &s, int i, int j, vector<double> compTimes);
double compCostReinsertionv2(int l, vector<int> &s, int i, int j, infoSequence **sequencesMatrix);
double cpuTime();
void swap(vector<int> &solution, double &cost, infoSequence **sequencesMatrix);
void printSolution(vector<int> solucao, double **mJobs, double ** mSetupTimes);

void invert(vector<int> &solucao, double &custo);
inline double compCostInvert(vector<int> &s, int i, int j, infoSequence **sequencesMatrix);

int main(int argc, char** argv) {

    double processingTime = 0;
    double totalTime = 0;
    vector<int> s;
    int i_max = 20;
    int i_ils;
    readData(argc, argv, &n, &mJobs, &mSetupTimes);

    double comptime;
    double init, end, execTime, custo;
    double bestSolution = numeric_limits<double>::infinity(), bestTime = numeric_limits<double>::infinity();
    double somaTempos = 0, somaCustos = 0;

    

	std::string arg1(argv[1]);
    //cout <<"\n"<< arg1 << endl;
    //std::string arg2(argv[2]);
    int nSequences = stoi(argv[3]);
    //cout << "tamanho da subsequencia: " << nSequences<< endl;
    for(int i = 4; i <= nSequences; i++){
        listaSub.push_back(i);
    }

     if(n >= 150){
      i_ils = n/2;
    }
    else{
      i_ils = 100;
    }

    sequencesMatrix = new infoSequence*[n+1];
    for(int i = 0; i <= n; ++i){
        sequencesMatrix[i] = new infoSequence[n+1];
        /*for(int j = 0; j < n+1; j++){
            sequencesMatrix[i][j] = 0;
        }*/
    }

     int testes = 1;

     if(argc > 2 && argv[2][1] == 'b'){ // Benchmark mode
        testes = 10;
    }
   
    /*double alfa = (double)rand() / RAND_MAX; // gera aleatorio entre 0 e 1
    double fs;
    vector<int> solve = construction(n, mJobs, mSetupTimes, alfa, fs);

    printSolution(solve, mJobs, mSetupTimes);*/
    
    for(int run = 0; run < testes; run++){

        unsigned seed = time(0);
        //cout << "\nseed: " << seed << endl;
        srand(seed);
        
        init = cpuTime();
        s = ils(i_max, i_ils);
        custo = sequenceTime(s, mJobs, mSetupTimes);
        end = cpuTime();
        execTime = end - init;
       //cout << "Tempo de Execução: " << execTime << endl;
        //cout << "custo: " << custo << endl;
        //printSolution(s, mJobs, mSetupTimes);

        somaTempos += execTime;
        somaCustos += custo;

        if(custo < bestSolution)
            bestSolution = custo;
        if(execTime < bestTime)
            bestTime = execTime;

        cout << arg1 << "," << custo << "," << execTime << "," << seed << endl;
        printSolution(s,mJobs, mSetupTimes);
        cout << "\n";

        }

       
        /*if(testes == 10){
            cout << arg1 << "," << bestSolution << "," << somaCustos/10 << "," << bestTime << "," << somaTempos/10 << endl; 
            //cout << "tempo de execucao: " << execTime << endl;
        }*/

        /*unsigned seed = time(0);
        //cout << "\nseed: " << seed << endl;
        srand(seed);
        double costa;
        vector<int> solucao = construction(n, mJobs, mSetupTimes, 0.5, costa);

        for(int i = 0; i < n;i++){
            cout << solucao[i] << " ";
        }
       cout <<  endl;
        invert(solucao, costa);*/
        /*int pos_i = 0;
        int pos_j = 24;
        vector<int> subsequence(solucao.begin() + pos_i, solucao.begin() + pos_j + 1);
        solucao.erase(solucao.begin() + pos_i, solucao.begin() + pos_j + 1);
        solucao.insert(solucao.begin() + pos_i, subsequence.rbegin(), subsequence.rend());

        
        for(int i = 0; i < n;i++){
            cout << solucao[i] << " ";
        }
        cout << " cost = " << sequenceTime(solucao, mJobs, mSetupTimes) << endl;
        cout << " cost = " << compCostInvert(solucao, pos_i, pos_j, sequencesMatrix) << endl;
        //endl;*/
        /*cout << sequencesMatrix[pos_j][pos_i].firstJob << endl;
        cout << sequencesMatrix[pos_j][pos_i].lastJob << endl;*/


    return 0;
}

 void printData(int n, double ** mJobs, double ** mSetupTimes){
    cout <<"Jobs QTD: "<< n << "\n";


    for (int i = 1; i <= n; i++){
        for(int j = 0; j < 4; j++){
            cout << mJobs[i][j] << " ";
        }
    }
    
   for (int i = 0; i <= n; i++){
        for(int j = 1; j <= n; j++){
            cout << mSetupTimes[i][j] << " ";
        }
    }
 } 

vector<int> construction(int n, double ** mJobs, double ** mSetupTimes, double alfa, double &cost){
    vector<int> s;
    vector<int> listaCandidatos;
    double custoAtual = 0;
    //double exetime;

    for(int i = 1; i <= n; i++){
        listaCandidatos.push_back(i); // insere todos os nós na lista de candidatos
    }

    int k; // indice do job a ser adicionado

    for(int j = 0; j < 1; j++){ // tamanho subsequencia inicial
        if(alfa == 0)
            k = 0; // escolhe o primeiro job caso alfa = 0
        else
            int k = rand() % ((int)std::floor(alfa *listaCandidatos.size())); // seleciona um job dentre os alfa menores release dates
        s.push_back(listaCandidatos[k]); // adiciona o job a solução
        listaCandidatos.erase(listaCandidatos.begin() + k); // apaga da lista de candidatos oq ja foi pra solução
        if(j == 0){
            custoAtual += mJobs[s[0]][1] + mSetupTimes[0][s[j]] + mJobs[s[0]][2]; // computa o tempo de conclusão do primeiro job
        }
        else{
            if(custoAtual >= mJobs[s[j]][1]){
                custoAtual += mSetupTimes[s[j-1]][s[j]] + mJobs[s[j]][2];
            }
            else{
                custoAtual = mSetupTimes[s[j-1]][s[j]] + mJobs[s[j]][2] + mJobs[s[j]][1];
            }
        }
    }
   // cout << custoAtual << endl;

    vector<CustoIn> custoInsertion = calculaCusto(listaCandidatos, s, custoAtual); // calcula os custo de inserção dos candidatos
    std::sort(custoInsertion.begin(), custoInsertion.end(), comp); // ordena de forma crescente de acordo com os custos
    //cout << "print1\n";
    int sel;
    //cout << "tam" << custoInsertion.size() << "\n";
    while(!listaCandidatos.empty()){
        if(alfa == 0){
            sel = 0;
        }
        else{
            sel = rand() % ((int)std::floor(alfa * (custoInsertion.size() - 1)) + 1); // escolhe um nó dentro de uma faixa definida por alfa
        }
        //cout << "print2\n";
        //s.insert(s.begin() + custoInsertion[sel].arestaOut, custoInsertion[sel].noIn); // insere o nó na solucao
       /* for(int i = 0; i < s.size(); i++){
            cout << s[i] << " ";
        }*/
        //cout << "custo atual: " << custoAtual <<endl;
        s.push_back(custoInsertion[sel].noIn);
        //cout << "node: " << custoInsertion[sel].noIn <<" custo: " <<custoInsertion[sel].custo << endl;
        custoAtual += max(custoInsertion[sel].custo,0.0) + mSetupTimes[s[s.size()-2]][s[s.size()-1]] + mJobs[s[s.size()-1]][2]; 
        //cout << "print3\n";
        for(int i = 0; i < listaCandidatos.size(); i++){
            if(listaCandidatos[i]==custoInsertion[sel].noIn)
                listaCandidatos.erase(listaCandidatos.begin() + i); // exclui o nó da lista de candidatos
            }
        
        custoInsertion.erase(custoInsertion.begin(), custoInsertion.end()); // exclui o nó da lista de custos
        custoInsertion = calculaCusto(listaCandidatos, s, custoAtual); // calcula os novos custos de inserção
        std::sort(custoInsertion.begin(), custoInsertion.end(), comp); // ordena os custos
        
    }
    
    setSequencesMatrix(sequencesMatrix,s,n,mJobs,mSetupTimes);
    //compTime = sequenceTime(s, mJobs, mSetupTimes);
    infoSequence initialSolution;
    initialSolution = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][n-1]);
    //idleTime = initialSolution.waitingTime;
    cost = initialSolution.initialTime + initialSolution.duration;
    //printSolution(s, mJobs, mSetupTimes);

    return s;
}

vector<CustoIn> calculaCusto(vector<int> listaCandidatos, vector<int> &s, double custoAtual){
  vector<CustoIn> custoInsertion (listaCandidatos.size());
    //for(int i = 0, l = 0; i < s.size()-1; i++){
        int l = 0;
        for(auto k : listaCandidatos){
            custoInsertion[l].custo = max((mJobs[k][1] - custoAtual), 0.0) + mSetupTimes[s[s.size() - 1]][k];
            custoInsertion[l].noIn = k; // nó inserido
            //custoInsertion[l].arestaOut = i; // posicao de inserção;
            l++;
        }

  return custoInsertion;

}

inline double sequenceTime(vector<int> s, double ** mJobs, double **mSetupTimes){
    
    double cTime = mSetupTimes[0][s[0]] + mJobs[s[0]][2] + mJobs[s[0]][1];
    double totalWT = mJobs[s[0]][1];
    /*for(int i = 0; i < s.size(); i++){
        cout << s[i] << " ";
    }*/
    for(int i = 0, j = 1; j < s.size(); i++ ,j++){
        //cout<< i << " " << j << endl;
        if(cTime >= mJobs[s[j]][1]){
            cTime += mSetupTimes[s[i]][s[j]] + mJobs[s[j]][2];
        }
        else{
            totalWT += mJobs[s[j]][1] - cTime;
            cTime += mSetupTimes[s[i]][s[j]] + mJobs[s[j]][2] + (mJobs[s[j]][1] - cTime);
        }
    }
    //cout << "\ntotalWT: " << totalWT - mJobs[s[0]][1]<< endl;
    return cTime;
} 


 bool comp(const CustoIn& a, const CustoIn& b) // comparação dos custos utilizada para ordenar os objetos
  {
    return a.custo < b.custo;
  }


double compCostSwap2(vector<int> &s, int i, int j, infoSequence **sequencesMatrix){
 double cost;
    infoSequence swaproute;
    infoSequence dJob;

    dJob.initialTime = 0;
    dJob.duration = 0;
    dJob.firstJob = 0;
    dJob.lastJob = 0;
  
    if(i == 0){
        if(j == 1){
            //dJob.initialTime = 0;
            infoSequence dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[j][i]);
            //swaproute = concatenateSequence(mSetupTimes, mJobs, sequencesMatrix[j][i], sequencesMatrix[j+1][n-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j+1][s.size()-1]);
            cost = swaproute.initialTime + swaproute.duration;
        }
        else{
            //dJob.initialTime = 0;
            infoSequence dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[j][j]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[1][j-1]);
            //swaproute = concatenateSequencev2(mSetupTimes, mJobs, msequencesMatrix[j][j], sequencesMatrix[i+1][j-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[i][i]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j+1][n-1]);
            cost = swaproute.initialTime + swaproute.duration;
        }
    }
    else{
        if(i == n-2){
            //dJob.initialTime = mJobs[s[0]][1];
            infoSequence dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][i-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j][i]);
            //swaproute = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[0][i-1], sequencesMatrix[j][i]);
            cost = swaproute.initialTime + swaproute.duration;
        }
        else{
            if(j == i+1){
                //dJob.initialTime = mJobs[s[0]][1];
                infoSequence dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][i-1]);
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j][i]);
                //swaproute = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[0][i-1], sequencesMatrix[j][i]);
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j+1][n-1]);
                cost = swaproute.initialTime + swaproute.duration;
            }
            else{
                //dJob.initialTime = mJobs[s[0]][1];
                infoSequence dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][i-1]);
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j][j]);
                //swaproute = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[0][i-1], sequencesMatrix[j][j]);
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[i+1][j-1]);
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[i][i]);
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j+1][n-1]);
                cost = swaproute.initialTime + swaproute.duration;
            }    
        }
    }
    return cost;
}

void swap(vector<int> &solution, double &cost, infoSequence **sequencesMatrix){
  //double inicioSwap = cpuTime();
  double delta, delta2;
  double menor = cost;
  int pos_i = -1, pos_j = -1; // guarda as posições para realizar a operação
    for(int i = 0; i < solution.size() - 1; i++){ // exclui da operação a primeira e a ultima posição do vetor
        for(int j = i + 1; j < solution.size(); j++){
           delta = compCostSwap2(solution, i, j, sequencesMatrix);
            if(delta < menor){
                menor = delta;
                pos_i = i;
                pos_j = j;
            }
        }
    }

    if(pos_i >= 0){ // realiza a operação
        melhoras++;
        swap(solution[pos_i], solution[pos_j]);
        cost =  menor;
        //setSequencesMatrix(sequencesMatrix,solution,n,mJobs,mSetupTimes);
        melhorasSwap++;

        swap(sequencesMatrix[pos_i][pos_i], sequencesMatrix[pos_j][pos_j]);

        for(int i = 0; i < pos_i; i++){
            for(int j = pos_i; j < n; j++){
                sequencesMatrix[i][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[i][j-1], sequencesMatrix[j][j]);
                sequencesMatrix[j][i] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][i]);
            }
        }

        for(int j = pos_i + 1; j < n; j++){
            sequencesMatrix[pos_i][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[pos_i][j-1], sequencesMatrix[j][j]);
            sequencesMatrix[j][pos_i] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][pos_i]);
        }
        
        for(int i = pos_i + 1; i < pos_j; i++){
            for(int j = pos_j; j < n; j++){
                sequencesMatrix[i][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[i][j-1], sequencesMatrix[j][j]);
                sequencesMatrix[j][i] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][i]);
            }
        }  

         for(int j = pos_j + 1; j < n; j++){
            sequencesMatrix[pos_j][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[pos_j][j-1], sequencesMatrix[j][j]);
            sequencesMatrix[j][pos_j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][pos_j]);
        }
    
    }
  //double fimSwap = cpuTime();
 // swapTime += (fimSwap - inicioSwap);   
}
 
double compCostReinsertionv2(int l, vector<int> &s, int i, int j, infoSequence **sequencesMatrix){
    infoSequence swaproute;
    infoSequence dJob;
    infoSequence dummyJob;

    dJob.initialTime = 0;
    dJob.duration = 0;
    dJob.firstJob = 0;
    dJob.lastJob = 0;

    if(i < j){
        if(i == 0){
            dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[i+l][j+l-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[i][i+l-1]);
            if(j != n-l)
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j+l][n-1]);
        }
        else{
            dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][i-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[i+l][j+l-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[i][i+l-1]);
            if(j != n-l)
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j+l][n-1]);
        }
    }
    else{
        if(j == 0){
            dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[i][i+l-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[0][i-1]);
            if(i != n-l)
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[i+l][n-1]);
        }
        else{
            dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][j-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[i][i-1+l]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j][i-1]);
            if(i != n-l)
                swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[i+l][n-1]);
        }
       
    }
    double cost = swaproute.initialTime + swaproute.duration;

    return cost;
}


void reInsertion(int l, vector<int> &solucao, double &custo){ // reinsere um nó em posição diferente
  //double inicioreinsertion = cpuTime();
    double menor = custo;
    double delta, delta2;
    int pos_i = -1, pos_j = -1;
    for(int i = 0; i < solucao.size() - l; i++){ // varre a solução exceto o 0 e o final
        for(int j = 0; j < solucao.size() - l; j++){
            if(i != j){ // exclui a posição inicial do nó
                delta = compCostReinsertionv2(l, solucao, i, j, sequencesMatrix);
                if(delta < menor){
                    menor = delta;
                    pos_i = i;
                    pos_j = j;
                }
            }
        }
    }
    if(pos_i != -1){
        //cout << "i: " << pos_i << " j: " << pos_j << endl;     
        vector<int> subsequence(solucao.begin() + pos_i, solucao.begin() + pos_i + l);
        solucao.erase(solucao.begin() + pos_i, solucao.begin() + pos_i + l);
        solucao.insert(solucao.begin() + pos_j, subsequence.begin(), subsequence.end());
        custo = menor;
        //setSequencesMatrix(sequencesMatrix,solucao,n,mJobs,mSetupTimes);
        melhoras++;
        melhorasReinsert[l-1]++;



////// SEQUENCE ATT         
         if(pos_i > pos_j){
            swap(pos_i, pos_j);
        }

        for(int i = pos_i; i < pos_j + l; i++){
            sequencesMatrix[i][i].firstJob = solucao[i];
            sequencesMatrix[i][i].lastJob = solucao[i];
            sequencesMatrix[i][i].duration = mJobs[solucao[i]][2];
            sequencesMatrix[i][i].initialTime = mJobs[solucao[i]][1];
        }
       
        for(int i = 0; i < pos_i; i++){
            for(int j = pos_i; j < n; j++){
                sequencesMatrix[i][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[i][j-1], sequencesMatrix[j][j]);
                sequencesMatrix[j][i] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][i]);
            }
        }
        for(int i = pos_i; i < pos_j; i++){
            for(int j = i + 1; j < n; j++){
                sequencesMatrix[i][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[i][j-1], sequencesMatrix[j][j]);
                sequencesMatrix[j][i] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][i]);
            }
        }
        for(int i = pos_j; i < pos_j+l ; i++){
            for(int j = i + 1; j < n; j++){
                sequencesMatrix[i][j] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[i][j-1], sequencesMatrix[j][j]);
                sequencesMatrix[j][i] = concatenateSequencev2(mSetupTimes, mJobs, sequencesMatrix[j][j], sequencesMatrix[j-1][i]);
            }
        }
    }
  //double fimReinsertion =  cpuTime();
  //tempo_reinsertion += (fimReinsertion - inicioreinsertion);
}

inline double compCostInvert(vector<int> &s, int i, int j, infoSequence **sequencesMatrix){
    infoSequence swaproute;
    infoSequence dJob;
    infoSequence dummyJob;

    dJob.initialTime = 0;
    dJob.duration = 0;
    dJob.firstJob = 0;
    dJob.lastJob = 0;

    if(i == 0){
        dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[j][i]);
        swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j+1][n-1]);
        if(j == n-1)
            swaproute = dummyJob;
    }
    else
    {
        dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][i-1]);
        swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j][i]);
        swaproute = concatenateSequencev2(mSetupTimes, mJobs, swaproute, sequencesMatrix[j+1][n-1]);
        if(j == n-1){
            dummyJob = concatenateSequencev2(mSetupTimes, mJobs, dJob, sequencesMatrix[0][i-1]);
            swaproute = concatenateSequencev2(mSetupTimes, mJobs, dummyJob, sequencesMatrix[j][i]);
        }
    }
    
    return swaproute.duration + swaproute.initialTime;
}

void invert(vector<int> &solucao, double &custo){
    double menor = custo;
    double delta, delta2;
    //vector<int> teste = solucao;
    int pos_i = -1, pos_j = -1;
    for(int i = 0; i < solucao.size()-1; i++){ // varre a solução exceto o 0 e o final
        for(int j = i+1; j < solucao.size(); j++){
            delta = compCostInvert(solucao, i, j, sequencesMatrix);

            /*vector<int> subsequence2(teste.begin() + i, teste.begin() + j + 1);
            teste.erase(teste.begin() + i, teste.begin() + j + 1);
            teste.insert(teste.begin() + i, subsequence2.rbegin(), subsequence2.rend());
            if(delta != sequenceTime(teste, mJobs, mSetupTimes))
                cout << "calc: " << delta <<  " real: " << sequenceTime(teste, mJobs, mSetupTimes) << endl;
            //cout << "real: " << sequenceTime(teste, mJobs, mSetupTimes);
            teste = solucao;*/
            if(delta < menor){
                menor = delta;
                pos_i = i;
                pos_j = j;
            }
        }
    }

    if(pos_i != -1){
        //cout << "i: " << pos_i << " j: " << pos_j << endl;     
        vector<int> subsequence(solucao.begin() + pos_i, solucao.begin() + pos_j + 1);
        solucao.erase(solucao.begin() + pos_i, solucao.begin() + pos_j + 1);
        solucao.insert(solucao.begin() + pos_i, subsequence.rbegin(), subsequence.rend());
        custo = menor;
        setSequencesMatrix(sequencesMatrix,solucao,n,mJobs,mSetupTimes);
        melhoras++;
       /* cout << pos_i << " ; " << pos_j << endl;

        for(int i = 0; i < n;i++){
            cout << solucao[i] << " ";
        }*/
        //melhorasReinsert[l-1]++;
    }

}

vector<int> perturb(vector<int> solucao){
  vector<int> s = solucao;

  int tam_max = ceil(((double)n)/10);
  //int tam_max = 15; // tamanho maximo das subsequencias
  int inicio1, fim1, inicio2, fim2;

  inicio1 = (rand() % ((n - (2 * tam_max)) - 1 + 1)); // posicao minima = 0, posicao maxima = final - 2*tmax
  fim1 = (rand() % ((inicio1 + (tam_max - 1)) - (inicio1 + 1) + 1)) + (inicio1 + 1); // minima = inicio+1, maxima = inicio1 + tmax - 1;
  inicio2 = (rand() % ((n - tam_max) - (fim1 + 1) + 1) + (fim1 + 1)); // minima = fim1 + 1, maxima = final - tmax;
  fim2 = (rand() % ((inicio2 + (tam_max - 1)) - (inicio2 + 1) + 1) + (inicio2 + 1)); // minima = inicio2 + 1, maxima = inicio2 + tmax - 1;


  int d1 = fim1 - inicio1; // tamanho da primeira subsequencia, usado para corrigir erros na inserção
  int d2 = fim2 - inicio2; // tamanho da segunda subsequencia, usado pra corrigir erros na inserção


  //cout << "inicio1: " << inicio1 <<  " fim2: " << fim2 << endl;

  s.erase(s.begin() + inicio2, s.begin() + fim2 + 1); // apaga primeira subsequencia
  s.erase(s.begin() + inicio1, s.begin() + fim1 + 1); // apaga segunda subsequencia
  s.insert(s.begin() + inicio1, &solucao[inicio2], &solucao[fim2] + 1);  // inclui a segunda subsequencia na posicao da primeira
  s.insert(s.begin() + inicio2 + (-1*(d1 - d2)) , &solucao[inicio1], &solucao[fim1] + 1); // inclui a segunda subsequencia na posicao da segunda

  //compCompletionTime(s,compTimes);
  setSequencesMatrix(sequencesMatrix,s,n,mJobs,mSetupTimes);

  return s;
}

vector<int> ils(int i_max, int i_ils){
    double ff = numeric_limits<double>::infinity(); // custo final
    vector<int> s, s1, sf; // s, s', s*
    double fs, fs1;
    for (int i = 0; i < i_max; i++){
        //cout << "ITER_ILS: " << i << endl;
        double alfa = (double)rand() / RAND_MAX; // gera aleatorio entre 0 e 1
        s = construction(n, mJobs, mSetupTimes, alfa, fs); // constroi solucao inicial
        s1 = s;
        fs1 = fs;
        int iter_ILS = 0;
        while (iter_ILS < i_ils){
        rvnd(s, fs); // explora as estruturas de vizinhança
        if (fs < fs1){
            s1 = s;
            fs1 = fs;
            melhorasptb++;
            iter_ILS = 0;
        }
        s = perturb(s1); // perturba a solução evitando resultado ótimo local
        fs = sequenceTime(s,mJobs,mSetupTimes);
       // cout << "fs " <<fs << endl;
        iter_ILS++;
        }
        if (fs1 < ff){
        sf = s1;
        ff = fs1;
        //cout << "\n" << ff << " " << " na " << i+1 <<  " iter " << melhorasptb << " melhoras" << " Oropt-" << melhorasReinsert[0] << " Oropt2-" << melhorasReinsert[1] << " Oropt3-" << melhorasReinsert[2] << " Oropt4-" << melhorasReinsert[3] << " Oropt5-" << melhorasReinsert[4] << " Oropt6-" << melhorasReinsert[5] << " Oropt7-" << melhorasReinsert[6] << " Oropt8-" << melhorasReinsert[7] << " Oropt9-" << melhorasReinsert[8] << " Oropt10-" << melhorasReinsert[9]<< " Oropt11-" << melhorasReinsert[10]<< " Oropt12-" << melhorasReinsert[11]<< " Oropt13-" << melhorasReinsert[12]<< " 2opt-" << melhoras2opt << " troca-" << melhorasSwap << endl;
        }

	

    }
    return sf; // retorna a melhor solucao

}

void rvnd(vector<int> &solucao, double &custo){
  vector<int> s = solucao;
  //vector<int> nLista = {0,1,2,3,4,5,6,7,8,9,10,11,12,13}; // lista de estruturas in02_360.dat,16876,16902.2,67.8574,72.3509
  //vector<int> nLista = {0,1,2,3}; //instances/in02_360.dat,17081,17101.1,28.9016,32.833
  vector<int> nLista = listaSub;
  double custoMod =  custo;
  int sel, pos;

  while(!nLista.empty()){ // roda enquanto existirem estruturas de vizinhança na lista

    int k = rand() % nLista.size();

    switch(nLista[k]){
        case 0:
            swap(solucao, custoMod, sequencesMatrix);
            break;

        case 1:
            reInsertion(1,solucao, custoMod);
            break;

        case 2:
            reInsertion(2,solucao, custoMod);      
            break;

        case 3:
            reInsertion(3,solucao, custoMod);
            break;
    
        case 4:
            reInsertion(4,solucao, custoMod);   
            break;

        case 5:
            reInsertion(5,solucao, custoMod);        
            break;

        case 6:
            reInsertion(6,solucao, custoMod);       
            break;

        case 7:
            reInsertion(7,solucao, custoMod);  
            break;
    
        case 8:
            reInsertion(8,solucao, custoMod);  
            break;

        case 9:
            reInsertion(9,solucao, custoMod);  
            break;

        case 10:
            reInsertion(10,solucao, custoMod);  
            break;
        
        case 11:
            reInsertion(11,solucao, custoMod);
            break;
        
        case 12:
            reInsertion(12,solucao, custoMod);
            break;
        
        case 13:
            reInsertion(13,solucao, custoMod);
            break;
        
        case 14:
            invert(solucao, custoMod);
            break;

      

    }

    //custoMod = custoTotal(solucao); // calcula o custo do Movimento

    if(custo > custoMod){ // movimento melhorou o custo
      custo = custoMod;
      s = solucao;
      //nLista = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
      //nLista = {0,1,2,3};
      nLista = listaSub;
    }
    else { // nao melhorou, exclui o movimento da lista
      solucao = s;
      nLista.erase(nLista.begin() + k);
      custoMod = custo;
    }
  }
}

double cpuTime() {
	static struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return ((double)usage.ru_utime.tv_sec)+(((double)usage.ru_utime.tv_usec)/((double)1000000));
}

void printSolution(vector<int> solucao, double **mJobs, double ** mSetupTimes){
    for(int i = 0; i < n; i++){
        cout << solucao[i] << " ";
    }
    cout << "\n";
    /*for(int i = 0; i < n; i++){
        cout << mJobs[solucao[i]][1] << " ";
    }*/

    double cTime = mSetupTimes[0][solucao[0]] + mJobs[solucao[0]][2] + mJobs[solucao[0]][1];
    double totalWT = mJobs[solucao[0]][1];
    /*for(int i = 0; i < s.size(); i++){
        cout << s[i] << " ";
    }*/
    cout << totalWT;
    for(int i = 0, j = 1; j < solucao.size(); i++ ,j++){
        //cout<< i << " " << j << endl;
        if(cTime >= mJobs[solucao[j]][1]){
            cTime += mSetupTimes[solucao[i]][solucao[j]] + mJobs[solucao[j]][2];
            cout << " " << 0;
        }
        else{
            totalWT = mJobs[solucao[j]][1] - cTime;
            cout << " " << totalWT;
            cTime += mSetupTimes[solucao[i]][solucao[j]] + mJobs[solucao[j]][2] + (mJobs[solucao[j]][1] - cTime);
        }
    }
}