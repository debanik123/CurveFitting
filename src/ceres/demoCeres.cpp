#include <iostream>
#include <opencv2/core/core.hpp>
#include <ceres/ceres.h>
#include <chrono>
#include <fstream>
#include "CommonCurve.h" //std::left std::setw std::setfill
#include "CeresCost.h" 
#include <boost/program_options.hpp>
#include <algorithm>
#include <iterator>
#include <filesystem>
namespace fs = std::filesystem;
using namespace boost;
namespace po = boost::program_options;


int main ( int argc, char** argv )
{
    std::string dataFile = "readDataCeres.txt";
    std::string parameter = "parametersCeres.txt";
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
        // x data 
        double x = i/100.0;
        x_data.push_back ( x );

        // y data
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
    // Constructing the Least Squares Problem
    ceres::Problem problem;
    for ( int i=0; i<N; i++ )
    {
    // Add an error term to the question
    // Use automatic derivation, template parameters: 
    // error type, output dimension, input dimension, dimension should be consistent with the previous struct
        problem.AddResidualBlock (   
            new ceres::AutoDiffCostFunction<za::CURVE_FITTING_COST, 1, 3> ( 
                new za::CURVE_FITTING_COST ( x_data[i], y_data[i] )
            ),
            nullptr,            // Kernel function, not used here, empty
            abc                 // Parameters to be estimated
        );
    }

    //Configure solver

    ceres::Solver::Options options;   

    // How to solve the incremental equation  
    options.linear_solver_type = ceres::DENSE_QR;  

    // Output to std::cout
    options.minimizer_progress_to_stdout = true;   
    options.max_num_iterations = iterate;   

    // Optimization information
    ceres::Solver::Summary summary;      

    // Timer to compute solving time         
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    // Start to optimize
    ceres::Solve ( options, &problem, &summary );  
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );
    std::cout<<"solve time cost = "<<time_used.count()<<" seconds. \n";
    std::ofstream mySol;
    mySol.open (&pathParameter[0]);
    // Output result
    std::cout<<summary.BriefReport() << "\n";
    mySol<<"Estimated a,b,c =";
    for ( auto val:abc ) mySol<<val<<" ";
    mySol<<"\n";
    mySol.close();

    return 0;
}

