//////////////////////////////////////////
// The code from Zeynep Gurler's HW is altered for the application. Sincere thanks!
//////////////////////////////////////////
//Mehmet Abdullah Şeşen 150220029 Spring 2024

#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>
#include "doublelinklist.h"

using namespace std;

struct attack{   // class for pokemon attacks
	private:
		string name;
		int pp;
		int accuracy;
		int damage;
		int first;
	public:
		attack(const string&, int, int, int, int);
		int get_first();
		int get_accuracy();
		int get_damage();
		int get_pp();
		string get_name();
};

attack::attack(const string& name_in, int p, int a, int d, int f){
	name = name_in;
	pp = p;
	accuracy = a;
	damage = d;
	first = f;
}

string attack::get_name(){
	return name;
}

int attack::get_pp(){
	return pp;
}

int attack::get_damage(){
	return damage;
}

int attack::get_first(){
	return first;
}

int attack::get_accuracy(){
	return accuracy;
}

struct pokemon{  // class for pokemons
	int pp;
	int hp;
	string name;
	DoublyList<attack*> attacks;
	pokemon(const string &name_in, int, int); 
	string get_name();
	void set(char, int to_reduce);
};

pokemon::pokemon(const string &name_in, int p, int h){
	name = name_in;
	pp = p;
	hp = h;
}

void pokemon::set(char which, int to_reduce){
	if(which == 'p'){
		pp += to_reduce;
	}else{
		hp -= to_reduce;
	}
}

struct node{  // class for the nodes of the graph
	string ifef;  // attack is efficient or nonefficent
	string curattack;  // attack name
	int num;  // node num
	pokemon* pikachu;  // current pikachu
	pokemon* blastoise;  // currrent blastoise
	char status;  // whose turn?
	int level;  // level of the node
	bool isleaf;  // node is a leaf or not
	double prob;  // prob of the node happening
	 
	DoublyList<node*> child;  // children nodes of the node
	
	//Member functions goes there...
node(const string& ifef_in, const string& curattack_in, int num_in, pokemon* pikachu_in, pokemon* blastoise_in, char status_in, int level_in, bool isleaf_in, double prob_in);

};

node::node(const string& ifef_in, const string& curattack_in, int num_in, pokemon* pikachu_in, pokemon* blastoise_in, char status_in, int level_in, bool isleaf_in, double prob_in){
	    ifef = ifef_in;
        curattack = curattack_in;
        num = num_in;
        pikachu = pikachu_in;
        blastoise = blastoise_in;
        status = status_in;
        level = level_in;
        isleaf = isleaf_in;
        prob = prob_in;
}

struct graph{
	//Graph definitions goes there...
	DoublyList<node*> nodes; // List of nodes in the graph

    // Constructor
    graph();

    // Method to add a node to the graph
    void addNode(node* newNode);
};

graph::graph() {}

void graph::addNode(node* newNode) {
        nodes.addBack(newNode);
}

graph createGraphRecursively(pokemon* pikachu, pokemon* blastoise, int depth, node* parentNode, int& node_count) {
    // Base case: depth is 0 or one of the Pokemon's HP is zero
    if (depth == 0 || pikachu->hp <= 0 || blastoise->hp <= 0) {
        graph g;
        g.addNode(parentNode);
        return g;
    }

    graph g;
    for (int i = 0; i < parentNode->pikachu->attacks.elemcount; ++i) {
        attack* pikachu_attack = parentNode->pikachu->attacks.get(i);
        // Create nodes for successful and failed attacks
        if (pikachu_attack->get_first() <= parentNode->level) {
            pokemon* newPikachu = new pokemon("pikachu", parentNode->pikachu->pp + pikachu_attack->get_pp(), parentNode->pikachu->hp);
            pokemon* hit_blastoise = new pokemon("blastoise", parentNode->blastoise->pp, parentNode->blastoise->hp - pikachu_attack->get_damage());
            pokemon* dodged_blastoise = new pokemon("blastoise", parentNode->blastoise->pp, parentNode->blastoise->hp);
            node* newNode_success = new node("efficient", pikachu_attack->get_name(), node_count++, newPikachu, hit_blastoise, 'b', parentNode->level + 1, true, parentNode->prob * pikachu_attack->get_accuracy() / 100);
            node* newNode_fail = new node("nonefficient", pikachu_attack->get_name(), node_count++, newPikachu, dodged_blastoise, 'b', parentNode->level + 1, true, parentNode->prob * (100 - pikachu_attack->get_accuracy()) / 100);
            g.addNode(newNode_success);
            g.addNode(newNode_fail);
            parentNode->child.addBack(newNode_success);
            parentNode->child.addBack(newNode_fail);
            graph subGraph = createGraphRecursively(pikachu, blastoise, depth - 1, newNode_success, node_count);
            // Concatenate nodes from subgraph to the main graph
            for (int j = 0; j < subGraph.nodes.elemcount; ++j) {
                g.addNode(subGraph.nodes.get(j));
            }
        }
    }
    return g;
}


