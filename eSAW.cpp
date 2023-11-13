#pragma once
#include <memory>
#include <iostream>
#include "doubly_linked_dictionary.cpp"
#include <vector>
#include <algorithm>
#include <optional>
#include <vector>


///////////////////////// Helper functions start /////////////////////////////


class Rectangle {
        private: // The boundaries of the rectangle in each cardinal direction.
                int max_x = 0;
                int max_y = 0;
                int min_x = 0;
                int min_y = 0;
        public:
                Rectangle(){}

                bool exceeds( const Vertex& v ) const {
                        if( max_x < v.x ||
                            max_y < v.y ||
                            min_x > v.x ||
                            min_y > v.y ){

                                return true;
                        }else{
                                return false;
                        }
                }
                // The size of the rectangle is adjusted so that
                // it contains a vertex v.
                void adjust( const Vertex& v ){
                        if( max_x< v.x ) max_x = v.x ;
                        if( max_y< v.y ) max_y = v.y ;
                        if( min_x> v.x ) min_x = v.x ;
                        if( min_y> v.y ) min_y = v.y ;
                }
};



class Path : public Doubly_Linked_List {

        public:
                Path(){}

	bool exceeds( Rectangle rectangle ){
        	// We check for each vertex whether it exceeds the rectangle,
        	// If one does, we return true, if none does we return false.
        	Node_Itr itr( this->start);
        	while( itr!=nullptr ){
                	if( rectangle.exceeds( itr.current->v ))
                        	return true;
                	else
                        	itr.advance();
        	}

        	return false;
	}

};



std:: shared_ptr< Path > find_path( Vertex start, Vertex end, std:: vector< Vertex >  allowed_verticies ){

        // If start is is already equal to end we  return an empty list.
        if( start == end  ){
                std::shared_ptr< Path > path = std:: shared_ptr< Path >( new Path );
                path->append( end );
                return path;
        }

        for( auto& n : normal_neighbors( start ) ){
                // We check that n is an allowed vertex.
                if( std::find( allowed_verticies.begin(), allowed_verticies.end(), n ) != allowed_verticies.end() ){
                        // We attempt to find a path to the end vertex from n.

                        std:: vector< Vertex > aw = allowed_verticies;
                        aw.erase(std::remove(aw.begin(), aw.end(), start ), aw.end());
                        std:: shared_ptr< Path > path = find_path( n, end, aw );
                        // If we find the end, we return the path starting from n.
                        if( path != nullptr ){
                                path->prepend( start );
                                return path;
                        }
                }
        }
        // Return empty path, denoted as a nullptr, when no valid neighbor is available.
        // When this happens the end was not not found.
        return nullptr;

}


/////////////////////////////////// Helper functions end /////////////////////////////////////////////


class Extension : public Doubly_Linked_Dictionary {
	
	public:
		Extension(){}

		std::shared_ptr< Node > first_to_exceed( Rectangle rect, std::shared_ptr< Path> modification ){
			
			// We try to find the first vertex in the modification to exceed
			// the rectangle.
			Node_Itr itr( modification->start);
			while( itr != nullptr ){
				if( rect.exceeds( itr.current->v ) )
					return itr.current;
				else 
					itr.advance();
			}
			//If the modifcation does not exceed the rectangle, we
			//return the current end of the extension.
			
			return this->end();
		}	

		bool not_straight( Rectangle rectangle ){
			if(  (not rectangle.exceeds( this->end()->v ) ) or ( not rectangle.exceeds( this->end()->prev->v ) ) or this->end()->prev->prev == nullptr or (not rectangle.exceeds( this->end()->prev->prev->v ) ) )
				return true;
			else
				return false;
		}

		void straighten(){
	
			Vertex last = this->end()->v;
			Vertex second_last = this->list->end->prev->v;
			this->append(  (last  - second_last) + last );

		}

		void loop_erase_for_front_concat(  Path& dll ){

			// We iterate over the forward modification until we find an intersection 
			// with the extension, to perfrom appropriat deletions.
                        Node_Itr dll_itr( dll.start );
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
                                                dll_itr.advance();
                                                dll.remove( to_be_removed );
                                        }

                                        // The beginning of the dld and the end of the dll have been shortened
                                        // thereby ensuring that no loop can occure when concatinating dll + dld.
                                        
                                        break;
                                }
	
                                dll_itr.advance();
                        }

                }

		std:: shared_ptr< Path > intersection( const Path& path ){
                	// We iterate over the path, checking
                	// whether vertecies are in the extension at every step.
                	Node_Itr itr( path.start );
               		while( itr.current != nullptr ){
				// If the intersection is non-empty, we return a pointer
				// to to a path containing the intersection.
				//std::cout<< itr.current->v << std::endl;
                        	if( this->contains( itr.current->v ) ){
                                	std:: shared_ptr< Path > intersec = std::shared_ptr< Path >( new Path() );
        	                	while( itr.current != nullptr ){
                	                	if( this->contains( itr.current->v ) ){
                        		               	intersec->append( itr.current->v );
                                	        	itr.advance();
						}
						else
							itr.advance();
					}
					return intersec;
				}
                	        else
                        		itr.advance();
                	}
			// We return a nullptr to signify that there is no intersection.
			return nullptr;
        	}

		Vertex prev_vertex( const Vertex& v ){
			if( this->start()->v == v )
				std::cout<< "ERROR: start of list does not have previous vertex" << std::endl;
                	return (this->dict[ v ])->prev->v;
        	}
	
	        Vertex next_vertex( const Vertex& v ){
			if( this->end()->v == v )
                                std::cout<< "ERROR: end of list does not have next vertex" << std::endl;
        	        return (this->dict[ v ])->next->v;
        	}

};



