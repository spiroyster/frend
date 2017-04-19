#ifndef FREND_HPP
#define FREND_HPP

#include <vector>
#include <memory>
#include <functional>
#include <type_traits>

namespace frend
{


struct VEC2 
{ 
    float x, y; 
};
struct VEC3 
{ 
    float x, y, z; 
};
struct VEC4 
{ 
    float x, y, z, w; 
};
struct MAT22 
{
    float 
    a, b, 
    c, d;
};
struct MAT33
{
    float 
    a, b, c, 
    d, e, f, 
    g, h, j;
};
struct MAT44
{
    float 
    a, b, c, d,
    e, f, g, h,
    i, j, k, l,
    m, n, o, p;
};

struct VEC2i
{
    VEC2i(unsigned int u, unsigned int v)
        :   u_(u), v_(v)
    {
    }
    unsigned int u_;
    unsigned int v_;
};

namespace Matrix
{
    // Identity
    //MAT44 Identity();
    //MAT33 Identity();

    // Multiply

    // Translate
    //MAT33 Translate(const VEC2&);
    //MAT33 Translate(const VEC3&);
    //MAT44 Translate(const VEC2&);
    //MAT44 Translate(const VEC3&);
    //MAT44 Translate(const VEC4&);
    
    // Rotate

    // Scale
    // rodreigues
    // 
}
namespace Vector
{
    // rodregiues
    // dot
    // unitise
    // 
}


// class Image
// {
//     std::vector<unsigned char> bits_;
//     unsigned int width_;
//     unsigned int height_;
//     unsigned int channels_;
// public:

//     unsigned int width() const { return width_; }
//     unsigned int height() const { return height_; }
//     unsigned int channels() const { return channels_; }
//     const std::vector<unsigned char>& bits() const { return bits_; }

//     Image(unsigned int width, unsigned int height, unsigned int channels, const std::vector<unsigned char>& bits)
//         :   width_(width), height_(height), channels_(channels), bits_(bits)
//     {
//     }

//     Image(unsigned int width, unsigned int height, unsigned int channels)
//         :   width_(width), height_(height), channels_(channels), 
//             bits_(width*height*channels, static_cast<unsigned char>(0))
//     {
//     }
// };

// template <class TEXEL>
// class Image2D
// {
//     std::vector<TEXEL> texels_;
//     unsigned int width_;
//     unsigned int height_;
// public:

//     unsigned int width() const { return width_; }
//     unsigned int height() const { return height_; }
//     const std::vector<TEXEL>& texels() const { return texels_; }

//     Image(unsigned int width, unsigned int height)
//         :   width_(width), height_(height)
//     {
//         texels_.insert(width_*height_);
//     }

//     Image(unsigned int width, unsigned int height, const std::vector<TEXEL>& texels)
//         :   width_(width), height_(height), texels_(texels), 
//     {
//         assert(texels_.size()==width_*height_)
//     }
// };


struct Geometry
{
    std::vector<VEC3> points_;
    std::vector<VEC3> normals_;
    std::vector<VEC2> uv_;
    std::vector<unsigned int> indicies_;     // In threes t1p1, t1p2, t1p3, t2p1, t2p2, t2p3
};



// fragment interface
template <class RAY>
class FragmentInterface
{
public:
    virtual void ResetFragmentTexel(RAY& ray) = 0;
    virtual void AccumulateFragmentTexel(const VEC2& uv, RAY& ray) = 0;
};

// the fragment, translates RAY payload to OUTTXEL for the result.
template <class RAY, class OUTTEXEL>
class Fragment : public FragmentInterface<RAY>
{
protected:
    OUTTEXEL* texel_;
    unsigned int u_, v_;

    
    void ResetFragmentTexel(RAY& ray)
    {
        Reset(ray);
    }
    void AccumulateFragmentTexel(const VEC2& uv, RAY& ray)
    {
        Accumulate(uv, ray);
    }
    
    
public:
    void InitialiseFragmentTexel(const VEC2i& uv, OUTTEXEL* texel)
    {
        texel_ = texel;
        u_ = uv.u_;
        v_ = uv.v_;
    }

    virtual void Reset(RAY& ray) = 0;
    virtual void Accumulate(const VEC2& uv, RAY& ray) = 0;

};

// Intersection of a ray with RAY payload. Fragment maynot be present.
class Intersection
{
public:
    Intersection()
        :   weight_(0), payload_(0), destin_(0), origin_(0), fragment_(0)
    {
    }

    VEC4 p_;                     // position
    VEC4 d_;                     // direction
    float weight_;               // weighting of this ray.
    void* payload_;
    
    Intersection* destin_;                // destination (result of intersection)
    Intersection* origin_;                // origin (ray that spawned this ray)
    void* fragment_;
};


class Shader
{
public:

};



// The interface available to the shader kernel.
class ShaderFunctionInterface
{
public:
    // shader type

    // CPU bound function pointer.


    // OpenCL pointer.
};

// the interface available to the path kernel


// the interface available to the 


typedef std::function<void(ShaderFunctionInterface& sfi, const Intersection& intersection)> ShaderFunction;
    

// The ray pool contains the caches for rendering a single frame.
// A Pool is customised (templated) with the type that a ray should
// hold.
template <class RAY>
class Pool
{
    std::vector<Intersection> intersections_;
    std::vector<RAY> rays_;

public:
    Pool(unsigned int count)
        :   rays_(count), intersections_(count)
    {
        // Initialise the fragments.
        for ( unsigned int n = 0; n < count; ++n )
            intersections_[n].fragment_ = &rays_[n];
    }

    // Also manage the shaders here since these need to be of the
    // RAY type.
    
    // Add a CPU bound shader.
    //typedef std::function<void(ShaderFunctionInterface& sfi, const Intersection& intersection)> ShaderFunction;
    
    Shader CreateShader(ShaderFunction shader)
    {
        return Shader();
    }

    // Add an OpenCL bound shader.
    
};


// A frame contains a resultant image output (for convieneice)
// and also compositors which are interfaces for shaders to accumulate
// contributions/results to. 
template <class FRAGMENT, class OUTTEXEL>
class Frame
{
    std::vector<FRAGMENT> fragments_;
    std::vector<OUTTEXEL> frameImage_;
    unsigned int width_, height_;

public:
    
    Frame(unsigned int width, unsigned int height)
        :   width_(width), height_(height), frameImage_(width*height), fragments_(width*height)
    {
        
        // Initialise the fragments.
        for ( unsigned int n = 0; n < width_*height_; ++n )
            fragments_[n].InitialiseFragmentTexel(nUV(n), &frameImage_[n]);
    }

    const std::vector<OUTTEXEL>& FrameImageTexels() const
    {
        return frameImage_;
    }

    unsigned int Width() const { return width_; }
    unsigned int Height() const { return height_; }

    unsigned int UVn(unsigned int u, unsigned int v) const
    {
        return ( v * width_ ) + u;
    }
    VEC2i nUV(unsigned int n) const
    {
        unsigned int v = n / width_;
        return VEC2i(n - (v*width_), v);
    }

    // Also camera setup?

};







class Scene
{
public:

    // Add geometry with associated shader.



};

// This object performs the actual rendering, and uses the camera

class Render
{
public:
    // Render: pass frame to render, and ray pool

    // Constructor with flags for runtime.

    

    // RenderFrame

};



}   // namespace frend

#endif // FREND_HPP
