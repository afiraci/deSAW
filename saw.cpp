#include <vector>
#include <iostream>
#include <memory>
#include "doubly_linked_dictionary.cpp"
#include "doubly_linked_list.cpp"
#include <random>

#include <sciplot/sciplot.hpp>



int main(int argc, char** argv)
{
	std::random_device rd;
    	std::mt19937 g(rd());

	std::unique_ptr< Doubly_Linked_Dictionary > W = std:: unique_ptr< Doubly_Linked_Dictionary >( new Doubly_Linked_Dictionary() );
	W + Vertex(0,0);

	for( int i = 0; i<10000; i++){

		std::array<Vertex, 4> neighbors = normal_neighbors( W->end()->v );
		std:: shuffle( neighbors.begin(), neighbors.end(), g );		
		for( auto & v :  neighbors ){
			if( W->contains( v ) ){
				continue;
		       	}else{
				W + v;
				std::cout<< W->end()<< std::endl;
				break;
			}
		}
		
	}
	std::vector<int> W_x;
	std::vector<int> W_y;
	Node_Itr itr( W->start() );
	while( itr.current != nullptr ){
		W_x.push_back(itr.current->v.x);
		W_y.push_back(itr.current->v.y);
		itr.advance();
	}

	std:: vector< int > vec;
    	vec.push_back(1);
    	vec.push_back(2);
   	vec.push_back(3);
    	vec.push_back(4);
    	vec.push_back(5);

    	sciplot:: Plot2D plot;
    	plot.drawCurve(W_x, W_y).label("SAW");

    	sciplot::Figure fig = {{plot}};
    	// Create canvas to hold figure
    	sciplot::Canvas canvas = {{fig}};
    	// Save the plot to a PDF file
    	canvas.save("SAW");

return 0;
}
