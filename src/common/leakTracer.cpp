/* 
 * Homepage: <http://www.andreasen.org/LeakTracer/>
 *
 * Authors:
 *  Erwin S. Andreasen <erwin@andreasen.org>
 *  Henner Zeller <H.Zeller@acm.org>
 *
 * This program is Public Domain
 *
 * Modified by Christopher Olsen for use at Stolen Notebook
 * - added file and line tracing for C++ new and delete
 * - modified file writing to allow multiple libraries to write
 *   to the same log
 */
#ifdef THREAD_SAVE
#define _THREAD_SAVE
#include <pthread.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>


/*
 * underlying allocation, de-allocation used within 
 * this tool
 */
#define LT_MALLOC  malloc
#define LT_FREE    free
#define LT_REALLOC realloc

/*
 * prime number for the address lookup hash table.
 * if you have _really_ many memory allocations, use a
 * higher value, like 343051 for instance.
 */
#define SOME_PRIME 35323
#define ADDR_HASH(addr) ((unsigned int) addr % SOME_PRIME)

/**
 * allocate a bit more memory in order to check if there is a memory
 * overwrite. Either 0 or more than sizeof(unsigned int). Note, you can
 * only detect memory over_write_, not _reading_ beyond the boundaries. Better
 * use electric fence for these kind of bugs 
 *   <ftp://ftp.perens.com/pub/ElectricFence/>
 */
#define MAGIC "\xAA\xBB\xCC\xDD"
#define MAGIC_SIZE (sizeof(MAGIC)-1)

/**
 * on 'new', initialize the memory with this value.
 * if not defined - uninitialized. This is very helpful because
 * it detects if you initialize your classes correctly .. if not,
 * this helps you faster to get the segmentation fault you're 
 * implicitly asking for :-). 
 *
 * Set this to some value which is likely to produce a
 * segmentation fault on your platform.
 */
#define SAVEVALUE   0x00

/**
 * on 'delete', clean memory with this value.
 * if not defined - no memory clean.
 *
 * Set this to some value which is likely to produce a
 * segmentation fault on your platform.
 */
#define MEMCLEAN    0xEE

/**
 * Initial Number of memory allocations in our list.
 * Doubles for each re-allocation.
 */
#define INITIALSIZE 32768


/**
 * Log filename
 */
#define LOGFILE   "leak.log"

static class LeakTracer {
	struct Leak {
		const void *addr;
		size_t      size;
		const void *allocAddr;
		bool        type;
		int         nextBucket;
    const char* file;
    int         line;
	};

	int  newCount;      // how many memory blocks do we have
	int  leaksCount;    // amount of entries in the leaks array
	int  firstFreeSpot; // Where is the first free spot in the leaks array?
	int  currentAllocated; // currentAllocatedMemory
	int  maxAllocated;     // maximum Allocated
	unsigned long totalAllocations; // total number of allocations. stats.
	unsigned int  abortOn;  // resons to abort program (see abortReason_t)

	/**
	 * Have we been initialized yet?  We depend on this being
	 * false before constructor has been called!  
	 */
	bool initialized;	
	bool destroyed;		// Has our destructor been called?


	/**
	 * pre-allocated array of leak info structs.
	 */
	Leak *leaks;

	/**
	 * fast hash to lookup the spot where an allocation is 
	 * stored in case of an delete. map<void*,index-in-leaks-array>
	 */
	int  *leakHash;     // fast lookup

#ifdef THREAD_SAVE
	pthread_mutex_t mutex;
#endif

	enum abortReason_t {
		OVERWRITE_MEMORY    = 0x01,
		DELETE_NONEXISTENT  = 0x02,
		NEW_DELETE_MISMATCH = 0x04
	};

public:
	LeakTracer()
  {
		initialize();
    // destroy existing log
    remove(LOGFILE);
	}
	
