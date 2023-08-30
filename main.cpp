//
//  main.cpp
//  proj4
//
//  Created by Kai Yamasaki on 4/7/23.
// IDENTIFIER  = 5949F553E20B650AB0FB2266D3C0822B13D248B0
#include <iomanip>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <cstdio>
#include <limits>
#include <queue>
#include <cmath>
#include <cstring>
using namespace std;



inline void xcode_redirect(int argc, char **argv, int optind = 1) {
    while (optind < argc) {
        switch (*argv[optind]) {
            case '<':  // redirect input
                if (!freopen(argv[++optind], "r", stdin)) {
                    std::cerr << "Unable to open input file: " << argv[optind] <<
std::endl;
                    exit(1);
                }
                break;
            case '>':  // redirect output
                if (!freopen(argv[++optind], "w", stdout)) {
                    std::cerr << "Unable to open output file: " << argv[optind] <<
std::endl;
                    exit(1);
                }
                break;
        }  // switch
        ++optind;
    }  // while
}  // xcode_redirect()

void getMode(int argc, char * argv[], bool &command) {
  // These are used with getopt_long()
  opterr = false; // Let us handle all error output for command line options
  int choice;
  int index = 0;
  option long_options[] = {
    // DONE: Fill in two lines, for the "mode" ('m') and
    // the "help" ('h') options.
    // ./project0 --mode nosize
    // ./project0 --help          //third argument always nullptr, cuz feature not used
      { "mode", no_argument, nullptr, 'm' } ,
      { "help", no_argument, nullptr, 'h'} ,
    { nullptr, 0, nullptr, '\0' },
  };  // long_options[]

  // TODO: Fill in the double quotes, to match the mode and help options.
  while ((choice = getopt_long(argc, argv, "mh", long_options, &index)) != -1) {
      // : means m is followed by an argument, while h has no argument
    switch (choice) {
        case 'm': {  // Need a block here to declare a variable inside a case
            command = true;
            break;
        }
        case 'h': {
            cout << "HELP: Print a short description of this program and its arguments\n";
            exit(0);
            break;
        }
      //default:
        //cerr << "Error: invalid option" << endl;
        //exit(1);
    }  // switch ..choice
  }  // while
}



class Node {
public:
    Node() {}
    Node(uint32_t num, int32_t x, int32_t y) : number(num), included(0), xCoor(x), yCoor(y) {
        parent = -1;
        terrain = getTerrain();
    }
    Node(uint32_t num, int32_t x, int32_t y, int32_t par) : number(num), included(0), xCoor(x), yCoor(y) {
        parent = par;
        terrain = getTerrain();
    }
    uint32_t getTerrain() {
        if (xCoor < 0 && yCoor < 0) {
            //any axis
            return 3;
        }
        else if (xCoor == 0 && yCoor <= 0) {
            return 2;
        }
        else if (xCoor <= 0 && yCoor == 0) {
            return 2;
        }
        else {
            return 1;
        }
        /*
        if (xCoor == 0 || yCoor == 0) {
            //any axis
            return 2;
        }
        else if (xCoor > 0 && yCoor > 0) {
            //first quadrant
            return 1;
        }
        else if (xCoor > 0 && yCoor < 0) {
            //4th quadrant
            return 1;
        }
        else if (xCoor < 0 && yCoor > 0) {
            //second quadrant
            return 1;
        }
        else if (xCoor < 0 && yCoor < 0) {
            //third quardrant aka sea
            return 3;
        }
        else {
            cerr << "ERROR: COULDN'T ACCURATELY DEFINE QUADRANT/TERRAIN\n";
            exit(1);
        }
         */
    }
    
    void printCoordinates() {
        cout << xCoor << " " << yCoor << endl;
    }
    
    bool isFree() {
        if (included > 0) {
            return false;
        }
        return true;
    }
    
    void addIncluded() {
        included = included + 1;
    }
    
    int32_t getXCopy() const {
        return xCoor;
    }
    
    int32_t getYCopy() const {
        return yCoor;
    }
    
    uint32_t getNum() const {
        return number;
    }
    
private:
    int32_t parent;
    uint32_t number;
    uint32_t included;
    int32_t xCoor;
    int32_t yCoor;
    uint32_t terrain;
    //terrain breakdown
    //1 for land
    //2 for coast
    //3 for sea
};

