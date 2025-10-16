#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional> // pour std::hash
using namespace std;

string simpleHash(const string &data) {
    hash<string> hasher;
    size_t hashValue = hasher(data);
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hashValue;
    return ss.str();
}


string merkleRoot(vector<string> transactions) {
    if (transactions.empty())
        return "";

    // 1. Calcul des hachages de base
    vector<string> layer;
    for (auto &tx : transactions)
        layer.push_back(simpleHash(tx));

    int level = 1;
    while (layer.size() > 1) {
        cout << "\nNiveau " << level++ << " (" << layer.size() << " nœuds):" << endl;
        for (auto &node : layer)
            cout << "  " << node << endl;

        vector<string> nextLayer;
        for (size_t i = 0; i < layer.size(); i += 2) {
            string left = layer[i];
            string right = (i + 1 < layer.size()) ? layer[i + 1] : left; 
            nextLayer.push_back(simpleHash(left + right));
        }
        layer = nextLayer;
    }

    cout << "\n===== MERKLE TREE CONSTRUIT =====" << endl;
    return layer[0];
}


int main() {
    vector<string> transactions = {
        "Alice -> Bob : 5 BTC",
        "Bob -> Charlie : 2 BTC",
        "Charlie -> Dave : 1 BTC",
        "Dave -> Eve : 0.5 BTC"
    };

    cout << "===== MERKLE TREE BASIQUE =====" << endl;
    cout << "Transactions initiales :" << endl;
    for (auto &tx : transactions)
        cout << " - " << tx << endl;

    string root1 = merkleRoot(transactions);
    cout << "\nMerkle Root final : " << root1 << endl;


    cout << "\n===== TEST : Modification d’une transaction =====" << endl;
    transactions[0] = "Alice -> Bob : 6 BTC"; 
    string root2 = merkleRoot(transactions);
    cout << "\nNouvelle Merkle Root : " << root2 << endl;
    cout << "(Remarque : la racine change complètement -> preuve d'intégrité)" << endl;

    
    cout << "\n===== TEST : Nombre impair de transactions =====" << endl;
    vector<string> transactions2 = {
        "A -> B : 10",
        "B -> C : 4",
        "C -> D : 2"
    };
    string root3 = merkleRoot(transactions2);
    cout << "\nMerkle Root (3 transactions) : " << root3 << endl;

    return 0;
}
