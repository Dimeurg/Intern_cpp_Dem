#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "Point.h"
#include <random>
#include <algorithm> 
#include <mutex>
#include <thread>
#include <functional>

class node{
	public:
		std::vector<Point> transitions;
		float weights;
		
		node(float _weights): weights(_weights){}
		node(){weights = 0.0f;}
		~node(){transitions.clear();}
};

typedef std::map<Point, node> Nodes;
typedef std::vector<std::string> Graf;

class ant{
	public:
	
	Point current_node;
	std::vector<Point> change_nodes;
	void move(Nodes & _nodes, Point _start_point, Point _end_point);
	bool is_searching;
	float pheromone;

	ant():current_node(Point(-1,-1)), is_searching(true) {}
	ant(Point _start_point):current_node(_start_point), is_searching(true){}
	~ant() {change_nodes.clear();}

	private:
		void move_search(const Nodes & _nodes, Point _start_point, Point _end_point);
		void ant::move_back(Nodes & _nodes);
};


void ant::move_search(const Nodes & _nodes, Point _start_point, Point _end_point){
	if(current_node == Point(-1,-1))
		current_node = _start_point;
	if(current_node == _start_point)
		change_nodes.push_back(current_node);

	std::vector<Point> next_nodes = _nodes.at(current_node).transitions; 
	Point next_node;
	float random = 0;
	for(size_t i = 0; i < next_nodes.size(); ++i){
		random += _nodes.at(next_nodes[i]).weights;
	}
	std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, random);
	float result_random = float(dist(e2));

	for(size_t i = 0; i < next_nodes.size(); ++i){
		float weight = _nodes.at(next_nodes[i]).weights;
		if(result_random < weight){
			next_node = next_nodes[i];
			break;
		}
		else
			result_random -= weight;
	}

	change_nodes.push_back(next_node);
	current_node = next_node;
	if(current_node == _end_point){
		is_searching = false;
		pheromone = 1;
	}
}

void ant::move_back(Nodes & _nodes){
	float k = 0.9f;
	int current_index = int(change_nodes.size() - 1);
	if(current_index >= 0){
		current_node = change_nodes[current_index];
		if(current_index != 0){
			Point p1 = change_nodes[current_index];
			pheromone *= k;

			std::mutex mutex;
			std::lock_guard<std::mutex> lock(mutex);

			float new_weight = std::max(_nodes.at(p1).weights, pheromone);
			_nodes[current_node].weights = new_weight;
			current_index--;
		}

		change_nodes.pop_back();
	}
	else{
		is_searching = true;
	}

}

void ant::move(Nodes & _nodes, Point _start_point, Point _end_point){
	if(is_searching){
		move_search(_nodes, _start_point, _end_point);
	}
	else{
		ant::move_back(_nodes);
	}
}

class Pole{
	public:
		Nodes nodes;
		std::vector<ant> ants;
		Point start, end;
		Point sizes;
		
		Pole():sizes(0,0){}
		~Pole(){ants.clear(); nodes.clear();}
};

using namespace std;

bool operator <(const cv::Point & _p1, const cv::Point & _p2) 
	{
		if(_p1.y < _p2.y)
			return true;
		if(_p1.y > _p2.y)
			return false;
		return (_p1.x < _p2.x);
	}
	
void find_transitions_help(Point _p, Graf& _graf, node & _node){
	size_t size_y = _graf.size();
	size_t size_x;
		//1-st line
		if((_p.y - 1) >= 0){
			size_x = _graf[_p.y-1].size();
			
			//1-st element
			if((_p.x - 1) >= 0 && (_p.x - 1) < size_x){
				if(_graf[_p.y-1][_p.x-1] == '\\' || _graf[_p.y-1][_p.x-1] == 'x'){
					_node.transitions.push_back(Point(_p.x - 2, _p.y -2));
				}
			}
			if(_p.x < size_x){
				//2-nd element
				if(_graf[_p.y-1][_p.x] == '|')
					_node.transitions.push_back(Point(_p.x, _p.y -2));
			
				//3-rd element
				if((_p.x + 1) < size_x){
					if(_graf[_p.y-1][_p.x+1] == '/' || _graf[_p.y-1][_p.x+1] == 'x'){
						_node.transitions.push_back(Point(_p.x + 2, _p.y - 2));
					}
				}
			}
		}
		//2-nd line
		{
			size_x = _graf[_p.y].size();
			if(_p.x < size_x){
				//1-st element
				if((_p.x - 1) >= 0)
					if(_graf[_p.y][_p.x-1] == '-')
						_node.transitions.push_back(Point(_p.x - 2, _p.y));
					
				//2-nd element
				if((_p.x + 1) < size_x)
					if(_graf[_p.y][_p.x + 1] == '-')
						_node.transitions.push_back(Point(_p.x + 2, _p.y));
			}
		}		
		
		//3-rd line
		if((_p.y + 1) < size_y){
			
			size_x = _graf[_p.y+1].size();
			//1-st element
			if((_p.x - 1) >= 0 && (_p.x - 1) < size_x)
				if(_graf[_p.y+1][_p.x-1] == '/' || _graf[_p.y+1][_p.x-1] == 'x')
					_node.transitions.push_back(Point(_p.x - 2, _p.y + 2));
	
			if(_p.x < size_x){
				//2-nd element
				if(_graf[_p.y+1][_p.x] == '|')
					_node.transitions.push_back(Point(_p.x, _p.y +2));
			
				//3-rd element
				if((_p.x + 1) < size_x)
					if(_graf[_p.y+1][_p.x+1] == '\\' || _graf[_p.y+1][_p.x+1] == 'x')
						_node.transitions.push_back(Point(_p.x + 2, _p.y + 2));
			}
		}
		
		
}

