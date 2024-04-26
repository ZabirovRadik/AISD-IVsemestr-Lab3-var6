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
		bool operator==(const Edge& e) const{
			if (from == e.from && to == e.to && dist == e.dist)
				return true;
			return false;
		}
	}
private:
	std::unordered_set<Vertex> _vertices;
	std::map<Vertex, std::vector<Edge>> _edges; 
	static std::vector<Vertex> for_a_walk;
public:
	//проверка-добавление-удаление вершин
	bool has_vertex(const Vertex& v) const {
		return _vertices.find(v);
	}

	void add_vertex(const Vertex& v) {
		if (_vertices.find(v) != _vertices.cend())
			return false;
		_vertices.insert(v);
		_edges.insert(v, {});
	}

	bool remove_vertex(const Vertex& v) {
		auto it = _vertices.find(v);
		if (it == _vertices.cend())
			return false;
		_vertices.erace(it);
		_edges.remove(v);
		for (const Vertex& from : _vertices) {
			auto& edges = _edges[from];
			for (auto it = edges.begin(); it != edges.end(); ++it)
				if (it->to == vertix) {
					edges.erace(it);
					--it;
				}
		}
		return true;
	}

	const std::vector<Vertex>& vertices() const {
		std::vector<Vertex> vector(_vertises.begin(), _vertises.end());
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
		for (const Edge& it : edges) {
			if (it->from == from && it->to == to) {
				edges.erace(it);
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
		for (const Edge& it : edges) {
			if (*it == e) {
				edges.erace(it);
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
		auto& edges = _edges[from];
		for (const Edge& it : edges) {
			if (it->from == from && it->to == to) {
				return true;
			}
		}
		return false;
	}

	bool has_edge(const Edge& e) const{ //c учетом расстояния в Edge
		if (_vertices.find(from) == _vertices.cend())
			throw std::out_of_range("Unknown from");
		if (_vertices.find(to) == _vertices.cend())
			throw std::out_of_range("Unknown to");
		auto& edges = _edges[from];
		for (const Edge& it : edges) {
			if (*it == e) {
				return true;
			}
		}
		return false;
	}

	//получение всех ребер, выходящих из вершины
	const std::vector<Edge>& edges(const Vertex& vertex) const{
		if(_vertices.find(vertex) == _vertices.cend())
			throw std::out_of_range("Unknown vertex");
		return _edges[from];
	}

	size_t order() const { //порядок
		return _vertices.size();
	}

	size_t degree(const Vertex& v) const {//степень вершины
		if (_vertices.find(vertex) == _vertices.cend())
			throw std::out_of_range("Unknown vertex");
		auto& edges_of_v = _edges[v.from];
		size_t counter = edges_of_v.cend() - edges_of_v.cbegin();
		for (const auto& vector : _edges) {
			if (edges_of_v == vector)
				continue;
			for (const Edge& it : vector) {
				if (it->to == v.to)
					++counter;
			}
		}
		return counter;
	}

	const std::map<Vertex, Distance>& get_distances(const Vertex& from) {
		if (_vertices.find(vertex) == _vertices.cend())
			throw std::out_of_range("Unknown vertex");
		std::map<Vertex, Distance> distances;
		size_t i = 0;
		for (const Vertex& v : _vertices) {
			if (v == from)
				continue;
			distances[v] = std::numeric_limits<Distance>::max();
		}
		std::deque<Vertex> deq;
		std::unordered_set<Vertex> history;
		deq.push_back(from);
		history.insert(from);
		while (!deq.empty()) {
			auto& edges = _edges[deq.front()];
			for (const auto& it : edges) {
				distances[it->to] = std::min(distances[it->to], distances[it.from] + it.dist);
				if (it->to in history)
					deq.push_front(it->to);
				else {
					deq.push_back(it->to);
					history.insert(it->to);
				}
			}
			deq.pop_front();
		}
		return distances;
	}

	//поиск кратчайшего пути
	const std::vector<Edge>& shortest_path(const Vertex& from, const Vertex& to) const {
		std::map<Vertex, Distance> distances = get_distances(from);
		std::deque<Vertex> deq;
		std::deque<Edge> answer;
		deq.push_front(to);
		while (deq.front() != from) {
			bool exit = false;
			for (const Vertex & v : _vertices) {
				if (v == to)
					continue;
				const auto& edges = _edges[v];
				for (const Edge& it : edges) {
					if (it->to == deq.front() && distances[it->to] - it->dist == distance[it->from]) {
						answer.push_front(*it);
						deq.push_front(it->from);
						exit = true;
						break;
					}
				}
				if (exit) {
					break;
				}
			}
		}
		std::vector<Edge> vector(answer.end(), answer.begin());
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
			const auto& edges = _edges[front_elem];
			for (auto& it : _edges) {
				if (!marks[it->to])
					order_immersion.push_front(it->to);
			}
			order_immersion.pop_front();
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
			const auto& edges = _edges[front_elem];
			for (auto& it : _edges) {
				if (!marks[it->to])
					order_immersion.push_front(it->to);
			}
			order_immersion.pop_front();
		}
	}

	void print_vertex(const Vertex& v) const {
		std::cout << v << " ";
	}

	void print(const Vertex& start_vertex) const{
		std::cout << "Your graph when traversing in depth:" << std::endl;
		walk(start_vertex, print_vertex);
	}

	void push_to_static_vector(const Vertex& v) {
		for_a_walk.push_back(v);
	}

	const std::vector<Vertex>& print(const Vertex& start_vertex) const {
		walk(start_vertex, push_to_static_vector);
		return for_a_walk;
	}
};


template<typename Vertex, typename Distance = double>
const Vertex& get_center(const Graph<Vertex, Distance>& graph) {
	std::vector<Vertex> vertices = graph.vertices();
	Vertex center;
	Distance best_of_worst = std::numeric_limits<Distance>::max();
	for (const auto& v : vertices) {
		Distance max_dist = std::numeric_limits<Distance>::min();
		for (const auto& [_, distance] : graph.get_distances(v)) {
			if (distance > answer)
				max_dist = distance;
		}
		if (max_dist < best_of_worst) {
			best_of_worst = max_dist;
			center = v;
		}
	}
	return center;
}