//RETURNS THE UNSQUARED DISTANCE FOR PART A ONLY
double AcalcDistance(Node &one, Node &two) {
    if (one.getTerrain() == 3 && two.getTerrain() == 1) {
        return numeric_limits<double>::infinity();
    }
    else if (one.getTerrain() == 1 && two.getTerrain() == 3) {
        return numeric_limits<double>::infinity();
    }
    else {
        double xdify = static_cast<double>(one.getXCopy()) - static_cast<double>(two.getXCopy());
        double ydify = static_cast<double>(one.getYCopy()) - static_cast<double>(two.getYCopy());
        xdify = xdify * xdify;
        ydify = ydify * ydify;
        return (xdify + ydify);
    }
}

double BcalcDistance(Node &one, Node &two) {
    double xdify = static_cast<double>(one.getXCopy()) - static_cast<double>(two.getXCopy());
    double ydify = static_cast<double>(one.getYCopy()) - static_cast<double>(two.getYCopy());
    xdify = xdify * xdify;
    ydify = ydify * ydify;
    return (xdify + ydify);
    
}


class Edge {
public:
    Edge(Node &one, Node &two, bool isMST) : oneLoc(one.getNum()), twoLoc(two.getNum()) {
        if (isMST) {
            distance = AcalcDistance(one, two);
        }
        else {
            distance = BcalcDistance(one, two);
        }
         
    }
    double getDist() const {
        return distance;
    }
    
    void printEdge() {
        cout << oneLoc << " " << twoLoc << " " << distance << endl;
    }
    
    uint32_t getOneLoc() const {
        return oneLoc;
    }
    
    uint32_t getTwoLoc() const {
        return twoLoc;
    }
    
private:
    uint32_t oneLoc; //should always be the one already included
    uint32_t twoLoc; //should always be the one not yet included
    double distance;
};

struct AEdgeDistComparator {
    bool operator()(Edge& a, Edge& b) const {
            // Change the comparison logic as needed
            // For example, to implement a min priority queue, use "less" instead of "greater"
            return (a.getDist() > b.getDist());
    }
};


/*
void functionMST(vector<Node> &inputs) {
    vector<pair<uint32_t, uint32_t>> indexOrder;
    indexOrder.reserve(inputs.size());
    double weight = 0;
    size_t numIn = 1;
    inputs[0].addIncluded();
   //priority_queue<Edge, vector<Edge>, AEdgeDistComparator> minEdges;
    
    ///switch all In to a counter that shuld be equal to the size of the ivector once done
    ShortestEdge second = {0, 0, numeric_limits<double>::infinity()};
    ShortestEdge meep = {0, 0, numeric_limits<double>::infinity()};
    while (numIn < inputs.size()) {
        meep = second;
        
        ShortestEdge meep = {0, 0, numeric_limits<double>::infinity()};
        
        for (uint32_t i = 0; i < inputs.size(); ++i) {
            if (!inputs[i].isFree()) { //if its inside
                for (uint32_t j = 0; j < inputs.size(); ++j) {
                    
                    if (inputs[j].isFree()) {   //if its outside
                        Edge now(inputs[i], inputs[j], true);
                        if (now.getDist() < meep.disty) {
                            meep.locOne = i;
                            meep.locTwo = j;
                            meep.disty = now.getDist();
                        }
                        //minEdges.push(now);
                    }
                }
            }
        }
         
        if (meep.disty == numeric_limits<double>::infinity()) {
            cerr << "Cannot construct MST\n";
            exit(1);
        }
        else {
            inputs[meep.locTwo].addIncluded();
            numIn++;
            weight = weight + sqrt(meep.disty);
            if (meep.locOne < meep.locTwo) {
                indexOrder.push_back(pair<uint32_t, uint32_t>(meep.locOne, meep.locTwo));
            }
            else {
                indexOrder.push_back(pair<uint32_t, uint32_t>(meep.locTwo, meep.locOne));
            }
        }
    }
    cout << weight << endl;
    for (auto z : indexOrder) {
        cout << z.first << " " << z.second << endl;
    }
}*/
struct PrimData {
    double d;
    int32_t p;
    bool k;
    PrimData() : d{ std::numeric_limits<double>::infinity() }, p{ -1 }, k{ false } {}
};

