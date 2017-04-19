#include "frend.hpp"
#include "ppm.hpp"
#include "geom.hpp"

class MyRay
{
public:
	float rgba[4];
};

class MyTexel
{
public:
	unsigned char rgba[4];
};

class MyFragment : public frend::Fragment<MyRay, MyTexel>
{
public:
	void Reset(MyRay& ray)
	{
	}
    void Accumulate(const frend::VEC2& uv, MyRay& ray)
	{
	}
};



int main( int argc, char** argv )
{

	unsigned int width = 640;
	unsigned int height = 480;

	// Create our ray pool, which uses the same ray type as our frame sampler.
	frend::Pool<MyRay> rayPool(width * height * 8);

	// the pool also holds the shaders for the specified ray type!
	rayPool.CreateShader([](frend::ShaderFunctionInterface& sfi, const frend::Intersection& intersection)
	{
		// No reflection so simply calculate the colour.

		// And simply accumulate the colour.
		MyRay* ray = static_cast<MyRay*>(intersection.fragment_);

		ray->rgba[0] = 255;
		
	});





	// Camera kernel??? which needs to know ray type (to reset) and
	// needs to know 




	// Then we need a renderer which takes the frame and the pool/context
	// and the geometry and camera information and then performs the recurssive
	// rendering





	// Create our frame with the correct samplers for composition (same ray type,
	// and fragment sampler is compatible).
	frend::Frame<MyFragment, MyTexel> frame(width, height);

	
	// Create our renderer.

	// Set the pool, and the frame

	// Render by passing the camera kernel, geometry
	


	// Create scene tree from the geometry.
	// Assign the shaders

	// Render the frame by passing it the Frame and Pool



	
	// Generate our geometry.

	

	// Create our normal shader (this needs to know ray type) 




	// Create our scene from the geometry and associate the correct
	// normal shader with it.



	// Clear the frame ready for rendering and setup the 

	// Use the scene geometry context, render the frame.


	// When satisfied, grab an image for the frame and save it out as ppm
	//frend::WritePPM("output.ppm", frame.FrameImage());

	return 0;
}
