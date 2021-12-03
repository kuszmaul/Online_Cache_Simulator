#ifndef RAM_H_GUARD
#define RAM_H_GUARD

#include <list>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "params.h"
#include "OSTree.h"

typedef uint64_t (*hash_func_t)(const void *, size_t, uint64_t);

// This class represents a single physical page 
class Page {
private:
	uint64_t last_accessed = 0;
	uint64_t virtual_addr = 0;
	uint64_t physical_addr;
	bool free  = true;
public:
	Page(uint64_t phy) : physical_addr(phy) {}

	uint64_t inline get_phys() {
		return physical_addr;
	}
	uint64_t inline get_virt() {
		return virtual_addr;
	}
	uint64_t inline last_touched() {
		return last_accessed;
	}
	void inline place_page(uint64_t v_addr, uint64_t timestamp) {
		virtual_addr = v_addr;
		last_accessed = timestamp;
		free = false;
	}
	void inline evict_page() {
		virtual_addr = 0;
		free = true;
	}
	void inline access_page(uint64_t timestamp) {
		last_accessed = timestamp;
	}
	bool inline is_free() {
		return free;
	}
};

class RAM {
protected:
	uint64_t memory_size;
	uint32_t page_size;
	uint32_t page_faults = 0;
	uint64_t access_number = 0;
public:
	RAM(uint64_t size, uint32_t p_size): memory_size(size), page_size(p_size) {}
	virtual ~RAM() {};
	virtual bool memory_access(uint64_t virtual_addr) = 0;
	uint64_t inline get_memory_size() {return memory_size;}
	uint32_t inline get_page_size() {return page_size;}
	void print() {
		printf("number of page_faults %u memory size %llu page size %u\n",
			page_faults, memory_size, page_size);
	}
	uint32_t get_faults() {
		return page_faults;
	}
};

/*
 * An LRU_Size_Simulation simulates LRU running on every possible
 * memory size from 1 to MEM_SIZE.
 * Returns a success function which gives the number of page faults
 * for every memory size.
 */
class LRU_Size_Simulation : public RAM {
protected:
	std::list<Page *> free_pages;
	std::vector<Page *> memory;
	std::vector<uint64_t> page_faults;
	uint32_t num_pages;
	OSTreeHead LRU_queue;
	std::unordered_map<uint64_t, Page *> page_table;
public:
	LRU_Size_Simulation(uint64_t size, uint32_t p_size);
	~LRU_Size_Simulation();
	bool memory_access(uint64_t virtual_addr);
	Page *evict_oldest();
	size_t moveFrontQueue(uint64_t curts, uint64_t newts);
	void printSuccessFunction();
	std::vector<uint64_t> getSuccessFunction();
};

/*
 * This simulator is fed a sequence of memory access and a box profile
 * and simulates LRU across the box profile.
 */
class LRU_BOX_Simulation : public LRU_Size_Simulation {
protected:
	size_t box_size_bound;     // maximum pages that can fit in this box
	size_t pages_present  = 0; // how many pages in the current box have pages in them
public:
	LRU_BOX_Simulation(uint64_t size, uint32_t p_size, uint32_t min_box, uint32_t max_box);
	~LRU_BOX_Simulation();
	bool memory_access(uint64_t virtual_addr); 
	void set_box_size(size_t size);
};

#endif
