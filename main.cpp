//DS Assignment 2 - AVL 
//ROLL No- 214101011
#include<bits/stdc++.h>
#include<fstream>
using namespace std;

static int node_id = 0;                 // will be used for creating .Dot code of graph
class Node{
	private:
		int key;
		int bf;                       // balance factor bf = height(left subtree) – height(right subtree)
		Node *left, *right;
		int nodeID ;
	friend class avl_Tree;
	public:
		Node(){}
		Node(Node*){}
		Node(int k){  
			bf = 0;  
			key = k;         
			left = NULL;
			right= NULL;
			nodeID = node_id++;    //when new node is created , it assigns a new_node it and increments static node_id by 1.
		}
};

class avl_Tree{
	private:
		void deleteTree(Node* root);    // This will be called by the destructor
	public:
		Node *head;
		avl_Tree(){
			head = new Node();          //right of this will point to root of tree
			head->left = NULL;          //No need for us here.
			head->right = NULL;			// Stores the tree on right of head
		};
		
		void insert(int k);
		bool search(int k);
		void Delete(int k);
		void print(const std::string& name);
		~avl_Tree();			
};



void avl_Tree::insert(int k ){
	//A1 - initialize
	//head->right points to root of tree

	Node* t = head->right ;  // t is temp node , will iterate through the tree and bp will point where rebalancing may be needed based on node's BF.
	Node* bp = head->right ; // bp is balance point node whose Bf != 0 , ie it can be unbalanced after inserting a node.
	Node* par_bp = head;     //point to Parent of bp node
	
	Node* curr;                   // will point to new node to insert
	Node* new_node = new Node(k); // A5 - holds the new node to insert ;

	if(t == NULL){               // No node then insert as root of tree;
		head->right = new_node; // head right points to root of tree i.e the new node.
		return;  
	}
	//A2- Compare key and move down tree
	while(t){ // searching for k and inserting if not present.
		
		if(k < t->key){      //A3 - So we move to left subtree
			curr = t->left;  // curr is now left child of t.
			if(!curr){			// So we can insert new node here
				curr = new_node; //curr points to new node.
				t->left = curr;  //left child of t is the new node.
				break;
			}
			else{ 
				if(curr->bf != 0){   // chance of unbalance
					par_bp = t;  	    // so setting up par_bp and bp here.
					bp = curr;		// par_bp is parent of bp and bp points to point of potential unbalance
				}
				t = curr;           // t and curr now points to same node , i,e new inserted node.
			}
		}

		//A4
		else if(k > t->key){ // A4 - We move to right subtree
			curr = t->right ; // set curr as right child of t.
			if(!curr){    	 // same as above for right subtree
				curr = new_node;
				t->right = curr;
				break;
			}
			else{ 
				if(curr->bf !=0){
					par_bp = t;
					bp = curr;
				}
				t = curr;
			}
		}

		else {
			cout<< " KEY " << k<< " ALREADY EXIST!\n";
			return;
		}
	} 
	//end of while loop

	//A5 - Assigning key to new node is done while creating new node.

	//A6 - Adjusting balance factor
	int direction ;   //helps decide the bf of all ancestor nodes and how to link nodes according to new inserted node'bp position.
	//it takes key -1 and +1 to help decide 
	if(k < bp->key){
		direction = 1; //tells key inserted in left subtree of bp. that's it.
	}
	else{
		direction = -1; //tells key inserted in right subtree of bp. that's it.
	}

	Node* ch_bp;  // ch_bp is child node of bp. This extra ptr is used as we have to hold all par_bp , bp and ch_bp while rotating tree.

	if(direction == 1){     // set ch_bp and t left of bp if new node is added in left subtree of bp.
		ch_bp = bp->left;
		t = bp->left;
	}
	else{			        //set ch_bp and t right of bp if new node is added in right subtree of bp.
		ch_bp = bp->right;
		t = bp->right;
	}

	while(t != curr){       // to change bal factor of all nodes till curr added node
		if(k < t->key){
			t->bf = 1;    //node added in left of t so bf will increase by 1 here. bf = ht(left sub) - ht(right sub)
			t = t->left; // move to left node
		}
		else if(k> t->key){
			t->bf = -1;    //node added in right of t so bf will decrease by 1 here.
			t = t->right; // move to right node
		}
		else{           // if k = t->key , we have reached curr node.
			break;
		}
	}

	//A7- Balancing tree if needed.
	if(bp->bf == 0){        // if previously bp was perfect balanced, now its tilted left/right but still balanced with bf = direction.
		bp->bf = direction; // case 1 - tilts perfect balanced tree but still balanced
		return;
	}

	else if(bp->bf == -1*direction){ // if previously bp was tilted in opposite side, now its more balanced with bf = 0
		bp->bf = 0 ;  // case 2 -  makes tree more balanced
		return;
	}
	// bp is parent of ch_bp 

	else {  //This means the node added to already tilted side of tree , thus, tree will go out of balance so we need to balance it.
		//case 3 - unbalanced tree

		if(ch_bp->bf == direction){      //A8 - case of LL or RR - "single rotation" if bp and ch_bp have same sided tilt.
			//single rotation
			t = ch_bp;						                //t again now points to child of bp whichever side node is added
			if(direction == 1){                             // both left tilted - LL
				bp->left = ch_bp->right;					//ch_bp is child of bp, everything right of ch_bp will be now left of bp
				ch_bp->right = bp;	
															//and bp will be right of ch_bp.
				//    O(bp)(2)     O(ch_bp)					
				//   /			  /	      \
				//	O(ch_bp)  => O(curr)   O(bp)				//<- Simple understanding of rotation of subtree in RR
				// /			           /
				//O(curr)			O(ch_bp->right)
			}
			else if(direction == -1){			           // both right tilted - RR
				bp->right = ch_bp->left;					//ch_bp is child of bp, everything left of ch_bp will be now right of bp
				ch_bp->left = bp;							//and bp will be left of ch_bp

				//   O(bp) (-2)        O(ch_bp)				//Simple understanding of rotation of subtree in LL.
				//     \			  /	      \
				//	   O(ch_bp)  =>  O(bp)   O(curr)
				//       \				\
				//       O(curr)		O(ch_bp->left)
			}
			bp->bf = 0;									//after this it can be proven that both bp and ch_bp will be perfect balanced (bf = 0)
			ch_bp->bf = 0;
		}
		//end of single rotation

		else if(ch_bp->bf == -1*direction){ // A9 - case of LR or RL - "double rotation" if bp and ch_bp have different sided tilt.
			//double rotation
			if(direction == 1){				//bp's bf is 1 and its child's bf is -1 -> LR case
				t = ch_bp->right;				// set t as right of ch_bp (temporary)
				ch_bp->right = t->left;			//everytthing left of t is now right of ch_bp
				t->left = ch_bp;				//ch_bp is left of t
				bp->left = t->right;		//everything left of t is now left of bp
				t->right = bp;				// right of t is bp.
				// so , now t i.e (ch_bp->right) is parent of bp and ch_bp
				// ch_bp is left of t and bp is right of t.

				//      O(bp)(-2)    	 O(t)
				//     /			  /       \
				//	  O(ch_bp) =>    O(ch_bp)   O(bp)   			//Rotation in case of LR in subtree
				//     \			  \        /
				// 		O(t)		O(t->left) O(t->right)
			}

			else if(direction == -1){		   //bp's bf is -1 and its child's bf is 1 -> RL case
				t = ch_bp->left;				//hold left of ch_bp in t
				ch_bp->left = t->right;			//right of t is now left or ch_bp
				t->right = ch_bp;				//ch_bp is now right of t
				bp->right = t->left;		//right of bp points to left of t
				t->left = bp;				//bp is now to left of t
			}
			//      O(bp)(2)    	       O(t)
			//     		\			    /       \
			//	  		O(ch_bp)  =>    O(bp)    O(ch_bp)        //Rotation in case of RL in subtree
			//     		/	            \        /
			// 		O(t)		    O(t->left) O(t->right)


			//t is now root of new subtree
			//setting bal factor of nodes accordingly
			if(t->bf == direction){     // if t is right tilted(say) then,
				bp->bf = -1*direction; // t's left subtree is now bp's right subtree or vice versa (see diagram above)
				ch_bp->bf = 0;		  // so , bp will have opposite tilt to t.
				//when ch_bp's bf was -1 ,t's right was +1 and vice versa, so its balanced now.
			}

			else if(t->bf == -1*direction){ // if t is left tilted(say) , then
				bp->bf = 0;					//bp will get more balanced
				ch_bp->bf = direction;		// and ch_bp will tilt opposite to it.
			}

			else if(t->bf == 0){     // if t is perfect balance, bp and ch_bp will be perfect balanced 
				bp->bf = 0;
				ch_bp->bf = 0;
			}
			
			t->bf = 0;        // Finally set t's bf = 0 as it is now perfect balanced (can be proven based on algorithm)
		} 							
		//end of double rotation
	}
	//end of case 3 - rebalancing tree.
	//end of A7- balancing tree

	//A-10 Final touch - t is now root of the new subtree
	if(bp==par_bp->right){  //so, if bp is right child of its parent(par_bp) , then set t as right of par_bp
		par_bp->right = t;
	}
	else{
		par_bp->left = t;  // else , set t as left of par_bp 
	}
}
//End of Insert method !

