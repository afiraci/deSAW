#include <vector>
#include <iostream>
#include <memory>
#include "doubly_linked_dictionary.cpp"
#include "doubly_linked_list.cpp"
#include <random>
#include "eSAW.cpp"
#include <sciplot/sciplot.hpp>
#include <bits/stdc++.h>



int main(int argc, char** argv)
{
	
	std:: random_device rd;
	auto seed = rd();
	std::cout<< "seed for this run : "<< seed << std::endl;
        std::mt19937 mt( seed );
	time_t start, end;
	
	Extendable_SAW e_saw;

	time(&start);

	for( int i = 0; i<1000000; i++)
		e_saw.extend( mt );
	
	time(&end);
	
	double time_taken = double(end - start);
	std:: cout << "Time taken by program is : " << std:: fixed
        << time_taken << std::setprecision(5);
	std:: cout << " sec " << std::endl;

	std::vector<int> W_x;
	std::vector<int> W_y;
	Node_Itr itr( e_saw.start() );
	while( itr.current != nullptr ){
		W_x.push_back(itr.current->v.x);
		W_y.push_back(itr.current->v.y);
		itr.advance();
	}


	std::vector<int> E_x;
        std::vector<int> E_y;

	Node_Itr itr1( e_saw.forward_extension->start() );
	while( itr1.current != nullptr ){
		E_x.push_back(itr1.current->v.x);
                E_y.push_back(itr1.current->v.y);
                itr1.advance();

	}


    	sciplot:: Plot2D plot;
    	plot.drawCurve(W_x, W_y).label("eSAW");
	plot.drawCurve(E_x, E_y).label("Ext" );
    	sciplot::Figure fig = {{plot}};
    	// Create canvas to hold figure
    	sciplot::Canvas canvas = {{fig}};
    	// Save the plot to a PDF file
    	canvas.save("eSAW.pdf");

return 0;
}
