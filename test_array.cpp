#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include "doubly_linked_dictionary.cpp"
#include <vector>
#include <optional>
#include "eSAW.cpp"




std:: array<Vertex,4> shuffled_array( std:: mt19937 g ){
	std:: array<Vertex, 4> arr;

	for( int i=0; i<4; i++){
                arr[i] = Vertex(0,i);
        }

	std::shuffle( arr.begin(), arr.end(), g );

	return arr;

}



int main(){
	/*
	std::random_device rd;
        std::mt19937 g(rd());
	std:: array< Vertex, 4 > arr = shuffled_array( g );
	for( int i=0; i<4; i++){
                std::cout<< arr[i].x <<" "<< arr[i].y << std::endl;
        }
	std:: vector< Vertex > vec;
	
	for( int i=0; i<5; i++ ){
		vec.push_back( Vertex(0,i) );
	}

	

        for( int i=1; i<5; i++ ){
                vec.push_back( Vertex(i,0) );
        }

	
	std:: unique_ptr< Path_t > path = find_path( Vertex(0,0), Vertex(0,4), vec  );
	
	if (path)
		std::cout << path;
	else
		std::cout<< "no path could be found" << std::endl;
	*/
	/*
	Doubly_Linked_List dll;
	
	dll.append( Vertex(0,0) );
	dll.append( Vertex(0,1) );
	dll.append( Vertex(0,2) );
	Extension  e;
	e + Vertex(0,1);
	e + Vertex(0,2);
	e + Vertex(0,3);
	e.loop_erase( dll );
	dll + e;
	std::cout<< e; 
	*/

	Extendable_SAW e_saw;
	e_saw.extend();
	e_saw.extend();
	e_saw.extend();

	return 0;
}	

