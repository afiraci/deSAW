#pragma oncejeff rubidge
#include <cassert>
#include <iostream>
#include <memory>
#include "doubly_linked_list.cpp"
#include <unordered_map>
#include <inttypes.h>
#include <math.h>
#include <random>
#include <iostream>
#include "bytell_hash_map.hpp"




Vertex normal[4]= { Vertex(1,0),Vertex(0,1),Vertex(-1,0),Vertex(0,-1)};

Vertex diagonal[4]={Vertex(1,1),Vertex(1,-1),Vertex(-1,1),Vertex(-1,-1)};

// Returns direct neighbors in the square grid.
std::array<Vertex,4>  normal_neighbors( Vertex v ){
        std:: array<Vertex,4> neighbors;
        
        for( int i = 0; i < 4; i++ ){
                neighbors[i] = v + normal[i];
        }
        return neighbors;
}


std::array<Vertex,8> all_neighbors( Vertex v ){
	std::array<Vertex,8> neighbors;
	// Normal neighbors are asigned to the first four enteries.
	for( int i = 0; i<4; i++)
		neighbors[i] = v + normal[i];
	// Diagonal neighbors are asigned to the last four enteries.
	for(int i = 0; i<4; i++)
		neighbors[i+4] = v + diagonal[i];

	return neighbors;
}


std::array<Vertex, 4> random_neighbors( const Vertex& v, std::mt19937& g ){
        std::array<Vertex, 4> neighbors = normal_neighbors( v );
        std:: shuffle( neighbors.begin(), neighbors.end(), g );
        return neighbors;
}

std:: array<Vertex,4> shuffled_array( Vertex v,  std:: mt19937& g ){
        std:: array<Vertex, 4> arr;

        for( int i=0; i<4; i++){
                arr[i] = v +  normal[i];
        }

        std::shuffle( arr.begin(), arr.end(), g );

        return arr;

}



// In order for to use an unordered_map for a custom class one must supply a 
// hash-function and an equality operator for said class. We supply define the equality operator 
// for Node in node.h. Here we add the hash-function. 

template <>
struct std::hash<Vertex>
{
  std::size_t operator()(const Vertex& v ) const 
  {
    using std::size_t;
    // Compute individual hash values for both integers. 
    // Combines them using XOR
    // and bit shifting
    // We multipl
    return ((size_t) v.y)| (((size_t)  v.x) <<32);
  }
};



class Doubly_Linked_Dictionary {
	public:
		std:: unique_ptr<Doubly_Linked_List> list;
		ska:: bytell_hash_map< Vertex, std:: shared_ptr<Node> >  dict;
		
		void append(const Vertex& v ){
			this->list->append(v);
                        this->dict[ v ] = this->list->end;
		}

		void prepend( const Vertex& v ){
			this->list->prepend( v );
			this->dict[ v ] = this->list->start;
		}
	
		Doubly_Linked_Dictionary(){
			list = std::unique_ptr<Doubly_Linked_List>( new Doubly_Linked_List() );
		}

		std:: shared_ptr<Node>  &operator[]( Vertex v ){
			return this->dict[ v ];
		}

		// First node 
                std:: shared_ptr< Node > start(){
                        return this->list->start;
                }
                // Last node
                std:: shared_ptr< Node >  end(){
                        return this->list->end;
		}

		friend void operator+( Doubly_Linked_Dictionary & dld , const Vertex& v ){
			dld.append(v);
		//	dld.dict[ v ] = dld.list->end;
		}
		
		bool contains( const Vertex& v ){
			return not( this->dict.find( v ) == this->dict.end() );
		}

		void remove( const Vertex& v ){
			this->list->remove( (this->dict[v]) );
			this->dict.erase(v);
		}

		void insert(const Vertex& v, const Vertex& next_, const Vertex& prev_ ){
			this->list->insert( v, this->dict[ next_ ], this->dict[ prev_ ]);
			this->dict[ v ] = this->dict[ next_ ]->prev;
		}

		// Concatinate dll to the beginning of the dld, while preserving direction and order of
		// the dll.  
		friend void operator+( const Doubly_Linked_List& dll, Doubly_Linked_Dictionary& dld ){
			// While loop runs backwards over list.
	
			Node_Itr itr( dll.end );
                        while( itr.current != nullptr ){
				// Append the vertex.
                                dld.prepend( itr.current->v );
				// Go backwards to the previous node in the list.
                                itr.go_back();
                        }// We break from the while loop when the start of the list is exceed, 
			 // which is when the iterator takes the value nullptr.

		}

		friend std:: ostream& operator<<( std::ostream& os, const Doubly_Linked_Dictionary& dld ){
                        Node_Itr itr( dld.list->start );
                        while( itr.current != nullptr ){
                                os << itr.current->v << std::endl;
                                itr.advance();
                        }
                        return os;
                }

