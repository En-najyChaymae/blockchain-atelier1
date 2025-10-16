# 🧩 Atelier 1 – Exercice 1 : Arbre de Merkle
Exemple d'execution :
===== MERKLE TREE BASIQUE =====
Transactions initiales :
 - Alice -> Bob : 5 BTC
 - Bob -> Charlie : 2 BTC
 - Charlie -> Dave : 1 BTC
 - Dave -> Eve : 0.5 BTC

Niveau 1 (4 noeuds):
  45c638d854f19780
  0745a2d06694ad7c
  9f2d26490a998f09
  c4c0e57c3c86ef2f

Niveau 2 (2 noeuds):
  a5541c967d89689e
  134bdea12025f349

===== MERKLE TREE CONSTRUIT =====
Merkle Root final : 39f5cd8e5aef4be5

===== TEST : Modification d’une transaction =====
Nouvelle Merkle Root : 349c87533f8868ab
(Remarque : la racine change complètement -> preuve d'intégrité)

===== TEST : Nombre impair de transactions =====
Merkle Root (3 transactions) : 529e3c84560273e6



