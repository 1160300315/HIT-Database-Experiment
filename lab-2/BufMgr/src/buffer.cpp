#include <memory>
#include <iostream>
#include "buffer.h"
#include "exceptions/buffer_exceeded_exception.h"
#include "exceptions/page_not_pinned_exception.h"
#include "exceptions/page_pinned_exception.h"
#include "exceptions/bad_buffer_exception.h"
#include "exceptions/hash_not_found_exception.h"


namespace badgerdb {

BufMgr::BufMgr(std::uint32_t bufs)
	: numBufs(bufs) {
    bufDescTable = new BufDesc[bufs]; // allocating heap mem. for bufDescTable

    // initialize necessary frames number
    for (FrameId i = 0; i < bufs; i++)
    {
        bufDescTable[i].frameNo = i;
        bufDescTable[i].valid = false;
    }

    bufPool = new Page[bufs]; // allocating spaces for page

    // determining the value for hashtable
    int htsize = ((((int) (bufs * 1.2))*2)/2)+1;
    hashTable = new BufHashTbl (htsize);  // allocate the buffer hash table
    clockHand = bufs - 1; // point the the last element
}


BufMgr::~BufMgr() {
    for(FrameId i = 0; i < numBufs; i++) {
        if(bufDescTable[i].valid == true && bufDescTable[i].dirty == true) {
            flushFile(bufDescTable[i].file);
        }
    }

    // delete malloc memory. 
    delete[] bufDescTable;
    delete hashTable;
    delete[] bufPool;
}

void BufMgr::advanceClock() {
    clockHand = (clockHand + 1) % numBufs;
}

void BufMgr::allocBuf(FrameId & frame) {
    unsigned int counter = 0;

    // clock algorithm implementation.
    do {
        if(bufDescTable[clockHand].valid == true) {
            if(bufDescTable[clockHand].refbit == true) {
                bufDescTable[clockHand].refbit = false;
                advanceClock();
            } else if(bufDescTable[clockHand].pinCnt != 0) {
                counter++;
                advanceClock();
            } else if(bufDescTable[clockHand].pinCnt == 0 && bufDescTable[clockHand].dirty == true) {
                flushFile(bufDescTable[clockHand].file);
                frame = clockHand;
                return;
            }
            else if(bufDescTable[clockHand].pinCnt == 0 && bufDescTable[clockHand].dirty == false) {
                hashTable->remove(bufDescTable[clockHand].file, bufDescTable[clockHand].pageNo);
                frame = clockHand;
                return;
            }
        } else if(bufDescTable[clockHand].valid == false) {
            frame = clockHand;
            return;
        }
    } while(counter <= numBufs);

    // BufferExceededException is throwed if all pages in
    // buffer are pinned, so no page is available.
    throw BufferExceededException();

}


void BufMgr::readPage(File* file, const PageId pageNo, Page*& page) {
    FrameId frame_id;    
    
    try {
        hashTable->lookup(file, pageNo, frame_id);
        bufDescTable[frame_id].refbit = true;
        bufDescTable[frame_id].pinCnt++;
        page = &bufPool[frame_id];
    }
    catch(HashNotFoundException e) {
        FrameId new_frame;
        allocBuf(new_frame);

        bufPool[new_frame] = file->readPage(pageNo);
        hashTable->insert(file, pageNo, new_frame);
        bufDescTable[new_frame].Set(file, pageNo);
        page = &bufPool[new_frame];
    }
}


void BufMgr::unPinPage(File* file, const PageId pageNo, const bool dirty) {
    FrameId frame_num;
    
    try {
        hashTable->lookup(file, pageNo, frame_num);

        if(bufDescTable[frame_num].pinCnt > 0) {
            bufDescTable[frame_num].pinCnt -= 1;
            if(dirty == true) {
                bufDescTable[frame_num].dirty = true;
            }
            return;
        }
        
        else if(bufDescTable[frame_num].pinCnt == 0){
            throw PageNotPinnedException(\
            	file->filename(), bufDescTable[frame_num].pageNo, frame_num\
           	);
        }
    }
    catch(HashNotFoundException e){
    }
}


void BufMgr::flushFile(const File* file) {
    for(unsigned int i = 0; i < numBufs; i++) {
        if(bufDescTable[i].valid == false && bufDescTable[i].file == file) {
            throw BadBufferException(bufDescTable[i].frameNo, bufDescTable[i].dirty, bufDescTable[i].valid, bufDescTable[i].refbit);
        }

        if(bufDescTable[i].file == file && bufDescTable[i].pinCnt > 0) {
            throw PagePinnedException(file->filename(), bufDescTable[i].pageNo, bufDescTable[i].frameNo);
        }

        if(bufDescTable[i].valid == true && bufDescTable[i].file == file) {
            if(bufDescTable[i].dirty == true) {
                bufDescTable[i].file->writePage(bufPool[i]);
                bufDescTable[i].dirty = false;
            }
            hashTable->remove(bufDescTable[i].file, bufDescTable[i].pageNo);
            bufDescTable[i].Clear();
        }
    }
}


void BufMgr::allocPage(File* file, PageId &pageNo, Page*& page) {
    FrameId frame_id;
    allocBuf(frame_id);
    bufPool[frame_id] = file->allocatePage();
    page = &bufPool[frame_id];
    pageNo = page->page_number();
    hashTable->insert(file, pageNo, frame_id);
    bufDescTable[frame_id].Set(file, pageNo);
}


void BufMgr::disposePage(File* file, const PageId PageNo)
{
    FrameId frame_no;
    try {
        hashTable->lookup(file, PageNo, frame_no);
        if(bufDescTable[frame_no].pinCnt != 0) {
                throw PagePinnedException(\
                	bufDescTable[frame_no].file->filename(), \
                	bufDescTable[frame_no].pageNo, \
                	bufDescTable[frame_no].frameNo \
                );
        }
            bufDescTable[frame_no].Clear();
            hashTable->remove(file, PageNo);
            file->deletePage(PageNo);
            return;
        }
    catch(HashNotFoundException e) {
        file->deletePage(PageNo);
    }
}

// printing the usage of buffer pool frame
void BufMgr::printSelf(void)
{
    BufDesc* tmpbuf;
    int validFrames = 0;

    for (std::uint32_t i = 0; i < numBufs; i++)
    {
        tmpbuf = &(bufDescTable[i]);
        std::cout << "FrameNo:" << i << " ";
        tmpbuf->Print();

        if (tmpbuf->valid == true)
            validFrames++;
    }

    std::cout << "Total Number of Valid Frames:" << validFrames << "\n";
}

}
