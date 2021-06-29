#include <iostream>
#include <g2o/core/base_vertex.h>
#include <g2o/core/base_unary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <Eigen/Core>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <chrono>
#include <boost/program_options.hpp>
#include <filesystem>
#include "CommonCurve.h" //std::left std::setw std::setfill

namespace fs = std::filesystem;
using namespace boost;
namespace po = boost::program_options;
// The vertex of the curve model, template parameters: optimization of variable dimensions and data types
class CurveFittingVertex: public g2o::BaseVertex<3, Eigen::Vector3d>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() // Reset
    {
        _estimate << 0,0,0;
    }
    
    virtual void oplusImpl( const double* update ) // Update
    {
        _estimate += Eigen::Vector3d(update);
    }
    // Save and read: leave blank
    virtual bool read( std::istream& in ) {}
    virtual bool write( std::ostream& out ) const {}
};

// Error model template parameters: observation dimension, type, connection vertex type
class CurveFittingEdge: public g2o::BaseUnaryEdge<1,double,CurveFittingVertex>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    CurveFittingEdge( double x ): BaseUnaryEdge(), _x(x) {}
    // Calculate curve model error
    void computeError()
    {
        const CurveFittingVertex* v = static_cast<const CurveFittingVertex*> (_vertices[0]);
        const Eigen::Vector3d abc = v->estimate();
        _error(0,0) = _measurement - std::exp( abc(0,0)*_x*_x + abc(1,0)*_x + abc(2,0) ) ;
    }
    virtual bool read( std::istream& in ) {}
    virtual bool write( std::ostream& out ) const {}
public:
    double _x;  //x value, y value _measurement
};

