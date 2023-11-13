#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include "deSAW.cpp"
#include <sciplot/sciplot.hpp>
#include <bits/stdc++.h>



int main(int argc, char** argv)
{
	
	std:: random_device rd;
	auto seed = rd();
	std::cout<< "seed for this run : "<< seed << std::endl;
        std::mt19937 mt( seed );
	time_t start, end;
	
	
	Doubly_Extendable_SAW de_saw;

	time(&start);

	for( int i = 0; i<1000000; i++)
		de_saw.extend( mt );
	
	time(&end);
	
	double time_taken = double(end - start);
	std:: cout << "Time taken by program is : " << std:: fixed
        << time_taken << std::setprecision(5);
	std:: cout << " sec " << std::endl;
	//std::cout<<"walk: "<< std::endl << *(de_saw.walk) << std::endl;
	//std::cout<<"forward_extension: "<<std::endl << *(de_saw.forward_extension) <<std::endl;	
	//std::cout<<"backward_extension: "<<std::endl << *(de_saw.backward_extension) <<std::endl;
	
	std::vector<int> W_x;
	std::vector<int> W_y;
	Node_Itr itr( de_saw.start() );
	while( itr.current != nullptr ){
		W_x.push_back(itr.current->v.x);
		W_y.push_back(itr.current->v.y);
		itr.advance();
	}


	std::vector<int> E_x;
        std::vector<int> E_y;

	Node_Itr itr1(de_saw.forward_extension->start() );
	while( itr1.current != nullptr ){
		E_x.push_back(itr1.current->v.x);
                E_y.push_back(itr1.current->v.y);
                itr1.advance();

	}

	std::vector<int> B_x;
        std::vector<int> B_y;

        Node_Itr itr2(de_saw.backward_extension->start() );
        while( itr2.current != nullptr ){
                B_x.push_back(itr2.current->v.x);
                B_y.push_back(itr2.current->v.y);
                itr2.advance();

        }



    	sciplot:: Plot2D plot;
    	plot.drawCurve(W_x, W_y).label("deSAW");
	plot.drawCurve(E_x, E_y).label("F_Ext" );
	plot.drawCurve(B_x, B_y).label("B_Ext" );
    	sciplot::Figure fig = {{plot}};
    	// Create canvas to hold figure
    	sciplot::Canvas canvas = {{fig}};
    	// Save the plot to a PDF file
    	canvas.save("deSAW.pdf");
	
	return 0;
}
