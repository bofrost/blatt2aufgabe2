/**
 * author: simon
 */

#include <iostream>
#include <vector>
#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

enum Msg{LOAD, TOKEN};

#include "server.hpp"
#include "message.hpp"
#include "task.hpp"
#include "ringNode.hpp"
#include "loadGenerator.hpp"
#include "counter.hpp"

using namespace boost;

//TODO: Einräumen
typedef std::pair<int, int> SocketPair;
SocketPair socket_pairs[10];
std::thread threads[10];

template<class Graph> struct exercise_vertex {
	exercise_vertex(Graph& g_) :
			g(g_) {
	}
	typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

	void operator()(const Vertex& v) const {
		std::vector<int> out;
		std::vector<int> in;
		typedef graph_traits<Graph> GraphTraits;
		typename property_map<Graph, vertex_index_t>::type index = get(
				vertex_index, g);
		typename property_map<Graph, edge_index_t>::type indexE = get(
						edge_index, g);

		std::cout << "out-edges: ";
		typename GraphTraits::out_edge_iterator out_i, out_end;
		typename GraphTraits::edge_descriptor e;
		for (tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i) {
			e = *out_i;
			Vertex src = source(e, g), targ = target(e, g);
			std::cout << indexE(e) << ":(" << index[src] << "," << index[targ] << ") ";
			if(index[src]<index[targ])
				out.push_back(socket_pairs[indexE(e)].second);
			else
				out.push_back(socket_pairs[indexE(e)].first);
		}
		std::cout << std::endl;
		std::cout << "in-edges: ";
		//typedef typename graph_traits<Graph> GraphTraits;
		typename GraphTraits::in_edge_iterator in_i, in_end;
		for (tie(in_i, in_end) = in_edges(v, g); in_i != in_end; ++in_i) {
			e = *in_i;
			Vertex src = source(e, g), targ = target(e, g);
			std::cout << "(" << index[src] << "," << index[targ] << ") ";
			if(index[src]<index[targ])
				in.push_back(socket_pairs[indexE(e)].first);
			else
				in.push_back(socket_pairs[indexE(e)].second);
		}
		Task *task;
		if(index(v)==0)
			task = new LoadGenerator(in, out);
		else
			task = new RingNode(in, out, index(v)==1);
		 //= new Task(in, out);
		threads[index(v)]=std::thread(&Task::loop, task);
		//t.join();

		std::cout << std::endl;
	}
	Graph& g;
};

int main(int argc, char **argv) {
	std::cout << "Starte Test..." << std::endl;

	// http://www.boost.org/doc/libs/1_46_1/libs/graph/doc/quick_tour.html
	// create a typedef for the Graph type
	//typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
	typedef adjacency_list<vecS, vecS, undirectedS, no_property,
			boost::property<boost::edge_index_t, int> > Graph;

	// Make convenient labels for the vertices
	enum {
		L, A, B, C, D, E, N
	};
	const int num_vertices = N;
	const char* name = "LABCDE";

	// writing out the edges in the graph
	typedef std::pair<int, int> Edge;
	Edge edge_array[] = { Edge(A, B), Edge(B, C), Edge(C, D), Edge(D, E), Edge(
			E, A), Edge(L, A), Edge(L, B), Edge(L, C), Edge(L, D), Edge(L, E) };
	const int num_edges = sizeof(edge_array) / sizeof(edge_array[0]);

	// declare a graph object
	Graph g(num_vertices);

	// add the edges to the graph object
	for (int i = 0; i < num_edges; ++i)
		add_edge(edge_array[i].first, edge_array[i].second, i, g);

	//Write graph as dot file http://stackoverflow.com/questions/9181183/how-to-print-a-boost-graph-in-graphviz-with-one-of-the-properties-displayed
	char* file = "./graph.dot";
	std::ofstream dotfile(file);
	write_graphviz(dotfile, g, make_label_writer(&name[0]));

	//create socketpairs
	std::cout << "edges(g) = ";
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
		int src = source(*ei, g), targ = target(*ei, g);
		typename property_map<Graph, edge_index_t>::type index = get(
						edge_index, g);
		std::cout << index(*ei) <<":(" << name[src] << "," << name[targ] << ") ";
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, &socket_pairs[index(*ei)].first) < 0) {
			std::cout << "Error occurred while trying to get a socket pair."
					<< std::endl;
		}
	}
	std::cout << std::endl;

	// get the property map for vertex indices
//	typedef property_map<Graph, vertex_index_t>::type IndexMap;
//	IndexMap index = get(vertex_index, g);
//
//	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
//	typedef typename graph_traits<Graph>
//	      ::vertex_descriptor Vertex;
//	std::pair<vertex_iter, vertex_iter> vp;
//	for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
//
//
//	}

	std::for_each(vertices(g).first, vertices(g).second,
			exercise_vertex<Graph>(g));

	for(int i=0;i<10;i++){
		threads[i].join();
	}
	return 0;
	
	// Aktuell noch nicht so viel. Man kann sich mit Telnet drauf verbinden und Nachrichten verschicken.
	// Wenn der erste Buchstabe ein kleines q ist wird der Server beendet
	// Noch kein Weiterleiten des Tokens, oder synchronisiertes Verhalten
	Counter* c1 = new Counter();
	Counter* c2 = new Counter();
	Counter* c3 = new Counter();

	Server* server1 = new Server("1111", "1112", c1);
	Server* server2 = new Server("1112", "1113", c2);
	Server* server3 = new Server("1113", "1111", c3);

	std::thread t1(&Server::start, server1);
	std::thread t2(&Server::start, server2);
	std::thread t3(&Server::start, server3);
	
	t1.join();
	t2.join();
	t3.join();

	delete server1;
	delete server2;
	delete server3;
	server1 == NULL;
	server2 == NULL;
	server3 == NULL;
	delete c1;
	delete c2;
	delete c3;
	c1 == NULL;
	c2 == NULL;
	c3 == NULL;

	return 0;

}