void avl_Tree::Delete(int k){
   Node *t = head->right; 
   Node *bp, *par_bp, *ch_bp;

	bool present = false;

	stack<Node*> nodes; 
	nodes.push(head); 

	if(t == NULL) cout<< "No element present !!!/n";

	while(t){
		if(k < t->key){
			nodes.push(t);
			t = t->left;
		}
		else if(k > t->key){
			nodes.push(t);
			t = t->right;
		}
		else{
			present = true;
			break;
		}
	}
	if(!present) throw "Element to delet do not exist !!";
	else{
		//t has a single child
		if(!t->left || !t->right){
			Node *curr = t->left ? t->left : t->right;
			Node *t_par = nodes.top();

			if(!curr){
				curr = t;
				if(t == t_par->left)
					t_par->left = nullptr;
				else t_par->right = nullptr;
				t = nullptr;
			}
			else{
				t->key = curr->key;
				t->bf = curr->bf;
				t->left = curr->left;
				t->right = curr->right;
			}
			delete curr;
		}
		// Node has both the children
		else{
			nodes.push(t);
			Node *suc_t = t->right; 
			while(suc_t->left){
				nodes.push(suc_t);
			 	suc_t = suc_t->left;
			}
			t->key = suc_t->key;
			k = suc_t->key; // because the Node that is being deleted is the successor
			Node *curr = suc_t->left ? suc_t->left : suc_t->right;
			Node *t_par = nodes.top();

			if(!curr){
				curr = suc_t;
				if(suc_t == t_par->left)
					t_par->left = nullptr;
				else t_par->right = nullptr;
				suc_t = nullptr;
			}
			else{
				suc_t->key = curr->key;
				suc_t->bf = curr->bf;
				suc_t->left = curr->left;
				suc_t->right = curr->right;
			}
			delete curr;
		}
	}


	auto link = [](int direction, Node *bp){
		return direction == 1 ? bp->left : bp->right; 
	};

	while(nodes.top() != head){
		bp = nodes.top();
		int direction = k < bp->key ? 1 : -1;
		nodes.pop();
		par_bp = nodes.top();

		if(bp->bf == direction){
			bp->bf = 0;
			continue;
		}
		else if(bp->bf == 0){
			bp->bf = -1*direction;
			return;
		}
		else{
			ch_bp = link(bp->bf, bp);
			//single rotation
			if(ch_bp->bf == -1*direction){
				if(direction == -1){
					t = ch_bp;
					bp->left = ch_bp->right;
					ch_bp->right = bp;
					bp->bf = 0;
					ch_bp->bf = 0; 
				}
				else if(direction == 1){
					t = ch_bp;
					bp->right = ch_bp->left;
					ch_bp->left = bp;
					bp->bf = 0;
					ch_bp->bf = 0;
				}
			}
			//single rotation
			else if(ch_bp->bf == 0){
				if(direction == -1){
					t = ch_bp;
					bp->left = ch_bp->right;
					ch_bp->right = bp;
					ch_bp->bf = direction; 
				}
				else if(direction == 1){
					t = ch_bp;
					bp->right = ch_bp->left;
					ch_bp->left = bp;
					ch_bp->bf = direction;
				}
			}
			//double rotation
			else if(ch_bp->bf == direction){
				if(direction == -1){
					t = ch_bp->right;
					ch_bp->right = t->left;
					t->left = ch_bp;
					bp->left = t->right;
					t->right = bp;
					bp->bf = t->bf == 0 ? 0 : t->bf == 1 ? -1 : 0;
					ch_bp->bf = t->bf == 0 ? 0 : t->bf == 1 ? 0 : 1;
					t->bf = 0;
				}
				else if(direction == 1){
					t = ch_bp->left;
					ch_bp->left = t->right;
					t->right = ch_bp;
					bp->right = t->left;
					t->left = bp;
					bp->bf = t->bf == 0 ? 0 : t->bf == 1 ? 0 : 1;
					ch_bp->bf = t->bf == 0 ? 0 : t->bf == 1 ? -1 : 0;
					t->bf = 0;
				}
			}
			if(t->bf == 1 || t->bf == -1){
				if(bp == par_bp->right) par_bp->right = t;
				else par_bp->left = t;
				return;
			}
		}
		if(bp == par_bp->right) par_bp->right = t;
		else par_bp->left = t;		
	}

}
// END OF DELETE METHOD


