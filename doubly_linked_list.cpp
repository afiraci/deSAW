#pragma once
#include <array>
#include <utility>
#include <memory>
#include <iostream>
#include <assert.h>



class Vertex { 
	public:
		int x;
		int y;

		Vertex(){}
		
		Vertex( int  x_, int y_ ){
			x = x_;
			y = y_;
		}
 
		Vertex operator+( const Vertex& v2){
        		return Vertex( this->x +v2.x, this->y +v2.y );
		}

		Vertex operator-( const Vertex& v2){
			return Vertex( this->x - v2.x, this->y - v2.y );
		}

		friend bool operator==( const Vertex& v1, const Vertex& v2 ){
                        return v1.x==v2.x && v1.y==v2.y;
                }

		friend bool operator!=( const Vertex& v1, const Vertex& v2 ){
			return (not (v1 == v2) );	
		}

		friend std:: ostream& operator<<( std::ostream& os, const  Vertex&   v ){

                        return os<< "(" << v.x << "," << v.y << ")";
                }



};


class Node {
	public:
		Vertex v;
		std:: shared_ptr< Node > next;
		std:: shared_ptr< Node > prev;
	
		Node( Vertex v1 ){
			this->v = v1;	
			this->next = nullptr;
			this->prev = nullptr;
		}

		Node( Vertex v_, std::shared_ptr< Node > next_, std::shared_ptr< Node > prev_ ){
			v = v_;
			next = next_;
			prev = prev_;
		}

		friend std:: ostream& operator<<( std::ostream& os, const  Node&   n ){

			return os<< "(" << n.v.x << "," << n.v.y << ")";
		}
};


class Node_Itr {
	public:
		std:: shared_ptr< Node > current;

		Node_Itr( std:: shared_ptr< Node > current_ ){
			current = current_;
		}

		void advance(){
			current = current->next;
		}

		void go_back(){
			current = current->prev;
		}

		friend bool operator==( const Node_Itr& itr, const std:: shared_ptr< Node >& node_ptr ){
			return itr.current == node_ptr;
		}

		friend bool operator!=( const Node_Itr& itr, const std:: shared_ptr< Node >& node_ptr ){
                        return itr.current != node_ptr;
                }
};


class Doubly_Linked_List {
	public:
		std:: shared_ptr< Node > start = nullptr;
		std:: shared_ptr< Node > end = nullptr;
		
		Doubly_Linked_List(){}
		

		//append a vertex to the end of the list.	
		void append( Vertex v ){
			//We allocate a new node.
			std:: shared_ptr< Node > new_end = std:: shared_ptr< Node >( new Node( v ) );
			//Its appended, thus it becomes the new end of the list, 
			//this is signified by next being the nullptr.
			new_end->next = nullptr;
			//The old end becomes the previous node of the new end.
			new_end->prev = this->end;
			//We alter the member of the list, so as to reflect its new end.
			this->end = new_end;
			//We must also alter the old end of the list, such that it points to the new end with next.
			if( new_end->prev != nullptr ){
				new_end->prev->next = new_end;
			}
			//When new_node is the first node to populate the list it is also its start.
			if( new_end->prev == nullptr ){
				this->start = new_end;
			}
		}


		void insert( const Vertex& v, std:: shared_ptr< Node> next_, std:: shared_ptr< Node > prev_ ){
			
			//We allocate a new node.
                        std:: shared_ptr< Node > new_node = std:: shared_ptr< Node >( new Node( v ) );
                        //We link it to next_ and prev_.
                        new_node->next = next_;
                        new_node->prev = prev_;
                        //We link next_ and prev_ to it.
			//If next_ and/or prev_ are nullptr, then we are appending/ prepending
			//and we must alter the start and end members of the list.
                        if( next_ == nullptr){
				this->end = new_node;
			}else{
				next_->prev = new_node;
			}
			if( prev_ == nullptr ){
				this->start = new_node;
			}else{
				prev_->next = new_node;
			}
                }

		void prepend( Vertex v ){
			this->insert( v, this->start, nullptr );
		}


		void remove( std:: shared_ptr<Node> n ){
			assert( n != nullptr );		
			//The Node is dallocated when the refference count of shared_ptr hits zero.
			//Thus it suffices to break the link with the previous and next node.
			
			if( n->next != nullptr ){
				n->next->prev = n->prev;
			}
			if( n->prev != nullptr ){
				n->prev->next = n->next;
			}

			//When our node is the last and/or first in the list we must 
			//update the start and/or end members of the list.
			
			if( n->next == nullptr ){
				this->end = n->prev;
			}
			if( n->prev == nullptr ){
				this->start = n->next;
			}
		}

		friend std:: ostream& operator<<( std::ostream& os, const  Doubly_Linked_List & dll ){
                	Node_Itr itr( dll.start );
			while( itr.current != nullptr ){
				os << itr.current->v << std::endl;
				itr.advance();
			}
			return os;

                }

		bool contains( const Vertex& v ){

			Node_Itr itr( this->start );
			while( itr.current != nullptr ){
				if( itr.current->v == v ){
				       return true;
				}
				else
					itr.advance();
			}
			
			return false;
		}



		

};



