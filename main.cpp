#include "frend.hpp"

#include "..\EZNetBPM\EZNetBPM.hpp"

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
	unsigned int maxbounce = 4;

	unsigned int numberOfshaders = 12;
	unsigned int NORMALSHADER = 0;

	// Create our context.
	frend::Context<MyRay> context(width*height*maxbounce, numberOfshaders,
		[](const frend::Ray& ray)
	{
		// Path kernel 
	});

	// Set the shader to use for the geometry when there is an intersection.
	context.Shader(NORMALSHADER, 
		[](const frend::Ray& ray)
	{
		// Shader kernel
	});

	// Create and add our geometry.
	context.StaticGeometry(frend::geom::Cube(), NORMALSHADER);

	// Create our frame with the correct samplers for composition (same ray type,
	// and fragment sampler is compatible).
	frend::Frame<MyFragment, MyTexel> frame(width, height);

	// And then proceed to render the frame.
	context.Render(frame, 
		[](const frend::Ray& ray)
	{
		// Projection kernel
	});

	// Save out the image.
	EZNetBPM::Image frameImage(width, height, 32, &frame.FrameImageTexels()[0]);
	frameImage.Write(EZNetBPM::Type::P3, "image.ppm");
	
	return 0;
}