void find_transitions(Graf& _graf, Nodes & _nodes){
		for(auto& item : _nodes){
			find_transitions_help(item.first, _graf, item.second);
		}
}

void find_weights(Graf& _graf, Pole & _pole){
	for(int i = 0; i < _graf.size(); ++i){
		for(int j = 0; j < _graf[i].size(); ++j){
			switch(_graf[i][j]){
				case 's':
				_pole.nodes[Point(j,i)].weights = 0.0f;
				_pole.start = Point(j,i);
				break;
				
				case 'o':
				_pole.nodes[Point(j,i)].weights = 0.1f;
				break;
				
				case 'e':
				_pole.nodes[Point(j,i)].weights = 1.0f;
				_pole.end = Point(j,i);
				break;
			}
		}
	}
}

void file_read(std::string _filename, Graf & _graf, Point & _p){
	ifstream input(_filename);
	input >> _p.x >> _p.y;
	
	char buf[50];
	input.getline(buf, 50);
	
	while(!input.eof()){
		input.getline(buf, 50);
		_graf.push_back(buf);
	}
}

void draf_graf(Nodes & _nodes, cv::Mat & _img, Point _p_size){
	int step = 60;
	int start_x = 30, start_y = 45;

	for(int i = 0; i < _p_size.y; ++i){
		for(int j = 0; j < _p_size.x; ++j){
			
			if(_nodes.count(Point(j,i))){
				node n = _nodes[Point(j,i)];
				int x = step * j + start_x + step/2;
				int y = step * i + start_y + step/2;
				
				float red = 255 * n.weights * 5 ;
				cv::Scalar color(0,0,red);
				circle(_img, cv::Point(x,y),step/2, color, 8);

				char buf[20];
				sprintf_s(buf, "%.2f", n.weights);				
				cv::putText(_img, buf, cv::Point(x - step/2,y), 0, 1, cv::Scalar(112, 25, 25), 2);

				for(size_t k = 0; k < n.transitions.size();++k){
					int x2 = x + step*(n.transitions[k].x - j);
					int y2 = y + step*(n.transitions[k].y - i);

					cv::line(_img, cv::Point(x, y), cv::Point(x2,y2), cv::Scalar(255,196,0), 4);
				}
			}
		}
	}
}

void draf_ants(std::vector<ant> & _ants, cv::Mat & _img){
	int step = 60;
	int start_x = 30, start_y = 45;

	cv::Scalar color(255,255,255);
	for(auto& item: _ants){
		Point p = item.current_node;
		int x = step * p.x + start_x + step/2;
		int y = step * p.y + start_y + step/2;
		circle(_img, cv::Point(x,y),5, color, 2);
	}
}

void draf_pole(Pole & _pole, cv::Mat & _img)
{
	draf_graf(_pole.nodes, _img, _pole.sizes);
	draf_ants(_pole.ants, _img);
}

void move_2_ants(Pole & _pole, int _pos){
	_pole.ants[_pos].move(_pole.nodes, _pole.start, _pole.end);
	_pole.ants[_pos+1].move(_pole.nodes, _pole.start, _pole.end);
}

int main(int argc,char** argv)
{
	Pole pole;
	std::vector<std::string> graf;

	file_read("graf.txt", graf, pole.sizes);
	
	for(int i = 0; i < graf.size(); ++i)
		std::cout << graf[i] << std::endl;
	
	find_weights(graf, pole);
	find_transitions(graf, pole.nodes);
	
	ant prototype_ant(pole.start);
	for(int i = 0; i < 40; i++){
		pole.ants.push_back(prototype_ant);
	}

	cv::Mat img_bg;
	img_bg = cv::imread("bg.jpg");

	std::vector<std::thread> threads;
	while(true){
		
		for(size_t i = 0; i < pole.ants.size(); i+=2){
			threads.push_back(std::thread(move_2_ants, std::ref(pole), i));
		}

		for(size_t i = 0; i < threads.size(); i++){
			threads[i].join();
		}
		threads.clear();

		float ki = 0.9f;
		for(auto& item: pole.nodes){
			Point p = item.first;
			if(p != pole.start && p != pole.end){
				float new_weight = item.second.weights * ki;
				item.second.weights = std::max(0.1f, new_weight);
			}
		}

		img_bg = cv::imread("bg.jpg");
		draf_pole(pole, img_bg);
		cv::imshow("win",img_bg);
		cv::waitKey(50);
	}

	return 0;
}