#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <set>
#include <vector>
#include <limits>
#include <algorithm>
#include <stdio.h>

const int INF = std::numeric_limits<int>::max();

class suffix_tree {
private:
	struct search_result {
		int depth;
		bool first;
		bool second;
		int end_index;
		search_result(int depth, bool first, bool second) {
			this->depth = depth;
			this->first = first;
			this->second = second;
			this->end_index = -1;
		}
	};
	
	struct suffix_tree_suffix;
	struct suffix_tree_edge;

	struct suffix_tree_node {
		suffix_tree_node * suffix_link;
		suffix_tree_node * next_to_delete;
		suffix_tree * tree;
		std::map<char, suffix_tree_edge *> edges;

		suffix_tree_node(suffix_tree_node * suffix_link, suffix_tree * tree) {
			this->suffix_link = suffix_link;
			this->tree = tree;
		}

		void add_edge(char ch, suffix_tree_edge * edge)	{
			edges.insert(std::make_pair(ch, edge));
		}

		void remove_edge(char ch) {
			edges.erase(ch);
		}

		suffix_tree_edge * find_edge(char ch) {
			auto it = edges.find(ch);
			if (it == edges.end()) {
				return NULL;
			} else {
				return it->second;
			}
		}

		search_result find_delimiters(int first_delimiter, int second_delimiter, int current_depth)	{
			search_result result(-1, false, false);
			if (edges.empty()) {
				return result;
			}

			result.depth = current_depth;
			for(auto it = edges.begin(); it != edges.end(); ++it) {
				search_result temp = it->second->find_delimiters(first_delimiter, second_delimiter, current_depth);
				int distance = std::distance(edges.begin(), it);
				result.first = result.first || temp.first;
				result.second = result.second || temp.second;
				if (result.depth < temp.depth)
				{
					result.depth = temp.depth;
					result.end_index = temp.end_index;
				}
			}
			if (!result.first || !result.second) {
				result.depth = -1;
			}
			return result;
		}
	};

	struct suffix_tree_edge {
		int begin_index;
		int end_index;
		suffix_tree_node * start_node;
		suffix_tree_node * end_node;
		suffix_tree_edge * next_to_delete;

		suffix_tree_edge(int begin, int end, suffix_tree_node * start) {
			begin_index = begin;
			end_index = end;
			start_node = start;
			end_node = new suffix_tree_node(NULL, start->tree);
			get_tree()->update_last_node(end_node);
		}

		suffix_tree_node * split_edge(suffix_tree_suffix * suffix) {
			remove();
			suffix_tree_edge * new_edge = new suffix_tree_edge(begin_index, begin_index + suffix->get_span(), suffix->origin_node);
			get_tree()->update_last_edge(new_edge);
			new_edge->insert();
			new_edge->end_node->suffix_link = suffix->origin_node;
			begin_index += suffix->get_span() + 1;
			start_node = new_edge->end_node;
			insert();
			return new_edge->end_node;
		}

		void insert() {
			char begin_char = get_tree()->get_char(begin_index);
			start_node->add_edge(begin_char, this);
		}

		void remove() {
			char begin_char = get_tree()->get_char(begin_index);
			start_node->remove_edge(begin_char);
		}

		int get_span() {
			return end_index - begin_index;
		}

		bool contains(int char_index) {
			return char_index >= begin_index && char_index <= end_index;	
		}

		suffix_tree * get_tree() {
			return start_node->tree;
		}

		search_result find_delimiters(int first_delimiter, int second_delimiter, int current_depth) {
			int min_distance = std::min(first_delimiter - begin_index, second_delimiter - begin_index);
			search_result result(-1, false, false);
			result.first = contains(first_delimiter);
			result.second = contains(second_delimiter);
			if (result.first && result.second) {
				result.first = true;
				result.second = false;
				return result;
			}

			search_result temp = end_node->find_delimiters(first_delimiter, second_delimiter, current_depth + get_span()+1);
			if (temp.first && temp.second) {
				result.end_index = temp.end_index == -1 ? end_index : temp.end_index;
				temp.end_index = result.end_index;
			}

			if (!result.first && !result.second) {
				return temp;
			}

			result.first = result.first || temp.first;
			result.second = result.second || temp.second;

			if (!result.first || !result.second) {
				result.depth = -1;
			}
			return result;
		}
	};

	struct suffix_tree_suffix {
		suffix_tree_node * origin_node;
		int begin_index;
		int end_index;

		suffix_tree * get_tree()
		{
			return origin_node->tree;
		}

		suffix_tree_suffix(suffix_tree_node * origin, int begin, int end) {
			origin_node = origin;
			begin_index = begin;
			end_index = end;
		}

		bool is_explicit() {
			return begin_index > end_index;
		}

