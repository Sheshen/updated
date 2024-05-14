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

void createGraphRecursively(int depth, node* head){

		pokemon* blastoise = head->blastoise;
		pokemon* pikachu = head->pikachu;
		int node_count = head->num;

		//cout << head->blastoise << " - " << head->child.elemcount << " - " << head->curattack << " - " << head->ifef << " - " << head->isleaf << " - " << head->level << " - " << head->num << " - " << head->pikachu << " - " << head->prob << " - " << head->status << endl;
		cout << head->isleaf << " pikachu hp: " << pikachu->hp << " blastoise hp: " << blastoise->hp << " level: "<< head->level << " depth: "<< depth << endl;
		if(head->isleaf && pikachu->hp > 0 && blastoise->hp > 0 && head->level < depth){
			cout << "buradayım" << endl;
			head->isleaf=false;	//since there are new child nodes this node will no longer be a leaf
			
			if (head->status=='p'){
				//if it is pikachus turn perform each of pikachus available attacks

				//counting the number of avaliable attacks
				int num_of_available_attacks = 0;
				if (head->level >= 3){num_of_available_attacks++;}
				if (pikachu->pp>=10){num_of_available_attacks++;}
				if (pikachu->pp>=20){num_of_available_attacks++;}
				if (pikachu->pp>=25){num_of_available_attacks++;}
				//cout << num_of_available_attacks << endl;  //FOR DEBUG

				for (int p = 0; p < pikachu->attacks.elemcount; p++){
					if (pikachu->attacks.get(p)->get_accuracy() != 100){
						//creating new pokemons with calculated pp and hp values
						pokemon* hit_blastoise = new pokemon("blastoise", blastoise->pp,blastoise->hp-pikachu->attacks.get(p)->get_damage());
						pokemon* dodged_blastoise = new pokemon("blastoise", blastoise->pp,blastoise->hp);
						pokemon* newPikachu = new pokemon("pikachu", pikachu->pp + pikachu->attacks.get(p)->get_pp(), pikachu->hp);
						
						if (pikachu->attacks.get(p)->get_first() <= head->level){  //checking if the first level that the attack can be used is reached
							if(newPikachu->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
							
								//creating the new node for each attack and adding to the nodes
								node* newNode_success = new node("efficient", pikachu->attacks.get(p)->get_name(), node_count++, newPikachu, hit_blastoise,'b', head->level+1, true, head->prob*pikachu->attacks.get(p)->get_accuracy()/100/num_of_available_attacks);
								node* newNode_fail = new node("nonefficent", pikachu->attacks.get(p)->get_name(), node_count++,newPikachu, dodged_blastoise,'b', head->level+1, true, head->prob*(100-pikachu->attacks.get(p)->get_accuracy())/100/num_of_available_attacks);
								cout << "hatta burada p" << endl;
								createGraphRecursively(depth, newNode_success);
								createGraphRecursively(depth, newNode_fail);
								head->child.addBack(newNode_success);
								head->child.addBack(newNode_fail);

							}
						}
					}else{
							//creating new pokemons with calculated pp and hp values
							pokemon* hit_blastoise = new pokemon("blastoise", blastoise->pp,blastoise->hp-pikachu->attacks.get(p)->get_damage());
							pokemon* newPikachu = new pokemon("pikachu", pikachu->pp + pikachu->attacks.get(p)->get_pp(), pikachu->hp);
							
							if (pikachu->attacks.get(p)->get_first() <= head->level){  //checking if the first level that the attack can be used is reached

								if(newPikachu->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
									//creating the new node for each attack and adding to the nodes
									node* newNode = new node("efficient", pikachu->attacks.get(p)->get_name(), node_count++, newPikachu, hit_blastoise,'b', head->level+1, true, head->prob*pikachu->attacks.get(p)->get_accuracy()/100/num_of_available_attacks);
									head->child.addBack(newNode);
									createGraphRecursively(depth, newNode);
								} 
							} 
						}
				}

			}else if (head->status=='b'){
				//if it is blastoits turn perform each of blastoits available attacks

				
				//counting the number of avaliable attacks
				int num_of_available_attacks = 0;
				if (head->level >= 3){num_of_available_attacks++;}
				if (blastoise->pp>=10){num_of_available_attacks++;}
				if (blastoise->pp>=20){num_of_available_attacks++;}
				if (blastoise->pp>=25){num_of_available_attacks++;}
				//cout << num_of_available_attacks << endl;			//FOR DEBUG


				for (int b = 0; b < blastoise->attacks.elemcount; b++){
					if (blastoise->attacks.get(b)->get_accuracy() != 100){
						//creating new pokemons with calculated pp and hp values
						pokemon* hit_pikachu = new pokemon("pikachu", pikachu->pp,pikachu->hp-blastoise->attacks.get(b)->get_damage());
						pokemon* dogded_pikachu = new pokemon("pikachu", pikachu->pp,pikachu->hp);
						pokemon* newBlastoise = new pokemon("blastoise", blastoise->pp + blastoise->attacks.get(b)->get_pp(), blastoise->hp);
						
						if (blastoise->attacks.get(b)->get_first() <= head->level){  //checking if the first level that the attack can be used is reached
							if(newBlastoise->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
								//creating the new node for each attack and adding to the nodes
								node* newNode_success = new node("efficient", blastoise->attacks.get(b)->get_name(), node_count++, hit_pikachu, newBlastoise,'p', head->level+1, true, head->prob*blastoise->attacks.get(b)->get_accuracy()/100/num_of_available_attacks);
								node* newNode_fail = new node("nonefficient", blastoise->attacks.get(b)->get_name(), node_count++, dogded_pikachu, newBlastoise,'p', head->level+1, true, head->prob*(100-blastoise->attacks.get(b)->get_accuracy())/100/num_of_available_attacks);
								head->child.addBack(newNode_success);
								head->child.addBack(newNode_fail);
								cout << "hatta burada b" << endl;
								createGraphRecursively(depth, newNode_success);
								createGraphRecursively(depth, newNode_fail);
							}
					}}else {
						//creating new pokemons with calculated pp and hp values
						pokemon* hit_pikachu = new pokemon("pikachu", pikachu->pp,pikachu->hp-blastoise->attacks.get(b)->get_damage());
						pokemon* newBlastoise = new pokemon("blastoise", blastoise->pp + blastoise->attacks.get(b)->get_pp(), blastoise->hp);
						
						if (blastoise->attacks.get(b)->get_first() <= head->level){  //checking if the first level that the attack can be used is reached
							if(newBlastoise->pp >= 0){	//if pp has fallen below zero the move is not permissable so do not create the node 
								//creating the new node for each attack and adding to the nodes
								node* newNode = new node("efficient", blastoise->attacks.get(b)->get_name(), node_count++, hit_pikachu, newBlastoise,'p', head->level+1, true, head->prob*blastoise->attacks.get(b)->get_accuracy()/100/num_of_available_attacks);
								head->child.addBack(newNode);
								createGraphRecursively(depth, newNode);
							}
						}
					}
				}


			}

		}

}

node* createGraph(pokemon* pikachu, pokemon* blastoise, int depth){
	
		int node_count = 0;

		node* initialNode = new node("initial", "initial", node_count++, pikachu, blastoise, 'p', 0, true, 1); 	//initial node is defined here
		createGraphRecursively(depth, initialNode);

		return initialNode;
}

void printLastLayer(node* head, int max, int depth=0){	
	if(depth == max){
        cout << "P_HP:" << head->pikachu->hp << " P_PP:" << head->pikachu->pp << " B_HP:" << head->blastoise->hp << " B_PP:" << head->blastoise->pp << " PROB:" << head->prob << "\n";
	}else {
		cout << "depth: " << depth<< ", max: " << max << ", child count: "<<head->child.elemcount << endl;
		Node<node*>* traversal_node = head->child.head;
        for (int i = 0; i < head->child.elemcount; i++) {
            printLastLayer(traversal_node->data, max, depth + 1);
			traversal_node = traversal_node->next;
        }
	}
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
		node* head = createGraph(pikachu, blastoise, max);
		//printLastLayer(head, max);
	}else if(part == "part2"){
		pika_or_blastoise = argv[2];
		node* head = createGraph(pikachu, blastoise, max);
	}else
		return -1;



}