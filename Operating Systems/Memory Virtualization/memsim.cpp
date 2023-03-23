#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct page_entry{
    unsigned v_addr;
    unsigned page_num;
    char op;
    int dirty_bit;
    int time;
};

int writes = 0, reads = 0, total_events = 0;

vector<page_entry*> tlb;
vector<page_entry*> buffer;

int find(page_entry* new_entry, vector<page_entry*> cache){
    for(int index = 0; index < cache.size(); index++){
        if(cache[index]->page_num == new_entry->page_num){
            return index;
            break;
        }
    }
    return -1;
}

/*************************** FIFO *******************************************************************************************************/

void fifo(page_entry* entry, int frames, char* mode){
    int found = find(entry, tlb);

    if(found != -1){
        if (entry->dirty_bit == 1)
        {
            tlb[found]->dirty_bit = 1;
            tlb[found]->op = entry->op;
        }
        if(strcmp(mode, "debug") == 0) cout << "FIFO TLB hit! Entry found in TLB!\n";
    }
    else{
        reads = reads + 1;

        if (tlb.size() == frames){
            if (tlb[0]->dirty_bit == 1)
                writes = writes + 1;

            tlb.erase(tlb.begin());
            tlb.push_back(entry);

            if(strcmp(mode, "debug") == 0) cout << "FIFO TLB miss! New entry added by eviction!\n";
        }
        else {
            tlb.push_back(entry);
            if(strcmp(mode, "debug") == 0) cout << "FIFO TLB miss! TLB has space! No eviction performed!\n";
        }
    }
}

/*************************** LRU *******************************************************************************************************/

void lru(page_entry* entry, int frames, char* mode){
    int found = find(entry, tlb);
    
    if(found != -1){   //page entry found in tlb                                            
        tlb[found]->time = entry->time;
        if (entry->dirty_bit == 1){
            tlb[found]->dirty_bit = 1;
            tlb[found]->op = entry->op;
        }
        if(strcmp(mode, "debug") == 0) cout << "LRU TLB hit! Entry found in TLB!\n";
    }
    else{ //if not present
        reads = reads + 1;

        if(tlb.size() == frames){   //if page table is full
            int to_evict = 0;
            int time_earliest = tlb[0]->time;
            for(int i = 0; i < tlb.size(); i++){         //search through table for smallest
                if(tlb[i]->time <= time_earliest){
                    time_earliest = tlb[i]->time;
                    to_evict = i;
                }
            }

            if (tlb[to_evict]->dirty_bit == 1) {
                writes = writes + 1;
            }

            tlb.erase(tlb.begin() + to_evict);     //erase lru element
            tlb.push_back(entry);  

            if(strcmp(mode, "debug") == 0) cout << "LRU TLB miss! New entry added by eviction!\n";   
        }
        else{ 
            tlb.push_back(entry);
            if(strcmp(mode, "debug") == 0) cout << "LRU TLB miss! TLB has space! No eviction performed!\n";
        }
    }
}

/************************ SEGMENTED-FIFO (VMS) *******************************************************************************************************/

