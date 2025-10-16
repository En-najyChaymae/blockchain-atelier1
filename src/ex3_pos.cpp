#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <random>
#include <ctime>
using namespace std;

// === Hash simplifié (démo pédagogique) ===
string simpleHash(const string& s) {
    hash<string> H;
    size_t hv = H(s);
    stringstream ss; ss << hex << setw(16) << setfill('0') << hv;
    return ss.str();
}

// === Modèle PoS ===
struct Validator {
    string name;
    double stake; 
};

class Block {
public:
    int index{};
    string data;
    string prevHash;
    string validator;   
    string hash;
    long long timestamp{};

    Block(int i, string d, string prev, string val)
        : index(i), data(move(d)), prevHash(move(prev)), validator(move(val)) {
        timestamp = time(nullptr);
        hash = computeHash();
    }

    string computeHash() const {
        stringstream ss;
        ss << index << timestamp << data << prevHash << validator;
        return simpleHash(ss.str());
    }
};

class Blockchain {
public:
    vector<Block> chain;
    vector<Validator> validators;

    explicit Blockchain(vector<Validator> v)
        : validators(move(v)) {
        // Genesis
        string val = chooseValidator();
        chain.emplace_back(0, "Genesis-Pos", "0", val);
    }

    void addBlock(const string& data) {
        string val = chooseValidator();
        const string& prev = chain.back().hash;
        Block b((int)chain.size(), data, prev, val);
        chain.push_back(b);
        cout << "✅ Bloc " << b.index
             << " | validator=" << b.validator
             << " | hash=" << b.hash.substr(0,12) << "...\n";
    }

    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); ++i) {
            const Block& c = chain[i];
            const Block& p = chain[i-1];
            if (c.prevHash != p.hash) return false;
            if (c.computeHash() != c.hash) return false;
        }
        return true;
    }

    void setStake(const string& name, double newStake) {
        for (auto& v : validators) {
            if (v.name == name) { v.stake = max(0.0, newStake); break; }
        }
    }

private:
    string chooseValidator() const {
        vector<double> w; w.reserve(validators.size());
        for (auto& v : validators) w.push_back(max(0.0, v.stake));
        bool allZero = true; for (double x : w) if (x > 0) { allZero = false; break; }
        if (allZero) for (double& x : w) x = 1.0;

        random_device rd;
        mt19937 gen(rd());
        discrete_distribution<int> dist(w.begin(), w.end());
        int idx = dist(gen);
        return validators[idx].name;
    }
};

int main() {
    cout << "==== Proof of Stake (PoS) — Demo ====\n";

    vector<Validator> vals = {
        {"Alice", 40.0}, {"Bob", 35.0}, {"Charlie", 25.0}
    };
    Blockchain bc(vals);

    cout << "\n-- Ajout de 3 blocs (PoS) --\n";
    bc.addBlock("Paiement X->Y : 7");
    bc.addBlock("Paiement X->Y : 7");  
    bc.addBlock("Frais + bonus minage (PoS)");

    cout << "\nIntégrité chaîne : " << (bc.isChainValid() ? "OK" : "CORROMPUE") << "\n";

    // 2) Petit test de "fairness" (qui gagne sur plusieurs tours ?)
    cout << "\n-- Simulation de 20 sélections PoS (compte de victoires) --\n";
    int nRounds = 20;
    int a=0,b=0,c=0;
    for (int i=0; i<nRounds; ++i) {
        // Recrée un bloc éphémère juste pour observer le gagnant
        string winnerBefore = bc.validators[0].name; // init
       
        vector<double> w; for (auto& v : bc.validators) w.push_back(max(0.0, v.stake));
        bool allZero = true; for (double x : w) if (x > 0) { allZero = false; break; }
        if (allZero) for (double& x : w) x = 1.0;
        random_device rd; mt19937 gen(rd()); discrete_distribution<int> dist(w.begin(), w.end());
        int idx = dist(gen);
        string winner = bc.validators[idx].name;

        if (winner == "Alice") ++a;
        else if (winner == "Bob") ++b;
        else ++c;
    }
    cout << "Victoires ~ sur " << nRounds << " tours -> "
         << "Alice=" << a << ", Bob=" << b << ", Charlie=" << c << "\n";

    // 3) On change les mises (stake) et on observe l’impact
    cout << "\n-- Augmentation de la mise de Alice (x3) et nouvelle simulation --\n";
    bc.setStake("Alice", 120.0);  // 40 -> 120
    a=b=c=0;
    for (int i=0; i<nRounds; ++i) {
        vector<double> w; for (auto& v : bc.validators) w.push_back(max(0.0, v.stake));
        bool allZero = true; for (double x : w) if (x > 0) { allZero = false; break; }
        if (allZero) for (double& x : w) x = 1.0;
        random_device rd; mt19937 gen(rd()); discrete_distribution<int> dist(w.begin(), w.end());
        int idx = dist(gen);
        string winner = bc.validators[idx].name;

        if (winner == "Alice") ++a;
        else if (winner == "Bob") ++b;
        else ++c;
    }
    cout << "Après boost de stake -> "
         << "Alice=" << a << ", Bob=" << b << ", Charlie=" << c << " (sur " << nRounds << ")\n";

    return 0;
}