void functionMST(vector<Node> &inputs) {
    double weight = 0;
    vector<PrimData> tracker;
    tracker.resize(inputs.size());
    vector<uint32_t> order;
    order.reserve(inputs.size());
    tracker[0].d = 0;
    double minSeen = numeric_limits<double>::infinity();
    uint32_t minIndex = numeric_limits<uint32_t>::infinity();
    
    for (uint32_t i = 0; i < inputs.size(); ++i) {
        if (tracker[i].d < minSeen) {
            minSeen = tracker[i].d;
            minIndex = i;
        }
    }
    tracker[minIndex].k = true;
    order.push_back(minIndex);
    
    
    while (order.size() < inputs.size()) {
        for (uint32_t i = 0; i < inputs.size(); ++i) {
            if (tracker[i].k == false) {
                double distCheck = AcalcDistance(inputs[minIndex], inputs[i]);
                if (distCheck < tracker[i].d) {
                    //distance is less than the current closest distance
                    tracker[i].p = static_cast<int32_t>(minIndex);
                    tracker[i].d = distCheck;
                    /*
                    if (tracker[i].d < minSeen)  {
                        nextIndex = i;
                        minSeen = tracker[i].d;
                    }
                     */
                }
            }
        }
        
        minSeen = numeric_limits<double>::infinity();
        for (uint32_t i = 0; i < inputs.size(); ++i) {
            if (!tracker[i].k && tracker[i].d < minSeen) {
                minSeen = tracker[i].d;
                minIndex = i;
            }
        }
        tracker[minIndex].k = true;
        order.push_back(minIndex);
        weight = weight + sqrt(tracker[minIndex].d);
    }
    cout << weight << endl;
    for (uint32_t i = 1; i < order.size(); ++i) {
        if (static_cast<int32_t>(order[i]) > tracker[order[i]].p) {
            cout << tracker[order[i]].p << " " << order[i] << endl;
        }
        else {
            cout << order[i] << " " << tracker[order[i]].p << endl;
        }
    }
}

struct PrimDataB {
    double d;
    bool k;
    PrimDataB() : d{ std::numeric_limits<double>::infinity() }, k{ false } {}
};

void otherFASTTSP(vector<Node> &inputs) {
    double weight = 0;
    vector<PrimDataB> tracker;
    tracker.resize(inputs.size());
    vector<uint32_t> order;
    order.reserve(inputs.size());
    order.push_back(0);
    tracker[0].d = 0;
    tracker[0].k = true;
    double minSeen = numeric_limits<double>::infinity();
    uint32_t minIndex = numeric_limits<uint32_t>::infinity();
    
    for (uint32_t i = 1; i < inputs.size(); ++i) {
        tracker[i].d = BcalcDistance(inputs[0], inputs[i]);
        if (tracker[i].d < minSeen) {
            minSeen = tracker[i].d;
            minIndex = i;
        }
    }
    tracker[minIndex].k = true;
    order.push_back(minIndex);
    weight = weight + (2 * sqrt(tracker[minIndex].d));
    //this should have weight equivalent to 0 1 + 1 0
        uint32_t indexAdd = numeric_limits<uint32_t>::infinity();
        uint32_t spotChange = numeric_limits<uint32_t>::infinity();
        double minCost = numeric_limits<double>::infinity();
        //outer loop all the vertexes
        
        
        
        for (uint32_t a = 1; a < inputs.size(); ++a) {
            minCost = numeric_limits<double>::infinity();
            if (tracker[a].k == false) {
                for (uint32_t b = 0; b < order.size() - 1; ++b) {
                    uint32_t i = order[b];
                    uint32_t j = order[b + 1];
                    double cik = BcalcDistance(inputs[i], inputs[a]);
                    double ckj = BcalcDistance(inputs[a], inputs[j]);
                    double cij = BcalcDistance(inputs[i], inputs[j]);
                    double costChange = sqrt(cik) + sqrt(ckj) - sqrt(cij);
                    if (costChange < minCost) {
                        minCost = costChange;
                        spotChange = b;
                        indexAdd = a;
                    }
                }
                weight = weight - sqrt(BcalcDistance(inputs[order[spotChange]], inputs[order[spotChange + 1]]));
                order.insert(order.begin() + spotChange + 1, indexAdd);
                tracker[indexAdd].k = true;
                weight = weight + sqrt(BcalcDistance(inputs[order[spotChange]], inputs[order[spotChange + 1]]));
                weight = weight + sqrt(BcalcDistance(inputs[order[spotChange + 1]], inputs[order[spotChange + 2]]));
            }
            
        }
                
    
    cout << weight << endl;
    for (uint32_t b = 0; b < order.size(); ++b) {
        cout << order[b] << " ";
    }
    cout << endl;
}

