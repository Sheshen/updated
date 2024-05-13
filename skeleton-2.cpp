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

graph createGraph(pokemon* pikachu, pokemon* blastoise, int depth){
	
		int node_count = 0;

		graph match; //graph that holds the nodes of each possible move in the match
		node* initialNode = new node("initial", "initial", node_count++, pikachu, blastoise, 'p', 0, true, 1); 	//initial node is defined here
		match.addNode(initialNode);

		for(int i = 0;i < depth; i++){ //for loop is repeated for the number of times specified by the parameter from the command line prompt and limited by the max
			
			//cout << "i: " << i << endl;							//FOR DEBUG

			int temp = match.nodes.elemcount;
			for(int j = 0; j < temp; j++){
				
				//cout << "j: " << j << endl;						//FOR DEBUG
				if(match.nodes.get(j)->isleaf && match.nodes.get(j)->pikachu->hp > 0 && match.nodes.get(j)->blastoise->hp > 0){			//this is a loop that choses the leaf nodes that has the pokemons alive and makes the next attack moves and updates the leaves.
					match.nodes.get(j)->isleaf=false;	//since there are new child nodes this node will no longer be a leaf
					
					if (match.nodes.get(j)->status=='p'){
						//if it is pikachus turn perform each of pikachus available attacks

						//counting the number of avaliable attacks
						int num_of_available_attacks = 0;
						if (match.nodes.get(j)->level >= 3){num_of_available_attacks++;}
						if (match.nodes.get(j)->pikachu->pp>=10){num_of_available_attacks++;}
						if (match.nodes.get(j)->pikachu->pp>=20){num_of_available_attacks++;}
						if (match.nodes.get(j)->pikachu->pp>=25){num_of_available_attacks++;}
						//cout << num_of_available_attacks << endl;  //FOR DEBUG

						for (int p = 0; p < pikachu->attacks.elemcount; p++){
							if (pikachu->attacks.get(p)->get_accuracy() != 100){
								//creating new pokemons with calculated pp and hp values
								pokemon* hit_blastoise = new pokemon("blastoise", match.nodes.get(j)->blastoise->pp,match.nodes.get(j)->blastoise->hp-pikachu->attacks.get(p)->get_damage());
								pokemon* dodged_blastoise = new pokemon("blastoise", match.nodes.get(j)->blastoise->pp,match.nodes.get(j)->blastoise->hp);
								pokemon* newPikachu = new pokemon("pikachu", match.nodes.get(j)->pikachu->pp + pikachu->attacks.get(p)->get_pp(), match.nodes.get(j)->pikachu->hp);
								
								if (pikachu->attacks.get(p)->get_first() <= match.nodes.get(j)->level){  //checking if the first level that the attack can be used is reached
									if(newPikachu->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
									
										//creating the new node for each attack and adding to the nodes
										node* newNode_success = new node("efficient", pikachu->attacks.get(p)->get_name(), node_count++, newPikachu, hit_blastoise,'b', match.nodes.get(j)->level+1, true, match.nodes.get(j)->prob*pikachu->attacks.get(p)->get_accuracy()/100/num_of_available_attacks);
										node* newNode_fail = new node("nonefficent", pikachu->attacks.get(p)->get_name(), node_count++,newPikachu, dodged_blastoise,'b', match.nodes.get(j)->level+1, true, match.nodes.get(j)->prob*(100-pikachu->attacks.get(p)->get_accuracy())/100/num_of_available_attacks);
										match.addNode(newNode_success);
										match.addNode(newNode_fail);
										match.nodes.get(j)->child.addBack(newNode_success);
										match.nodes.get(j)->child.addBack(newNode_fail);
									}
								}
							}else{
									//creating new pokemons with calculated pp and hp values
									pokemon* hit_blastoise = new pokemon("blastoise", match.nodes.get(j)->blastoise->pp,match.nodes.get(j)->blastoise->hp-pikachu->attacks.get(p)->get_damage());
									pokemon* newPikachu = new pokemon("pikachu", match.nodes.get(j)->pikachu->pp + pikachu->attacks.get(p)->get_pp(), match.nodes.get(j)->pikachu->hp);
									
									if (pikachu->attacks.get(p)->get_first() <= match.nodes.get(j)->level){  //checking if the first level that the attack can be used is reached

										if(newPikachu->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
											//creating the new node for each attack and adding to the nodes
											node* newNode = new node("efficient", pikachu->attacks.get(p)->get_name(), node_count++, newPikachu, hit_blastoise,'b', match.nodes.get(j)->level+1, true, match.nodes.get(j)->prob*pikachu->attacks.get(p)->get_accuracy()/100/num_of_available_attacks);
											match.addNode(newNode);
											match.nodes.get(j)->child.addBack(newNode);
										} 
									} 
								}
						}

					}else if (match.nodes.get(j)->status=='b'){
						//if it is blastoits turn perform each of blastoits available attacks

						//counting the number of avaliable attacks
						int num_of_available_attacks = 0;
						if (match.nodes.get(j)->level >= 3){num_of_available_attacks++;}
						if (match.nodes.get(j)->blastoise->pp>=10){num_of_available_attacks++;}
						if (match.nodes.get(j)->blastoise->pp>=20){num_of_available_attacks++;}
						if (match.nodes.get(j)->blastoise->pp>=25){num_of_available_attacks++;}
						//cout << num_of_available_attacks << endl;			//FOR DEBUG

						for (int b = 0; b < blastoise->attacks.elemcount; b++){
							if (blastoise->attacks.get(b)->get_accuracy() != 100){
								//creating new pokemons with calculated pp and hp values
								pokemon* hit_pikachu = new pokemon("pikachu", match.nodes.get(j)->pikachu->pp,match.nodes.get(j)->pikachu->hp-blastoise->attacks.get(b)->get_damage());
								pokemon* dogded_pikachu = new pokemon("pikachu", match.nodes.get(j)->pikachu->pp,match.nodes.get(j)->pikachu->hp);
								pokemon* newBlastoise = new pokemon("blastoise", match.nodes.get(j)->blastoise->pp + blastoise->attacks.get(b)->get_pp(), match.nodes.get(j)->blastoise->hp);
								
								if (blastoise->attacks.get(b)->get_first() <= match.nodes.get(j)->level){  //checking if the first level that the attack can be used is reached
									if(newBlastoise->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
										//creating the new node for each attack and adding to the nodes
										node* newNode_success = new node("efficient", blastoise->attacks.get(b)->get_name(), node_count++, hit_pikachu, newBlastoise,'p', match.nodes.get(j)->level+1, true, match.nodes.get(j)->prob*blastoise->attacks.get(b)->get_accuracy()/100/num_of_available_attacks);
										node* newNode_fail = new node("nonefficient", blastoise->attacks.get(b)->get_name(), node_count++, dogded_pikachu, newBlastoise,'p', match.nodes.get(j)->level+1, true, match.nodes.get(j)->prob*(100-blastoise->attacks.get(b)->get_accuracy())/100/num_of_available_attacks);
										match.addNode(newNode_success);
										match.addNode(newNode_fail);
										match.nodes.get(j)->child.addBack(newNode_success);
										match.nodes.get(j)->child.addBack(newNode_fail);
									}
							}}else {
								//creating new pokemons with calculated pp and hp values
								pokemon* hit_pikachu = new pokemon("pikachu", match.nodes.get(j)->pikachu->pp,match.nodes.get(j)->pikachu->hp-blastoise->attacks.get(b)->get_damage());
								pokemon* newBlastoise = new pokemon("blastoise", match.nodes.get(j)->blastoise->pp + blastoise->attacks.get(b)->get_pp(), match.nodes.get(j)->blastoise->hp);
								
								if (blastoise->attacks.get(b)->get_first() <= match.nodes.get(j)->level){  //checking if the first level that the attack can be used is reached
									if(newBlastoise->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
										//creating the new node for each attack and adding to the nodes
										node* newNode = new node("efficient", blastoise->attacks.get(b)->get_name(), node_count++, hit_pikachu, newBlastoise,'p', match.nodes.get(j)->level+1, true, match.nodes.get(j)->prob*blastoise->attacks.get(b)->get_accuracy()/100/num_of_available_attacks);
										match.addNode(newNode);								
										match.nodes.get(j)->child.addBack(newNode);
									}
								}
							}
						}


					}

				}//else {cout << "leaf olmayan bir nodu geçtim." << endl;} //FOR DEBUG
			}

			
		}

	return match;
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