class Extendable_SAW {
	public:
		std:: unique_ptr< Doubly_Linked_Dictionary >  walk;
		std:: unique_ptr< Extension > forward_extension;
		Rectangle rectangle;

		void adjust( const Vertex& v ){

			if(  rectangle.exceeds( v ) ){
				rectangle.adjust( v );
				
			}

			if( forward_extension->not_straight( this->rectangle ) )
				forward_extension->straighten();

		
			
		}
		

		// Constructing a new extension from a forward_modification.
		void construct_new_extension( std:: shared_ptr< Path > forward_modification ){

			// The current extension is emptied.
			Node_Itr itr( forward_extension->start() );
			while( itr!= nullptr ){
				 auto to_be_removed = itr.current;
				 itr.advance();
				 forward_extension->remove( to_be_removed->v );
			}
                        // We find the fist vertex of the forward extension to exceed the rectangle.
			auto vertex_outside_rect = this->forward_extension->first_to_exceed( this->rectangle , forward_modification );
			
			// We remove the vertecies after the first vertex to exceed.
			Node_Itr itr_1( vertex_outside_rect );
			// We advance the iterator once, so as not to remove the vertex of interest. 
			itr_1.advance();
			while( itr_1 != nullptr ){

				auto to_be_removed = itr_1.current;
                                itr_1.advance();
                                forward_modification->remove( to_be_removed );
			}

			// The remaining verticies in the forward modification are concatinated with the now empty extension.
			*forward_modification +  *(this->forward_extension);

			// We extend the extension so it radiats outwards from the rectangle by three verticies. 
			
			if( vertex_outside_rect->prev !=nullptr ){
				auto radiate_direction = (vertex_outside_rect->v - vertex_outside_rect->prev->v); 

				for( int i = 0; i<2; i++)
					*(this->forward_extension) + ( this->forward_extension->end()->v + radiate_direction );
			} 	
		}


	
		Extendable_SAW(){
			walk = std:: unique_ptr< Doubly_Linked_Dictionary > ( new Doubly_Linked_Dictionary() );
			forward_extension = std:: unique_ptr < Extension > ( new Extension() );

			*walk + Vertex(0,0);
			*walk + Vertex(0,1);
			
			*forward_extension + Vertex(0,2);
			*forward_extension + Vertex(0,3);
			*forward_extension + Vertex(0,4);

		}

		std:: shared_ptr< Node > start(){
			return walk->start(); 
		}

		void extend( std:: mt19937& g ){
						
			//std::random_device rd;
	        	//std::mt19937 g(rd());
			// We examin the neighbors of the the current end of the walk and evaluate whether 
			// they are e-valid.
			auto neighbors = random_neighbors( walk->end()->v , g );
			for( auto n : neighbors ){
				//std:: cout<< "candidate: "<< n << std:: endl;
				// If a neighbor is occupied by the walk we reject it.
				if( walk->contains( n ) )
					continue;

				std:: vector< Vertex > disjoint_neighborhood;
				for( auto a : all_neighbors( walk->end()->v ) ){
					if( not walk->contains( a ) )
						disjoint_neighborhood.push_back( a );
				}
				
				// We attempt to find a path between the candiate n and the first vertex of the extension.
				std:: shared_ptr< Path > forward_modification = find_path( n , this->forward_extension->start()->v, disjoint_neighborhood );

				// If the path could be found the neighbor n is accepted and the extension
				// modified so as to be an extension of this new end vertex.				
				if( forward_modification ){
					//std::cout<< " forward modification: "<< *forward_modification ;
					rectangle.adjust( n );	
					*walk + n;

					// When the forward modifcation exceeds the rectangle, we construct a
					// completely new extension from the forward modification, to replace the old one.
					if(  forward_modification->exceeds( rectangle) ){
						this->construct_new_extension( forward_modification );
						// We remove the first vertex of the extension as it coincides 
						// with the new end of the walk.
						this->forward_extension->remove( this->forward_extension->start()->v );
					}
					// When the forward modifcation does not exceed the the rectangle,
					// we attach the forward modification the existing extension.
					else{   
						// We ensure no loops are created when attaching the forward modification to
						// the beginning of the extension, by appropriate deletions on both objects.
						this->forward_extension->loop_erase_for_front_concat( *forward_modification );
						forward_modification->remove( forward_modification->start );
						// Attaching forward modification to the beginning of the extension.
						*forward_modification + *forward_extension;
					}
					
					// We adjust the rectangle, and straighten extension as needed.
					/*
					std::cout<< "hi" << std:: endl;
					std:: cout<< *walk<< std::endl <<*extension;
					*/
					this->adjust( n );
					break;
							
				}	
			}			
		}		
};
