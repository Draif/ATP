#pragma once

enum EColor {RED, BLACK};

template <typename T>
class TRBTree {
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
		bool operator== ( TNode& a ) {
			return ( parent == a.parent && left == a.left && right == a.right && color == a.color && key == a.key );
		}

		bool operator!= (TNode& a){
			return !(*this == a);
		}

		TNode& operator= (TNode& a){
			parent = a.parent;
			left = a.left;
			right = a.right;
			color = a.color;
			key = a.key;
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

	TNode* Minimum (TNode* tp_root ) {
		if (!tp_root) return 0;
		TNode* curr = tp_root;
		while(curr->left != Limit)
			curr = curr->left;
		return curr;
	}
	TNode* Minimum (TNode* tp_root ) const {
		if (!tp_root) return 0;
		TNode* curr = tp_root;
		while(curr->left != Limit)
			curr = curr->left;
		return curr;
	}
	TNode* Maximum (TNode* tp_root ) {
		if (!tp_root) return 0;
		TNode* curr = tp_root;
		while(curr->right != Limit)
			curr = curr->right;
		return curr;
	}

	TNode* Maximum (TNode* tp_root ) const {
		if (!tp_root) return 0;
		TNode* curr = tp_root;
		while(curr->right != Limit)
			curr = curr->right;
		return curr;
	}
	TNode* Successor (TNode* node)  {
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

	TNode* Predecessor (TNode* node)  {
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

public:
	class iterator{
		friend class TRBTree;

	public:
		iterator():el(0), Limit(0){
		}
		iterator (TNode* a, TNode* l): el(a), Limit (l){}
				
		iterator (const iterator& another){
			el = another.el;
			Limit = another.Limit;
		}

		iterator& operator++(){
			if ( el->right != Limit){
				el = el->right;
				while(el->left != Limit)
					el = el->left;
				return *this;;
			}
			for (; el->parent != Limit; el = el->parent){
				if( el->parent->left == el){
					el = el->parent;
					return *this;       
					}
			}
			el = Limit;
			return *this;
		}


		iterator& operator--(){
			if ( el->left != Limit){
				while(el->right != Limit)
					el = el->right;
				return  *this;
			}
			for (;el->parent != Limit; el = el->parent){
				if( el->parent->right == el){
					el = el->parent;
					return *this;
				}             
			}
			el = Limit;
			return *this;
		}

		T& operator*() {
			return el->key;
		}

		bool operator== (const iterator& i){
			return (el == i.el && Limit == i.Limit);
		}

		bool operator!= (const iterator& i){
			return (el != i.el || Limit != i.Limit);
		}
		iterator& operator= (const iterator& a){
			el = a.el;
			Limit = a.Limit;
			return *this;
		}

	private:
		TNode* el;
		TNode* Limit;
	};

	class const_iterator{
		friend class TRBTree;

	public:
		const_iterator():el(0), Limit(0){
		}
		const_iterator (TNode* a, TNode* l): el(a), Limit (l){}
				
		const_iterator (const const_iterator& another){
			el = another.el;
			Limit = another.Limit;
		}

		const_iterator& operator++(){
			if ( el->right != Limit){
				el = el->right;
				while(el->left != Limit)
					el = el->left;
				return *this;;
			}
			for (; el->parent != Limit; el = el->parent){
				if( el->parent->left == el){
					el = el->parent;
					return *this;       
					}
			}
			el = Limit;
			return *this;
		}


		const_iterator& operator--(){
			if ( el->left != Limit){
				while(el->right != Limit)
					el = el->right;
				return  *this;
			}
			for (;el->parent != Limit; el = el->parent){
				if( el->parent->right == el){
					el = el->parent;
					return *this;
				}             
			}
			el = Limit;
			return *this;
		}

		const T& operator*() {
			return el->key;
		}

		bool operator== (const const_iterator& i){
			return (el == i.el && Limit == i.Limit);
		}

		bool operator!= (const const_iterator& i){
			return (el != i.el || Limit != i.Limit);
		}
		const_iterator& operator= (const const_iterator& a){
			el = a.el;
			Limit = a.Limit;
			return *this;
		}

	private:
		TNode* el;
		TNode* Limit;
	};



public:
	TRBTree<T>(){
		Limit = new TNode;
		Limit->color = BLACK;
		Root = Limit;
	}

	TRBTree<T>( const TRBTree<T>& a ){
		Limit = new TNode;
		Limit->color = BLACK;
		Root = Limit;
		*this = a;
	}
	~TRBTree () {
		if ( Root != Limit )
			destroy (Root); 
		delete Limit;
	}

	TRBTree<T>& operator=(const TRBTree<T>& a ){
		if ( !empty() )
			destroy(Root);
		Root = Limit;
		for (const_iterator pos = a.begin(); pos != a.end(); ++pos)
			insert (*(pos));
		return *this;
	}

	void swap( TRBTree<T>& a){
		std::swap (Root, a.Root);
		std::swap (Limit, a.Limit);
	}

	iterator begin(){
		return iterator ( Minimum (Root), Limit );
	}
	const_iterator begin() const {
		return const_iterator ( Minimum (Root), Limit );
	}
	iterator end(){
		return iterator ( Limit, Limit );
	}
	const_iterator end() const {
		return const_iterator ( Limit, Limit );
	}
	bool empty(){
		return (Root == Limit);
	}
	void clear(){
		if ( Root != Limit )
			destroy (Root); 
		Root = Limit;
	}

	iterator find(T k){
		TNode *curr = Root;
		while (curr != Limit) {
			if (curr->key == k) return iterator (curr, Limit);
			curr = (k < curr->key) ? curr->left : curr->right;
		}
		return iterator(Limit, Limit);
	}

	iterator insert (T val){
		TNode* z = new TNode;
		z->key = val;
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
		return iterator(z, Limit);
	}

	iterator erase ( iterator a) {
		TNode* z = a.el;
		++a;
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
		return a;
	}

};