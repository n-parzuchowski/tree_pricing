// tree.cpp calculates option prices using a tree method
// A bit of overkill, but a good learning enviroment of object-oriented programming
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

double excersize( bool call , double strike , double spot ){
  if (call)
    return ( max(spot - strike,0.0) ); 
  else
    return (max(strike-spot,0.0) );
}

// node in the pricing tree 
struct tree_node{
  int time,ypos;
  double spot,optval;
  double no_risk_prob;  
  int get_neighbor_top_left(void);
  int get_neighbor_bottom_left(void);
  int get_neighbor_top_right(void);
  int get_neighbor_bottom_right(void);
};

class tree{
  tree_node * node;
  int num_nodes;
public:
  void allocate_tree(int); 
  void populate(int);
  void kill_tree(void);
  void assign_values_probs(double,double,double,double,double);
  double price_option(double,bool,bool,double,double,double);
};
//==============================================================================
//==============================================================================
int tree_node::get_neighbor_top_right(){
  int ii_before = (time+2)*(time+1)/2;
  return ii_before + ypos + 1;
}
//==============================================================================
//==============================================================================
int tree_node::get_neighbor_bottom_right(){  
  int ii_before = (time+2)*(time+1)/2;
  return ii_before + ypos;
}
//==============================================================================
//==============================================================================
int tree_node::get_neighbor_top_left(){
  if (time*(time+1)/2+ypos+1 == (time+1)*(time+2)/2)
    return -1;
  int ii_before = time*(time-1)/2;
  return ii_before + ypos;
}
//==============================================================================
//==============================================================================
int tree_node::get_neighbor_bottom_left(){  
  int ii_before = time*(time-1)/2;
  if (ypos == 0){
    return -1;
  }
  else
    return ii_before + ypos - 1; 
}
//==============================================================================
//==============================================================================
void tree::allocate_tree(int Nstep){
  num_nodes = (Nstep+1)*(Nstep+2) /2;
  node = new tree_node [num_nodes];
}
//==============================================================================
//==============================================================================
void tree::kill_tree(){
  delete[] node;
}
//==============================================================================
//==============================================================================
void tree::populate(int Nsteps)
{
  for (int N=0; N< Nsteps+1; N++){
    int ii_before = N *(N+1)/2;

    for (int ii=ii_before; ii < (N+1)*(N+2)/2; ii++ ){

      node[ii].time = N;
      node[ii].ypos = ii-ii_before;	
    }
  }

}  
//==============================================================================
//============================================================================== 
void tree::assign_values_probs(double S, double dS_up , double dS_down, double rate, double dT){
  
  node[0].spot=S;  
  for ( int ii = 0 ; ii < num_nodes; ii++ ){
    
    int jj = node[ii].get_neighbor_bottom_right();
    int kk = node[ii].get_neighbor_top_right();

    if (kk < num_nodes){ 
      node[jj].spot = node[ii].spot - dS_down;
      node[kk].spot = node[ii].spot + dS_up;
    }
    
    node[ii].no_risk_prob = (node[ii].spot*(exp(dT*rate)-1)+dS_down) / (dS_up + dS_down);

  }
}
//==============================================================================
//============================================================================== 
double tree::price_option(double strike, bool call, bool ee, double rate , double dT , double bar){
  //price option by backwards induction 

  for (int ii=num_nodes-1 ; ii>=0 ; ii--){

    int jj = node[ii].get_neighbor_bottom_right();
    int kk = node[ii].get_neighbor_top_right();

    double excersize_value = excersize(call,strike,node[ii].spot);
    double intrinsic_value = excersize_value;
    double dep = exp(-rate*dT);
      
    if (kk < num_nodes) {
      
      intrinsic_value = (node[ii].no_risk_prob * ( node[kk].optval - node[jj].optval) + node[jj].optval)*dep;
      	
      if (ee) {
	
	node[ii].optval = max(excersize_value,intrinsic_value);
      }
      else{

	node[ii].optval = intrinsic_value;
      }
    }
    else
      node[ii].optval = excersize_value;

    if (node[ii].spot < bar )
      node[ii].optval = 0.0;
    
  }
	
  return node[0].optval;
}  
//==============================================================================
//============================================================================== 
void print_error_message(string error)
// usage statement
{
  cout << error << endl;
  cout << "Usage: compute_tree <OPTION_TYPE (AC,EC,BC,AP,EP,BP)> <SPOT> <STRIKE> <EXPIRY> <RATE> <dS+> <dS-> <STEPS> [BARRIER]\n\n"; 
}
//==============================================================================
//==============================================================================       
int main(int argc, char* argv[])
{

  if (argc < 9){
    print_error_message("");
    return 0;
  }

  char* Opt_type = argv[1];
  double spot_0 = atof(argv[2]); // initial value of stock
  double strike = atof(argv[3]);
  double expiry_time = atof(argv[4]);
  double interest_rate = atof(argv[5]);
  double up_jump = atof(argv[6]); //change in stock per step if positive 
  double down_jump = atof(argv[7]); //change in stock per step if negative  
  int Nsteps = atoi(argv[8]);
  double barrier;
  bool early_exit=false, call=false;

  
  // Print what is being calculated 
  switch ( Opt_type[0] ) {
  case 'A':
    early_exit = true;
    if (Opt_type[1]=='C'){
      call = true;
      cout << "\nAmerican call option with spot " << spot_0 << " struck at " << strike << "\n\n";
    }
    else if (Opt_type[1]=='P') 
      cout << "\nAmerican put option with spot " << spot_0 << " struck at " << strike << "\n\n";
    else
      {
	print_error_message("\nRequested price not available");
	return 0;
      }
    break;
  case 'E':
    if (Opt_type[1]=='C'){
      call = true;
      cout << "\nEuropean call option with spot " << spot_0 << " struck at " << strike << "\n\n";
    }
    else if (Opt_type[1]=='P') 
      cout << "\nEuropean put option with spot " << spot_0 << " struck at " << strike << "\n\n";
    else
      {
	print_error_message("\nRequested price not available");
	return 0;
      }
    break;
    
  default:
    print_error_message("\nRequested price not available");
    return 0;
  } 

  if (argc == 10) {
    barrier = atof(argv[9]); // barrier option. 
    cout << "Barrier applied at "<< barrier << endl;
  }
  else
    barrier = spot_0 - 2 * Nsteps* down_jump;
  // time step
  double dT = expiry_time/Nsteps;  

  // set up tree 
  tree value; 

  // create tree object
  value.allocate_tree(Nsteps);
    
  value.populate(Nsteps);
  value.assign_values_probs(spot_0, up_jump , down_jump, interest_rate , dT);

  cout << "Option price: "<< value.price_option(strike, call,early_exit, interest_rate ,dT,barrier) << endl;

  // free the memory of the tree object
  value.kill_tree();

  // successful run
  return 0;
}