bool avl_Tree::search(int k){
	Node* root = head->right;
	while(root){
		if(root->key == k){  	//key found
			return true;
		}
		else if(k < root->key){	// if key is less than node , search in left subtree
			root= root->left;
		}
		else{					// else in right subtree
			root = root->right;
		}
	}
	return false;
}

void avl_Tree::print(const std::string& name){
	ofstream myfile;
    myfile.open(name); 

    queue<Node*> nodes;      //Needed for Level order traversal of the avl tree
   
    Node* root = head->right;
    nodes.push(root);

    myfile<<"digraph G{\n" ;                      //DOT code.
    myfile<<"node[shape=record];\n" ;

    while(!nodes.empty()){
        Node* cur =  nodes.front(); 
         nodes.pop(); 
        //Code for node data, fills node in format : left ptr|| key || node's balance factor || right ptr 
        //bf is height of left subtree - height of right subtree

        myfile<< cur->nodeID<<"[label = \"<f1>  "<< "|<f2>  "<< cur->key  << " |<f3>   " << cur->bf << " | <f4>   " << " \"] \n"; 

        //code for edge data. Tells which node is connected to which node
        if(cur->left){
            myfile<< cur->nodeID << ":f1 -> " << cur->left->nodeID << endl; 
            nodes.push(cur->left); // stores next nodes to queue
        }
        if(cur->right){
             myfile<< cur->nodeID << ":f4 -> " <<cur->right->nodeID <<endl;
             nodes.push(cur->right);
         }
    }
    myfile<< "}\n";
    myfile.close();

    //cmd line to convert gv file to png
    string cmd = "dot.exe -Tpng "+ name + " -o " + name + ".png";  //name of png is same as gv file with .png at end.
    system((const char*)cmd.c_str());
    //if this cmd throws error , comment it and run below line in cmd
    // dot.exe -Tpng name.gv -o name.png
    //replace name with name of file.
}


