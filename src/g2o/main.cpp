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
using namespace std; 

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
    virtual bool read( istream& in ) {}
    virtual bool write( ostream& out ) const {}
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
    virtual bool read( istream& in ) {}
    virtual bool write( ostream& out ) const {}
public:
    double _x;  //x value, y value _measurement
};

int main( int argc, char** argv )
{
    double a=1.0, b=2.0, c=1.0;         // True parameter value
    int N=100;                          // Data size
    double w_sigma=1.0;                 // Sigma for adding gaussian noise
    cv::RNG rng;                        // OpenCV random number generator
    double abc[3] = {0,0,0};            // abc Estimated value of the parameter

    vector<double> x_data, y_data;      // data
    
    cout<<"generating data: "<<endl;
    for ( int i=0; i<N; i++ )
    {
        double x = i/100.0;
        x_data.push_back ( x );
        y_data.push_back (
            exp ( a*x*x + b*x + c ) + rng.gaussian ( w_sigma )
        );
        cout<<x_data[i]<<" "<<y_data[i]<<endl;
    }
    
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
    cout<<"start optimization"<<endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    optimizer.initializeOptimization();
    optimizer.optimize(100);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout<<"solve time cost = "<<time_used.count()<<" seconds. "<<endl;
    
    // Output optimized value
    Eigen::Vector3d abc_estimate = v->estimate();
    cout<<"estimated model: "<<abc_estimate.transpose()<<endl;
    
    return 0;
}
