// tree.cpp calculates option prices using a tree method
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;


// node in the pricing tree 
struct tree_node{
  int time,ypos,link_back,link_up,link_down;
  double spot,optval_intr,optval_ex;
  double prob_no_risk;
};
  
void print_error_message(string error)
// usage statement
{
  cout << error << endl;
  cout << "Usage: compute_tree <OPTION_TYPE (AC,EC,BC,AP,EP,BP)> <SPOT> <STRIKE> <EXPIRY> <RATE> <dS+> <dS-> <STEPS> [BARRIER]\n\n"; 
}

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
  double barrier =  0.0;
  bool early_exit=false, call=false, barrier_exists=false; 
  
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
  case 'B':
    if ( argc != 10)
      {
	print_error_message("\nAdd barrier price to command line arguments");
	return 0;
      }
    
    barrier =  atof(argv[9]);
    barrier_exists = true;
    
    if (Opt_type[1]=='C'){
      call = true;
      cout << "\nEuropean call option with spot " << spot_0 << " struck at " << strike;
      cout << " and barrier " << barrier << "\n\n";
    }
    else if (Opt_type[1]=='P') {
      cout << "\nEuropean put option with spot " << spot_0 << " struck at ";
      cout<< strike <<  " and barrier " << barrier << "\n\n";
    }
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

  // time step
  double dT = expiry_time/Nsteps;  

  // set up tree 
  tree_node * tree; 
  tree = new tree_node [Nsteps]; 




  
  delete[] tree;
  
  // successful run
  return 0;
}