double partCFASTTSP(vector<Node> &inputs, vector<uint32_t> &path) {
    double weight = 0;
    vector<PrimData> tracker;
    tracker.resize(inputs.size());
    vector<uint32_t> order;
    order.reserve(inputs.size());
    order.push_back(0);
    tracker[0].d = 0;
    tracker[0].k = true;
    double minSeen = numeric_limits<double>::infinity();
    uint32_t minIndex = numeric_limits<uint32_t>::infinity();
    
    for (uint32_t i = 1; i < inputs.size(); ++i) {
        tracker[i].d = BcalcDistance(inputs[0], inputs[i]);
        if (tracker[i].d < minSeen) {
            minSeen = tracker[i].d;
            minIndex = i;
        }
    }
    tracker[minIndex].k = true;
    order.push_back(minIndex);
    weight = weight + (2 * sqrt(tracker[minIndex].d));
    //this should have weight equivalent to 0 1 + 1 0
        uint32_t indexAdd = numeric_limits<uint32_t>::infinity();
        uint32_t spotChange = numeric_limits<uint32_t>::infinity();
        double minCost = numeric_limits<double>::infinity();
        //outer loop all the vertexes
        
        
        
        for (uint32_t a = 1; a < inputs.size(); ++a) {
            minCost = numeric_limits<double>::infinity();
            if (tracker[a].k == false) {
                for (uint32_t b = 0; b < order.size() - 1; ++b) {
                    uint32_t i = order[b];
                    uint32_t j = order[b + 1];
                    double cik = BcalcDistance(inputs[i], inputs[a]);
                    double ckj = BcalcDistance(inputs[a], inputs[j]);
                    double cij = BcalcDistance(inputs[i], inputs[j]);
                    double costChange = sqrt(cik) + sqrt(ckj) - sqrt(cij);
                    if (costChange < minCost) {
                        minCost = costChange;
                        spotChange = b;
                        indexAdd = a;
                    }
                }
                weight = weight - sqrt(BcalcDistance(inputs[order[spotChange]], inputs[order[spotChange + 1]]));
                order.insert(order.begin() + spotChange + 1, indexAdd);
                tracker[indexAdd].k = true;
                weight = weight + sqrt(BcalcDistance(inputs[order[spotChange]], inputs[order[spotChange + 1]]));
                weight = weight + sqrt(BcalcDistance(inputs[order[spotChange + 1]], inputs[order[spotChange + 2]]));
            }
            
        }
                
    path = order;
    return weight;
    
}

struct PrimDataC {
    uint32_t number;
    double d;
    int32_t p;
    bool k;
    bool oop;
    PrimDataC() : number(numeric_limits<uint32_t>::infinity()), d{ std::numeric_limits<double>::infinity() }, p{ -1 }, k{ false }, oop(false) {}
};