void avl_Tree::deleteTree(Node* root) //deleting all nodes recursively 
{
    if (root == NULL)return;

    // first delete both subtrees 
    deleteTree(root->left);
    deleteTree(root->right); 
    //then delete the node 
    cout<< "deleting node: "<< root->key << endl;
    delete root;
}
avl_Tree::~avl_Tree(){   // calling deleteTree on root of tree 
	Node* root = head->right;
	deleteTree(root);
	delete head;
	cout<< "Finally , head node deleted !\n";
}


int main(){
	avl_Tree* tree = new avl_Tree();  //creates a head node whose right will point to tree root.

	//Showing Insert operations
	cout<<"INSERT opeartion : \n";
	tree->insert(6);  	// The output in command is only for search operation and tree destructor, for insert and delete operations , see the png images generated.
	tree->insert(2);
	//tree->print("i1.gv");   // creating gv file and then getting tree as png , can be seen in same folder this program is run in. name will be i1.gv.png.
	tree->insert(9);
	//tree->print("i2.gv");    // After inserting 7 , tree gets Left unbalanced and (LL) case is called to balance it.
	tree->insert(1);
	tree->insert(4);	
	tree->insert(8);     // showing duplicate entry error
	//tree->print("i3.gv");

	//making direction (RR) imbalance by adding all large elements
	tree->insert(11);
	//tree->print("i4.gv"); // after inserting 15 ,we get RR imbalance , and is rectified.

	tree->insert(3);
	tree->insert(5);
	//tree->print("i5.gv"); // after inserting 20,22 and 25. Tree is right slanted and is balanced accordingly

	//making direction right left unbalance in avl tree.
	tree->insert(7);
	//tree->print("i6.gv"); // after inserting 16 ,tree gets (RL) unbalanced with 10 as bal point and is balanced accordingly.

	tree->insert(10);
	tree->insert(12);
	tree->insert(13);

	tree->print("it.gv"); // This creates 2 imbalance , first at node 16 (RR) which is balanced , then at root node 8 (RL) which is balanced.




	//-----------------------------------------------------------------------------------------------------------------------------------//

	// DELETE OPERATIONS
	cout<< "Delete opeartion : \n";
	tree->Delete(11);      // deleting parent with 2 child , node data is replaced with successor node and that node is deleted. No imbalance.
	tree->print("d1.gv");

	//All cases of delete are covered now. 

	//-----------------------------------------------------------------------------------------------------------------------------------//

	cout<<"\nSearch opeartion !!\n";
	bool s = tree->search(40);
	if(s) cout<< " Element 40 exist \n";
	else cout<< " Element 40 does not exist \n";

	s = tree->search(15);
	if(s) cout<< " Element 15 exist \n";
	else cout<< " Element 15 does not exist \n\n";

	//-----------------------------------------------------------------------------------------------------------------------------------//

	cout<< "\nDELETING THE TREE !!!\n";  
	delete tree;					// calling the destructor which calls deleteTree internally to delete all nodes recursively.

	return 0;
}