void segmented_fifo(page_entry* entry, int frames, int size_buffer, char* mode){
    int tlb_size = frames - size_buffer;

    int found_tlb = find(entry, tlb);
    int found_buffer = find(entry, buffer);

    if(found_tlb == -1 && found_buffer == -1){ // not found in any of the buffers increment reads
        reads++;
        if(strcmp(mode, "debug") == 0) cout << "VMS miss! Entry not found in any of the buffers!\n";
    }

    if(found_tlb != -1){   //if present in primary, update info                                          
        tlb[found_tlb]->time = entry->time;
        tlb[found_tlb]->op = entry->op;
        if (entry->dirty_bit == 1){
            tlb[found_tlb]->dirty_bit = 1;
        }
        if(strcmp(mode, "debug") == 0) cout << "VMS TLB hit! Entry found in TLB!\n";
    }

    if(found_tlb == -1 && found_buffer == -1 && tlb.size() < tlb_size){ //if not found in primary and secondary and primary has space, add to primary
        tlb.push_back(entry);
        if(strcmp(mode, "debug") == 0) cout << "VMS miss! Entry added in primary!\n";
    }

    if(found_tlb == -1 && tlb.size() == tlb_size && found_buffer == -1 && buffer.size() < size_buffer){ //if not found in primary and secondary, add to primary while sending the last of primary to the secondary
        buffer.push_back(tlb[0]);
        tlb.erase(tlb.begin());
        tlb.push_back(entry);
    }

    if(found_tlb == -1 && tlb.size() == tlb_size && found_buffer == -1 && buffer.size() == size_buffer){ //if not found in primary and secondary, and both are full, evict from secondary and add to it and add to primary
        int to_evict = 0;
        int time_earliest = buffer[0]->time;
        for(int i = 0; i < buffer.size(); i++){
            if(buffer[i]->time <= time_earliest){
                time_earliest = buffer[i]->time;
                to_evict = i;
            }
        }
        if(buffer[to_evict]->dirty_bit == 1){
            writes = writes + 1; //before eviction from secondary increase writes
        }
        buffer.erase(buffer.begin() + to_evict);
        buffer.push_back(tlb[0]);
        tlb.erase(tlb.begin());
        tlb.push_back(entry);

        if(strcmp(mode, "debug") == 0) cout << "VMS miss! Page added to primary by eviction from secondary\n";
    }

    if(found_tlb == -1 && found_buffer != -1){ //found at second
        buffer[found_buffer]->time = entry->time;
        buffer[found_buffer]->op = entry->op;
        if(entry->dirty_bit == 1){
            buffer[found_buffer]->dirty_bit = 1;
        }
        if(tlb.size() < tlb_size){ //sending it at primary buffer if it has space already
            tlb.push_back(buffer[found_buffer]);
            buffer.erase(buffer.begin() + found_buffer);
        }
        if(tlb.size() == tlb_size){// sending it at primary buffer but making space there first
            page_entry* tmp = buffer[found_buffer];
            buffer.erase(buffer.begin() + found_buffer);
            buffer.push_back(tlb[0]);
            tlb.erase(tlb.begin());
            tlb.push_back(tmp);
        }
        if(strcmp(mode, "debug") == 0) cout << "VMS secondary buffer hit! Entry placed in head of the primary!\n";
    }
}

/*************************** MAIN *******************************************************************************************************/

int main(int argc, char* argv[]){
    char* tracefile = argv[1];
    int frames = atoi(argv[2]);
    char* algo = argv[3];
    char* mode;
    int parameter = 0;
    if(argc == 5){
        mode = argv[4];
    }
    else if(argc == 6){
        parameter = atoi(argv[4]);
        mode = argv[5];
    }

    FILE *read = fopen(tracefile, "r");
    if (read == NULL){
        cout << "Trace file cannot be opened.\n";
        return -1;
    }

    if (frames <= 0) {
        cout << "Ilegal number of frames. Number should be greater than 0.\n";
        return -1;
    }

    if(!(strcmp(algo, "fifo") == 0 || strcmp(algo, "lru") == 0 || strcmp(algo, "vms") == 0)){
        cout << "Incorrect algorythm. Entry should be either \"fifo\", or \"lru\", or \"vms\".\n";
        return -1;
    }
    
    if(strcmp(algo, "vms") == 0 && (parameter < 0 || parameter > 100)){
    	cout << "Parameter for \"vms\" should be in the range 1 <= p <= 100 \n";
    	return -1;
    }
    
    if(!(strcmp(mode, "debug") == 0 || strcmp(mode, "quiet") == 0)){
        cout << "Incorrect mode. Entry should be either \"debug\", or \"quiet\".\n";
        return -1;
    }
    
    unsigned address; char operation;
    while(fscanf(read, "%x %c", &address, &operation) != EOF){
       total_events++;

       page_entry* page = new page_entry;
       page->v_addr = address; page->op = operation;
       page->time = total_events;
       if(operation == 'R') page->dirty_bit = 0;
       else if(operation == 'W') page->dirty_bit = 1;

       page->page_num = address / 4096;

       if(strcmp(mode, "debug") == 0){
           cout << endl << "Virtual address: " << address << endl
                << "Operation: " << operation << endl
                << "Page number: " << page->page_num << endl;
       }

       if(strcmp(algo, "fifo") == 0){
            fifo(page, frames, mode);
        }
        else if(strcmp(algo, "lru") == 0){
            lru(page, frames, mode);
        }
        else if(strcmp(algo, "vms") == 0){
            int size_buffer = frames * (parameter/100.0); //calculating the size of secondary buffer
            if(size_buffer == 0) fifo(page, frames, mode); //if size = 0  => p = 0%; call fifo()
            else if(size_buffer == frames) lru(page, frames, mode); //if size = num_frames  => p = 100%; call lru()
            else segmented_fifo(page, frames, size_buffer, mode); //else call vms()
        }
    }

    if(strcmp(algo, "vms") == 0){cout << "vms with parameter: " << parameter << "\n";}
    cout << "total memory frames: " << frames << "\n"
        << "events in trace: " << total_events << "\n"
        << "total disk reads: " << reads << "\n"
        << "total disk writes: " << writes << "\n";
	
    return 0;
}