void printLastLayer(graph match, int max){	
	for(int d = 0; d < match.nodes.elemcount; d++){
			if (match.nodes.get(d)->level == max){
					cout << "P_HP:"<< match.nodes.get(d)->pikachu->hp << " P_PP:" << match.nodes.get(d)->pikachu->pp << " B_HP:" << match.nodes.get(d)->blastoise->hp << " B_PP:" << match.nodes.get(d)->blastoise->pp << " PROB:"<<match.nodes.get(d)->prob << endl;
			}
		}
}

void dfs(node* currNode, DoublyList<node*>& path, DoublyList<node*>& easiestPath, char* pika_or_blastoise) {
	
	if (pika_or_blastoise = "pikachu"){
		if (currNode->pikachu->hp <= 0){
			if (easiestPath.elemcount == 0 || path.elemcount < easiestPath.elemcount) {
            easiestPath = path;
        	}
        	return;
		}

	}else if (pika_or_blastoise = "blastoise"){
		if (currNode->blastoise->hp <= 0){
			if (easiestPath.elemcount == 0 || path.elemcount < easiestPath.elemcount) {
            easiestPath = path;
        	}
        	return;
		}
	}

    for (int i = 0; i < currNode->child.elemcount; ++i) {
        node* child = currNode->child.get(i);
        path.addBack(child);
        dfs(child, path, easiestPath, pika_or_blastoise);
        path.removeBack();
    }
}

void easiestPath(char* pika_or_blastoise, graph match) {
    DoublyList<node*> path;
    DoublyList<node*> easiestPath;
    node* initialNode = match.nodes.get(0);
    path.addBack(initialNode);
    dfs(initialNode, path, easiestPath, pika_or_blastoise);

	cout << "Easiest path: ";
    for (int i = 0; i < easiestPath.elemcount; ++i) {
        node* n = easiestPath.get(i);
        cout << n->curattack << " -> ";
    }
    cout << "End" << endl;

}

int main(int argc, char** argv){
	string part = argv[1];
	int max;
	string which;
	char* pika_or_blastoise;

	if(part == "part1"){
		max = atoi(argv[2]);

	}else if(part == "part2"){
		pika_or_blastoise = argv[2];
	}
	else
		return -1;
		
	pokemon* pikachu = new pokemon("pikachu", 100, 200);
	pokemon* blastoise = new pokemon("blastoise", 100, 200);
	
	attack* thundershock = new attack("thundershock", -10, 100, 40, 0);
	attack* skullbash = new attack("skullbash", -15, 70, 50, 0);
	attack* slam = new attack("slam", -20, 80, 60, 0);
	attack* pskip = new attack("skip", 100, 100, 0, 3);
	pikachu->attacks.addBack(thundershock);
	pikachu->attacks.addBack(skullbash);
	pikachu->attacks.addBack(slam);
	pikachu->attacks.addBack(pskip);

	attack* tackle = new attack("tackle", -10, 100, 30, 0);
	attack* watergun = new attack("watergun", -20, 100, 40, 0);
	attack* bite = new attack("bite", -25, 100, 60, 0);
	attack* bskip = new attack("skip", 100, 100, 0, 3);
	blastoise->attacks.addBack(tackle);
	blastoise->attacks.addBack(watergun);
	blastoise->attacks.addBack(bite);
	blastoise->attacks.addBack(bskip);



	
	if(part == "part1"){		
		graph match = createGraph(pikachu, blastoise, max);
		printLastLayer(match, max);
	}else if(part == "part2"){
		pika_or_blastoise = argv[2];
		graph match = createGraph(pikachu, blastoise, 8);
		easiestPath(pika_or_blastoise, match);
	}else
		return -1;



}