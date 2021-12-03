
LRU_BOX_Simulation::LRU_BOX_Simulation(uint64_t size, uint32_t p_size, uint32_t min_box, uint32_t max_box) 
: LRU_Size_Simulation(size, p_size) {
	// now setup boxes stuff
	box_size_bound = min_box;
	pages_present  = 0;
}

void LRU_BOX_Simulation::memory_access(uint64_t virtual_addr) {
	uint64_t ts = access_number++;

	if (page_table.count(virtual_addr) > 0 && page_table[virtual_addr]->get_virt() == virtual_addr) {
		page_faults[moveFrontQueue(page_table[virtual_addr]->last_touched(), ts)]++; // move this page to the front of the LRU queue
		page_table[virtual_addr]->access_page(ts); // this is a memory access to the page stored in memory (it should update timestamp)
	} else {
		// if there are free pages then use one of them
		page_faults[num_pages]++;
		Page *p;
		if (free_pages.size() > 0) {
			p = free_pages.front();
			free_pages.pop_front();
		} else {
			p = evict_oldest();
			//page_table.erase(p->get_virt());
		}
		p->place_page(virtual_addr, ts);
		page_table[p->get_virt()] = p;
		LRU_queue.insert(ts, p->get_virt());
		assert(p->get_virt() ==  virtual_addr);
		//page_faults++;
	}
}

void LRU_BOX_Simulation::set_box_size(size_t new_size) {
	// reducing the size of memory kicks out the associated pages
	if (new_size < box_size_bound)
		pages_present = new_size;

	box_size_bound = new_size;
}
