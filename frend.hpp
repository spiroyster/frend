#ifndef FREND_HPP
#define FREND_HPP

#include <vector>
#include <memory>
#include <functional>
#include <type_traits>

namespace frend
{


struct VEC2 { float x, y; };
struct VEC3 { float x, y, z; };
struct VEC4 { float x, y, z, w; };

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

struct DIM2i
{
    DIM2i()
        :   u_(0), v_(0)
    {
    }

    DIM2i(unsigned int u, unsigned int v)
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

struct Geometry
{
    std::vector<VEC3> points_;
    std::vector<VEC3> normals_;
    std::vector<VEC2> uv_;
    std::vector<unsigned int> indicies_;     // In threes t1p1, t1p2, t1p3, t2p1, t2p2, t2p3
};

namespace geom
{
    Geometry Sphere(unsigned int uSegments, unsigned int vSegments);
    Geometry Quad(const VEC3& p1, const VEC3& p2, const VEC3& p3, const VEC4& p4);
    Geometry Cube();
}

// Intersection of a ray with RAY payload. Fragment maynot be present.
class Ray
{
public:
    Ray();
       
    VEC4 p_;                     // position
    VEC4 d_;                     // direction
    float weight_;               // weighting of this ray.
    void* payload_;
    
    Ray* destin_;                // destination (result of intersection)
    Ray* origin_;                // origin (ray that spawned this ray)
    
    void* fragment_;            // Pointer to fragment if calculated for this ray
    VEC2 fragmentUV_;           // UV of fragment if calculated for this ray
};

class FrameBase
{
    unsigned int width_;
    unsigned int height_;

protected:
    FrameBase(unsigned int width, unsigned int height);

public:
    unsigned int Width() const;
    unsigned int Height() const;
    unsigned int UVn(unsigned int u, unsigned int v) const;
    DIM2i nUV(unsigned int n) const;
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
    DIM2i uv_;
    
    void ResetFragmentTexel(RAY& ray)
    {
        Reset(ray);
    }
    void AccumulateFragmentTexel(const VEC2& uv, RAY& ray)
    {
        Accumulate(uv, ray);
    }
    
    
public:
    void InitialiseFragmentTexel(const DIM2i& uv, OUTTEXEL* texel)
    {
        texel_ = texel;
        uv_ = uv;
    }

    virtual void Reset(RAY& ray) = 0;
    virtual void Accumulate(const VEC2& uv, RAY& ray) = 0;

};


namespace Kernel
{
    // Kernel functions    
}

typedef std::function<void(const Ray&)> ShaderKernel;     // ray to accumulate to and spawn to
typedef std::function<void(const Ray&)> PathKernel;       // path kernel invoked in relation to intersected ray
typedef std::function<void(const Ray&)> ProjectionKernel; // primary ray pose to adjust

class ContextBase
{
    PathKernel pathKernel_;

protected:
    
    ContextBase(unsigned int count, unsigned int shaderCount, const PathKernel& pathKernel);

    void BaseMaterialShader(unsigned int n, const ShaderKernel& shaderKernel);
    void BaseMaterialShaderCL(unsigned int n, const std::string& source);
    void BaseFlushShaders();

    void BaseStaticGeometry(const Geometry& geometry, unsigned int shaderIndex);
    void BaseFlushGeometry();

    void BaseRender(FrameBase& frameBase, const ProjectionKernel& projectionKernel);

    std::vector<Ray> pool_;
    
public:
    // Accessor to raystates

};


// A frame contains a resultant image output (for convieneice)
// and also compositors which are interfaces for shaders to accumulate
// contributions/results to. 
template <class FRAGMENT, class OUTTEXEL>
class Frame : public FrameBase
{
    std::vector<FRAGMENT> fragments_;
    std::vector<OUTTEXEL> frameImage_;
    
public:
    
    Frame(unsigned int width, unsigned int height)
        :   FrameBase(width, height), frameImage_(width*height), fragments_(width*height)
    {
        
        // Initialise the fragments.
        for ( unsigned int n = 0; n < width*height; ++n )
            fragments_[n].InitialiseFragmentTexel(nUV(n), &frameImage_[n]);
    }

    const std::vector<OUTTEXEL>& FrameImageTexels() const
    {
        return frameImage_;
    }


};



// The ray pool contains the caches for rendering a single frame.
// A Pool is customised (templated) with the type that a ray should
// hold.
template <class RAY>
class Context : public ContextBase
{
    std::vector<RAY> payloads_;
public:
    Context(unsigned int count, unsigned int materialShaderCount, const PathKernel& pathKernel)
        :   ContextBase(count, materialShaderCount, pathKernel), payloads_(count)
    {
        // Assign the address of each payload to the pool states.
        for ( unsigned int n = 0; n < count; ++n )
            pool_[n].payload_ = &payloads_[n];
    }

    // shader array manipulation.
    void FlushShaders() { BaseFlushShaders(); }
    void Shader(unsigned int index, const ShaderKernel& shaderKernel) { BaseMaterialShader(index, shaderKernel); }
    void ShaderCL(unsigned int index, const std::string& source) { BaseMaterialShaderCL(index, source); }    
    
    // scene geometry
    void FlushGeometry() { BaseFlushGeometry(); }
    void StaticGeometry(const Geometry& geometry, unsigned int materialShader) { BaseStaticGeometry(geometry, materialShader); }
    
    // Render
    void Render(FrameBase& frame, const ProjectionKernel& projectionKernel) { BaseRender(frame, projectionKernel); }
};





}   // namespace frend

#endif // FREND_HPP
