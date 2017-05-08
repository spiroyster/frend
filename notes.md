Notes for frend.

Context manages the pools and caches.

Pools:




Caches:




PreRender.

Shader upload
Geometry -> Shader association
Geometry upload
Acceleration tree construction
Framebuffer create
    - added compositor/accumulator
    - image output dimensions and pixel format set


Render Pipeline.

Initial ray upload POSE
Intersection test
Kernel invoke
Shader invoke

Supported Primitives:
Triangle (  p1x, p1y, p1z, n1x, n1y, n1z, u1x, u1y, u1z,
            p2x, p2y, p2z, n2x, n2y, n2z, u2x, u2y, u2z,
            p3x, p3y, p3z, n3x, n3y, n3z, u3x, u3y, u3z)
AABB (px, py, pz, szx, szy, szz, )???
AAS  (px, py, pz, r)


A ray structure is a link in the chain.

POSE
{
    prev       if 0 then is primary ray
    pixel      if 0 then needs projecting
    pos        position
    dir        direction
    next       if 0 then has not been reflected
};


SHADERINVOKE
{
    shader     the shader to invoke
    POSE*      the intersection information
};



StartPipe:

Initial ray upload (pixel given, no next)
-> Intersection test.
    - no intersection  nextray goto startPipe
    - intersection goto next

Intersection result of intersection point and normal, plus geometry information of
intersected primitive.

The material shader is executed this will either:
(1) Do nothing
(2) Ammend data to the frame compositor.
(3) Spawn more rays in the raypool if required.


The shader que is constructed with the required information includiung the intersection
state.

So when a shader is executed is will add the contributions from the intersection and resultant shade. If the ray pool is big engouh a chain can be traversed to calculate
bdpt.





Each starts with a payload. This payload is relevant to the framebuffer compositor
although not directly.

When a ray is spawned the payload is set by the shader and is const.

Accumulation and contribution calculation (composition) can be done by a seperate
shader???

So we have a material shader, defined on a per primitive (or globally set per geometry)
basis which implements the behavior of the interaction within the photometric domain
(what it looks like). This shader can both spawn (rays) and accumulate (to the frame buffer). Similar to tradiational raytracing.

And we can also utilise a path tracer kernel. This is executed for each geometry intersection hit and can both spawn and accumulate, however there is a distinction between this and the material since the shader is designed to proivde 'content' for the simulation while a pt kernel provides 'means' to do this. It also means the same material shaders may be used with different.


From shader point of view...

void MaterialShader(const POSE* intersection, intersected geometry)
{
    // read the payload from the ray pose

    // perform the necessaril accumulation

    // spawn the necessary rayss


}

void PathKernel(const POSE* intersection)
{
    
    // Read the pose, then we can use this information
    // to retireve all other nodes of this path for processing.

    // for each node we can find out previous paths and
    // calculate contributions in each case.

}





Payloads need to be defined and declared by client. One for each ray
and one for each pixel of the frame buffer.

The shader needs to know the type, to be able to read ray payload and manipulate
the resultant pixel in the framebuffer. The framebuffer is guarded by a compositor
which can strategically composite the resultant payload.

class context
{
    // rays
    // payloads
    // shader que

    // compiled and loaded shaders.
    // compiled scenes
};


// Framebuffer by default contains nothing, client is reposinsible for
// inheriting from it and containing the required buffer info. Also needs
// to have the compositor (which receives accumulations from shaders/kernels)
// and then needs to populate directly to resultant buffer. 

class framebuffer
{
    // payloads (one for each pixel)
    // delegate for compositor...

    

    
};

// so we need a context which holds the caches..

// shaders would get passed the pose (which has pointer to item).

// framebuffer optionally contains resultant data... it gets called
// with the result....


The context is created and sets up the runtime.

Context is responsible for rendering.
Context is responsible for scene tree management.
Context is responsible for shader management.
This includes kernel, shader and fragment programs?

kerenel program is simple non-state lambda which can spawn rays, calculate
fragments and accumulate.

shader is a simple non-state lambda which can spawn rays and accumulate.

fragment is a state-able function which process an accumulation for a fragment.

Context
    CreateShader(std::function)
    CreateShaderCL(std::string source)
    CreateShaderCUDA(std::string source) etc...

    // Render routine/pipe.
    Render(frame, cameratransform, scene);
 



Rendering is done like this.

 First ensure enough primary rays are set in relation to the size of the framebuffer

 Set the camera initial ray positions in pool.

 Reset the fragments of the frame image.

 Loop for each pool chunk.
 {
    prepare pose for chunk number of rays.
        Copy this to romulous.
        Invoke romulous.
        Read results of intersections and set next free rays with POSE
    
    invoke path kernel for each ray in chunk.
        spawn rays
        ammend current rays.

    shader invocation list.
    {
        Create a list of the shaders to invoke with the ptrs to the correct state.

        Invoke the shaders.

        This invokcation will either accumulate or create more rays

        Put the spawned rays into romuluous.
    }
        
    finalise chunk.

    Copy new spawns to next free rays in pool.
    Replay fragment calls to frame.

 }











RenderPipe....

One large array with everything. This is split into chunks for processing...

For each chunk:
{
    romulous is the copy of the PXYZ, NXYZ, UV to perform processing on.

    The results is a list of shaders to invoke with the primitives they have intersected...

    reamus holds lots of information. And the states.

    This is invoked CL styley. The results is a list of rays to spawn (this could be larger than the chunk).

    These results are put into the next chunk.
}