/*
double partCMST(vector<Node> &inputs, vector<uint32_t> &currentPath) {
    double weight = 0;
    vector<PrimDataC> tracker;
    tracker.resize(inputs.size());
    vector<uint32_t> order;
    order.reserve(inputs.size());
    for (uint32_t i = 0; i < currentPath.size(); ++i) {
        tracker[currentPath[i]].oop = true;
    }
    uint32_t z = 0;
    while (tracker[z].oop) {
        ++z;
    }
    tracker[z].d = 0;
    //tracker[0].d = 0;
    double minSeen = numeric_limits<double>::infinity();
    uint32_t minIndex = numeric_limits<uint32_t>::infinity();
    
    for (uint32_t i = 0; i < inputs.size(); ++i) {
        if (!tracker[i].oop) {
            if (tracker[i].d < minSeen) {
                minSeen = tracker[i].d;
                minIndex = i;
            }
        }
    }
    tracker[minIndex].k = true;
    order.push_back(inputs[minIndex].getNum());
    

    while (order.size() < inputs.size() - currentPath.size()) {
        for (uint32_t i = 0; i < inputs.size(); ++i) {
            if (!tracker[i].oop) {
                if (tracker[i].k == false) {
                    double distCheck = BcalcDistance(inputs[minIndex], inputs[i]);
                    if (distCheck < tracker[i].d) {
                        //distance is less than the current closest distance
                        tracker[i].p = static_cast<int32_t>(minIndex);
                        tracker[i].d = distCheck;
                        
                    }
                }
            }
        }
        
        minSeen = numeric_limits<double>::infinity();
        for (uint32_t i = 0; i < inputs.size(); ++i) {
            if (!tracker[i].oop) {
                if (!tracker[i].k && tracker[i].d < minSeen) {
                    minSeen = tracker[i].d;
                    minIndex = i;
                }
            }
        }
        tracker[minIndex].k = true;
        order.push_back(minIndex);
        //uint32_t aba = 2;
        //order.push_back(aba);
        weight = weight + sqrt(tracker[minIndex].d);
        
        
        
    }
    
    cout << weight << endl;
    for (uint32_t i = 1; i < order.size(); ++i) {
        if (static_cast<int32_t>(order[i]) > tracker[order[i]].p) {
            cout << tracker[order[i]].p << " " << order[i] << endl;
        }
        else {
            cout << order[i] << " " << tracker[order[i]].p << endl;
        }
    }
     
    //return weight;
    
    double minWeight = numeric_limits<double>::infinity();
    uint32_t bestVertex = 0;
    for (uint32_t i = 0; i < inputs.size(); ++i) {
        if (!tracker[i].oop) {
            double weight = sqrt(BcalcDistance(inputs[currentPath[0]], inputs[i]));
            weight = weight + sqrt(BcalcDistance(inputs[currentPath[currentPath.size()] - 1], inputs[i]));
            if (weight < minWeight) {
                bestVertex = i;
                minWeight = weight;
            }
    
        }
    }
    pair<uint32_t, double> vertexTotalWeight;
    vertexTotalWeight.first = bestVertex;
    vertexTotalWeight.second = minWeight + weight;
    
    return weight;
}
    */
/*
bool greaterComparator(uint32_t a, uint32_t b) {
    return a > b;
}
 */


class partC {
public:
    partC();
    partC(vector<Node> &in) : input(in), currentDistance(0) {
        
        
        
        bestDistance = partCFASTTSP(in, bestPathSeen);
        currentPath = bestPathSeen;
        
        
        /*
        cout << "Best path length = ";
        cout << bestDistance << endl;
        for (uint32_t i = 0; i < bestPathSeen.size(); ++i) {
            cout << bestPathSeen[i] << " ";
        }
        cout << endl;
         */
        
        
        currentPath.resize(input.size());
        for (uint32_t i = 0; i < input.size(); ++i) {
            currentPath[i] = input[i].getNum();
        }
        size_t permLength = 1;
        genPerms(permLength);
        
        //cout << endl << endl << endl;
        cout << bestDistance << endl;
        for (uint32_t i = 0; i < currentPath.size(); ++i) {
            cout << bestPathSeen[i] << " ";
        }
        cout << endl;
        
    }
    
    void genPerms(size_t permLength) {
            
            
        
        if (permLength == currentPath.size()) {
            double addition = sqrt(BcalcDistance(input[currentPath[0]], input[currentPath[permLength - 1]]));
            //currentDistance = currentDistance + sqrt(BcalcDistance(input[currentPath[0]], input[currentPath[permLength - 1]]));

            if (currentDistance + addition < bestDistance) {
               
                //cout << currentDistance << endl << endl << endl;
                
                for (size_t i = 0; i < permLength; ++i) {
                    bestPathSeen[i] = currentPath[i];
                }
                bestDistance = currentDistance + addition;
                //cout << "New best cost achieved: " << bestDistance << endl;
            }
            //currentDistance = currentDistance -  sqrt(BcalcDistance(input[currentPath[0]], input[currentPath[permLength - 1]]));
            return;
        }
        if (!isPromising(permLength)) {
            ///--permLength;
            /*currentDistance = currentDistance - sqrt(BcalcDistance(input[currentPath[permLength]], input[currentPath[permLength - 1]])); */
            return;
        }
        
        //cout << endl;
        for (size_t i = permLength; i < input.size(); ++i) {
            swap(currentPath[permLength], currentPath[i]);
            double addition = sqrt(BcalcDistance(input[currentPath[permLength]], input[currentPath[permLength - 1]]));

            currentDistance = currentDistance + addition;
            genPerms(permLength + 1);

            currentDistance = currentDistance - addition;
            swap(currentPath[permLength], currentPath[i]);
        }
    }
    
