#pragma once

enum EColor {RED, BLACK};


template <typename T>
class RedBlackTree {
public:
	struct TNode {
		TNode(): key(T()), left(0), right(0), parent(0), color(RED){
		}
		explicit TNode(T srcval): key(srcval), left(0), right(0), parent(0), color(RED){
		}
		T key;
		TNode* parent;
		TNode* left;
		TNode* right;
		EColor color;
		TNode& operator= (TNode& equal) {
			key = equal.key;
			parent = equal.parent;
			left = equal.left;
			right = equal.right;
			color = equal.color;
			return *this;
		}
		};

private:
	TNode* Limit;
	TNode* Root;
	void destroy(TNode* curr){
		if (curr->left != Limit) destroy(curr->left);
		if (curr->right != Limit) destroy(curr->right);
		delete curr;
	}

	void LeftRotation (TNode* x) {
		if ( x->right == Limit )
			return;
		TNode* y;
		y = x->right;
		x->right = y->left;
		if (y->left != Limit)
			y->left->parent = x;
		y->parent = x->parent;
		if ( x->parent == Limit)
			Root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	void RightRotation (TNode* x) {
		if ( x->left == Limit )
			return;
		TNode* y;
		y = x->left;
		x->left = y->right;
		if (y->right != Limit)
			y->right->parent = x;
		y->parent = x->parent;
		if ( x->parent == Limit)
			Root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->right = x;
		x->parent = y;
	}

	void InsertFixup (TNode* z) {
		while ( z->parent->color == RED ){
			if (z->parent == z->parent->parent->left) {
				TNode* y =z->parent->parent->right;
				if (y->color == RED){
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else{
					if (z == z->parent->right) {
						z = z->parent;
						LeftRotation (z);
					}
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					RightRotation (z->parent->parent);
				}
			}
			else {
				TNode* y =z->parent->parent->left;
				if (y->color == RED){
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->left) {
						z = z->parent;
						RightRotation (z);
					}
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					LeftRotation (z->parent->parent);
				}
			}
		}
		Root->color = BLACK;
	}

	void Transplant (TNode* u, TNode* v) {
		if ( u->parent == Limit )
			Root = v;
		else if ( u == u->parent->left )
			u->parent->left = v;
		else 
			u->parent->right = v;
		v->parent = u->parent;
	}

	void DeleteFixup ( TNode* x ){
		while (x != Root && x->color == BLACK ) {
			if (x == x->parent->left ){
				TNode* w = x->parent->right;
				if ( w->color == RED ) {
					w->color = BLACK;
					x->parent->color = RED;
					LeftRotation ( x->parent );
					w = x->parent->right;
				}
				if ( w->left->color == BLACK && w->right->color == BLACK ){
					w->color = RED;
					x = x->parent;
				} else {
					if (w->right->color == BLACK ){
						w->left->color = BLACK;
						w->color = RED;
						RightRotation (w);
						w = x->parent->right;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					LeftRotation (x->parent);
					x = Root;
				} 
			} else {
				TNode* w = x->parent->left;
				if ( w->color == RED ) {
					w->color = BLACK;
					x->parent->color = RED;
					RightRotation ( x->parent );
					w = x->parent->left;
				}
				if ( w->right->color == BLACK && w->left->color == BLACK ){
					w->color = RED;
					x = x->parent;
				} else {
					if (w->left->color == BLACK ){
						w->right->color = BLACK;
						w->color = RED;
						LeftRotation (w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					RightRotation (x->parent);
					x = Root;
				}
			}
		}
		x->color = BLACK;
	}

void PrintNode(const TNode& node){
    cout<< "Node: " <<node.key.Value<< ((node.color == RED)? " RED": " BLACK") <<endl;
	if (node.left != Limit) cout << "\tLeft: "<< node.left->key.Value<<endl;
    if (node.right != Limit) cout << "\tRight: "<<node.right->key.Value<<endl;
    if (node.parent != Limit) cout << "\tParent: "<<node.parent->key.Value<<endl;
		else cout << "\tROOT" << endl;
	cout << "_____________________________________________\n";
}

public:
	RedBlackTree<T>(){
		Limit = new TNode;
		Limit->color = BLACK;
		Root = Limit;
	}
	~RedBlackTree() {
		if ( Root != Limit )
			destroy (Root); 
		delete Limit;
	}
	RedBlackTree<T>( const RedBlackTree<T>& );
	RedBlackTree<T> &operator= ( const RedBlackTree<T>& a );

	TNode* GetRoot() const {
		return Root;
	}

	bool IsEmpty() const {
		return ( Root == Limit );
	}

	 TNode* Minimum() const {
		return Minimum (Root);
	}

	TNode* Maximum() const {
		return Maximum (Root);
	}

	 TNode* Minimum (TNode* tp_root ) const{
		if (!tp_root) return 0;
		TNode* curr = tp_root;
		while(curr->left != Limit)
			curr = curr->left;
		return curr;
	}

TNode* Maximum (TNode* tp_root ) const{
		if (!tp_root) return 0;
		const TNode* curr = tp_root;
		while(curr->right != Limit)
			curr = curr->right;
		return curr;
	}

	const TNode* Successor (const TNode* node) const {
		if ( node == 0 )
			return 0;
		if ( node->right != Limit)
			return Minimum (node->right);
		for (const TNode* curr = node; curr->parent != 0; curr = curr->parent){
			if( curr->parent->left == curr)
				return curr->parent;             
		}
		return 0;
	}

	const TNode* Predecessor (const TNode* node) const {
		if ( node == 0 )
			return 0;
		if ( node->left != Limit)
			return Maximum (node->left);
		for (const TNode* curr = node; curr->parent != 0; curr = curr->parent){
			if( curr->parent->right == curr)
				return curr->parent;              
		}
		return 0;
	}

	const TNode* Find (const T& k) const {
		TNode *curr = Root;
		while (curr != Limit) {
			if (curr->key == k) return curr;
			curr = (k < curr->key) ? curr->left : curr->right;
		}
		return 0;
	}

	void Insert (TNode* z) {
		TNode* y = Limit;
		TNode* x = Root;
		while ( x != Limit) {
			y = x;
			if (z->key < x->key) 
				x = x->left;
			else
				x = x->right;
		}
		z->parent = y;
		if (y == Limit)
			Root = z;
		else if ( z->key < y->key )
			y->left = z;
		else 
			y->right = z;
		z->left = z->right = Limit;
		z->color = RED;
		InsertFixup (z);
	}

	void Delete ( TNode* z ){
		TNode* x;
		TNode* y = z;
		int y_original_color = y->color;
		if ( z->left == Limit ) {
			x = z->right;
			Transplant ( z, z->right );
		} 
		else if ( z->right == Limit ){
			x = z->left;
			Transplant ( z, z->left );
		}
		else {
			y = Minimum ( z->right );
			y_original_color = y->color;
			x = y->right;
			if ( y->parent == z )
				x->parent = y;
			else {
				Transplant ( y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			Transplant (z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		if ( y_original_color == BLACK )
			DeleteFixup (x);
		delete z;
	}
	
void PrintTree(const  TNode* node){
	PrintNode(*node);
    if (node->left != Limit) PrintTree(node->left);
    if (node->right != Limit) PrintTree(node->right);
}

};



template<typename T>
class TTree {   
public:
    struct TNode {
        TNode(): key(T()), left(0), right(0), parent(0), color(RED){
        }
        explicit TNode(T srcval): key(srcval), left(0), right(0), parent(0), color(RED){
        }
        T key;
        TNode* parent;
        TNode* left;
        TNode* right;
        EColor color;
    };
private:
    TNode *Root;
    void destroy(TNode* curr){
        if (curr->left) destroy(curr->left);
        if (curr->right) destroy(curr->right);
        delete curr;
    }
private:
    TTree(const TTree&);
    TTree& operator=(const TTree&);

	void Transplant (TNode* u, TNode* v){
		if ( u->parent == 0)
			Root = v;
		else if (u == u->parent->left)
			u->parent->left = v;
		else 
			u->parent->right = v;
		if ( v != 0)
			v->parent = u->parent;
	}
public:
    TTree() : Root(0) {}
    ~TTree(){
        if (Root) destroy(Root);
    }
    const TNode* GetRoot() const {
        return Root;
    }
    bool IsEmpty() const {
        return (Root==0);
    }
    const TNode* Minimum() const {
        return Minimum (Root);
    }
    const TNode* Maximum() const {
       return Maximum (Root);
    }

    const TNode* Minimum (const TNode* tp_root ) const{
        if (!tp_root) return 0;
        const TNode* curr = tp_root;
        while(curr->left!=0)
            curr=curr->left;
        return curr;
    }

    const TNode* Maximum (const TNode* tp_root ) const{
        if (!tp_root) return 0;
        const TNode* curr = tp_root;
        while(curr->right != 0)
            curr = curr->right;
        return curr;
    }
    
    const TNode* Successor (const TNode* node) const {
        if ( node == 0 )
            return 0;
        if ( node->right != 0)
            return Minimum (node->right);
        for (const TNode* curr = node; curr->parent != 0; curr = curr->parent){
            if( curr->parent->left == curr)
                return curr->parent;             
        }
        return 0;
    }

    const TNode* Predecessor (const TNode* node) const {
        if ( node == 0 )
            return 0;
        if ( node->left != 0)
            return Maximum (node->left);
        for (const TNode* curr = node; curr->parent != 0; curr = curr->parent){
            if( curr->parent->right == curr)
                return curr->parent;              
        }
        return 0;
    }
    const TNode* Find (const T& k) const {
        TNode *curr = Root;
        while (curr) {
            if (curr->key == k) return curr;
            curr = (k < curr->key) ? curr->left : curr->right;
        }
        return 0;
    }

    void Insert (TNode* node){
        TNode* curr = Root;
        node->left = 0;
        node->right = 0;
        while (curr) {
            if (node->key <= curr->key){
                if (curr->left == 0) { 
                    node->parent = curr;
                    curr->left = node;
                    return;
                }
                curr = curr->left;
            }
            else {
                if (curr->right == 0) { 
                    node->parent = curr;
                    curr->right = node;
                    return;
                }
                curr = curr->right;
            }
        }
        Root = node;
        node->parent = 0;
    }
	

	void Delete (const TNode* pnode ){
		TNode* z = const_cast <TNode*> (pnode);
		if (z->left == 0)
			Transplant (z, z->right);
		else if (z->right == 0 )
			Transplant (z, z->left);
		else {
			TNode* y = const_cast <TNode*> (Minimum (z->right));
			if ( y->parent != z){
				Transplant (y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			Transplant (z, y);
			y->left = z->left;
			y->left->parent = y;
		}
		delete z;
	}

};