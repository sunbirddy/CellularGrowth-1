//
//  simulation.h
//  CellularGrowth
//
//  Created by Sage Jenson on 9/30/16.
//

#ifndef simulation_h
#define simulation_h

#include <vector>
#include <deque>
#include <set>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <tuple>
#include "utils.h"
#include "cell.h"
#include "grid.h"

enum food_mode_enum {CONSTANT, BREADTH, DENSITY, X_AXIS_DENSITY,
    PLANAR, FACE, RD};

class Face {
public:
    Cell *a, *b, *c;
    Vec3f normal;
    float area;
    
    Face(Cell* _a, Cell* _b, Cell* _c){
        a = _a;
        b = _b;
        c = _c;
        
        normal = (a->position-b->position).cross(c->position-b->position);
        area = 0.5 * normal.length();
        normal.normalize();
    }
    
    bool is_equal(const Face& other) const {
        return  ((a==other.a) and (b==other.b) and (c==other.c)) or
                ((b==other.a) and (c==other.b) and (a==other.c)) or
                ((c==other.a) and (a==other.b) and (b==other.c)) or
                ((a==other.a) and (c==other.b) and (b==other.c)) or
                ((b==other.a) and (a==other.b) and (c==other.c)) or
                ((c==other.a) and (b==other.b) and (a==other.c));
    }
    
    bool operator< (const Face & other) const {
        if (is_equal(other)){
            return true;
        } else {
            return area*10000.0 + (normal.x+normal.y) < other.area*100000.0 + (other.normal.x+other.normal.y);
        }
    }
};

class Simulation{
public:
    
    Simulation(float _roi_squared, float _spring_factor,
               float _bulge_factor, float _planar_factor,
               float _repulsion_strength, float _link_rest_length,
               int _food_mode, float _split_threshold);
    ~Simulation(void);
    
    void initialize(void);
    
    std::vector<Cell*>* get_cells(void);
    
    void update(void);
    int get_population(void);
    int get_frame_num(void);
    void set_split_threshold(float _split_threshold);
    int get_size(void);
    void point_list(void);
    void reset(void);
    void update_faces(void);
    void face_helper(void);
    void add_food(void);
    void set_rd_values(float _feed, float _kill, float _ra, float _rb);
    Cell* farthest;
    
    std::set<Face> face_set;
    
private:
    void init_springs(float);
    void spread_food(Cell* c, float amount, float decay);
    void add_food(float amount);
    void calcification_food(float amount);
    void x_axis_density(float amount);
    void planar(float amount);
    void face_food(float amount);
    void reaction_diffusion(void);
    void average_positions(void);
    std::vector<Cell*> find_collisions(Cell* c);
    float split_threshold, roi_squared, spring_factor, bulge_factor,
    planar_factor, repulsion_strength, link_rest_length;
    
    float feed, kill, ra, rb;
    int frame_num;
    bool equal_food;
    Grid* g;
    
    // sphere creation
    std::vector<Vec3f> icosa_vertices(void);
    void subdivide_iteration(std::deque<Vec3f>* vertices);
    std::vector<Vec3f> remove_duplicates(std::deque<Vec3f>* dup_list);
    std::vector<Vec3f> subdivided_icosahedron(int levels);
    std::vector<Vec3f> init_sphere_points(float n, float r);
    std::string s;
    food_mode_enum mode;
};

#endif /* simulation_h */