	void initialize()
  {
		if (initialized)
			return;

		initialized = true;
		newCount = 0;
		leaksCount = 0;
		firstFreeSpot = 1; // index '0' is special
		currentAllocated = 0;
		maxAllocated = 0;
		totalAllocations = 0;
		abortOn =  OVERWRITE_MEMORY; // only _severe_ reason
		leaks = 0;
		leakHash = 0;

		char uniqFilename[256];
		const char *filename = "leak.log";
		struct stat dummy;
		if (stat(filename, &dummy) == 0) {
			fprintf(stderr, 
				"LeakTracer: file exists; using %s instead\n",
				uniqFilename);
		}
		else {
			sprintf(uniqFilename, "%s", filename);
		}

		leakHash = (int*) LT_MALLOC(SOME_PRIME * sizeof(int));
		memset ((void*) leakHash, 0x00, SOME_PRIME * sizeof(int));

#ifdef THREAD_SAVE
		/*
		 * create default, non-recursive ('fast') mutex
		 * to lock our datastructure where we keep track of
		 * the user's new/deletes
		 */
		if(pthread_mutex_init(&mutex, NULL) < 0)
			exit(1);
#endif
	}

  /*
	 * the workhorses:
	 */
	void *registerAlloc(size_t size, bool type, const char* file, int line);
	void  registerFree (void *p, bool type);

	/**
	 * write a hexdump of the given area.
	 */
	void  hexdump(const unsigned char* area, int size);
	
	/**
	 * Terminate current running progam.
	 */
	void progAbort(abortReason_t reason)
  {
		if (abortOn & reason)
    {
			writeLeakReport();
			abort();
		}
	}



  /**
	 * write a Report over leaks, e.g. still pending deletes
	 */
	void writeLeakReport();

	~LeakTracer()
  {
		writeLeakReport();
		free(leaks);
#ifdef THREAD_SAVE
		pthread_mutex_destroy(&mutex);
#endif
		destroyed = true;
	}
} leakTracer;






/**
   registerAlloc
*/
void* LeakTracer::registerAlloc(size_t size, bool type,
                                const char* file, int line)
{
	initialize();

	if(destroyed)
		return LT_MALLOC(size);


	void *p = LT_MALLOC(size + MAGIC_SIZE);
	// Need to call the new-handler
	if(!p)
		exit (1);

#ifdef SAVEVALUE
	/* initialize with some defined pattern */
	memset(p, SAVEVALUE, size + MAGIC_SIZE);
#endif
	
#ifdef MAGIC
	/*
	 * the magic value is a special pattern which does not need
	 * to be uniform.
	 */
  memcpy((char*)p+size, MAGIC, MAGIC_SIZE);
#endif

#ifdef THREAD_SAVE
	pthread_mutex_lock(&mutex);
#endif

	++newCount;
	++totalAllocations;
	currentAllocated += (int)size;
	if (currentAllocated > maxAllocated)
		maxAllocated = currentAllocated;
	
	for (;;) {
		for (int i = firstFreeSpot; i < leaksCount; i++)
			if (leaks[i].addr == NULL) {
				leaks[i].addr = p;
				leaks[i].size = size;
				leaks[i].type = type;
				leaks[i].allocAddr=0;
        leaks[i].file = file;
        leaks[i].line = line;
				firstFreeSpot = i+1;
				// allow to lookup our index fast.
				int *hashPos = &leakHash[ ADDR_HASH(p) ];
				leaks[i].nextBucket = *hashPos;
				*hashPos = i;
#ifdef THREAD_SAVE
				pthread_mutex_unlock(&mutex);
#endif
				return p;
			}
		
		// Allocate a bigger array
		// Note that leaksCount starts out at 0.
		int new_leaksCount = (leaksCount == 0) ? INITIALSIZE 
			                               : leaksCount * 2;
		leaks = (Leak*)LT_REALLOC(leaks, 
					  sizeof(Leak) * new_leaksCount);
		if (!leaks)
			exit(1);

		memset(leaks+leaksCount, 0x00,
		       sizeof(Leak) * (new_leaksCount-leaksCount));
		leaksCount = new_leaksCount;
	}
}



/**
   hexdump
*/
void LeakTracer::hexdump(const unsigned char* area, int size) {
	for (int j=0; j < size ; ++j) {
		if (j % 16 == 15) {
			for (int k=-15; k < 0 ; k++) {
				(char) *(area + j + k);
			}
		}
	}
}