		Vertex min( const Vertex& v1, const Vertex& v2 ){
                        Node_Itr itr_1( this->dict[ v1 ] );
                        Node_Itr itr_2( this->dict[ v2 ] );

                        for(;;  itr_1.advance(), itr_2.advance() ){
                                //When reaching v2 from v1, v1 must occure first,
                                //so we return v1.
                                //When the iterator starting from v2 does not visit v1 before reaching the end,
                                //v1 must occure before v2, so we return v1.
                                if( ( itr_1.current and itr_1.current->v == v2)  or itr_2.current == nullptr )
                                        return v1;
                                if( (itr_2.current and itr_2.current->v == v1 ) or itr_1.current == nullptr)
                                        return v2;
                        }


                }

                // Finding the vertex in the list which occures first in the 
                Vertex first( std:: shared_ptr< Doubly_Linked_List >  dll ){
			Vertex min = dll->start->v;

			for(Node_Itr itr( dll->start); itr.current != nullptr; itr.advance() ){
				if( this->contains( itr.current->v ) ){
				min = this->min( min, itr.current->v );
				//std::cout<< min <<std::endl;
				}
			}

			return min;

                }

		Vertex max( const Vertex& v1, const Vertex& v2){
			

			if( v1 == this->min( v1,v2) )
				return v2;
			else 
				return v1;
		}

		Vertex last( std::shared_ptr< Doubly_Linked_List > dll ){
			Vertex max = dll->end->v;

			for( Node_Itr itr( dll->start ); itr.current != nullptr; itr.advance() ){
				if( this->contains( itr.current->v ) ){
                                max = this->max( max, itr.current->v );
                                //std::cout<< max <<std::endl;
                                }
			}
			return max;
		}


		/*

		void loop_erase(  Doubly_Linked_List &dll ){


			Node_Itr dll_itr( dll.end );
			while( dll_itr != nullptr ){
				if( this->contains( dll_itr.current->v ) ){
					// The iterator points to the first self-intersection of the dll with the dld.
					// We now consider a iterator starting at the intersection in the dld.

					Node_Itr dld_itr( this->dict[ dll_itr.current->v ] );
					
					// All the nodes before and including the intersection are erased from the dld.
					while( dld_itr != nullptr ){
						Vertex to_be_removed = dld_itr.current->v;
						dld_itr.go_back();
						this->remove( to_be_removed );
					}
					
					// We advance the dll iterator, as we do note delete the intersection 
					// in the dll. We then remove the nodes afer the intersection in the dll.
					dll_itr.advance();

					while( dll_itr != nullptr ){
						auto to_be_removed = dll_itr.current;
						dld_itr.advance();
						dll.remove( to_be_removed );
					}

					// the beginning of the dld and the end of the dll have been shortened
					// thereby ensuring that no loop can occure when concatinating dll + dld.
					break;
				}
						
				dll_itr.advance();	
			}


	
		}

		

		//The following function recursivly appends a neighbor of the end of walk and 
		//returns whether it did so succesfully. Backtracking is implemented, that is, 
		//when the walk runs out of vertices it pops its end untill it can choose a hitherto
		//unexplored vertex.
		
		bool append_random_neighbors( unsigned int number_of_vertices_to_append , std::mt19937 g ){
			
			if( number_of_vertices_to_append == 0 ) return true;	
			
			//std::array<Vertex, 4> neighbors = normal_neighbors( this->end()->v );
		//	std::shuffle( neighbors.begin(), neighbors.end(), g );
			std:: array<Vertex, 4> neighbors = shuffled_array( this->end()->v, g );
			// We note the current last vertex, to later check whether it has changed.
			Vertex old_end = this->end()->v;
			// We attempt to append each of the four neighbors	
			std:: cout<< "shuffled neighbors:" << std::endl;
			for( int i=0; i<4; i++){
				std::cout<< neighbors[i].x<< " " << neighbors[i].y << std::endl;
			}

			for( auto & v : neighbors ){
					std::cout<< number_of_vertices_to_append <<"\n"<< std::endl;
				// We check whether the neighbor is already occupied.
				if( this->contains(v) ){
					continue;
				// If it is not we try to append it.
				}else{ 

					//std::cout<< "appending "<< "("<<v.x<<","<<v.y<<")"<<std::endl;
					// We append the neighbor.
					this->append(v);
					// We append the nexte neighbor 
					if( append_random_neighbors( number_of_vertices_to_append -1 , g) == true ){
						return true;
					// If the appension of further vertecies in unsuccesful, the appended vertex is removed.
					}else{
						//std::cout<< "removing "<< this->end()<<std::endl; 	
						this->remove( this->end()->v );
					}
				
				
				}
				
		 	 	
			}
			// If no vertex can be appended successfully we return false to signify the failure of extending the current 
			// walk by the given number of vertecies. 
			if( this->end()->v == old_end){
				return false;
			}
				
		}

		*/
		
};



