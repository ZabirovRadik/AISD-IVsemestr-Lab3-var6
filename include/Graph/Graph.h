#pragma once
#include <iostream>
#include <unordered_set>
#include <random>
#include <map>
#include <deque>
#include <functional>
#define LEN_WORD sizeof(size_t) * 8
#define L 11
#define A 33333

template<typename Vertex, typename Distance = double>
class Graph {
public:
	struct Edge {
		Vertex from, to;
		Distance dist;
		bool operator==(const Edge& e) const {
			if (from == e.from && to == e.to && dist == e.dist)
				return true;
			return false;
		}
	};
private:
	std::unordered_set<Vertex> _vertices;
	std::map<Vertex, std::vector<Edge>> _edges;
	std::vector<Vertex> for_a_walk;
public:
	

	//проверка-добавление-удаление вершин
	bool has_vertex(const Vertex& v) const {
		return _vertices.find(v) != _vertices.end();
	}

	void add_vertex(const Vertex& v) {
		if (_vertices.find(v) != _vertices.cend())
			return;
		_vertices.insert(v);
		_edges.emplace(v, std::vector<Edge>{});
	}

	bool remove_vertex(const Vertex& v) {
		auto it = _vertices.find(v);
		if (it == _vertices.cend())
			return false;
		_vertices.erase(it);
		_edges.erase(v);
		for (const Vertex& from : _vertices) {
			auto& edges = _edges[from];
			for (auto it = edges.begin(); it != edges.end(); ++it)
				if (it->to == v) {
					edges.erase(it);
					--it;
				}
		}
		return true;
	}

	const std::unordered_set<Vertex>& vertices() const {
		return _vertices;
	}

	std::vector<Vertex> vector_of_vertices() const {
		std::vector<Vertex> vector(_vertices.begin(), _vertices.end());
		return vector;
	}

