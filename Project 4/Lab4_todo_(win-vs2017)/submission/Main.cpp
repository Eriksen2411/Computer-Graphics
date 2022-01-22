//============================================================
// STUDENT NAME: Nguyen Duc Danh
// NUS User ID.: e0407670
// COMMENTS TO GRADER:
//
// ============================================================

#include "Util.h"
#include "Vector3d.h"
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Scene.h"
#include "Raytrace.h"
#include <string>


// Constants for Scene 1.
static constexpr int imageWidth1 = 640;
static constexpr int imageHeight1 = 480;
static constexpr int reflectLevels1 = 0;  // 0 -- object does not reflect scene.
static constexpr int hasShadow1 = true;
static constexpr std::string_view outImageFile1 = "out1.png";

// Constants for Scene 2.
static constexpr int imageWidth2 = 640;
static constexpr int imageHeight2 = 480;
static constexpr int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static constexpr int hasShadow2 = true;
static constexpr std::string_view outImageFile2 = "out2.png";



///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage( const std::string &imageFilename, const Scene &scene, 
                  int reflectLevels, bool hasShadow )
{
    int imgWidth = scene.camera.getImageWidth();
    int imgHeight = scene.camera.getImageHeight();

    Image image( imgWidth, imgHeight ); // To store the result of ray tracing.

    double startTime = Util::GetCurrRealTime();
    double startCPUTime = Util::GetCurrCPUTime();

    // Generate image, rendering in parallel on Windows and Linux.
    #ifndef __APPLE__
    #pragma warning( push )
    #pragma warning( disable : 6993 )
    #pragma omp parallel for
    #endif
    for ( int y = 0; y < imgHeight; y++ )
    {
        double pixelPosY = y + 0.5;

        for ( int x = 0; x < imgWidth; x++ )
        {
            double pixelPosX = x + 0.5;
            Ray ray = scene.camera.getRay( pixelPosX, pixelPosY );
            Color pixelColor = Raytrace::TraceRay( ray, scene, reflectLevels, hasShadow );
            pixelColor.clamp();
            image.setPixel( x, y, pixelColor );
        }
    }
    #ifndef __APPLE__
    #pragma warning( pop )
    #endif

    double cpuTimeElapsed = Util::GetCurrCPUTime() - startCPUTime;
    double realTimeElapsed = Util::GetCurrRealTime() - startTime;
    std::cout << "CPU time taken = " << cpuTimeElapsed << "sec" << std::endl;
    std::cout << "Real time taken = " << realTimeElapsed << "sec" << std::endl;

    // Write image to file.
    if ( !image.writeToFile( imageFilename ) ) return;
    else Util::ErrorExit("File: %s could not be written.\n", imageFilename.c_str() );
}



// Forward declarations. These functions are defined later in the file.
void DefineScene1( Scene &scene, int imageWidth, int imageHeight );
void DefineScene2( Scene &scene, int imageWidth, int imageHeight );



int main()
{
// Define Scene 1.

    Scene scene1;
    DefineScene1( scene1, imageWidth1, imageHeight1 );

// Render Scene 1.

    std::cout << "Render Scene 1..." << std::endl;
    RenderImage( std::string(outImageFile1), scene1, reflectLevels1, hasShadow1 );
    std::cout << "Scene 1 completed." << std::endl;

// Delete Scene 1 surfaces.

    for (auto& surface : scene1.surfaces)
    {
        delete surface;
    }


// Define Scene 2.

    Scene scene2;
    DefineScene2( scene2, imageWidth2, imageHeight2 );

// Render Scene 2.

    std::cout << "Render Scene 2..." << std::endl;
    RenderImage( std::string(outImageFile2), scene2, reflectLevels2, hasShadow2 );
    std::cout << "Scene 2 completed." << std::endl;

// Delete Scene 2 surfaces.

    for (auto& surface : scene2.surfaces)
    {
        delete surface;
    }


    std::cout << "All done. Press Enter to exit." << std::endl;
    std::cin.get();
    return 0;
}



///////////////////////////////////////////////////////////////////////////
// Modelling of Scene 1.
///////////////////////////////////////////////////////////////////////////

