#include <iostream>
#include <fstream>
using namespace std;

ifstream in("grader_test14.in");
ofstream out("abce.out");

struct Node {
    int value;
    Node* parent;
    Node* left;
    Node* right;
};

class SplayTreeBST {
    Node* root;
public:
    SplayTreeBST(){
        root = NULL;
    }
    Node* getRoot(){return root;} //getter pt radacina

    Node* newN(int x){
        Node* Nod = new Node();
        Nod->value = x;
        Nod->parent = NULL;
        Nod->left = NULL;
        Nod->right = NULL;
        return Nod;
    }

    Node* Successor(Node* nod){
        if (nod != NULL) {
            if (nod->right != NULL){ // daca are fiu drept, gasim cel mai la stanga urmas al acestui fiu
                nod = nod->right;
                while (nod->left != NULL)
                    nod = nod->left;
                //Splay(nod);
                return nod;
            }
            else{
                if (nod->parent != NULL){
                    Node* copyNod = nod;
                    while(nod->parent != NULL){
                        nod = nod->parent;
                        if (nod->value > copyNod->value){ // nodul parametru face parte din subarborele drept al nodului curent, deci am gasit succesorul
                            //Splay(nod);
                            return nod;
                        }
                    }
                    //Splay(copyNod);
                    return copyNod; //  nu avem un stramos fata de care nodul sa se afle in subarborele stang, deci returnam nodul dat ca parametru
                }
                else{
                    //Splay(nod);
                    return nod; //it does not have a right descendant nor a parent, so the "nod" has the highest value in the tree
                }
            }
        }
    }
    int BiggerOrEqual(int x){
        Node* nod = Find2(x);
        if (nod != NULL){
            if (x == nod->value){ //valoarea x se afla in arbore, x >= x
                Splay(nod); // facem splay pe nodul accesat
                return x;
            }

            // "nod" este nodul care ar fi parintele lui x daca am adauga x, iar acum cautam succesorul lui x
            //presupunand ca acesta face parte din arbore si e o frunza
            while (nod != NULL){
                if (nod->value > x){ //value x is in the left subtree of "nod"
                    Splay(nod);
                    return nod->value;
                }
                nod = nod->parent;
            }
            //nu am gasit un nod pentru care x sa fie in subarborele stang al sau, deci x nu are o val mai mare decat el in arbore si nici nu se afla in acesta
            return 1000000001; //  x is bigger than each value in the tree
        }
    }

    int SmallerOrEqual(int x){
        Node* nod = Find2(x);
        if (nod != NULL){
            if (x == nod->value){
                Splay(nod);
                return x;
            }

            while (nod != NULL) {
                if (nod->value < x){ //value x is in the right subtree of "nod"
                    Splay(nod);
                    return nod->value;
                }
                nod = nod->parent;
            }
                return 1000000001;
        }
    }

    void ToLeft(Node* nod){
        Node* newSubTreeRoot = nod->right; //fiul drept al parametrului va fi noua radacina a subarborelui, ia locul parametrului
        nod->right = newSubTreeRoot->left;
          //acum creem legatura dintre parintele parametrului si noua subradacina
        if (nod->parent == NULL) root = newSubTreeRoot; // subarborele este de fapt intregul arbore
        else if (nod == nod->parent->left) nod->parent->left = newSubTreeRoot; // parametrul era fiu stang
        else nod->parent->right = newSubTreeRoot;

        if (newSubTreeRoot->left != NULL)  // daca noua subradacina are fiu stang, parintele lui va deveni parametrul
            newSubTreeRoot->left->parent = nod;

        newSubTreeRoot->parent = nod->parent; //parintele parametrului devine acum parintele noii radacini

        newSubTreeRoot->left = nod;  //parametrul devine copilul sau stang
        nod->parent = newSubTreeRoot;
    }

    void ToRight(Node* nod){  //ia ca parametru nodul care trebuie rotit
        Node* newSubTreeRoot = nod->left;
        nod->left = newSubTreeRoot->right;

        if (nod->parent == NULL) root = newSubTreeRoot;
        else if (nod == nod->parent->left) nod->parent->left = newSubTreeRoot;
        else nod->parent->right = newSubTreeRoot;

        if (newSubTreeRoot->right != NULL) // daca noua subradacina are fiu drept, parintele lui va deveni parametrul
            newSubTreeRoot->right->parent = nod;

        newSubTreeRoot->parent = nod->parent;
        newSubTreeRoot->right = nod;
        nod->parent = newSubTreeRoot;
    }