/**
   registerFree
*/
void LeakTracer::registerFree (void *p, bool type) {
	initialize();

	if(p == NULL)
		return;

	if(destroyed)
  {
		fprintf(stderr, "Deallocation after destruction of LeakTracer (0x%p)\n", p);
		fprintf(stderr, "May be result of static destructors called after LeakTracer destructor\n", p);
                return;
	}

#ifdef THREAD_SAVE
	pthread_mutex_lock(&mutex);
#endif
	int *lastPointer = &leakHash[ADDR_HASH(p)];
	int i = *lastPointer;
  if(!i)
    return;

	while (i != 0 && leaks[i].addr != p)
  {
		lastPointer = &leaks[i].nextBucket;
		i = *lastPointer;
	}

	if (leaks[i].addr == p) {
		*lastPointer = leaks[i].nextBucket; // detach.
		newCount--;
		leaks[i].addr = NULL;
		currentAllocated -= (int)leaks[i].size;
		if (i < firstFreeSpot)
			firstFreeSpot = i;

		if (leaks[i].type != type)
			progAbort( NEW_DELETE_MISMATCH );

#ifdef MAGIC
		if (memcmp((char*)p + leaks[i].size, MAGIC, MAGIC_SIZE))
    {
			hexdump((unsigned char*)p+leaks[i].size,
				MAGIC_SIZE);
			progAbort( OVERWRITE_MEMORY );
		}
#endif

#ifdef THREAD_SAVE
#  ifdef MEMCLEAN
		int allocationSize = leaks[i].size;
#  endif
		pthread_mutex_unlock(&mutex);
#else
#define             allocationSize leaks[i].size
#endif

#ifdef MEMCLEAN
		// set it to some garbage value.
		memset((unsigned char*)p, MEMCLEAN, allocationSize + MAGIC_SIZE);
#endif
		LT_FREE(p);
		return;
	}

#ifdef THREAD_SAVE
	pthread_mutex_unlock(&mutex);
#endif
	progAbort( DELETE_NONEXISTENT );
}



/**
   writeLeakReport
*/
void LeakTracer::writeLeakReport()
{
	initialize();

  FILE* report = fopen("leak.log", "a+");
	if (newCount > 0) {
		fprintf(report, "# LeakReport\n");
		fprintf(report, "# %10s | %9s  # Pointer Addr\n",
			"from new @", "size");
	}
	for (int i = 0; i <  leaksCount; i++)
  {
		if (leaks[i].addr != NULL)
    {
			// This ought to be 64-bit safe?
      fprintf(report, "L %s: [%d]  %9ld  # %p\n",
        leaks[i].file,
        leaks[i].line,
				(long) leaks[i].size,
				leaks[i].addr);
		}
  }
	fprintf(report, "# total allocation requests: %6ld ; max. mem used"
		" %d kBytes\n", totalAllocations, maxAllocated / 1024);
	fprintf(report, "# leak %6d Bytes\n", currentAllocated);
	if (currentAllocated > 50 * 1024) {
		fprintf(report, "# .. that is %d kByte!! A lot ..\n", 
			currentAllocated / 1024);
	}
}

/** -- The actual new/delete operators -- **/

// NEW
void* operator new(unsigned int size)
{
  return leakTracer.registerAlloc(size,false, "unknown", 0);
}

void* operator new(unsigned int size, const char* file, int line)
{
	return leakTracer.registerAlloc(size,false, file, line);
}

void* operator new[](unsigned int size)
{
  return leakTracer.registerAlloc(size,false, "unknown", 0);
}

void* operator new[] (unsigned int size, const char* file, int line)
{
	return leakTracer.registerAlloc(size,true, file, line);
}


// DELETE
void operator delete(void* p)
{
	leakTracer.registerFree(p,false);
}

void operator delete (void *p, const char* file, int line)
{
	leakTracer.registerFree(p,false);
}


void operator delete[](void* p)
{
	leakTracer.registerFree(p,false);
}

void operator delete[] (void *p, const char* file, int line)
{
	leakTracer.registerFree(p,true);
}

/* Emacs: 
 * Local variables:
 * c-basic-offset: 8
 * End:
 * vi:set tabstop=8 shiftwidth=8 nowrap: 
 */
