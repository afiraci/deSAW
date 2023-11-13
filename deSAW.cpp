#include "eSAW.cpp"


class Backward_Extension : public Extension {
	
	public:

		Backward_Extension(){}

		void straighten(){
			 Vertex last = this->end()->v;
			 Vertex second_last;
			 if( this->start()->v == last )
                         	second_last = Vertex(0,0 );
			 else
				 second_last = this->end()->prev->v;
			 this->append( ( last - second_last ) + last ) ;

		}
		
		void internal_reroute( std:: shared_ptr< Path > backward_modification, const Rectangle& rectangle){
			

			// If the modification traverses the first veretx ( the origin )
			// we remove verticies occuring before the origin, including the origin.
			// from the backward modification. 
			if( backward_modification->contains( Vertex(0,0) ) ){

				Node_Itr itr( backward_modification->start );
				while( itr.current != nullptr ){
					if( itr.current->v != Vertex(0,0) ){
						auto to_be_removed = itr.current;
						itr.advance();
						backward_modification->remove( to_be_removed );
					}
					else{
						// removing the origin.
						backward_modification->remove( itr.current );
						break;
					}
				}


				// We remove verticies in the extension before and including the
				// last vertex of backward modification. 
				Node_Itr itr_1( this->start() );
				while( itr_1.current->v != backward_modification->end->v ){
					Vertex to_be_removed = itr_1.current->v;
					itr_1.advance();
					this->remove( to_be_removed );

				}
				// removing the last vertex of the backward modification in the 
				// backward extension.
				this->remove( itr_1.current->v );
				
				// concatinating the backward modification to the front of the backward extension.  
				*backward_modification + *this;

					
			}
			else{

				// We delete the section in the backward extension which lies between the first
				// veretx of the extension which intersects the modification, and the last vertex of the extension to intersect
				// the modification, and replace with the corresponding section from the modification.  
				// the backward_modification, excluding said first and last vertex.
				Vertex first = this->first( backward_modification);
				Vertex last = this->last( backward_modification);

				Node_Itr itr( this->dict[ first ]->next );
				while( itr.current->v != last ){
					Vertex to_be_removed = itr.current->v;
					itr.advance();
					this->remove( to_be_removed );
					//std::cout<< "removing:"<< to_be_removed<<std::endl;
				}
			
				// We insert the backward modification into the backward extension. 

				//We move the iterator to the first vertex
				Node_Itr itr_1( backward_modification->start );
				while( itr_1.current->v != first  ){
					//std::cout<< "skipping:"<<itr_1.current->v<<std::endl;
					itr_1.advance();
				}
				itr_1.advance();


	

				// We insert the remainig verticies from the modification into the extension.
				while( itr_1.current->v != last){
					//std::cout<< "inserting:"<< itr_1.current->v<<std::endl;
					this->insert( itr_1.current->v, last , this->prev_vertex( last ) ); 
					itr_1.advance();
				}
			}
                        
			
			//This needs to be changed: we should only check whether vertecies which have actually been inserted 
			//exceed the rectangle.
			//We check whether the backward_extension exceeds the rectangle, and straighten it apropriatly.
                        Node_Itr itr_1( backward_modification->start );
                        while( itr_1.current != nullptr ){
	                        if( rectangle.exceeds( itr_1.current->v ) ){
        	                        itr_1.advance();
                	                Node_Itr itr_2( this->dict[ itr_1.current->v ] );
                        			while( itr_2.current != nullptr ){
							Vertex to_be_removed = itr_2.current->v;
                                         		itr_2.advance();
                                                        this->remove( to_be_removed );
                                                }
							//std::cout<< *this;
                                                        this->straighten();
                                                        this->straighten();
							break;
                                }
                                else
                                	itr_1.advance();
			}
			
		}
};


class Doubly_Extendable_SAW : public Extendable_SAW {
	public:

		std:: unique_ptr< Backward_Extension > backward_extension; 
		
	
		
		Doubly_Extendable_SAW(){
			backward_extension = std:: unique_ptr< Backward_Extension > ( new Backward_Extension() );
			*backward_extension + Vertex( 0, -1);
			*backward_extension + Vertex( 0, -2);
			*backward_extension + Vertex( 0, -3);


		} 