    void Splay(Node* nod){ // ducem nodul dat ca parametru in radacina
        while(root->value != nod->value){ // cat timp nodul pe care se face splay nu a ajuns in radacina
            if (nod->parent == root){ // daca e copilul radacinii e nevoie de o singura rotire
                if (root->value > nod->value) ToRight(root);//zig situation
                else ToLeft(root); //zag situation
            }
            else {
                Node* aux1 = nod->parent;
                Node* aux2 = aux1->parent; //parintele parintelui nodului dat ca parametru
                if (nod->value > aux1->value && aux1->value > aux2->value) // nodul e fiul drept al unui fiu drept
                    ToLeft(aux2), ToLeft(aux1);
                else if (nod->value < aux1->value && aux1->value < aux2->value)
                    ToRight(aux2), ToRight(aux1); //zig zig
                else if (nod->value > aux1->value && aux1->value < aux2->value)
                    ToLeft(aux1),ToRight(aux2);
                else
                    ToRight(aux1), ToLeft(aux2);
            }
        }
    }

    Node* Find2(int x){ // daca nu exista nod cu valoarea x returneaza valoarea ultimului nod accesat, care e o frunza, sau NULL daca arborele e gol
        if (root == NULL){
            return NULL;
        }
        Node* aux1 = root;
        Node* aux2 = NULL;
        while (aux1 != NULL) {
            aux2 = aux1;
            if (x < aux1->value) aux1 = aux1->left;
            else if (x > aux1->value) aux1 = aux1->right;
            else {
                return aux1; // x == aux1->value, so we found value x
            }
        }
        return aux2;
    }

    void Find(int x){
        if (root == NULL){
            out << 0 << '\n';
            return;
        }

        Node* aux1 = root;
        Node* aux2 = NULL;
        while (aux1 != NULL){
            aux2 = aux1;
            if (x < aux1->value) aux1 = aux1->left; //mergem pe ramura stanga
            else if (x > aux1->value) aux1 = aux1->right;
            else {
                Splay(aux1); // daca a gasit nodul cu valoarea x face splay si returnam 1
                out << 1 << '\n';
                return; // x == aux1->value, so we found value x
            }
        }
        out << 0 << '\n';
        Splay(aux2); // n am gasit valoarea x, facem splay pe ultimul nod vizitat, care va fi o frunza
    }

    void Add(int x){
        Node* nod = newN(x);
        if (root == NULL){
            //tree is empty, the node to be added is placed in the root
            root = nod;
            return;
        }
        Node* aux1 = root;
        Node* aux2 = NULL;
        while (aux1 != NULL){
            aux2 = aux1;
            if (x < aux1->value) aux1 = aux1->left;
            else if (x > aux1->value) aux1 = aux1->right;
            else {
                Splay(aux1);
                return; // x == aux1->value, valoarea e deja in arbore, facem splay pe ea
            }
        }
        nod->parent = aux2; // n am gasit valoarea x in arbore; ultimul nod accesat(o frunza) devine parinte pt noul nod adaugat cu valoarea x

        if (nod->value < aux2->value) aux2->left = nod; // decidem daca e copil drept sau stang
        else aux2->right = nod;
        Splay(nod); // facem splay pe nodul adaugat
    }

    /*void ShowInterval(int X, int Y){
        int Z = BiggerOrEqual(X);
        while (Z <= Y ){
            cout << Z << ' ';
            Z = BiggerOrEqual(Z + 1);
        }
    }*/
    void ShowInterval(int X, int Y){
        int Z = BiggerOrEqual(X); //prima valoare care se afla in arbore si e >= X
        Node* nod = Find2(Z);
        while (nod->value <= Y){
            out << nod->value << ' ';
            Node* successor = Successor(nod);
            if (nod->value == successor->value){
                break;
            }
            nod = successor;
        }
        out << '\n';
    }

