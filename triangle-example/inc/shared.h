#ifndef SHARED_H
#define SHARED_H

/****************************************************************************
 *
 * DIFFERENCE, or a quick demo of loading images from a file. Uses the STB
 *  library (https://github.com/nothings/stb).
 *
 * Author: Haysn Hornbeck.
 *
 */

// ***** INCLUDES

#include <cmath>
using std::fabs;
using std::isnan;
using std::log10;
using std::sqrt;

#include <cstddef>
using std::size_t;

#include <unistd.h>	
// usleep


#include <algorithm>
using std::sort;

#include <atomic>
using std::atomic;

#include <chrono>
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::steady_clock;
using std::chrono::time_point;
using seconds = duration<float>;

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <limits>
using std::numeric_limits;

#include <memory>
using std::make_shared;
using std::shared_ptr;

#include <mutex>
using std::mutex;

#include <stdexcept>
using std::out_of_range;

#include <string>
using std::string;

#include <thread>
using std::thread;

#include <vector>
using std::vector;

// handle STB in a source file, not here


// ***** DATA DECLARATIONS

typedef unsigned char uchar;
typedef unsigned int uint;

// helpful for representing results
typedef struct DR {

	uint x;				// necessary for sorting
	uint y;

	float progress = 0.;
	double psnr;			// so we can be algorithmically lazy
	double mae;
	double rmse;

	// let it be sortable
	bool operator<( const DR& other ) const;

	} DiffResult;

// ***** CLASSES

// represent a pixel
class Pixel {
				// the components contained within
	vector<float> c;
	float maxAll;

	public:
				// must fix the number of components from the get-go
	Pixel( uchar components );

	float r() const;	// handy shortcuts
	float g() const;
	float b() const;
	
	bool r( const float& );
	bool g( const float& );
	bool b( const float& );

				// workhorse functions
	bool set( const uchar index, const float& value );
	float get( const uchar index ) const;

	ulong pixels() const;	// how many pixels?
	uint width() const;	//  and so on...
	uint height() const;
	uchar channels() const;

	float max() const;	// the maximal value

	// no square brackets, so we can properly update the max value
	
	};

// represent a scanline
class Scanline {

	uint w = 0;		// width of the scanline
	uint c = 0;		// # of components

	vector<Pixel> data;	// store data

	public:
				// must fix these from the get-go
	Scanline( uint width, uchar components );

	ulong pixels() const;	// how many pixels?
	uint width() const;	//  and so on...
	uint height() const;
	uchar channels() const;

	float max() const;	// the maximal value

				// access via square brackets; throws exceptions!
	Pixel& operator[]( size_t index );
	const Pixel& operator[]( size_t index ) const;
	
	};

// represent an image
class Image {

	uint w = 0;		// width, height, you get it
	uint h = 0;
	uint c = 0;

	vector<Scanline> data;	// store data

	public:
				// must fix these from the get-go
	Image( uint width, uint height, uchar components );

	ulong pixels() const;	// how many pixels?
	uint width() const;	//  and so on...
	uint height() const;
	uchar channels() const;

	float max() const;	// the maximal value

				// access via square brackets; throws exceptions!
	Scanline& operator[]( size_t index );
	const Scanline& operator[]( size_t index ) const;

	};

// A java-ish container for program code
class Difference {

	static vector<shared_ptr<Image>> images;	// allow multiple comparisons
	static vector<DiffResult> state;		// what are the results?
	static mutex dataLock;				// protect the above
	
	static atomic<int> activeThreads;		// how many loading routines are running?

	int linearize( uint x, uint y );		// turn this into a linear index

	static const double maxPrecisLoss;	// how much precision are we willing to lose?

	public:
						// the ACTUAL main routine
	int run( const int argc, const char** argv );

						// load the given image
	static void loadImage( const char*, uint index );

						// calculate the metrics
	static void calcMetrics( uint x, uint y );

	};

// ***** STATIC FUNCTIONS

int main( const int argc, const char** argv );		// the main routine

#endif
