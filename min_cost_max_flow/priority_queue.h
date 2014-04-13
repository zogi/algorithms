// Zoltán Gilián <zoltan.gilian@gmail.com>
// 2014

#include <vector>
#include <map>

template <typename Elem, typename Prio, typename Comp = std::less<Prio> >
struct priority_queue
{
	priority_queue(Comp comp_ = Comp()): comp(comp_) {}
	bool increase_priority(Elem e, Prio p);
	void pop();
	Elem top_element() const { return es[0].first; }
	Prio top_priority() const { return es[0].second; }
	bool empty() const { return es.empty(); }
private:
	typedef std::map<Elem, int> ElemMap;
	std::vector<std::pair<Elem, Prio> > es;
	ElemMap m;
	Comp comp;
	void bubble_up(int i);
	void sink_down(int i);
};

template <typename Elem, typename Prio, typename Comp>
void priority_queue<Elem, Prio, Comp>::bubble_up(int i)
{
	for (int j = ((i+1)>>1)-1; i > 0; i = j, j = ((j+1)>>1)-1) {
		if (comp(es[j].second, es[i].second)) {
			std::swap(m[es[j].first], m[es[i].first]);
			std::swap(es[j], es[i]);
		}
	}
}

template <typename Elem, typename Prio, typename Comp>
void priority_queue<Elem, Prio, Comp>::sink_down(int i)
{
	while (true) {
		int r = (i+1)<<1, l = r - 1;
		if (l >= es.size()) break;
		int g = l;
		if (r < es.size() && comp(es[l].second, es[r].second)) {
			g = r;
		}
		if (comp(es[i].second, es[g].second)) {
			std::swap(m[es[i].first], m[es[g].first]);
			std::swap(es[i], es[g]);
		} else {
			break;
		}
		i = g;
	}
}

template <typename Elem, typename Prio, typename Comp>
bool priority_queue<Elem, Prio, Comp>::increase_priority(Elem e, Prio p)
{
	typename ElemMap::iterator it = m.find(e);
	if (it == m.end()) {
		es.push_back(std::make_pair(e, p));
		it = m.insert(std::make_pair(e, es.size()-1)).first;
	} else {
		if (!comp(es[it->second].second, p)) return false;
		es[it->second].second = p;
	}
	bubble_up(it->second);
	return true;
}

template <typename Elem, typename Prio, typename Comp>
void priority_queue<Elem, Prio, Comp>::pop()
{
	std::swap(m[es.front().first], m[es.back().first]);
	std::swap(es.front(), es.back());
	m.erase(es.back().first);
	es.pop_back();
	sink_down(0);
}