    bool isPromising(size_t &permLength) {
    
    /*
            double smallestMST = partCMST(permLength);
            if (currentDistance + smallestMST < bestDistance) {
                return true;
            }
            else {
                return false;
            }
            return true;
        */
        if (currentPath.size() - permLength < 5)  {
            return true;
        }
        else {
            double weights = partCMST(permLength);
            bool promise = (currentDistance + weights < bestDistance);
            return promise;
        }
        
        
        
        //double weights = partCMST(permLength);
        //bool promise = (currentDistance + weights < bestDistance);
        
        /*
        if (promise && permLength == currentPath.size() - 1) {
            bestDistance = currentDistance + weights[0] + weights[1] + weights[2];
            for (size_t i = 0; i < currentPath.size(); ++i) {
                bestPathSeen[i] = currentPath[i];
            }
         cout << "New best cost achieved: " << bestDistance;
        }
         */
        /*
        for (size_t i = 0; i < currentPath.size(); ++i) {
            cout << setw(2) << currentPath[i] << ' ';
        }
        double total = currentDistance + weights[0] + weights[1] + weights[2];
        cout << setw(4) << permLength << setw(10) << currentDistance;
        cout << setw(10) << weights[1] << setw(10) << weights[2];
        cout << setw(10) << weights[0] << setw(10) << total << "  " << promise << '\n';
         
         */
        
        //return promise;
    
    }
    
    
    vector<PrimDataC> getTracker(size_t permLength) {
        vector<PrimDataC> tracker;
        tracker.resize(input.size() - permLength);
       
        //vector<uint32_t> copy(currentPath.begin(), currentPath.begin() + permLength);
        //sort(copy.begin(), copy.end());
        vector<uint32_t> copy;
        copy.resize(permLength);
        for (uint32_t i = 0; i < permLength; ++i) {
            copy[i] = currentPath[i];
        }
        sort(copy.begin(), copy.end());
        uint32_t count = 0;
        uint32_t past = 0;
        for (uint32_t i = 0; i < tracker.size(); ++i) {
            while (count < copy.size() && (i + past) == copy[count]) {
                ++count;
                ++past;
            }
            tracker[i].number = i + past;
        }
        return tracker;
    }
    
