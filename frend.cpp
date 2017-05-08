#include "frend.hpp"

namespace frend
{

Ray::Ray() 
    :   weight_(0), payload_(0), destin_(0), origin_(0), fragment_(0)
{
}

FrameBase::FrameBase(unsigned int width, unsigned int height)
    :   width_(width), height_(height)
{
}

unsigned int FrameBase::Width() const 
{ 
    return width_; 
}
unsigned int FrameBase::Height() const 
{ 
    return height_; 
}

unsigned int FrameBase::UVn(unsigned int u, unsigned int v) const
{
    return ( v * width_ ) + u;
}

DIM2i FrameBase::nUV(unsigned int n) const
{
    unsigned int v = n / width_;
    return DIM2i(n - (v*width_), v);
}



ContextBase::ContextBase(unsigned int count, unsigned int shaderCount, const PathKernel& pathKernel)
:   pool_(count), pathKernel_(pathKernel)
{
    // Initialise our buffers and pools.

}
    
void ContextBase::BaseMaterialShader(unsigned int n, const ShaderKernel& shaderKernel)
{
    // set the shader at index n to be the kernel supplied
}
void ContextBase::BaseMaterialShaderCL(unsigned int n, const std::string& source)
{
    // set the shader at index n to be the kernel compiled by the source provided.
    // OpenCL
}
void ContextBase::BaseFlushShaders()
{
    // Flush all the shaders.
}

void ContextBase::BaseStaticGeometry(const Geometry& geometry, unsigned int shaderIndex)
{

}

void ContextBase::BaseFlushGeometry()
{

}

void ContextBase::BaseRender(FrameBase& frameBase, const ProjectionKernel& projectionKernel)
{
    // Reset the fragment

    // Assert there is enough primary rays in the pool for the framebuffer.
    
    // Invoke the projectionKernel for each primary ray. This is invoked here
    // CPU bound currently to set the initial positions of the primary rays.


    



}


}   // frend