		void adjust( const Vertex& v ){

                        if(  rectangle.exceeds( v ) ){
                                rectangle.adjust( v );

                        }

                        if( forward_extension->not_straight( this->rectangle ) )
                                forward_extension->straighten();
			if( backward_extension->not_straight( this->rectangle ) )
				backward_extension->straighten();


                }


		std:: shared_ptr< Path >  construct_sensitive_region( std::shared_ptr< Path > forward_modification , std:: shared_ptr< Path > intersec ){

			std:: shared_ptr< Path > sensitive_region = std:: shared_ptr< Path > ( new Path() );
			
			// We go to the start of the intersecion.
			Node_Itr itr( forward_modification->start );
			while( itr.current->v != intersec->start->v ){
				itr.advance();
			}

			// We append the verticies from the start to the end of the intersection 
			// in the forward_modification, and append to the sensitive region at every step. 
			while( itr.current->v != intersec->end->v ){
				sensitive_region->append( itr.current->v );	
				itr.advance();
			}
			sensitive_region->append( itr.current->v );

			return sensitive_region;
		}
	
		void extend( std:: mt19937& g ){


                        // We examin the neighbors of the the current end of the walk and evaluate whether
                        // they are e-valid.
                        auto neighbors = random_neighbors( walk->end()->v , g );
                        for( auto n : neighbors ){
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
				// If no path is found the neighbor n is not e-valid and we continue to the next neighbor of the endpoint.
				//

				if( not forward_modification)
					continue;
				// We now need to check whether the forward modification will interfere with the current
				// backward extension.
				//std::cout<< "forward modification: "<<*forward_modification<< std::endl;
				auto intersec =  backward_extension->intersection( *forward_modification );
				
				if( intersec ){
					//std::cout<< "hi" <<std::endl;
					auto sensitive_region = this->construct_sensitive_region( forward_modification, intersec );
					//std::cout<< "hi"<< std::endl;
				//	std::cout<<"backwards ext. :"<< *backward_extension;
					//std::cout<<"intersction :"<< *intersec;
					//std::cout<<"sensitiv_region :"<< *sensitive_region;
					// We construct the disjoint neighborhood of the sensitive region in which we attempt to find
					// a backward modification, by which to reroute the backward extension so as to resolve the intersection.
					std:: vector< Vertex >  disjoint_neighborhood_of_sensitive_region;
					Node_Itr itr( sensitive_region->start );

				        while( itr.current != nullptr ){
						for( auto v : all_neighbors( itr.current->v ) ){
							if( (not sensitive_region->contains( v )) and ( (not walk->contains( v )) or walk->start()->v == v) )
								disjoint_neighborhood_of_sensitive_region.push_back( v );		
						}
						itr.advance();

					}	
					Vertex first_vertex_of_backward_modification;
                                        Vertex last_vertex_of_backward_modification;
				      
					// We find the last vertex of the backward extension just before entering the sensitive region for the first time.
					//
					if( sensitive_region->start->v != backward_extension->start()->v )
						first_vertex_of_backward_modification = this->backward_extension->prev_vertex( sensitive_region->start->v );
					else 
						first_vertex_of_backward_modification = this->walk->start()->v;
					
					last_vertex_of_backward_modification = this->backward_extension->next_vertex( sensitive_region->end->v ); 

					


					auto backward_modification = find_path( first_vertex_of_backward_modification,
						       				last_vertex_of_backward_modification, 
										disjoint_neighborhood_of_sensitive_region );
				        //std::cout<< *backward_modification;
					// If no backward modifcation path could be found then the neighbor n is not de-valid and we move on to the 
					// next neighbor of the endpoint. 

					if( not backward_modification )
						continue;
					//std::cout<< "backward modification:"<<std::endl<<*backward_modification<<std::endl;
					// Now we must reroute the backward extension with the backward modification. 
					// to resolve the intersection. 
					rectangle.adjust( n );
					this->backward_extension->internal_reroute( backward_modification, rectangle); 
				}
				


                                // If the path could be found the neighbor n is accepted and the extension
                                // modified so as to be an extension of this new end vertex.
                                if( forward_modification ){
                                        //std::cout<< " forward modification: "<< *forward_modification ;
                                        rectangle.adjust( n );
                                        *walk + n;

                                        // When the forward modifcation exceeds the rectangle, we construct a
                                        // completely new extension from the forward modification, to replace the old forward extension.
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
                                        this->adjust( n );
                                        break;

                                }
                        }
                }



};