void DefineScene1( Scene &scene, int imageWidth, int imageHeight )
{
    scene.backgroundColor = Color( 0.2f, 0.3f, 0.5f );

    scene.amLight.I_a = Color( 1.0f, 1.0f, 1.0f ) * 0.25f;

// Define materials.

    // Light red.
    Material lightRed = Material();
    lightRed.k_d = Color( 0.8f, 0.4f, 0.4f );
    lightRed.k_a = lightRed.k_d;
    lightRed.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    lightRed.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    lightRed.n = 64.0f;

    // Light green.
    Material lightGreen = Material();
    lightGreen.k_d = Color( 0.4f, 0.8f, 0.4f );
    lightGreen.k_a = lightGreen.k_d;
    lightGreen.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    lightGreen.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    lightGreen.n = 64.0f;

    // Light blue.
    Material lightBlue = Material();
    lightBlue.k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
    lightBlue.k_a = lightBlue.k_d;
    lightBlue.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    lightBlue.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
    lightBlue.n = 64.0f;

    // Yellow.
    Material yellow = Material();
    yellow.k_d = Color( 0.6f, 0.6f, 0.2f );
    yellow.k_a = yellow.k_d;
    yellow.k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
    yellow.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    yellow.n = 64.0f;

    // Gray.
    Material gray = Material();
    gray.k_d = Color( 0.6f, 0.6f, 0.6f );
    gray.k_a = gray.k_d;
    gray.k_r = Color( 0.6f, 0.6f, 0.6f );
    gray.k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
    gray.n = 128.0f;

    // Insert into scene materials vector.
    scene.materials = { lightRed, lightGreen, lightBlue, yellow, gray };


// Define point light sources.

    scene.ptLights.resize(2);

    PointLightSource light0 = { Vector3d(100.0, 120.0, 10.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };
    PointLightSource light1 = { Vector3d(5.0, 80.0, 60.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };

    scene.ptLights = { light0, light1 };


// Define surface primitives.

    scene.surfaces.resize(15);

    auto horzPlane = new Plane( 0.0, 1.0, 0.0, 0.0, scene.materials[2] ); // Horizontal plane.
    auto leftVertPlane = new Plane( 1.0, 0.0, 0.0, 0.0, scene.materials[4] ); // Left vertical plane.
    auto rightVertPlane = new Plane( 0.0, 0.0, 1.0, 0.0, scene.materials[4] ); // Right vertical plane.
    auto bigSphere =  new Sphere( Vector3d( 40.0, 20.0, 42.0 ), 22.0, scene.materials[0] ); // Big sphere.
    auto smallSphere = new Sphere( Vector3d( 75.0, 10.0, 40.0 ), 12.0, scene.materials[1] ); // Small sphere.

    // Cube +y face.
    auto cubePosYTri1 = new Triangle( Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      scene.materials[3] );
    auto cubePosYTri2 = new Triangle( Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      scene.materials[3] );

    // Cube +x face.
    auto cubePosXTri1 = new Triangle( Vector3d( 50.0, 0.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 90.0 ),
                                      scene.materials[3]);
    auto cubePosXTri2 = new Triangle( Vector3d( 50.0, 0.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 50.0, 0.0, 90.0 ),
                                      scene.materials[3] );

    // Cube -x face.
    auto cubeNegXTri1 = new Triangle( Vector3d( 30.0, 0.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      scene.materials[3]);
    auto cubeNegXTri2 = new Triangle( Vector3d( 30.0, 0.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      Vector3d( 30.0, 0.0, 70.0 ),
                                      scene.materials[3] );

    // Cube +z face.
    auto cubePosZTri1 = new Triangle( Vector3d( 50.0, 0.0, 90.0 ),
                                      Vector3d( 50.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      scene.materials[3]);
    auto cubePosZTri2 = new Triangle( Vector3d( 50.0, 0.0, 90.0 ),
                                      Vector3d( 30.0, 20.0, 90.0 ),
                                      Vector3d( 30.0, 0.0, 90.0 ),
                                      scene.materials[3] );

    // Cube -z face.
    auto cubeNegZTri1 = new Triangle( Vector3d( 30.0, 0.0, 70.0 ),
                                      Vector3d( 30.0, 20.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      scene.materials[3] );
    auto cubeNegZTri2 = new Triangle( Vector3d( 30.0, 0.0, 70.0 ),
                                      Vector3d( 50.0, 20.0, 70.0 ),
                                      Vector3d( 50.0, 0.0, 70.0 ),
                                      scene.materials[3] );

    scene.surfaces = { horzPlane, leftVertPlane, rightVertPlane, 
                       bigSphere, smallSphere,
                       cubePosXTri1, cubePosXTri2,
                       cubePosYTri1, cubePosYTri2,
                       cubePosZTri1, cubePosZTri2,
                       cubeNegXTri1, cubeNegXTri2,
                       cubeNegZTri1, cubeNegZTri2 };


// Define camera.

    scene.camera = Camera( Vector3d( 150.0, 120.0, 150.0 ),  // eye
                           Vector3d( 45.0, 22.0, 55.0 ),  // lookAt
                           Vector3d( 0.0, 1.0, 0.0 ),  //upVector
                           (-1.0 * imageWidth) / imageHeight,  // left
                           (1.0 * imageWidth) / imageHeight,  // right
                           -1.0, 1.0, 3.0,  // bottom, top, near
                           imageWidth, imageHeight );  // image_width, image_height
}



///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////

void DefineScene2( Scene &scene, int imageWidth, int imageHeight )
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

    scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

    // Define materials.

    // Light Orange.
    Material Orange = Material();
    Orange.k_d = Color(1.0f, 0.537f, 0.188f);
    Orange.k_a = Orange.k_d;
    Orange.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    Orange.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    Orange.n = 64.0f;

    // Razzmatazz.
    Material Razzmatazz = Material();
    Razzmatazz.k_d = Color(0.933f, 0.106f, 0.325f);
    Razzmatazz.k_a = Razzmatazz.k_d;
    Razzmatazz.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    Razzmatazz.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    Razzmatazz.n = 64.0f;

    Material Cerise = Material();
    Cerise.k_d = Color(0.831f, 0.125f, 0.584f);
    Cerise.k_a = Cerise.k_d;
    Cerise.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    Cerise.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    Cerise.n = 64.0f;

    // Light Turquoise.
    Material Turquoise = Material();
    Turquoise.k_d = Color(0.145f, 0.894f, 0.894f);
    Turquoise.k_a = Turquoise.k_d;
    Turquoise.k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
    Turquoise.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    Turquoise.n = 64.0f;

    // Yellow.
    Material Genoa = Material();
    Genoa.k_d = Color(0.6f, 0.6f, 0.2f);
    Genoa.k_a = Genoa.k_d;
    Genoa.k_r = Color(0.6f, 0.6f, 0.6f);
    Genoa.k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
    Genoa.n = 128.0f;


    // Insert into scene materials vector.
    scene.materials = { Genoa, Turquoise, Orange, Razzmatazz, Cerise };


    // Define point light sources.

    scene.ptLights.resize(1);

    PointLightSource light0 = { Vector3d(200.0, 200.0, 400.0), Color(1.0f, 1.0f, 1.0f) * 0.6f };

    scene.ptLights = { light0 };


    // Define surface primitives.

    scene.surfaces.resize(36);

    auto horzPlane = new Plane(0.0, 1.0, 0.0, 0.0, scene.materials[0]); // Horizontal plane.
    auto leftVertPlane = new Plane(1.0, 0.0, 0.0, 0.0, scene.materials[1]); // Left vertical plane.
    auto rightVertPlane = new Plane(0.0, 0.0, 1.0, 0.0, scene.materials[1]); // Right vertical plane.

    auto bigSphere = new Sphere(Vector3d(60.0, 35.0, 60.0), 30.0, scene.materials[2]); // Big sphere.

    // Corner 1 (30, 65 30)
    auto Corner1Tri1 = new Triangle(Vector3d(30.0, 65.0, 30.0), Vector3d(45.0, 65.0, 30.0), Vector3d(30.0, 65.0, 45.0), scene.materials[3]);
    auto Corner1Tri2 = new Triangle(Vector3d(30.0, 65.0, 30.0), Vector3d(30.0, 50.0, 30.0), Vector3d(45.0, 65.0, 30.0), scene.materials[3]);
    auto Corner1Tri3 = new Triangle(Vector3d(30.0, 65.0, 30.0), Vector3d(30.0, 50.0, 30.0), Vector3d(30.0, 65.0, 45.0), scene.materials[3]);

    auto Corner1Tri4 = new Triangle(Vector3d(30.0, 50.0, 30.0), Vector3d(45.0, 65.0, 30.0), Vector3d(30.0, 65.0, 45.0), scene.materials[4]);

    // Corner 2 (90, 65, 30)
    auto Corner2Tri1 = new Triangle(Vector3d(90.0, 65.0, 30.0), Vector3d(75.0, 65.0, 30.0), Vector3d(90.0, 65.0, 45.0), scene.materials[3]);
    auto Corner2Tri2 = new Triangle(Vector3d(90.0, 65.0, 30.0), Vector3d(90.0, 50.0, 30.0), Vector3d(75.0, 65.0, 30.0), scene.materials[3]);
    auto Corner2Tri3 = new Triangle(Vector3d(90.0, 65.0, 30.0), Vector3d(90.0, 50.0, 30.0), Vector3d(90.0, 65.0, 45.0), scene.materials[3]);

    auto Corner2Tri4 = new Triangle(Vector3d(90.0, 50.0, 30.0), Vector3d(75.0, 65.0, 30.0), Vector3d(90.0, 65.0, 45.0), scene.materials[4]);

    // Corner 3 (30, 65, 90)
    auto Corner3Tri1 = new Triangle(Vector3d(30.0, 65.0, 90.0), Vector3d(30.0, 65.0, 75.0), Vector3d(45.0, 65.0, 90.0), scene.materials[3]);
    auto Corner3Tri2 = new Triangle(Vector3d(30.0, 65.0, 90.0), Vector3d(30.0, 50.0, 90.0), Vector3d(45.0, 65.0, 90.0), scene.materials[3]);
    auto Corner3Tri3 = new Triangle(Vector3d(30.0, 65.0, 90.0), Vector3d(30.0, 50.0, 90.0), Vector3d(30.0, 65.0, 75.0), scene.materials[3]);

    auto Corner3Tri4 = new Triangle(Vector3d(30.0, 50.0, 90.0), Vector3d(30.0, 65.0, 75.0), Vector3d(45.0, 65.0, 90.0), scene.materials[4]);

    // Corner 4 (90, 65, 90)
    auto Corner4Tri1 = new Triangle(Vector3d(90.0, 65.0, 90.0), Vector3d(75.0, 65.0, 90.0), Vector3d(90.0, 65.0, 75.0), scene.materials[3]);
    auto Corner4Tri2 = new Triangle(Vector3d(90.0, 65.0, 90.0), Vector3d(90.0, 50.0, 90.0), Vector3d(75.0, 65.0, 90.0), scene.materials[3]);
    auto Corner4Tri3 = new Triangle(Vector3d(90.0, 65.0, 90.0), Vector3d(90.0, 50.0, 90.0), Vector3d(90.0, 65.0, 75.0), scene.materials[3]);

    auto Corner4Tri4 = new Triangle(Vector3d(90.0, 50.0, 90.0), Vector3d(75.0, 65.0, 90.0), Vector3d(90.0, 65.0, 75.0), scene.materials[4]);

    // Corner 5 (30, 5, 30)
    auto Corner5Tri1 = new Triangle(Vector3d(30.0, 5.0, 30.0), Vector3d(45.0, 5.0, 30.0), Vector3d(30.0, 5.0, 45.0), scene.materials[3]);
    auto Corner5Tri2 = new Triangle(Vector3d(30.0, 5.0, 30.0), Vector3d(30.0, 20.0, 30.0), Vector3d(45.0, 5.0, 30.0), scene.materials[3]);
    auto Corner5Tri3 = new Triangle(Vector3d(30.0, 5.0, 30.0), Vector3d(30.0, 20.0, 30.0), Vector3d(30.0, 5.0, 45.0), scene.materials[3]);

    auto Corner5Tri4 = new Triangle(Vector3d(30.0, 20.0, 30.0), Vector3d(45.0, 5.0, 30.0), Vector3d(30.0, 5.0, 45.0), scene.materials[4]);

    // Corner 6 (90, 5, 30)
    auto Corner6Tri1 = new Triangle(Vector3d(90.0, 5.0, 30.0), Vector3d(75.0, 5.0, 30.0), Vector3d(90.0, 5.0, 45.0), scene.materials[3]);
    auto Corner6Tri2 = new Triangle(Vector3d(90.0, 5.0, 30.0), Vector3d(90.0, 20.0, 30.0), Vector3d(75.0, 5.0, 30.0), scene.materials[3]);
    auto Corner6Tri3 = new Triangle(Vector3d(90.0, 5.0, 30.0), Vector3d(90.0, 20.0, 30.0), Vector3d(90.0, 5.0, 45.0), scene.materials[3]);

    auto Corner6Tri4 = new Triangle(Vector3d(90.0, 20.0, 30.0), Vector3d(75.0, 5.0, 30.0), Vector3d(90.0, 5.0, 45.0), scene.materials[4]);

    // Corner 7 (30, 5, 90)
    auto Corner7Tri1 = new Triangle(Vector3d(30.0, 5.0, 90.0), Vector3d(45.0, 5.0, 90.0), Vector3d(30.0, 5.0, 75.0), scene.materials[3]);
    auto Corner7Tri2 = new Triangle(Vector3d(30.0, 5.0, 90.0), Vector3d(30.0, 20.0, 90.0), Vector3d(45.0, 5.0, 90.0), scene.materials[3]);
    auto Corner7Tri3 = new Triangle(Vector3d(30.0, 5.0, 90.0), Vector3d(30.0, 20.0, 90.0), Vector3d(30.0, 5.0, 75.0), scene.materials[3]);

    auto Corner7Tri4 = new Triangle(Vector3d(30.0, 20.0, 90.0), Vector3d(45.0, 5.0, 90.0), Vector3d(30.0, 5.0, 75.0), scene.materials[4]);

    // Corner 8 (90, 5, 90)
    auto Corner8Tri1 = new Triangle(Vector3d(90.0, 5.0, 90.0), Vector3d(75.0, 5.0, 90.0), Vector3d(90.0, 5.0, 75.0), scene.materials[3]);
    auto Corner8Tri2 = new Triangle(Vector3d(90.0, 5.0, 90.0), Vector3d(90.0, 20.0, 90.0), Vector3d(75.0, 5.0, 90.0), scene.materials[3]);
    auto Corner8Tri3 = new Triangle(Vector3d(90.0, 5.0, 90.0), Vector3d(90.0, 20.0, 90.0), Vector3d(90.0, 5.0, 75.0), scene.materials[3]);

    auto Corner8Tri4 = new Triangle(Vector3d(90.0, 10.0, 90.0), Vector3d(75.0, 5.0, 90.0), Vector3d(90.0, 5.0, 75.0), scene.materials[4]);

    scene.surfaces = { horzPlane, leftVertPlane, rightVertPlane,bigSphere,
                       Corner1Tri1, Corner1Tri2, Corner1Tri3, Corner1Tri4, Corner2Tri1, Corner2Tri2, Corner2Tri3, Corner2Tri4,
                       Corner3Tri1, Corner3Tri2, Corner3Tri3, Corner3Tri4, Corner4Tri1, Corner4Tri2, Corner4Tri3, Corner4Tri4,
                       Corner5Tri1, Corner5Tri2, Corner5Tri3, Corner5Tri4, Corner6Tri1, Corner6Tri2, Corner6Tri3, Corner6Tri4,
                       Corner7Tri1, Corner7Tri2, Corner7Tri3, Corner7Tri4, Corner8Tri1, Corner8Tri2, Corner8Tri3, Corner8Tri4 };


    // Define camera.

    scene.camera = Camera(Vector3d(175.0, 150.0, 220.0),  // eye
        Vector3d(60.0, 35.0, 60.0),  // lookAt
        Vector3d(0.0, 1.0, 0.0),  //upVector
        (-1.0 * imageWidth) / imageHeight,  // left
        (1.0 * imageWidth) / imageHeight,  // right
        -1.0, 1.0, 3.0,  // bottom, top, near
        imageWidth, imageHeight);  // image_width, image_height
}
