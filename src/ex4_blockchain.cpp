#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <openssl/sha.h>   
using namespace std;


class Transaction {
public:
    string id;
    string sender;
    string receiver;
    double amount;

    Transaction(string id_, string sender_, string receiver_, double amount_) {
        id = id_;
        sender = sender_;
        receiver = receiver_;
        amount = amount_;
    }

    string toString() const {
        stringstream ss;
        ss << id << ":" << sender << "->" << receiver << ":" << amount;
        return ss.str();
    }
};


string sha256(const string &data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data.c_str(), data.size(), hash);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}


string computeMerkleRoot(vector<Transaction> &transactions) {
    if (transactions.empty()) return "";

    vector<string> layer;
    for (auto &t : transactions)
        layer.push_back(sha256(t.toString()));

    while (layer.size() > 1) {
        vector<string> nextLayer;
        for (size_t i = 0; i < layer.size(); i += 2) {
            string left = layer[i];
            string right = (i + 1 < layer.size()) ? layer[i + 1] : left; // si impair, on duplique
            nextLayer.push_back(sha256(left + right));
        }
        layer = nextLayer;
    }

    return layer[0];
}

class Block {
public:
    int index;
    string previousHash;
    string merkleRoot;
    string hash;
    long nonce;
    time_t timestamp;
    vector<Transaction> transactions;

    Block(int idx, string prevHash, vector<Transaction> txs) {
        index = idx;
        previousHash = prevHash;
        transactions = txs;
        timestamp = time(nullptr);
        nonce = 0;
        merkleRoot = computeMerkleRoot(transactions);
        hash = calculateHash();
    }

   
    string calculateHash() const {
        stringstream ss;
        ss << index << previousHash << merkleRoot << timestamp << nonce;
        return sha256(ss.str());
    }


    void mineBlock(int difficulty) {
        string target(difficulty, '0');
        cout << "\n⛏️  Minage du bloc " << index << "..." << endl;
        do {
            nonce++;
            hash = calculateHash();
        } while (hash.substr(0, difficulty) != target);

        cout << "✅ Bloc " << index << " miné avec succès !" << endl;
        cout << "Hash : " << hash.substr(0, 16)
             << "... | Nonce : " << nonce << endl;
    }
};


class Blockchain {
private:
    vector<Block> chain;
    int difficulty;

public:
    Blockchain(int diff = 3) {
        difficulty = diff;

        // Bloc de genèse
        vector<Transaction> genesisTx = {
            Transaction("0", "system", "Alice", 50.0)
        };

        Block genesis(0, "0", genesisTx);
        genesis.mineBlock(difficulty);
        chain.push_back(genesis);
    }

    Block getLatestBlock() const {
        return chain.back();
    }

    // Ajout d'un nouveau bloc avec minage
    void addBlock(vector<Transaction> txs) {
        Block newBlock(chain.size(), getLatestBlock().hash, txs);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    // Vérification d’intégrité de la chaîne
    bool isChainValid() {
        for (size_t i = 1; i < chain.size(); ++i) {
            if (chain[i].hash != chain[i].calculateHash()) {
                cout << "❌ Hash incorrect dans le bloc " << i << endl;
                return false;
            }
            if (chain[i].previousHash != chain[i - 1].hash) {
                cout << "❌ Chaînage invalide entre le bloc " << i - 1 << " et " << i << endl;
                return false;
            }
        }
        return true;
    }

    // Affichage de la chaîne
    void printBlockchain() {
        cout << "\n===== CHAÎNE ACTUELLE =====" << endl;
        for (auto &b : chain) {
            cout << "Bloc " << b.index 
                 << " | Hash : " << b.hash.substr(0, 16)
                 << " | Prev : " << b.previousHash.substr(0, 8)
                 << " | Tx : " << b.transactions.size() << endl;
        }
    }
};


int main() {
    cout << "===== 🧱 MINI BLOCKCHAIN (Merkle + PoW) =====\n";

    // Création d'une blockchain avec difficulté 3
    Blockchain myChain(3);

    // Bloc 1
    vector<Transaction> tx1 = {
        Transaction("1", "Alice", "Bob", 10.5),
        Transaction("2", "Bob", "Charlie", 3.0),
        Transaction("3", "Charlie", "Dave", 1.2)
    };

    // Bloc 2
    vector<Transaction> tx2 = {
        Transaction("4", "Eve", "Frank", 5.7),
        Transaction("5", "Frank", "Alice", 2.3)
    };


    myChain.addBlock(tx1);
    myChain.addBlock(tx2);

  
    myChain.printBlockchain();

   
    cout << "\nValidation de la chaîne : "
         << (myChain.isChainValid() ? "✔️  Intègre" : "❌  Corrompue")
         << endl;

    return 0;
}