int main( int argc, char** argv )
{

    std::string dataFile = "readDataG2O.txt";
    std::string parameter = "parametersG2O.txt";
    std::string path = "./result/";
    // create directory if not exist
    struct stat info;
    if( stat( path.c_str(), &info ) != 0 )fs::create_directories(path);
    std::string pathRealData = path + za::getCurrentTime() + dataFile;
    std::string pathParameter = path + za::getCurrentTime() + parameter;
  
    // True parameter value to be estimated 
    double a, b, c;  

    // Total number of data point       
    int N, iterate ;   

    // Noise Sigma value                       
    double w_sigma; 
    // Cammand line parser 
	try 
		{
			po::options_description desc("Allowed options");
			desc.add_options()
				("help,h", "produce help message")
				("first,a", po::value<double>(),
					"first parameter")			
                ("second,b", po::value<double>(),
					"second parameter")
				("third,c", po::value<double>(),
					"third parameter")				
                ("number,n", po::value<int>(&N)->default_value(100)->implicit_value(500),
					"number of data")                
                ("iteration,i", po::value<int>(&iterate)->default_value(100)->implicit_value(200),
					"number of iterations")
				("noise,s", po::value<double>(&w_sigma)->default_value(1)->implicit_value(5),
					"noise added");		
                    
	
			/* Point out that all unknown values ​​should be converted to the value of the "input-file" option.
					* Also use the command_line_parser class instead of parse_command_line */
			po::variables_map vm;        
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);  

			if (vm.count("help")) 
			{
				std::cout << "Usage: options_description [options]\n";
				std::cout << desc;
				return 0;
			}
 			if (vm.count("first") ) 
        	{

                a =  vm["first"].as<double>();
        	} 
        	else 
       		{
            	std::cout << "First parameter not set.\n";
				return -1;
        	}			
 			if (vm.count("second") ) 
        	{

                b =  vm["second"].as<double>();
        	} 
        	else 
       		{
            	std::cout << "Second parameter not set.\n";
				return -1;
        	}			
 			if (vm.count("third") ) 
        	{

                a =  vm["third"].as<double>();
        	} 
        	else 
       		{
            	std::cout << "Third parameter not set.\n";
				return -1;
        	}			
        }// end of try
			
        catch(std::exception& e)
		{
			std::cout << e.what() << "\n";
			return 1;
		}







      // OpenCV random number generator               
    cv::RNG rng;       

    // abc Estimated value of the parameter                 
    double abc[3] = {0,0,0};            

   // set of x and y data 
    std::vector<double> x_data, y_data;      
    std::ofstream myfile;
    const int nameWidth     = 6;
    const int numWidth      = 8;
    myfile.open (&pathRealData[0]);
    for(int i = 0; i < 25; i++) za::printElement("-", 1, myfile);
    myfile <<"\n";
    za::printElement("|", 1, myfile);
    za::printElement("N", nameWidth, myfile);
    za::printElement("|", 1, myfile);
    za::printElement("X", nameWidth, myfile);
    za::printElement("|", 1, myfile);
    za::printElement("Y", 10, myfile);
    za::printElement("|", 1, myfile);
    myfile <<"\n";
    for(int i = 0; i < 25; i++) za::printElement("-", 1, myfile);
    myfile <<"\n";
    std::cout<<"Generating data: \n";
    
    for ( int i=0; i<N; i++ )
    {
        double x = i/100.0;
        x_data.push_back ( x );
        y_data.push_back (
            exp ( a*x*x + b*x + c ) + rng.gaussian ( w_sigma )
        );
        za::printElement("|", 1, myfile);
        za::printElement(i+1, nameWidth, myfile);
        za::printElement("|", 1, myfile);
        za::printElement(x_data[i], nameWidth, myfile);
        za::printElement("|", 1, myfile);
        za::printElement(y_data[i], 10, myfile);
        za::printElement("|", 1, myfile);
        myfile <<"\n";
        for(int i = 0; i < 25; i++) za::printElement("-", 1, myfile);
        myfile <<"\n";
    }
     myfile.close();   
    // Build graph optimization, first set g2o
    // The dimension of the optimized variable for each error term is 3, and the dimension of the error value is 1
    typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block;  
    //zouma
    // Linear equation solver
    std::unique_ptr<Block::LinearSolverType> linearSolver 
				(new g2o::LinearSolverDense<Block::PoseMatrixType>());

    // Matrix block solver
    std::unique_ptr<Block> solver_ptr (new Block(std::move(linearSolver)));
	
    // Gradient descent method, choose from GN, LM, DogLeg
    g2o::OptimizationAlgorithmGaussNewton * solver = new g2o::OptimizationAlgorithmGaussNewton(std::move(solver_ptr));
    //zouma
    
    g2o::SparseOptimizer optimizer;     // Graph model
    optimizer.setAlgorithm( solver );   // Set up the solver
    optimizer.setVerbose( true );       // Turn on debug output
    
    // Add vertices to the graph
    CurveFittingVertex* v = new CurveFittingVertex();
    v->setEstimate( Eigen::Vector3d(0,0,0) );
    v->setId(0);
    optimizer.addVertex( v );
    
    // Add edges to the graph
    for ( int i=0; i<N; i++ )
    {
        CurveFittingEdge* edge = new CurveFittingEdge( x_data[i] );
        edge->setId(i);
        edge->setVertex( 0, v );                // Set the connected vertices
        edge->setMeasurement( y_data[i] );      // Observed value
        edge->setInformation( Eigen::Matrix<double,1,1>::Identity()*1/(w_sigma*w_sigma) ); // Information matrix: the inverse of the covariance matrix
        optimizer.addEdge( edge );
    }
    
    //Perform optimization
    std::cout<<"start optimization\n";
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    optimizer.initializeOptimization();
    optimizer.optimize(iterate);
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );
    std::cout<<"solve time cost = "<<time_used.count()<<" seconds. \n";
    
    // Output optimized value
    Eigen::Vector3d abc_estimate = v->estimate();
    std::ofstream mySol;
    mySol.open (&pathParameter[0]);
    mySol<<"estimated model a, b, c: "<<abc_estimate.transpose()<<"\n";
    mySol.close();    
    return 0;
}