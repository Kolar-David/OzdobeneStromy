# Uživatelská dokumentace

Tento modul má na starost práci s binárními vyhledávacími AVL stromy. 
Obsahuje třídu BinaryTree, která umožňuje v základu standartní operace s vyhledávacími stromy jako vkládání, mazání, nalezení nejbližsího nižšího a vyššího klíče, či minima a maxima.
Stromy odvozené z této třídy lze používat jako množinu či jako mapu. 
Hodnoty ve vrcholech navíc mohou být dynamicky přepočítávány podle hodnot jejich synů (lze tak udržovat například v každém vrcholu údaje o minimu v jeho podstromu) podle uživatelem dodané funkce.
Pokud uživatel dodá další funkce, je možné provádět i intervalové dotazy podle klíčů.


Objekt BinaryTree lze vytvořit následujícím způsobem:

Set: 

`BinaryTree<Key> strom;`

Mapa:

`BinaryTree<Key, Val> strom;`

## Funkce BinaryTree

`Node<Key, Val> *find(Key key)`

Jako vstup obdrží klíč a vrátí pointer k vrcholu s odpovídajícím klíčem. Pokud nic nenalezne, vrátí `nullptr`.

`Node<Key, Val> *max()`

Vrátí pointer s maximem. Pokud je strom prázný, vrátí `nullptr`.

`Node<Key, Val> *nextBigger(Key key)`

Vrátí poiner nejmenšího prvku s klíčem ostře větším než `key`. Pokud žádný neexistuje, vrátí `nullptr`.
      
`Node<Key, Val> *nextSmaller(Key key)`

Vrátí poiner největšího prvku s klíčem ostře menším než `key`. Pokud žádný neexistuje, vrátí `nullptr`.
       
`virtual void insert(Key key, Val val = Val())`

Obdrží klíč a volitelně i hodnotu, vytvoří z nich nový vrchol a ten uloží do stromu. Pokud se ve stromu již nějaký vrchol se stejným klíčem nachází, nechá strom netknutý.

`void remove(Key key) `

Smaže vrchol s odpovídajícím klíčem. Pokud neexistuje, funkce neudělá nic.

`pair<bool, Q> rangeQuery(Key start, Key end)`

Funkce obdrží uzavřený interval klíčů a vrátí `pair` kde první složka je `bool` obsahující `true`, pokud obsahoval interval nějaké klíče (jinak `false`) a hodnotu daného intervalu podle uživatelem dodané asociativní operace (viz později). 

`void refresh(Key key)`

Pokud uživatel upraví zavoláním `find` nějaký vrchol, může se rozhodit výpočet pomocných hodnot ve vrcholech. 
To lze napravit zavoláním funkce `refresh`, která aktualizuje vrcholy na cestě mezi vrcholem s daným klíčem a kořenem.


## Modifikace stromu

Modifikace stromu se provádí výhradně pomocí dědičnosti.

Při dědičnosti je nutné přepsat tyto funkce:

`virtual void updateValue(Node<Key, Val> *node)`

Tato funkce má na starost aktualizaci hodnot na základě synů daného vrcholu. Můžete předpokládat, že pointer bude vždy ukazovat na validní vrchol.
  
`virtual Q evaluateSubtree(Node<Key, Val> *node)`

Tato funkce vyhodnotí podstrom daného vrcholu a přiřadí mu hodnotu typu `Q`. 

`virtual Q evaluateNode(Node<Key, Val> *node)`

Tato funkce vyhodnotí daný vrchol a přiřadí mu hodnotu typu `Q`. 
       
`virtual Q queryOperator(Q q1, Q q2)`

Tato funkce reprezentuje uživatelem vybranou asociativní operaci, která z 2 hodnot typu `Q` vytvoří novou hodnotu tohoto typu.

Zde můžete vidět příklad modifikace stromu, díky které lze dělat dotazy na minima hodnot ve vybraném intervalu klíčů:

```
// Datovy typ hodnoty ve vrcholech
struct NodeValue {
    int minimum;
    int val;
    NodeValue(int val=1)
            : minimum(val), val(val) {}
};
class MinimumTree : public BinaryTree<int, NodeValue, int> {
    int evaluateNode(Node<int, NodeValue> *node) override {
        return node->value.val;
    }
    int evaluateSubtree(Node<int, NodeValue> *node) override {
        return node->value.minimum;
    }
    int queryOperator(int q1, int q2) override {
        if (q1 <= q2) {
            return q1;
        };
        return q2;
    }
    void updateValue(Node<int, NodeValue> *node) override {
        node->value.minimum = node->value.val;
        if (node->left != nullptr) {
            if (node->value.minimum > node->left->value.minimum) {
                node->value.minimum = node->left->value.minimum;
            }
        }
        if (node->right != nullptr) {
            if (node->value.minimum > node->right->value.minimum) {
                node->value.minimum = node->right->value.minimum;
            }
        }
    }


};
```

Klíče odpovídají celým číslům, typ `NodeValue` je ukládán ve vrcholech a obsahuje minimum daného podstromu a hodnotu v daném vrcholu. Typ `Q` odpovídá celým číslům.

# Technická dokumentace

Třída `BinaryTree` odpovídá rekurzivní implementaci AVL stromu. 
Při její tvorbě jsem čerpal z knihy Průvodce labyrintem algoritmů, kde jsou principy jednotlivých operací popsány. Uvnitř se během standartních operací zároveň dochází i k aktualizaci pomocných hodnot vrcholů. Každý vrchol navíc udržuje interval klíčů v jeho podstromu, aby bylo možné provádět intervalové dotazy.