	//проверка-добавление-удаление ребер
	void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
		if (_vertices.find(from) == _vertices.cend())
			throw std::out_of_range("Unknown from");
		if (_vertices.find(to) == _vertices.cend())
			throw std::out_of_range("Unknown to");
		_edges[from].push_back(Edge{ from,to,d });
	}

	bool remove_edge(const Vertex& from, const Vertex& to) {
		if (_vertices.find(from) == _vertices.cend())
			throw std::out_of_range("Unknown from");
		if (_vertices.find(to) == _vertices.cend())
			throw std::out_of_range("Unknown to");
		auto& edges = _edges[from];
		for (size_t i = 0; i < edges.size(); ++i) {
			if (edges[i].from == from && edges[i].to == to) {
				edges.erase(edges.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool remove_edge(const Edge& e) { //c учетом расстояния
		if (_vertices.find(e.from) == _vertices.cend())
			throw std::out_of_range("Unknown from");
		if (_vertices.find(e.to) == _vertices.cend())
			throw std::out_of_range("Unknown to");
		auto& edges = _edges[e.from];
		for (size_t i = 0; i < edges.size(); ++i) {
			if (edges[i] == e) {
				edges.erase(edges.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool has_edge(const Vertex& from, const Vertex& to) const {
		if (_vertices.find(from) == _vertices.cend())
			throw std::out_of_range("Unknown from");
		if (_vertices.find(to) == _vertices.cend())
			throw std::out_of_range("Unknown to");
		auto iter_on_elem = _edges.find(from);
		auto& edges = iter_on_elem->second;
		for (const Edge& it : edges) {
			if (it.from == from && it.to == to) {
				return true;
			}
		}
		return false;
	}

	bool has_edge(const Edge& e) const{ //c учетом расстояния в Edge
		if (_vertices.find(e.from) == _vertices.cend())
			throw std::out_of_range("Unknown from");
		if (_vertices.find(e.to) == _vertices.cend())
			throw std::out_of_range("Unknown to");
		auto iter_on_elem = _edges.find(e.from);
		auto& edges = iter_on_elem->second;
		for (const Edge& it : edges) {
			if (it == e) {
				return true;
			}
		}
		return false;
	}

	//получение всех ребер, выходящих из вершины
	const std::vector<Edge>& edges(const Vertex& vertex) const{
		if(_vertices.find(vertex) == _vertices.cend())
			throw std::out_of_range("Unknown vertex");
		auto iter_on_elem = _edges.find(vertex);
		return iter_on_elem->second;
	}

	size_t order() const { //порядок графа
		return _vertices.size();
	}

	size_t degree(const Vertex& v) const {//степень вершины
		if (_vertices.find(v) == _vertices.cend())
			throw std::out_of_range("Unknown vertex");
		auto iter_on_elem = _edges.find(v);
		auto& edges_of_v = iter_on_elem->second;
		size_t counter = edges_of_v.size();
		for (const auto& [ _, edges ] : _edges) {
			if (edges_of_v == edges)
				continue;
			for (const Edge& it : edges) {
				if (it.to == v)
					++counter;
			}
		}
		return counter;
	}

	std::map<Vertex, Distance> get_distances(const Vertex& from) const{
		if (_vertices.find(from) == _vertices.cend())
			throw std::out_of_range("Unknown vertex");
		std::map<Vertex, Distance> distances;
		size_t i = 0;
		for (const Vertex& v : _vertices) {
			distances[v] = std::numeric_limits<Distance>::max();
		}
		distances[from] = 0;
		for (std::size_t i = 0; i < _vertices.size() - 1; ++i) {
			for (const auto& [ _,edges ] : _edges) {
				for(const auto& edge: edges)
					if (distances[edge.from] < distances[edge.to] - edge.dist) {
						distances[edge.to] = distances[edge.from] + edge.dist;
					}
			}
		}
		for (const auto& [_, edges] : _edges) {
			for (const auto& edge : edges)
				if (distances[edge.from] < distances[edge.to] - edge.dist) {
					std::cout << "Graph contains negative cycle" << std::endl;
				}
		}
		return distances;
	}

	//поиск кратчайшего пути
	std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
		std::map<Vertex, Distance> distances = get_distances(from);
		std::deque<Vertex> deq;
		std::deque<Edge> answer;
		deq.push_front(to);
		while (deq.front() != from) {
			bool exit = false;
			for (const Vertex & v : _vertices) {
				if (v == to)
					continue;
				auto iter_on_elem = _edges.find(v);
				const auto& edges = iter_on_elem->second;
				for (const Edge& it : edges) {
					if (it.to == deq.front() && distances[it.to] - it.dist == distances[it.from]) {
						answer.push_front(it);
						deq.push_front(it.from);
						exit = true;
						break;
					}
				}
				if (exit) {
					break;
				}
			}
		}
		std::vector<Edge> vector(answer.begin(), answer.end());
		return vector;
	}

	Distance get_longest_distance(const std::map<Vertex, Distance>& distances) {
		Distance answer = std::numeric_limits<Distance>::min();
		for (const auto& [_, distance] : distances) {
			if (distance > answer)
				answer = distance;
		}
		return answer;
	}

	//обход
	void walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) {
		std::map<Vertex, bool> marks;
		for (const Vertex& v : _vertices) {
			marks[v] = false;
		}
		std::deque<Vertex> order_immersion;
		order_immersion.push_front(start_vertex);
		while (!order_immersion.empty()) {
			auto& front_elem = order_immersion.front();
			if (marks[front_elem]) {
				order_immersion.pop_front();
				continue;
			}
			action(front_elem);
			marks[front_elem] = true;
			order_immersion.pop_front();
			auto iter_on_elem = _edges.find(front_elem);
			if (iter_on_elem != _edges.cend())
				for (auto& it : iter_on_elem->second) {
					if (!marks[it.to])
						order_immersion.push_front(it.to);
				}
		}
	}

	void walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) const{
		std::map<Vertex, bool> marks;
		for (const Vertex& v : _vertices) {
			marks[v] = false;
		}
		std::deque<Vertex> order_immersion;
		order_immersion.push_front(start_vertex);
		while (!order_immersion.empty()) {
			auto& front_elem = order_immersion.front();
			if (marks[front_elem]) {
				order_immersion.pop_front();
				continue;
			}
			action(front_elem);
			marks[front_elem] = true;
			order_immersion.pop_front();
			auto iter_on_elem = _edges.find(front_elem);
			if (iter_on_elem != _edges.cend()) 
				for (auto& it : iter_on_elem->second) {
					if (!marks[it.to])
						order_immersion.push_front(it.to);
				}
		}
	}

	void print_vertex(const Vertex& v) const {
		std::cout << v << " ";
	}

	void print(const Vertex& start_vertex) const{
		std::cout << "Your graph when traversing in depth:" << std::endl;
		std::function<void(const Vertex&)> action = [this](const Vertex& v) { print_vertex(v); };
		walk(start_vertex, action);
	}

	void push_to_static_vector(const Vertex& v) {
		for_a_walk.push_back(v);
	}

	const std::vector<Vertex>& graph_to_vector(const Vertex& start_vertex) {
		std::function<void(const Vertex&)> action = [this](const Vertex& v) { push_to_static_vector(v); };
		for_a_walk.clear();
		walk(start_vertex, action);
		return for_a_walk;
	}
};


template<typename Vertex, typename Distance = double>
const Vertex& get_center(const Graph<Vertex, Distance>& graph) {
	std::unordered_set<Vertex> vertices = graph.vertices();
	if (vertices.size() == 0)
		throw std::out_of_range("graph without vertices");
	Vertex center;
	Distance best_of_worst = std::numeric_limits<Distance>::max();
	for (const auto& v : vertices) {
		Distance max_dist = std::numeric_limits<Distance>::min();
		for (const auto [_, distance] : graph.get_distances(v)) {
			if (distance > max_dist)
				max_dist = distance;
		}
		if (max_dist < best_of_worst) {
			best_of_worst = max_dist;
			center = v;
		}
	}
	return center;
}
