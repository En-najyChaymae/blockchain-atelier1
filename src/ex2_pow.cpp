#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <functional>
using namespace std;

// Hash simplifié (simulation SHA256)

string simpleHash(const string &data) {
    hash<string> hasher;
    size_t hashValue = hasher(data);
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hashValue;
    return ss.str();
}

// Proof of Work (minage simulé)
void proofOfWorkSimulation(int difficulty) {
    cout << "\n===== Proof of Work Simulation =====" << endl;
    string target(difficulty, '0');
    string hash;
    long nonce = 0;

    auto start = chrono::high_resolution_clock::now();

    do {
        string data = "block" + to_string(nonce);
        hash = simpleHash(data);
        nonce++;
    } while (hash.substr(0, difficulty) != target);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "✅ Bloc miné avec succès !" << endl;
    cout << "Hash trouvé : " << hash << endl;
    cout << "Difficulté : " << difficulty << " | Nonce : " << nonce 
         << " | Temps : " << elapsed.count() << " sec\n";
}


struct Validator {
    string name;
    double stake;
};

string selectValidator(const vector<Validator>& validators) {
    double totalStake = 0;
    for (auto &v : validators)
        totalStake += v.stake;

    double r = ((double) rand() / RAND_MAX) * totalStake;
    double cumulative = 0;
    for (auto &v : validators) {
        cumulative += v.stake;
        if (r <= cumulative)
            return v.name;
    }
    return validators.back().name;
}

void proofOfStakeSimulation(const vector<Validator>& validators) {
    cout << "\n===== Proof of Stake Simulation =====" << endl;
    auto start = chrono::high_resolution_clock::now();

    string winner = selectValidator(validators);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "🏆 Validateur choisi : " << winner << endl;
    cout << "Temps d’exécution : " << elapsed.count() << " sec\n";
}


int main() {
    srand(time(0));
    cout << "==== Simulation Proof of Work vs Proof of Stake ====\n";

    
    proofOfWorkSimulation(3); 

    vector<Validator> validators = {
        {"Alice", 50.0},
        {"Bob", 30.0},
        {"Charlie", 20.0}
    };
    proofOfStakeSimulation(validators);

    return 0;
}