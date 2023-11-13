#include "deSAW.cpp"

int main(){
 
	Doubly_Linked_Dictionary dld;

	dld + Vertex(0,0);
	dld + Vertex(0,1);
	dld + Vertex(0,2);
	dld + Vertex(0,3);
	/*
	//std::cout<< dld.min( Vertex(0,3), Vertex(0,2) )<<std::endl;
	
	std::shared_ptr< Path > path = std::shared_ptr< Path > ( new Path() );
	path->append( Vertex(0,3) );
//	path->append( Vertex(0,2) );
//	path->append( Vertex(0,1) );
	std::cout<< dld.first( path ) <<std::endl;

	*/

	dld.insert( Vertex(0,5), Vertex(0,3), Vertex(0,2));
	dld.remove( Vertex(0,5) );

	std::cout<< dld.contains( Vertex(0,5)) <<std::endl;
	std::cout<< *(dld.list) <<std::endl;


}
