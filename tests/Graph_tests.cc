#include <gtest/gtest.h>
#include <Graph/Graph.h>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <cctype>

TEST(GRAPH_METHODS, First) {
	Graph<int> graph;
	for(int v = -5; v < 5; ++v)
		graph.add_vertex(v);
	for (const auto& vert : graph.vertices()) {
		std::cout << vert << " ";
	}
	graph.add_edge(-5, -4, 8.5);
	graph.add_edge(-4, -3, 7.2);
	graph.add_edge(-3, -2, 6.9);
	graph.add_edge(-2, 0, 6.9);
	graph.add_edge(-2, -3, 6.9);
	graph.add_edge(-2, 3, 6.9);
	graph.add_edge(0, 1, 6.9);
	graph.add_edge(2, 1, 3.7);
	graph.add_edge(2, 3, 2.9);
	graph.add_edge(3, 2, 2.4);
	graph.add_edge(3, 4, 2.4);
	graph.add_edge(3, -1, 2.4);
	for (int i = -5; i < 5; ++i) {
		EXPECT_TRUE(graph.has_vertex(i));
	}
	std::cout << "\nEdges from '-5':\n";
	for (const auto& edge : graph.edges(-2)) {
		std::cout << edge.to << " ";
	}
}

TEST(GRAPH_TASK, MainTest) {
	//Example from https://www.geeksforgeeks.org/bellman-ford-algorithm-simple-implementation/
	Graph<int> graph;
	for (int v = 1; v < 6; ++v)
		graph.add_vertex(v);
	graph.add_edge(1, 2, -1);
	graph.add_edge(1, 3, 4);
	graph.add_edge(2, 3, 3);
	graph.add_edge(2, 4, 2);
	graph.add_edge(2, 5, 2);
	graph.add_edge(4, 2, 1);
	graph.add_edge(4, 3, 5);
	graph.add_edge(5, 4, -3);
	for (int v = 1; v < 6; ++v) {
		std::cout << "\nFrom " << v << std::endl;
		for (const auto& [_, dist] : graph.get_distances(v)) {
			std::cout << dist << " " << std::endl;
		}
	}
	std::cout << "\nThe best place for a warehouse: " << get_center(graph) << std::endl;
	std::cout << "\nThe shortest path from 1 to 4:" << std::endl;
	std::cout << "1 ";
	for (const auto& it : graph.shortest_path(1, 4)) {
		std::cout << it.to << " ";
	}
	std::cout << "\nDegree of 2: " << graph.degree(2) << std::endl;

}

TEST(GRAPH_TASK, FromHomeworkGraphTheory) {
	Graph<int> graph;
	for (int v = 1; v < 7; ++v)
		graph.add_vertex(v);
	graph.add_edge(1, 2, 6);
	graph.add_edge(1, 3, 2);
	graph.add_edge(1, 4, 5);
	graph.add_edge(2, 4, 6);
	graph.add_edge(2, 5, 7);
	graph.add_edge(2, 6, 6);
	graph.add_edge(3, 2, -5);
	graph.add_edge(3, 5, 6);
	graph.add_edge(4, 5, -4);
	graph.add_edge(4, 6, 5);
	graph.add_edge(5, 6, 7);
	for (int v = 1; v < 7; ++v) {
		std::cout << "\nFrom " << v << std::endl;
		for (const auto& [_, dist] : graph.get_distances(v)) {
			std::cout << dist << " " << std::endl;
		}
	}
	std::cout << "\nThe best place for a warehouse: " << get_center(graph) << std::endl;
	std::cout << "\nThe shortest path from 1 to 6:" << std::endl;
	std::cout << "1 ";
	for (const auto& it : graph.shortest_path(1, 6)) {
		std::cout << it.to << " ";
	}
	Graph<int>::Edge e{ 3, 2, -5 };
	EXPECT_TRUE(graph.has_edge(e));
	graph.remove_edge(e);
	EXPECT_FALSE(graph.has_edge(3,2));
	graph.remove_vertex(1);
	std::cout << "\n\nAfter remove vertex 1:" << std::endl;
	for (int v = 2; v < 7; ++v) {
		std::cout << "\nFrom " << v << std::endl;
		for (const auto& [_, dist] : graph.get_distances(v)) {
			std::cout << dist << " " << std::endl;
		}
	}
}