    double partCMST(size_t permLength) {
        /*
        if (currentDistance > 115.0560138 && currentDistance < 115.1 && permLength == 3) {
            
            cout << "LOOKING HERE" << endl;
        }
         */
        double weight = 0;
        vector<PrimDataC> tracker = getTracker(permLength);
        vector<uint32_t> order;
        order.reserve(input.size() - permLength);
        /*
        tracker.resize(input.size() - permLength);
        vector<uint32_t> order;
        order.reserve(input.size() - permLength);
        vector<uint32_t> copy(currentPath.begin(), currentPath.begin() + permLength);
        sort(copy.begin(), copy.end());
        //copy.push_back(1);
        uint32_t count = 0;
        uint32_t past = 0;
        for (uint32_t i = 0; i + past < input.size(); ++i) {
            while (count < copy.size() && (i + past) == copy[count]) {
                ++count;
                ++past;
            }
            tracker[i].number = i + past;
        }
         */
    
        tracker[0].d = 0;
        /*
        double minSeen = numeric_limits<double>::infinity();
        uint32_t minIndex = numeric_limits<uint32_t>::infinity();
        
        for (uint32_t i = 0; i < tracker.size(); ++i) {
                if (tracker[i].d < minSeen) {
                    minSeen = tracker[i].d;
                    minIndex = i;
                }
        }
         */
        //tracker[minIndex].k = true;
        uint32_t minIndex = 0;
        tracker[minIndex].k = true;
        order.push_back(tracker[minIndex].number);

        
        //if (tracker.size() != 1) {
            while (order.size() != tracker.size()) {
                for (uint32_t i = 0; i < tracker.size(); ++i) {
                        if (tracker[i].k == false) {
                            double distCheck = BcalcDistance(input[tracker[minIndex].number], input[tracker[i].number]);
                            if (distCheck < tracker[i].d) {
                                //distance is less than the current closest distance
                                tracker[i].p = static_cast<int32_t>(minIndex);
                                tracker[i].d = distCheck;
                            }
                        }
                }
                
                double minSeen = numeric_limits<double>::infinity();
                for (uint32_t i = 0; i < tracker.size(); ++i) {
                        if (!tracker[i].k && tracker[i].d < minSeen) {
                            minSeen = tracker[i].d;
                            minIndex = i;
                        }
                }
                tracker[minIndex].k = true;
                order.push_back(tracker[minIndex].number);
                //uint32_t aba = 2;
                //order.push_back(aba);
                weight = weight + sqrt(tracker[minIndex].d);
                
                
                
            }
        //}

        
        double minWeightOne = numeric_limits<double>::infinity();
        double minWeightTwo = numeric_limits<double>::infinity();
        //uint32_t bestVertex = 0;
        for (uint32_t i = 0; i < tracker.size(); ++i) {
            //if (!tracker[i].oop) {
            double vertWeight = sqrt(BcalcDistance(input[currentPath[0]], input[tracker[i].number]));
            double secondWeight = sqrt(BcalcDistance(input[currentPath[permLength - 1]], input[tracker[i].number]));
                if (vertWeight < minWeightOne) {
                   // bestVertex = tracker[i].number;
                    minWeightOne = vertWeight;
                }
            if (secondWeight < minWeightTwo) {
                //bestVertex = tracker[i].number;
                minWeightTwo = secondWeight;
            }
        
            //}
        }
        /*
        double minWeightTwo = numeric_limits<double>::infinity();
        for (uint32_t i = 0; i < tracker.size(); ++i) {
            double secondWeight = sqrt(BcalcDistance(input[currentPath[permLength - 1]], input[tracker[i].number]));
            if (secondWeight < minWeightTwo) {
                //bestVertex = tracker[i].number;
                minWeightTwo = secondWeight;
            }
        }
         */
        /*
        vector<double> weights;
        weights.push_back(weight);
        weights.push_back(minWeightOne);
        weights.push_back(minWeightTwo);
        return weights;
        */
         
        
        
        return (weight + minWeightOne + minWeightTwo);
    }
    
private:
    vector<Node> input;
    vector<uint32_t> currentPath;
    double currentDistance;
    vector<uint32_t> bestPathSeen;
    double bestDistance;
};

void functionOPTTSP(vector<Node> &input) {
    partC instance(input);
}



void readInput(vector<Node> &inputs) {
    uint32_t numNodes;
    cin >> numNodes;
    inputs.reserve(numNodes);
    int32_t x;
    int32_t y;
    for (uint32_t i = 0; i < numNodes; ++i) {
        cin >> x >> y;
        Node now(i, x, y);
        inputs.push_back(now);
    }
}


int main(int argc, char * argv[]) {
    cout << std::boolalpha;
    cout << setprecision(2); //Always show 2 decimal places
    cout << fixed; //Disable scientific notation for large numbers
    bool isMode = false;
    getMode(argc, argv, isMode);
    xcode_redirect(argc, argv);
    uint32_t x = 0;
    uint32_t typie = 0;
    //typie meaning
    // 1 is MST
    // 2 is FASTTSP
    // 3 is OPTTSP
    if (isMode) {
        while (strcmp(argv[x], "-m") != 0 && strcmp(argv[x], "--mode") != 0) {
            ++x;
        }
        ++x;
        //cout << argv[x] << endl;
        if (strcmp(argv[x], "MST") == 0) {
            typie = 1;
        }
        else if (strcmp(argv[x], "FASTTSP") == 0) {
            typie = 2;
        }
        else if (strcmp(argv[x], "OPTTSP") == 0) {
            typie = 3;
        }
    }
    vector<Node> inputs;
    readInput(inputs);
    /*
    for (int z = 0; z < inputs.size(); ++z) {
        inputs[z].printCoordinates();
    }
    cout << inputs.size() << endl;
     */
    
    
    if (typie == 1) {
        //MST
        functionMST(inputs);
    }
    else if (typie == 2) {
        //functionFASTTSP(inputs);
        otherFASTTSP(inputs);
    }
    else if (typie == 3) {
        //functionOPTTSP(inputs);
        /*
        cout << "TESTING PARTCMST: " << endl;
        inputs[0].addIncluded();
        size_t perm = 1;
        vector<uint32_t> currentPath;
        currentPath.push_back(4);
        double weight = partCMST(inputs, currentPath);
         */
        functionOPTTSP(inputs);
        
        
    }
    
    
    
    
   
    return 0;
}


