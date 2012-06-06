#ifndef simplesim_h

struct simpletraffic {
	unsigned int src;
	unsigned int dest
}

class simplesim {
private:
	Graph g;
	simpletraffic* traffic;
	unsigned int traffic_count;
	void update_weight();
	int count = 0;
	
public:
	void next();
	void next(int n);
	

}
#endif
