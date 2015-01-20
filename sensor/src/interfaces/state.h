/*
* Interface for model states
* A State can return its own representation in different formats.
*/

#include <eigen/Eigen>
#include "headers/numeric_typedefs.h"

class State
{
public:
    virtual ~State() {}

    virtual const Eigen::VectorXf& GetX() const = 0;
    virtual Eigen::VectorXf& GetX() = 0;

    virtual const Eigen::MatrixXf& GetP() const = 0;
    virtual Eigen::MatrixXf& GetP() = 0;

    virtual U32 GetCurrentTimeStamp() const = 0;
};