/*
 size_t bestVertex(size_t &permLength, vector<uint32_t> &pathCopy) {
     double minDist = numeric_limits<double>::infinity();
     size_t bestLoc = numeric_limits<uint32_t>::infinity();
     //weight between the first and last node of currentPth & vertex in RandomMST
     for (size_t i  = 0; i < pathCopy.size(); ++i) {
         double weight = sqrt(BcalcDistance(input[currentPath[0]], input[pathCopy[i]]));
         weight = weight + sqrt(BcalcDistance(input[currentPath[permLength - 1]], input[pathCopy[i]]));
         if (weight < minDist) {
             minDist = weight;
             bestLoc = i;
         }
     }
     return bestLoc;
 }
 
 
 */

/*
double partCMST(size_t &permLength) {
    double weight = 0;
    vector<PrimDataC> tracker;
    tracker.resize(input.size());
    vector<uint32_t> order;
    order.reserve(input.size());
    for (uint32_t i = 0; i < permLength; ++i) {
        tracker[currentPath[i]].oop = true;
    }
    uint32_t z = 0;
    while (tracker[z].oop) {
        ++z;
    }
    tracker[z].d = 0;
    //tracker[0].d = 0;
    double minSeen = numeric_limits<double>::infinity();
    uint32_t minIndex = numeric_limits<uint32_t>::infinity();
    
    for (uint32_t i = 0; i < input.size(); ++i) {
        if (!tracker[i].oop) {
            if (tracker[i].d < minSeen) {
                minSeen = tracker[i].d;
                minIndex = i;
            }
        }
    }
    tracker[minIndex].k = true;
    order.push_back(input[minIndex].getNum());
    

    while (order.size() < input.size() - permLength) {
        for (uint32_t i = 0; i < input.size(); ++i) {
            if (!tracker[i].oop) {
                if (tracker[i].k == false) {
                    double distCheck = BcalcDistance(input[minIndex], input[i]);
                    if (distCheck < tracker[i].d) {
                        //distance is less than the current closest distance
                        tracker[i].p = static_cast<int32_t>(minIndex);
                        tracker[i].d = distCheck;
                    }
                }
            }
        }
        
        minSeen = numeric_limits<double>::infinity();
        for (uint32_t i = 0; i < input.size(); ++i) {
            if (!tracker[i].oop) {
                if (!tracker[i].k && tracker[i].d < minSeen) {
                    minSeen = tracker[i].d;
                    minIndex = i;
                }
            }
        }
        tracker[minIndex].k = true;
        order.push_back(minIndex);
        //uint32_t aba = 2;
        //order.push_back(aba);
        weight = weight + sqrt(tracker[minIndex].d);
        
        
        
    }

    
    double minWeight = numeric_limits<double>::infinity();
    uint32_t bestVertex = 0;
    for (uint32_t i = 0; i < input.size(); ++i) {
        if (!tracker[i].oop) {
            double weight = sqrt(BcalcDistance(input[currentPath[0]], input[i]));
            weight = weight + sqrt(BcalcDistance(input[currentPath[permLength - 1]], input[i]));
            if (weight < minWeight) {
                bestVertex = i;
                minWeight = weight;
            }
    
        }
    }
    pair<uint32_t, double> vertexTotalWeight;
    vertexTotalWeight.first = bestVertex;
    vertexTotalWeight.second = minWeight + weight;
    
    if (minWeight == numeric_limits<double>::infinity()) {
        return (sqrt(BcalcDistance(input[currentPath[0]], input[currentPath[permLength - 1]])));
    }
    return (weight + minWeight);
}
*/