		bool is_implicit() {
			return end_index >= begin_index;
		}

		void canonize()	{
			if (!is_explicit())
			{
				suffix_tree * tree = get_tree();
				suffix_tree_edge * edge = origin_node->find_edge(tree->get_char(begin_index));	
				int edge_span = edge->get_span();
				while(edge_span <= get_span())
				{
					begin_index += edge_span + 1;
					origin_node = edge->end_node;
					if (begin_index <= end_index)
					{
						edge = edge->end_node->find_edge(tree->get_char(begin_index));
						edge_span = edge->get_span();
					}
				}
			}
		}

		int get_span() {
			return end_index - begin_index;
		}

		void inc_begin_index() {
			++begin_index;
		}

		void change_origin_node() {
			origin_node = origin_node->suffix_link;
		}

		void inc_end_index() {
			++end_index;
		}
	};

	std::string text;
	suffix_tree_node * root;
	size_t delimiters_count;
	suffix_tree_suffix active;

	suffix_tree_node * last_node_to_delete;
	suffix_tree_edge * last_edge_to_delete;

	void add_prefix(suffix_tree_suffix * active, int end_index)	{
		suffix_tree_node * last_parent_node = NULL;
		suffix_tree_node * parent_node;

		while(true)	{
			suffix_tree_edge * edge;
			parent_node = active->origin_node;

			if (active->is_explicit()) {
				char end_char = text[end_index];
				edge = active->origin_node->find_edge(end_char);
				if (edge != NULL) {
					break;
				}
			} else {
				char actve_begin_char = text[active->begin_index];
				edge = active->origin_node->find_edge(actve_begin_char);
				int span = active->get_span();
				if (text[edge->begin_index + span + 1] == text[end_index])
				{
					break;
				}
				parent_node = edge->split_edge(active);
			}

			suffix_tree_edge * new_edge = new suffix_tree_edge(end_index, INF, parent_node);
			update_last_edge(new_edge);
			new_edge->insert();
			update_suffix_link(last_parent_node, parent_node);
			last_parent_node = parent_node;

			if (active->origin_node == root) {
				active->inc_begin_index();
			} else {
				active->change_origin_node();
			}
			active->canonize();
		}
		update_suffix_link(last_parent_node, parent_node);
		active->inc_end_index();
		active->canonize();
	}

	void update_suffix_link(suffix_tree_node * node, suffix_tree_node * suffix_link) {
		if (node != NULL && node != root) {
			node->suffix_link = suffix_link;
		}
	}

	void add_delimiter() {
		delimiters_count += 1;
		char delimiter = (char) 'Z' + delimiters_count;
		add_character(delimiter);
	}

	void delete_nodes() {
		while(last_node_to_delete != NULL) {
			suffix_tree_node * temp = last_node_to_delete->next_to_delete;
			delete last_node_to_delete;
			last_node_to_delete = temp;
		}
	}

	void delete_edges() {
		while(last_edge_to_delete != NULL) {
			suffix_tree_edge * temp = last_edge_to_delete->next_to_delete;
			delete last_edge_to_delete;
			last_edge_to_delete = temp;
		}
	}

public:
	suffix_tree() :
		root(new suffix_tree_node(NULL, this)),
		active(root, 0, -1),
		last_node_to_delete(NULL),
		last_edge_to_delete(NULL)
	{
		update_last_node(root);
	}

	void add_string(const std::string & string)	{
		for(auto it = string.begin(); it != string.end(); ++it)	{
			add_character(*it);
		}
		add_delimiter();
	}

	void add_character(char character) {
		text.push_back(character);
		add_prefix(&active, text.length() - 1);
	}

	char get_char(size_t index) {
		return text[index];
	}

	void update_last_node(suffix_tree_node * new_last) {
		new_last->next_to_delete = last_node_to_delete;
		last_node_to_delete = new_last;
	}

	void update_last_edge(suffix_tree_edge * new_last) {
		new_last->next_to_delete = last_edge_to_delete;
		last_edge_to_delete = new_last;
	}

	std::string get_lcs(size_t string1_length, size_t string2_length) {
		search_result result = root->find_delimiters(string1_length, string1_length + string2_length + 1, 0);
		if (result.depth == 0) {
			return std::string();
		} else {
			return text.substr(result.end_index - result.depth + 1, result.depth);
		}
	}

	~suffix_tree() {
		delete_nodes();
		delete_edges();
	}
};


int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	std::string s;
	std::cin >> s;
	std::string reverseString = s;
	std::reverse(reverseString.begin(), reverseString.end());
	suffix_tree tree;
	tree.add_string(s);
	tree.add_string(reverseString);
	std::cout <<tree.get_lcs(s.length(), reverseString.length());
	return 0;
}
