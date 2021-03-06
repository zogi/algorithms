// Zoltán Gilián <zoltan.gilian@gmail.com>
// 2014

#include <cstring>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <set>

#include "priority_queue.h"

namespace mcmf {

template <typename T>
struct matrix: std::vector<T>
{
	typedef typename std::vector<T> base;
	typedef T result_type;
	matrix(int rows_=0, int cols_=0, T val = T()): base(rows_*cols_, val), rows(rows_), cols(cols_) {}
	T & operator()(int i, int j) { return base::operator[](i*cols+j); }
	T operator()(int i, int j) const { return base::operator[](i*cols+j); }
	void resize(int rows_, int cols_, T t = T()) { base::resize(rows_*cols_, t); rows = rows_; cols = cols_; }

	int rows, cols;
};

namespace detail {

template <typename HasArc, typename ArcCost>
bool dijkstra(int num_nodes, int start_node, int goal_node, HasArc has_arc, ArcCost arc_cost, std::vector<int> & parent)
{
	parent.resize(num_nodes, -1);

	auto visited = std::vector<bool>(num_nodes, false);
	auto q = priority_queue<int, double, std::greater<double>>();

	q.increase_priority(start_node, 0);
	while (!q.empty()) {
		int i = q.top_element();
		double di = q.top_priority();
		q.pop();
		visited[i] = true;
		if (i == goal_node) return true;
		for (int j = 0; j < num_nodes; ++j) {
			if (visited[j]) continue;
			if (!has_arc(i, j)) continue;
			double dj = di + arc_cost(i, j);
			if (q.increase_priority(j, dj)) {
				parent[j] = i;
			}
		}
	}
	return false;
}

template <typename Residual>
struct has_arc_residual
{
	has_arc_residual(Residual const & residual_): residual(residual_) {}
	bool operator()(int i, int j) const { return residual(i, j) > 0; }
	Residual const & residual;
};

} // namespace detail

template <typename ArcCapacity, typename ArcCost>
void min_cost_max_flow(int num_nodes, int source, int sink, ArcCapacity arc_capacity, ArcCost arc_cost, matrix<int> & flow)
{
	flow.resize(num_nodes, num_nodes, 0);

	auto residual = matrix<int>(num_nodes, num_nodes);
	for (int i = 0; i < num_nodes; ++i) {
		for (int j = 0; j < num_nodes; ++j) {
			residual(i, j) = arc_capacity(i, j);
		}
	}
	auto has_arc = detail::has_arc_residual<matrix<int>>(residual);
	std::vector<int> parent;
	while (detail::dijkstra(num_nodes, source, sink, has_arc, arc_cost, parent)) {
		int path_max = -1;
		for (int i = sink; i != source; i = parent[i]) {
			int this_cap = residual(parent[i], i);
			if (path_max < 0 || this_cap < path_max)
				path_max = this_cap;
		}
		for (int i = sink; i != source; i = parent[i]) {
			int p = parent[i];

			residual(p, i) -= path_max;
			residual(i, p) += path_max;

			int & flow_ip = flow(i, p);
			if (path_max >= flow_ip) {
				path_max -= flow_ip;
				flow_ip = 0;
			}
			flow(p, i) += path_max;
		}
	}
}

namespace detail {

template <typename MatchPossible, typename MatchCost>
struct matching_network
{
	matching_network(int na_, int nb_, MatchPossible const & match_possible_, MatchCost const & match_cost_): na(na_), nb(nb_), source(na_+nb_), sink(na_+nb_+1), match_possible(match_possible_), match_cost(match_cost_) {}
	bool is_source(int i) const { return i == source; }
	bool is_sink(int i) const { return i == sink; }
	bool is_in_a(int i) const { return i < na; }
	bool is_in_b(int i) const { return i >= na && i < na + nb; }
	inline bool has_arc(int i, int j) const;
	inline typename MatchCost::result_type arc_cost(int i, int j) const;
	int capacity(int i, int j) const { return int(has_arc(i, j)); }

	int const na, nb, source, sink;
	MatchPossible const & match_possible;
	MatchCost const & match_cost;
};
template <typename MatchPossible, typename MatchCost>
bool matching_network<MatchPossible, MatchCost>::has_arc(int i, int j) const {
	return
		(is_source(i) && is_in_a(j)) ||
		(is_in_b(i) && is_sink(j)) ||
		(is_in_a(i) && is_in_b(j) && match_possible(i, j-na));
}
template <typename MatchPossible, typename MatchCost>
typename MatchCost::result_type
matching_network<MatchPossible, MatchCost>::arc_cost(int i, int j) const {
	return (is_in_a(i) && is_in_b(j)) ? match_cost(i, j-na) : 0;
};

} // namespace detail

template <typename MatchPossible, typename MatchCost>
void min_cost_max_match(int na, int nb, MatchPossible match_possible, MatchCost match_cost, std::vector<int> & matching)
{
	typedef detail::matching_network<MatchPossible, MatchCost> MatchingNetwork;

	auto nw = MatchingNetwork(na, nb, match_possible, match_cost);
	matrix<int> flow;
	min_cost_max_flow(na + nb + 2, nw.source, nw.sink,
			[&nw](int i, int j){ return nw.capacity(i, j); },
			[&nw](int i, int j){ return nw.arc_cost(i, j); },
			flow);
	matching = std::vector<int>(na, -1);
	for (int i = 0; i < na; ++i) {
		for (int j = 0; j < nb; ++j) {
			if (flow(i, j+na) > 0) {
				matching[i] = j;
				break;
			}
		}
	}
}

} // namespace mcmf