    void Delete(int X, bool splay){
        Node* nod = Find2(X);
        if (nod->value == X){
            if (nod->right == NULL && nod->left == NULL){ // the node does not have any descendants
                if (nod->parent == NULL){ //daca nodul e radacina inseamna ca s-a golit arborele
                    delete root;
                    root = NULL;
                }
                else{
                    Node* ancestor = nod->parent;
                    if (ancestor->value > nod->value) //nodul care trebuie sters este fiu stang, deci parintele nu va mai avea fiu stang
                        ancestor->left = NULL;
                    else ancestor->right = NULL;
                    delete nod;
                    if (splay) Splay(ancestor); //splay pe parintele nodului care a fost sters
                }
            }
            else if (nod->right != NULL && nod->left != NULL){ // are ambii copii, deci ii gasim succesorul, punem in nod valoarea acestuia
                    //iar apoi apelam stergerea pentru pozitia initiala a valorii sucesorului
                Node* successor = Successor(nod); // successor cannot have a left child
                int val = successor->value;
                Delete(val, 0); //al doilea parametru este 0 pentru ca splay ul trebuie facut pe parintele nodului dat ca parametru, nu si pe parintele fostei pozitii a succesorului
                nod->value = val;
                if (nod->parent != NULL) //daca nodul e radacina nu mai e nevoie de splay
                    Splay(nod->parent);
            }
            else { //node has one descendant
                Node* descendant;
                if (nod->right != NULL) descendant = nod->right;
                else descendant = nod->left; //retinem care este unicul copil al nodului
                if (nod->parent == NULL){ //daca nodul de sters este radacina, atunci fiul sau devine radacina
                    root = descendant;
                    root->parent = NULL;
                    delete nod;
                }
                else{
                    Node* ancestor = nod->parent; //retinem parintele
                    //acum creem legatura dintre parintele si copilul nodului, apoi stergem nodul
                    if (ancestor->value > nod->value)
                        ancestor->left = descendant;
                    else ancestor->right = descendant;
                    descendant->parent = ancestor;
                    delete nod;
                    if (splay) Splay(ancestor);
                }
            }
        }
        else{ //else the value is not in the tree so we do not have to delete anything
            Splay(nod); // facem splay pe nodul la care am
        }

    }

     void ShowSRD(Node* n){ //in order
        if (n != NULL){
            ShowSRD(n->left);
            if (n->value == root->value) cout << "ROOT: ";
            cout <<"value: " << n->value;
            if (n->parent != NULL)
                cout << " parent: " << n->parent->value;
            if (n->right != NULL)
                cout << " right: " << n->right->value;
            if (n->left != NULL)
                cout << " left: " << n->left->value << '\n';
            ShowSRD(n->right);
        }
    }
};

int Q, type, X, Y;
int main()
{
    SplayTreeBST S;
    /*
    S.Add(15);
    S.Add(9);
    S.Add(7);
    //S.Add(8);
    //S.Add(10);
    //S.Add(17);
    //S.Add(7);
    //S.Add(13);
    //S.Add(16);
    //S.Add2(6);
    //S.Add2(8);
    //S.Add2(9);
    S.Delete(8, 1);
    //S.Add(10);
    //S.Add(8);
    //S.Add(7);
    //S.Delete(17, 1);
    //S.ShowInterval(9, 20);
    //cout << S.BiggerOrEqual(11) << '\n';
    //cout << S.BiggerOrEqual(6);
    //S.Add(9);
    //S.Add(8);
    //S.Find(4);
    S.ShowSRD(S.getRoot());
    //S.Find(8);
    */
    in >> Q;
    for (int i = 0; i < Q; i++){
        in >> type >> X;
        switch (type){
            case 1: {
                S.Add(X);
            break;}
            case 2: {
                S.Delete(X, 1);
            break;}
            case 3: {
                S.Find(X);
            break;}
            case 4: {
                out << S.SmallerOrEqual(X) << '\n';
            break;}
            case 5: {
                out << S.BiggerOrEqual(X) << '\n';
            break;}
            case 6: {
                in >> Y;
                S.ShowInterval(X, Y);
            break;}
        }
    }
    in.close();
    out.close();
    return 0;
}
